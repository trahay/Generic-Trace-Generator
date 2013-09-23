/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
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

/**
 *  @file       otf2_print.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This tool prints out all event files of an archive to console.
 *
 *  @return                  Returns EXIT_SUCCESS if successful, EXIT_FAILURE
 *                           if an error occures.
 */

#include <config.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>
#include <assert.h>

#include "otf2/otf2.h"

#include "otf2_hash_table.h"
#include "otf2_vector.h"



/* ___ Shorthand macros. ____________________________________________________ */


#define PRIUint8      PRIu8
#define PRIInt8       PRId8
#define PRIUint16     PRIu16
#define PRIInt16      PRId16
#define PRIUint32     PRIu32
#define PRIInt32      PRId32
#define PRIUint64     PRIu64
#define PRIUint64Full PRIu64
#define PRIInt64      PRId64
#define PRIFloat      "f"
#define PRIDouble     "f"

#define BUFFER_SIZE 128

/* ___ Global variables. ____________________________________________________ */


/** @internal
 *  @brief Name of the program. */
static const char* otf2_NAME;

/** @internal
 *  @brief Defines if debug is turned on (1) or off (0). */
static bool otf2_DEBUG;

/** @internal
 *  @brief Defines if all data is printed (1) or not (0). */
static bool otf2_ALL;

/** @internal
 *  @brief Defines if global definitions are printed (1) or not (0). */
static bool otf2_GLOBDEFS;

/** @internal
 *  @brief Defines if information from anchor file are printed (1) or not (0). */
static bool otf2_ANCHORFILE_INFO;

/** @internal
 *  @brief Defines if thumbnail headers should be printed. */
static bool otf2_THUMBNAIL_INFO;

/** @internal
 *  @brief Defines if thumbnail headers should be printed. */
static bool otf2_THUMBNAIL_SAMPLES;

/** @internal
 *  @brief Defines if a single location is selected. */
static uint64_t otf2_LOCAL = OTF2_UNDEFINED_LOCATION;

/** @internal
 *  @brief Tell if a local location was found (1) or not (0). */
static bool otf2_LOCAL_FOUND;

/** @internal
 *  @brief Defines lower bound of selected time interval. */
static uint64_t otf2_MINTIME;

/** @internal
 *  @brief Defines upper bound of selected time interval. */
static uint64_t otf2_MAXTIME = OTF2_UNDEFINED_UINT64;

/** @internal
 *  @brief Defines number of printed events in each step (UINT64_MAX means unlimited). */
static uint64_t otf2_STEP = OTF2_UNDEFINED_UINT64;

/** @internal
 *  @brief Defines if events are printed or not. */
static bool otf2_SILENT;

/** @internal
 *  @brief Defines if dot output is selected. */
static bool otf2_DOT;

/** @internal
 *  @brief Defines if we want to see the mapping tables. */
static bool otf2_MAPPINGS;

/** @internal
 *  @brief Defines if we want to see timer synchronizations. */
static bool otf2_CLOCK_OFFSETS;

/** @internal
 *  @brief Don't read local defs, to prevent the reader to apply mappings
 *         and clock corrections. */
static bool otf2_NOLOCALDEFS;

/** @internal
 *  @brief Print also any snapshots. */
static bool otf2_NOSNAPSHOTS;

/** @internal
 *  @brief width of the column with the anchor file information. */
static int otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH = 30;


/** @internal
 *  @brief width of the column with the anchor file information. */
static int otf2_THUMBNAIL_HEADER_COLUMN_WIDTH = 16;


struct otf2_print_data;
struct otf2_print_defs;
struct otf2_print_def_name;

/* ___ Structures. __________________________________________________________ */


/** @internal
 *  @brief Keeps all data for the callbacks. */
struct otf2_print_data
{
    /** @brief Reader handle. */
    OTF2_Reader* reader;
    /** @brief List of locations to process. */
    otf2_vector* locations_to_read;

    /** @brief Collected definitions. */
    struct otf2_print_defs* defs;

    /** @brief File handle for dot output. */
    FILE* dot_file;
};

/** @internal
 *  @brief Region definition element. */
struct otf2_print_def_name
{
    /** @brief The ID of the definition. */
    uint64_t def_id;
    /** @brief The name if the definition. */
    char*    name;
};


/* ___ Prototypes for static functions. _____________________________________ */

static void
otf2_print_die( const char* fmt,
                ... )
{
    if ( fmt )
    {
        va_list va;
        fprintf( stderr, "%s: ", otf2_NAME );
        va_start( va, fmt );
        vfprintf( stderr, fmt, va );
        va_end( va );
    }
    fprintf( stderr, "Try '%s --help' for more information.\n", otf2_NAME );
    exit( EXIT_FAILURE );
}

static void
otf2_print_anchor_file_information( OTF2_Reader* reader );

static void
otf2_print_thumbnails( OTF2_Reader* reader );

static void
otf2_get_parameters( int    argc,
                     char** argv,
                     char** anchorFile );

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

static void
otf2_print_add_location_to_read( struct otf2_print_data* data,
                                 OTF2_LocationRef        locationID );

static void
otf2_print_add_string( otf2_hash_table* strings,
                       uint32_t         stringID,
                       const char*      content );

static void
otf2_print_add_def64_name( const char*      def_name,
                           otf2_hash_table* defs,
                           otf2_hash_table* strings,
                           uint64_t         defID,
                           uint32_t         stringID );

static char*
otf2_print_get_buffer( size_t len );

static const char*
otf2_print_get_id64( uint64_t ID );

static const char*
otf2_print_get_name( const char* name,
                     uint64_t    ID );

static const char*
otf2_print_get_def64_name( const otf2_hash_table* defs,
                           uint64_t               defID );

static const char*
otf2_print_get_def_name( const otf2_hash_table* defs,
                         uint32_t               defID );

static char*
otf2_print_get_string( const otf2_hash_table* strings,
                       uint32_t               stringID );

static void
print_attribute_list( struct otf2_print_data* data,
                      OTF2_AttributeList*     attributes );

static const char*
otf2_print_get_invalid( uint64_t ID );


#include "otf2_print_types.h"


/* ___ Prototypes for event callbacks. ______________________________________ */


static OTF2_CallbackCode
print_unknown( OTF2_LocationRef    locationID,
               OTF2_TimeStamp      time,
               void*               userData,
               OTF2_AttributeList* attributes );


static OTF2_CallbackCode
print_global_def_unknown( void* userData );


#include "otf2_print_inc.c"


static OTF2_CallbackCode
print_def_mapping_table( void*             userData,
                         OTF2_MappingType  mapType,
                         const OTF2_IdMap* iDMap );

static OTF2_CallbackCode
print_def_clock_offset( void*    userData,
                        uint64_t time,
                        int64_t  offset,
                        double   stddev );


/* ___ main _________________________________________________________________ */


