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
 *  @file       tess/OTF2_Event_Seek_test/OTF2_Event_Seek_test.c
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
             uint64_t            event_position,
             void*               userdata,
             OTF2_AttributeList* attributeList,
             uint32_t            region );

/* ___ main _________________________________________________________________ */

static uint64_t number_of_events = 100000;

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
        fprintf( stderr, "==ERROR== " );
        vfprintf( stderr, description, va );
        fprintf( stderr, "\n" );
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
        fprintf( stderr, "==ERROR== " );
        vfprintf( stderr, description, va );
        fprintf( stderr, "\n" );
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
        fprintf( stderr, "==ERROR== " );
        vfprintf( stderr, description, va );
        fprintf( stderr, "\n" );
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
    return ++sequence;
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
    OTF2_Archive* archive = OTF2_Archive_Open( "OTF2_Event_Seek_trace",
                                               "TestTrace",
                                               OTF2_FILEMODE_WRITE,
                                               256 * 1024,
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
    status = OTF2_Archive_SetCreator( archive, "OTF2_Event_Seek" );
    check_status( status, "Set creator." );

    OTF2_EvtWriter* evt_writer = OTF2_Archive_GetEvtWriter( archive, 0 );
    check_pointer( evt_writer, "Get event writer." );

    OTF2_AttributeList* attr_list = OTF2_AttributeList_New();
    srand( 3 );
    for ( uint64_t j = 1; j <= number_of_events; j++ )
    {
        double r = rand() / ( double )RAND_MAX;
        if ( r <= .25 )
        {
            status = OTF2_AttributeList_AddUint64( attr_list, 0, j );
            check_status( status, "Add attribute." );
        }
        status = OTF2_EvtWriter_Enter(
            evt_writer,
            attr_list,
            j,
            0 );
        check_status( status, "Write event." );
    }
    OTF2_AttributeList_Delete( attr_list );

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

    uint64_t number_of_events_written;
    OTF2_EvtWriter_GetNumberOfEvents( evt_writer,
                                      &number_of_events_written );

    status = OTF2_GlobalDefWriter_WriteLocation(
        global_def_writer, 0, 0,
        OTF2_LOCATION_TYPE_CPU_THREAD,
        number_of_events_written,
        0 );
    check_status( status, "Write location." );

    /* Write definitions. */
    status = OTF2_GlobalDefWriter_WriteRegion(
        global_def_writer, 0, 0, 0, 0,
        OTF2_REGION_ROLE_UNKNOWN, OTF2_PARADIGM_UNKNOWN, OTF2_REGION_FLAG_NONE,
        OTF2_UNDEFINED_UINT32, 0, 0 );
    check_status( status, "Write region" );

    status = OTF2_GlobalDefWriter_WriteAttribute(
        global_def_writer, 0, 0, OTF2_TYPE_UINT64 );
    check_status( status, "Write attribute" );

    OTF2_DefWriter* def_writer = OTF2_Archive_GetDefWriter( archive, 0 );
    check_pointer( def_writer, "Get definition writer." );

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
OTF2_ErrorCode
check_archive( void )
{
    OTF2_ErrorCode status;
    uint64_t       expected_event_position;

    /* Get a reader handle. */
    OTF2_Reader* reader = OTF2_Reader_Open( "OTF2_Event_Seek_trace/TestTrace.otf2" );
    check_pointer( reader, "Create new reader handle." );

    /* Get global definition reader. */
    OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
    check_pointer( global_def_reader, "Create global definition reader handle." );

    /* Define and register definition callbacks. */
    OTF2_GlobalDefReaderCallbacks* check_defs = OTF2_GlobalDefReaderCallbacks_New();
    check_pointer( check_defs, "Create global definition callback handle." );

    status = OTF2_Reader_RegisterGlobalDefCallbacks( reader, global_def_reader,
                                                     check_defs,
                                                     NULL );
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

    /* Define and register event callbacks. */
    OTF2_EvtReaderCallbacks* check_evts = OTF2_EvtReaderCallbacks_New();
    check_pointer( check_evts, "Create global event callbacks." );

    OTF2_EvtReaderCallbacks_SetEnterCallback( check_evts, Enter_check );

    status = OTF2_Reader_RegisterEvtCallbacks( reader,
                                               evt_reader,
                                               check_evts,
                                               &expected_event_position );
    check_status( status, "Register event callbacks." );

    OTF2_EvtReaderCallbacks_Delete( check_evts );

    /* Read until an error occures. */
    uint64_t events_read = 0;
    expected_event_position = 1;
    status                  = OTF2_Reader_ReadLocalEvents( reader,
                                                           evt_reader,
                                                           1,
                                                           &events_read );
    check_status( status, "Read first event." );

    status = OTF2_EvtReader_Seek( evt_reader, 1 );
    check_status( status, "Seek back to first event." );

    status = OTF2_EvtReader_ReadEvents( evt_reader,
                                        1,
                                        &events_read );
    check_status( status, "Read first event again." );

    expected_event_position = 100;
    status                  = OTF2_EvtReader_Seek( evt_reader, expected_event_position );
    check_status( status, "Seek to event #%" PRIu64 ".", expected_event_position );

    status = OTF2_EvtReader_ReadEvents( evt_reader,
                                        1,
                                        &events_read );
    check_status( status, "Read one event." );

    expected_event_position = number_of_events / 2;
    status                  = OTF2_EvtReader_Seek( evt_reader, expected_event_position );
    check_status( status, "Seek to event #%" PRIu64 ".", expected_event_position );

    status = OTF2_EvtReader_ReadEvents( evt_reader,
                                        1,
                                        &events_read );
    check_status( status, "Read one event." );

    expected_event_position = number_of_events / 4;
    status                  = OTF2_EvtReader_Seek( evt_reader, expected_event_position );
    check_status( status, "Seek to event #%" PRIu64 ".", expected_event_position );

    status = OTF2_EvtReader_ReadEvents( evt_reader,
                                        1,
                                        &events_read );
    check_status( status, "Read one event." );

    expected_event_position = 1;
    status                  = OTF2_EvtReader_Seek( evt_reader, 1 );
    check_status( status, "Seek back to first event (2)." );

    status = OTF2_EvtReader_ReadEvents( evt_reader,
                                        1,
                                        &events_read );
    check_status( status, "Read first event again (2)." );

    srand( 1 );
    for ( uint64_t j = 0; j < number_of_events / 4; j++ )
    {
        double r = rand() / ( double )RAND_MAX;
        expected_event_position = ( uint64_t )( r * number_of_events );

        status = OTF2_EvtReader_Seek( evt_reader, expected_event_position );
        check_status( status, "%" PRIu64 "st/nd/th seek to random event #%" PRIu64 ".", j, expected_event_position );

        status = OTF2_EvtReader_ReadEvents( evt_reader,
                                            1,
                                            &events_read );
        check_status( status, "Read one event." );
    }

    /* Close reader */
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
             uint64_t            event_position,
             void*               userdata,
             OTF2_AttributeList* attributeList,
             uint32_t            region )
{
    uint64_t expected_event_position = *( uint64_t* )userdata;

    check_condition( time == expected_event_position,
                     "Time doesn't match the expected event position: %" PRIu64, time );

    check_condition( event_position == expected_event_position,
                     "The told event position doesn't match the expected event position: %" PRIu64, event_position );

    if ( OTF2_AttributeList_TestAttributeByID( attributeList, 0 ) )
    {
        OTF2_ErrorCode ret;
        uint64_t       attr_value;
        ret = OTF2_AttributeList_GetUint64( attributeList, 0, &attr_value );
        check_status( ret, "Get uint64_t attribute." );
        check_condition( attr_value == expected_event_position,
                         "The attribute value doesn't match the expected event position: %" PRIu64, attr_value );
    }

    return OTF2_CALLBACK_SUCCESS;
}


/** @} */
