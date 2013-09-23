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
 *  @file       OTF2_CallbackInterrupt_test.c
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
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



/** @brief Number of events per location. */
#define NUMBER_OF_EVENTS 10

/** @brief Archive path */
#define OTF2_ARCHIVE_PATH "OTF2_CallbackInterrupt_trace"

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

static OTF2_CallbackCode
Enter_check( uint64_t            locationID,
             uint64_t            time,
             uint64_t            eventPosition,
             void*               userData,
             OTF2_AttributeList* attributeList,
             OTF2_RegionRef      region );


static OTF2_CallbackCode
Leave_check( uint64_t            locationID,
             uint64_t            time,
             uint64_t            eventPosition,
             void*               userData,
             OTF2_AttributeList* attributeList,
             OTF2_RegionRef      region );

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
    status = OTF2_Archive_SetCreator( archive, "OTF2_CallbackInterrupt" );
    check_status( status, "Set creator." );

    /* Write location definitions. */
    OTF2_GlobalDefWriter* global_def_writer = NULL;
    global_def_writer = OTF2_Archive_GetGlobalDefWriter( archive );
    check_pointer( global_def_writer, "Get global definition writer" );

    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 0, "" );
    check_status( status, "Write string definition." );
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 1, "machine" );
    check_status( status, "Write string definition." );
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 2, "test" );
    check_status( status, "Write string definition." );
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 3, "process" );
    check_status( status, "Write string definition." );
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 4, "thread" );
    check_status( status, "Write string definition." );
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 5, "foo" );
    check_status( status, "Write string definition." );
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 6, "bar" );
    check_status( status, "Write string definition." );

    status = OTF2_GlobalDefWriter_WriteSystemTreeNode( global_def_writer,
                                                       0,
                                                       2,
                                                       1,
                                                       OTF2_UNDEFINED_SYSTEM_TREE_NODE );
    check_status( status, "Write system tree node definition." );

    status = OTF2_GlobalDefWriter_WriteLocationGroup( global_def_writer,
                                                      0,
                                                      3,
                                                      OTF2_LOCATION_GROUP_TYPE_PROCESS,
                                                      0 );
    check_status( status, "Write location group definition." );

    status = OTF2_GlobalDefWriter_WriteLocation( global_def_writer,
                                                 0,
                                                 4,
                                                 OTF2_LOCATION_TYPE_CPU_THREAD,
                                                 2 * NUMBER_OF_EVENTS,
                                                 0 );
    check_status( status, "Write location definition." );

    /* Write definitions. */
    status = OTF2_GlobalDefWriter_WriteRegion( global_def_writer,
                                               0,
                                               5, 6, 0,
                                               OTF2_REGION_ROLE_FUNCTION,
                                               OTF2_PARADIGM_USER,
                                               OTF2_REGION_FLAG_NONE,
                                               0, 0, 0 );
    check_status( status, "Write region definition" );

    /* Define local event and definition writer. */
    OTF2_EvtWriter* evt_writer = NULL;

    evt_writer = OTF2_Archive_GetEvtWriter( archive, 0 );
    for ( uint64_t j = 0; j < NUMBER_OF_EVENTS; j++ )
    {
        /* Write enter. */
        uint64_t ts = get_time();
        status = OTF2_EvtWriter_Enter( evt_writer, NULL, ts, 0 );
        check_status( status, "Write Enter." );
    }
    for ( uint64_t j = 0; j < NUMBER_OF_EVENTS; j++ )
    {
        /* Write leave. */
        uint64_t ts = get_time();
        status = OTF2_EvtWriter_Leave( evt_writer, NULL, ts, 0 );
        check_status( status, "Write Leave." );
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
    OTF2_ErrorCode status;

    /* Get a reader handle. */
    OTF2_Reader* reader = OTF2_Reader_Open( trace );
    check_pointer( reader, "Create new reader handle." );

    /* Get global event reader. */
    OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( reader, 0 );
    check_pointer( evt_reader, "Create event reader." );

    /* Define and register event callbacks. */
    OTF2_EvtReaderCallbacks* check_evts = OTF2_EvtReaderCallbacks_New();
    check_pointer( check_evts, "Create event callbacks." );

    OTF2_EvtReaderCallbacks_SetEnterCallback( check_evts, Enter_check );
    OTF2_EvtReaderCallbacks_SetLeaveCallback( check_evts, Leave_check );

    uint32_t counter = 0;
    status = OTF2_Reader_RegisterEvtCallbacks( reader,
                                               evt_reader,
                                               check_evts,
                                               &counter );
    check_status( status, "Register global event callbacks." );

    OTF2_EvtReaderCallbacks_Delete( check_evts );

    /* Read the first NUMBER_OF_EVENTS records (the enters) */
    uint64_t events_read = 0;
    status = OTF2_Reader_ReadLocalEvents( reader, evt_reader, OTF2_UNDEFINED_UINT64, &events_read );
    if ( status != OTF2_ERROR_INTERRUPTED_BY_CALLBACK )
    {
        check_status( status, "Expected OTF2_ERROR_INTERRUPTED_BY_CALLBACK." );
    }
    if ( events_read != NUMBER_OF_EVENTS )
    {
        check_status( status, "Didn't read %d events.", NUMBER_OF_EVENTS );
    }

    /* Read the second NUMBER_OF_EVENTS records (the leaves) */
    events_read = 0;
    status      = OTF2_Reader_ReadLocalEvents( reader, evt_reader, OTF2_UNDEFINED_UINT64, &events_read );
    if ( status != OTF2_SUCCESS )
    {
        check_status( status, "Expected OTF2_SUCCESS." );
    }
    if ( events_read != NUMBER_OF_EVENTS )
    {
        check_status( status, "Didn't read %d events.", NUMBER_OF_EVENTS );
    }

    /* Close reader */
    status = OTF2_Reader_Close( reader );
    check_status( status, "Close reader." );

    return OTF2_SUCCESS;
}



/* ___ Implementation of callbacks __________________________________________ */


OTF2_CallbackCode
Enter_check( uint64_t            locationID,
             uint64_t            time,
             uint64_t            eventPosition,
             void*               userData,
             OTF2_AttributeList* attributeList,
             OTF2_RegionRef      region )
{
    uint32_t* counter = userData;

    ( *counter )++;

    if ( ( *counter ) == NUMBER_OF_EVENTS )
    {
        return OTF2_CALLBACK_INTERRUPT;
    }

    return OTF2_CALLBACK_SUCCESS;
}

OTF2_CallbackCode
Leave_check( uint64_t            locationID,
             uint64_t            time,
             uint64_t            eventPosition,
             void*               userData,
             OTF2_AttributeList* attributeList,
             OTF2_RegionRef      region )
{
    uint32_t* counter = userData;

    ( *counter )++;

    return OTF2_CALLBACK_SUCCESS;
}

/** @} */
