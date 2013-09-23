/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2013,
 * Technische Universitaet Dresden, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 */

/** @internal
 *
 *  @file       otf2_marker.cpp
 *
 *  @brief      Read or edit a marker file.
 */

#include <config.h>

#include <otf2/otf2.h>

#include <cstdlib>
#include <cstring>

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>


/* ___ Types and global variables. __________________________________________ */


struct otf2_definitions
{
    std::set<OTF2_LocationRef>       locations;
    std::set<OTF2_LocationGroupRef>  location_groups;
    std::set<OTF2_SystemTreeNodeRef> system_tree_nodes;
    std::set<OTF2_GroupRef>          groups;
    std::set<OTF2_CommRef>           comms;
};

struct otf2_clock
{
    uint64_t resolution;
    uint64_t offset;
    uint64_t length;
};

struct otf2_marker
{
    OTF2_TimeStamp   timestamp;
    OTF2_TimeStamp   duration;
    OTF2_MarkerScope scope;
    uint64_t         scope_ref;
    std::string      text;
};

struct otf2_marker_category
{
    OTF2_MarkerRef             ref;
    OTF2_MarkerSeverity        severity;
    std::vector< otf2_marker > markers;
};

typedef std::map< std::string, otf2_marker_category* >      otf2_marker_categories_map;
typedef otf2_marker_categories_map::iterator                otf2_marker_categories_map_it;
typedef otf2_marker_categories_map::const_iterator          otf2_marker_categories_map_cit;
typedef otf2_marker_categories_map::value_type              otf2_marker_categories_map_value;

typedef std::map< std::string, otf2_marker_categories_map > otf2_marker_groups_map;
typedef otf2_marker_groups_map::iterator                    otf2_marker_groups_map_it;
typedef otf2_marker_groups_map::const_iterator              otf2_marker_groups_map_cit;
typedef otf2_marker_groups_map::value_type                  otf2_marker_groups_map_value;
static otf2_marker_groups_map otf2_marker_groups;

typedef std::map< OTF2_MarkerRef, otf2_marker_category* > otf2_marker_ref_to_category_map;
typedef otf2_marker_ref_to_category_map::iterator         otf2_marker_ref_to_category_map_it;
typedef otf2_marker_ref_to_category_map::const_iterator   otf2_marker_ref_to_category_map_cit;
typedef otf2_marker_ref_to_category_map::value_type       otf2_marker_ref_to_category_map_value;
static otf2_marker_ref_to_category_map otf2_marker_ref_to_category;

typedef std::pair< std::string, otf2_marker_category* >   otf2_marker_category_entry;
typedef std::vector< otf2_marker_category_entry >         otf2_marker_category_entry_vector;
typedef otf2_marker_category_entry_vector::iterator       otf2_marker_category_entry_vector_it;
typedef otf2_marker_category_entry_vector::const_iterator otf2_marker_category_entry_vector_cit;
typedef otf2_marker_category_entry_vector::value_type     otf2_marker_category_entry_vector_value;

static std::map< std::string, otf2_marker_category_entry_vector > otf2_marker_duplicate_marker_categories;

typedef std::pair< OTF2_MarkerRef, otf2_marker > otf2_marker_entry;
static std::vector< otf2_marker_entry > otf2_marker_invalid_markers;

/* maintain unused marker refs */
static OTF2_MarkerRef otf2_marker_max_marker_ref = OTF2_UNDEFINED_MARKER;

/** @internal
 *  @brief Defines to add a new marker definition to a marker file. */
static bool otf2_ADD_DEF = false;

/** @internal
 *  @brief Defines to add a marker to a marker file. */
static bool otf2_ADD_MARKER = false;

/** @internal
 *  @brief Defines to print a marker file. */
static bool otf2_READ = false;

/** @internal
 *  @brief Defines to print a marker file. */
static bool otf2_READ_DEF = false;

/** @internal
 *  @brief Defines to print only the definitions of an marker file. */
static bool otf2_READ_DEFS_ONLY = false;

/** @internal
 *  @brief Defines to reset a marker file. */
static bool otf2_RESET = false;

/** @internal
 *  @brief Defines to reset a marker file. */
static bool otf2_REMOVE_MARKERS = false;

/** @internal
 *  @brief Defines to remove als the defs, when removing markers. */
static bool otf2_REMOVE_DEF = false;

/** @internal
 *  @brief Defines to print a marker file. */
static bool otf2_FILE_EXISTS = false;

/** @internal
 *  @brief Marker object to store input parameter values. */
static otf2_marker otf2_INPUT_MARKER;

/**
 * Stored comandline parameters.
 */
static const char*         otf2_PARAMETER_GROUP_NAME;
static const char*         otf2_PARAMETER_CATEGORY_NAME;
static OTF2_MarkerSeverity otf2_PARAMETER_SEVERITY;

/** @internal
 *  @brief Stores global definitions for scope validation. */
static otf2_definitions otf2_DEFINITIONS;

/** @internal
 *  @brief Stores clock properties for marker time range validation. */
static otf2_clock otf2_CLOCK;

/* ___ Output handling. _____________________________________________________ */

class otf2_marker_stream : public std::ostream
{
    // Write a stream buffer that prefixes each line with Plop
    class otf2_marker_stream_buf : public std::stringbuf
    {
        std::ostream& output;
        std::string   progname;
        bool          fatal;

public:
        otf2_marker_stream_buf( std::ostream&      output_,
                                const std::string& progname_,
                                bool               fatal_ )
            : output( output_ ),
              progname( progname_ ),
              fatal( fatal_ )
        {
        };

        // When we sync the stream with the output.
        // 1) Output Plop then the buffer
        // 2) Reset the buffer
        // 3) flush the actual output stream we are using.
        virtual int
        sync( void )
        {
            output << progname << ": " << str();
            str( "" );
            output.flush();
            if ( fatal )
            {
                // Don't use std::endl here, it could result in recursion
                output << "Try '" << progname << " --help' for more information.\n";
                output.flush();
                exit( EXIT_FAILURE );
            }
            return 0;
        }
    };

