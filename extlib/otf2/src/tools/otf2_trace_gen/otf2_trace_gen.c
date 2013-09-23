/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2012,
 *    RWTH Aachen University, Germany
 *    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *    Technische Universitaet Dresden, Germany
 *    University of Oregon, Eugene, USA
 *    Forschungszentrum Juelich GmbH, Germany
 *    German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *    Technische Universitaet Muenchen, Germany
 *
 * See the COPYING file in the package base directory for details.
 *
 */

/** @internal
 *
 *  @file       otf2_trace_gen.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This tool generates a basic OTF2 archive to test the reader
 *              components and tools.
 */

#include <config.h>

#include <otf2/otf2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>



/* ___ Global variables. ____________________________________________________ */



/** @internal
 *  @brief Defines the number of locations to generate. */
static uint64_t otf2_LOCATIONS = 4;

/** @internal
 *  @brief Defines the number of events to generate per location. */
static uint64_t otf2_EVENTS = 1;

/** @internal
 *  @brief Defines if to generate MPI-only traces. This default if nothing is
 * specified in command line parameters. */
static bool otf2_MPI = true;

/** @internal
 *  @brief Defines if to generate OpenMP-only traces. */
static bool otf2_OPENMP;

/** @internal
 *  @brief Defines if to generate hybrid (MPI and OpenMP) traces. */
static bool otf2_HYBRID;

/** @internal
 *  @brief Defines if to generate additional attributes to events. */
static bool otf2_ATTRIBUTES;


/* ___ Prototypes for static functions. _____________________________________ */



static inline void
otf2_get_parameters
(
    int    argc,
    char** argv
);

static inline void
check_pointer
(
    void* pointer,
    char* description
);

static inline void
check_status
(
    OTF2_ErrorCode status,
    char*          description
);

static OTF2_FlushType
pre_flush
(
    void*         userData,
    OTF2_FileType fileType,
    uint64_t      locationId,
    void*         callerData,
    bool          final
);

static OTF2_TimeStamp
post_flush
(
    void*         userData,
    OTF2_FileType fileType,
    uint64_t      locationId
);

static OTF2_FlushCallbacks flush_callbacks =
{
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
};

static uint64_t
get_time
(
    void
);

/* empty string definition */
enum
{
    STRING_EMPTY
};

/* definition IDs for regions */
enum
{
    REGION_MAIN,
    REGION_OPENMP_PARALLEL,
    REGION_OPENMP_LOOP,
    REGION_OPENMP_IMPLICIT_BARRIER,
    REGION_MPI_SEND,
    REGION_MPI_RECV,
    REGION_MPI_BARRIER
};


/* definition IDs for Groups */
enum
{
    GROUP_MPI_LOCATIONS,
    GROUP_MPI_COMM_WORLD,
    GROUP_MPI_COMM_SELF,
    GROUP_ALL_LOCATIONS
};

/* definition IDs for MPI comms */
enum
{
    MPI_COMM_MPI_COMM_WORLD,
    MPI_COMM_MPI_COMM_SELF
};

/* definition IDs for metrics */
enum
{
    METRIC_A,
    METRIC_B,
    METRIC_C,
    METRIC_D,
    METRIC_E,

    NUM_OF_METRICS
};

/* definition IDs for metric classes and instances */
enum
{
    METRIC_CLASS_1,
    METRIC_CLASS_2,
    METRIC_CLASS_3,
    METRIC_CLASS_4,
    METRIC_INSTANCE_1,

    NUM_OF_CLASSES
};

static OTF2_AttributeValue
next_value_attr_1( void )
{
    static OTF2_AttributeValue value = { .uint64 = 12345 };
    value.uint64++;
    return value;
}

static OTF2_AttributeValue
next_value_attr_2( void )
{
    static OTF2_AttributeValue value = { .int16 = 23456 };
    value.int16++;
    return value;
}

/* attributes */
struct attribute_def
{
    char*               name;
    OTF2_Type           type;
    OTF2_AttributeValue ( * next_value )( void );
} attribute_defs[] = {
    {
        .name       = "attribute 1",
        .type       = OTF2_TYPE_UINT64,
        .next_value = next_value_attr_1
    },
    {
        .name       = "attribute 2",
        .type       = OTF2_TYPE_INT16,
        .next_value = next_value_attr_2
    }
};
#define number_of_attribute_defs \
    ( sizeof( attribute_defs ) / sizeof( attribute_defs[ 0 ] ) )

#define ADD_ATTRIBUTES() \
    do \
    { \
        if ( otf2_ATTRIBUTES ) \
        { \
            for ( size_t a = 0; a < number_of_attribute_defs; a++ ) \
            { \
                status = OTF2_AttributeList_AddAttribute( attributes, \
                                                          a, \
                                                          attribute_defs[ a ].type, \
                                                          attribute_defs[ a ].next_value() ); \
                if ( status != OTF2_SUCCESS ) \
                { \
                    check_status( status, "Adding attribute" ); \
                    break; \
                } \
            } \
        } \
    } \
    while ( 0 )

static bool
record_for_all_locations( uint64_t rank, uint64_t thread )
{
    return true;
}

static bool
record_for_location_0( uint64_t rank, uint64_t thread )
{
    return rank == 0 && thread == 0;
}

/* ___ main _________________________________________________________________ */



/** Main function
 *
 *  This tool generates a basic unified OTF2 archive to test the reader
 *  components and tools.
 *
 *  @return                 Returns EXIT_SUCCESS if successful, EXIT_FAILURE
 *                          if an error occures.
 */
