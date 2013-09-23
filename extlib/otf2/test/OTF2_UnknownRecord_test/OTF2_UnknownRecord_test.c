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
 *  @status     alpha
 *
 *  @maintainer Ronny Tschueter <ronny.tschueter@zih.tu-dresden.de>
 *  @authors    Ronny Tschueter <ronny.tschueter@zih.tu-dresden.de>
 *
 *  @brief      Generates a event sequence including unknown records of unknown type.
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

#include <otf2_internal.h>
#include <otf2_reader.h>
#include <otf2_archive.h>

#include <otf2_events_int.h>

#include <OTF2_File.h>
#include <OTF2_Buffer.h>

#define OTF2_UNKNOWN_RECORD_TEST_CHUNK_SIZE ( 1024 * 1024 )
#define OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_EVENTS 6
#define OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_UNKNOWN_RECORDS 4
#define OTF2_UNKNOWN_RECORD_TEST_TRACE_DIR "./unknown_record_test_trace"
#define OTF2_UNKNOWN_RECORD_TEST_TRACE_NAME "traces"


/* ___ Global variables. ____________________________________________________ */


/** @internal
 *  @brief Defines if debug is turned on (1) or off (0). */
static bool otf2_DEBUG;


/** @internal
 *  @brief Global time counter. */
static uint64_t expected_time = 0;


/** @internal
 *  @brief Global region counter. */
static uint64_t expected_region = 0;


/** @internal
 *  @brief Global counter of unknown records. */
static uint64_t unknown_records_read = 0;


/* ___ Function prototypes. _________________________________________________ */


static inline OTF2_ErrorCode
create_archive
(
    void
);

static inline OTF2_ErrorCode
check_archive
(
    void
);

static inline void
write_definitions
(
    OTF2_Archive* archive
);

static inline void
write_events
(
    OTF2_Archive* archive
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

static uint64_t
get_time
(
    void
);

static inline OTF2_CallbackCode
def_location_check
(
    void*             userData,
    uint64_t          locationIdentifier,
    uint32_t          stringID,
    OTF2_LocationType locationType,
    uint64_t          numberOfEvents,
    uint32_t          locationGroup
);

static inline OTF2_CallbackCode
enter_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            regionID
);

static inline OTF2_CallbackCode
leave_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            regionID
);

static inline OTF2_CallbackCode
unknown_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes
);

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

static OTF2_FlushCallbacks flush_callbacks =
{
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
};


int
main()
{
    OTF2_ErrorCode status;

    status = create_archive();
    check_status( status, "Create archive." );

    check_archive();
    check_status( status, "Check archive." );

    return 0;
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
    OTF2_ErrorCode status;

    OTF2_Archive* archive = OTF2_Archive_Open( OTF2_UNKNOWN_RECORD_TEST_TRACE_DIR,
                                               OTF2_UNKNOWN_RECORD_TEST_TRACE_NAME,
                                               OTF2_FILEMODE_WRITE,
                                               OTF2_UNKNOWN_RECORD_TEST_CHUNK_SIZE,
                                               4 * 1024 * 1024,
                                               OTF2_SUBSTRATE_POSIX,
                                               OTF2_COMPRESSION_NONE );
    check_pointer( archive, "Create writer archive handle" );

    status = OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
    check_status( status, "Set flush callbacks." );
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set archive to master mode." );

    /* Write definitions */
    write_definitions( archive );

    /* Write events */
    write_events( archive );

    status = OTF2_Archive_Close( archive );
    check_status( status, "Close archive." );

    return status;
}