    // My Stream just uses a version of my special buffer
    otf2_marker_stream_buf buffer;
public:
    otf2_marker_stream( std::ostream&      out,
                        const std::string& progName,
                        bool               fatal = false )
        : std::ostream( &buffer ),
          buffer( out, progName, fatal )
    {
    };
};

static otf2_marker_stream out( std::cout, "otf2-marker" );
static otf2_marker_stream err( std::cerr, "otf2-marker" );
static otf2_marker_stream die( std::cerr, "otf2-marker", true );

/* ___ Prototypes for static functions. _____________________________________ */


static void
otf2_get_parameters( int    argc,
                     char** argv,
                     char** anchorFile );

static void
otf2_marker_test_file( const std::string& anchorFile );

static void
otf2_marker_read_file( const std::string& anchorFile );

static void
otf2_marker_add_marker_def( void );

static void
otf2_marker_add_marker( void );

static void
otf2_marker_remove_markers( void );

static void
otf2_marker_print_marker( const otf2_marker& marker );

void
otf2_marker_print_category( const std::string&          group_name,
                            const std::string&          category_name,
                            const otf2_marker_category& marker_category );

static void
otf2_marker_print( void );

static uint64_t
otf2_parse_number( const char* option,
                   const char* argument );

static OTF2_MarkerSeverity
otf2_marker_parse_severity( const char* argument );

void
otf2_marker_format_severity( const otf2_marker_category& marker_category,
                             std::string&                severity );

static void
otf2_parse_time( otf2_marker& marker,
                 const char*  argument );

static void
otf2_marker_parse_scope( const char* argument );

static void
otf2_marker_format_scope( const otf2_marker& marker,
                          std::string&       scope );

static void
check_pointer( void*       pointer,
               const char* description );

static void
check_status( OTF2_ErrorCode status,
              const char*    description );

static OTF2_CallbackCode
cb_unknown( void* userData );

static OTF2_CallbackCode
cb_def_marker( void*               userData,
               OTF2_MarkerRef      self,
               const char*         markerGroup,
               const char*         markerCategory,
               OTF2_MarkerSeverity severity );

static OTF2_CallbackCode
cb_marker( void*            userData,
           OTF2_TimeStamp   timestamp,
           OTF2_TimeStamp   duration,
           OTF2_MarkerRef   marker,
           OTF2_MarkerScope markerScope,
           uint64_t         scope,
           const char*      text );

static OTF2_CallbackCode
cb_location( void*                 userData,
             OTF2_LocationRef      self,
             OTF2_StringRef        name,
             OTF2_LocationType     locationType,
             uint64_t              numberOfEvents,
             OTF2_LocationGroupRef locationGroup );

static OTF2_CallbackCode
cb_location_group( void*                  userData,
                   OTF2_LocationGroupRef  self,
                   OTF2_StringRef         name,
                   OTF2_LocationGroupType locationGroupType,
                   OTF2_SystemTreeNodeRef systemTreeParent );

static OTF2_CallbackCode
cb_system_tree_node( void*                  userData,
                     OTF2_SystemTreeNodeRef self,
                     OTF2_StringRef         name,
                     OTF2_StringRef         className,
                     OTF2_SystemTreeNodeRef parent );

static OTF2_CallbackCode
cb_group( void*           userData,
          OTF2_GroupRef   self,
          OTF2_StringRef  name,
          OTF2_GroupType  groupType,
          OTF2_Paradigm   paradigm,
          OTF2_GroupFlag  groupFlags,
          uint32_t        numberOfMembers,
          const uint64_t* members );

static OTF2_CallbackCode
cb_comm( void*          userData,
         OTF2_CommRef   self,
         OTF2_StringRef name,
         OTF2_GroupRef  group,
         OTF2_CommRef   parent );

static OTF2_CallbackCode
cb_clock_properties( void*    userData,
                     uint64_t timerResolution,
                     uint64_t globalOffset,
                     uint64_t traceLength );

/* ___ Implementation for static functions. _________________________________ */


/** @brief Read or edit a marker file.
 *
 *  @return         Returns EXIT_SUCCESS if successful, EXIT_FAILURE if an
 *                  error occures.
 */