int
main
(
    int    argc,
    char** argv
)
{
    otf2_get_parameters( argc, argv );

    char  archive_path[ 128 ];
    char* archive_name = "TestTrace";

    /* Array used to store metric values */
    OTF2_MetricValue metric_values[ NUM_OF_METRICS ];

    /* If you add new metrics please adapt also the array of metric types
     * and the routine to generate random metric values. */

    /* Array of metric types */
    OTF2_Type metric_types[ NUM_OF_METRICS ] = { OTF2_TYPE_UINT64, OTF2_TYPE_DOUBLE, OTF2_TYPE_INT64, OTF2_TYPE_DOUBLE, OTF2_TYPE_INT64 };

    /* Initialize accumulated metric */
    metric_values[ 0 ].unsigned_int   = 0;
    metric_values[ 1 ].floating_point = 12.5;
    metric_values[ 2 ].signed_int     = 1;
    metric_values[ 3 ].floating_point = 10.0;
    metric_values[ 4 ].unsigned_int   = 1;

    /* Distribute metric members to its classes */
    uint32_t metric_members_of_class_1[ 2 ] = { METRIC_A, METRIC_B };
    uint32_t metric_members_of_class_2[ 1 ] = { METRIC_C };
    uint32_t metric_members_of_class_3[ 1 ] = { METRIC_D };
    uint32_t metric_members_of_class_4[ 1 ] = { METRIC_E };
    uint8_t  number_of_members_in_class_1   = sizeof( metric_members_of_class_1 ) / sizeof( metric_members_of_class_1[ 0 ] );
    uint8_t  number_of_members_in_class_2   = sizeof( metric_members_of_class_2 ) / sizeof( metric_members_of_class_2[ 0 ] );
    uint8_t  number_of_members_in_class_3   = sizeof( metric_members_of_class_3 ) / sizeof( metric_members_of_class_3[ 0 ] );
    uint8_t  number_of_members_in_class_4   = sizeof( metric_members_of_class_4 ) / sizeof( metric_members_of_class_4[ 0 ] );

    /* Array of offsets needed to get correct type or values of a metric in corresponding arrays. */
    uint8_t metric_classes_offsets[ NUM_OF_CLASSES ] = { 0, 2, 3, 4, 4 };

    /* The collection of metric classes of instances for which we write metric events */
    struct metric_write_parameter
    {
        uint64_t          id;
        uint8_t           number_of_members;
        OTF2_Type*        metric_types;
        OTF2_MetricValue* metric_values;
        bool              ( * recordForLocation )( uint64_t rank,
                                                   uint64_t thread );
    } metric_write_parameters[] =
    {
        {
            .id                = METRIC_CLASS_1,
            .number_of_members = number_of_members_in_class_1,
            .metric_types      = &metric_types[ metric_classes_offsets[ 0 ] ],
            .metric_values     = &metric_values[ metric_classes_offsets[ 0 ] ],
            .recordForLocation = record_for_all_locations
        },
        {
            .id                = METRIC_CLASS_2,
            .number_of_members = number_of_members_in_class_2,
            .metric_types      = &metric_types[ metric_classes_offsets[ 1 ] ],
            .metric_values     = &metric_values[ metric_classes_offsets[ 1 ] ],
            .recordForLocation = record_for_all_locations
        },
        {
            .id                = METRIC_CLASS_3,
            .number_of_members = number_of_members_in_class_3,
            .metric_types      = &metric_types[ metric_classes_offsets[ 2 ] ],
            .metric_values     = &metric_values[ metric_classes_offsets[ 2 ] ],
            .recordForLocation = record_for_all_locations
        },
        {
            .id                = METRIC_INSTANCE_1,
            .number_of_members = number_of_members_in_class_4,
            .metric_types      = &metric_types[ metric_classes_offsets[ 3 ] ],
            .metric_values     = &metric_values[ metric_classes_offsets[ 3 ] ],
            .recordForLocation = record_for_location_0
        }
    };
    #define number_of_metric_writes ( sizeof( metric_write_parameters ) / \
                                      sizeof( metric_write_parameters[ 0 ] ) )

#define CREATE_NEW_METRIC_VALUES() \
    do \
    { \
        metric_values[ 0 ].unsigned_int++; \
        metric_values[ 1 ].floating_point += 12.5; \
        metric_values[ 2 ].unsigned_int    = ( -1 ) * metric_values[ 2 ].unsigned_int;  \
        metric_values[ 3 ].floating_point += 10.0; \
        metric_values[ 4 ].unsigned_int++; \
    } \
    while ( 0 )

