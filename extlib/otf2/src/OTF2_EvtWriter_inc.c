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
 *  @file       src/OTF2_EvtWriter_inc.c
 *  @source     templates/OTF2_EvtWriter_inc.tmpl.c
 *
 *  @status     alpha
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This lowest user-visible layer provides write routines to write
 *              event data of a single location. The inc file holds the
 *              generated part of event writer module.
 */


static inline OTF2_ErrorCode
otf2_evt_writer_write_attribute_list( const OTF2_EvtWriter* writerHandle,
                                      OTF2_AttributeList*   attributeList )
{
    /* Write no attribute list to the buffer, if there is no one. */
    if ( attributeList == NULL || attributeList->capacity == 0 )
    {
        return OTF2_SUCCESS;
    }

    /* Write the attribute lists record type id to the trace */
    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_ATTRIBUTE_LIST );

    /* Write initial record length */
    uint32_t data_size_of_attr_list = otf2_attribute_list_get_data_size( attributeList );
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, data_size_of_attr_list );

    /* Store how many items are in the list to the trace */
    OTF2_Buffer_WriteUint32( writerHandle->buffer, attributeList->capacity );

    for ( otf2_attribute* attr = attributeList->head; attr; attr = attr->next )
    {
        /* Write attribute id to the trace */
        OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->attribute_id );

        /* Write the type to the trace */
        OTF2_Buffer_WriteUint8( writerHandle->buffer, attr->type_id );

        switch ( attr->type_id )
        {
            case OTF2_TYPE_UINT8:
                OTF2_Buffer_WriteUint8( writerHandle->buffer,
                                        attr->value.uint8 );
                break;

            case OTF2_TYPE_UINT16:
                OTF2_Buffer_WriteUint16( writerHandle->buffer,
                                         attr->value.uint16 );
                break;

            case OTF2_TYPE_UINT32:
                OTF2_Buffer_WriteUint32( writerHandle->buffer,
                                         attr->value.uint32 );
                break;

            case OTF2_TYPE_UINT64:
                OTF2_Buffer_WriteUint64( writerHandle->buffer,
                                         attr->value.uint64 );
                break;

            case OTF2_TYPE_INT8:
                OTF2_Buffer_WriteInt8( writerHandle->buffer,
                                       attr->value.int8 );
                break;

            case OTF2_TYPE_INT16:
                OTF2_Buffer_WriteInt16( writerHandle->buffer,
                                        attr->value.int16 );
                break;

            case OTF2_TYPE_INT32:
                OTF2_Buffer_WriteInt32( writerHandle->buffer,
                                        attr->value.int32 );
                break;

            case OTF2_TYPE_INT64:
                OTF2_Buffer_WriteInt64( writerHandle->buffer,
                                        attr->value.int64 );
                break;

            case OTF2_TYPE_FLOAT:
                OTF2_Buffer_WriteFloat( writerHandle->buffer,
                                        attr->value.float32 );
                break;

            case OTF2_TYPE_DOUBLE:
                OTF2_Buffer_WriteDouble( writerHandle->buffer,
                                         attr->value.float64 );
                break;

            case OTF2_TYPE_STRING:
                /* OTF2_TYPE_STRING attributes reference a String definition */
                OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->value.stringRef );
                break;

            case OTF2_TYPE_ATTRIBUTE:
                /* OTF2_TYPE_ATTRIBUTE attributes reference a Attribute definition */
                OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->value.attributeRef );
                break;

            case OTF2_TYPE_LOCATION:
                /* OTF2_TYPE_LOCATION attributes reference a Location definition */
                OTF2_Buffer_WriteUint64( writerHandle->buffer, attr->value.locationRef );
                break;

            case OTF2_TYPE_REGION:
                /* OTF2_TYPE_REGION attributes reference a Region definition */
                OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->value.regionRef );
                break;

            case OTF2_TYPE_GROUP:
                /* OTF2_TYPE_GROUP attributes reference a Group definition */
                OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->value.groupRef );
                break;

            case OTF2_TYPE_METRIC:
                /* OTF2_TYPE_METRIC attributes reference a MetricClass, or a MetricInstance definition */
                OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->value.metricRef );
                break;

            case OTF2_TYPE_COMM:
                /* OTF2_TYPE_COMM attributes reference a Comm definition */
                OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->value.commRef );
                break;

            case OTF2_TYPE_PARAMETER:
                /* OTF2_TYPE_PARAMETER attributes reference a Parameter definition */
                OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->value.parameterRef );
                break;

            case OTF2_TYPE_RMA_WIN:
                /* OTF2_TYPE_RMA_WIN attributes reference a RmaWin definition */
                OTF2_Buffer_WriteUint32( writerHandle->buffer, attr->value.rmaWinRef );
                break;

            default:
                /*
                 * When getting an unknown type, we write it nevertheless as an
                 * uint64, because we assume that future additions will always be
                 * definition references and they are always of compressed types.
                 */
                OTF2_Buffer_WriteUint64( writerHandle->buffer, attr->value.uint64 );
                break;
        }
    }

    otf2_attribute_list_remove_all_attributes( attributeList );

    /* Write real record length */
    return OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, data_size_of_attr_list );
}


