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
 * @status alpha
 *
 * @maintainer Daniel Lorenz <d.lorenz@fz-juelich.de>
 * @authors    Johannes Spazier <johannes.spazier@tu-dresden.de>
 *
 */

#include <config.h>

#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include <otf2/OTF2_GeneralDefinitions.h>

#include <UTILS_IO.h>

#include "otf2_config_tool_backend.h"
#include "otf2_config_tool_frontend.h"
#include "otf2_config_library_dependencies.hpp"

#define ACTION_LIBS 1
#define ACTION_CFLAGS 2
#define ACTION_LDFLAGS 3
#define ACTION_CPPFLAGS 4
#define ACTION_CC 5

#define SHORT_HELP \
    "Try 'otf2-config --help' for more information."


void
get_rpath_struct_data( bool backend );

std::string
prepare_string( std::string str,
                bool        backend );

std::string
remove_multiple_whitespaces( std::string str );

std::string
replace_all( const std::string &pattern,
             const std::string &replacement,
             std::string        original );

std::string
append_ld_run_path_to_rpath( std::string rpath );

std::string m_rpath_head      = "";
std::string m_rpath_delimiter = "";
std::string m_rpath_tail      = "";

int
main( int    argc,
      char** argv )
{
    int  action   = 0;
    int  ret      = EXIT_SUCCESS;
    bool frontend = true;
    bool cuda     = false;
    bool install  = true;

    OTF2_Config_LibraryDependencies deps;

    /* parsing the command line */
    for ( int i = 1; i < argc; i++ )
    {
        if ( strcmp( argv[ i ], "--help" ) == 0 || strcmp( argv[ i ], "-h" ) == 0 )
        {
            std::string usage =
                #include "otf2_config_usage.h"
            ;
            std::cout << usage << std::endl;
            std::cout << "Report bugs to <" << PACKAGE_BUGREPORT << ">" << std::endl;
            exit( EXIT_SUCCESS );
        }
        else if ( strcmp( argv[ i ], "--libs" ) == 0 )
        {
            action = ACTION_LIBS;
        }
        else if ( strcmp( argv[ i ], "--cflags" ) == 0 )
        {
            action = ACTION_CFLAGS;
        }
        else if ( strcmp( argv[ i ], "--ldflags" ) == 0 )
        {
            action = ACTION_LDFLAGS;
        }
        else if ( strcmp( argv[ i ], "--cppflags" ) == 0 )
        {
            action = ACTION_CPPFLAGS;
        }
        else if ( strcmp( argv[ i ], "--cc" ) == 0 )
        {
            action = ACTION_CC;
        }
        else if ( strcmp( argv[ i ], "--backend" ) == 0 )
        {
            frontend = false;
        }
        else if ( strcmp( argv[ i ], "--cuda" ) == 0 )
        {
            cuda = true;
        }
        else if ( strcmp( argv[ i ], "--version" ) == 0 )
        {
            std::cout << OTF2_VERSION << std::endl;
            exit( EXIT_SUCCESS );
        }
        else if ( strcmp( argv[ i ], "--otf2-revision" ) == 0 )
        {
            std::cout << SCOREP_COMPONENT_REVISION << std::endl;
            exit( EXIT_SUCCESS );
        }
        else if ( strcmp( argv[ i ], "--common-revision" ) == 0 )
        {
            std::cout << SCOREP_COMMON_REVISION << std::endl;
            exit( EXIT_SUCCESS );
        }
        else if ( strcmp( argv[ i ], "--interface-version" ) == 0 )
        {
            std::cout << LIBRARY_INTERFACE_VERSION << std::endl;
            exit( EXIT_SUCCESS );
        }
        else if ( strcmp( argv[ i ], "--build-check" ) == 0 )
        {
            install = false;
        }
        else
        {
            std::cerr << "otf2-config: unrecognized option '" << argv[ i ] << "'" << std::endl;
            std::cerr << SHORT_HELP << std::endl;
            exit( EXIT_FAILURE );
        }
    }

    if ( 0 == action )
    {
        std::cerr << "otf2-config: no command specified" << std::endl;
        std::cerr << SHORT_HELP << std::endl;
        exit( EXIT_FAILURE );
    }

    std::string             str;
    std::string             temp;
    std::deque<std::string> libs;
    libs.push_back( "libotf2" );

    /* print data for frontend */
    if ( frontend )
    {
        switch ( action )
        {
            case ACTION_LIBS:

                /* Do not append a newline.
                   One intended use case is to insert a call to otf2_config
                   in backticks into the build command. E.g.
                   gcc `otf2_config --libs` mycode.c -o mycode
                   This does not work anymore if you insert newlines. */
                std::cout << deps.GetLibraries( libs, frontend ) << std::endl;
                break;

            case ACTION_LDFLAGS:
                get_rpath_struct_data( false );
                std::cout << deps.GetLDFlags( libs, install, frontend );
                if ( OTF2_FRONTEND_USE_RPATH_FLAG )
                {
                    str = deps.GetRpathFlags( libs, install, frontend,
                                              m_rpath_head,
                                              m_rpath_delimiter,
                                              m_rpath_tail );
                    str = append_ld_run_path_to_rpath( str );
                }
                if ( cuda )
                {
                    str = " -Xlinker " + prepare_string( str, false );
                }

                /* Do not append a newline.
                   One intended use case is to insert a call to otf2_config
                   in backticks into the build command. E.g.
                   gcc `otf2_config --libs` mycode.c -o mycode
                   This does not work anymore if you insert newlines. */
                std::cout << str << std::endl;
                break;

            case ACTION_CFLAGS:
            case ACTION_CPPFLAGS:
                str += "-I" OTF2_FRONTEND_PREFIX "/include ";
                str += OTF2_FRONTEND_SION_CPPFLAGS " ";
                if ( cuda )
                {
                    /* Construct string in multiple steps to please IBM compiler. */
                    temp = prepare_string( str, false );
                    str  = " -Xcompiler ";
                    str += temp.c_str();
                }

                /* Do not append a newline.
                   One intended use case is to insert a call to otf2_config
                   in backticks into the build command. E.g.
                   gcc `otf2_config --libs` mycode.c -o mycode
                   This does not work anymore if you insert newlines. */
                std::cout << str.c_str() << std::endl;
                break;

            case ACTION_CC:

                /* Do not append a newline.
                   One intended use case is to insert a call to otf2_config
                   in backticks into the build command. E.g.
                   gcc `otf2_config --libs` mycode.c -o mycode
                   This does not work anymore if you insert newlines. */
                std::cout << OTF2_FRONTEND_CC << std::endl;
                break;
        }
    }
    /* print data for backend */
    else
    {
        switch ( action )
        {
            case ACTION_LIBS:

                /* Do not append a newline.
                   One intended use case is to insert a call to otf2_config
                   in backticks into the build command. E.g.
                   gcc `otf2_config --libs` mycode.c -o mycode
                   This does not work anymore if you insert newlines. */
                std::cout << deps.GetLibraries( libs, frontend ) << std::endl;
                break;

            case ACTION_LDFLAGS:
                get_rpath_struct_data( false );
                std::cout << deps.GetLDFlags( libs, install, frontend );
                if ( OTF2_BACKEND_USE_RPATH_FLAG )
                {
                    str = deps.GetRpathFlags( libs, install, frontend,
                                              m_rpath_head,
                                              m_rpath_delimiter,
                                              m_rpath_tail );
                    str = append_ld_run_path_to_rpath( str );
                }
                if ( cuda )
                {
                    str = " -Xlinker " + prepare_string( str, false );
                }

                /* Do not append a newline.
                   One intended use case is to insert a call to otf2_config
                   in backticks into the build command. E.g.
                   gcc `otf2_config --libs` mycode.c -o mycode
                   This does not work anymore if you insert newlines. */
                std::cout << str << std::endl;
                break;

            case ACTION_CFLAGS:
            case ACTION_CPPFLAGS:
                str += "-I" OTF2_BACKEND_PREFIX "/include ";
                str +=  OTF2_BACKEND_SION_CPPFLAGS " ";
                if ( cuda )
                {
                    /* Construct string in multiple steps to please IBM compiler. */
                    temp = prepare_string( str, false );
                    str  = " -Xcompiler ";
                    str += temp.c_str();
                }

                /* Do not append a newline.
                   One intended use case is to insert a call to otf2_config
                   in backticks into the build command. E.g.
                   gcc `otf2_config --libs` mycode.c -o mycode
                   This does not work anymore if you insert newlines. */
                std::cout << str.c_str() << std::endl;
                break;

            case ACTION_CC:

                /* Do not append a newline.
                   One intended use case is to insert a call to otf2_config
                   in backticks into the build command. E.g.
                   gcc `otf2_config --libs` mycode.c -o mycode
                   This does not work anymore if you insert newlines. */
                std::cout << OTF2_BACKEND_CC << std::endl;
                break;
        }
    }

    return ret;
}

