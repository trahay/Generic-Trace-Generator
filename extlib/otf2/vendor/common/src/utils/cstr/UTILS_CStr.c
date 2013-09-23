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
 * @file            UTILS_CStr.c
 * @maintainer      Daniel Lorenz <d.lorenz@fz-juelich.de>
 * @ingroup         UTILS_CStr_module
 *
 * @brief           Helper functions for string handling
 */

#include <config.h>
#include <UTILS_CStr.h>

#include <string.h>
#include <stdlib.h>

#include <UTILS_Error.h>

const size_t UTILS_CStr_npos = ( size_t )-1;


char*
UTILS_CStr_dup( const char* source )
{
    if ( source == NULL )
    {
        return NULL;
    }
    char* dup = ( char* )malloc( strlen( source ) + 1 );
    if ( dup == NULL )
    {
        UTILS_ERROR_POSIX();
        return NULL;
    }
    strcpy( dup, source );
    return dup;
}

size_t
UTILS_CStr_find( const char* str,
                 const char* pattern,
                 size_t      pos )
{
    const size_t length = strlen( pattern );

    for (; str[ pos ] != '\0'; pos++ )
    {
        if ( strncmp( &str[ pos ], pattern, length ) == 0 )
        {
            return pos;
        }
    }
    return UTILS_CStr_npos;
}
