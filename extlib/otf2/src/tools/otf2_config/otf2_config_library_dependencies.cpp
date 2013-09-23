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
 * @file otf2_config_library_dependecies.cpp
 * @status alpha
 * @maintainer Daniel Lorenz <d.lorenz@fz-juelich.de>
 *
 * Implements the representation and analysis of library dependencies
 */

#include <config.h>

#include <iostream>
#include <stdlib.h>
#include <cstring>

#include "otf2_config_library_dependencies.hpp"

using namespace std;

/* **************************************************************************************
                                                                          local functions
****************************************************************************************/

static inline string
strip_leading_whitespace( const string& input )
{
    const char* pos = input.c_str();
    while ( ( *pos != '\0' ) && ( ( *pos == ' ' ) || ( *pos == '\t' ) ) )
    {
        pos++;
    }
    return pos;
}

/**
 * Strips the head and leading delimiter from a input string.
 * Ignores leading whitespaces.
 */
static string
strip_head( const string& input,
            const string& head_orig,
            const string& delimiter_orig )
{
    string output    = strip_leading_whitespace( input );
    string head      = strip_leading_whitespace( head_orig );
    string delimiter = strip_leading_whitespace( delimiter_orig );

    if ( output.compare( 0, head.length(), head ) == 0 )
    {
        output.erase( 0, head.length() );
    }

    output = strip_leading_whitespace( input );
    if ( output.compare( 0, delimiter.length(), delimiter ) == 0 )
    {
        output.erase( 0, delimiter.length() );
    }
    return output;
}

/**
 * Checks whether @a input contains @a item.
 */
static bool
has_item( const deque<string>& input,
          const string&        item )
{
    deque<string>::const_iterator i;
    for ( i = input.begin(); i != input.end(); i++ )
    {
        if ( *i == item )
        {
            return true;
        }
    }
    return false;
}

/**
 * Removes dublicate entries from a deque container of strings. It keeps only the
 * last occurence of each entry. This ensures that the dependencies are maintained.
 */
static deque<string>
remove_double_entries( const deque<string>& input )
{
    deque<string>                         output;
    deque<string>::const_reverse_iterator i;
    for ( i = input.rbegin(); i != input.rend(); i++ )
    {
        if ( !has_item( output, *i ) )
        {
            output.push_front( *i );
        }
    }

    return output;
}

/**
 * Converts deque of strings into a string where all entries are space separated.
 */
static string
deque_to_string( const deque<string>& input,
                 const string&        head,
                 const string&        delimiter,
                 const string&        tail )
{
    string                        output = head;
    deque<string>::const_iterator i;
    for ( i = input.begin(); i != input.end(); i++ )
    {
        output += delimiter + *i;
    }
    output += tail;
    return output;
}

/* **************************************************************************************
                                                                          class la_object
****************************************************************************************/

OTF2_Config_LibraryDependencies::la_object::la_object( void )
{
}

OTF2_Config_LibraryDependencies::la_object::la_object( const la_object &source )
{
    m_lib_name       = source.m_lib_name;
    m_build_dir      = source.m_build_dir;
    m_install_dir    = source.m_install_dir;
    m_libs           = source.m_libs;
    m_ldflags        = source.m_ldflags;
    m_rpath          = source.m_rpath;
    m_dependency_las = source.m_dependency_las;
}


OTF2_Config_LibraryDependencies::la_object::la_object( const string&        lib_name,
                                                       const string&        build_dir,
                                                       const string&        install_dir,
                                                       const deque<string>& libs,
                                                       const deque<string>& ldflags,
                                                       const deque<string>& rpath,
                                                       const deque<string>& dependency_las )
{
    m_lib_name       = lib_name;
    m_build_dir      = build_dir;
    m_install_dir    = install_dir;
    m_libs           = libs;
    m_ldflags        = ldflags;
    m_rpath          = rpath;
    m_dependency_las = dependency_las;
}

OTF2_Config_LibraryDependencies::la_object::~la_object()
{
}

/* **************************************************************************************
                                                  class OTF2_Config_LibraryDependencies
****************************************************************************************/

OTF2_Config_LibraryDependencies::OTF2_Config_LibraryDependencies( void )
{
    deque<string>                      libs;
    deque<string>                      ldflags;
    deque<string>                      rpaths;
    deque<string>                      dependency_las;
    std::map< std::string, la_object>* la_objects = &m_backend_objects;

    /* otf2_config_library_dependencies_backend.cpp is generated by
       vendor/common/build-config/generate-libaray-dependency.sh */
    #include <otf2_config_library_dependencies_backend_inc.hpp>

    la_objects = &m_frontend_objects;

    /* otf2_config_library_dependencies_frontend.cpp is generated by
       vendor/common/build-config/generate-libaray-dependency.sh */
    #include <otf2_config_library_dependencies_frontend_inc.hpp>
}

