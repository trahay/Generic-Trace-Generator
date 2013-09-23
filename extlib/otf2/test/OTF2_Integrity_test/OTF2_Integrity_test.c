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
 *  @file       OTF2_Integrity_test.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This test generates an OTF2 archive with a defined data set.
 *              Afterwards it reads this archive back in and compares the data
 *              sets.
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>

/* ___ Global defines _______________________________________________________ */



/** @brief Number of locations. Should be small. */
#define NUM_LOCATIONS 4

/** @brief Number of events per location. */
#define NUMBER_OF_EVENTS 20000

/** @brief Archive path */
#define OTF2_ARCHIVE_PATH "OTF2_Integrity_trace"

/** @brief Trace name */
#define OTF2_TRACE_NAME "TestTrace"


/* ___ Global variables. ____________________________________________________ */



/** @internal
 *  @brief Defines if debug is turned on (1) or off (0). */
static bool otf2_DEBUG;


/** @internal
 *  @brief Global time counter. */
static uint64_t otf2_Time;


/** @internal
 *  @brief Global region counter. */
static uint64_t otf2_Region;



/* ___ Prototypes for static functions. _____________________________________ */



static inline void
check_pointer
(
    void* pointer,
    char* description,
    ...
);

static inline void
check_status
(
    OTF2_ErrorCode status,
    char*          description,
    ...
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

static inline uint64_t
get_time
(
    void
);

static inline OTF2_ErrorCode
create_archive
(
    void
);

static inline OTF2_ErrorCode
check_archive
(
    const char* trace
);



/* ___ Prototypes for all callbacks. ________________________________________ */

static inline OTF2_CallbackCode
GlobDefLocation_check
(
    void*             userData,
    uint64_t          locationIdentifier,
    uint32_t          stringID,
    OTF2_LocationType locationType,
    uint64_t          numberOfEvents,
    uint32_t          locationGroup
);

static inline OTF2_CallbackCode
Enter_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            regionID
);

static inline OTF2_CallbackCode
Leave_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            regionID
);

static inline OTF2_CallbackCode
MpiSend_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            receiver,
    uint32_t            communicator,
    uint32_t            msgTag,
    uint64_t            msgLength
);

static inline OTF2_CallbackCode
MpiRecv_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            sender,
    uint32_t            communicator,
    uint32_t            msgTag,
    uint64_t            msgLength
);




/* ___ main _________________________________________________________________ */



/** Main function
 *
 *  This test generates an OTF2 archive with a defined data set. Afterwards it
 *  reads this archive back in and compares the data sets.
 *
 *  @return                 Returns EXIT_SUCCESS if successful, EXIT_FAILURE
 *                          if an error occures.
 */
int
main( int   argc,
      char* argv[] )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    otf2_DEBUG = !!getenv( "OTF2_DEBUG_TESTS" );

    if ( argc == 1 )
    {
        status = create_archive();
        check_status( status, "Create Archive." );

        status = check_archive( OTF2_ARCHIVE_PATH "/" OTF2_TRACE_NAME ".otf2" );
        check_status( status, "Check Archive." );
    }
    else
    {
        status = check_archive( argv[ 1 ] );
        check_status( status, "Check Archive." );
    }


    return EXIT_SUCCESS;
}



/* ___ Implementation of static functions ___________________________________ */



/** @internal
 *  @brief Check if pointer is NULL.
 *
 *  Checks if a pointer is NULL. If so it prints an error with the passed
 *  description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param pointer          Pointer to be checked.
 *  @param description      Description for error/debug message.
 *  @param ...              Variable argument list like e.g. in printf.
 */
