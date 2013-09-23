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


/**
 * @status          alpha
 * @file            UTILS_Debug.c
 * @maintainer      Daniel Lorenz <d.lorenz@fz-juelich.de>
 * @ingroup         UTILS_Exception_module
 *
 * @brief           Implementation of debug output handling in SCOREP.
 *
 * @author          Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 * @created         2009-05-08
 */


#include <config.h>
#include <UTILS_Debug.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>


#include <UTILS_Error.h>
#include <UTILS_CStr.h>
#include <UTILS_IO.h>


#include "normalize_file.h"


static uint64_t debug_level;

static bool
is_base_digit( int        c,
               int        base,
               int* const out )
{
    switch ( c )
    {
        case 'f':
        case 'e':
        case 'd':
        case 'c':
        case 'b':
        case 'a':
            if ( base < 16 )
            {
                return false;
            }
            c = c - 'a' + '9' + 1;
        case '9':
        case '8':
            if ( base <= 8 )
            {
                return false;
            }
        case '7':
        case '6':
        case '5':
        case '4':
        case '3':
        case '2':
            if ( base <= 2 )
            {
                return false;
            }
        case '1':
        case '0':
            *out = c - '0';
            return true;
    }

    return false;
}

static int
parse_number( const char*     numberString,
              uint64_t* const numberReference )
{
    uint64_t number = 0;

    assert( numberString );
    assert( numberReference );

    /* determine base of number */
    int base = 10;
    if ( numberString[ 0 ] == '0' && tolower( numberString[ 1 ] ) == 'x' )
    {
        base          = 16;
        numberString += 2;
    }
    else if ( numberString[ 0 ] == '0' && tolower( numberString[ 1 ] ) == 'b' )
    {
        base          = 2;
        numberString += 2;
    }
    else if ( numberString[ 0 ] == '0' )
    {
        base = 8;
        /* keep the 0, "0" is also valid */
    }
    const char* value_iterator = numberString;
    int         digit;
    while ( is_base_digit( tolower( *value_iterator ), base, &digit ) )
    {
        uint64_t new_number = base * number + digit;

        /* Check for overflow */
        if ( new_number < number )
        {
            return ERANGE;
        }

        number = new_number;
        value_iterator++;
    }

    /* Have we consumed at least one digit? */
    if ( value_iterator == numberString )
    {
        return EINVAL;
    }
    numberString = value_iterator;

    /* Have we consumed the complete string */
    if ( *numberString != '\0' )
    {
        return EINVAL;
    }

    *numberReference = number;

    return 0;
}

static const char* debug_module_names[] =
{
    #define UTILS_DEFINE_DEBUG_MODULE( name, bit ) \
    UTILS_STRINGIFY( name )
    UTILS_DEBUG_MODULES,
    #undef UTILS_DEFINE_DEBUG_MODULE
    NULL
};

static bool
string_equal_icase( const char* s1,
                    const char* s2 )
{
    while ( *s1 && *s2 )
    {
        if ( toupper( *s1 ) != toupper( *s2 ) )
        {
            return false;
        }
        s1++;
        s2++;
    }
    if ( *s1 || *s2 )
    {
        return false;
    }
    return true;
}

static int
parse_debug_level( const char*     levelString,
                   uint64_t* const levelReference )
{
    char*    levelStringCopy;
    uint64_t level = 0;

    assert( levelString );
    assert( levelReference );

    levelStringCopy = UTILS_CStr_dup( levelString );
    if ( !levelStringCopy )
    {
        return ENOMEM;
    }

    char* token;
    for ( token = strtok( levelStringCopy, " ,;" );
          token; token = strtok( NULL, " ,;" ) )
    {
        /* some common constants */
        if ( string_equal_icase( token, "all" )
             || strcmp( token, "~0" ) == 0
             || strcmp( token, "-1" ) == 0 )
        {
            level |= UINT64_MAX;
            continue;
        }

        bool     invert    = false;
        uint64_t level_mod = 0;
        if ( *token == '~' )
        {
            invert = true;
            token++;
        }

        /* try module names */
        level_mod = 1;
        const char** module_name = debug_module_names;
        while ( *module_name )
        {
            if ( string_equal_icase( *module_name, token ) )
            {
                break;
            }
            module_name++;
            level_mod <<= 1;
        }
        if ( *module_name )
        {
            goto modify_level;
        }

        /* parse it as a number */
        level_mod = 0;
        if ( 0 == parse_number( token, &level_mod ) )
        {
            goto modify_level;
        }

        free( levelStringCopy );
        return 1;

modify_level:
        if ( invert )
        {
            level &= ~level_mod;
        }
        else
        {
            level |= level_mod;
        }
    }
    free( levelStringCopy );

    *levelReference = level;

    return 0;
}

/**
 * This is the init function for the debug system, which is mainly used to
 * collect all needed information from the system and warn that the debug mode
 * was set on compile time.
 */