#define WRITE_METRICS( new, t, rank, thread ) \
    do \
    { \
        if ( new ) \
        { \
            CREATE_NEW_METRIC_VALUES(); \
        } \
        for ( uint32_t m = 0; m < number_of_metric_writes; ++m ) \
        { \
            if ( metric_write_parameters[ m ].recordForLocation( rank, thread ) ) \
            { \
                status = OTF2_EvtWriter_Metric( evt_writer, \
                                                NULL, \
                                                t, \
                                                metric_write_parameters[ m ].id, \
                                                metric_write_parameters[ m ].number_of_members, \
                                                metric_write_parameters[ m ].metric_types, \
                                                metric_write_parameters[ m ].metric_values ); \
                check_status( status, "Write metrics." ); \
            } \
        } \
    } \
    while ( 0 )

    /* Calculate number of MPI ranks and OpenMP threads. */
    uint64_t num_ranks   = 1;
    uint64_t num_threads = 1;

    if ( otf2_MPI )
    {
        num_ranks = otf2_LOCATIONS;
        snprintf( archive_path, sizeof( archive_path ),
                  "otf2_trace_gen_trace_mpi_%" PRIu64 "_%" PRIu64 "%s",
                  num_ranks, otf2_EVENTS,
                  otf2_ATTRIBUTES ? "_attr" : "" );
    }
    else if ( otf2_OPENMP )
    {
        num_threads = otf2_LOCATIONS;
        snprintf( archive_path, sizeof( archive_path ),
                  "otf2_trace_gen_trace_omp_%" PRIu64 "_%" PRIu64 "%s",
                  num_threads, otf2_EVENTS,
                  otf2_ATTRIBUTES ? "_attr" : "" );
    }
    else if ( otf2_HYBRID )
    {
        if ( ( otf2_LOCATIONS % 4 ) == 0 )
        {
            num_threads = 4;
        }
        else if ( ( otf2_LOCATIONS % 3 ) == 0 )
        {
            num_threads = 3;
        }
        else if ( ( otf2_LOCATIONS % 2 ) == 0 )
        {
            num_threads = 2;
        }
        else
        {
            for ( uint64_t num_threads = 5; num_threads < otf2_LOCATIONS; num_threads++ )
            {
                if ( ( otf2_LOCATIONS % num_threads ) == 0 )
                {
                    break;
                }
            }
        }
        num_ranks = ( uint64_t )( otf2_LOCATIONS / num_threads );
        snprintf( archive_path, sizeof( archive_path ),
                  "otf2_trace_gen_trace_mpi_omp_%" PRIu64 "x%" PRIu64 "_%" PRIu64 "%s",
                  num_ranks, num_threads, otf2_EVENTS,
                  otf2_ATTRIBUTES ? "_attr" : "" );
    }

    /* Print initial information. */
    printf( "\nGenerating an OTF2 test archive\n" );
    printf( "with %" PRIu64 " rank(s) and %" PRIu64 " thread(s) "
            "each (%" PRIu64 " locations total)\n",
            num_ranks, num_threads, otf2_LOCATIONS );
    printf( "to %s/%s.otf2\n\n", archive_path, archive_name );


    /* Create new archive handle. */
    OTF2_Archive* archive = OTF2_Archive_Open( archive_path,
                                               archive_name,
                                               OTF2_FILEMODE_WRITE,
                                               1024 * 1024,
                                               4 * 1024 * 1024,
                                               OTF2_SUBSTRATE_POSIX,
                                               OTF2_COMPRESSION_NONE );
    check_pointer( archive, "Create archive" );

    OTF2_ErrorCode status;
    status = OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
    check_status( status, "Set flush callbacks." );

    /* Set master slave mode, description, and creator. */
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set master slave mode." );
    status = OTF2_Archive_SetDescription( archive, "Simple OTF2 trace archive to valid reader components" );
    check_status( status, "Set description." );
    status = OTF2_Archive_SetCreator( archive, "otf2_trace_gen" );
    check_status( status, "Set creator." );


    /* Generate location IDs. Just to have non-consecutive location IDs. */
    uint64_t locations[ otf2_LOCATIONS ];
    uint64_t mpi_ranks[ num_ranks ];
    uint64_t master_threads[ num_ranks ];

    for ( uint64_t rank = 0; rank < num_ranks; rank++ )
    {
        for ( uint64_t thread = 0; thread < num_threads; thread++ )
        {
            locations[ num_threads * rank + thread ] = ( rank << 16 ) + thread;
        }
        mpi_ranks[ rank ]      = rank;
        master_threads[ rank ] = rank << 16;
    }

    /* Create local event writer. */
    OTF2_EvtWriter* evt_writer = NULL;

    /* Create an attribute list. */
    OTF2_AttributeList* attributes = NULL;
    if ( otf2_ATTRIBUTES )
    {
        attributes = OTF2_AttributeList_New();
        check_pointer( attributes, "Get attributes handle." );
    }


    uint64_t t = get_time();
    for ( uint64_t rank = 0; rank < num_ranks; ++rank )
    {
        evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
        check_pointer( evt_writer, "Get event writer." );

        /* Enter main. */
        WRITE_METRICS( true, t, rank, 0 );
        ADD_ATTRIBUTES();
        status = OTF2_EvtWriter_Enter( evt_writer, attributes, t, REGION_MAIN );
        check_status( status, "Write Enter." );
    }

    for ( uint64_t event = 0; event < otf2_EVENTS; ++event )
    {
        t = get_time();
        for ( uint64_t rank = 0; !otf2_OPENMP && rank < num_ranks; ++rank )
        {
            evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
            check_pointer( evt_writer, "Get event writer." );

            /* Write enter MPI_Send. */
            WRITE_METRICS( true, t, rank, 0 );
            ADD_ATTRIBUTES();
            status = OTF2_EvtWriter_Enter( evt_writer, attributes, t, REGION_MPI_SEND );
            check_status( status, "Write Enter." );

            status = OTF2_EvtWriter_MpiSend(
                evt_writer,
                NULL,
                t,
                ( rank + 1 ) % num_ranks, /* receiver rank */
                rank,                     /* each rank has its local communicator */
                0,
                1024 );
            check_status( status, "Write MPI_Send." );

            t = get_time();

            /* Write leave MPI_Send. */
            WRITE_METRICS( true, t, rank, 0 );
            ADD_ATTRIBUTES();
            status = OTF2_EvtWriter_Leave( evt_writer, attributes, t, REGION_MPI_SEND );
            check_status( status, "Write Leave." );

            /* Immediately enter the MPI_Recv */
            t = get_time();

            /* Write enter MPI_Recv. */
            WRITE_METRICS( true, t, rank, 0 );
            ADD_ATTRIBUTES();
            status = OTF2_EvtWriter_Enter( evt_writer, attributes, t, REGION_MPI_RECV );
            check_status( status, "Write Enter." );
        }

        for ( uint64_t rank = 0; !otf2_OPENMP && rank < num_ranks; ++rank )
        {
            evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
            check_pointer( evt_writer, "Get event writer." );

            status = OTF2_EvtWriter_MpiRecv(
                evt_writer, NULL, t,
                ( rank + num_ranks - 1 ) % num_ranks, /* sender rank */
                rank,                                 /* each rank has its local communicator */
                0,
                1024 );
            check_status( status, "Write MPI_Recv." );

            /* Write enter MPI_Recv. */
            WRITE_METRICS( true, t, rank, 0 );
            ADD_ATTRIBUTES();
            status = OTF2_EvtWriter_Leave( evt_writer, attributes, t, REGION_MPI_RECV );
            check_status( status, "Write Leave." );

            /* MPI Barrier. */
            t = get_time();

            WRITE_METRICS( true, t, rank, 0 );
            ADD_ATTRIBUTES();
            status = OTF2_EvtWriter_Enter( evt_writer, attributes, t, REGION_MPI_BARRIER );
            check_status( status, "Write Enter." );

            status = OTF2_EvtWriter_MpiCollectiveBegin(
                evt_writer,
                NULL,
                t );
            check_status( status, "Write MpiCollectiveBegin." );
        }

        /* End all started MPI_Barrier */
        t = get_time();
        for ( uint64_t rank = 0; !otf2_OPENMP && rank < num_ranks; ++rank )
        {
            evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
            check_pointer( evt_writer, "Get event writer." );

            status = OTF2_EvtWriter_MpiCollectiveEnd(
                evt_writer,
                NULL,
                t,
                OTF2_COLLECTIVE_OP_BARRIER,
                rank,                  /* each rank has its local communicator */
                OTF2_UNDEFINED_UINT32, /* MPI_Barrier has no root */
                0,
                0 );
            check_status( status, "Write MpiCollectiveEnd." );

            WRITE_METRICS( true, t, rank, 0 );
            ADD_ATTRIBUTES();
            status = OTF2_EvtWriter_Leave( evt_writer, attributes, t, REGION_MPI_BARRIER );
            check_status( status, "Write Leave." );
        }

        /* If in OpenMP or Hybrid mode. */
        if ( !otf2_MPI )
        {
            uint64_t t_enter = get_time();
            uint64_t t_leave = get_time();

            for ( uint64_t rank = 0; rank < num_ranks; ++rank )
            {
                evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
                /* At this point the master creates a team of several parallel threads */
                status = OTF2_EvtWriter_OmpFork( evt_writer,
                                                 attributes,
                                                 t_enter,
                                                 num_threads );
            }

            for ( uint64_t rank = 0; rank < num_ranks; ++rank )
            {
                for ( uint64_t thread = 0; thread < num_threads; ++thread )
                {
                    evt_writer = OTF2_Archive_GetEvtWriter( archive, locations[ num_threads * rank + thread ] );
                    check_pointer( evt_writer, "Get event writer." );

                    /* Enter OpenMP parallel  loop (outer region). */
                    WRITE_METRICS( true, t_enter, rank, thread );
                    ADD_ATTRIBUTES();
                    status = OTF2_EvtWriter_Enter( evt_writer, attributes, t_enter, REGION_OPENMP_PARALLEL );
                    check_status( status, "Write Enter." );

                    /* Enter OpenMP parallel loop  (inner region). */
                    WRITE_METRICS( false, t_enter, rank, thread );
                    ADD_ATTRIBUTES();
                    status = OTF2_EvtWriter_Enter( evt_writer, attributes, t_enter, REGION_OPENMP_LOOP );
                    check_status( status, "Write Enter." );

                    /* Enter OpenMP implicit barrier. */
                    WRITE_METRICS( false, t_enter, rank, thread );
                    ADD_ATTRIBUTES();
                    status = OTF2_EvtWriter_Enter( evt_writer, attributes, t_enter, REGION_OPENMP_IMPLICIT_BARRIER );
                    check_status( status, "Write Enter." );
                }
            }


            for ( uint64_t rank = 0; rank < num_ranks; ++rank )
            {
                for ( uint64_t thread = 0; thread < num_threads; ++thread )
                {
                    evt_writer = OTF2_Archive_GetEvtWriter( archive, locations[ num_threads * rank + thread ] );
                    check_pointer( evt_writer, "Get event writer." );

                    /* Leave OpenMP implicit barrier. */
                    WRITE_METRICS( true, t_leave, rank, thread );
                    ADD_ATTRIBUTES();
                    status = OTF2_EvtWriter_Leave( evt_writer, attributes, t_leave, REGION_OPENMP_IMPLICIT_BARRIER );
                    check_status( status, "Write Enter." );

                    /* Leave OpenMP parallel loop (inner region). */
                    WRITE_METRICS( false, t_leave, rank, thread );
                    ADD_ATTRIBUTES();
                    status = OTF2_EvtWriter_Leave( evt_writer, attributes, t_leave, REGION_OPENMP_LOOP );
                    check_status( status, "Write Leave." );

                    /* Leave OpenMP parallel loop (outer region). */
                    WRITE_METRICS( false, t_leave, rank, thread );
                    ADD_ATTRIBUTES();
                    status = OTF2_EvtWriter_Leave( evt_writer, attributes, t_leave, REGION_OPENMP_PARALLEL );
                    check_status( status, "Write Leave." );
                }
            }

            for ( uint64_t rank = 0; rank < num_ranks; ++rank )
            {
                evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
                /* All threads have finished execution of statements in parallel region.
                 * Now they synchronize and terminates. Only the master continues execution. */
                t_leave = get_time();
                status  = OTF2_EvtWriter_OmpJoin( evt_writer,
                                                  attributes,
                                                  t_leave );
            }
        }

        for ( uint64_t rank = 0; !otf2_OPENMP && rank < num_ranks; ++rank )
        {
            evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
            check_pointer( evt_writer, "Get event writer." );

            t = get_time();

            WRITE_METRICS( true, t, rank, 0 );
            ADD_ATTRIBUTES();
            status = OTF2_EvtWriter_Enter( evt_writer, attributes, t, REGION_MPI_BARRIER );
            check_status( status, "Write Enter." );

            status = OTF2_EvtWriter_MpiCollectiveBegin(
                evt_writer,
                NULL,
                t );
            check_status( status, "Write MpiCollectiveBegin." );
        }

        /* End all started MPI_Barrier */
        t = get_time();
        for ( uint64_t rank = 0; !otf2_OPENMP && rank < num_ranks; ++rank )
        {
            evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
            check_pointer( evt_writer, "Get event writer." );

            status = OTF2_EvtWriter_MpiCollectiveEnd(
                evt_writer,
                NULL,
                t,
                OTF2_COLLECTIVE_OP_BARRIER,
                rank,                  /* each rank has its local communicator */
                OTF2_UNDEFINED_UINT32, /* MPI_Barrier has no root */
                0,
                0 );
            check_status( status, "Write MpiCollectiveEnd." );

            WRITE_METRICS( true, t, rank, 0 );
            ADD_ATTRIBUTES();
            status = OTF2_EvtWriter_Leave( evt_writer, attributes, t, REGION_MPI_BARRIER );
            check_status( status, "Write Leave." );
        }
    }

    t = get_time();
    for ( uint64_t rank = 0; rank < num_ranks; ++rank )
    {
        evt_writer = OTF2_Archive_GetEvtWriter( archive, master_threads[ rank ] );
        check_pointer( evt_writer, "Get event writer." );

        /* Leave main. */
        WRITE_METRICS( true, t, rank, 0 );
        ADD_ATTRIBUTES();
        status = OTF2_EvtWriter_Leave( evt_writer, attributes, t, REGION_MAIN );
        check_status( status, "Write Leave." );
    }

    /* last timestamp */
    t = get_time();

    /* Write the global and local definitions. */
    OTF2_GlobalDefWriter* glob_def_writer = NULL;
    glob_def_writer = OTF2_Archive_GetGlobalDefWriter( archive );
    check_pointer( glob_def_writer, "Get global definition writer" );

    /* Write the timerange of the trace */
    status = OTF2_GlobalDefWriter_WriteClockProperties( glob_def_writer,
                                                        1, 0, t );
    check_status( status, "Write time range definition." );

    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               STRING_EMPTY,
                                               "" );
    check_status( status, "Write string definition." );
    uint32_t string = STRING_EMPTY + 1;

    /* Write system tree definitions. */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer, string, "Rack 0" );
    check_status( status, "Write string definition." );
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer, string + 1, "Rack" );
    check_status( status, "Write string definition." );

    status = OTF2_GlobalDefWriter_WriteSystemTreeNode( glob_def_writer, 0,
                                                       string, string + 1,
                                                       OTF2_UNDEFINED_SYSTEM_TREE_NODE );
    check_status( status, "Write system tree node definition." );
    string += 2;

    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer, string, "Blade" );
    check_status( status, "Write string definition." );
    uint32_t string_blade = string;
    string++;

    for ( uint64_t rank = 0; rank < ( uint64_t )( num_ranks / 2 ); rank++ )
    {
        char name_buffer[ 64 ];
        sprintf( name_buffer, "Blade #%" PRIu64, rank );

        status = OTF2_GlobalDefWriter_WriteString( glob_def_writer, string, name_buffer );
        check_status( status, "Write string definition." );

        status = OTF2_GlobalDefWriter_WriteSystemTreeNode( glob_def_writer, rank + 1,
                                                           string, string_blade,
                                                           0 );
        check_status( status, "Write system tree node definition." );
        string++;
    }

    /* Write location group and location definitions. */
    for ( uint64_t rank = 0; rank < num_ranks; ++rank )
    {
        char name_buffer[ 64 ];

        sprintf( name_buffer, "Process %" PRIu64, rank );
        status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                                   string,
                                                   name_buffer );
        check_status( status, "Write string definition." );

        status = OTF2_GlobalDefWriter_WriteLocationGroup( glob_def_writer,
                                                          rank,
                                                          string,
                                                          OTF2_LOCATION_GROUP_TYPE_PROCESS,
                                                          ( rank / 2 ) + 1 );
        check_status( status, "Write location group definition." );
        string++;


        for ( uint64_t thread = 0; thread < num_threads; thread++ )
        {
            sprintf( name_buffer, "Thread %" PRIu64 ".%" PRIu64, rank, thread );
            status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                                       string,
                                                       name_buffer );
            check_status( status, "Write string definition." );

            evt_writer = OTF2_Archive_GetEvtWriter( archive, locations[ num_threads * rank + thread ] );
            check_pointer( evt_writer, "Get event writer." );

            uint64_t num_events = 0;
            status = OTF2_EvtWriter_GetNumberOfEvents( evt_writer, &num_events );
            check_status( status, "Get the numberof written events." );

            status = OTF2_GlobalDefWriter_WriteLocation( glob_def_writer,
                                                         locations[ rank * num_threads + thread ],
                                                         string,
                                                         OTF2_LOCATION_TYPE_CPU_THREAD,
                                                         num_events, rank );
            check_status( status, "Write location definition." );
            string++;
        }
    }

    /* Write region definition for "main". */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "main (mangled name)" );
    string++;
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "main (demangled name)" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteRegion( glob_def_writer,
                                               REGION_MAIN,
                                               string,
                                               string - 1,
                                               STRING_EMPTY,
                                               OTF2_REGION_ROLE_UNKNOWN,
                                               OTF2_PARADIGM_UNKNOWN,
                                               OTF2_REGION_FLAG_NONE,
                                               STRING_EMPTY,
                                               0,
                                               0 );
    check_status( status, "Write region definition" );
    string++;

    /* Write region definition for an all-threads function. */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "!$omp parallelfor" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteRegion( glob_def_writer,
                                               REGION_OPENMP_PARALLEL,
                                               string,
                                               string,
                                               STRING_EMPTY,
                                               OTF2_REGION_ROLE_UNKNOWN,
                                               OTF2_PARADIGM_UNKNOWN,
                                               OTF2_REGION_FLAG_NONE,
                                               STRING_EMPTY,
                                               0,
                                               0 );
    check_status( status, "Write region definition" );
    status = OTF2_GlobalDefWriter_WriteRegion( glob_def_writer,
                                               REGION_OPENMP_LOOP,
                                               string,
                                               string,
                                               STRING_EMPTY,
                                               OTF2_REGION_ROLE_UNKNOWN,
                                               OTF2_PARADIGM_UNKNOWN,
                                               OTF2_REGION_FLAG_NONE,
                                               STRING_EMPTY,
                                               0,
                                               0 );
    check_status( status, "Write region definition" );
    string++;

    /* Write region definition for OpenMP implicit barrier. */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "!$omp implicit barrier" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteRegion( glob_def_writer,
                                               REGION_OPENMP_IMPLICIT_BARRIER,
                                               string,
                                               string,
                                               STRING_EMPTY,
                                               OTF2_REGION_ROLE_UNKNOWN,
                                               OTF2_PARADIGM_UNKNOWN,
                                               OTF2_REGION_FLAG_NONE,
                                               STRING_EMPTY,
                                               0,
                                               0 );
    string++;

    /* Write region definition for "MPI_Send". */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "MPI_Send" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteRegion( glob_def_writer,
                                               REGION_MPI_SEND,
                                               string,
                                               string,
                                               STRING_EMPTY,
                                               OTF2_REGION_ROLE_UNKNOWN,
                                               OTF2_PARADIGM_UNKNOWN,
                                               OTF2_REGION_FLAG_NONE,
                                               STRING_EMPTY,
                                               0,
                                               0 );
    check_status( status, "Write region definition" );
    string++;

    /* Write region definition for "MPI_Recv". */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "MPI_Recv" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteRegion( glob_def_writer,
                                               REGION_MPI_RECV,
                                               string,
                                               string,
                                               STRING_EMPTY,
                                               OTF2_REGION_ROLE_UNKNOWN,
                                               OTF2_PARADIGM_UNKNOWN,
                                               OTF2_REGION_FLAG_NONE,
                                               STRING_EMPTY,
                                               0,
                                               0 );
    check_status( status, "Write region definition" );
    string++;

    /* Write region definition for "MPI_Barrier". */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "MPI_Barrier" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteRegion( glob_def_writer,
                                               REGION_MPI_BARRIER,
                                               string,
                                               string,
                                               STRING_EMPTY,
                                               OTF2_REGION_ROLE_UNKNOWN,
                                               OTF2_PARADIGM_UNKNOWN,
                                               OTF2_REGION_FLAG_NONE,
                                               STRING_EMPTY,
                                               0,
                                               0 );
    check_status( status, "Write region definition" );
    string++;

    /* Define the list of locations which are mpi ranks */
    status = OTF2_GlobalDefWriter_WriteGroup( glob_def_writer,
                                              GROUP_MPI_LOCATIONS,
                                              STRING_EMPTY,
                                              OTF2_GROUP_TYPE_COMM_LOCATIONS,
                                              OTF2_PARADIGM_MPI,
                                              OTF2_GROUP_FLAG_NONE,
                                              num_ranks,
                                              master_threads );
    check_status( status, "Write MPI_COMM_WORLD location definition." );

    /* Define the MPI group for MPI_COMM_WORLD */
    status = OTF2_GlobalDefWriter_WriteGroup( glob_def_writer,
                                              GROUP_MPI_COMM_WORLD,
                                              STRING_EMPTY,
                                              OTF2_GROUP_TYPE_COMM_GROUP,
                                              OTF2_PARADIGM_MPI,
                                              OTF2_GROUP_FLAG_NONE,
                                              num_ranks,
                                              mpi_ranks );
    check_status( status, "Write MPI Group definition." );

    /* Define the MPI group for COMM_SELF like communicators */
    status = OTF2_GlobalDefWriter_WriteGroup( glob_def_writer,
                                              GROUP_MPI_COMM_SELF,
                                              STRING_EMPTY,
                                              OTF2_GROUP_TYPE_COMM_SELF,
                                              OTF2_PARADIGM_MPI,
                                              OTF2_GROUP_FLAG_NONE,
                                              0,
                                              NULL );
    check_status( status, "Write MPI Group definition." );

    /* Define a group that includes all locations. It will be used by metric instance. */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "All locations" );
    check_status( status, "Write string definition." );
    status = OTF2_GlobalDefWriter_WriteGroup( glob_def_writer,
                                              GROUP_ALL_LOCATIONS,
                                              string,
                                              OTF2_GROUP_TYPE_LOCATIONS,
                                              OTF2_PARADIGM_UNKNOWN,
                                              OTF2_GROUP_FLAG_NONE,
                                              otf2_LOCATIONS,
                                              locations );
    check_status( status, "Write group definition." );
    string++;

    /* Define MPI_COMM_WORD */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "My MPI_COMM_WORLD communicator" );
    status = OTF2_GlobalDefWriter_WriteComm( glob_def_writer,
                                             MPI_COMM_MPI_COMM_WORLD,
                                             string,
                                             GROUP_MPI_COMM_WORLD,
                                             OTF2_UNDEFINED_COMM );
    check_status( status, "Write MPI_COMM_WORLD definition." );
    string++;

    /* Define MPI_COMM_SELF */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "My MPI_COMM_SELF communicator" );
    status = OTF2_GlobalDefWriter_WriteComm( glob_def_writer,
                                             MPI_COMM_MPI_COMM_SELF,
                                             string,
                                             GROUP_MPI_COMM_SELF,
                                             OTF2_UNDEFINED_COMM );
    check_status( status, "Write MPI_COMM_SELF definition." );
    string++;

    /* Write member definition of first metric */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "Metric_A" );
    check_status( status, "Write string definition" );
    string++;
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "FLOPS" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteMetricMember( glob_def_writer,
                                                     METRIC_A,
                                                     string - 1,
                                                     STRING_EMPTY,
                                                     OTF2_METRIC_TYPE_PAPI,
                                                     OTF2_METRIC_ACCUMULATED_START,
                                                     metric_types[ 0 ],
                                                     OTF2_BASE_DECIMAL,
                                                     0,
                                                     string );
    check_status( status, "Write metric member definition" );
    string++;

    /* Write member definition of second metric */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "Metric_B" );
    check_status( status, "Write string definition" );
    string++;
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "s" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteMetricMember( glob_def_writer,
                                                     METRIC_B,
                                                     string - 1,
                                                     STRING_EMPTY,
                                                     OTF2_METRIC_TYPE_OTHER,
                                                     OTF2_METRIC_ABSOLUTE_POINT,
                                                     metric_types[ 1 ],
                                                     OTF2_BASE_DECIMAL,
                                                     -6,
                                                     string );
    check_status( status, "Write metric member definition" );
    string++;

    /* Write member definition of third metric */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "Metric_C" );
    check_status( status, "Write string definition" );
    string++;
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "apples" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteMetricMember( glob_def_writer,
                                                     METRIC_C,
                                                     string - 1,
                                                     STRING_EMPTY,
                                                     OTF2_METRIC_TYPE_OTHER,
                                                     OTF2_METRIC_RELATIVE_LAST,
                                                     metric_types[ 2 ],
                                                     OTF2_BASE_DECIMAL,
                                                     0,
                                                     string );
    check_status( status, "Write metric member definition" );
    string++;

    /* Write member definition of fourth metric */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "Metric_D" );
    check_status( status, "Write string definition" );
    string++;
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "pears" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteMetricMember( glob_def_writer,
                                                     METRIC_D,
                                                     string - 1,
                                                     STRING_EMPTY,
                                                     OTF2_METRIC_TYPE_OTHER,
                                                     OTF2_METRIC_ABSOLUTE_POINT,
                                                     metric_types[ 3 ],
                                                     OTF2_BASE_DECIMAL,
                                                     0,
                                                     string );
    check_status( status, "Write metric member definition" );
    string++;

    /* Write member definition of fifth metric */
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "Metric_E" );
    check_status( status, "Write string definition" );
    string++;
    status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                               string,
                                               "cherries" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteMetricMember( glob_def_writer,
                                                     METRIC_E,
                                                     string - 1,
                                                     STRING_EMPTY,
                                                     OTF2_METRIC_TYPE_OTHER,
                                                     OTF2_METRIC_ABSOLUTE_POINT,
                                                     metric_types[ 4 ],
                                                     OTF2_BASE_DECIMAL,
                                                     0,
                                                     string );
    check_status( status, "Write metric member definition" );
    string++;

    /* Define metric classes and instances */
    status = OTF2_GlobalDefWriter_WriteMetricClass( glob_def_writer,
                                                    METRIC_CLASS_1,
                                                    number_of_members_in_class_1,
                                                    metric_members_of_class_1,
                                                    OTF2_METRIC_SYNCHRONOUS_STRICT,
                                                    OTF2_RECORDER_KIND_CPU );
    check_status( status, "Write metric class definition." );
    status = OTF2_GlobalDefWriter_WriteMetricClass( glob_def_writer,
                                                    METRIC_CLASS_2,
                                                    number_of_members_in_class_2,
                                                    metric_members_of_class_2,
                                                    OTF2_METRIC_SYNCHRONOUS_STRICT,
                                                    OTF2_RECORDER_KIND_CPU );
    check_status( status, "Write metric class definition." );
    status = OTF2_GlobalDefWriter_WriteMetricClass( glob_def_writer,
                                                    METRIC_CLASS_3,
                                                    number_of_members_in_class_3,
                                                    metric_members_of_class_3,
                                                    OTF2_METRIC_SYNCHRONOUS_STRICT,
                                                    OTF2_RECORDER_KIND_CPU );
    check_status( status, "Write metric class definition." );
    status = OTF2_GlobalDefWriter_WriteMetricClass( glob_def_writer,
                                                    METRIC_CLASS_4,
                                                    number_of_members_in_class_4,
                                                    metric_members_of_class_4,
                                                    OTF2_METRIC_ASYNCHRONOUS,
                                                    OTF2_RECORDER_KIND_ABSTRACT );
    check_status( status, "Write metric class definition." );

    /* We can define an instance with differing recorder and scope.
     * In this example metric values are valid for all locationa
     * but are recorded on first location. */
    status = OTF2_GlobalDefWriter_WriteMetricInstance( glob_def_writer,
                                                       METRIC_INSTANCE_1,
                                                       METRIC_CLASS_4,
                                                       locations[ 0 ],
                                                       OTF2_SCOPE_GROUP,
                                                       GROUP_ALL_LOCATIONS );
    check_status( status, "Write metric instance definition." );

    /* write attribute defs */
    if ( otf2_ATTRIBUTES )
    {
        for ( size_t a = 0; a < number_of_attribute_defs; a++ )
        {
            status = OTF2_GlobalDefWriter_WriteString( glob_def_writer,
                                                       string,
                                                       attribute_defs[ a ].name );
            check_status( status, "Write string definition" );
            status = OTF2_GlobalDefWriter_WriteAttribute( glob_def_writer,
                                                          a,
                                                          string,
                                                          attribute_defs[ a ].type );
            check_status( status, "Write ATTRIBUTE definition." );
            string++;
        }
    }

    /* write local mappings for MPI communicators and metrics */
    /* write local mappings for metrics, this is an identity map, just to write
     * something out
     */
    OTF2_IdMap* metric_map = OTF2_IdMap_Create( OTF2_ID_MAP_DENSE,
                                                NUM_OF_CLASSES );
    check_pointer( metric_map, "Create ID map for metrics." );
    for ( uint32_t c = 0; c < NUM_OF_CLASSES; c++ )
    {
        OTF2_IdMap_AddIdPair( metric_map, c, c );
    }

    for ( uint64_t rank = 0; rank < num_ranks; rank++ )
    {
        OTF2_IdMap* mpi_comm_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, 2 );
        check_pointer( mpi_comm_map, "Create ID map for MPI Comms." );

        /* Each location uses its rank as the communicator id which maps to the global 0 */
        OTF2_IdMap_AddIdPair( mpi_comm_map, rank, MPI_COMM_MPI_COMM_WORLD );
        OTF2_IdMap_AddIdPair( mpi_comm_map, rank + num_ranks, MPI_COMM_MPI_COMM_SELF );

        for ( uint64_t thread = 0; thread < num_threads; thread++ )
        {
            /* Open a definition writer, so the file is created. */
            OTF2_DefWriter* def_writer = OTF2_Archive_GetDefWriter(
                archive,
                locations[ num_threads * rank + thread ] );
            check_pointer( def_writer, "Get definition writer." );

            /* Write metric mappings to local definitions. */
            status = OTF2_DefWriter_WriteMappingTable( def_writer,
                                                       OTF2_MAPPING_METRIC,
                                                       metric_map );
            check_status( status, "Write Metric mappings." );

            if ( otf2_MPI || otf2_HYBRID )
            {
                /* Write MPI Comm mappings to local definitions. */
                status = OTF2_DefWriter_WriteMappingTable( def_writer,
                                                           OTF2_MAPPING_COMM,
                                                           mpi_comm_map );
                check_status( status, "Write MPI Comm mapping." );
            }

            /* Write clock offsets to local definitions. */
            status = OTF2_DefWriter_WriteClockOffset( def_writer,
                                                      0, 0, 0.0 );
            check_status( status, "Write start clock offset." );
            status = OTF2_DefWriter_WriteClockOffset( def_writer,
                                                      t, 0, 0.0 );
            check_status( status, "Write end clock offset." );
        }

        OTF2_IdMap_Free( mpi_comm_map );
    }
    OTF2_IdMap_Free( metric_map );

    /* Finalize. */
    status = OTF2_Archive_Close( archive );
    check_status( status, "Close archive." );
    OTF2_AttributeList_Delete( attributes );


    printf( "Done\n\n" );


    return EXIT_SUCCESS;
}



