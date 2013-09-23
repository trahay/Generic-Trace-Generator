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
 *  @file       src/OTF2_IdMap.c
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @status     alpha
 */


#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "otf2_id_map.h"

static bool
scorep_id_map_binary_search( const OTF2_IdMap* instance,
                             uint64_t          localId,
                             uint64_t*         index );


OTF2_IdMap*
OTF2_IdMap_Create( OTF2_IdMapMode mode,
                   uint64_t       capacity )
{
    if ( OTF2_ID_MAP_DENSE != mode && OTF2_ID_MAP_SPARSE != mode )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid mode value." );
        return NULL;
    }
    if ( capacity == 0 )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid capacity value." );
        return NULL;
    }

    /* Double capacity if sparse mapping requested */
    if ( mode == OTF2_ID_MAP_SPARSE )
    {
        capacity *= 2;
    }

    OTF2_IdMap* instance = malloc( sizeof( *instance ) );
    if ( !instance )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                     "Can't allocate id map object." );
        return NULL;
    }

    instance->items = malloc( capacity * sizeof( *instance->items ) );
    if ( !instance->items )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                     "Can't allocate id map object." );
        return NULL;
    }

    instance->mode     = mode;
    instance->capacity = capacity;
    instance->size     = 0;

    return instance;
}


OTF2_IdMap*
OTF2_IdMap_CreateFromUint64Array( uint64_t        length,
                                  const uint64_t* mappings,
                                  bool            optimizeSize )
{
    if ( !mappings )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid mappings argument." );
        return NULL;
    }

    if ( length == 0 )
    {
        /* Its only an error when not optimizing for size
           because length == 0 impies number of dispositions == 0,
           for what we return NULL */
        if ( !optimizeSize )
        {
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "Invalid lendth value." );
        }
        return NULL;
    }

    OTF2_IdMapMode mode     = OTF2_ID_MAP_DENSE;
    uint64_t       capacity = length;
    if ( optimizeSize )
    {
        uint64_t number_of_dispositons = 0;
        for ( uint64_t i = 0; i < length; i++ )
        {
            if ( mappings[ i ] != i )
            {
                number_of_dispositons++;
                if ( number_of_dispositons * 2 >= length )
                {
                    /* We have too many dispositions */
                    break;
                }
            }
        }

        if ( number_of_dispositons * 2 < length )
        {
            mode     = OTF2_ID_MAP_SPARSE;
            capacity = number_of_dispositons;
            /* Return NULL for the identity map */
            if ( number_of_dispositons == 0 )
            {
                return NULL;
            }
        }
    }

    OTF2_IdMap* map = OTF2_IdMap_Create( mode, capacity );
    if ( !map )
    {
        return NULL;
    }

    for ( uint64_t i = 0; i < length; i++ )
    {
        if ( mode == OTF2_ID_MAP_SPARSE )
        {
            /* Only add pair to the mappings if it is a disposition */
            if ( mappings[ i ] != i )
            {
                OTF2_IdMap_AddIdPair( map, i, mappings[ i ] );
            }
        }
        else
        {
            OTF2_IdMap_AddIdPair( map, i, mappings[ i ] );
        }
    }

    return map;
}


OTF2_IdMap*
OTF2_IdMap_CreateFromUint32Array( uint64_t        length,
                                  const uint32_t* mappings,
                                  bool            optimizeSize )
{
    if ( !mappings )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid mappings argument." );
        return NULL;
    }

    if ( length == 0 )
    {
        /* Its only an error when not optimizing for size
           because length == 0 impies number of dispositions == 0,
           for what we return NULL */
        if ( !optimizeSize )
        {
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "Invalid lendth value." );
        }
        return NULL;
    }

    OTF2_IdMapMode mode     = OTF2_ID_MAP_DENSE;
    uint64_t       capacity = length;
    if ( optimizeSize )
    {
        uint64_t number_of_dispositons = 0;
        for ( uint64_t i = 0; i < length; i++ )
        {
            if ( mappings[ i ] != i )
            {
                number_of_dispositons++;
                if ( number_of_dispositons * 2 >= length )
                {
                    /* We have too many dispositions */
                    break;
                }
            }
        }

        if ( number_of_dispositons * 2 < length )
        {
            mode     = OTF2_ID_MAP_SPARSE;
            capacity = number_of_dispositons;
            /* Return NULL for the identity map */
            if ( number_of_dispositons == 0 )
            {
                return NULL;
            }
        }
    }

    OTF2_IdMap* map = OTF2_IdMap_Create( mode, capacity );
    if ( !map )
    {
        return NULL;
    }

    for ( uint64_t i = 0; i < length; i++ )
    {
        if ( mode == OTF2_ID_MAP_SPARSE )
        {
            /* Only add pair to the mappings if it is a disposition */
            if ( mappings[ i ] != i )
            {
                OTF2_IdMap_AddIdPair( map, i, mappings[ i ] );
            }
        }
        else
        {
            OTF2_IdMap_AddIdPair( map, i, mappings[ i ] );
        }
    }

    return map;
}