static void
debug_init( void )
{
    static uint32_t init_flag;

    if ( init_flag == 0 )
    {
        init_flag = 1;

        const char* env_name           = UTILS_STRINGIFY( PACKAGE_MANGLE_NAME_CAPS( DEBUG ) );
        const char* debug_level_string = getenv( env_name );

        debug_level = 0;
        if ( debug_level_string )
        {
            int ret = parse_debug_level( debug_level_string, &debug_level );
            if ( ret )
            {
                fprintf( stderr, "[%s] Invalid value for %s: %s\n",
                         PACKAGE_NAME, env_name, debug_level_string );
            }
        }

        debug_level &= ~( UTILS_DEBUG_FUNCTION_ENTRY | UTILS_DEBUG_FUNCTION_EXIT );
        if ( debug_level )
        {
            fprintf( stderr, "[%s] Active debug module(s):", PACKAGE_NAME );
            uint64_t     level_mod   = 1;
            const char** module_name = debug_module_names;
            while ( *module_name )
            {
                if ( debug_level & level_mod )
                {
                    fprintf( stderr, " %s", *module_name );
                }
                level_mod <<= 1;
                module_name++;
            }
            fprintf( stderr, "\n" );
        }
    }
}


void
UTILS_Debug_Printf( uint64_t    bitMask,
                    const char* file,
                    uint64_t    line,
                    const char* function,
                    const char* msgFormatString,
                    ... )
{
    debug_init();

    uint64_t kind = bitMask & ( UTILS_DEBUG_FUNCTION_ENTRY | UTILS_DEBUG_FUNCTION_EXIT );
    bitMask &= ~( UTILS_DEBUG_FUNCTION_ENTRY | UTILS_DEBUG_FUNCTION_EXIT );

    if ( debug_level == 0 || ( debug_level & bitMask ) != bitMask )
    {
        return;
    }

    assert( kind != ( UTILS_DEBUG_FUNCTION_ENTRY | UTILS_DEBUG_FUNCTION_EXIT ) );

    size_t msg_format_string_length = msgFormatString ?
                                      strlen( msgFormatString ) : 0;

    char* normalized_file = normalize_file( file );

    if ( kind )
    {
        const char* kind_str = "Entering";
        if ( bitMask & UTILS_DEBUG_FUNCTION_EXIT )
        {
            kind_str = "Leaving";
        }

        fprintf( stdout,
                 "[%s] %s:%" PRIu64 ": %s function '%s'%s",
                 PACKAGE_NAME,
                 normalized_file,
                 line,
                 kind_str,
                 function,
                 msg_format_string_length ? ": " : "\n" );
    }
    else
    {
        fprintf( stdout,
                 "[%s] %s:%" PRIu64 "%s",
                 PACKAGE_NAME,
                 normalized_file,
                 line,
                 msg_format_string_length ? ": " : "\n" );
    }

    free( normalized_file );

    if ( msg_format_string_length )
    {
        va_list va;
        va_start( va, msgFormatString );
        vfprintf( stdout, msgFormatString, va );
        fprintf( stdout, "\n" );
        va_end( va );
    }
}


void
UTILS_Debug_RawPrintf( uint64_t    bitMask,
                       const char* msgFormatString,
                       ... )
{
    debug_init();

    /* enter/exit does not make sense for the raw one */
    uint64_t kind = bitMask & ( UTILS_DEBUG_FUNCTION_ENTRY | UTILS_DEBUG_FUNCTION_EXIT );
    assert( kind == 0 );

    if ( debug_level == 0 || ( debug_level & bitMask ) != bitMask )
    {
        return;
    }

    va_list va;
    va_start( va, msgFormatString );
    vfprintf( stdout, msgFormatString, va );
    va_end( va );
}


void
UTILS_Debug_Prefix( uint64_t    bitMask,
                    const char* file,
                    uint64_t    line,
                    const char* function )
{
    debug_init();

    uint64_t kind = bitMask & ( UTILS_DEBUG_FUNCTION_ENTRY | UTILS_DEBUG_FUNCTION_EXIT );
    bitMask &= ~( UTILS_DEBUG_FUNCTION_ENTRY | UTILS_DEBUG_FUNCTION_EXIT );

    if ( debug_level == 0 || ( debug_level & bitMask ) != bitMask )
    {
        return;
    }

    assert( kind != ( UTILS_DEBUG_FUNCTION_ENTRY | UTILS_DEBUG_FUNCTION_EXIT ) );

    char* normalized_file = normalize_file( file );

    if ( kind )
    {
        const char* kind_str = "Entering";
        if ( bitMask & UTILS_DEBUG_FUNCTION_EXIT )
        {
            kind_str = "Leaving";
        }

        fprintf( stdout,
                 "[%s] %s:%" PRIu64 ": %s function '%s': ",
                 PACKAGE_NAME,
                 normalized_file,
                 line,
                 kind_str,
                 function );
    }
    else
    {
        fprintf( stdout,
                 "[%s] %s:%" PRIu64 ": ",
                 PACKAGE_NAME,
                 normalized_file,
                 line );
    }

    free( normalized_file );
}
