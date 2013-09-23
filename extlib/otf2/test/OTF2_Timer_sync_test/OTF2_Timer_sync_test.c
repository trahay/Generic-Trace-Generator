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
 *  @file       tess/OTF2_Timer_sync_test/OTF2_Timer_sync.c
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @brief      This test generates an OTF2 archive with a defined data set.
 *              Afterwards it reads this archive back in and check if the timer
 *              synchronization works.
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

/* ___ Prototypes for static functions. _____________________________________ */



static void
check_pointer( void* pointer,
               char* description,
               ... );

static void
check_status( OTF2_ErrorCode status,
              char*          description,
              ... );

static void
check_condition( bool  condition,
                 char* description,
                 ... );

static OTF2_FlushType
pre_flush( void*         userData,
           OTF2_FileType fileType,
           uint64_t      locationId,
           void*         callerData,
           bool          final );

static OTF2_TimeStamp
post_flush( void*         userData,
            OTF2_FileType fileType,
            uint64_t      locationId );

static OTF2_FlushCallbacks flush_callbacks =
{
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
};

static uint64_t
get_time( void );

static OTF2_ErrorCode
create_archive( void );

static OTF2_ErrorCode
check_archive( void );



/* ___ Prototypes for all callbacks. ________________________________________ */

static OTF2_CallbackCode
Enter_check( uint64_t            locationID,
             uint64_t            time,
             void*               userData,
             OTF2_AttributeList* attributes,
             uint32_t            regionID );

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
main( void )
{
    OTF2_ErrorCode status;

    status = create_archive();
    check_status( status, "Create Archive." );

    status = check_archive();
    check_status( status, "Check Archive." );

    return EXIT_SUCCESS;
}



/* ___ Test data ____________________________________________________________ */

typedef struct otf2_event_timestamp_struct
{
    uint64_t local_timestamp;
    uint64_t expected_global_timestamp;
} otf2_event_timestamp;
static const otf2_event_timestamp event_timestamps[] = {
    { .local_timestamp =  0, .expected_global_timestamp =  0 },
    { .local_timestamp =  1, .expected_global_timestamp =  2 },
    { .local_timestamp =  2, .expected_global_timestamp =  4 },
    { .local_timestamp =  3, .expected_global_timestamp =  6 },
    { .local_timestamp =  4, .expected_global_timestamp =  8 },
    { .local_timestamp =  5, .expected_global_timestamp = 10 },
    { .local_timestamp =  6, .expected_global_timestamp = 11 },
    { .local_timestamp =  7, .expected_global_timestamp = 12 },
    { .local_timestamp =  8, .expected_global_timestamp = 13 },
    { .local_timestamp =  9, .expected_global_timestamp = 14 },
    { .local_timestamp = 10, .expected_global_timestamp = 15 },
    { .local_timestamp = 11, .expected_global_timestamp = 16 },
};
#define number_of_events \
    ( sizeof( event_timestamps ) / sizeof( event_timestamps[ 0 ] ) )

typedef struct otf2_clock_offset_struct
{
    uint64_t time;
    int64_t  offset;
} otf2_clock_offset;
static const otf2_clock_offset clock_offsets[] = {
    { .time =  0, .offset = 0 },
    { .time =  5, .offset = 5 },
    { .time = 10, .offset = 5 },
};
#define number_of_clock_offsets \
    ( sizeof( clock_offsets ) / sizeof( clock_offsets[ 0 ] ) )



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
void
check_pointer( void* pointer, char* description, ... )
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
void
check_status( OTF2_ErrorCode status, char* description, ... )
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
void
check_condition( bool condition, char* description, ... )
{
    va_list va;
    va_start( va, description );

    if ( !condition )
    {
        printf( "==ERROR== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
        exit( EXIT_FAILURE );
    }

    va_end( va );
}


/** @brief Pre flush callback.
 *
 *  @return                 Returns always OTF2_NO_FLUSH.
 */
static OTF2_FlushType
pre_flush( void*         userData,
           OTF2_FileType fileType,
           uint64_t      locationId,
           void*         callerData,
           bool          final )
{
    return OTF2_FLUSH;
}

static OTF2_TimeStamp
post_flush( void*         userData,
            OTF2_FileType fileType,
            uint64_t      locationId )
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
uint64_t
get_time( void )
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
OTF2_ErrorCode
create_archive( void )
{
    OTF2_ErrorCode status;

    /* Create new archive handle. */
    OTF2_Archive* archive = OTF2_Archive_Open( "OTF2_Timer_sync_trace",
                                               "TestTrace",
                                               OTF2_FILEMODE_WRITE,
                                               1024 * 1024,
                                               4 * 1024 * 1024,
                                               OTF2_SUBSTRATE_POSIX,
                                               OTF2_COMPRESSION_NONE );
    check_pointer( archive, "Create archive" );

    status = OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
    check_status( status, "Set flush callbacks." );
    /* Set master slave mode, description, and creator. */
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set master slave mode." );
    status = OTF2_Archive_SetDescription( archive, "OTF2 trace for timer synchronization test." );
    check_status( status, "Set description." );
    status = OTF2_Archive_SetCreator( archive, "OTF2_Timer_sync" );
    check_status( status, "Set creator." );

    /* Write location definitions. */
    OTF2_GlobalDefWriter* global_def_writer =
        OTF2_Archive_GetGlobalDefWriter( archive );
    check_pointer( global_def_writer, "Get global definition writer" );

    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 0, "" );
    check_status( status, "Write string definition." );

    status = OTF2_GlobalDefWriter_WriteSystemTreeNode(
        global_def_writer, 0, 0, 0, OTF2_UNDEFINED_SYSTEM_TREE_NODE );
    check_status( status, "Write system tree node." );

    status = OTF2_GlobalDefWriter_WriteLocationGroup(
        global_def_writer, 0, 0, OTF2_LOCATION_GROUP_TYPE_PROCESS, 0 );
    check_status( status, "Write location group." );

    status = OTF2_GlobalDefWriter_WriteLocation(
        global_def_writer, 0, 0,
        OTF2_LOCATION_TYPE_CPU_THREAD,
        number_of_events,
        0 );
    check_status( status, "Write location." );

    /* Write definitions. */
    status = OTF2_GlobalDefWriter_WriteRegion(
        global_def_writer, 0, 0, 0, 0,
        OTF2_REGION_ROLE_UNKNOWN, OTF2_PARADIGM_UNKNOWN, OTF2_REGION_FLAG_NONE,
        OTF2_UNDEFINED_STRING, 0, 0 );
    check_status( status, "Write region" );

    OTF2_EvtWriter* evt_writer = OTF2_Archive_GetEvtWriter( archive, 0 );
    check_pointer( evt_writer, "Get event writer." );

    for ( uint32_t j = 0; j < number_of_events; j++ )
    {
        status = OTF2_EvtWriter_Enter(
            evt_writer, NULL,
            event_timestamps[ j ].local_timestamp,
            0 );
        check_status( status, "Write event." );
    }

    OTF2_DefWriter* def_writer = OTF2_Archive_GetDefWriter( archive, 0 );
    check_pointer( def_writer, "Get definition writer." );

    for ( uint32_t i = 0; i < number_of_clock_offsets; i++ )
    {
        status = OTF2_DefWriter_WriteClockOffset(
            def_writer,
            clock_offsets[ i ].time,
            clock_offsets[ i ].offset,
            0.0 );
        check_status( status, "Write timer sync." );
    }

    /* Finalize. */
    status = OTF2_Archive_Close( archive );
    check_status( status, "Close archive." );

    return OTF2_SUCCESS;
}