void
OTF2_IdMap_Free( OTF2_IdMap* instance )
{
    if ( instance )
    {
        free( instance->items );
    }
    free( instance );
}


OTF2_ErrorCode
OTF2_IdMap_GetSize( const OTF2_IdMap* instance,
                    uint64_t*         size )
{
    if ( !instance || !size )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer arguments." );
    }

    *size = instance->size;
    if ( instance->mode == OTF2_ID_MAP_SPARSE )
    {
        *size /= 2;
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_IdMap_GetMode( const OTF2_IdMap* instance,
                    OTF2_IdMapMode*   mode )
{
    if ( !instance || !mode )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer arguments." );
    }

    *mode = instance->mode;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_IdMap_Clear( OTF2_IdMap* instance )
{
    if ( !instance )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for instance argument." );
    }

    /* Forget everything... */
    instance->size = 0;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_IdMap_AddIdPair( OTF2_IdMap* instance,
                      uint64_t    localId,
                      uint64_t    globalId )
{
    if ( !instance )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for instance argument." );
    }
    if ( instance->mode == OTF2_ID_MAP_DENSE &&
         instance->size != localId )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Adding ids out of order in DENSE mode." );
    }

    /* Eventually resize dynamic array */
    if ( instance->size == instance->capacity )
    {
        uint64_t* new_items = realloc( instance->items,
                                       2 * instance->capacity * sizeof( *instance->items ) );
        if ( !new_items )
        {
            // not enough memory
            return UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                                "Can't allocate id map array." );
        }
        instance->items     = new_items;
        instance->capacity *= 2;
    }

    /* Add identifier */
    if ( instance->mode == OTF2_ID_MAP_DENSE )
    {
        instance->items[ instance->size++ ] = globalId;
    }
    else
    {
        uint64_t index;
        if ( scorep_id_map_binary_search( instance, localId, &index ) )
        {
            /* In theory, this should never happen... */
            instance->items[ index + 1 ] = globalId;
        }
        else
        {
            /* Shift entries back, to make room for new entry */
            for ( uint64_t shift_index = instance->size; shift_index-- > index; )
            {
                instance->items[ shift_index + 2 ] = instance->items[ shift_index ];
            }

            /* Insert mapping */
            instance->items[ index ]     = localId;
            instance->items[ index + 1 ] = globalId;
            instance->size              += 2;
        }
    }

    return OTF2_SUCCESS;
}


/*
 * Performs a binary search in @a instance (which must be a sparse mapping
 * table) for the given @a local_id. Returns 1 if the ID could be found and
 * stores the corresponding table index at the memory location pointed to by
 * @a index. Otherwise, it returns 0 and the insertion location is stored at
 * the location pointed to by @a index.
 */
bool
scorep_id_map_binary_search( const OTF2_IdMap* instance,
                             uint64_t          localId,
                             uint64_t*         index )
{
    UTILS_ASSERT( instance && index );

    long left  = 0;
    long right = ( ( long )instance->size / 2 ) - 1;

    while ( left <= right )
    {
        long mid = ( left + right ) & ~1; /* mid = [(left + right) / 2] * 2 */

        if ( instance->items[ mid ] < localId )
        {
            left = ( mid / 2 ) + 1;
        }
        else if ( instance->items[ mid ] > localId )
        {
            right = ( mid / 2 ) - 1;
        }
        else
        {
            *index = ( uint64_t )mid;
            return 1;
        }
    }

    *index = ( uint64_t )( left * 2 );
    return 0;
}


OTF2_ErrorCode
OTF2_IdMap_GetGlobalId( const OTF2_IdMap* instance,
                        uint64_t          localId,
                        uint64_t*         globalId )
{
    if ( !instance || !globalId )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer arguments." );
    }

    /* Use the identity function, if the localId is not in the map. */
    *globalId = localId;
    if ( instance->mode == OTF2_ID_MAP_DENSE )
    {
        if ( localId < instance->size )
        {
            *globalId = instance->items[ localId ];
        }
    }
    else
    {
        uint64_t index;
        if ( scorep_id_map_binary_search( instance, localId, &index ) )
        {
            *globalId = instance->items[ index + 1 ];
        }
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_IdMap_Traverse( const OTF2_IdMap*           instance,
                     OTF2_IdMap_TraverseCallback callback,
                     void*                       userData )
{
    if ( !instance || !callback )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer arguments." );
    }

    if ( instance->mode == OTF2_ID_MAP_DENSE )
    {
        for ( uint64_t local_id = 0; local_id < instance->size; ++local_id )
        {
            callback( local_id,
                      instance->items[ local_id ],
                      userData );
        }
    }
    else
    {
        for ( uint64_t index = 0; index < instance->size; index += 2 )
        {
            callback( instance->items[ index ],
                      instance->items[ index + 1 ],
                      userData );
        }
    }

    return OTF2_SUCCESS;
}