int
main( int    argc,
      char** argv )
{
    otf2_NAME = strrchr( argv[ 0 ], '/' );
    if ( otf2_NAME )
    {
        otf2_NAME++;
    }
    else
    {
        otf2_NAME = argv[ 0 ];
    }

    char* anchor_file = NULL;
    otf2_get_parameters( argc, argv, &anchor_file );

    if ( otf2_NOLOCALDEFS && ( otf2_MAPPINGS || otf2_CLOCK_OFFSETS ) )
    {
        otf2_print_die( "--no-local-defs is mutual exclusive to --show-mappings and --show-clock-offsets\n" );
    }

    printf( "\n=== OTF2-PRINT ===\n" );

    /* Get a reader handle. */
    OTF2_Reader* reader = OTF2_Reader_Open( anchor_file );
    check_pointer( reader, "Create new reader handle." );

    if ( otf2_ANCHORFILE_INFO )
    {
        otf2_print_anchor_file_information( reader );
    }


    if ( otf2_THUMBNAIL_INFO )
    {
        otf2_print_thumbnails( reader );
    }

    /* Only exit if --show-info was given. */
    if ( ( otf2_ANCHORFILE_INFO || otf2_THUMBNAIL_INFO ) && !otf2_ALL )
    {
        OTF2_Reader_Close( reader );

        /* This is just to add a message to the debug output. */
        check_status( OTF2_SUCCESS, "Delete reader handle." );
        check_status( OTF2_SUCCESS, "Programm finished successfully." );

        return EXIT_SUCCESS;
    }
/* ___ Read Global Definitions _______________________________________________*/

    uint32_t       number_of_snapshots;
    OTF2_ErrorCode status = OTF2_Reader_GetNumberOfSnapshots( reader, &number_of_snapshots );
    check_status( status, "Read number of snapshots." );

    /* Add a nice table header. */
    if ( otf2_GLOBDEFS )
    {
        printf( "\n" );
        printf( "=== Global Definitions =========================================================" );
        printf( "\n\n" );
        printf( "%-*s %12s  Attributes\n", otf2_DEF_COLUMN_WIDTH, "Definition", "ID" );
        printf( "--------------------------------------------------------------------------------\n" );
    }
    /* Define definition callbacks. */
    OTF2_GlobalDefReaderCallbacks* def_callbacks = otf2_print_create_global_def_callbacks();

    /* Get number of locations from the anchor file. */
    uint64_t num_locations = 0;
    status = OTF2_SUCCESS;
    status = OTF2_Reader_GetNumberOfLocations( reader, &num_locations );
    check_status( status, "Get number of locations. Number of locations: %" PRIu64,
                  num_locations );


    /* User data for callbacks. */
    struct otf2_print_data user_data;
    struct otf2_print_defs user_defs;
    user_data.reader            = reader;
    user_data.locations_to_read = otf2_vector_create();
    user_data.defs              = &user_defs;
    otf2_print_def_create_hash_tables( user_data.defs );
    user_data.dot_file = NULL;


    /* If in dot output mode open dot file. */
    char dot_path[ 1024 ] = "";
    if ( otf2_DOT )
    {
        sprintf( dot_path, "%.*s.SystemTree.dot", ( int )strlen( anchor_file ) - 5, anchor_file );

        user_data.dot_file = fopen( dot_path, "w" );
        if ( user_data.dot_file == NULL )
        {
            fprintf( stderr,
                     "%s: cannot open dot file for system tree\n",
                     otf2_NAME );
            return EXIT_FAILURE;
        }

        fprintf( user_data.dot_file, "/* This is the graph representation of the system tree. */\n" );
        fprintf( user_data.dot_file, "digraph SystemTree\n" );
        fprintf( user_data.dot_file, "{\n" );
    }


    /* Read global definitions. */
    uint64_t              definitions_read  = 0;
    OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
    check_pointer( global_def_reader, "Create global definition reader handle." );

    status = OTF2_Reader_RegisterGlobalDefCallbacks( reader, global_def_reader,
                                                     def_callbacks,
                                                     &user_data );
    check_status( status, "Register global definition callbacks." );
    OTF2_GlobalDefReaderCallbacks_Delete( def_callbacks );

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

    /* Check if a requested local trace file was found. */
    if ( otf2_LOCAL == OTF2_UNDEFINED_LOCATION )
    {
        check_condition( otf2_LOCAL_FOUND, "Find location definitions in global definition file." );
    }
    else
    {
        check_condition( otf2_LOCAL_FOUND, "Find event file for selected location." );
    }

    OTF2_DefReaderCallbacks* local_def_callbacks = OTF2_DefReaderCallbacks_New();
    check_pointer( def_callbacks, "Create global definition callback handle." );
    if ( otf2_MAPPINGS )
    {
        OTF2_DefReaderCallbacks_SetMappingTableCallback( local_def_callbacks, print_def_mapping_table );
    }
    if ( otf2_CLOCK_OFFSETS )
    {
        OTF2_DefReaderCallbacks_SetClockOffsetCallback( local_def_callbacks, print_def_clock_offset );
    }

    /* Open a new local event reader for each found location ID. */
    if ( otf2_MAPPINGS || otf2_CLOCK_OFFSETS )
    {
        printf( "\n" );
        printf( "=== Per Location Definitions ===================================================" );
        printf( "\n\n" );
        printf( "%-*s %12s  Attributes\n", otf2_DEF_COLUMN_WIDTH, "Definition", "Location" );
        printf( "--------------------------------------------------------------------------------\n" );
    }

    for ( size_t i = 0; i < otf2_vector_size( user_data.locations_to_read ); i++ )
    {
        uint64_t* location_item      = otf2_vector_at( user_data.locations_to_read, i );
        uint64_t  locationIdentifier = *location_item;

        /* Do not open the event reader, when only showing the global defs */
        if ( !otf2_GLOBDEFS || otf2_ALL )
        {
            OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( reader,
                                                                   locationIdentifier );
            check_pointer( evt_reader, "Create local event reader for location %" PRIu64 ".",
                           locationIdentifier );
        }

        if ( !otf2_NOSNAPSHOTS && number_of_snapshots > 0 )
        {
            OTF2_SnapReader* snap_reader = OTF2_Reader_GetSnapReader( reader,
                                                                      locationIdentifier );
            check_pointer( snap_reader, "Create local snapshot reader for location %" PRIu64 ".",
                           locationIdentifier );
        }

        if ( otf2_NOLOCALDEFS || ( otf2_GLOBDEFS && !otf2_ALL ) )
        {
            continue;
        }

        /* Also open a definition reader and read all local definitions. */
        OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( reader,
                                                               locationIdentifier );
        /* a local def file is not mandatory */
        if ( def_reader )
        {
            if ( otf2_MAPPINGS || otf2_CLOCK_OFFSETS )
            {
                status = OTF2_Reader_RegisterDefCallbacks( reader,
                                                           def_reader,
                                                           local_def_callbacks,
                                                           &locationIdentifier );
                check_status( status, "Register local definition callbacks." );
            }

            uint64_t definitions_read = 0;
            status = OTF2_SUCCESS;
            do
            {
                uint64_t def_reads = 0;
                status = OTF2_Reader_ReadAllLocalDefinitions( reader,
                                                              def_reader,
                                                              &def_reads );
                definitions_read += def_reads;

                /* continue reading, if we have an duplicate mapping table */
                if ( OTF2_ERROR_DUPLICATE_MAPPING_TABLE != status )
                {
                    break;
                }
            }
            while ( true );
            check_status( status,
                          "Read %" PRIu64 " definitions for location %" PRIu64,
                          definitions_read,
                          locationIdentifier );

            /* Close def reader, it is no longer useful and occupies memory */
            status = OTF2_Reader_CloseDefReader( reader, def_reader );
            check_status( status, "Close local definition reader." );
        }
    }
    OTF2_DefReaderCallbacks_Delete( local_def_callbacks );


    /* If in dot output mode close dot file and terminate. */
    if ( otf2_DOT )
    {
        fprintf( user_data.dot_file, "}\n" );
        fclose( user_data.dot_file );

        printf( "\nGenerate system tree dot graph for \"%s\".\n\n", anchor_file );
        printf( "Dot file written to \"%s\".\n\n", dot_path );
        printf( "To generate an image from the dot file run:\n" );
        printf( "\"dot -Tpng %s -o SystemTree.png\"\n\n", dot_path );

        OTF2_Reader_Close( reader );

        /* This is just to add a message to the debug output. */
        check_status( OTF2_SUCCESS, "Delete reader handle." );
        check_status( OTF2_SUCCESS, "Programm finished successfully." );

        return EXIT_SUCCESS;
    }

    if ( ( otf2_GLOBDEFS || otf2_MAPPINGS || otf2_CLOCK_OFFSETS ) && !otf2_ALL )
    {
        OTF2_Reader_Close( reader );

        /* This is just to add a message to the debug output. */
        check_status( OTF2_SUCCESS, "Delete reader handle." );
        check_status( OTF2_SUCCESS, "Programm finished successfully." );

        return EXIT_SUCCESS;
    }



/* ___ Read Event Records ____________________________________________________*/



    /* Add a nice table header. */
    if ( !otf2_SILENT )
    {
        printf( "=== Events =====================================================================\n" );
    }

    /* Define event callbacks. */
    OTF2_GlobalEvtReaderCallbacks* evt_callbacks = otf2_print_create_global_evt_callbacks();

    /* Get global event reader. */
    OTF2_GlobalEvtReader* global_evt_reader = OTF2_Reader_GetGlobalEvtReader( reader );
    check_pointer( global_evt_reader, "Create global event reader." );


    /* Register the above defined callbacks to the global event reader. */
    if ( !otf2_SILENT )
    {
        status = OTF2_Reader_RegisterGlobalEvtCallbacks( reader,
                                                         global_evt_reader,
                                                         evt_callbacks,
                                                         &user_data );
        check_status( status, "Register global event callbacks." );
    }
    OTF2_GlobalEvtReaderCallbacks_Delete( evt_callbacks );


    /* Read until events are all read. */
    uint64_t events_read = otf2_STEP;

    while ( events_read == otf2_STEP )
    {
        if ( !otf2_SILENT )
        {
            printf( "%-*s %15s %20s  Attributes\n",
                    otf2_EVENT_COLUMN_WIDTH, "Event", "Location", "Timestamp" );
            printf( "--------------------------------------------------------------------------------\n" );
        }

        status = OTF2_Reader_ReadGlobalEvents( reader,
                                               global_evt_reader,
                                               otf2_STEP,
                                               &events_read );
        check_status( status, "Read %" PRIu64 " events.", events_read );

        /* Step through output if otf2_STEP is defined. */
        if ( otf2_STEP != OTF2_UNDEFINED_UINT64 )
        {
            printf( "Press ENTER to print next %" PRIu64 " events.", otf2_STEP );
            getchar();
        }
    }

/* ___ Read Snapshot Records ____________________________________________________*/



    if ( !otf2_NOSNAPSHOTS && number_of_snapshots > 0 )
    {
        /* Add a nice table header. */
        if ( !otf2_SILENT )
        {
            printf( "=== Snapshots ==================================================================\n" );
        }

        /* Define snapshot callbacks. */
        OTF2_GlobalSnapReaderCallbacks* snap_callbacks = otf2_print_create_global_snap_callbacks();

        /* Get global snapshots reader. */
        OTF2_GlobalSnapReader* global_snap_reader = OTF2_Reader_GetGlobalSnapReader( reader );
        check_pointer( global_snap_reader, "Create global snapshots reader." );


        /* Register the above defined callbacks to the global snapshots reader. */
        if ( !otf2_SILENT )
        {
            status = OTF2_Reader_RegisterGlobalSnapCallbacks( reader,
                                                              global_snap_reader,
                                                              snap_callbacks,
                                                              &user_data );
            check_status( status, "Register global snapshots callbacks." );
        }
        OTF2_GlobalSnapReaderCallbacks_Delete( snap_callbacks );


        /* Read until snapshots are all read. */
        uint64_t records_read = otf2_STEP;
        while ( records_read == otf2_STEP )
        {
            if ( !otf2_SILENT )
            {
                printf( "%-*s %15s %20s  Attributes\n",
                        otf2_EVENT_COLUMN_WIDTH, "Snapshot", "Location", "Timestamp" );
                printf( "--------------------------------------------------------------------------------\n" );
            }

            status = OTF2_Reader_ReadGlobalSnapshots( reader,
                                                      global_snap_reader,
                                                      otf2_STEP,
                                                      &records_read );
            check_status( status, "Read %" PRIu64 " snapshot records.", records_read );

            /* Step through output if otf2_STEP is defined. */
            if ( otf2_STEP != OTF2_UNDEFINED_UINT64 )
            {
                printf( "Press ENTER to print next %" PRIu64 " snapshot records.", otf2_STEP );
                getchar();
            }
        }
    }

    OTF2_Reader_Close( reader );

    /* This is just to add a message to the debug output. */
    check_status( OTF2_SUCCESS, "Delete reader handle." );
    check_status( OTF2_SUCCESS, "Programm finished successfully." );

    otf2_vector_foreach( user_data.locations_to_read, free );
    otf2_vector_free( user_data.locations_to_read );
    otf2_print_def_destroy_hash_tables( user_data.defs );

    return EXIT_SUCCESS;
}



/* ___ Implementation of static functions ___________________________________ */

static void
otf2_parse_number_argument( const char* option,
                            const char* argument,
                            uint64_t*   number )
{
    const char* p = argument;
    *number = 0;
    while ( *p )
    {
        if ( ( *p < '0' ) || ( *p > '9' ) )
        {
            otf2_print_die( "invalid number argument for %s: %s\n",
                            option, argument );
        }
        uint64_t new_number = *number * 10 + *p - '0';
        if ( new_number < *number )
        {
            otf2_print_die( "number argument to large for '%s': %s\n",
                            option, argument );
        }
        *number = new_number;
        p++;
    }
    if ( p == argument )
    {
        otf2_print_die( "empty number argument for '%s'\n", option );
    }
}

/** @internal
 *  @brief Get command line parameters.
 *
 *  Parses command line parameters and checks for their existence.
 *  Prints help for parameters '-h' or '--help'.
 *
 *  @param argc             Programs argument counter.
 *  @param argv             Programs argument values.
 *  @param anchorFile       Return pointer for the anchor file path.
 */
