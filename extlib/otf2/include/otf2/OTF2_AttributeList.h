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


#ifndef OTF2_ATTRIBUTE_LIST_H
#define OTF2_ATTRIBUTE_LIST_H


/**
 *  @file       include/otf2/OTF2_AttributeList.h
 *  @source     template/OTF2_AttributeList.tmpl.h
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer enables dynamic appending of arbitrary attributes
 *              to any type of event record.
 *
 *  @section attribute_usage_write How to use the attribute list for writing
 *  additional attributes to event records.
 *
 *  First create an attribute list handle.
 *  @code
 *  OTF2_AttributeList attribute_list = OTF2_AttributeList_New();
 *  @endcode
 *
 *  To write your additional attribute to an event record add your attributes to
 *  an empty attribute list right before you call the routine to write the
 *  event.
 *  @code
 *  OTF2_AttributeValue attr_value;
 *  attr_value.uint32 = attribute_value;
 *  OTF2_AttributeList_AddAttribute( attribute_list, attribute_id, OTF2_UINT8, attr_value );
 *  ...
 *  @endcode
 *
 *  Then call the routine to write the event and pass the attribute list. The
 *  additional attributes are added to the event record and will be appended
 *  when reading the event later on.
 *  Please note: All attributes in the list will be added to event record. So
 *  make sure that there are only those attributes in the attribute list that
 *  you actually like to write.
 *  Please note: After writing the event record all attributes are removed from
 *  the attribute list. So the attribute list is empty again. If you want to
 *  write identical attributes to multiple events you have to add them each time
 *  new.
 *  @code
 *  OTF2_EvtWriter_WriteEnter( ..., attribute_list, ... );
 *  @endcode
 */


#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Value container for an attributes.
 *
 * For definition references (@ref OTF2_MappingType) use the same data type as
 * the definition.
 */
typedef union OTF2_AttributeValue_union
{
    /** @brief Arbitrary value of type uint8_t */
    uint8_t  uint8;
    /** @brief Arbitrary value of type uint16_t */
    uint16_t uint16;
    /** @brief Arbitrary value of type uint32_t */
    uint32_t uint32;
    /** @brief Arbitrary value of type uint64_t */
    uint64_t uint64;
    /** @brief Arbitrary value of type int8_t */
    int8_t   int8;
    /** @brief Arbitrary value of type int16_t */
    int16_t  int16;
    /** @brief Arbitrary value of type int32_t */
    int32_t  int32;
    /** @brief Arbitrary value of type int64_t */
    int64_t  int64;
    /** @brief Arbitrary value of type float */
    float    float32;
    /** @brief Arbitrary value of type double */
    double   float64;
    /**
     * @brief References a @ref String definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_STRING is available.
     */
    OTF2_StringRef stringRef;
    /**
     * @brief References a @ref Attribute definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_ATTRIBUTE is available.
     */
    OTF2_AttributeRef attributeRef;
    /**
     * @brief References a @ref Location definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_LOCATION is available.
     */
    OTF2_LocationRef locationRef;
    /**
     * @brief References a @ref Region definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_REGION is available.
     */
    OTF2_RegionRef regionRef;
    /**
     * @brief References a @ref Group definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_GROUP is available.
     */
    OTF2_GroupRef groupRef;
    /**
     * @brief References a @ref MetricClass, or a MetricInstance definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_METRIC is available.
     */
    OTF2_MetricRef metricRef;
    /**
     * @brief References a @ref Comm definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_COMM is available.
     */
    OTF2_CommRef commRef;
    /**
     * @brief References a @ref Parameter definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_PARAMETER is available.
     */
    OTF2_ParameterRef parameterRef;
    /**
     * @brief References a @ref RmaWin definition
     * and will be mapped to the global definition if a mapping table of
     * type @ref OTF2_MAPPING_RMA_WIN is available.
     */
    OTF2_RmaWinRef rmaWinRef;
} OTF2_AttributeValue;


/** @brief Attribute list handle. */
typedef struct OTF2_AttributeList_struct OTF2_AttributeList;


/** @brief Create a new attribute list handle.
 *
 *  @return                 Returns a handle to the attribute list if
 *                          successful, NULL otherwise.
 */
OTF2_AttributeList*
OTF2_AttributeList_New( void );


