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
 *  @file       src/otf2_attribute_list_inc.c
 *  @source     templates/otf2_attribute_list_inc.tmpl.c
 *
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @brief      Convenient helper function to add and get attributes.
 */


OTF2_ErrorCode
OTF2_AttributeList_AddUint8( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attributeID,
                             uint8_t             uint8Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.uint8 = uint8Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_UINT8,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddUint16( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attributeID,
                              uint16_t            uint16Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.uint16 = uint16Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_UINT16,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddUint32( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attributeID,
                              uint32_t            uint32Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.uint32 = uint32Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_UINT32,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddUint64( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attributeID,
                              uint64_t            uint64Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.uint64 = uint64Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_UINT64,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddInt8( OTF2_AttributeList* attributeList,
                            OTF2_AttributeRef   attributeID,
                            int8_t              int8Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.int8 = int8Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_INT8,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddInt16( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attributeID,
                             int16_t             int16Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.int16 = int16Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_INT16,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddInt32( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attributeID,
                             int32_t             int32Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.int32 = int32Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_INT32,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddInt64( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attributeID,
                             int64_t             int64Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.int64 = int64Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_INT64,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddFloat( OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef   attributeID,
                             float               float32Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.float32 = float32Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_FLOAT,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddDouble( OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef   attributeID,
                              double              float64Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.float64 = float64Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_DOUBLE,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddStringRef( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attributeID,
                                 OTF2_StringRef      stringRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.stringRef = stringRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_STRING,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddAttributeRef( OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef   attributeID,
                                    OTF2_AttributeRef   attributeRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.attributeRef = attributeRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_ATTRIBUTE,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddLocationRef( OTF2_AttributeList* attributeList,
                                   OTF2_AttributeRef   attributeID,
                                   OTF2_LocationRef    locationRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.locationRef = locationRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_LOCATION,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddRegionRef( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attributeID,
                                 OTF2_RegionRef      regionRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.regionRef = regionRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_REGION,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddGroupRef( OTF2_AttributeList* attributeList,
                                OTF2_AttributeRef   attributeID,
                                OTF2_GroupRef       groupRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.groupRef = groupRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_GROUP,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddMetricRef( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attributeID,
                                 OTF2_MetricRef      metricRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.metricRef = metricRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_METRIC,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddCommRef( OTF2_AttributeList* attributeList,
                               OTF2_AttributeRef   attributeID,
                               OTF2_CommRef        commRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.commRef = commRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_COMM,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddParameterRef( OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef   attributeID,
                                    OTF2_ParameterRef   parameterRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.parameterRef = parameterRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_PARAMETER,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_AddRmaWinRef( OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef   attributeID,
                                 OTF2_RmaWinRef      rmaWinRef )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.rmaWinRef = rmaWinRef;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            OTF2_TYPE_RMA_WIN,
                                            attribute_value );
}


OTF2_ErrorCode
OTF2_AttributeList_GetUint8( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attributeID,
                             uint8_t*                  uint8Value )
{
    if ( !uint8Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for uint8Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_UINT8 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *uint8Value = attribute_value.uint8;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetUint16( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attributeID,
                              uint16_t*                 uint16Value )
{
    if ( !uint16Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for uint16Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_UINT16 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *uint16Value = attribute_value.uint16;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetUint32( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attributeID,
                              uint32_t*                 uint32Value )
{
    if ( !uint32Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for uint32Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_UINT32 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *uint32Value = attribute_value.uint32;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetUint64( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attributeID,
                              uint64_t*                 uint64Value )
{
    if ( !uint64Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for uint64Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_UINT64 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *uint64Value = attribute_value.uint64;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetInt8( const OTF2_AttributeList* attributeList,
                            OTF2_AttributeRef         attributeID,
                            int8_t*                   int8Value )
{
    if ( !int8Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for int8Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_INT8 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *int8Value = attribute_value.int8;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetInt16( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attributeID,
                             int16_t*                  int16Value )
{
    if ( !int16Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for int16Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_INT16 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *int16Value = attribute_value.int16;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetInt32( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attributeID,
                             int32_t*                  int32Value )
{
    if ( !int32Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for int32Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_INT32 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *int32Value = attribute_value.int32;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetInt64( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attributeID,
                             int64_t*                  int64Value )
{
    if ( !int64Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for int64Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_INT64 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *int64Value = attribute_value.int64;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetFloat( const OTF2_AttributeList* attributeList,
                             OTF2_AttributeRef         attributeID,
                             float*                    float32Value )
{
    if ( !float32Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for float32Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_FLOAT )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *float32Value = attribute_value.float32;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetDouble( const OTF2_AttributeList* attributeList,
                              OTF2_AttributeRef         attributeID,
                              double*                   float64Value )
{
    if ( !float64Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for float64Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_DOUBLE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *float64Value = attribute_value.float64;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetStringRef( const OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef         attributeID,
                                 OTF2_StringRef*           stringRef )
{
    if ( !stringRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for stringRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_STRING )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *stringRef = attribute_value.stringRef;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetAttributeRef( const OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef         attributeID,
                                    OTF2_AttributeRef*        attributeRef )
{
    if ( !attributeRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for attributeRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_ATTRIBUTE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *attributeRef = attribute_value.attributeRef;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetLocationRef( const OTF2_AttributeList* attributeList,
                                   OTF2_AttributeRef         attributeID,
                                   OTF2_LocationRef*         locationRef )
{
    if ( !locationRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for locationRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_LOCATION )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *locationRef = attribute_value.locationRef;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetRegionRef( const OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef         attributeID,
                                 OTF2_RegionRef*           regionRef )
{
    if ( !regionRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for regionRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_REGION )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *regionRef = attribute_value.regionRef;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetGroupRef( const OTF2_AttributeList* attributeList,
                                OTF2_AttributeRef         attributeID,
                                OTF2_GroupRef*            groupRef )
{
    if ( !groupRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for groupRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_GROUP )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *groupRef = attribute_value.groupRef;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetMetricRef( const OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef         attributeID,
                                 OTF2_MetricRef*           metricRef )
{
    if ( !metricRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for metricRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_METRIC )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *metricRef = attribute_value.metricRef;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetCommRef( const OTF2_AttributeList* attributeList,
                               OTF2_AttributeRef         attributeID,
                               OTF2_CommRef*             commRef )
{
    if ( !commRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for commRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_COMM )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *commRef = attribute_value.commRef;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetParameterRef( const OTF2_AttributeList* attributeList,
                                    OTF2_AttributeRef         attributeID,
                                    OTF2_ParameterRef*        parameterRef )
{
    if ( !parameterRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for parameterRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_PARAMETER )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *parameterRef = attribute_value.parameterRef;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_AttributeList_GetRmaWinRef( const OTF2_AttributeList* attributeList,
                                 OTF2_AttributeRef         attributeID,
                                 OTF2_RmaWinRef*           rmaWinRef )
{
    if ( !rmaWinRef )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for rmaWinRef." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != OTF2_TYPE_RMA_WIN )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *rmaWinRef = attribute_value.rmaWinRef;

    return OTF2_SUCCESS;
}