static inline void
check_pointer
(
    void* pointer,
    char* description,
    ...
)
{
    va_list va;
    va_start( va, description );

    if ( pointer == NULL )
    {
        printf( "==ERROR== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
        exit( EXIT_FAILURE );
    }

    if ( otf2_DEBUG )
    {
        printf( "==DEBUG== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
    }

    va_end( va );
}


/** @internal
 *  @brief Check if status is not OTF2_SUCCESS.
 *
 *  Checks if status is not OTF2_SUCCESS. If so it prints an error with the
 *  passed description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param status           Status to be checked.
 *  @param description      Description for error/debug message.
 *  @param ...              Variable argument list like e.g. in printf.
 */
static inline void
check_status
(
    OTF2_ErrorCode status,
    char*          description,
    ...
)
{
    va_list va;
    va_start( va, description );

    if ( status != OTF2_SUCCESS )
    {
        printf( "==ERROR== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
        exit( EXIT_FAILURE );
    }

    if ( otf2_DEBUG )
    {
        printf( "==DEBUG== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
    }

    va_end( va );
}


/** @brief Pre flush callback.
 *
 *  @param evtWriter        Ignored.
 *  @param evtReader        Ignored.
 *
 *  @return                 Returns always OTF2_NO_FLUSH.
 */
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


/** @internal
 *  @brief Get timestamp.
 *
 *  Generates consecutive timestamps.
 *
 *  @return                 Returns a timestamp.
 */
static inline uint64_t
get_time
(
    void
)
{
    static uint64_t sequence;
    return sequence++;
}


/** @internal
 *  @brief Create an OTF2 archive.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static inline OTF2_ErrorCode
create_archive
(
    void
)
{
    /* Create new archive handle. */
    OTF2_Archive* archive = OTF2_Archive_Open( OTF2_ARCHIVE_PATH,
                                               OTF2_TRACE_NAME,
                                               OTF2_FILEMODE_WRITE,
                                               1024 * 1024,
                                               4 * 1024 * 1024,
                                               OTF2_SUBSTRATE_POSIX,
                                               OTF2_COMPRESSION_NONE );
    check_pointer( archive, "Create archive" );


    /* Set master slave mode, description, and creator. */
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;
    status = OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
    check_status( status, "Set flush callbacks." );
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set master slave mode." );
    status = OTF2_Archive_SetDescription( archive, "OTF2 trace for integrity test." );
    check_status( status, "Set description." );
    status = OTF2_Archive_SetCreator( archive, "OTF2_Integrity" );
    check_status( status, "Set creator." );


    /* Generate location IDs. Just to have non-consecutive location IDs. */
    uint64_t locations[ NUM_LOCATIONS ];
    for ( uint64_t i = 0; i < NUM_LOCATIONS; ++i )
    {
        locations[ i ] = i * i;
    }


    /* Write location definitions. */
    OTF2_GlobalDefWriter* global_def_writer = NULL;
    global_def_writer = OTF2_Archive_GetGlobalDefWriter( archive );
    check_pointer( global_def_writer, "Get global definition writer" );

    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 0, "Process" );
    check_status( status, "Write string definition." );

    for ( uint64_t i = 0; i < NUM_LOCATIONS; ++i )
    {
        status = OTF2_GlobalDefWriter_WriteLocation( global_def_writer,
                                                     locations[ i ],
                                                     0,
                                                     OTF2_LOCATION_TYPE_CPU_THREAD,
                                                     NUMBER_OF_EVENTS * 6,
                                                     0 );
        check_status( status, "Write location definition." );
    }

    /* Write definitions. */
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 1, "MPI_Send" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 2, "Send an MPI message" );
    check_status( status, "Write string definition" );

    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 3, "MPI_Recv" );
    check_status( status, "Write string definition" );
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 4, "Receive an MPI message" );
    check_status( status, "Write string definition" );

    status = OTF2_GlobalDefWriter_WriteRegion( global_def_writer, 0, 1, 1, 2, OTF2_REGION_ROLE_UNKNOWN, OTF2_PARADIGM_UNKNOWN, OTF2_REGION_FLAG_NONE, 0, 0, 0 );
    check_status( status, "Write region definition" );

    status = OTF2_GlobalDefWriter_WriteRegion( global_def_writer, 1, 3, 3, 4, OTF2_REGION_ROLE_UNKNOWN, OTF2_PARADIGM_UNKNOWN, OTF2_REGION_FLAG_NONE, 0, 0, 0 );
    check_status( status, "Write region definition" );

    /* Define local event and definition writer. */
    OTF2_EvtWriter* evt_writer = NULL;
    OTF2_DefWriter* def_writer = NULL;


    for ( uint64_t i = 0; i < NUM_LOCATIONS; ++i )
    {
        /* Just open a definition writer, so the file is created. */
        def_writer = OTF2_Archive_GetDefWriter( archive, locations[ i ] );
        check_pointer( def_writer, "Get definition writer." );

        status = OTF2_Archive_CloseDefWriter( archive, def_writer );
        check_status( status, "Close definition writer." );
    }

    for ( uint64_t j = 0; j < NUMBER_OF_EVENTS; j++ )
    {
        for ( uint64_t i = 0; i < NUM_LOCATIONS; ++i )
        {
            evt_writer = OTF2_Archive_GetEvtWriter( archive, locations[ i ] );
            check_pointer( evt_writer, "Get event writer." );

            /* Write enter, mpi_send, leave and region definition. */
            uint64_t ts = get_time();
            status = OTF2_EvtWriter_Enter( evt_writer, NULL, ts, 0 );
            check_status( status, "Write Enter." );
            status = OTF2_EvtWriter_MpiSend( evt_writer, NULL, ts, locations[ i ], 0, 42, 1024 );
            check_status( status, "Write MPI_Send." );
            status = OTF2_EvtWriter_Leave( evt_writer, NULL, get_time(), 0 );
            check_status( status, "Write Leave." );

            /* Write enter, mpi_recv, leave and region definition. */
            status = OTF2_EvtWriter_Enter( evt_writer, NULL, get_time(), 1 );
            check_status( status, "Write Enter." );
            ts     = get_time();
            status = OTF2_EvtWriter_MpiRecv( evt_writer, NULL, ts, locations[ i ], 0, 42, 1024 );
            check_status( status, "Write MPI_Send." );
            status = OTF2_EvtWriter_Leave( evt_writer, NULL, ts, 1 );
            check_status( status, "Write Leave." );
        }
    }


    /* Finalize. */
    OTF2_Archive_Close( archive );


    return OTF2_SUCCESS;
}


