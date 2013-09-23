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
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Sets defined information in the archive and writes an anchor
 *              file. Then reads the anchor file back in and compares the read
 *              information with the defined information.
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>
#include <UTILS_IO.h>
#include <UTILS_CStr.h>

#include <otf2_internal.h>
#include <otf2_reader.h>
#include <otf2_archive.h>

/* ___ Global variables. ____________________________________________________ */



/** @brief Defines if debug is turned on (1) or off (0). */
static bool otf2_DEBUG;



/* ___ Prototypes for static functions. _____________________________________ */



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

static inline void
check_pointer( void* pointer,
               char* description,
               ... );

static inline void
check_status( OTF2_ErrorCode status,
              char*          description,
              ... );

static void
check_condition( bool  condition,
                 char* description,
                 ... );


/* ___ main _________________________________________________________________ */



int
main( void )
{
    char archive_path[] = "OTF2_AnchorFile_trace/trace";
    char archive_name[] = "archive.otf2";

    /* Information stored in the trace. */
    uint64_t           chunk_size_events = 1024 * 1024;
    uint64_t           chunk_size_defs   = 4 * 1024 * 1024;
    OTF2_FileSubstrate substrate         = OTF2_SUBSTRATE_POSIX;
    OTF2_Compression   compression       = OTF2_COMPRESSION_NONE;
    uint64_t           num_locations     = 123;
    uint64_t           num_definitions   = 456;
    char*              machine_name      = "TestMachine";
    char*              creator           = "TestCreator";
    char*              description       = "TestDescription";
    /* We will write only two properties. The third one will be deleted,
     * that means it won't be written to anchor file. */
    const char* const invalid_property_names[] =
    {
        "",              // empty
        "foo",           // no "::" at all
        "::foo",         // empty first component 1
        "::foo::bar",    // empty first component 2
        "foo::::",       // empty middle component 1
        "::::foo",       // empty middle component 2
        "foo::",         // empty last component 1
        "foo::bar::",    // empty last component 2
        "foo::ba ::baz", // invalid character 1
        "foo::bar::ba ", // invalid character 2
        NULL
    };
    char*    string_property_names[]     = { "NamespaceA::Property1", "NamespaceA::Property2", "NamespaceA::Property3" };
    char*    string_property_values[]    = { "Valid", "OK", "true" };
    char*    bool_property_names[]       = { "NamespaceB::BoolProperty1", "NamespaceB::BoolProperty2" };
    bool     bool_property_values[]      = { true, false };
    uint32_t number_of_string_properties = sizeof( string_property_names ) / sizeof( *string_property_names );
    uint32_t number_of_bool_properties   = sizeof( bool_property_names ) / sizeof( *bool_property_names );
    uint32_t number_of_properties        = number_of_string_properties + number_of_bool_properties;
    char*    property_name_to_delete     = "Whatever::Property";
    char*    arbitrary_property_value    = "Whatever::Value";
    char*    value_to_delete_property    = "";

    /* Print out info. */
    if ( otf2_DEBUG )
    {
        printf( "\n=== OTF2_ANCHORFILE_TEST [DEBUG MODE] ===\n\n" );
    }
    else
    {
        printf( "\n=== OTF2_ANCHORFILE_TEST ===\n\n" );
    }

    char* extension_dot = strrchr( archive_name, '.' );

    /* cut the extension from the archive_name when writing */
    *extension_dot = '\0';

    /* Create new archive handle. */
    OTF2_Archive* archive = OTF2_Archive_Open( archive_path,
                                               archive_name,
                                               OTF2_FILEMODE_WRITE,
                                               chunk_size_events,
                                               chunk_size_defs,
                                               substrate,
                                               compression );
    check_pointer( archive, "Open new archive." );


    OTF2_ErrorCode status = OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
    check_status( status, "Set flush callbacks." );

    /* Set number of locations. */
    status = otf2_archive_set_number_of_locations( archive, num_locations );
    check_status( status, "Set number of locations." );

    /* Set number of global definitions. */
    status = otf2_archive_set_number_of_global_defs( archive, num_definitions );
    check_status( status, "Set number of definitions." );

    /* Set machine name, creator and description (optional) */
    status = OTF2_Archive_SetMachineName( archive, machine_name );
    check_status( status, "Set machine name." );
    status = OTF2_Archive_SetCreator( archive, creator );
    check_status( status, "Set creator." );
    status = OTF2_Archive_SetDescription( archive, description );
    check_status( status, "Set description." );

    /* Set to master to write anchor file. */
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set master slave mode." );

    /* Check for some invalid property names */
    for ( const char* const* invalid_property_name_it = invalid_property_names;
          *invalid_property_name_it;
          invalid_property_name_it++ )
    {
        status = OTF2_Archive_SetProperty( archive,
                                           *invalid_property_name_it,
                                           "value",
                                           true );
        if ( status == OTF2_SUCCESS )
        {
            status = OTF2_ERROR_PROPERTY_NAME_INVALID;
        }
        else if ( status == OTF2_ERROR_PROPERTY_NAME_INVALID )
        {
            status = OTF2_SUCCESS;
        }
        check_status( status, "Test invalid property names failed for %s.", *invalid_property_name_it );
    }

    /* Write some additional meta-information to anchor file. */
    status = OTF2_Archive_SetProperty( archive,
                                       property_name_to_delete,
                                       value_to_delete_property,
                                       false );
    check_status( status, "Set empty string as value of a non-existing property should have no effect." );

    status = OTF2_Archive_SetProperty( archive,
                                       property_name_to_delete,
                                       arbitrary_property_value,
                                       false );
    check_status( status, "Write arbitrary property that will be deleted later on." );
    status = OTF2_Archive_SetProperty( archive,
                                       property_name_to_delete,
                                       value_to_delete_property,
                                       true );
    check_status( status, "Delete property at the front of the list." );

    status = OTF2_Archive_SetProperty( archive,
                                       string_property_names[ 0 ],
                                       arbitrary_property_value,
                                       false );
    check_status( status, "Write first property." );

    status = OTF2_Archive_SetProperty( archive,
                                       string_property_names[ 1 ],
                                       string_property_values[ 1 ],
                                       false );
    check_status( status, "Write second property." );

    status = OTF2_Archive_SetProperty( archive,
                                       string_property_names[ 0 ],
                                       string_property_values[ 0 ],
                                       true );
    check_status( status, "Re-write first property." );

    status = OTF2_Archive_SetProperty( archive,
                                       property_name_to_delete,
                                       arbitrary_property_value,
                                       false );
    check_status( status, "Write arbitrary property that will be deleted later on." );

    status = OTF2_Archive_SetProperty( archive,
                                       string_property_names[ 2 ],
                                       string_property_values[ 2 ],
                                       false );
    check_status( status, "Write third property." );

    status = OTF2_Archive_SetProperty( archive,
                                       property_name_to_delete,
                                       value_to_delete_property,
                                       true );
    check_status( status, "Delete property somewhere in the middle of the list." );

    status = OTF2_Archive_SetProperty( archive,
                                       property_name_to_delete,
                                       arbitrary_property_value,
                                       false );
    check_status( status, "Write arbitrary property that will be deleted later on." );
    status = OTF2_Archive_SetProperty( archive,
                                       property_name_to_delete,
                                       value_to_delete_property,
                                       true );
    check_status( status, "Delete   property at the end of the list." );

    status = OTF2_Archive_SetBoolProperty( archive,
                                           bool_property_names[ 0 ],
                                           bool_property_values[ 0 ],
                                           false );
    check_status( status, "Write fourth property (first one of type boolean)." );

    status = OTF2_Archive_SetBoolProperty( archive,
                                           bool_property_names[ 1 ],
                                           bool_property_values[ 1 ],
                                           false );
    check_status( status, "Write fifth property (second one of type boolean)." );

    /* Set to master to write anchor file. */
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set master slave mode." );

    /* Set to master to write anchor file. */
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set master slave mode." );

    /* Close archive. */
    status = OTF2_Archive_Close( archive );
    check_status( status, "Close archive." );




    /* Open new archive. */
    /* add the extension back to the archive_name when reading */
    *extension_dot = '.';

    char* full_path = UTILS_IO_JoinPath( 2, archive_path, archive_name );
    check_pointer( full_path, "Could not create full path to anchor file." );

    OTF2_Reader* reader = OTF2_Reader_Open( full_path );
    check_pointer( reader, "Open new reader for reading." );

    free( full_path );

    /* Information read from the trace. */
    uint8_t            version_read[ 3 ];
    uint64_t           num_locations_read;
    uint64_t           num_definitions_read;
    uint64_t           chunk_size_events_read;
    uint64_t           chunk_size_defs_read;
    OTF2_FileSubstrate substrate_read;
    OTF2_Compression   compression_read;
    char*              machine_name_read;
    char*              creator_read;
    char*              description_read;
    char**             property_names_read;
    char*              string_property_value_read;
    bool               bool_property_value_read;
    uint32_t           number_of_properties_read;



    /* Get version. */
    status = OTF2_Reader_GetVersion( reader,
                                     &version_read[ 0 ],
                                     &version_read[ 1 ],
                                     &version_read[ 2 ] );
    check_status( status, "Get version." );
    check_condition( OTF2_VERSION_MAJOR == version_read[ 0 ],
                     "Major version does not match: "
                     "Expected %u. Got %u.",
                     OTF2_VERSION_MAJOR, version_read[ 0 ] );
    check_condition( OTF2_VERSION_MINOR == version_read[ 1 ],
                     "Minor version does not match: "
                     "Expected %u. Got %u.",
                     OTF2_VERSION_MINOR, version_read[ 1 ] );
    check_condition( OTF2_VERSION_BUGFIX == version_read[ 2 ],
                     "Bugfix version does not match: "
                     "Expected %u. Got %u.",
                     OTF2_VERSION_BUGFIX, version_read[ 2 ] );

    /* Get number of locations. */
    status = OTF2_Reader_GetNumberOfLocations( reader,
                                               &num_locations_read );
    check_status( status, "Get number of locations." );
    check_condition( num_locations == num_locations_read,
                     "Number of locations does not match: "
                     "Expected %" PRIu64 ". Got %" PRIu64 ".",
                     num_locations, num_locations_read );

    /* Get number of definitions. */
    status = OTF2_Reader_GetNumberOfGlobalDefinitions( reader,
                                                       &num_definitions_read );
    check_status( status, "Get number of definitions." );
    check_condition( num_definitions == num_definitions_read,
                     "Number of definitions does not match: "
                     "Expected %" PRIu64 ". Got %" PRIu64 ".",
                     num_definitions, num_definitions_read );

    /* Get chunk sizes. */
    status = OTF2_Reader_GetChunkSize( reader,
                                       &chunk_size_events_read,
                                       &chunk_size_defs_read );
    check_status( status, "Get chuck sizes." );
    check_condition( chunk_size_events == chunk_size_events_read,
                     "Event chunk size does not match: "
                     "Expected %" PRIu64 ". Got %" PRIu64 ".",
                     chunk_size_events, chunk_size_events_read );
    check_condition( chunk_size_defs == chunk_size_defs_read,
                     "Definition chunk size does not match: "
                     "Expected %" PRIu64 ". Got %" PRIu64 ".",
                     chunk_size_defs, chunk_size_defs_read );

    /* Get substrate. */
    status = OTF2_Reader_GetFileSubstrate( reader,
                                           &substrate_read );
    check_status( status, "Get file substrate." );
    check_condition( substrate == substrate_read,
                     "File subtrate does not match: Expected %u. Got %u.",
                     substrate, substrate_read );

    /* Get compression. */
    status = OTF2_Reader_GetCompression( reader,
                                         &compression_read );
    check_status( status, "Get compression." );
    check_condition( compression == compression_read,
                     "Compression does not match: Expected %u. Got %u.",
                     compression, compression_read );

    /* Get machine name */
    status = OTF2_Reader_GetMachineName( reader, &machine_name_read );
    check_status( status, "Get machine name." );
    check_condition( 0 == strcmp( machine_name, machine_name_read ),
                     "Machine name does not match: Expected %s. Got %s.",
                     machine_name, machine_name_read );
    free( machine_name_read );

    status = OTF2_Reader_GetCreator( reader, &creator_read );
    check_status( status, "Get creator." );
    check_condition( 0 == strcmp( creator, creator_read ),
                     "Creator does not match: Expected %s. Got %s.",
                     creator, creator_read );
    free( creator_read );

    status = OTF2_Reader_GetDescription( reader, &description_read );
    check_status( status, "Get description." );
    check_condition( 0 == strcmp( description, description_read ),
                     "Description does not match: Expected %s. Got %s.",
                     description, description_read );
    free( description_read );

    /* Get additional meta-information (a.k.a. properties) */
    status = OTF2_Reader_GetPropertyNames( reader, &number_of_properties_read, &property_names_read );
    check_status( status, "Get number of properties." );

    /* Check number of properties */
    check_condition( number_of_properties == number_of_properties_read,
                     "Number of properties does not match: Expected %u. Got %u.",
                     number_of_properties, number_of_properties_read );

    char* converted_name;
    for ( uint32_t i = 0; i < number_of_properties_read; i++ )
    {
        /*
         * First, this test writes #number_of_string_properties properties of type
         * string to the anchor file. Afterwards, #number_of_bool_properties are
         * written to the anchor file.
         */

        /* Property names are store in upper case. If everything works correctly,
         * OTF2_Reader_GetProperty will return our property names converted from
         * mixed case to upper case. Therefore, we also have to convert property
         * names to upper case. */
        if ( i < number_of_string_properties )
        {
            converted_name = UTILS_CStr_dup( string_property_names[ i ] );
        }
        else
        {
            converted_name = UTILS_CStr_dup( bool_property_names[ i - number_of_string_properties ] );
        }
        for ( char* ptr = converted_name; *ptr; ptr++ )
        {
            *ptr = toupper( *ptr );
        }

        /* Check name of property */
        check_condition( 0 == strcmp( converted_name, property_names_read[ i ] ),
                         "Property name does not match: Expected %s. Got %s.",
                         converted_name, property_names_read[ i ] );
        free( converted_name );

        if ( i < number_of_string_properties )
        {
            /* Check property value (value is of type string) */
            status = OTF2_Reader_GetProperty( reader, property_names_read[ i ], &string_property_value_read );
            check_status( status, "Get property value." );
            check_condition( 0 == strcmp( string_property_values[ i ], string_property_value_read ),
                             "Property value does not match: Expected %s. Got %s.",
                             string_property_values[ i ], string_property_value_read );
            free( string_property_value_read );
        }
        else
        {
            /* Check property value (value is of type boolean) */
            status = OTF2_Reader_GetBoolProperty( reader,
                                                  property_names_read[ i ],
                                                  &bool_property_value_read );
            check_status( status, "Get boolean property value." );
            check_condition( bool_property_values[ i - number_of_string_properties ] == bool_property_value_read,
                             "Property value does not match: Expected %s. Got %s.",
                             ( bool_property_values[ i - number_of_string_properties ] ) ? "true" : "false",
                             ( bool_property_value_read ) ? "true" : "false" );
        }
    }
    free( property_names_read );

    /* Close reader. */
    status = OTF2_Reader_Close( reader );
    check_status( status, "Close reader." );

    return EXIT_SUCCESS;
}



/* ___ Implementation of static functions ___________________________________ */



/** @brief Pre flush callback.
 *
 *  @param evtWriter        Ignored.
 *  @param evtReader        Ignored.
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


/** @brief Get generic timestamp.
 *
 *  @return                 Returns consecutive timestamps 0,1,2 ...
 */
static inline uint64_t
get_time( void )
{
    static uint64_t sequence;
    return sequence++;
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
check_pointer( void* pointer,
               char* description,
               ... )
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
check_status( OTF2_ErrorCode status,
              char*          description,
              ... )
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


/** @internal
 *  @brief Check if condition is true. Otherwise fail.
 *
 *  Checks if condition is true. If not it prints an error with the
 *  passed description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param condition        Condition which must hold true.
 *  @param description      Description for error/debug message.
 *  @param ...              Variable argument list like e.g. in printf.
 */
void
check_condition( bool  condition,
                 char* description,
                 ... )
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

    if ( otf2_DEBUG )
    {
        printf( "==DEBUG== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
    }

    va_end( va );
}