void
write_definitions
(
    OTF2_Archive* archive
)
{
    OTF2_ErrorCode status;

    /* Write location definitions. */
    OTF2_GlobalDefWriter* global_def_writer = NULL;
    global_def_writer = OTF2_Archive_GetGlobalDefWriter( archive );
    check_pointer( global_def_writer, "Get global definition writer" );

    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 0, "Process" );
    check_status( status, "Write string definition." );

    status = OTF2_GlobalDefWriter_WriteLocation( global_def_writer,
                                                 0,  // location ID
                                                 0,
                                                 OTF2_LOCATION_TYPE_CPU_THREAD,
                                                 OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_EVENTS + OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_UNKNOWN_RECORDS,
                                                 0 );
    check_status( status, "Write location definition." );

    /* Write definitions. */
    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 1, "simple_example.c" );
    check_status( status, "Write string definition" );

    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 2, "main" );
    check_status( status, "Write string definition" );

    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 3, "foo" );
    check_status( status, "Write string definition" );

    status = OTF2_GlobalDefWriter_WriteString( global_def_writer, 4, "bar" );
    check_status( status, "Write string definition" );

    status = OTF2_GlobalDefWriter_WriteRegion( global_def_writer, 0, 2, 2, 0, OTF2_REGION_ROLE_UNKNOWN, OTF2_PARADIGM_UNKNOWN, OTF2_REGION_FLAG_NONE, 1, 0, 0 );
    check_status( status, "Write region definition" );

    status = OTF2_GlobalDefWriter_WriteRegion( global_def_writer, 1, 3, 3, 0, OTF2_REGION_ROLE_UNKNOWN, OTF2_PARADIGM_UNKNOWN, OTF2_REGION_FLAG_NONE, 1, 0, 0 );
    check_status( status, "Write region definition" );

    status = OTF2_GlobalDefWriter_WriteRegion( global_def_writer, 2, 4, 4, 0, OTF2_REGION_ROLE_UNKNOWN, OTF2_PARADIGM_UNKNOWN, OTF2_REGION_FLAG_NONE, 1, 0, 0 );
    check_status( status, "Write region definition" );

    /* Just open a definition writer, so the file is created. */
    OTF2_DefWriter* def_writer = OTF2_Archive_GetDefWriter( archive, 0 );
    check_pointer( def_writer, "Get definition writer." );
}

void
write_events
(
    OTF2_Archive* archive
)
{
    OTF2_ErrorCode status;

    /* Open a new buffer handle. */
    OTF2_Buffer* buffer = OTF2_Buffer_New( archive,
                                           NULL,
                                           OTF2_UNKNOWN_RECORD_TEST_CHUNK_SIZE,
                                           OTF2_BUFFER_WRITE,
                                           OTF2_BUFFER_CHUNKED,
                                           OTF2_FILETYPE_EVENTS,
                                           0 );
    check_pointer( buffer, "Create new buffer handle" );

    /* Maximum record length of an enter/leave record is the record id byte
     * plus the individual size of the attributes (uint32_t region,
     * excluding timestamp). */
    size_t         record_length = 6;
    OTF2_ErrorCode ret           = OTF2_Buffer_WriteTimeStamp( buffer,
                                                               get_time(),
                                                               record_length );
    OTF2_Buffer_WriteUint8( buffer, OTF2_EVENT_ENTER );
    OTF2_Buffer_WriteUint32( buffer, 0 );

    /* Write record of unknown type */
    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    /* Write unknown record type */
    OTF2_Buffer_WriteUint8( buffer, 0xFE );
    /* Write record length byte */
    OTF2_Buffer_WriteUint8( buffer, 1 );
    /* Write dummy data */
    OTF2_Buffer_WriteUint8( buffer, 0 );

    /* Write record of unknown type */
    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    /* Write unknown record type */
    OTF2_Buffer_WriteUint8( buffer, 0xFE );
    /* Write length for a large record, that means first
     * record length is set to UINT8_MAX and length information
     * is encoded in additional 8 bytes */
    OTF2_Buffer_WriteUint8( buffer, UINT8_MAX );
    /* Write length in additional 8 bytes */
    OTF2_Buffer_WriteUint64Full( buffer, 2 );
    /* Write dummy data */
    OTF2_Buffer_WriteUint16( buffer, 0 );

    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    OTF2_Buffer_WriteUint8( buffer, OTF2_EVENT_ENTER );
    OTF2_Buffer_WriteUint32( buffer, 1 );


    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    OTF2_Buffer_WriteUint8( buffer, OTF2_EVENT_ENTER );
    OTF2_Buffer_WriteUint32( buffer, 2 );

    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    OTF2_Buffer_WriteUint8( buffer, OTF2_EVENT_LEAVE );
    OTF2_Buffer_WriteUint32( buffer, 2 );

    /* Write record of unknown type */
    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    /* Write unknown record type */
    OTF2_Buffer_WriteUint8( buffer, 0xFE );
    /* Write record length byte */
    OTF2_Buffer_WriteUint8( buffer, 9 );
    /* Write dummy data */
    OTF2_Buffer_WriteUint64Full( buffer, 0 );
    OTF2_Buffer_WriteUint8( buffer, 0 );


    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    OTF2_Buffer_WriteUint8( buffer, OTF2_EVENT_LEAVE );
    OTF2_Buffer_WriteUint32( buffer, 1 );

    /* Write record of unknown type */
    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    /* Write unknown record type */
    OTF2_Buffer_WriteUint8( buffer, 0xFE );
    /* Write record length byte */
    OTF2_Buffer_WriteUint8( buffer, 0 );
    /* No dummy data needed */


    ret = OTF2_Buffer_WriteTimeStamp( buffer,
                                      get_time(),
                                      record_length );
    OTF2_Buffer_WriteUint8( buffer, OTF2_EVENT_LEAVE );
    OTF2_Buffer_WriteUint32( buffer, 0 );

    OTF2_Buffer_WriteUint8( buffer, OTF2_BUFFER_END_OF_FILE );

    /* Switch to reading and read data back in. */
    status = OTF2_Buffer_Delete( buffer );
    check_status( status, "Close buffer" );
}