/** @internal
 *  @brief Read an OTF2 archive an compare values.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static inline OTF2_ErrorCode
check_archive
(
    const char* trace
)
{
    /* Get a reader handle. */
    OTF2_Reader* reader = OTF2_Reader_Open( trace );
    check_pointer( reader, "Create new reader handle." );

    /* Get number of locations from the anchor file. */
    uint64_t       num_locations = 0;
    OTF2_ErrorCode status        = OTF2_SUCCESS;
    status = OTF2_Reader_GetNumberOfLocations( reader, &num_locations );
    check_status( status, "Get number of locations. Number of locations: %" PRIu64,
                  num_locations );

    /* Get global definition reader. */
    OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
    check_pointer( global_def_reader, "Create global definition reader handle." );

    /* Define and register definition callbacks. */
    OTF2_GlobalDefReaderCallbacks* check_defs = OTF2_GlobalDefReaderCallbacks_New();
    check_pointer( check_defs, "Create global definition callback handle." );
    OTF2_GlobalDefReaderCallbacks_SetLocationCallback( check_defs, GlobDefLocation_check );

    status = OTF2_Reader_RegisterGlobalDefCallbacks( reader, global_def_reader,
                                                     check_defs,
                                                     reader );
    check_status( status, "Register global definition callbacks." );

    OTF2_GlobalDefReaderCallbacks_Delete( check_defs );

    /* Read definitions. */
    uint64_t definitions_read = 0;
    status = OTF2_Reader_ReadGlobalDefinitions( reader, global_def_reader,
                                                OTF2_UNDEFINED_UINT64,
                                                &definitions_read );
    check_status( status, "Read global definitions. Number of definitions: %" PRIu64,
                  definitions_read );

    OTF2_Reader_CloseGlobalDefReader( reader,
                                      global_def_reader );

    /* Check if number of global definitions read matches the number of global
     * definitions defined in the anchor file. */
    uint64_t defs_anchor = 0;
    status = OTF2_Reader_GetNumberOfGlobalDefinitions( reader, &defs_anchor );
    check_status( status, "Get number of global definitions: %" PRIu64,
                  defs_anchor );

    if ( defs_anchor != definitions_read )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT,
                      "Number of global definitions read and defined in anchor file do not match!" );
    }

    printf( "Read %" PRIu64 " global definitions\n", definitions_read );

    /* Get global event reader. */
    OTF2_GlobalEvtReader* global_evt_reader = OTF2_Reader_GetGlobalEvtReader( reader );
    check_pointer( global_evt_reader, "Create global event reader." );

    /* Define and register event callbacks. */
    OTF2_GlobalEvtReaderCallbacks* check_evts = OTF2_GlobalEvtReaderCallbacks_New();
    check_pointer( check_evts, "Create global event callbacks." );

    OTF2_GlobalEvtReaderCallbacks_SetEnterCallback( check_evts, Enter_check );
    OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback( check_evts, Leave_check );
    OTF2_GlobalEvtReaderCallbacks_SetMpiSendCallback( check_evts, MpiSend_check );
    OTF2_GlobalEvtReaderCallbacks_SetMpiRecvCallback( check_evts, MpiRecv_check );

    status = OTF2_Reader_RegisterGlobalEvtCallbacks( reader, global_evt_reader,
                                                     check_evts,
                                                     NULL );
    check_status( status, "Register global event callbacks." );

    OTF2_GlobalEvtReaderCallbacks_Delete( check_evts );

    /* Read until an error occures. */
    uint64_t events_read = 0;
    status = OTF2_Reader_ReadGlobalEvents( reader, global_evt_reader, OTF2_UNDEFINED_UINT64, &events_read );
    check_status( status, "Read %" PRIu64 " events.", events_read );

    if ( events_read != ( 6 * NUM_LOCATIONS * NUMBER_OF_EVENTS ) )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT,
                      "Number of events read does not match!" );
    }

    printf( "Read %" PRIu64 " events\n", events_read );

    /* Close reader */
    status = OTF2_Reader_Close( reader );
    check_status( status, "Close reader." );

    return OTF2_SUCCESS;
}



