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

#include <config.h>

#define __STDC_FORMAT_MACROS    // for PRIu8 in inttypes.h
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <iostream>

#include <otf2/otf2.h>

#include "otf2_snapshots_parameters.hpp"


/* ___ Global variables. ____________________________________________________ */

/** @internal
 *  @brief Name of the program. */
static const char* otf2_NAME;


/** @brief Self-explanatory. */
static inline void
print_help( void )
{
    printf(
        #include "otf2_snapshots_usage.h"
        "\n"
        "Report bugs to <%s>\n",
        PACKAGE_BUGREPORT );
}

/** @brief Inintialize all member of the otf2_snapshots_param struct. */
static inline void
initialize_otf2_snapshots_param( otf2_snapshots_param* param )
{
    param->archive_path         = NULL;
    param->archive_name         = NULL;
    param->number_breaks        = OTF2_UNDEFINED_UINT64;
    param->tick_rate_for_breaks = OTF2_UNDEFINED_UINT64;
    param->progress             = false;
    param->verbose              = false;
}

static void
otf2_snapshot_die( const char* fmt,
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
            otf2_snapshot_die( "invalid number argument for %s: %s\n",
                               option, argument );
        }
        uint64_t new_number = *number * 10 + *p - '0';
        if ( new_number < *number )
        {
            otf2_snapshot_die( "number argument to large for '%s': %s\n",
                               option, argument );
        }
        *number = new_number;
        p++;
    }
    if ( p == argument )
    {
        otf2_snapshot_die( "empty number argument for '%s'\n", option );
    }
}


/** @brief Get command line parameters.
 *
 *  Parses command line parameters and checks for their existence.
 *  Prints help for parameters '-h' or '--help'.
 *
 *  @param argc  Programs argument counter.
 *  @param argv  Programs argument values.
 *  @param param Return pointer for the parameters.
 */
void
get_parameters( int                   argc,
                char**                argv,
                otf2_snapshots_param* param )
{
    initialize_otf2_snapshots_param( param );

    otf2_NAME = strrchr( argv[ 0 ], '/' );
    if ( otf2_NAME )
    {
        otf2_NAME++;
    }
    else
    {
        otf2_NAME = argv[ 0 ];
    }

    int i = 0;

    for ( i = 1; i < argc; i++ )
    {
        if ( !strcmp( argv[ i ], "--help" ) || !strcmp( argv[ i ], "-h" ) )
        {
            print_help();
            exit( EXIT_SUCCESS );
        }

        else if ( !strcmp( argv[ i ], "--number" ) || !strcmp( argv[ i ], "-n" ) )
        {
            char* opt = argv[ i ];
            char* arg;

            if ( i + 1 >= argc )
            {
                otf2_snapshot_die( "missing argument for '%s'\n", opt );
            }
            arg = argv[ i + 1 ];
            i++;

            otf2_parse_number_argument( opt, arg, &( param->number_breaks ) );
        }

        else if ( !strcmp( argv[ i ], "-p" ) )
        {
            char* opt = argv[ i ];
            char* arg;

            if ( i + 1 >= argc )
            {
                otf2_snapshot_die( "missing argument for '%s'\n", opt );
            }
            arg = argv[ i + 1 ];
            i++;

            otf2_parse_number_argument( opt, arg, &( param->tick_rate_for_breaks ) );
        }

        else if ( !strcmp( argv[ i ], "--progress" ) )
        {
            param->progress = true;
        }

        else if ( !strcmp( argv[ i ], "--verbose" ) )
        {
            param->verbose = true;
        }

        else if ( !strcmp( argv[ i ], "--version" ) || !strcmp( argv[ i ], "-V" ) )
        {
            printf( "otf2-snapshots: version %s\n", OTF2_VERSION );
            exit( EXIT_SUCCESS );
        }

        else
        {
            break;
        }
    }

    /* ___ Split the path to the anchor file into path and archive name. ____ */

    if ( 1 != argc - i )
    {
        otf2_snapshot_die( "missing or too many anchorfile argument(s)\n" );
    }

    int   path_len;
    char* last_slash = strrchr( argv[ i ], '/' );

    if ( last_slash )
    {
        param->archive_name = last_slash + 1;
        path_len            = strlen( argv[ i ] ) - strlen( param->archive_name );
        param->archive_path = new char[ path_len + 1 ];
        strncpy( param->archive_path, argv[ i ], path_len );
        param->archive_path[ path_len ] = '\0';
    }
    else
    {
        param->archive_name = argv[ i ];
        param->archive_path = new char[ 2 ];
        strcpy( param->archive_path, "." );
    }

    /* Remove the .otf2 from the archive name. */
    char* extension = strstr( param->archive_name, ".otf2" );
    if ( extension )
    {
        extension[ 0 ] = '\0';
    }
    else
    {
        otf2_snapshot_die( "missing anchorfile\n" );
    }
}