/** Make string with compiler or linker flags compatible to CUDA
 *  compiler requirements.
 *
 *  @param str              String to be processed.
 *
 *  @return Returns string with compiler or linker flags that can be
 *          passes to CUDA compiler.
 */
std::string
prepare_string( std::string str, bool backend )
{
    std::string pattern1 = " ";
    std::string replace1 = ",";
    std::string pattern2 = ( backend ? OTF2_BACKEND_WL_FLAG : OTF2_FRONTEND_WL_FLAG );
    std::string replace2 = "";

    str = remove_multiple_whitespaces( str );
    /* Replace all white-spaces by comma */
    str = replace_all( pattern1, replace1, str );
    /* Replace flag for passing arguments to linker through compiler
     * (flags not needed beacause we use '-Xlinker' to specify linker
     * flags when using CUDA compiler */
    if ( pattern2.length() != 0 )
    {
        str = replace_all( pattern2, replace2, str );
    }

    return str;
}

/** Trim  and replace multiple white-spaces in @ str by a single one.
 *
 *  @param str              String to be processed.
 *
 *  @return Returns string where all multiple white-spaces are replaced
 *          by a single one.
 */
std::string
remove_multiple_whitespaces( std::string str )
{
    std::string            search = "  "; // this string contains 2 spaces
    std::string::size_type pos;

    /* Trim */
    pos = str.find_last_not_of( ' ' );
    if ( pos != std::string::npos )
    {
        str.erase( pos + 1 );
        pos = str.find_first_not_of( ' ' );
        if ( pos != std::string::npos )
        {
            str.erase( 0, pos );
        }
    }
    else
    {
        str.erase( str.begin(), str.end() );
    }

    /* Remove multiple white-spaces */
    while ( ( pos = str.find( search ) ) != std::string::npos )
    {
        /* remove 1 character from the string at index */
        str.erase( pos, 1 );
    }

    return str;
}