int
main( int    argc,
      char** argv )
{
    char* anchor_file = NULL;
    otf2_get_parameters( argc, argv, &anchor_file );

    /* Check if marker file already exists. */
    otf2_marker_test_file( anchor_file );

    if ( otf2_FILE_EXISTS && !otf2_RESET )
    {
        otf2_marker_read_file( anchor_file );
    }

    if ( otf2_READ || otf2_READ_DEF || otf2_READ_DEFS_ONLY )
    {
        otf2_marker_print();
        /* When we print we don't do enything else. */
        return EXIT_SUCCESS;
    }

    /* Add a group. */
    if ( otf2_ADD_DEF )
    {
        otf2_marker_add_marker_def();
    }

    /* Add a marker. */
    if ( otf2_ADD_MARKER )
    {
        otf2_marker_add_marker();
    }

    /* Remove marker group. */
    if ( otf2_REMOVE_MARKERS )
    {
        otf2_marker_remove_markers();
    }

    /* Write file if a marker group or marker was added. */
    if ( otf2_ADD_DEF || otf2_ADD_MARKER || otf2_RESET || otf2_REMOVE_MARKERS )
    {
        OTF2_ErrorCode status = OTF2_ERROR_INVALID;

        /* Create new reader and marker writer handle. */
        OTF2_Reader* reader = OTF2_Reader_Open( anchor_file );
        check_pointer( reader, "Create reader." );
        OTF2_MarkerWriter* marker_writer = OTF2_Reader_GetMarkerWriter( reader );
        check_pointer( marker_writer, "Create marker writer." );

        /* Write all marker groups and according markers. */
        for ( otf2_marker_groups_map_cit i = otf2_marker_groups.begin();
              i != otf2_marker_groups.end();
              ++i )
        {
            const otf2_marker_categories_map& group = i->second;

            /* write all categories and the according markers */
            for ( otf2_marker_categories_map_cit j = group.begin(); j != group.end(); ++j )
            {
                const otf2_marker_category& marker_category( *j->second );
                status = OTF2_MarkerWriter_WriteDefMarker( marker_writer,
                                                           marker_category.ref,
                                                           i->first.c_str(),
                                                           j->first.c_str(),
                                                           marker_category.severity );
                check_status( status, "Write marker definition." );

                for ( std::vector< otf2_marker >::const_iterator k = marker_category.markers.begin();
                      k != marker_category.markers.end();
                      ++k )
                {
                    const otf2_marker& marker( *k );
                    status = OTF2_MarkerWriter_WriteMarker( marker_writer,
                                                            marker.timestamp,
                                                            marker.duration,
                                                            marker_category.ref,
                                                            marker.scope,
                                                            marker.scope_ref,
                                                            marker.text.c_str() );
                    check_status( status, "Write marker." );
                }
            }
        }

        /* Write duplicates */
        for ( std::map< std::string, otf2_marker_category_entry_vector >::const_iterator i = otf2_marker_duplicate_marker_categories.begin();
              i != otf2_marker_duplicate_marker_categories.end();
              ++i )
        {
            const std::string                        group_name( i->first );
            const otf2_marker_category_entry_vector& entries( i->second );
            for ( otf2_marker_category_entry_vector_cit j = entries.begin();
                  j != entries.end();
                  ++j )
            {
                const otf2_marker_category_entry& entry = *j;
                const std::string                 category_name( entry.first );
                const otf2_marker_category&       marker_category( *entry.second );

                status = OTF2_MarkerWriter_WriteDefMarker( marker_writer,
                                                           marker_category.ref,
                                                           i->first.c_str(),
                                                           category_name.c_str(),
                                                           marker_category.severity );
                check_status( status, "Write marker definition." );

                for ( std::vector< otf2_marker >::const_iterator k = marker_category.markers.begin();
                      k != marker_category.markers.end();
                      ++k )
                {
                    const otf2_marker& marker( *k );
                    status = OTF2_MarkerWriter_WriteMarker( marker_writer,
                                                            marker.timestamp,
                                                            marker.duration,
                                                            marker_category.ref,
                                                            marker.scope,
                                                            marker.scope_ref,
                                                            marker.text.c_str() );
                    check_status( status, "Write marker." );
                }
            }
        }

        /* Write invalid markers */
        for ( std::vector< otf2_marker_entry >::const_iterator i = otf2_marker_invalid_markers.begin();
              i != otf2_marker_invalid_markers.end();
              ++i )
        {
            const otf2_marker_entry& entry( *i );
            OTF2_MarkerRef           marker_category_ref = entry.first;
            const otf2_marker&       marker( entry.second );
            status = OTF2_MarkerWriter_WriteMarker( marker_writer,
                                                    marker.timestamp,
                                                    marker.duration,
                                                    marker_category_ref,
                                                    marker.scope,
                                                    marker.scope_ref,
                                                    marker.text.c_str() );
            check_status( status, "Write marker." );
        }

        /* Close marker writer handle. */
        status = OTF2_Reader_CloseMarkerWriter( reader, marker_writer );
        check_status( status, "Delete marker writer handle." );

        /* Finalize. */
        status = OTF2_Reader_Close( reader );
        check_status( status, "Close reader." );
    }

    return EXIT_SUCCESS;
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
void
otf2_get_parameters( int    argc,
                     char** argv,
                     char** anchorFile )
{
    /* Check if there is at least one command line parameter. */
    if ( argc < 2 )
    {
        die << "missing argument!" << std::endl;
    }

    /* Check for passed '--help' or '-h' and print help. */
    if ( !strcmp( argv[ 1 ], "--help" ) || !strcmp( argv[ 1 ], "-h" ) )
    {
        std::string usage =
            #include "otf2_marker_usage.h"
        ;
        std::cout << usage << std::endl;
        std::cout << "Report bugs to <" << PACKAGE_BUGREPORT << ">" << std::endl;
        exit( EXIT_SUCCESS );
    }

    /* Check for passed '--version' or '-V' and print version information. */
    if ( !strcmp( argv[ 1 ], "--version" ) || !strcmp( argv[ 1 ], "-V" ) )
    {
        out << "version " << OTF2_VERSION << std::endl;
        exit( EXIT_SUCCESS );
    }

    if ( !strcmp( argv[ 1 ], "--add-def" ) )
    {
        if ( argc != 6 )
        {
            die << "invalid number of arguments for operation!" << std::endl;
        }

        otf2_ADD_DEF = true;

        otf2_PARAMETER_GROUP_NAME    = argv[ 2 ];
        otf2_PARAMETER_CATEGORY_NAME = argv[ 3 ];
        otf2_PARAMETER_SEVERITY      = otf2_marker_parse_severity( argv[ 4 ] );
        *anchorFile                  = argv[ 5 ];
    }
    else if ( !strcmp( argv[ 1 ], "--add" ) )
    {
        if ( argc != 8 )
        {
            die << "invalid number of arguments for operation!" << std::endl;
        }

        otf2_PARAMETER_GROUP_NAME    = argv[ 2 ];
        otf2_PARAMETER_CATEGORY_NAME = argv[ 3 ];
        otf2_parse_time( otf2_INPUT_MARKER, argv[ 4 ] );
        otf2_marker_parse_scope( argv[ 5 ] );
        otf2_INPUT_MARKER.text = argv[ 6 ];
        *anchorFile            = argv[ 7 ];

        otf2_ADD_MARKER = true;
    }
    else if ( !strcmp( argv[ 1 ], "--reset" ) )
    {
        if ( argc != 3 )
        {
            die << "invalid number of arguments for operation!" << std::endl;
        }

        *anchorFile = argv[ 2 ];

        otf2_RESET = true;
    }
    else if ( !strcmp( argv[ 1 ], "--remove-def" ) )
    {
        if ( argc != 4 && argc != 5 )
        {
            die << "invalid number of arguments for operation!" << std::endl;
        }

        otf2_PARAMETER_GROUP_NAME = argv[ 2 ];
        if ( argc == 5 )
        {
            otf2_PARAMETER_CATEGORY_NAME = argv[ 3 ];
            *anchorFile                  = argv[ 4 ];
        }
        else
        {
            *anchorFile = argv[ 3 ];
        }

        otf2_REMOVE_MARKERS = true;
        otf2_REMOVE_DEF     = true;
    }
    else if ( !strcmp( argv[ 1 ], "--clear-def" ) )
    {
        if ( argc != 4 && argc != 5 )
        {
            die << "invalid number of arguments for operation!" << std::endl;
        }

        otf2_PARAMETER_GROUP_NAME = argv[ 2 ];
        if ( argc == 5 )
        {
            otf2_PARAMETER_CATEGORY_NAME = argv[ 3 ];
            *anchorFile                  = argv[ 4 ];
        }
        else
        {
            *anchorFile = argv[ 3 ];
        }

        otf2_REMOVE_MARKERS = true;
    }
    else if ( !strcmp( argv[ 1 ], "--def" ) )
    {
        if ( argc != 4 && argc != 5 )
        {
            die << "invalid number of arguments for operation!" << std::endl;
        }

        otf2_PARAMETER_GROUP_NAME = argv[ 2 ];
        if ( argc == 5 )
        {
            otf2_PARAMETER_CATEGORY_NAME = argv[ 3 ];
            *anchorFile                  = argv[ 4 ];
        }
        else
        {
            *anchorFile = argv[ 3 ];
        }

        otf2_READ_DEF = true;
    }
    else if ( !strcmp( argv[ 1 ], "--defs-only" ) )
    {
        if ( argc != 3 )
        {
            die << "invalid number of arguments for operation!" << std::endl;
        }

        *anchorFile = argv[ 2 ];

        otf2_READ_DEFS_ONLY = true;
    }
    else if ( argv[ 1 ][ 0 ] == '-' && argc > 2 )
    {
        die << "unrecognized option: '" << argv[ 1 ] << "'" << std::endl;
    }
    else
    {
        if ( argc != 2 )
        {
            die << "invalid number of arguments for operation!" << std::endl;
        }

        otf2_READ   = true;
        *anchorFile = argv[ 1 ];
    }

    return;
}


void
otf2_marker_test_file( const std::string& anchorFile )
{
    /* Check anchor file path for ".otf2". */
    std::string::size_type dot = anchorFile.rfind( "." );
    if ( dot == std::string::npos ||
         anchorFile.substr( dot ) != ".otf2" )
    {
        die << "invalid anchor file name: " << anchorFile << std::endl;
    }

    /* Check if marker file exists. */
    std::string marker_file = anchorFile;
    marker_file.erase( marker_file.end() - 5, marker_file.end() );
    marker_file += ".marker";

    std::fstream marker_test;
    marker_test.open( marker_file.c_str() );

    if ( marker_test.is_open() == true )
    {
        otf2_FILE_EXISTS = true;
    }
}


void
otf2_marker_read_file( const std::string& anchorFile )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* Create new reader and marker reader handle. */
    OTF2_Reader* reader = OTF2_Reader_Open( anchorFile.c_str() );
    check_pointer( reader, "Create reader." );
    OTF2_MarkerReader* marker_reader = OTF2_Reader_GetMarkerReader( reader );
    check_pointer( marker_reader, "Create marker reader." );

    /* Register callbacks. */
    OTF2_MarkerReaderCallbacks* callbacks = OTF2_MarkerReaderCallbacks_New();
    check_pointer( callbacks, "Create marker callbacks handle" );

    status = OTF2_MarkerReaderCallbacks_SetUnknownCallback( callbacks, cb_unknown );
    check_status( status, "Set Unknown callback" );
    status = OTF2_MarkerReaderCallbacks_SetDefMarkerCallback( callbacks, cb_def_marker );
    check_status( status, "Set marker definition callback" );
    status = OTF2_MarkerReaderCallbacks_SetMarkerCallback( callbacks, cb_marker );
    check_status( status, "Set marker callback" );

    status = OTF2_Reader_RegisterMarkerCallbacks( reader, marker_reader, callbacks, NULL );
    check_status( status, "Register marker callbacks." );
    OTF2_MarkerReaderCallbacks_Delete( callbacks );

    /* Read markers. */
    uint64_t markers_read = 0;
    status = OTF2_Reader_ReadAllMarkers( reader, marker_reader, &markers_read );

    /* ___ Read global definitions to validate IDs for marker scopes. ___ */
    /* Create global definitions reader handle. */
    OTF2_GlobalDefReader* def_reader = OTF2_Reader_GetGlobalDefReader( reader );
    check_pointer( def_reader, "Create global definitions reader." );

    /* Register callbacks for global definitions reader. */
    OTF2_GlobalDefReaderCallbacks* def_callbacks = OTF2_GlobalDefReaderCallbacks_New();
    check_pointer( def_callbacks, "Create global defintion callbacks handle" );

    status = OTF2_GlobalDefReaderCallbacks_SetLocationCallback( def_callbacks, cb_location );
    check_status( status, "Set location defintion callback" );
    status = OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback( def_callbacks, cb_location_group );
    check_status( status, "Set location group defintion callback" );
    status = OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback( def_callbacks, cb_system_tree_node );
    check_status( status, "Set system tree node defintion callback" );
    status = OTF2_GlobalDefReaderCallbacks_SetGroupCallback( def_callbacks, cb_group );
    check_status( status, "Set group defintion callback" );
    status = OTF2_GlobalDefReaderCallbacks_SetCommCallback( def_callbacks, cb_comm );
    check_status( status, "Set communicator defintion callback" );
    status = OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback( def_callbacks, cb_clock_properties );
    check_status( status, "Set clock properties defintion callback" );


    status = OTF2_Reader_RegisterGlobalDefCallbacks( reader, def_reader, def_callbacks, NULL );
    check_status( status, "Register global definition callbacks." );
    OTF2_GlobalDefReaderCallbacks_Delete( def_callbacks );

    /* Read global definitions. */
    uint64_t defs_read = 0;
    status = OTF2_Reader_ReadAllGlobalDefinitions( reader, def_reader, &defs_read );

    status = OTF2_Reader_Close( reader );
    check_status( status, "Close reader." );

    return;
}