OTF2_ErrorCode
OTF2_EvtWriter_BufferFlush( OTF2_EvtWriter*     writerHandle,
                            OTF2_AttributeList* attributeList,
                            OTF2_TimeStamp      time,
                            OTF2_TimeStamp      stopTime )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_TimeStamp ); /* stopTime */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_BUFFER_FLUSH );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint64Full( writerHandle->buffer, stopTime );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MeasurementOnOff( OTF2_EvtWriter*      writerHandle,
                                 OTF2_AttributeList*  attributeList,
                                 OTF2_TimeStamp       time,
                                 OTF2_MeasurementMode measurementMode )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_MeasurementMode ); /* measurementMode */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MEASUREMENT_ON_OFF );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint8( writerHandle->buffer, measurementMode );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_Enter( OTF2_EvtWriter*     writerHandle,
                      OTF2_AttributeList* attributeList,
                      OTF2_TimeStamp      time,
                      OTF2_RegionRef      region )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RegionRef ) + 1; /* region */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_ENTER );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, region );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_Leave( OTF2_EvtWriter*     writerHandle,
                      OTF2_AttributeList* attributeList,
                      OTF2_TimeStamp      time,
                      OTF2_RegionRef      region )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RegionRef ) + 1; /* region */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_LEAVE );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, region );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiSend( OTF2_EvtWriter*     writerHandle,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time,
                        uint32_t            receiver,
                        OTF2_CommRef        communicator,
                        uint32_t            msgTag,
                        uint64_t            msgLength )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint32_t ) + 1;     /* receiver */
    record_data_length += sizeof( OTF2_CommRef ) + 1; /* communicator */
    record_data_length += sizeof( uint32_t ) + 1;     /* msgTag */
    record_data_length += sizeof( uint64_t ) + 1;     /* msgLength */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_SEND );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, receiver );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, communicator );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, msgTag );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, msgLength );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiIsend( OTF2_EvtWriter*     writerHandle,
                         OTF2_AttributeList* attributeList,
                         OTF2_TimeStamp      time,
                         uint32_t            receiver,
                         OTF2_CommRef        communicator,
                         uint32_t            msgTag,
                         uint64_t            msgLength,
                         uint64_t            requestID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint32_t ) + 1;     /* receiver */
    record_data_length += sizeof( OTF2_CommRef ) + 1; /* communicator */
    record_data_length += sizeof( uint32_t ) + 1;     /* msgTag */
    record_data_length += sizeof( uint64_t ) + 1;     /* msgLength */
    record_data_length += sizeof( uint64_t ) + 1;     /* requestID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_ISEND );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, receiver );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, communicator );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, msgTag );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, msgLength );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, requestID );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiIsendComplete( OTF2_EvtWriter*     writerHandle,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 uint64_t            requestID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint64_t ) + 1; /* requestID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_ISEND_COMPLETE );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, requestID );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiIrecvRequest( OTF2_EvtWriter*     writerHandle,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      time,
                                uint64_t            requestID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint64_t ) + 1; /* requestID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_IRECV_REQUEST );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, requestID );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiRecv( OTF2_EvtWriter*     writerHandle,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time,
                        uint32_t            sender,
                        OTF2_CommRef        communicator,
                        uint32_t            msgTag,
                        uint64_t            msgLength )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint32_t ) + 1;     /* sender */
    record_data_length += sizeof( OTF2_CommRef ) + 1; /* communicator */
    record_data_length += sizeof( uint32_t ) + 1;     /* msgTag */
    record_data_length += sizeof( uint64_t ) + 1;     /* msgLength */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_RECV );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, sender );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, communicator );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, msgTag );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, msgLength );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiIrecv( OTF2_EvtWriter*     writerHandle,
                         OTF2_AttributeList* attributeList,
                         OTF2_TimeStamp      time,
                         uint32_t            sender,
                         OTF2_CommRef        communicator,
                         uint32_t            msgTag,
                         uint64_t            msgLength,
                         uint64_t            requestID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint32_t ) + 1;     /* sender */
    record_data_length += sizeof( OTF2_CommRef ) + 1; /* communicator */
    record_data_length += sizeof( uint32_t ) + 1;     /* msgTag */
    record_data_length += sizeof( uint64_t ) + 1;     /* msgLength */
    record_data_length += sizeof( uint64_t ) + 1;     /* requestID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_IRECV );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, sender );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, communicator );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, msgTag );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, msgLength );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, requestID );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiRequestTest( OTF2_EvtWriter*     writerHandle,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               uint64_t            requestID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint64_t ) + 1; /* requestID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_REQUEST_TEST );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, requestID );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiRequestCancelled( OTF2_EvtWriter*     writerHandle,
                                    OTF2_AttributeList* attributeList,
                                    OTF2_TimeStamp      time,
                                    uint64_t            requestID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint64_t ) + 1; /* requestID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_REQUEST_CANCELLED );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, requestID );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiCollectiveBegin( OTF2_EvtWriter*     writerHandle,
                                   OTF2_AttributeList* attributeList,
                                   OTF2_TimeStamp      time  )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * For records without attributes, we need one additional zero byte for the
     * record length.
     */
    record_length += 1;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_COLLECTIVE_BEGIN );

    /* Write the zero byte, which indicates no attributes. */
    OTF2_Buffer_WriteUint8( writerHandle->buffer, 0 );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_MpiCollectiveEnd( OTF2_EvtWriter*     writerHandle,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 OTF2_CollectiveOp   collectiveOp,
                                 OTF2_CommRef        communicator,
                                 uint32_t            root,
                                 uint64_t            sizeSent,
                                 uint64_t            sizeReceived )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_CollectiveOp ); /* collectiveOp */
    record_data_length += sizeof( OTF2_CommRef ) + 1;  /* communicator */
    record_data_length += sizeof( uint32_t ) + 1;      /* root */
    record_data_length += sizeof( uint64_t ) + 1;      /* sizeSent */
    record_data_length += sizeof( uint64_t ) + 1;      /* sizeReceived */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_MPI_COLLECTIVE_END );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint8( writerHandle->buffer, collectiveOp );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, communicator );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, root );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, sizeSent );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, sizeReceived );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_OmpFork( OTF2_EvtWriter*     writerHandle,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time,
                        uint32_t            numberOfRequestedThreads )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint32_t ) + 1; /* numberOfRequestedThreads */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_OMP_FORK );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, numberOfRequestedThreads );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_OmpJoin( OTF2_EvtWriter*     writerHandle,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time  )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * For records without attributes, we need one additional zero byte for the
     * record length.
     */
    record_length += 1;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_OMP_JOIN );

    /* Write the zero byte, which indicates no attributes. */
    OTF2_Buffer_WriteUint8( writerHandle->buffer, 0 );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_OmpAcquireLock( OTF2_EvtWriter*     writerHandle,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               uint32_t            lockID,
                               uint32_t            acquisitionOrder )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint32_t ) + 1; /* lockID */
    record_data_length += sizeof( uint32_t ) + 1; /* acquisitionOrder */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_OMP_ACQUIRE_LOCK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, lockID );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, acquisitionOrder );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_OmpReleaseLock( OTF2_EvtWriter*     writerHandle,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               uint32_t            lockID,
                               uint32_t            acquisitionOrder )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint32_t ) + 1; /* lockID */
    record_data_length += sizeof( uint32_t ) + 1; /* acquisitionOrder */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_OMP_RELEASE_LOCK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, lockID );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, acquisitionOrder );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_OmpTaskCreate( OTF2_EvtWriter*     writerHandle,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              uint64_t            taskID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint64_t ) + 1; /* taskID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_OMP_TASK_CREATE );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, taskID );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_OmpTaskSwitch( OTF2_EvtWriter*     writerHandle,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              uint64_t            taskID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint64_t ) + 1; /* taskID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_OMP_TASK_SWITCH );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, taskID );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_OmpTaskComplete( OTF2_EvtWriter*     writerHandle,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      time,
                                uint64_t            taskID )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( uint64_t ) + 1; /* taskID */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_OMP_TASK_COMPLETE );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, taskID );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_Metric( OTF2_EvtWriter*         writerHandle,
                       OTF2_AttributeList*     attributeList,
                       OTF2_TimeStamp          time,
                       OTF2_MetricRef          metric,
                       uint8_t                 numberOfMetrics,
                       const OTF2_Type*        typeIDs,
                       const OTF2_MetricValue* metricValues )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_MetricRef ) + 1;              /* metric */
    record_data_length += sizeof( uint8_t );                         /* numberOfMetrics */
    record_data_length += numberOfMetrics * ( sizeof( OTF2_Type ) ); /* typeIDs */
    if ( numberOfMetrics > 0 && !typeIDs )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid typeIDs array argument." );
    }
    record_data_length += numberOfMetrics * ( sizeof( OTF2_MetricValue ) + 1 ); /* metricValues */
    if ( numberOfMetrics > 0 && !metricValues )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid metricValues array argument." );
    }
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_METRIC );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, metric );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, numberOfMetrics );
    for ( uint8_t numberOfMetrics_i = 0;
          numberOfMetrics_i < numberOfMetrics;
          numberOfMetrics_i++ )
    {
        OTF2_Buffer_WriteUint8( writerHandle->buffer,
                                typeIDs[ numberOfMetrics_i ] );
        OTF2_Buffer_WriteMetricValue( writerHandle->buffer,
                                      metricValues[ numberOfMetrics_i ] );
    }

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ParameterString( OTF2_EvtWriter*     writerHandle,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      time,
                                OTF2_ParameterRef   parameter,
                                OTF2_StringRef      string )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_ParameterRef ) + 1; /* parameter */
    record_data_length += sizeof( OTF2_StringRef ) + 1;    /* string */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_PARAMETER_STRING );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, parameter );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, string );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ParameterInt( OTF2_EvtWriter*     writerHandle,
                             OTF2_AttributeList* attributeList,
                             OTF2_TimeStamp      time,
                             OTF2_ParameterRef   parameter,
                             int64_t             value )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_ParameterRef ) + 1; /* parameter */
    record_data_length += sizeof( int64_t ) + 1;           /* value */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_PARAMETER_INT );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, parameter );
    OTF2_Buffer_WriteInt64( writerHandle->buffer, value );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ParameterUnsignedInt( OTF2_EvtWriter*     writerHandle,
                                     OTF2_AttributeList* attributeList,
                                     OTF2_TimeStamp      time,
                                     OTF2_ParameterRef   parameter,
                                     uint64_t            value )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_ParameterRef ) + 1; /* parameter */
    record_data_length += sizeof( uint64_t ) + 1;          /* value */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_PARAMETER_UNSIGNED_INT );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, parameter );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, value );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaWinCreate( OTF2_EvtWriter*     writerHandle,
                             OTF2_AttributeList* attributeList,
                             OTF2_TimeStamp      time,
                             OTF2_RmaWinRef      win )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_WIN_CREATE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaWinDestroy( OTF2_EvtWriter*     writerHandle,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              OTF2_RmaWinRef      win )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_WIN_DESTROY );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaCollectiveBegin( OTF2_EvtWriter*     writerHandle,
                                   OTF2_AttributeList* attributeList,
                                   OTF2_TimeStamp      time  )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * For records without attributes, we need one additional zero byte for the
     * record length.
     */
    record_length += 1;

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_COLLECTIVE_BEGIN );

    /* Write the zero byte, which indicates no attributes. */
    OTF2_Buffer_WriteUint8( writerHandle->buffer, 0 );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaCollectiveEnd( OTF2_EvtWriter*     writerHandle,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 OTF2_CollectiveOp   collectiveOp,
                                 OTF2_RmaSyncLevel   syncLevel,
                                 OTF2_RmaWinRef      win,
                                 uint32_t            root,
                                 uint64_t            bytesSent,
                                 uint64_t            bytesReceived )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_CollectiveOp );     /* collectiveOp */
    record_data_length += sizeof( OTF2_RmaSyncLevel ) + 1; /* syncLevel */
    record_data_length += sizeof( OTF2_RmaWinRef ) + 1;    /* win */
    record_data_length += sizeof( uint32_t ) + 1;          /* root */
    record_data_length += sizeof( uint64_t ) + 1;          /* bytesSent */
    record_data_length += sizeof( uint64_t ) + 1;          /* bytesReceived */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_COLLECTIVE_END );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint8( writerHandle->buffer, collectiveOp );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, syncLevel );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, root );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, bytesSent );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, bytesReceived );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaGroupSync( OTF2_EvtWriter*     writerHandle,
                             OTF2_AttributeList* attributeList,
                             OTF2_TimeStamp      time,
                             OTF2_RmaSyncLevel   syncLevel,
                             OTF2_RmaWinRef      win,
                             OTF2_GroupRef       group )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaSyncLevel ) + 1; /* syncLevel */
    record_data_length += sizeof( OTF2_RmaWinRef ) + 1;    /* win */
    record_data_length += sizeof( OTF2_GroupRef ) + 1;     /* group */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_GROUP_SYNC );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, syncLevel );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, group );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaRequestLock( OTF2_EvtWriter*     writerHandle,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               OTF2_RmaWinRef      win,
                               uint32_t            remote,
                               uint64_t            lockId,
                               OTF2_LockType       lockType )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint32_t ) + 1;       /* remote */
    record_data_length += sizeof( uint64_t ) + 1;       /* lockId */
    record_data_length += sizeof( OTF2_LockType );      /* lockType */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_REQUEST_LOCK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, remote );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, lockId );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, lockType );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaAcquireLock( OTF2_EvtWriter*     writerHandle,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               OTF2_RmaWinRef      win,
                               uint32_t            remote,
                               uint64_t            lockId,
                               OTF2_LockType       lockType )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint32_t ) + 1;       /* remote */
    record_data_length += sizeof( uint64_t ) + 1;       /* lockId */
    record_data_length += sizeof( OTF2_LockType );      /* lockType */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_ACQUIRE_LOCK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, remote );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, lockId );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, lockType );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaTryLock( OTF2_EvtWriter*     writerHandle,
                           OTF2_AttributeList* attributeList,
                           OTF2_TimeStamp      time,
                           OTF2_RmaWinRef      win,
                           uint32_t            remote,
                           uint64_t            lockId,
                           OTF2_LockType       lockType )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint32_t ) + 1;       /* remote */
    record_data_length += sizeof( uint64_t ) + 1;       /* lockId */
    record_data_length += sizeof( OTF2_LockType );      /* lockType */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_TRY_LOCK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, remote );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, lockId );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, lockType );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaReleaseLock( OTF2_EvtWriter*     writerHandle,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               OTF2_RmaWinRef      win,
                               uint32_t            remote,
                               uint64_t            lockId )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint32_t ) + 1;       /* remote */
    record_data_length += sizeof( uint64_t ) + 1;       /* lockId */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_RELEASE_LOCK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, remote );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, lockId );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaSync( OTF2_EvtWriter*     writerHandle,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time,
                        OTF2_RmaWinRef      win,
                        uint32_t            remote,
                        OTF2_RmaSyncType    syncType )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint32_t ) + 1;       /* remote */
    record_data_length += sizeof( OTF2_RmaSyncType );   /* syncType */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_SYNC );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, remote );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, syncType );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaWaitChange( OTF2_EvtWriter*     writerHandle,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              OTF2_RmaWinRef      win )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_WAIT_CHANGE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaPut( OTF2_EvtWriter*     writerHandle,
                       OTF2_AttributeList* attributeList,
                       OTF2_TimeStamp      time,
                       OTF2_RmaWinRef      win,
                       uint32_t            remote,
                       uint64_t            bytes,
                       uint64_t            matchingId )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint32_t ) + 1;       /* remote */
    record_data_length += sizeof( uint64_t ) + 1;       /* bytes */
    record_data_length += sizeof( uint64_t ) + 1;       /* matchingId */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_PUT );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, remote );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, bytes );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, matchingId );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaGet( OTF2_EvtWriter*     writerHandle,
                       OTF2_AttributeList* attributeList,
                       OTF2_TimeStamp      time,
                       OTF2_RmaWinRef      win,
                       uint32_t            remote,
                       uint64_t            bytes,
                       uint64_t            matchingId )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint32_t ) + 1;       /* remote */
    record_data_length += sizeof( uint64_t ) + 1;       /* bytes */
    record_data_length += sizeof( uint64_t ) + 1;       /* matchingId */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_GET );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, remote );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, bytes );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, matchingId );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaAtomic( OTF2_EvtWriter*     writerHandle,
                          OTF2_AttributeList* attributeList,
                          OTF2_TimeStamp      time,
                          OTF2_RmaWinRef      win,
                          uint32_t            remote,
                          OTF2_RmaAtomicType  type,
                          uint64_t            bytesSent,
                          uint64_t            bytesReceived,
                          uint64_t            matchingId )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint32_t ) + 1;       /* remote */
    record_data_length += sizeof( OTF2_RmaAtomicType ); /* type */
    record_data_length += sizeof( uint64_t ) + 1;       /* bytesSent */
    record_data_length += sizeof( uint64_t ) + 1;       /* bytesReceived */
    record_data_length += sizeof( uint64_t ) + 1;       /* matchingId */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_ATOMIC );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, remote );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, type );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, bytesSent );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, bytesReceived );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, matchingId );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaOpCompleteBlocking( OTF2_EvtWriter*     writerHandle,
                                      OTF2_AttributeList* attributeList,
                                      OTF2_TimeStamp      time,
                                      OTF2_RmaWinRef      win,
                                      uint64_t            matchingId )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint64_t ) + 1;       /* matchingId */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_OP_COMPLETE_BLOCKING );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, matchingId );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaOpCompleteNonBlocking( OTF2_EvtWriter*     writerHandle,
                                         OTF2_AttributeList* attributeList,
                                         OTF2_TimeStamp      time,
                                         OTF2_RmaWinRef      win,
                                         uint64_t            matchingId )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint64_t ) + 1;       /* matchingId */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_OP_COMPLETE_NON_BLOCKING );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, matchingId );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaOpTest( OTF2_EvtWriter*     writerHandle,
                          OTF2_AttributeList* attributeList,
                          OTF2_TimeStamp      time,
                          OTF2_RmaWinRef      win,
                          uint64_t            matchingId )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint64_t ) + 1;       /* matchingId */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_OP_TEST );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, matchingId );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_RmaOpCompleteRemote( OTF2_EvtWriter*     writerHandle,
                                    OTF2_AttributeList* attributeList,
                                    OTF2_TimeStamp      time,
                                    OTF2_RmaWinRef      win,
                                    uint64_t            matchingId )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* win */
    record_data_length += sizeof( uint64_t ) + 1;       /* matchingId */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_RMA_OP_COMPLETE_REMOTE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, win );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, matchingId );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadFork( OTF2_EvtWriter*     writerHandle,
                           OTF2_AttributeList* attributeList,
                           OTF2_TimeStamp      time,
                           OTF2_Paradigm       model,
                           uint32_t            numberOfRequestedThreads )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_Paradigm ); /* model */
    record_data_length += sizeof( uint32_t ) + 1;  /* numberOfRequestedThreads */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_FORK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint8( writerHandle->buffer, model );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, numberOfRequestedThreads );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadJoin( OTF2_EvtWriter*     writerHandle,
                           OTF2_AttributeList* attributeList,
                           OTF2_TimeStamp      time,
                           OTF2_Paradigm       model )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_Paradigm ); /* model */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_JOIN );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint8( writerHandle->buffer, model );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadTeamBegin( OTF2_EvtWriter*     writerHandle,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      time,
                                OTF2_CommRef        threadTeam )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_CommRef ) + 1; /* threadTeam */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_TEAM_BEGIN );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, threadTeam );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadTeamEnd( OTF2_EvtWriter*     writerHandle,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              OTF2_CommRef        threadTeam )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_CommRef ) + 1; /* threadTeam */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_TEAM_END );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, threadTeam );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadAcquireLock( OTF2_EvtWriter*     writerHandle,
                                  OTF2_AttributeList* attributeList,
                                  OTF2_TimeStamp      time,
                                  OTF2_Paradigm       model,
                                  uint32_t            lockID,
                                  uint32_t            acquisitionOrder )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_Paradigm ); /* model */
    record_data_length += sizeof( uint32_t ) + 1;  /* lockID */
    record_data_length += sizeof( uint32_t ) + 1;  /* acquisitionOrder */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_ACQUIRE_LOCK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint8( writerHandle->buffer, model );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, lockID );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, acquisitionOrder );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadReleaseLock( OTF2_EvtWriter*     writerHandle,
                                  OTF2_AttributeList* attributeList,
                                  OTF2_TimeStamp      time,
                                  OTF2_Paradigm       model,
                                  uint32_t            lockID,
                                  uint32_t            acquisitionOrder )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_Paradigm ); /* model */
    record_data_length += sizeof( uint32_t ) + 1;  /* lockID */
    record_data_length += sizeof( uint32_t ) + 1;  /* acquisitionOrder */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_RELEASE_LOCK );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint8( writerHandle->buffer, model );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, lockID );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, acquisitionOrder );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadTaskCreate( OTF2_EvtWriter*     writerHandle,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 OTF2_CommRef        threadTeam,
                                 uint32_t            creatingThread,
                                 uint32_t            generationNumber )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_CommRef ) + 1; /* threadTeam */
    record_data_length += sizeof( uint32_t ) + 1;     /* creatingThread */
    record_data_length += sizeof( uint32_t ) + 1;     /* generationNumber */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_TASK_CREATE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, threadTeam );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, creatingThread );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, generationNumber );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadTaskSwitch( OTF2_EvtWriter*     writerHandle,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 OTF2_CommRef        threadTeam,
                                 uint32_t            creatingThread,
                                 uint32_t            generationNumber )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_CommRef ) + 1; /* threadTeam */
    record_data_length += sizeof( uint32_t ) + 1;     /* creatingThread */
    record_data_length += sizeof( uint32_t ) + 1;     /* generationNumber */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_TASK_SWITCH );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, threadTeam );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, creatingThread );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, generationNumber );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}


OTF2_ErrorCode
OTF2_EvtWriter_ThreadTaskComplete( OTF2_EvtWriter*     writerHandle,
                                   OTF2_AttributeList* attributeList,
                                   OTF2_TimeStamp      time,
                                   OTF2_CommRef        threadTeam,
                                   uint32_t            creatingThread,
                                   uint32_t            generationNumber )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes (excluding the timestamp), plus the size of the attribute list.
     */
    uint64_t record_length = 1 + otf2_attribute_list_get_size( attributeList );

    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    record_data_length += sizeof( OTF2_CommRef ) + 1; /* threadTeam */
    record_data_length += sizeof( uint32_t ) + 1;     /* creatingThread */
    record_data_length += sizeof( uint32_t ) + 1;     /* generationNumber */
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional bytes to store real record length. For records that exceed 255 bytes
     * we need a maximum of 9 bytes to encode their length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    ret = OTF2_Buffer_WriteTimeStamp( writerHandle->buffer,
                                      time,
                                      record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    ret = otf2_evt_writer_write_attribute_list( writerHandle, attributeList );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_THREAD_TASK_COMPLETE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, threadTeam );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, creatingThread );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, generationNumber );

    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    return ret;
}
