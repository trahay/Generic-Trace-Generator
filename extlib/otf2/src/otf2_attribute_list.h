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


#ifndef OTF2_INTERNAL_ATTRIBUTE_LIST_H
#define OTF2_INTERNAL_ATTRIBUTE_LIST_H


/**
 *  @file       src/otf2_attribute_list.h
 *
 *  @brief      Internal AttributeList functions.
 */


/** @internal
 *  @brief An attribute list element.
 *  Please see otf2_attribute_struct for a detailed description. */
typedef struct otf2_attribute_struct otf2_attribute;


/** @internal
 *  @brief An attribute list element. */
struct otf2_attribute_struct
{
    /** Type of the the attribute value. */
    OTF2_Type           type_id;
    /** ID of the attribute. */
    OTF2_AttributeRef   attribute_id;
    /** Value of the attribute. */
    OTF2_AttributeValue value;
    /** Pointer to the next attribute in the list. */
    otf2_attribute*     next;
};


/** @internal
 *  @brief Attribute list handle. */
struct OTF2_AttributeList_struct
{
    /** Number of Elements in the attribute list. */
    uint32_t         capacity;
    /** Pointer to first element. */
    otf2_attribute*  head;
    /** Pointer to last element. */
    otf2_attribute** tail;
    /** Pointer to unused entries. */
    otf2_attribute*  free;
};


/** @internal
 *  @brief Initialize an already allocated attribute list.
 */
void
otf2_attribute_list_init( OTF2_AttributeList* attributeList );


/** @internal
 *  @brief De-initializes an already allocated attribute list.
 */
void
otf2_attribute_list_clear( OTF2_AttributeList* attributeList );


/** @internal
 *  @brief Drop all attributes.
 */
static inline OTF2_ErrorCode
otf2_attribute_list_remove_all_attributes( OTF2_AttributeList* attributeList )
{
    /* move the whole list into the free list */
    *attributeList->tail = attributeList->free;
    attributeList->free  = attributeList->head;

    attributeList->capacity = 0;
    attributeList->head     = NULL;
    attributeList->tail     = &attributeList->head;

    return OTF2_SUCCESS;
}


/** @internal
 *  @brief Maximum size of an entry in extended attribute list.
 */
#define OTF2_MAXIMUM_ATTRIBUTE_LIST_ENTRY_SIZE 15


/** @brief Get the pure data size of an attribute list.
 *
 *  @param attributeList    Attribute list handle.
 *
 *  @return                 Returns the pure data size of an attribute list.
 *                          Returns zero if the list does not exist.
 */
static inline uint32_t
otf2_attribute_list_get_data_size( const OTF2_AttributeList* attributeList )
{
    /* If NULL is passed or capacity is zero just return 0.
     * This avoids a check in the event writer. */
    if ( attributeList == NULL
         || attributeList->capacity == 0 )
    {
        return 0;
    }

    /* Maximum data size of an attribute list 5 Bytes for the number of elements
     * + 15 times number * of elements (5 for the attribute id, 1 for the attribute type,
     * 9 for the attribute value). */
    return attributeList->capacity * OTF2_MAXIMUM_ATTRIBUTE_LIST_ENTRY_SIZE + 5;
}




/** @brief Get the size of an attribute list.
 *
 *  @param attributeList    Attribute list handle.
 *
 *  @return                 Returns the size of an attribute list. Returns zero
 *                          if the list does not exist.
 */
static inline uint32_t
otf2_attribute_list_get_size( const OTF2_AttributeList* attributeList )
{
    /* If NULL is passed or capacity is zero just return 0.
     * This avoids a check in the event writer. */
    if ( attributeList == NULL
         || attributeList->capacity == 0 )
    {
        return 0;
    }

    /* Maximum data size of an attribute list 5 Bytes for the number of elements
     * + 15 times number * of elements (5 for the attribute id, 1 for the attribute type,
     * 9 for the attribute value). */
    uint32_t size = otf2_attribute_list_get_data_size( attributeList );

    /* Furthermore we have to store information about the length of this list */
    if ( size < UINT8_MAX )
    {
        /* Data sizes up to 254 bytes can be encoded in 1 byte. */
        size += 1;
    }
    else
    {
        /* For larger data sizes 9 bytes are used to encode record length information. */
        size += 9;
    }

    /* Finally 1 additional byte is used for Record ID*/
    return size + 1;
}


#endif /* OTF2_INTERNAL_ATTRIBUTE_LIST_H */