OTF2_Config_LibraryDependencies::~OTF2_Config_LibraryDependencies()
{
}

string
OTF2_Config_LibraryDependencies::GetLibraries( const deque<string>& input_libs,
                                               bool                 frontend )
{
    /* Traversing backwards will add the -l flags from the otf2_* lib last.
       this makes the system more robust against broken dependencies in installed
       .la files of other libraries, in particular libbfd.
       During configure we tested whether linking works and this is more reliable
       than installed .la files. */
    deque<string>                   deps = get_dependencies( input_libs, frontend );
    deque<string>                   libs;
    deque<string>::reverse_iterator i;

    map< string, la_object>* la_objects;
    la_objects = ( frontend ? &m_frontend_objects : &m_backend_objects );

    for ( i = deps.rbegin(); i != deps.rend(); i++ )
    {
        la_object obj = ( *la_objects )[ *i ];
        libs.push_front( "-l" + obj.m_lib_name.substr( 3 ) );
        libs.insert( libs.end(),
                     obj.m_libs.begin(),
                     obj.m_libs.end() );
    }
    libs = remove_double_entries( libs );

    return deque_to_string( libs, "", " ", "" );
}

string
OTF2_Config_LibraryDependencies::GetLDFlags( const deque<string>& libs,
                                             bool                 install,
                                             bool                 frontend )
{
    deque<string>           deps = get_dependencies( libs, frontend );
    deque<string>           flags;
    deque<string>::iterator i;

    map< string, la_object>* la_objects;
    la_objects = ( frontend ? &m_frontend_objects : &m_backend_objects );

    for ( i = deps.begin(); i != deps.end(); i++ )
    {
        la_object obj = ( *la_objects )[ *i ];
        if ( install )
        {
            flags.push_back( "-L" + obj.m_install_dir );
        }
        else
        {
            flags.push_back( "-L" + obj.m_build_dir + "/.libs" );
        }
        flags.insert( flags.end(),
                      obj.m_ldflags.begin(),
                      obj.m_ldflags.end() );
    }
    flags = remove_double_entries( flags );

    return deque_to_string( flags, "", " ", "" );
}

string
OTF2_Config_LibraryDependencies::GetRpathFlags( const deque<string>& libs,
                                                bool                 install,
                                                bool                 frontend,
                                                const string&        head,
                                                const string&        delimiter,
                                                const string&        tail )
{
    deque<string>           deps = get_dependencies( libs, frontend );
    deque<string>           flags;
    deque<string>::iterator i;
    deque<string>::iterator j;

    map< string, la_object>* la_objects;
    la_objects = ( frontend ? &m_frontend_objects : &m_backend_objects );

    for ( i = deps.begin(); i != deps.end(); i++ )
    {
        la_object obj = ( *la_objects )[ *i ];
        if ( install )
        {
            flags.push_back( obj.m_install_dir );
        }
        else
        {
            flags.push_back( obj.m_build_dir + "/.libs" );
            // to support pre-installed components we need to add m_build_dir too.
            flags.push_back( obj.m_build_dir );
        }
        for ( j = obj.m_rpath.begin(); j != obj.m_rpath.end(); j++ )
        {
            size_t index = j->find( "-R" );
            if ( index == 0 )
            {
                j->replace( index, strlen( "-R" ), "" );
            }
            flags.push_back( strip_head( *j, head, delimiter ) );
        }
    }
    flags = remove_double_entries( flags );

    return deque_to_string( flags, head, delimiter, tail );
}

deque<string>
OTF2_Config_LibraryDependencies::get_dependencies( const deque<string>& libs,
                                                   bool                 frontend )
{
    deque<string>            deps = libs;
    map< string, la_object>* la_objects;
    la_objects = ( frontend ? &m_frontend_objects : &m_backend_objects );

    for ( int i = 0; i < deps.size(); i++ )
    {
        if ( la_objects->find( deps[ i ] ) == la_objects->end() )
        {
            cerr << "ERROR: Can not resolve dependency \"" << deps[ i ] << "\"" << endl;
            exit( EXIT_FAILURE );
        }
        la_object obj = ( *la_objects )[ deps[ i ] ];

        deps.insert( deps.end(),
                     obj.m_dependency_las.begin(),
                     obj.m_dependency_las.end() );
    }
    return remove_double_entries( deps );
}
