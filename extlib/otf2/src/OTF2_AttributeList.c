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
 *  @file       src/OTF2_AttributeList.c
 *
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
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

#include "otf2_attribute_list.h"

/* ___ Macros _______________________________________________________________ */



/** @internal
 *  @brief Defines maximum size of an attribute list.
 *  In general an attribute list can be of arbitrary size. But, since an
 *  attribute list has to fit into a chunk, there has to be a limit. This limit
 *  can be up to one-seventeenth of the chunk size. If necessary this limit can
 *  be adapted later on. */
#define OTF2_ATTRIBUTE_LIST_MAX ( uint32_t )1024


static inline otf2_attribute**
otf2_attribute_list_find_id( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attribute );

void
otf2_attribute_list_init( OTF2_AttributeList* attributeList )
{
    if ( !attributeList )
    {
        return;
    }

    attributeList->capacity = 0;
    attributeList->head     = NULL;
    attributeList->tail     = &attributeList->head;
    attributeList->free     = NULL;
}


OTF2_AttributeList*
OTF2_AttributeList_New( void )
{
    OTF2_AttributeList* new_list = calloc( 1, sizeof( *new_list ) );
    if ( NULL == new_list )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for attribute list handle!" );
        return NULL;
    }

    otf2_attribute_list_init( new_list );

    return new_list;
}


void
otf2_attribute_list_clear( OTF2_AttributeList* attributeList )
{
    if ( !attributeList )
    {
        return;
    }

    otf2_attribute* entry = attributeList->head;
    while ( entry )
    {
        otf2_attribute* next = entry->next;
        free( entry );
        entry = next;
    }

    entry = attributeList->free;
    while ( entry )
    {
        otf2_attribute* next = entry->next;
        free( entry );
        entry = next;
    }
}


OTF2_ErrorCode
OTF2_AttributeList_Delete( OTF2_AttributeList* attributeList )
{
    if ( attributeList == NULL )
    {
        return OTF2_SUCCESS;
    }

    otf2_attribute_list_clear( attributeList );

    free( attributeList );

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_AddAttribute( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attribute,
                                 OTF2_Type           type,
                                 OTF2_AttributeValue attributeValue )
{
    /* Validate arguments. */
    if ( attributeList == NULL )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid attribute list!" );
    }

    if ( attributeList->capacity >= OTF2_ATTRIBUTE_LIST_MAX )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "There is no space left in the attribute list!" );
    }

    if ( otf2_attribute_list_find_id( attributeList, attribute ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "The passed attribute ID already exists!" );
    }

    otf2_attribute* new_entry;
    if ( attributeList->free )
    {
        new_entry           = attributeList->free;
        attributeList->free = new_entry->next;
    }
    else
    {
        new_entry = calloc( 1, sizeof( *new_entry ) );
        if ( new_entry == NULL )
        {
            return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                                "Could not allocate memory for new attribute!" );
        }
    }

    new_entry->type_id      = type;
    new_entry->attribute_id = attribute;
    new_entry->value        = attributeValue;
    new_entry->next         = NULL;

    attributeList->capacity++;
    *attributeList->tail = new_entry;
    attributeList->tail  = &new_entry->next;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_RemoveAttribute( OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef   attribute )
{
    /* Validate arguments. */
    if ( attributeList == NULL )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid attribute list!" );
    }

    /* Check if requested attribute is in the list. */
    otf2_attribute** entry
        = otf2_attribute_list_find_id( attributeList, attribute );
    if ( !entry )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "The passed attribute ID does not exists!" );
    }

    otf2_attribute* remove_entry = *entry;
    *entry = ( *entry )->next;
    if ( remove_entry->next == NULL )
    {
        /* adjust tail, if the removed one was the last */
        attributeList->tail = entry;
    }

    /* keep the removed one in the free list */
    remove_entry->next  = attributeList->free;
    attributeList->free = remove_entry;
    attributeList->capacity--;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_RemoveAllAttributes( OTF2_AttributeList* attributeList )
{
    /* Validate arguments. */
    if ( attributeList == NULL )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid attribute list!" );
    }

    return otf2_attribute_list_remove_all_attributes( attributeList );
}