void
otf2_get_parameters( int    argc,
                     char** argv,
                     char** anchorFile )
{
    bool process_options = true;
    int  i;
    for ( i = 1; process_options && i < argc; i++ )
    {
        if ( !strcmp( argv[ i ], "--help" ) || !strcmp( argv[ i ], "-h" ) )
        {
            printf(
                #include "otf2_print_usage.h"
                "\n"
                "Report bugs to <%s>\n",
                PACKAGE_BUGREPORT );
            exit( EXIT_SUCCESS );
        }

        else if ( !strcmp( argv[ i ], "--version" ) || !strcmp( argv[ i ], "-V" ) )
        {
            printf( "%s: version %s\n", otf2_NAME, OTF2_VERSION );
            exit( EXIT_SUCCESS );
        }

        else if ( !strcmp( argv[ i ], "--debug" ) || !strcmp( argv[ i ], "-d" ) )
        {
            otf2_DEBUG = true;
        }

        /* Check for requested system tree dot output. */
        else if ( !strcmp( argv[ i ], "--system-tree" ) )
        {
            otf2_DOT = true;
        }

        else if ( !strcmp( argv[ i ], "--show-all" ) || !strcmp( argv[ i ], "-A" ) )
        {
            otf2_ANCHORFILE_INFO = true;
            otf2_THUMBNAIL_INFO  = true;
            otf2_ALL             = true;
            otf2_GLOBDEFS        = true;
        }

        else if ( !strcmp( argv[ i ], "--show-global-defs" ) || !strcmp( argv[ i ], "-G" ) )
        {
            otf2_GLOBDEFS = true;
        }

        else if ( !strcmp( argv[ i ], "--show-info" ) || !strcmp( argv[ i ], "-I" ) )
        {
            otf2_ANCHORFILE_INFO = true;
        }

        else if ( !strcmp( argv[ i ], "--show-thumbnails" ) || !strcmp( argv[ i ], "-T" ) )
        {
            otf2_THUMBNAIL_INFO = true;
        }

        else if ( !strcmp( argv[ i ], "--show-thumbnail-samples" ) )
        {
            otf2_THUMBNAIL_INFO    = true;
            otf2_THUMBNAIL_SAMPLES = true;
        }

        else if ( !strcmp( argv[ i ], "--show-mappings" ) || !strcmp( argv[ i ], "-M" ) )
        {
            otf2_MAPPINGS = true;
        }

        else if ( !strcmp( argv[ i ], "--show-clock-offsets" ) || !strcmp( argv[ i ], "-C" ) )
        {
            otf2_CLOCK_OFFSETS = true;
        }

        else if ( !strcmp( argv[ i ], "--no-local-defs" ) )
        {
            otf2_NOLOCALDEFS = true;
        }

        else if ( !strcmp( argv[ i ], "--no-snapshots" ) )
        {
            otf2_NOSNAPSHOTS = true;
        }

        else if ( !strcmp( argv[ i ], "--silent" ) )
        {
            otf2_SILENT = true;
        }

        else if ( !strcmp( argv[ i ], "--location" )
                  || !strncmp( argv[ i ], "--location=", 11 )
                  || !strcmp( argv[ i ], "-L" ) )
        {
            char* opt = argv[ i ];
            char* arg;
            if ( opt[ 1 ] == '-' && opt[ 10 ] == '=' )
            {
                opt[ 10 ] = '\0';
                arg       = &opt[ 11 ];
            }
            else
            {
                if ( i + 1 >= argc )
                {
                    otf2_print_die( "missing argument for '%s'\n", opt );
                }
                arg = argv[ i + 1 ];
                i++;
            }

            otf2_parse_number_argument( opt, arg, &otf2_LOCAL );
        }

        else if ( !strcmp( argv[ i ], "--time" )
                  || !strncmp( argv[ i ], "--time=", 7 ) )
        {
            char* opt = argv[ i ];
            char* arg1;
            char* arg2;
            if ( opt[ 6 ] == '=' )
            {
                opt[ 6 ] = '\0';
                arg1     = &opt[ 7 ];
                arg2     = strchr( arg1, ',' );
                if ( !arg2 )
                {
                    otf2_print_die( "missing argument for '%s'\n", opt );
                }
                *arg2++ = '\0';
            }
            else
            {
                if ( i + 2 >= argc )
                {
                    otf2_print_die( "missing argument for '%s'\n", opt );
                }
                arg1 = argv[ i + 1 ];
                arg2 = argv[ i + 2 ];
                i   += 2;
            }

            otf2_parse_number_argument( opt, arg1, &otf2_MINTIME );
            otf2_parse_number_argument( opt, arg2, &otf2_MAXTIME );
        }

        else if ( !strcmp( argv[ i ], "--step" )
                  || !strncmp( argv[ i ], "--step=", 7 )
                  || !strcmp( argv[ i ], "-s" ) )
        {
            char* opt = argv[ i ];
            char* arg;
            if ( opt[ 1 ] == '-' && opt[ 6 ] == '=' )
            {
                opt[ 6 ] = '\0';
                arg      = &opt[ 7 ];
            }
            else
            {
                if ( i + 1 >= argc )
                {
                    otf2_print_die( "missing argument for '%s'\n", opt );
                }
                arg = argv[ i + 1 ];
                i++;
            }

            otf2_parse_number_argument( opt, arg, &otf2_STEP );
        }

        else if ( !strcmp( argv[ i ], "--" ) )
        {
            process_options = false;
        }

        else if ( argv[ i ][ 0 ] == '-' )
        {
            otf2_print_die( "unrecognized option '%s'\n", argv[ 1 ] );
        }

        else
        {
            break;
        }
    }

    if ( 1 != argc - i )
    {
        otf2_print_die( "missing or too many anchorfile argument(s)\n" );
    }

    *anchorFile = argv[ i ];
}