/** Replace all occurrences of @ pattern in string @ original by
 *  @ replacement.
 *
 *  @param pattern          String that should be replaced.
 *  @param replacement      Replacement for @ pattern.
 *  @param original         Input string.
 *
 *  @return Returns a string where all occurrences of @ pattern are
 *          replaced by @ replacement.
 */
std::string
replace_all( const std::string &pattern,
             const std::string &replacement,
             std::string        original )
{
    std::string::size_type pos            = original.find( pattern, 0 );
    int                    pattern_length = pattern.length();

    while ( pos != std::string::npos )
    {
        original.replace( pos, pattern_length, replacement );
        pos = original.find( pattern, 0 );
    }

    return original;
}

/**
   Construct rpath construction flags
 */
void
get_rpath_struct_data( bool backend )
{
    // Replace ${wl} by LIBDIR_FLAG_WL and erase everything from
    // $libdir on in order to create m_rpath_head and
    // m_rpath_delimiter. This will work for most and for the relevant
    // (as we know in 2012-07) values of LIBDIR_FLAG_CC. Possible
    // values are (see also ticket 530,
    // https://silc.zih.tu-dresden.de/trac-silc/ticket/530):
    // '+b $libdir'
    // '-L$libdir'
    // '-R$libdir'
    // '-rpath $libdir'
    // '${wl}-blibpath:$libdir:'"$aix_libpath"
    // '${wl}+b ${wl}$libdir'
    // '${wl}-R,$libdir'
    // '${wl}-R $libdir:/usr/lib:/lib'
    // '${wl}-rpath,$libdir'
    // '${wl}--rpath ${wl}$libdir'
    // '${wl}-rpath ${wl}$libdir'
    // '${wl}-R $wl$libdir'
    std::string rpath_flag = ( backend ? OTF2_BACKEND_RPATH_FLAG_CC : OTF2_FRONTEND_RPATH_FLAG_CC );
    std::string wl         = ( backend ? OTF2_BACKEND_WL_FLAG : OTF2_FRONTEND_WL_FLAG );
    std::string aix_path   = ( backend ? OTF2_BACKEND_AIX_LIBPATH : OTF2_FRONTEND_AIX_LIBPATH );
    size_t      index      = 0;
    while ( true )
    {
        index = rpath_flag.find( "${wl}", index );
        if ( index == std::string::npos )
        {
            break;
        }
        rpath_flag.replace( index, strlen( "${wl}" ), wl );
        ++index;
    }
    index = rpath_flag.find( "$libdir", 0 );
    if ( index != std::string::npos )
    {
        rpath_flag.erase( index );
    }

    bool is_aix = ( !aix_path.empty() );
    if ( is_aix )
    {
        m_rpath_head      = " " + rpath_flag;
        m_rpath_delimiter = ":";
        m_rpath_tail      = ":" + aix_path;
    }
    else
    {
        m_rpath_head      = "";
        m_rpath_delimiter = " " + rpath_flag;
        m_rpath_tail      = "";
    }
}

/**
 * Add content of the environment variable LD_RUN_PATH as -rpath argument
 */
std::string
append_ld_run_path_to_rpath( std::string rpath )
{
    /* Only necessary to decide whether we are on AIX or not */
    std::string aix_path = OTF2_BACKEND_AIX_LIBPATH OTF2_FRONTEND_AIX_LIBPATH;

    /* Get variable values */
    const char* ld_run_path = getenv( "LD_RUN_PATH" );
    if ( ld_run_path == NULL || *ld_run_path == '\0' )
    {
        return rpath;
    }

    /* On AIX ist just a colon separated list, after a head */
    if ( !aix_path.empty() )
    {
        if ( rpath == "" )
        {
            return m_rpath_head + ld_run_path;
        }
        return rpath + m_rpath_delimiter + ld_run_path;
    }

    /* Otherwise replace all colons by the rpath flags */
    rpath += m_rpath_delimiter + replace_all( ":", m_rpath_delimiter, ld_run_path );
    return rpath;
}