/* ___ Implementation of static functions ___________________________________ */


/** @internal
 *  @brief Parse a number from the command line.
 */
static uint64_t
otf2_parse_number_argument
(
    const char* option,
    const char* argument
)
{
    uint64_t number = 0;
    for ( uint8_t j = 0; argument[ j ]; j++ )
    {
        if ( ( argument[ j ] < '0' ) || ( argument[ j ] > '9' ) )
        {
            printf( "ERROR: Invalid number argument for %s: %s\n", option, argument );
            printf( "Try 'otf2-trace-gen --help' for information on usage.\n\n" );
            exit( EXIT_FAILURE );
        }
        uint64_t new_number = number * 10 + argument[ j ] - '0';
        if ( new_number < number )
        {
            printf( "ERROR: Number argument to large for %s: %s\n", option, argument );
            printf( "Try 'otf2-trace-gen --help' for information on usage.\n\n" );
            exit( EXIT_FAILURE );
        }
        number = new_number;
    }

    return number;
}

/** @internal
 *  @brief Get command line parameters.
 *
 *  Parses command line parameters and checks for their existence.
 *  Prints help for parameters '-h' or '--help'.
 *
 *  @param argc             Programs argument counter.
 *  @param argv             Programs argument values.
 */
static inline void
otf2_get_parameters
(
    int    argc,
    char** argv
)
{
    /* Check if there is at least one command line parameter. */
    if ( argc < 2 )
    {
        return;
    }

    /* Check for passed '--help' or '-h' and print help. */
    if ( !strcmp( argv[ 1 ], "--help" ) || !strcmp( argv[ 1 ], "-h" ) )
    {
        printf( "Usage: otf2-trace-gen [OPTION]... \n" );
        printf( "Generate a basic unified OTF2 archive.\n" );
        printf( "\n" );
        printf( "  -e, --events           Approx. number of events to generate per location.\n" );
        printf( "  -l, --locations        Number of locations to generate (Should be small).\n" );
        printf( "  -V, --version          Print version information.\n" );
        printf( "  -h, --help             Print this help information.\n" );
        printf( "      --mpi              MPI-only mode (default).\n" );
        printf( "      --openmp           OpenMP-only mode.\n" );
        printf( "      --hybrid           Hybrid mode.\n" );
        printf( "      --attributes       Add attributes to enter/leave events.\n" );
        printf( "\n" );
        printf( "Report bugs to <%s>\n", PACKAGE_BUGREPORT );
        exit( EXIT_SUCCESS );
    }

    /* Check for passed '--version' or '-V' and print version information. */
    if ( !strcmp( argv[ 1 ], "--version" ) || !strcmp( argv[ 1 ], "-V" ) )
    {
        printf( "otf2-trace-gen: version %s\n", OTF2_VERSION );
        exit( EXIT_SUCCESS );
    }

    for ( uint8_t i = 1; i < argc; i++ )
    {
        if ( !strcmp( argv[ i ], "--mpi" ) )
        {
            otf2_MPI    = true;
            otf2_OPENMP = false;
            otf2_HYBRID = false;
        }

        else if ( !strcmp( argv[ i ], "--openmp" ) )
        {
            otf2_MPI    = false;
            otf2_OPENMP = true;
            otf2_HYBRID = false;
        }

        else if ( !strcmp( argv[ i ], "--hybrid" ) )
        {
            otf2_MPI    = false;
            otf2_OPENMP = false;
            otf2_HYBRID = true;
        }

        else if ( !strcmp( argv[ i ], "--attributes" ) )
        {
            otf2_ATTRIBUTES = true;
        }

        else if ( !strcmp( argv[ i ], "--locations" ) || !strcmp( argv[ i ], "-l" ) )
        {
            if ( i + 1 == argc )
            {
                printf( "ERROR: Missing argument for %s.\n", argv[ i ] );
                printf( "Try 'otf2-trace-gen --help' for information on usage.\n\n" );
                exit( EXIT_FAILURE );
            }

            otf2_LOCATIONS = otf2_parse_number_argument( argv[ i ], argv[ i + 1 ] );
            i++;
        }

        else if ( !strcmp( argv[ i ], "--events" ) || !strcmp( argv[ i ], "-e" ) )
        {
            if ( i + 1 == argc )
            {
                printf( "ERROR: Missing argument for %s.\n", argv[ i ] );
                printf( "Try 'otf2-trace-gen --help' for information on usage.\n\n" );
                exit( EXIT_FAILURE );
            }

            otf2_EVENTS = otf2_parse_number_argument( argv[ i ], argv[ i + 1 ] );
            i++;
        }
        else
        {
            printf( "WARNING: Skipped unknown control option %s.\n", argv[ i ] );
        }
    }

    return;
}



static inline void
check_pointer
(
    void* pointer,
    char* description
)
{
    if ( pointer == NULL )
    {
        printf( "\nERROR: %s\n\n", description );
        exit( EXIT_FAILURE );
    }
}



static inline void
check_status
(
    OTF2_ErrorCode status,
    char*          description
)
{
    if ( status != OTF2_SUCCESS )
    {
        printf( "\nERROR: %s\n\n", description );
        exit( EXIT_FAILURE );
    }
}

static OTF2_FlushType
pre_flush
(
    void*         userData,
    OTF2_FileType fileType,
    uint64_t      locationId,
    void*         callerData,
    bool          final
)
{
    return OTF2_FLUSH;
}

static OTF2_TimeStamp
post_flush
(
    void*         userData,
    OTF2_FileType fileType,
    uint64_t      locationId
)
{
    return get_time();
}

static uint64_t
get_time
(
    void
)
{
    static uint64_t sequence;
    return sequence++;
}