static void
otf2_print_anchor_file_information( OTF2_Reader* reader )
{
    OTF2_ErrorCode status;

    uint8_t major;
    uint8_t minor;
    uint8_t bugfix;
    status = OTF2_Reader_GetVersion( reader, &major, &minor, &bugfix );
    check_status( status, "Read version." );

    printf( "\nContent of OTF2 anchor file:\n" );
    printf( "%-*s %u.%u.%u\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Version",
            major, minor, bugfix );

    uint64_t chunk_size_events;
    uint64_t chunk_size_definitions;
    status = OTF2_Reader_GetChunkSize( reader,
                                       &chunk_size_events,
                                       &chunk_size_definitions );
    check_status( status, "Read chunk size." );

    printf( "%-*s %" PRIu64 "\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Chunk size events",
            chunk_size_events );
    printf( "%-*s %" PRIu64 "\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Chunk size definitions",
            chunk_size_definitions );

    OTF2_FileSubstrate substrate;
    status = OTF2_Reader_GetFileSubstrate( reader, &substrate );
    check_status( status, "Read file substrate." );

    printf( "%-*s ", otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "File substrate" );
    switch ( substrate )
    {
        case OTF2_SUBSTRATE_NONE:
            printf( "NONE\n" );
            break;
        case OTF2_SUBSTRATE_SION:
            printf( "SION\n" );
            break;
        case OTF2_SUBSTRATE_POSIX:
            printf( "POSIX\n" );
            break;
        default:
            printf( "%s\n", otf2_print_get_invalid( substrate ) );
    }

    OTF2_Compression compression;
    status = OTF2_Reader_GetCompression( reader, &compression );
    check_status( status, "Read compression mode." );

    printf( "%-*s ", otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Compression" );
    switch ( compression )
    {
        case OTF2_COMPRESSION_NONE:
            printf( "NONE\n" );
            break;
        case OTF2_COMPRESSION_ZLIB:
            printf( "ZLIB\n" );
            break;
        default:
            printf( "%s\n", otf2_print_get_invalid( compression ) );
    }

    uint64_t number_of_locations;
    status = OTF2_Reader_GetNumberOfLocations( reader, &number_of_locations );
    check_status( status, "Read number of locations." );

    printf( "%-*s %" PRIu64 "\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Number of locations",
            number_of_locations );

    uint64_t number_of_global_definitions;
    status = OTF2_Reader_GetNumberOfGlobalDefinitions(
        reader,
        &number_of_global_definitions );
    check_status( status, "Read number of global definitions." );

    printf( "%-*s %" PRIu64 "\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Number of global definitions",
            number_of_global_definitions );

    char* string_buffer;
    status = OTF2_Reader_GetMachineName( reader, &string_buffer );
    check_status( status, "Read machine name." );

    printf( "%-*s %s\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Machine name",
            string_buffer );
    free( string_buffer );

    status = OTF2_Reader_GetCreator( reader, &string_buffer );
    check_status( status, "Read creator." );

    printf( "%-*s %s\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Creator",
            string_buffer );
    free( string_buffer );

    status = OTF2_Reader_GetDescription( reader, &string_buffer );
    check_status( status, "Read description." );

    printf( "%-*s %s\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Description",
            string_buffer );
    free( string_buffer );

    uint32_t number_of_properties;
    char**   property_names;
    status = OTF2_Reader_GetPropertyNames( reader, &number_of_properties, &property_names );
    check_status( status, "Read names of properties." );

    printf( "%-*s %" PRIu32 "\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Number of properties",
            number_of_properties );

    for ( uint32_t property_index = 0; property_index < number_of_properties; property_index++ )
    {
        printf( "%-*s %s\n",
                otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Property name",
                property_names[ property_index ] );

        status = OTF2_Reader_GetProperty( reader, property_names[ property_index ], &string_buffer );
        check_status( status, "Read value of property." );

        printf( "%-*s %s\n",
                otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Property value",
                string_buffer );
        free( string_buffer );
    }
    free( property_names );

    uint64_t trace_id;
    status = OTF2_Reader_GetTraceId( reader, &trace_id );
    check_status( status, "Read trace identifier." );

    printf( "%-*s %" PRIx64 "\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Trace identifier",
            trace_id );

    uint32_t number;
    status = OTF2_Reader_GetNumberOfSnapshots( reader,  &number );
    check_status( status, "Read number of snapshots." );

    printf( "%-*s %" PRIu32 "\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Number of snapshots:",
            number );

    status = OTF2_Reader_GetNumberOfThumbnails( reader,  &number );
    check_status( status, "Read Number of thumbnails." );

    printf( "%-*s %" PRIu32 "\n",
            otf2_ANCHOR_FILE_INFO_COLUMN_WIDTH, "Number of thumbnails:",
            number );
}

void
otf2_print_thumbnails( OTF2_Reader* reader )
{
    /* Do we have thumbnails at all? */
    uint32_t number_of_thumbnails;
    if ( OTF2_Reader_GetNumberOfThumbnails( reader,
                                            &number_of_thumbnails ) != OTF2_SUCCESS
         || number_of_thumbnails == 0 )
    {
        return;
    }

    printf( "\nThumbnail headers:\n" );
    OTF2_ErrorCode status;
    for ( uint32_t i = 0; i < number_of_thumbnails; i++ )
    {
        OTF2_ThumbReader* thumb_reader = OTF2_Reader_GetThumbReader( reader, i );
        if ( !thumb_reader )
        {
            continue;
        }

        char*              name        = NULL;
        char*              description = NULL;
        OTF2_ThumbnailType type;
        uint32_t           number_of_samples;
        uint32_t           number_of_metrics;
        uint64_t*          refs_to_defs = NULL;

        status = OTF2_ThumbReader_GetHeader( thumb_reader,
                                             &name,
                                             &description,
                                             &type,
                                             &number_of_samples,
                                             &number_of_metrics,
                                             &refs_to_defs );
        check_status( status, "Reading thumbnail header failed." );

        printf( "%-*s "
                "%12" PRIUint32
                "  "
                "Name: \"%s\", "
                "Description: \"%s\", "
                "Type: %s, "
                "Samples: %" PRIUint32 ", "
                "%" PRIUint32 " Metrics: ",
                otf2_THUMBNAIL_HEADER_COLUMN_WIDTH, "THUMBNAIL_HEADER",
                i,
                name,
                description,
                otf2_print_get_thumbnail_type( type ),
                number_of_samples,
                number_of_metrics );

        const char* sep = "";
        for ( uint32_t ref = 0; ref < number_of_metrics; ref++ )
        {
            printf( "%s%" PRIUint64, sep, refs_to_defs[ ref ] );
            sep = ", ";
        }
        printf( "\n" );

        free( name );
        free( description );
        free( refs_to_defs );

        if ( !otf2_THUMBNAIL_SAMPLES )
        {
            OTF2_Reader_CloseThumbReader( reader, thumb_reader );
            continue;
        }

        uint64_t* one_sample;
        one_sample = malloc( number_of_metrics * sizeof( *one_sample ) );
        check_pointer( one_sample, "Allocating sample array." );

        uint64_t baseline;
        for ( uint32_t sample = 0; sample < number_of_samples; sample++ )
        {
            status = OTF2_ThumbReader_ReadSample( thumb_reader,
                                                  &baseline,
                                                  number_of_metrics,
                                                  one_sample );
            check_status( status, "Reading thumbnail sample failed." );

            printf( "%-*s "
                    "%12" PRIUint32
                    "  "
                    "Baseline: %" PRIUint64 ", "
                    "Metrics: ",
                    otf2_THUMBNAIL_HEADER_COLUMN_WIDTH, "THUMBNAIL_SAMPLE",
                    sample,
                    baseline );

            const char* sep = "";
            for ( uint32_t metric = 0; metric < number_of_metrics; metric++ )
            {
                printf( "%s%" PRIUint64, sep, one_sample[ metric ] );
                sep = ", ";
            }
            printf( "\n" );
        }
        free( one_sample );

        OTF2_Reader_CloseThumbReader( reader, thumb_reader );
    }
}


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
check_status( OTF2_ErrorCode status,
              char*          description,
              ... )
{
    va_list va;
    va_start( va, description );

    if ( status != OTF2_SUCCESS )
    {
        printf( "==ERROR== %s ", OTF2_Error_GetName( status ) );
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


/** @internal
 *  @brief Add a locations to the list of locations to read events from.
 *
 *  @param locations        List of regions.
 *  @param locationID       Location ID of the location.
 */
void
otf2_print_add_location_to_read( struct otf2_print_data* data,
                                 OTF2_LocationRef        locationID )
{
    uint64_t* location_item = malloc( sizeof( *location_item ) );
    assert( location_item );

    *location_item = locationID;

    otf2_vector_push_back( data->locations_to_read, location_item );
}


/** @internal
 *  @brief Add a string to the set of strings.
 *
 *  @param strings          Set of strings.
 *  @param stringID         String ID of new element.
 *  @param content          Content of the new element.
 */
void
otf2_print_add_string( otf2_hash_table* strings,
                       uint32_t         stringID,
                       const char*      content )
{
    size_t                 hint;
    otf2_hash_table_entry* entry;

    if ( stringID == OTF2_UNDEFINED_STRING )
    {
        return;
    }

    uint64_t string_id_64 = stringID;
    entry = otf2_hash_table_find( strings, &string_id_64, &hint );

    if ( entry )
    {
        fprintf( stderr,
                 "%s: warning: duplicate String definition with id %" PRIu32 "\n",
                 otf2_NAME, stringID );
        return;
    }

    struct otf2_print_def_name* new_string = malloc( sizeof( *new_string )
                                                     + strlen( content ) + 1 );
    assert( new_string );

    new_string->def_id = string_id_64;
    new_string->name   = ( char* )new_string + sizeof( *new_string );
    memcpy( new_string->name, content, strlen( content ) + 1 );

    otf2_hash_table_insert( strings,
                            &new_string->def_id,
                            new_string,
                            &hint );

    entry = otf2_hash_table_find( strings, &string_id_64, &hint );

    assert( entry );
}


/** @internal
 *  @brief Add a def with id tye uint64_t to the set of defs.
 *
 *  @param regions          Set of regions.
 *  @param regionID         Region ID of new region.
 *  @param stringID         String ID of new region.
 */
void
otf2_print_add_def64_name( const char*      def_name,
                           otf2_hash_table* defs,
                           otf2_hash_table* strings,
                           uint64_t         defID,
                           uint32_t         stringID )
{
    size_t                 hint;
    otf2_hash_table_entry* entry;

    if ( defID == OTF2_UNDEFINED_UINT64 )
    {
        return;
    }

    entry = otf2_hash_table_find( defs, &defID, &hint );

    if ( entry )
    {
        fprintf( stderr,
                 "%s: warning: duplicate %s definition with id %" PRIu64 "\n",
                 otf2_NAME, def_name, defID );
        return;
    }

    struct otf2_print_def_name* new_def = malloc( sizeof( *new_def ) );
    assert( new_def );

    new_def->def_id = defID;

    new_def->name = otf2_print_get_string( strings, stringID );

    otf2_hash_table_insert( defs, &new_def->def_id, new_def, &hint );
}

/** @internal
 *  @brief Add a def with id type uint32_t to the set of defs.
 *
 *  @param regions          Set of regions.
 *  @param regionID         Region ID of new region.
 *  @param stringID         String ID of new region.
 */
static void
otf2_print_add_def_name( const char*      def_name,
                         otf2_hash_table* defs,
                         otf2_hash_table* strings,
                         uint32_t         defID,
                         uint32_t         stringID )
{
    uint64_t def64_id = defID;
    if ( defID == OTF2_UNDEFINED_UINT32 )
    {
        def64_id = OTF2_UNDEFINED_UINT64;
    }
    otf2_print_add_def64_name( def_name, defs, strings, def64_id, stringID );
}


char*
otf2_print_get_buffer( size_t len )
{
    #define NR_ENTRIES 8
    static uint32_t next_idx;
    static struct otf2_print_buffer
    {
        char*  buffer;
        size_t size;
    } buffers[ NR_ENTRIES ];
    struct otf2_print_buffer* next = &buffers[ next_idx ];

    next_idx++;
    if ( next_idx == NR_ENTRIES )
    {
        next_idx = 0;
    }

    if ( next->size <= len )
    {
        next->buffer = realloc( next->buffer, len );
        assert( next->buffer );
        next->size = len;
    }

    *next->buffer = '\0';

    return next->buffer;
}


static const char*
otf2_print_get_id( uint32_t ID )
{
    uint64_t id64 = ID;
    if ( ID == OTF2_UNDEFINED_UINT32 )
    {
        id64 = OTF2_UNDEFINED_UINT64;
    }
    return otf2_print_get_id64( id64 );
}


const char*
otf2_print_get_id64( uint64_t ID )
{
    if ( ID == OTF2_UNDEFINED_UINT64 )
    {
        return "UNDEFINED";
    }

    char* buffer = otf2_print_get_buffer( BUFFER_SIZE );

    snprintf( buffer, BUFFER_SIZE, "%" PRIu64, ID );

    return buffer;
}


const char*
otf2_print_get_invalid( uint64_t ID )
{
    char* buffer = otf2_print_get_buffer( BUFFER_SIZE );

    snprintf( buffer, BUFFER_SIZE, "INVALID <%" PRIu64 ">", ID );
    return buffer;
}


const char*
otf2_print_get_name( const char* name,
                     uint64_t    ID )
{
    if ( !name )
    {
        return otf2_print_get_id64( ID );
    }

    size_t len = name ? strlen( name ) + 1 : 0;
    /* add size for id */
    len += BUFFER_SIZE;

    char* buffer = otf2_print_get_buffer( len );

    snprintf( buffer, len, "\"%s\" <%" PRIu64 ">", name, ID );

    return buffer;
}


/** @internal
 *  @brief Get the name of a definition.
 *
 *  @param regions          Set of regions.
 *  @param strings          Set of strings.
 *  @param regionID         Region ID.
 *
 *  @return                 Returns the name of a region if successful, NULL
 *                          otherwise.
 */
const char*
otf2_print_get_def64_name( const otf2_hash_table* defs,
                           uint64_t               defID )
{
    otf2_hash_table_entry* entry;

    if ( defID == OTF2_UNDEFINED_UINT64 )
    {
        return "UNDEFINED";
    }

    entry = otf2_hash_table_find( defs, &defID, NULL );

    if ( !entry )
    {
        return otf2_print_get_invalid( defID );
    }

    struct otf2_print_def_name* def = entry->value;

    return otf2_print_get_name( def->name, defID );
}


/** @internal
 *  @brief Get the name of a definition.
 *
 *  @param regions          Set of regions.
 *  @param strings          Set of strings.
 *  @param regionID         Region ID.
 *
 *  @return                 Returns the name of a region if successful, NULL
 *                          otherwise.
 */
static const char*
otf2_print_get_def_name( const otf2_hash_table* defs,
                         uint32_t               defID )
{
    uint64_t def64_id = defID;
    if ( defID == OTF2_UNDEFINED_UINT32 )
    {
        def64_id = OTF2_UNDEFINED_UINT64;
    }
    return otf2_print_get_def64_name( defs, def64_id );
}


/** @internal
 *  @brief Get the content of a string.
 *
 *  @param strings          Set of strings.
 *  @param stringID         String ID.
 *
 *  @return                 Returns the content of a string if successful, NULL
 *                          otherwise.
 */
char*
otf2_print_get_string( const otf2_hash_table* strings,
                       uint32_t               stringID )
{
    otf2_hash_table_entry* entry;

    if ( stringID == OTF2_UNDEFINED_UINT32 )
    {
        return NULL;
    }

    uint64_t string_id_64 = stringID;
    entry = otf2_hash_table_find( strings, &string_id_64, NULL );

    if ( !entry )
    {
        return NULL;
    }

    struct otf2_print_def_name* def = entry->value;

    return def->name;
}


static inline const char*
otf2_print_get_scope_name( struct otf2_print_defs* defs,
                           OTF2_MetricScope        scopeType,
                           uint64_t                scope )
{
    switch ( scopeType )
    {
        #define scope_case( SCOPE_TYPE, scope_type ) \
    case OTF2_SCOPE_ ## SCOPE_TYPE: \
        return otf2_print_get_def64_name( defs->scope_type, scope )

        scope_case( LOCATION, locations );
        scope_case( LOCATION_GROUP, location_groups );
        scope_case( SYSTEM_TREE_NODE, system_tree_nodes );
        scope_case( GROUP, groups );

        #undef scope_case

        default:
            return otf2_print_get_id64( scope );
    }
}


/* ___ Implementation of callbacks __________________________________________ */



OTF2_CallbackCode
print_unknown( OTF2_LocationRef    locationID,
               OTF2_TimeStamp      time,
               void*               userData,
               OTF2_AttributeList* attributes )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "UNKNOWN",
            locationID, time );

    print_attribute_list( data, attributes );

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
OTF2_CallbackCode
print_buffer_flush( OTF2_LocationRef    locationID,
                    OTF2_TimeStamp      time,
                    void*               userData,
                    OTF2_AttributeList* attributes,
                    OTF2_TimeStamp      stopTime )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Stop Time: %" PRIUint64Full
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "BUFFER_FLUSH",
            locationID, time,
            stopTime,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_measurement_on_off( OTF2_LocationRef     locationID,
                          OTF2_TimeStamp       time,
                          void*                userData,
                          OTF2_AttributeList*  attributes,
                          OTF2_MeasurementMode measurementMode )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Mode: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MEASUREMENT_ON_OFF",
            locationID, time,
            otf2_print_get_measurement_mode( measurementMode ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_enter( OTF2_LocationRef    locationID,
             OTF2_TimeStamp      time,
             void*               userData,
             OTF2_AttributeList* attributes,
             OTF2_RegionRef      region )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Region: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "ENTER",
            locationID, time,
            otf2_print_get_def_name( defs->regions, region ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_leave( OTF2_LocationRef    locationID,
             OTF2_TimeStamp      time,
             void*               userData,
             OTF2_AttributeList* attributes,
             OTF2_RegionRef      region )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Region: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "LEAVE",
            locationID, time,
            otf2_print_get_def_name( defs->regions, region ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_send( OTF2_LocationRef    locationID,
                OTF2_TimeStamp      time,
                void*               userData,
                OTF2_AttributeList* attributes,
                uint32_t            receiver,
                OTF2_CommRef        communicator,
                uint32_t            msgTag,
                uint64_t            msgLength )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Receiver: %" PRIUint32 ", "
            "Communicator: %s, "
            "Tag: %" PRIUint32 ", "
            "Length: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_SEND",
            locationID, time,
            receiver,
            otf2_print_get_def_name( defs->comms, communicator ),
            msgTag,
            msgLength,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_isend( OTF2_LocationRef    locationID,
                 OTF2_TimeStamp      time,
                 void*               userData,
                 OTF2_AttributeList* attributes,
                 uint32_t            receiver,
                 OTF2_CommRef        communicator,
                 uint32_t            msgTag,
                 uint64_t            msgLength,
                 uint64_t            requestID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Receiver: %" PRIUint32 ", "
            "Communicator: %s, "
            "Tag: %" PRIUint32 ", "
            "Length: %" PRIUint64 ", "
            "Request: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_ISEND",
            locationID, time,
            receiver,
            otf2_print_get_def_name( defs->comms, communicator ),
            msgTag,
            msgLength,
            requestID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_isend_complete( OTF2_LocationRef    locationID,
                          OTF2_TimeStamp      time,
                          void*               userData,
                          OTF2_AttributeList* attributes,
                          uint64_t            requestID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Request: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_ISEND_COMPLETE",
            locationID, time,
            requestID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_irecv_request( OTF2_LocationRef    locationID,
                         OTF2_TimeStamp      time,
                         void*               userData,
                         OTF2_AttributeList* attributes,
                         uint64_t            requestID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Request: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_IRECV_REQUEST",
            locationID, time,
            requestID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_recv( OTF2_LocationRef    locationID,
                OTF2_TimeStamp      time,
                void*               userData,
                OTF2_AttributeList* attributes,
                uint32_t            sender,
                OTF2_CommRef        communicator,
                uint32_t            msgTag,
                uint64_t            msgLength )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Sender: %" PRIUint32 ", "
            "Communicator: %s, "
            "Tag: %" PRIUint32 ", "
            "Length: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_RECV",
            locationID, time,
            sender,
            otf2_print_get_def_name( defs->comms, communicator ),
            msgTag,
            msgLength,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_irecv( OTF2_LocationRef    locationID,
                 OTF2_TimeStamp      time,
                 void*               userData,
                 OTF2_AttributeList* attributes,
                 uint32_t            sender,
                 OTF2_CommRef        communicator,
                 uint32_t            msgTag,
                 uint64_t            msgLength,
                 uint64_t            requestID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Sender: %" PRIUint32 ", "
            "Communicator: %s, "
            "Tag: %" PRIUint32 ", "
            "Length: %" PRIUint64 ", "
            "Request: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_IRECV",
            locationID, time,
            sender,
            otf2_print_get_def_name( defs->comms, communicator ),
            msgTag,
            msgLength,
            requestID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_request_test( OTF2_LocationRef    locationID,
                        OTF2_TimeStamp      time,
                        void*               userData,
                        OTF2_AttributeList* attributes,
                        uint64_t            requestID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Request: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_REQUEST_TEST",
            locationID, time,
            requestID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_request_cancelled( OTF2_LocationRef    locationID,
                             OTF2_TimeStamp      time,
                             void*               userData,
                             OTF2_AttributeList* attributes,
                             uint64_t            requestID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Request: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_REQUEST_CANCELLED",
            locationID, time,
            requestID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_collective_begin( OTF2_LocationRef    locationID,
                            OTF2_TimeStamp      time,
                            void*               userData,
                            OTF2_AttributeList* attributes )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_COLLECTIVE_BEGIN",
            locationID, time,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_mpi_collective_end( OTF2_LocationRef    locationID,
                          OTF2_TimeStamp      time,
                          void*               userData,
                          OTF2_AttributeList* attributes,
                          OTF2_CollectiveOp   collectiveOp,
                          OTF2_CommRef        communicator,
                          uint32_t            root,
                          uint64_t            sizeSent,
                          uint64_t            sizeReceived )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Operation: %s, "
            "Communicator: %s, "
            "Root: %s, "
            "Sent: %" PRIUint64 ", "
            "Received: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "MPI_COLLECTIVE_END",
            locationID, time,
            otf2_print_get_collective_op( collectiveOp ),
            otf2_print_get_def_name( defs->comms, communicator ),
            otf2_print_get_id( root ),
            sizeSent,
            sizeReceived,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_omp_fork( OTF2_LocationRef    locationID,
                OTF2_TimeStamp      time,
                void*               userData,
                OTF2_AttributeList* attributes,
                uint32_t            numberOfRequestedThreads )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "# Requested Threads: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "OMP_FORK",
            locationID, time,
            numberOfRequestedThreads,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_omp_join( OTF2_LocationRef    locationID,
                OTF2_TimeStamp      time,
                void*               userData,
                OTF2_AttributeList* attributes )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "OMP_JOIN",
            locationID, time,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_omp_acquire_lock( OTF2_LocationRef    locationID,
                        OTF2_TimeStamp      time,
                        void*               userData,
                        OTF2_AttributeList* attributes,
                        uint32_t            lockID,
                        uint32_t            acquisitionOrder )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Lock: %" PRIUint32 ", "
            "Acquisition Order: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "OMP_ACQUIRE_LOCK",
            locationID, time,
            lockID,
            acquisitionOrder,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_omp_release_lock( OTF2_LocationRef    locationID,
                        OTF2_TimeStamp      time,
                        void*               userData,
                        OTF2_AttributeList* attributes,
                        uint32_t            lockID,
                        uint32_t            acquisitionOrder )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Lock: %" PRIUint32 ", "
            "Acquisition Order: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "OMP_RELEASE_LOCK",
            locationID, time,
            lockID,
            acquisitionOrder,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_omp_task_create( OTF2_LocationRef    locationID,
                       OTF2_TimeStamp      time,
                       void*               userData,
                       OTF2_AttributeList* attributes,
                       uint64_t            taskID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Task: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "OMP_TASK_CREATE",
            locationID, time,
            taskID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_omp_task_switch( OTF2_LocationRef    locationID,
                       OTF2_TimeStamp      time,
                       void*               userData,
                       OTF2_AttributeList* attributes,
                       uint64_t            taskID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Task: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "OMP_TASK_SWITCH",
            locationID, time,
            taskID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_omp_task_complete( OTF2_LocationRef    locationID,
                         OTF2_TimeStamp      time,
                         void*               userData,
                         OTF2_AttributeList* attributes,
                         uint64_t            taskID )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Task: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "OMP_TASK_COMPLETE",
            locationID, time,
            taskID,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_metric( OTF2_LocationRef        locationID,
              OTF2_TimeStamp          time,
              void*                   userData,
              OTF2_AttributeList*     attributes,
              OTF2_MetricRef          metric,
              uint8_t                 numberOfMetrics,
              const OTF2_Type*        typeIDs,
              const OTF2_MetricValue* metricValues )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Metric: %s, "
            "%" PRIUint8 " Values: ",
            otf2_EVENT_COLUMN_WIDTH, "METRIC",
            locationID, time,
            otf2_print_get_def_name( defs->metrics, metric ),
            numberOfMetrics );

    const char* sep = "";
    for ( uint8_t i = 0; i < numberOfMetrics; i++ )
    {
        switch ( typeIDs[ i ] )
        {
            case OTF2_TYPE_INT64:
                printf( "%s(INT64; %" PRId64 ")", sep, metricValues[ i ].signed_int );
                break;
            case OTF2_TYPE_UINT64:
                printf( "%s(UINT64; %" PRIu64 ")", sep, metricValues[ i ].unsigned_int );
                break;
            case OTF2_TYPE_DOUBLE:
                printf( "%s(DOUBLE; %f)", sep, metricValues[ i ].floating_point );
                break;
            default:
            {
                printf( "%s(%s; %08" PRIx64 ")",
                        sep,
                        otf2_print_get_invalid( typeIDs[ i ] ),
                        metricValues[ i ].unsigned_int );
            }
        }
        sep = ", ";
    }
    printf( "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_parameter_string( OTF2_LocationRef    locationID,
                        OTF2_TimeStamp      time,
                        void*               userData,
                        OTF2_AttributeList* attributes,
                        OTF2_ParameterRef   parameter,
                        OTF2_StringRef      string )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Parameter: %s, "
            "Value: \"%s\""
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "PARAMETER_STRING",
            locationID, time,
            otf2_print_get_def_name( defs->parameters, parameter ),
            otf2_print_get_def_name( defs->strings, string ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_parameter_int( OTF2_LocationRef    locationID,
                     OTF2_TimeStamp      time,
                     void*               userData,
                     OTF2_AttributeList* attributes,
                     OTF2_ParameterRef   parameter,
                     int64_t             value )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Parameter: %s, "
            "Value: %" PRIInt64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "PARAMETER_INT64",
            locationID, time,
            otf2_print_get_def_name( defs->parameters, parameter ),
            value,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_parameter_unsigned_int( OTF2_LocationRef    locationID,
                              OTF2_TimeStamp      time,
                              void*               userData,
                              OTF2_AttributeList* attributes,
                              OTF2_ParameterRef   parameter,
                              uint64_t            value )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Parameter: %s, "
            "Value: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "PARAMETER_UINT64",
            locationID, time,
            otf2_print_get_def_name( defs->parameters, parameter ),
            value,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}

OTF2_CallbackCode
print_rma_win_create( OTF2_LocationRef    locationID,
                      OTF2_TimeStamp      time,
                      void*               userData,
                      OTF2_AttributeList* attributes,
                      OTF2_RmaWinRef      win )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_WIN_CREATE",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_win_destroy( OTF2_LocationRef    locationID,
                       OTF2_TimeStamp      time,
                       void*               userData,
                       OTF2_AttributeList* attributes,
                       OTF2_RmaWinRef      win )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_WIN_DESTROY",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_collective_begin( OTF2_LocationRef    locationID,
                            OTF2_TimeStamp      time,
                            void*               userData,
                            OTF2_AttributeList* attributes  )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_COLLECTIVE_BEGIN",
            locationID, time,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_collective_end( OTF2_LocationRef    locationID,
                          OTF2_TimeStamp      time,
                          void*               userData,
                          OTF2_AttributeList* attributes,
                          OTF2_CollectiveOp   collectiveOp,
                          OTF2_RmaSyncLevel   syncLevel,
                          OTF2_RmaWinRef      win,
                          uint32_t            root,
                          uint64_t            bytesSent,
                          uint64_t            bytesReceived )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Operation: %s, "
            "Window: %s, "
            "Level of Synchronicity: %s, "
            "Root: %s, "
            "Sent: %" PRIUint64 ", "
            "Received: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_COLLECTIVE_END",
            locationID, time,
            otf2_print_get_collective_op( collectiveOp ),
            otf2_print_get_def_name( defs->rma_wins, win ),
            otf2_print_get_rma_sync_level( syncLevel ),
            otf2_print_get_id( root ),
            bytesSent,
            bytesReceived,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_group_sync( OTF2_LocationRef    locationID,
                      OTF2_TimeStamp      time,
                      void*               userData,
                      OTF2_AttributeList* attributes,
                      OTF2_RmaSyncLevel   syncLevel,
                      OTF2_RmaWinRef      win,
                      OTF2_GroupRef       group )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Level of Synchronicity: %s, "
            "Window: %s, "
            "Group: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_GROUP_SYNC",
            locationID, time,
            otf2_print_get_rma_sync_level( syncLevel ),
            otf2_print_get_def_name( defs->rma_wins, win ),
            otf2_print_get_def_name( defs->groups, group ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_request_lock( OTF2_LocationRef    locationID,
                        OTF2_TimeStamp      time,
                        void*               userData,
                        OTF2_AttributeList* attributes,
                        OTF2_RmaWinRef      win,
                        uint32_t            remote,
                        uint64_t            lockId,
                        OTF2_LockType       lockType )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Remote: %" PRIUint32 ", "
            "Lock: %" PRIUint64 ", "
            "Type: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_REQUEST_LOCK",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            remote,
            lockId,
            otf2_print_get_lock_type( lockType ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_acquire_lock( OTF2_LocationRef    locationID,
                        OTF2_TimeStamp      time,
                        void*               userData,
                        OTF2_AttributeList* attributes,
                        OTF2_RmaWinRef      win,
                        uint32_t            remote,
                        uint64_t            lockId,
                        OTF2_LockType       lockType )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Remote: %" PRIUint32 ", "
            "Lock: %" PRIUint64 ", "
            "Type: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_ACQUIRE_LOCK",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            remote,
            lockId,
            otf2_print_get_lock_type( lockType ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_try_lock( OTF2_LocationRef    locationID,
                    OTF2_TimeStamp      time,
                    void*               userData,
                    OTF2_AttributeList* attributes,
                    OTF2_RmaWinRef      win,
                    uint32_t            remote,
                    uint64_t            lockId,
                    OTF2_LockType       lockType )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Remote: %" PRIUint32 ", "
            "Lock: %" PRIUint64 ", "
            "Type: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_TRY_LOCK",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            remote,
            lockId,
            otf2_print_get_lock_type( lockType ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_release_lock( OTF2_LocationRef    locationID,
                        OTF2_TimeStamp      time,
                        void*               userData,
                        OTF2_AttributeList* attributes,
                        OTF2_RmaWinRef      win,
                        uint32_t            remote,
                        uint64_t            lockId )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Remote: %" PRIUint32 ", "
            "Lock: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_RELEASE_LOCK",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            remote,
            lockId,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_sync( OTF2_LocationRef    locationID,
                OTF2_TimeStamp      time,
                void*               userData,
                OTF2_AttributeList* attributes,
                OTF2_RmaWinRef      win,
                uint32_t            remote,
                OTF2_RmaSyncType    syncType )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Remote: %" PRIUint32 ", "
            "Sync Type: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_SYNC",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            remote,
            otf2_print_get_rma_sync_type( syncType ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_wait_change( OTF2_LocationRef    locationID,
                       OTF2_TimeStamp      time,
                       void*               userData,
                       OTF2_AttributeList* attributes,
                       OTF2_RmaWinRef      win )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_WAIT_CHANGE",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_put( OTF2_LocationRef    locationID,
               OTF2_TimeStamp      time,
               void*               userData,
               OTF2_AttributeList* attributes,
               OTF2_RmaWinRef      win,
               uint32_t            remote,
               uint64_t            bytes,
               uint64_t            matchingId )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Remote: %" PRIUint32 ", "
            "Bytes: %" PRIUint64 ", "
            "Matching: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_PUT",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            remote,
            bytes,
            matchingId,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_get( OTF2_LocationRef    locationID,
               OTF2_TimeStamp      time,
               void*               userData,
               OTF2_AttributeList* attributes,
               OTF2_RmaWinRef      win,
               uint32_t            remote,
               uint64_t            bytes,
               uint64_t            matchingId )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Remote: %" PRIUint32 ", "
            "Bytes: %" PRIUint64 ", "
            "Matching: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_GET",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            remote,
            bytes,
            matchingId,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_atomic( OTF2_LocationRef    locationID,
                  OTF2_TimeStamp      time,
                  void*               userData,
                  OTF2_AttributeList* attributes,
                  OTF2_RmaWinRef      win,
                  uint32_t            remote,
                  OTF2_RmaAtomicType  type,
                  uint64_t            bytesSent,
                  uint64_t            bytesReceived,
                  uint64_t            matchingId )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Remote: %" PRIUint32 ", "
            "Type: %s, "
            "Sent: %" PRIUint64 ", "
            "Received: %" PRIUint64 ", "
            "Matching: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_ATOMIC",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            remote,
            otf2_print_get_rma_atomic_type( type ),
            bytesSent,
            bytesReceived,
            matchingId,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_op_complete_blocking( OTF2_LocationRef    locationID,
                                OTF2_TimeStamp      time,
                                void*               userData,
                                OTF2_AttributeList* attributes,
                                OTF2_RmaWinRef      win,
                                uint64_t            matchingId )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Matching: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_OP_COMPLETE_BLOCKING",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            matchingId,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_op_complete_non_blocking( OTF2_LocationRef    locationID,
                                    OTF2_TimeStamp      time,
                                    void*               userData,
                                    OTF2_AttributeList* attributes,
                                    OTF2_RmaWinRef      win,
                                    uint64_t            matchingId )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Matching: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_OP_COMPLETE_NON_BLOCKING",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            matchingId,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_op_test( OTF2_LocationRef    locationID,
                   OTF2_TimeStamp      time,
                   void*               userData,
                   OTF2_AttributeList* attributes,
                   OTF2_RmaWinRef      win,
                   uint64_t            matchingId )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Matching: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_OP_TEST",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            matchingId,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_rma_op_complete_remote( OTF2_LocationRef    locationID,
                              OTF2_TimeStamp      time,
                              void*               userData,
                              OTF2_AttributeList* attributes,
                              OTF2_RmaWinRef      win,
                              uint64_t            matchingId )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Window: %s, "
            "Matching: %" PRIUint64 ", "
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "RMA_OP_COMPLETE_REMOTE",
            locationID, time,
            otf2_print_get_def_name( defs->rma_wins, win ),
            matchingId,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}

OTF2_CallbackCode
print_thread_fork( OTF2_LocationRef    locationID,
                   OTF2_TimeStamp      time,
                   void*               userData,
                   OTF2_AttributeList* attributes,
                   OTF2_Paradigm       model,
                   uint32_t            numberOfRequestedThreads )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Model: %s, "
            "# Requested Threads: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_FORK",
            locationID, time,
            otf2_print_get_paradigm( model ),
            numberOfRequestedThreads,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_thread_join( OTF2_LocationRef    locationID,
                   OTF2_TimeStamp      time,
                   void*               userData,
                   OTF2_AttributeList* attributes,
                   OTF2_Paradigm       model )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Model: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_JOIN",
            locationID, time,
            otf2_print_get_paradigm( model ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_thread_team_begin( OTF2_LocationRef    locationID,
                         OTF2_TimeStamp      time,
                         void*               userData,
                         OTF2_AttributeList* attributes,
                         OTF2_CommRef        threadTeam )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Thread Team: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_TEAM_BEGIN",
            locationID, time,
            otf2_print_get_def_name( defs->comms, threadTeam ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_thread_team_end( OTF2_LocationRef    locationID,
                       OTF2_TimeStamp      time,
                       void*               userData,
                       OTF2_AttributeList* attributes,
                       OTF2_CommRef        threadTeam )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Thread Team: %s"
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_TEAM_END",
            locationID, time,
            otf2_print_get_def_name( defs->comms, threadTeam ),
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_thread_acquire_lock( OTF2_LocationRef    locationID,
                           OTF2_TimeStamp      time,
                           void*               userData,
                           OTF2_AttributeList* attributes,
                           OTF2_Paradigm       model,
                           uint32_t            lockID,
                           uint32_t            acquisitionOrder )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Model: %s, "
            "Lock: %" PRIUint32 ", "
            "Acquisition Order: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_ACQUIRE_LOCK",
            locationID, time,
            otf2_print_get_paradigm( model ),
            lockID,
            acquisitionOrder,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_thread_release_lock( OTF2_LocationRef    locationID,
                           OTF2_TimeStamp      time,
                           void*               userData,
                           OTF2_AttributeList* attributes,
                           OTF2_Paradigm       model,
                           uint32_t            lockID,
                           uint32_t            acquisitionOrder )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Model: %s, "
            "Lock: %" PRIUint32 ", "
            "Acquisition Order: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_RELEASE_LOCK",
            locationID, time,
            otf2_print_get_paradigm( model ),
            lockID,
            acquisitionOrder,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_thread_task_create( OTF2_LocationRef    locationID,
                          OTF2_TimeStamp      time,
                          void*               userData,
                          OTF2_AttributeList* attributes,
                          OTF2_CommRef        threadTeam,
                          uint32_t            creatingThread,
                          uint32_t            generationNumber )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Thread Team: %s, "
            "Creating Thread: %" PRIUint32 ", "
            "Generation Number: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_TASK_CREATE",
            locationID, time,
            otf2_print_get_def_name( defs->comms, threadTeam ),
            creatingThread,
            generationNumber,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_thread_task_switch( OTF2_LocationRef    locationID,
                          OTF2_TimeStamp      time,
                          void*               userData,
                          OTF2_AttributeList* attributes,
                          OTF2_CommRef        threadTeam,
                          uint32_t            creatingThread,
                          uint32_t            generationNumber )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Thread Team: %s, "
            "Creating Thread: %" PRIUint32 ", "
            "Generation Number: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_TASK_SWITCH",
            locationID, time,
            otf2_print_get_def_name( defs->comms, threadTeam ),
            creatingThread,
            generationNumber,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_thread_task_complete( OTF2_LocationRef    locationID,
                            OTF2_TimeStamp      time,
                            void*               userData,
                            OTF2_AttributeList* attributes,
                            OTF2_CommRef        threadTeam,
                            uint32_t            creatingThread,
                            uint32_t            generationNumber )
{
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Thread Team: %s, "
            "Creating Thread: %" PRIUint32 ", "
            "Generation Number: %" PRIUint32
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "THREAD_TASK_COMPLETE",
            locationID, time,
            otf2_print_get_def_name( defs->comms, threadTeam ),
            creatingThread,
            generationNumber,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


/** @} */


static OTF2_CallbackCode
print_snap_snapshot_start( OTF2_LocationRef    locationID,
                           OTF2_TimeStamp      snapTime,
                           void*               userData,
                           OTF2_AttributeList* attributes,
                           uint64_t            numberOfRecords )
{
    if ( snapTime < otf2_MINTIME || snapTime > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "# Events: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "SNAPSHOT_START",
            locationID, snapTime,
            numberOfRecords,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
print_snap_snapshot_end( OTF2_LocationRef    locationID,
                         OTF2_TimeStamp      snapTime,
                         void*               userData,
                         OTF2_AttributeList* attributes,
                         uint64_t            contReadPos )
{
    if ( snapTime < otf2_MINTIME || snapTime > otf2_MAXTIME )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  "
            "Cont. Read Position: %" PRIUint64
            "%s",
            otf2_EVENT_COLUMN_WIDTH, "SNAPSHOT_END",
            locationID, snapTime,
            contReadPos,
            "\n" );

    print_attribute_list( data, attributes );

    return OTF2_CALLBACK_SUCCESS;
}


/* ___ Global definitions ____________________________________________________*/


OTF2_CallbackCode
print_global_def_unknown( void* userData )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s\n",
            otf2_DEF_COLUMN_WIDTH, "UNKNOWN" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_clock_properties( void*    userData,
                                   uint64_t timerResolution,
                                   uint64_t globalOffset,
                                   uint64_t traceLength )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s %12s  "
            "Ticks per Seconds: %" PRIUint64 ", "
            "Global Offset: %" PRIUint64 ", "
            "Length: %" PRIUint64
            "%s",
            otf2_DEF_COLUMN_WIDTH, "CLOCK_PROPERTIES",
            "",
            timerResolution,
            globalOffset,
            traceLength,
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_string( void*          userData,
                         OTF2_StringRef self,
                         const char*    string )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_string( defs->strings, self, string );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s %12u  \"%s\""
            "%s",
            otf2_DEF_COLUMN_WIDTH, "STRING",
            self,
            string,
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_attribute( void*             userData,
                            OTF2_AttributeRef self,
                            OTF2_StringRef    name,
                            OTF2_Type         type )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "Attribute",
                             defs->attributes,
                             defs->strings,
                             self,
                             name );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s, "
            "Type: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "ATTRIBUTE",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_type( type ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_system_tree_node( void*                  userData,
                                   OTF2_SystemTreeNodeRef self,
                                   OTF2_StringRef         name,
                                   OTF2_StringRef         className,
                                   OTF2_SystemTreeNodeRef parent )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "SystemTreeNode",
                             defs->system_tree_nodes,
                             defs->strings,
                             self,
                             name );

    if ( otf2_DOT )
    {
        /* Generate dot node entry. */
        fprintf( data->dot_file, "    n%u [label=\"%s %s (Node ID: %u)\"];\n",
                 self,
                 otf2_print_get_string( defs->strings, className ),
                 otf2_print_get_string( defs->strings, name ),
                 self );

        /* Generate dot edge entry. */
        if ( parent != OTF2_UNDEFINED_SYSTEM_TREE_NODE )
        {
            fprintf( data->dot_file, "    n%u -> n%u;\n", parent, self );
        }
    }

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s, "
            "Class: %s, "
            "Parent: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "SYSTEM_TREE_NODE",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_def_name( defs->strings, className ),
            otf2_print_get_def_name( defs->system_tree_nodes, parent ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_location_group( void*                  userData,
                                 OTF2_LocationGroupRef  self,
                                 OTF2_StringRef         name,
                                 OTF2_LocationGroupType locationGroupType,
                                 OTF2_SystemTreeNodeRef systemTreeParent )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "LocationGroup",
                             defs->location_groups,
                             defs->strings,
                             self,
                             name );

    if ( otf2_DOT )
    {
        /* Generate dot node entry. */
        fprintf( data->dot_file,
                 "    g%u [label=\"%s (LG ID: %u)\", shape=box];\n",
                 self,
                 otf2_print_get_string( defs->strings, name ),
                 self );

        /* Generate dot edge entry. */
        if ( systemTreeParent != OTF2_UNDEFINED_SYSTEM_TREE_NODE )
        {
            fprintf( data->dot_file, "      n%u -> g%u;\n",
                     systemTreeParent,
                     self );
        }
    }

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s, "
            "Type: %s, "
            "Parent: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "LOCATION_GROUP",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_location_group_type( locationGroupType ),
            otf2_print_get_def_name( defs->system_tree_nodes, systemTreeParent ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_location( void*                 userData,
                           OTF2_LocationRef      self,
                           OTF2_StringRef        name,
                           OTF2_LocationType     locationType,
                           uint64_t              numberOfEvents,
                           OTF2_LocationGroupRef locationGroup )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def64_name( "Location",
                               defs->locations,
                               defs->strings,
                               self,
                               name );

    /* Print definition if selected. */
    if ( otf2_DOT )
    {
        /* Generate dot node entry. */
        fprintf( data->dot_file,
                 "    l%" PRIu64 " [label=\"%s %s (L ID: %" PRIu64 ")\", shape=diamond];\n",
                 self,
                 otf2_print_get_location_type( locationType ),
                 otf2_print_get_string( defs->strings, name ),
                 self );

        /* Generate dot edge entry. */
        if ( locationGroup != OTF2_UNDEFINED_LOCATION_GROUP )
        {
            fprintf( data->dot_file, "    g%u -> l%" PRIu64 ";\n",
                     locationGroup,
                     self );
        }
    }

    if ( otf2_GLOBDEFS )
    {
        printf( "%-*s "
                "%12" PRIUint64
                "  "
                "Name: %s, "
                "Type: %s, "
                "# Events: %" PRIUint64 ", "
                "Group: %s"
                "%s",
                otf2_DEF_COLUMN_WIDTH, "LOCATION",
                self,
                otf2_print_get_def_name( defs->strings, name ),
                otf2_print_get_location_type( locationType ),
                numberOfEvents,
                otf2_print_get_def_name( defs->location_groups, locationGroup ),
                "\n" );
    }

    /* Only proceed if either no local location is selected (i.e. read all) or
     * location ID matches provided location ID. */
    if ( otf2_LOCAL != OTF2_UNDEFINED_LOCATION && otf2_LOCAL != self )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    /* add location to the list of locations to read events from */
    otf2_print_add_location_to_read( data, self );

    otf2_LOCAL_FOUND = true;

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_region( void*           userData,
                         OTF2_RegionRef  self,
                         OTF2_StringRef  name,
                         OTF2_StringRef  canonicalName,
                         OTF2_StringRef  description,
                         OTF2_RegionRole regionRole,
                         OTF2_Paradigm   paradigm,
                         OTF2_RegionFlag regionFlags,
                         OTF2_StringRef  sourceFile,
                         uint32_t        beginLineNumber,
                         uint32_t        endLineNumber )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "Region",
                             defs->regions,
                             defs->strings,
                             self,
                             name );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s "
            "(Aka. %s), "
            "Descr.: %s, "
            "Role: %s, "
            "Paradigm: %s, "
            "Flags: %s, "
            "File: %s, "
            "Begin: %" PRIUint32 ", "
            "End: %" PRIUint32
            "%s",
            otf2_DEF_COLUMN_WIDTH, "REGION",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_def_name( defs->strings, canonicalName ),
            otf2_print_get_def_name( defs->strings, description ),
            otf2_print_get_region_role( regionRole ),
            otf2_print_get_paradigm( paradigm ),
            otf2_print_get_region_flag( regionFlags ),
            otf2_print_get_def_name( defs->strings, sourceFile ),
            beginLineNumber,
            endLineNumber,
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_callsite( void*            userData,
                           OTF2_CallsiteRef self,
                           OTF2_StringRef   sourceFile,
                           uint32_t         lineNumber,
                           OTF2_RegionRef   enteredRegion,
                           OTF2_RegionRef   leftRegion )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "Callsite",
                             defs->callsites,
                             defs->strings,
                             self,
                             OTF2_UNDEFINED_STRING );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "sourceFile: %s, "
            "lineNumber: %" PRIUint32 ", "
            "enteredRegion: %s, "
            "leftRegion: %s, "
            "%s",
            otf2_DEF_COLUMN_WIDTH, "CALLSITE",
            self,
            otf2_print_get_def_name( defs->strings, sourceFile ),
            lineNumber,
            otf2_print_get_def_name( defs->regions, enteredRegion ),
            otf2_print_get_def_name( defs->regions, leftRegion ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_callpath( void*            userData,
                           OTF2_CallpathRef self,
                           OTF2_CallpathRef parent,
                           OTF2_RegionRef   region )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "Callpath",
                             defs->callpaths,
                             defs->strings,
                             self,
                             OTF2_UNDEFINED_STRING );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Parent: %s, "
            "Region: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "CALLPATH",
            self,
            otf2_print_get_def_name( defs->callpaths, parent ),
            otf2_print_get_def_name( defs->regions, region ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_group( void*           userData,
                        OTF2_GroupRef   self,
                        OTF2_StringRef  name,
                        OTF2_GroupType  groupType,
                        OTF2_Paradigm   paradigm,
                        OTF2_GroupFlag  groupFlags,
                        uint32_t        numberOfMembers,
                        const uint64_t* members )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "Group",
                             defs->groups,
                             defs->strings,
                             self,
                             name );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s, "
            "Type: %s, "
            "Paradigm: %s, "
            "Flags: %s, "
            "%" PRIUint32 " Members:",
            otf2_DEF_COLUMN_WIDTH, "GROUP",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_group_type( groupType ),
            otf2_print_get_paradigm( paradigm ),
            otf2_print_get_group_flag( groupFlags ),
            numberOfMembers );

    for ( uint32_t i = 0; i < numberOfMembers; i++ )
    {
        printf( " %" PRIUint64, members[ i ] );
    }
    printf( "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_metric_member( void*                userData,
                                OTF2_MetricMemberRef self,
                                OTF2_StringRef       name,
                                OTF2_StringRef       description,
                                OTF2_MetricType      metricType,
                                OTF2_MetricMode      metricMode,
                                OTF2_Type            valueType,
                                OTF2_MetricBase      metricBase,
                                int64_t              exponent,
                                OTF2_StringRef       unit )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "MetricMember",
                             defs->metric_members,
                             defs->strings,
                             self,
                             name );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }


    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s, "
            "Descr.: %s, "
            "Type: %s, "
            "Mode: %s, "
            "Value Type: %s, "
            "Base: %s, "
            "Exponent: %" PRIInt64 ", "
            "Unit: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "METRIC_MEMBER",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_def_name( defs->strings, description ),
            otf2_print_get_metric_type( metricType ),
            otf2_print_get_metric_mode( metricMode ),
            otf2_print_get_type( valueType ),
            otf2_print_get_metric_base( metricBase ),
            exponent,
            otf2_print_get_def_name( defs->strings, unit ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_metric_class( void*                       userData,
                               OTF2_MetricRef              self,
                               uint8_t                     numberOfMetrics,
                               const OTF2_MetricMemberRef* metricMembers,
                               OTF2_MetricOccurrence       metricOccurrence,
                               OTF2_RecorderKind           recorderKind )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "MetricClass",
                             defs->metrics,
                             defs->strings,
                             self,
                             OTF2_UNDEFINED_STRING );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Occurrence: %s, "
            "Kind: %s, "
            "%" PRIUint8 " Members: ",
            otf2_DEF_COLUMN_WIDTH, "METRIC_CLASS",
            self,
            otf2_print_get_metric_occurrence( metricOccurrence ),
            otf2_print_get_recorder_kind( recorderKind ),
            numberOfMetrics );

    const char* sep = "";
    for ( uint8_t i = 0; i < numberOfMetrics; i++ )
    {
        printf( "%s%s",
                sep,
                otf2_print_get_def_name( defs->metric_members, metricMembers[ i ] ) );
        sep = ", ";
    }
    printf( "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_metric_instance( void*            userData,
                                  OTF2_MetricRef   self,
                                  OTF2_MetricRef   metricClass,
                                  OTF2_LocationRef recorder,
                                  OTF2_MetricScope metricScope,
                                  uint64_t         scope )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "MetricInstance",
                             defs->metrics,
                             defs->strings,
                             self,
                             OTF2_UNDEFINED_STRING );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Class: %s, "
            "Recorder: %s, "
            "Scope: %s %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "METRIC_INSTANCE",
            self,
            otf2_print_get_def_name( defs->metrics, metricClass ),
            otf2_print_get_def_name( defs->locations, recorder ),
            otf2_print_get_metric_scope( metricScope ),
            otf2_print_get_scope_name( defs, metricScope, scope ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_comm( void*          userData,
                       OTF2_CommRef   self,
                       OTF2_StringRef name,
                       OTF2_GroupRef  group,
                       OTF2_CommRef   parent )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "Comm",
                             defs->comms,
                             defs->strings,
                             self,
                             name );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s, "
            "Group: %s, "
            "Parent: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "COMM",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_def_name( defs->groups, group ),
            otf2_print_get_def_name( defs->comms, parent ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_parameter( void*              userData,
                            OTF2_ParameterRef  self,
                            OTF2_StringRef     name,
                            OTF2_ParameterType parameterType )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "Parameter",
                             defs->parameters,
                             defs->strings,
                             self,
                             name );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s, "
            "Type: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "PARAMETER",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_parameter_type( parameterType ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_rma_win( void*          userData,
                          OTF2_RmaWinRef self,
                          OTF2_StringRef name,
                          OTF2_CommRef   comm )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    otf2_print_add_def_name( "RmaWin",
                             defs->rma_wins,
                             defs->strings,
                             self,
                             name );

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12" PRIUint32
            "  "
            "Name: %s, "
            "Communicator: %s, "
            "%s",
            otf2_DEF_COLUMN_WIDTH, "RMA_WIN",
            self,
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_def_name( defs->comms, comm ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_global_def_metric_class_recorder( void*            userData,
                                        OTF2_MetricRef   metricClass,
                                        OTF2_LocationRef recorder )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12s"
            "  "
            "Class: %s, "
            "Recorder: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "METRIC_CLASS_RECORDER",
            "",
            otf2_print_get_def_name( defs->metrics, metricClass ),
            otf2_print_get_def_name( defs->locations, recorder ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
print_global_def_system_tree_node_property( void*                  userData,
                                            OTF2_SystemTreeNodeRef systemTreeNode,
                                            OTF2_StringRef         name,
                                            OTF2_StringRef         value )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    if ( otf2_DOT )
    {
        static uint32_t prop_id;
        /* Generate dot node entry. */
        fprintf( data->dot_file,
                 "    subgraph {\n"
                 "        rank = same;\n"
                 "        n%u;\n"
                 "        np%u [label=\"%s=%s\" shape=box];\n"
                 "        np%u -> n%u;\n"
                 "    }\n",
                 systemTreeNode,
                 prop_id,
                 otf2_print_get_string( defs->strings, name ),
                 otf2_print_get_string( defs->strings, value ),
                 prop_id, systemTreeNode );
        prop_id++;
    }

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12s"
            "  "
            "System Tree Node: %s, "
            "Name: %s, "
            "Value: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "SYSTEM_TREE_NODE_PROPERTY",
            "",
            otf2_print_get_def_name( defs->system_tree_nodes, systemTreeNode ),
            otf2_print_get_def_name( defs->strings, name ),
            otf2_print_get_def_name( defs->strings, value ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
print_global_def_system_tree_node_domain( void*                  userData,
                                          OTF2_SystemTreeNodeRef systemTreeNode,
                                          OTF2_SystemTreeDomain  systemTreeDomain )
{
    struct otf2_print_data* data = userData;
    struct otf2_print_defs* defs = data->defs;

    if ( otf2_DOT )
    {
        static uint32_t domain_id;
        /* Generate dot node entry. */
        fprintf( data->dot_file,
                 "    subgraph {\n"
                 "        rank = same;\n"
                 "        n%u;\n"
                 "        nd%u [label=\"%s\" shape=box];\n"
                 "        nd%u -> n%u;\n"
                 "    }\n",
                 systemTreeNode,
                 domain_id,
                 otf2_print_get_system_tree_domain( systemTreeDomain ),
                 domain_id, systemTreeNode );
        domain_id++;
    }

    /* Print definition if selected. */
    if ( !otf2_GLOBDEFS )
    {
        return OTF2_CALLBACK_SUCCESS;
    }

    printf( "%-*s "
            "%12s"
            "  "
            "System Tree Node: %s, "
            "Domain: %s"
            "%s",
            otf2_DEF_COLUMN_WIDTH, "SYSTEM_TREE_NODE_DOMAIN",
            "",
            otf2_print_get_def_name( defs->system_tree_nodes, systemTreeNode ),
            otf2_print_get_system_tree_domain( systemTreeDomain ),
            "\n" );

    return OTF2_CALLBACK_SUCCESS;
}


static void
map_traverse_dense( uint64_t localId,
                    uint64_t globalId,
                    void*    userData )
{
    const char* sep = *( char** )userData;
    printf( "%s%" PRIu64, sep, globalId );
    *( char** )userData = ",";
}

static void
map_traverse_sparse( uint64_t localId,
                     uint64_t globalId,
                     void*    userData )
{
    const char* sep = *( char** )userData;
    printf( "%s%" PRIu64 "=>%" PRIu64, sep, localId, globalId );
    *( char** )userData = ",";
}

OTF2_CallbackCode
print_def_mapping_table( void*             userData,
                         OTF2_MappingType  mapType,
                         const OTF2_IdMap* iDMap )
{
    uint64_t* location_id_ptr = userData;

    printf( "%-*s %12" PRIu64 "  Type: %s, ",
            otf2_DEF_COLUMN_WIDTH, "MAPPING_TABLE",
            *location_id_ptr,
            otf2_print_get_mapping_type( mapType ) );

    OTF2_IdMapMode map_mode;
    OTF2_IdMap_GetMode( iDMap, &map_mode );

    const char*                 sep;
    OTF2_IdMap_TraverseCallback traverse_cb;
    const char*                 end;
    if ( map_mode == OTF2_ID_MAP_DENSE )
    {
        sep         = "[";
        traverse_cb = map_traverse_dense;
        end         = "]";
    }
    else
    {
        sep         = "{";
        traverse_cb = map_traverse_sparse;
        end         = "}";
    }

    OTF2_IdMap_Traverse( iDMap, traverse_cb, &sep );

    /* includes "\n" */
    puts( end );

    return OTF2_CALLBACK_SUCCESS;
}


OTF2_CallbackCode
print_def_clock_offset( void*    userData,
                        uint64_t time,
                        int64_t  offset,
                        double   stddev )
{
    uint64_t* location_id_ptr = userData;

    printf( "%-*s %12" PRIu64 "  Time: %" PRIu64 ", Offset: %+" PRIi64 ", "
            "StdDev: %f\n",
            otf2_DEF_COLUMN_WIDTH, "CLOCK_OFFSET",
            *location_id_ptr, time, offset, stddev );

    return OTF2_CALLBACK_SUCCESS;
}