/* ___ Implementation of callbacks __________________________________________ */



/** @internal
 *  @brief Callbacks for location definition.
 *
 *  @param userData             Optional user data.
 *  @param locationIdentifier   Location ID.
 *  @param stringID             String ID for the Location description.
 *  @param locationType         Type of the location.
 *  @param numberOfEvents       Number of events for this location.
 *
 *  @return                     Returns OTF2_SUCCESS if successful, an error
 *                              code if an error occures.
 */
static inline OTF2_CallbackCode
GlobDefLocation_check
(
    void*             userData,
    uint64_t          locationIdentifier,
    uint32_t          stringID,
    OTF2_LocationType locationType,
    uint64_t          numberOfEvents,
    uint32_t          locationGroup
)
{
    /* Check values. */
    if ( numberOfEvents != ( NUMBER_OF_EVENTS * 6 ) )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Check Location Definition." );
    }

    OTF2_Reader* reader = ( OTF2_Reader* )userData;

    /* Open a new local event reader for each found location ID. */
    OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( reader,
                                                           locationIdentifier );
    check_pointer( evt_reader, "Create local event reader for location %" PRIu64 ".",
                   locationIdentifier );


    /* Also open a definition reader and read all local definitions. */
    OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( reader,
                                                           locationIdentifier );
    check_pointer( def_reader, "Create local definition reader for location %" PRIu64 ".",
                   locationIdentifier );

    uint64_t       definitions_read = 0;
    OTF2_ErrorCode status           = OTF2_ERROR_INVALID;
    status = OTF2_Reader_ReadAllLocalDefinitions( reader, def_reader, &definitions_read );
    check_status( status, "Read %" PRIu64 " definitions for location %" PRIu64,
                  definitions_read, locationIdentifier );
    if ( definitions_read != 0 )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Number of local defs mismatch." );
    }

    return OTF2_CALLBACK_SUCCESS;
}


/** @internal
 *  @name Callbacks for events.
 *
 *  @param locationID       Location ID.
 *  @param time             Timestamp of the event.
 *  @param userData         Optional user data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occures.
 *  @{
 */
static inline OTF2_CallbackCode
Enter_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            regionID
)
{
    if ( time < otf2_Time )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Timestamp failure." );
    }
    otf2_Time = time;

    if ( regionID != otf2_Region )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Region ID failure." );
    }

    return OTF2_CALLBACK_SUCCESS;
}

static inline OTF2_CallbackCode
Leave_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            regionID
)
{
    if ( time < otf2_Time )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Timestamp failure." );
    }
    otf2_Time = time;

    if ( regionID != otf2_Region )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Region ID failure." );
    }
    otf2_Region = ( otf2_Region + 1 ) % 2;

    return OTF2_CALLBACK_SUCCESS;
}

static inline OTF2_CallbackCode
MpiSend_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            receiver,
    uint32_t            communicator,
    uint32_t            msgTag,
    uint64_t            msgLength
)
{
    if ( time < otf2_Time )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Timestamp failure." );
    }
    otf2_Time = time;

    if ( locationID != receiver )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Message receiver failure." );
    }

    if ( communicator != 0 )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Communicator failure." );
    }

    if ( msgTag != 42 )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Message tag failure." );
    }

    if ( msgLength != 1024 )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Message length failure." );
    }

    return OTF2_CALLBACK_SUCCESS;
}


static inline OTF2_CallbackCode
MpiRecv_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            sender,
    uint32_t            communicator,
    uint32_t            msgTag,
    uint64_t            msgLength
)
{
    if ( time < otf2_Time )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Timestamp failure." );
    }
    otf2_Time = time;

    if ( locationID != sender )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Message sender failure." );
    }

    if ( communicator != 0 )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Communicator failure." );
    }

    if ( msgTag != 42 )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Message tag failure." );
    }

    if ( msgLength != 1024 )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Message length failure." );
    }

    return OTF2_CALLBACK_SUCCESS;
}

/** @} */