void
otf2_marker_add_marker_def( void )
{
    otf2_marker_category& new_category( *new otf2_marker_category );
    new_category.ref      = ++otf2_marker_max_marker_ref;
    new_category.severity = otf2_PARAMETER_SEVERITY;

    std::string group_name    = otf2_PARAMETER_GROUP_NAME;
    std::string category_name = otf2_PARAMETER_CATEGORY_NAME;

    if ( otf2_marker_groups.find( group_name ) == otf2_marker_groups.end() )
    {
        otf2_marker_groups[ group_name ] = otf2_marker_categories_map();
    }
    otf2_marker_categories_map& group = otf2_marker_groups[ group_name ];

    if ( group.find( category_name ) != group.end() )
    {
        die << "marker category already exists for group \""
            << otf2_PARAMETER_GROUP_NAME << "\": \""
            << otf2_PARAMETER_CATEGORY_NAME << "\"" << std::endl;
    }

    group[ category_name ]                          = &new_category;
    otf2_marker_ref_to_category[ new_category.ref ] = &new_category;
}


void
otf2_marker_add_marker( void )
{
    std::string group_name    = otf2_PARAMETER_GROUP_NAME;
    std::string category_name = otf2_PARAMETER_CATEGORY_NAME;

    if ( otf2_marker_groups.find( group_name ) == otf2_marker_groups.end() )
    {
        die << "undefined marker group: \""
            << otf2_PARAMETER_GROUP_NAME << "\"" << std::endl;
    }
    otf2_marker_categories_map& group = otf2_marker_groups[ group_name ];

    if ( group.find( category_name ) == group.end() )
    {
        die << "undefined marker category for group \""
            << otf2_PARAMETER_GROUP_NAME << "\": \""
            << otf2_PARAMETER_CATEGORY_NAME << "\"" << std::endl;
    }
    otf2_marker_category& marker_category( *group[ category_name ] );

    /* Check if scope id exists for given scope. */
    switch ( otf2_INPUT_MARKER.scope )
    {
        case OTF2_MARKER_SCOPE_GLOBAL:
            break;
        case OTF2_MARKER_SCOPE_LOCATION:
            if ( otf2_DEFINITIONS.locations.count( otf2_INPUT_MARKER.scope_ref ) == 0 )
            {
                die << "invalid location reference: " << otf2_INPUT_MARKER.scope_ref << std::endl;
            }
            break;
        case OTF2_MARKER_SCOPE_LOCATION_GROUP:
            if ( otf2_DEFINITIONS.location_groups.count( otf2_INPUT_MARKER.scope_ref ) == 0 )
            {
                die << "invalid location group reference: " << otf2_INPUT_MARKER.scope_ref << std::endl;
            }
            break;
        case OTF2_MARKER_SCOPE_SYSTEM_TREE_NODE:
            if ( otf2_DEFINITIONS.system_tree_nodes.count( otf2_INPUT_MARKER.scope_ref ) == 0 )
            {
                die << "invalid system tree node reference: " << otf2_INPUT_MARKER.scope_ref << std::endl;
            }
            break;
        case OTF2_MARKER_SCOPE_GROUP:
            if ( otf2_DEFINITIONS.groups.count( otf2_INPUT_MARKER.scope_ref ) == 0 )
            {
                die << "invalid group reference: " << otf2_INPUT_MARKER.scope_ref << std::endl;
            }
            break;
        case OTF2_MARKER_SCOPE_COMM:
            if ( otf2_DEFINITIONS.comms.count( otf2_INPUT_MARKER.scope_ref ) == 0 )
            {
                die << "invalid communicator reference: " << otf2_INPUT_MARKER.scope_ref << std::endl;
            }
            break;
        default:
            die << "invalid scope: " << ( int )otf2_INPUT_MARKER.scope << std::endl;
    }

    /* Check if timestamp and duration match clock properties. */
    if ( otf2_INPUT_MARKER.timestamp < otf2_CLOCK.offset ||
         otf2_INPUT_MARKER.timestamp > ( otf2_CLOCK.offset + otf2_CLOCK.length ) )
    {
        die << "timestamp is out of range ["
            << otf2_CLOCK.offset << ", "
            << ( otf2_CLOCK.offset + otf2_CLOCK.length ) << "]: "
            << otf2_INPUT_MARKER.timestamp << std::endl;
    }
    if ( ( otf2_INPUT_MARKER.timestamp + otf2_INPUT_MARKER.duration ) >
         ( otf2_CLOCK.offset + otf2_CLOCK.length ) )
    {
        die << "duration is out of range [0, "
            << otf2_CLOCK.length << "]: "
            << otf2_INPUT_MARKER.duration << std::endl;
    }

    otf2_marker new_marker;

    new_marker.timestamp = otf2_INPUT_MARKER.timestamp;
    new_marker.duration  = otf2_INPUT_MARKER.duration;
    new_marker.scope     = otf2_INPUT_MARKER.scope;
    new_marker.scope_ref = otf2_INPUT_MARKER.scope_ref;
    new_marker.text      = otf2_INPUT_MARKER.text;

    marker_category.markers.push_back( new_marker );
}