/** @internal
 *  @brief Read an OTF2 archive an compare values.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
check_archive( void )
{
    OTF2_ErrorCode status;

    /* Get a reader handle. */
    OTF2_Reader* reader = OTF2_Reader_Open( "OTF2_Timer_sync_trace/TestTrace.otf2" );
    check_pointer( reader, "Create new reader handle." );

    /* Get global definition reader. */
    OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
    check_pointer( global_def_reader, "Create global definition reader handle." );

    /* Define and register definition callbacks. */
    OTF2_GlobalDefReaderCallbacks* check_defs = OTF2_GlobalDefReaderCallbacks_New();
    check_pointer( check_defs, "Create global definition callback handle." );

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
    check_status( status, "Read global definitions." );

    OTF2_Reader_CloseGlobalDefReader( reader,
                                      global_def_reader );

    /* Open the local event reader. */
    OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( reader, 0 );
    check_pointer( evt_reader, "Create local event reader." );

    /* Also open a definition reader and read all local definitions. */
    OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( reader, 0 );
    check_pointer( def_reader, "Create local definition reader." );

    status = OTF2_Reader_ReadAllLocalDefinitions( reader, def_reader,
                                                  &definitions_read );
    check_status( status, "Read local definitions." );

    /* Get global event reader. */
    OTF2_GlobalEvtReader* global_evt_reader = OTF2_Reader_GetGlobalEvtReader( reader );
    check_pointer( global_evt_reader, "Create global event reader." );

    /* Define and register event callbacks. */
    OTF2_GlobalEvtReaderCallbacks* check_evts = OTF2_GlobalEvtReaderCallbacks_New();
    check_pointer( check_evts, "Create global event callbacks." );

    OTF2_GlobalEvtReaderCallbacks_SetEnterCallback( check_evts, Enter_check );

    uint32_t timestamp_position = 0;
    status = OTF2_Reader_RegisterGlobalEvtCallbacks( reader, global_evt_reader,
                                                     check_evts,
                                                     &timestamp_position );
    check_status( status, "Register global event callbacks." );

    OTF2_GlobalEvtReaderCallbacks_Delete( check_evts );

    /* Read until an error occures. */
    uint64_t events_read = 0;
    status = OTF2_Reader_ReadGlobalEvents( reader, global_evt_reader, OTF2_UNDEFINED_UINT64, &events_read );
    check_status( status, "Read %" PRIu64 " events.", events_read );

    /* Close reader. */
    status = OTF2_Reader_Close( reader );
    check_status( status, "Close reader." );

    return OTF2_SUCCESS;
}



/* ___ Implementation of callbacks __________________________________________ */



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
OTF2_CallbackCode
Enter_check( uint64_t            locationID,
             uint64_t            time,
             void*               userData,
             OTF2_AttributeList* attributes,
             uint32_t            regionID )
{
    uint32_t timestamp_position = *( uint32_t* )userData;

    check_condition( timestamp_position < number_of_events,
                     "More events read than writen." );

    check_condition( event_timestamps[ timestamp_position ].expected_global_timestamp == time,
                     "Timer synchronization mismatch for event %u: expected %" PRIu64 ", got %" PRIu64,
                     timestamp_position,
                     event_timestamps[ timestamp_position ].expected_global_timestamp,
                     time );

    /* increment position for next event */
    *( uint32_t* )userData = timestamp_position + 1;

    return OTF2_CALLBACK_SUCCESS;
}


/** @} */