static inline OTF2_ErrorCode
check_archive
(
    void
)
{
    /* Get string length of trace file path:
     * length of path to directory
     * + one additional character for directory separator
     * + length of trace file name
     * + five additional for '.otf2' file extension */
    size_t string_length = strlen( OTF2_UNKNOWN_RECORD_TEST_TRACE_DIR ) + strlen( OTF2_UNKNOWN_RECORD_TEST_TRACE_DIR ) + 6;
    char*  trace_name    = malloc( ( string_length + 1 ) * sizeof( char ) );
    if ( trace_name == NULL )
    {
        return OTF2_ERROR_ENOMEM;
    }
    snprintf( trace_name, string_length, "%s/%s.otf2", OTF2_UNKNOWN_RECORD_TEST_TRACE_DIR, OTF2_UNKNOWN_RECORD_TEST_TRACE_NAME );

    /* Open reader. */
    OTF2_Reader* reader = OTF2_Reader_Open( trace_name );
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
    OTF2_GlobalDefReaderCallbacks_SetLocationCallback( check_defs, def_location_check );

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

    OTF2_GlobalEvtReaderCallbacks_SetEnterCallback( check_evts, enter_check );
    OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback( check_evts, leave_check );
    OTF2_GlobalEvtReaderCallbacks_SetUnknownCallback( check_evts, unknown_check );

    status = OTF2_Reader_RegisterGlobalEvtCallbacks( reader, global_evt_reader,
                                                     check_evts,
                                                     NULL );
    check_status( status, "Register global event callbacks." );

    OTF2_GlobalEvtReaderCallbacks_Delete( check_evts );

    /* Read until an error occures. */
    uint64_t events_read = 0;
    status = OTF2_Reader_ReadGlobalEvents( reader, global_evt_reader, OTF2_UNDEFINED_UINT64, &events_read );
    check_status( status, "Read %" PRIu64 " events.", events_read );

    if ( events_read != ( OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_EVENTS + OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_UNKNOWN_RECORDS ) )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT,
                      "Number of events read does not match!" );
    }

    if ( unknown_records_read != OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_UNKNOWN_RECORDS )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT,
                      "Number of unknown records read does not match!" );
    }

    printf( "Read %" PRIu64 " events\n", events_read );
    printf( "Read %" PRIu64 " unknown records\n", unknown_records_read );

    /* Close reader. */
    status = OTF2_Reader_Close( reader );
    check_status( status, "Close Reader." );

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
def_location_check
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
    if ( numberOfEvents != OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_EVENTS + OTF2_UNKNOWN_RECORD_TEST_NUMBER_OF_UNKNOWN_RECORDS )
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
enter_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            regionID
)
{
    if ( time != expected_time )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Timestamp failure." );
    }
    expected_time++;

    if ( regionID != expected_region )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Region ID failure." );
    }
    expected_region++;

    return OTF2_CALLBACK_SUCCESS;
}

static inline OTF2_CallbackCode
leave_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            regionID
)
{
    if ( time != expected_time )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Timestamp failure." );
    }
    expected_time++;

    expected_region--;
    if ( regionID != expected_region )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Region ID failure." );
    }

    return OTF2_CALLBACK_SUCCESS;
}

static inline OTF2_CallbackCode
unknown_check
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes
)
{
    if ( time != expected_time )
    {
        check_status( OTF2_ERROR_INTEGRITY_FAULT, "Timestamp failure." );
    }
    expected_time++;

    unknown_records_read++;

    return OTF2_CALLBACK_SUCCESS;
}
/** @} */


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


/** @brief Get generic timestamp.
 *
 *  @return                 Returns consecutive timestamps 0,1,2 ...
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


/** @brief Check if status is not OTF2_SUCCESS.
 *
 *  Checks if status is not OTF2_SUCCESS. If so it prints an error with the
 *  passed description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param pointer          Status to be checked.
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


/** @brief Check if pointer is NULL.
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