void
otf2_marker_remove_markers( void )
{
    std::string group_name = otf2_PARAMETER_GROUP_NAME;
    if ( otf2_marker_groups.find( group_name ) == otf2_marker_groups.end() )
    {
        die << "undefined marker group: \""
            << otf2_PARAMETER_GROUP_NAME << "\"" << std::endl;
    }
    otf2_marker_categories_map& group = otf2_marker_groups[ group_name ];

    if ( otf2_PARAMETER_CATEGORY_NAME )
    {
        /* remove only this particular marker category */
        std::string category_name = otf2_PARAMETER_CATEGORY_NAME;
        if ( group.find( category_name ) == group.end() )
        {
            die << "undefined marker category for group \""
                << otf2_PARAMETER_GROUP_NAME << "\": \""
                << otf2_PARAMETER_CATEGORY_NAME << "\"" << std::endl;
        }
        otf2_marker_category& marker_category( *group[ category_name ] );
        if ( otf2_REMOVE_DEF )
        {
            group.erase( category_name );
            otf2_marker_ref_to_category.erase( marker_category.ref );
            delete &marker_category;
        }
        else
        {
            marker_category.markers.clear();
        }
        return;
    }

    /* remove all categories and the group itself */
    for ( otf2_marker_categories_map_it i = group.begin(); i != group.end(); ++i )
    {
        otf2_marker_category& marker_category( *i->second );
        if ( otf2_REMOVE_DEF )
        {
            group.erase( i->first );
            otf2_marker_ref_to_category.erase( marker_category.ref );
            delete &marker_category;
        }
        else
        {
            marker_category.markers.clear();
        }
    }
    if ( otf2_REMOVE_DEF )
    {
        otf2_marker_groups.erase( group_name );
    }
}