/** @brief Delete an attribute list handle.
 *
 *  Deletes an attribute list handle and releases all associated resources.
 *
 *  @param attributeList    Attribute list handle.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_Delete( OTF2_AttributeList* attributeList );


/** @brief Add an attribute to an attribute list.
 *
 *  Adds an attribute to an attribute list. If the attribute already exists,
 *  it fails and returns an error.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to attribute definition.
 *  @param type             Type of the attribute.
 *  @param attributeValue   Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddAttribute( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attribute,
                                 OTF2_Type           type,
                                 OTF2_AttributeValue attributeValue );



/** @brief Add an OTF2_TYPE_UINT8 attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param uint8Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddUint8( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attribute,
                             uint8_t             uint8Value );


/** @brief Add an OTF2_TYPE_UINT16 attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param uint16Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddUint16( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attribute,
                              uint16_t            uint16Value );


/** @brief Add an OTF2_TYPE_UINT32 attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param uint32Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddUint32( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attribute,
                              uint32_t            uint32Value );


/** @brief Add an OTF2_TYPE_UINT64 attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param uint64Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddUint64( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attribute,
                              uint64_t            uint64Value );


/** @brief Add an OTF2_TYPE_INT8 attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param int8Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddInt8( OTF2_AttributeList* attributeList,
                            OTF2_AttributeRef   attribute,
                            int8_t              int8Value );


/** @brief Add an OTF2_TYPE_INT16 attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param int16Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddInt16( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attribute,
                             int16_t             int16Value );


/** @brief Add an OTF2_TYPE_INT32 attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param int32Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddInt32( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attribute,
                             int32_t             int32Value );


/** @brief Add an OTF2_TYPE_INT64 attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param int64Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddInt64( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attribute,
                             int64_t             int64Value );


/** @brief Add an OTF2_TYPE_FLOAT attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param float32Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddFloat( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attribute,
                             float               float32Value );


/** @brief Add an OTF2_TYPE_DOUBLE attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param float64Value  Value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddDouble( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attribute,
                              double              float64Value );


/** @brief Add an OTF2_STRING attribute to an attribute list.
 *
 *  @deprecated Use @a OTF2_AttributeList_AddStringRef() instead.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param stringRef        Reference to String definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddString( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attribute,
                              OTF2_StringRef      stringRef );


/** @brief Add an OTF2_TYPE_STRING attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param stringRef Reference to String definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddStringRef( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attribute,
                                 OTF2_StringRef      stringRef );


/** @brief Add an OTF2_TYPE_ATTRIBUTE attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param attributeRef Reference to Attribute definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddAttributeRef( OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef   attribute,
                                    OTF2_AttributeRef   attributeRef );


/** @brief Add an OTF2_TYPE_LOCATION attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param locationRef Reference to Location definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddLocationRef( OTF2_AttributeList* attributeList,
                                   OTF2_AttributeRef   attribute,
                                   OTF2_LocationRef    locationRef );


/** @brief Add an OTF2_TYPE_REGION attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param regionRef Reference to Region definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddRegionRef( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attribute,
                                 OTF2_RegionRef      regionRef );


/** @brief Add an OTF2_TYPE_GROUP attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param groupRef Reference to Group definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddGroupRef( OTF2_AttributeList* attributeList,
                                OTF2_AttributeRef   attribute,
                                OTF2_GroupRef       groupRef );


/** @brief Add an OTF2_TYPE_METRIC attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param metricRef Reference to Metric definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddMetricRef( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attribute,
                                 OTF2_MetricRef      metricRef );


/** @brief Add an OTF2_TYPE_COMM attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param commRef Reference to Comm definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddCommRef( OTF2_AttributeList* attributeList,
                               OTF2_AttributeRef   attribute,
                               OTF2_CommRef        commRef );


/** @brief Add an OTF2_TYPE_PARAMETER attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param parameterRef Reference to Parameter definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddParameterRef( OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef   attribute,
                                    OTF2_ParameterRef   parameterRef );


/** @brief Add an OTF2_TYPE_RMA_WIN attribute to an attribute list.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @param rmaWinRef Reference to RmaWin definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_AddRmaWinRef( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attribute,
                                 OTF2_RmaWinRef      rmaWinRef );


/** @brief Remove an attribute from an attribute list.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_RemoveAttribute( OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef   attribute );


/** @brief Remove all attributes from an attribute list.
 *
 *  @param attributeList    Attribute list handle.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_RemoveAllAttributes( OTF2_AttributeList* attributeList );


/** @brief Test if an attribute is in the attribute list.
 *
 *  @param attributeList    Attribute list handle.
 *  @param attribute        Reference to Attribute definition.
 *
 *  @return                 True if the id is in the list, else false.
 */
bool
OTF2_AttributeList_TestAttributeByID( const OTF2_AttributeList* attributeList,
                                      OTF2_AttributeRef         attribute );