bool
OTF2_AttributeList_TestAttributeByID( const OTF2_AttributeList* attributeList,
                                      OTF2_AttributeRef         attribute )
{
    if ( attributeList == NULL )
    {
        return false;
    }

    return NULL != otf2_attribute_list_find_id( attributeList, attribute );
}


OTF2_ErrorCode
OTF2_AttributeList_GetAttributeByID( const OTF2_AttributeList* attributeList,
                                     OTF2_AttributeRef         attribute,
                                     OTF2_Type*                type,
                                     OTF2_AttributeValue*      attributeValue )
{
    /* Validate arguments. */
    if ( attributeList == NULL || !type || !attributeValue )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid attribute list!" );
    }

    otf2_attribute** entry = otf2_attribute_list_find_id( attributeList,
                                                          attribute );
    if ( !entry )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "The passed attribute ID does not exist!" );
    }

    *type           = ( *entry )->type_id;
    *attributeValue = ( *entry )->value;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetAttributeByIndex( const OTF2_AttributeList* attributeList,
                                        uint32_t                  index,
                                        OTF2_AttributeRef*        attribute,
                                        OTF2_Type*                type,
                                        OTF2_AttributeValue*      attributeValue )
{
    /* Validate arguments. */
    if ( attributeList == NULL || !attribute || !type || !attributeValue )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid attribute list!" );
    }

    if ( index >= attributeList->capacity )
    {
        return UTILS_ERROR( OTF2_ERROR_INDEX_OUT_OF_BOUNDS,
                            "The passed index is out of range!" );
    }

    otf2_attribute* entry = attributeList->head;

    for ( uint32_t i = 0; i < index; i++ )
    {
        entry = entry->next;
    }

    *attribute      = entry->attribute_id;
    *type           = entry->type_id;
    *attributeValue = entry->value;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_PopAttribute( OTF2_AttributeList*  attributeList,
                                 OTF2_AttributeRef*   attribute,
                                 OTF2_Type*           type,
                                 OTF2_AttributeValue* attributeValue )
{
    /* Validate arguments. */
    if ( !attributeList || !attribute || !type || !attributeValue )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid attribute list!" );
    }

    if ( attributeList->capacity == 0 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Attribute list is empty!" );
    }

    otf2_attribute* entry = attributeList->head;

    *attribute      = entry->attribute_id;
    *type           = entry->type_id;
    *attributeValue = entry->value;

    attributeList->head = entry->next;
    if ( attributeList->head == NULL )
    {
        attributeList->tail = &attributeList->head;
    }
    entry->next         = attributeList->free;
    attributeList->free = entry;
    attributeList->capacity--;

    return OTF2_SUCCESS;
}


uint32_t
OTF2_AttributeList_GetNumberOfElements( const OTF2_AttributeList* attributeList )
{
    /* Validate arguments. */
    if ( attributeList == NULL )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid attribute list!" );
        return 0;
    }

    return attributeList->capacity;
}


/** @brief Test the existence of an attribute ID in the list.
 *
 *  Returns the first entry in the attribute list and removes it from the list.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Pointer to the returned ID of the attribute.
 *
 *  @return                 Returns a non-zero value if the ID exists in the
 *                          list, zero otherwise.
 */
otf2_attribute**
otf2_attribute_list_find_id( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attribute )
{
    UTILS_ASSERT( attributeList );

    otf2_attribute** entry = ( otf2_attribute** )&attributeList->head;

    while ( *entry )
    {
        if ( ( *entry )->attribute_id == attribute )
        {
            return entry;
        }
        entry = &( *entry )->next;
    }

    return NULL;
}

/* Include generated convenient helper functions */
#include "otf2_attribute_list_inc.c"

/* Deprecated functions. */

OTF2_ErrorCode
OTF2_AttributeList_AddString( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attribute,
                              OTF2_StringRef      stringRef )
{
    UTILS_DEPRECATED( "This function is deprecated in favor of OTF2_AttributeList_AddStringRef" );

    return OTF2_AttributeList_AddStringRef( attributeList,
                                            attribute,
                                            stringRef );
}


OTF2_ErrorCode
OTF2_AttributeList_GetString( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attribute,
                              OTF2_StringRef*           stringRef )
{
    UTILS_DEPRECATED( "This function is deprecated in favor of OTF2_AttributeList_GetStringRef" );

    return OTF2_AttributeList_GetStringRef( attributeList,
                                            attribute,
                                            stringRef );
}