void
otf2_marker_print_marker( const otf2_marker& marker )
{
    std::string scope;
    otf2_marker_format_scope( marker, scope );
    std::cout << "MARKER      "
              << "Time: "       << marker.timestamp << ", "
              << "Duration "    << marker.duration << ", "
              << "Scope: "      << scope << ", "
              << "Text: \""     << marker.text << "\"" << std::endl;
}


void
otf2_marker_print_category( const std::string&          group_name,
                            const std::string&          category_name,
                            const otf2_marker_category& marker_category )
{
    std::string severity;
    otf2_marker_format_severity( marker_category, severity );
    std::cout << "MARKER_DEF  "
              << "Group: \""    << group_name << "\", "
              << "Category: \"" << category_name << "\", "
              << "Severity: "   << severity
              << std::endl;

    if ( otf2_READ_DEFS_ONLY )
    {
        return;
    }

    for ( std::vector< otf2_marker >::const_iterator i = marker_category.markers.begin();
          i != marker_category.markers.end();
          ++i )
    {
        otf2_marker_print_marker( *i );
    }
}

void
otf2_marker_print( void )
{
    if ( otf2_READ_DEF )
    {
        std::string group_name = otf2_PARAMETER_GROUP_NAME;
        if ( otf2_marker_groups.find( group_name ) == otf2_marker_groups.end() )
        {
            die << "undefined marker group: \""
                << otf2_PARAMETER_GROUP_NAME << "\"" << std::endl;
        }
        otf2_marker_categories_map& group = otf2_marker_groups[ group_name ];

        if ( otf2_PARAMETER_CATEGORY_NAME )
        {
            /* print only this particular marker category */
            std::string category_name = otf2_PARAMETER_CATEGORY_NAME;
            if ( group.find( category_name ) == group.end() )
            {
                die << "undefined marker category for group \""
                    << otf2_PARAMETER_GROUP_NAME << "\": \""
                    << otf2_PARAMETER_CATEGORY_NAME << "\"" << std::endl;
            }
            const otf2_marker_category& marker_category( *group[ category_name ] );
            otf2_marker_print_category( group_name,
                                        category_name,
                                        marker_category );
            return;
        }

        /* print all categories and the group itself */
        for ( otf2_marker_categories_map_cit i = group.begin(); i != group.end(); ++i )
        {
            const otf2_marker_category& marker_category( *i->second );
            otf2_marker_print_category( group_name,
                                        i->first,
                                        marker_category );
        }
        return;
    }

    /* Print all marker groups and according markers. */
    for ( otf2_marker_groups_map_cit i = otf2_marker_groups.begin();
          i != otf2_marker_groups.end();
          ++i )
    {
        const otf2_marker_categories_map& group = i->second;

        /* print all categories and the group itself */
        for ( otf2_marker_categories_map_cit j = group.begin(); j != group.end(); ++j )
        {
            const otf2_marker_category& marker_category( *j->second );
            otf2_marker_print_category( i->first,
                                        j->first,
                                        marker_category );
        }
    }
}



uint64_t
otf2_parse_number( const char* option,
                   const char* argument )
{
    const char* p      = argument;
    uint64_t    number = 0;
    while ( *p )
    {
        if ( ( *p < '0' ) || ( *p > '9' ) )
        {
            die << "invalid number argument for " << option << ": "
                << argument << std::endl;
        }
        uint64_t new_number = number * 10 + *p - '0';
        if ( new_number < number )
        {
            die << "number argument to large for " << option << ": "
                << argument << std::endl;
        }
        number = new_number;
        p++;
    }
    if ( p == argument )
    {
        die << "empty number argument for " << option << std::endl;
    }

    return number;
}

OTF2_MarkerSeverity
otf2_marker_parse_severity( const char* argument )
{
    std::string severity( argument );
    if ( !severity.compare( "NONE" ) || !severity.compare( "none" ) )
    {
        return OTF2_SEVERITY_NONE;
    }
    if ( !severity.compare( "LOW" )  || !severity.compare( "low" ) )
    {
        return OTF2_SEVERITY_LOW;
    }
    if ( !severity.compare( "MEDIUM" )  || !severity.compare( "medium" ) )
    {
        return OTF2_SEVERITY_MEDIUM;
    }
    if ( !severity.compare( "HIGH" )  || !severity.compare( "high" ) )
    {
        return OTF2_SEVERITY_HIGH;
    }

    die << "invalid <SEVERITY> level: " << argument << std::endl;

    return OTF2_UNDEFINED_TYPE;
}