/** @brief Get an attribute from an attribute list by attribute ID.
 *
 *  @param attributeList       Attribute list handle.
 *  @param attribute           Reference to Attribute definition.
 *  @param[out] type           Returned type of the attribute.
 *  @param[out] attributeValue Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetAttributeByID( const OTF2_AttributeList* attributeList,
                                     OTF2_AttributeRef         attribute,
                                     OTF2_Type*                type,
                                     OTF2_AttributeValue*      attributeValue );


/** @brief Get an OTF2_TYPE_UINT8 attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] uint8Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetUint8( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attribute,
                             uint8_t*                  uint8Value );


/** @brief Get an OTF2_TYPE_UINT16 attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] uint16Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetUint16( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attribute,
                              uint16_t*                 uint16Value );


/** @brief Get an OTF2_TYPE_UINT32 attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] uint32Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetUint32( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attribute,
                              uint32_t*                 uint32Value );


/** @brief Get an OTF2_TYPE_UINT64 attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] uint64Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetUint64( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attribute,
                              uint64_t*                 uint64Value );


/** @brief Get an OTF2_TYPE_INT8 attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] int8Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetInt8( const OTF2_AttributeList* attributeList,
                            OTF2_AttributeRef         attribute,
                            int8_t*                   int8Value );


/** @brief Get an OTF2_TYPE_INT16 attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] int16Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetInt16( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attribute,
                             int16_t*                  int16Value );


/** @brief Get an OTF2_TYPE_INT32 attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] int32Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetInt32( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attribute,
                             int32_t*                  int32Value );


/** @brief Get an OTF2_TYPE_INT64 attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] int64Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetInt64( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attribute,
                             int64_t*                  int64Value );


/** @brief Get an OTF2_TYPE_FLOAT attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] float32Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetFloat( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attribute,
                             float*                    float32Value );


/** @brief Get an OTF2_TYPE_DOUBLE attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to Attribute definition.
 *
 *  @param[out] float64Value  Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetDouble( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attribute,
                              double*                   float64Value );


/** @brief Add an OTF2_STRING attribute to an attribute list.
 *
 *  @deprecated Use @a OTF2_AttributeList_GetStringRef() instead.
 *
 *  Convenient function around @a OTF2_AttributeList_AddAttribute.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] stringRef Returned string value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetString( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attribute,
                              OTF2_StringRef*           stringRef );


/** @brief Get an OTF2_TYPE_STRING attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] stringRef Returned string value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetStringRef( const OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef         attribute,
                                 OTF2_StringRef*           stringRef );


/** @brief Get an OTF2_TYPE_ATTRIBUTE attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] attributeRef Returned attribute value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetAttributeRef( const OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef         attribute,
                                    OTF2_AttributeRef*        attributeRef );


/** @brief Get an OTF2_TYPE_LOCATION attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] locationRef Returned location value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetLocationRef( const OTF2_AttributeList* attributeList,
                                   OTF2_AttributeRef         attribute,
                                   OTF2_LocationRef*         locationRef );


/** @brief Get an OTF2_TYPE_REGION attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] regionRef Returned region value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetRegionRef( const OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef         attribute,
                                 OTF2_RegionRef*           regionRef );


/** @brief Get an OTF2_TYPE_GROUP attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] groupRef Returned group value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetGroupRef( const OTF2_AttributeList* attributeList,
                                OTF2_AttributeRef         attribute,
                                OTF2_GroupRef*            groupRef );


/** @brief Get an OTF2_TYPE_METRIC attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] metricRef Returned metric value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetMetricRef( const OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef         attribute,
                                 OTF2_MetricRef*           metricRef );


/** @brief Get an OTF2_TYPE_COMM attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] commRef Returned comm value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetCommRef( const OTF2_AttributeList* attributeList,
                               OTF2_AttributeRef         attribute,
                               OTF2_CommRef*             commRef );


/** @brief Get an OTF2_TYPE_PARAMETER attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] parameterRef Returned parameter value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetParameterRef( const OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef         attribute,
                                    OTF2_ParameterRef*        parameterRef );


/** @brief Get an OTF2_TYPE_RMA_WIN attribute from an attribute list by attribute ID.
 *
 *  Convenient function around @a OTF2_AttributeList_GetAttributeByID.
 *
 *  @param attributeList  Attribute list handle.
 *  @param attribute      Reference to attribute definition.
 *
 *  @param[out] rmaWinRef Returned rmaWin value.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetRmaWinRef( const OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef         attribute,
                                 OTF2_RmaWinRef*           rmaWinRef );


/** @brief Get an attribute from an attribute list by attribute index.
 *
 *  @param attributeList       Attribute list handle.
 *  @param index               Position of the attribute in the attribute list.
 *  @param[out] attribute      Returned attribute reference.
 *  @param[out] type           Returned type of the attribute.
 *  @param[out] attributeValue Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_GetAttributeByIndex( const OTF2_AttributeList* attributeList,
                                        uint32_t                  index,
                                        OTF2_AttributeRef*        attribute,
                                        OTF2_Type*                type,
                                        OTF2_AttributeValue*      attributeValue );


/** @brief Get first attribute from an attribute list and remove it.
 *
 *  Returns the first entry in the attribute list and removes it from the list.
 *
 *  @param attributeList       Attribute list handle.
 *  @param[out] attribute      Returned attribute reference.
 *  @param[out] type           Returned type of the attribute.
 *  @param[out] attributeValue Returned value of the attribute.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_AttributeList_PopAttribute( OTF2_AttributeList*  attributeList,
                                 OTF2_AttributeRef*   attribute,
                                 OTF2_Type*           type,
                                 OTF2_AttributeValue* attributeValue );


/** @brief Get the number of entries in an attribute list.
 *
 *  @param attributeList    Attribute list handle.
 *
 *  @return                 Returns the number of elements in the list. Returns
 *                          zero if the list does not exist.
 */
uint32_t
OTF2_AttributeList_GetNumberOfElements( const OTF2_AttributeList* attributeList );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_ATTRIBUTE_LIST_H */
