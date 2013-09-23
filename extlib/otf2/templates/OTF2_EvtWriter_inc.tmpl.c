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
            @otf2 for entry in Type.entries if entry.type:
            case @@entry.name@@:
                OTF2_Buffer_Write@@entry.type.otf2@@( writerHandle->buffer,
                                                      attr->value.@@entry.type.lower@@ );
                break;

            @otf2 endfor
            @otf2 for entry in MappingType.entries:
            @otf2 with def = entry.def
            case @@Type.prefix@@@@entry.suffix@@:
                @otf2 if def is shared_def:
                /* @@Type.prefix@@@@entry.suffix@@ attributes reference a @@def.siblings|join(', or a ')@@ definition */
                @otf2 else:
                /* @@Type.prefix@@@@entry.suffix@@ attributes reference a @@def.name@@ definition */
                @otf2 endif
                OTF2_Buffer_Write@@def.self_attr.type.otf2@@( writerHandle->buffer, attr->value.@@def.ref_lname@@Ref );
                break;

            @otf2 endwith
            @otf2 endfor
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

@otf2 for event in events:

OTF2_ErrorCode
OTF2_EvtWriter_@@event.name@@( OTF2_EvtWriter*     writerHandle,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time @@event.funcargs()@@ )
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

    @otf2 if event.attributes:
    /*
     * Maximum memory usage of pure record data (excluding timestamp, type ID,
     * record length information, and optional attribute list).
     */
    uint64_t record_data_length = 0;

    @otf2  for attr in event.attributes:
    @otf2   if attr.type.errorcheck_on_argument:
    if ( !@@attr.name@@ )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "@@attr.type.errorcheck_on_argument@@" );
    }
    @otf2   endif
    record_data_length += @@attr.size()@@; /* @@attr.name@@ */
    @otf2   if attr is array_attr:
    @otf2    for array_attr in attr.array_attributes:
    record_data_length += @@attr.name@@ * ( @@array_attr.size()@@ ); /* @@array_attr.name@@ */
    if ( @@attr.name@@ > 0 && !@@array_attr.name@@ )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid @@array_attr.name@@ array argument." );
    }
    @otf2    endfor
    @otf2   endif
    @otf2  endfor
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    @otf2  if event is not singleton_event:
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

    @otf2  endif
    @otf2 else:
    /*
     * For records without attributes, we need one additional zero byte for the
     * record length.
     */
    record_length += 1;

    @otf2 endif
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_EVENT_@@event.upper@@ );

    @otf2 if event.attributes:
    @otf2  if event is not singleton_event:
    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    @otf2  endif
    @otf2  for attr in event.attributes:
    OTF2_Buffer_Write@@attr.type.otf2@@( writerHandle->buffer, @@attr.name@@ );
    @otf2   if attr is array_attr:
    for ( @@attr.type@@ @@attr.name@@_i = 0;
          @@attr.name@@_i < @@attr.name@@;
          @@attr.name@@_i++ )
    {
        @otf2 for array_attr in attr.array_attributes:
        OTF2_Buffer_Write@@array_attr.type.otf2@@( writerHandle->buffer,
                                                   @@array_attr.name@@[ @@attr.name@@_i ] );
        @otf2 endfor
    }
    @otf2   endif
    @otf2  endfor

    @otf2  if event is not singleton_event:
    /* Write real record length */
    ret = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );

    @otf2  endif
    @otf2 else:
    /* Write the zero byte, which indicates no attributes. */
    OTF2_Buffer_WriteUint8( writerHandle->buffer, 0 );

    @otf2 endif
    return ret;
}

@otf2 endfor