void
otf2_marker_format_severity( const otf2_marker_category& marker_category,
                             std::string&                severity )
{
    std::stringstream value;
    switch ( marker_category.severity )
    {
        case OTF2_SEVERITY_NONE:
            value << "NONE";
            break;
        case OTF2_SEVERITY_LOW:
            value << "LOW";
            break;
        case OTF2_SEVERITY_MEDIUM:
            value << "MEDIUM";
            break;
        case OTF2_SEVERITY_HIGH:
            value << "HIGH";
            break;

        default:
            value << "INVALID <" << ( int )marker_category.severity << ">";
    }

    severity = value.str();
}

void
otf2_parse_time( otf2_marker& marker,
                 const char*  argument )
{
    const char* timearg1_name = "<TIMESTAMP>";
    std::string timearg1      = argument;
    const char* timearg2_name;
    std::string timearg2;
    const char* p = NULL;

    if ( ( p = strchr( argument, '+' ) ) )
    {
        timearg1.assign( argument, p - argument );
        timearg2_name = "<DURATION>";
        timearg2.assign( p + 1 );
    }
    else if ( ( p = strchr( argument, '-' ) ) )
    {
        timearg1_name = "<TIMESTAMP-START>";
        timearg1.assign( argument, p - argument );
        timearg2_name = "<TIMESTAMP-END>";
        timearg2.assign( p + 1 );
    }

    marker.timestamp = otf2_parse_number( timearg1_name, timearg1.c_str() );
    marker.duration  = 0;
    if ( p )
    {
        marker.duration = otf2_parse_number( timearg2_name, timearg2.c_str() );
        if ( *p == '-' )
        {
            if ( marker.duration < marker.timestamp )
            {
                die << "invalid <TIMESTAMP-END> argument: "
                    << marker.duration << std::endl;
            }
            marker.duration -= marker.timestamp;
        }
    }
}

void
otf2_marker_parse_scope( const char* argument )
{
    std::string scope_total( argument );

    std::string::size_type colon = scope_total.find( ":" );
    std::string            scope = scope_total.substr( 0, colon );

    if ( colon == std::string::npos && scope.compare( "GLOBAL" ) && scope.compare( "global" ) )
    {
        die << "missing <SCOPE-REF> argument" << std::endl;
    }

    otf2_INPUT_MARKER.scope_ref = OTF2_UNDEFINED_UINT64;
    if ( colon != std::string::npos && scope.compare( "GLOBAL" ) && scope.compare( "global" ) )
    {
        otf2_INPUT_MARKER.scope_ref = otf2_parse_number( "<SCOPE-REF>", scope_total.substr( colon + 1 ).c_str() );
    }

    if ( !scope.compare( "GLOBAL" ) || !scope.compare( "global" ) )
    {
        otf2_INPUT_MARKER.scope = OTF2_MARKER_SCOPE_GLOBAL;
        return;
    }
    if ( !scope.compare( "LOCATION" ) || !scope.compare( "location" ) )
    {
        otf2_INPUT_MARKER.scope = OTF2_MARKER_SCOPE_LOCATION;
        return;
    }
    if ( !scope.compare( "LOCATION_GROUP" ) || !scope.compare( "location_group" ) )
    {
        otf2_INPUT_MARKER.scope = OTF2_MARKER_SCOPE_LOCATION_GROUP;
        return;
    }
    if ( !scope.compare( "SYSTEM_TREE_NODE" ) || !scope.compare( "system_tree_node" ) )
    {
        otf2_INPUT_MARKER.scope = OTF2_MARKER_SCOPE_SYSTEM_TREE_NODE;
        return;
    }
    if ( !scope.compare( "GROUP" ) || !scope.compare( "group" ) )
    {
        otf2_INPUT_MARKER.scope = OTF2_MARKER_SCOPE_GROUP;
        return;
    }
    if ( !scope.compare( "COMM" ) || !scope.compare( "comm" ) )
    {
        otf2_INPUT_MARKER.scope = OTF2_MARKER_SCOPE_COMM;
        return;
    }

    die << "invalid <SCOPE>: " << argument << std::endl;
}

void
otf2_marker_format_scope( const otf2_marker& marker,
                          std::string&       scope )
{
    std::stringstream value;
    switch ( marker.scope )
    {
        case OTF2_MARKER_SCOPE_GLOBAL:
            value << "GLOBAL";
            break;
        case OTF2_MARKER_SCOPE_LOCATION:
            value << "LOCATION:" << marker.scope_ref;
            break;
        case OTF2_MARKER_SCOPE_LOCATION_GROUP:
            value << "LOCATION_GROUP:" << marker.scope_ref;
            break;
        case OTF2_MARKER_SCOPE_SYSTEM_TREE_NODE:
            value << "SYSTEM_TREE_NODE:" << marker.scope_ref;
            break;
        case OTF2_MARKER_SCOPE_GROUP:
            value << "GROUP:" << marker.scope_ref;
            break;
        case OTF2_MARKER_SCOPE_COMM:
            value << "COMM:" << marker.scope_ref;
            break;
        default:
            value << "INVALID <" << ( int )marker.scope << ">:" << marker.scope_ref;
    }

    scope = value.str();
}

static void
check_pointer( void*       pointer,
               const char* description )
{
    if ( pointer == NULL )
    {
        die << description << std::endl;
    }
}


void
check_status( OTF2_ErrorCode status,
              const char*    description )
{
    if ( status != OTF2_SUCCESS )
    {
        die << description << std::endl;
    }
}


/** @brief Callback for a unknown record. */
OTF2_CallbackCode
cb_unknown( void* userData )
{
    ( void )userData;

    err << "unknown marker record" << std::endl;

    return OTF2_CALLBACK_SUCCESS;
}


/** @brief Callback for a marker definition record. */
OTF2_CallbackCode
cb_def_marker( void*               userData,
               OTF2_MarkerRef      self,
               const char*         markerGroup,
               const char*         markerCategory,
               OTF2_MarkerSeverity severity )
{
    ( void )userData;

    if ( otf2_marker_max_marker_ref == OTF2_UNDEFINED_MARKER || self > otf2_marker_max_marker_ref )
    {
        otf2_marker_max_marker_ref = self;
    }

    std::string           marker_group( markerGroup );
    std::string           marker_category( markerCategory );
    otf2_marker_category& new_category( *new otf2_marker_category );
    new_category.ref      = self;
    new_category.severity = severity;

    if ( otf2_marker_ref_to_category.find( self ) != otf2_marker_ref_to_category.end() )
    {
        err << "duplicate marker definition: " << self << std::endl;
        if ( otf2_marker_duplicate_marker_categories.find( marker_group ) == otf2_marker_duplicate_marker_categories.end() )
        {
            otf2_marker_duplicate_marker_categories[ marker_group ] = otf2_marker_category_entry_vector();
        }
        otf2_marker_duplicate_marker_categories[ marker_group ].push_back( otf2_marker_category_entry( marker_category, &new_category ) );
        return OTF2_CALLBACK_SUCCESS;
    }
    otf2_marker_ref_to_category[ self ] = &new_category;

    if ( otf2_marker_groups.find( marker_group ) == otf2_marker_groups.end() )
    {
        otf2_marker_groups[ marker_group ] = otf2_marker_categories_map();
    }
    otf2_marker_categories_map& group = otf2_marker_groups[ marker_group ];

    if ( group.find( marker_category ) != group.end() )
    {
        err << "duplicate marker category for group \""
            << markerGroup << "\": \""
            << markerCategory << "\"" << std::endl;
        if ( otf2_marker_duplicate_marker_categories.find( marker_group ) == otf2_marker_duplicate_marker_categories.end() )
        {
            otf2_marker_duplicate_marker_categories[ marker_group ] = otf2_marker_category_entry_vector();
        }
        otf2_marker_duplicate_marker_categories[ marker_group ].push_back( otf2_marker_category_entry( marker_category, &new_category ) );
        return OTF2_CALLBACK_SUCCESS;
    }

    group[ marker_category ] = &new_category;

    return OTF2_CALLBACK_SUCCESS;
}


/** @brief Callback for a marker record. */
OTF2_CallbackCode
cb_marker( void*            userData,
           OTF2_TimeStamp   timestamp,
           OTF2_TimeStamp   duration,
           OTF2_MarkerRef   marker,
           OTF2_MarkerScope scope,
           uint64_t         scopeRef,
           const char*      text )
{
    ( void )userData;

    otf2_marker new_marker;

    new_marker.timestamp = timestamp;
    new_marker.duration  = duration;
    new_marker.scope     = scope;
    new_marker.scope_ref = scopeRef;
    new_marker.text      = text;

    if ( otf2_marker_max_marker_ref == OTF2_UNDEFINED_MARKER || marker > otf2_marker_max_marker_ref )
    {
        otf2_marker_max_marker_ref = marker;
    }

    if ( otf2_marker_ref_to_category.find( marker ) == otf2_marker_ref_to_category.end() )
    {
        err << "unknown marker reference: " << marker << std::endl;
        otf2_marker_invalid_markers.push_back( otf2_marker_entry( marker, new_marker ) );
        return OTF2_CALLBACK_SUCCESS;
    }
    otf2_marker_category& marker_category( *otf2_marker_ref_to_category[ marker ] );

    marker_category.markers.push_back( new_marker );

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
cb_location( void*                 userData,
             OTF2_LocationRef      self,
             OTF2_StringRef        name,
             OTF2_LocationType     locationType,
             uint64_t              numberOfEvents,
             OTF2_LocationGroupRef locationGroup )
{
    otf2_DEFINITIONS.locations.insert( self );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
cb_location_group( void*                  userData,
                   OTF2_LocationGroupRef  self,
                   OTF2_StringRef         name,
                   OTF2_LocationGroupType locationGroupType,
                   OTF2_SystemTreeNodeRef systemTreeParent )
{
    otf2_DEFINITIONS.location_groups.insert( self );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
cb_system_tree_node( void*                  userData,
                     OTF2_SystemTreeNodeRef self,
                     OTF2_StringRef         name,
                     OTF2_StringRef         className,
                     OTF2_SystemTreeNodeRef parent )
{
    otf2_DEFINITIONS.system_tree_nodes.insert( self );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
cb_group( void*           userData,
          OTF2_GroupRef   self,
          OTF2_StringRef  name,
          OTF2_GroupType  groupType,
          OTF2_Paradigm   paradigm,
          OTF2_GroupFlag  groupFlags,
          uint32_t        numberOfMembers,
          const uint64_t* members )
{
    if ( groupType == OTF2_GROUP_TYPE_LOCATIONS ||
         groupType == OTF2_GROUP_TYPE_COMM_LOCATIONS )
    {
        otf2_DEFINITIONS.groups.insert( self );
    }

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
cb_comm( void*          userData,
         OTF2_CommRef   self,
         OTF2_StringRef name,
         OTF2_GroupRef  group,
         OTF2_CommRef   parent )
{
    otf2_DEFINITIONS.comms.insert( self );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
cb_clock_properties( void*    userData,
                     uint64_t timerResolution,
                     uint64_t globalOffset,
                     uint64_t traceLength )
{
    otf2_CLOCK.resolution = timerResolution;
    otf2_CLOCK.offset     = globalOffset;
    otf2_CLOCK.length     = traceLength;

    return OTF2_CALLBACK_SUCCESS;
}
