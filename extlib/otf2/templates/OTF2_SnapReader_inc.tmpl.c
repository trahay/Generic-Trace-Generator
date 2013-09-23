/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2013,
 * Technische Universitaet Dresden, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 */


/**
 *  @file       src/OTF2_SnapReader_inc.c
 *  @source     templates/OTF2_SnapReader_inc.tmpl.c
 *
 */


@otf2 for snap in snaps:

static inline OTF2_ErrorCode
otf2_snap_reader_read_@@snap.lower@@( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_@@snap.snap_name@@* record = &reader->current_snap.record.@@snap.lower@@;

    OTF2_ErrorCode ret;
    uint64_t          record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read @@snap.name@@ record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    @otf2  if snap.versions|length > 1:
    uint8_t* current_pos;
    @otf2  endif
    @otf2 for v in snap.versions:
    @otf2  if v > snap.since:
    /*
     * Read record attributes added in version @@v.major@@.@@v.minor@@, if there is
     * space left in the record
     */
    OTF2_Buffer_GetPosition( reader->buffer, &current_pos );
    if ( current_pos < record_end_pos )
    {
    @otf2  endif
    @otf2  for attr in snap.record_attributes_by_version[v]:
    @otf2   if attr.type.errorcheck_on_read:
    ret = OTF2_Buffer_Read@@attr.type.otf2@@( reader->buffer, &record->@@attr.lower@@ );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read @@attr.name@@ attribute of @@snap.name@@ record. @@attr.type.errorcheck_on_read@@" );
    }
    @otf2   else:
    OTF2_Buffer_Read@@attr.type.otf2@@( reader->buffer, &record->@@attr.lower@@ );
    @otf2   endif
    @otf2   if attr.array_attributes:
    @otf2    for array_attr in attr.array_attributes:
    record->@@array_attr.lower@@ = malloc( record->@@attr.lower@@ * sizeof( @@array_attr.type@@ ) );
    if ( record->@@attr.lower@@ > 0 && !record->@@array_attr.lower@@ )
    {
        @otf2 for array_attr in attr.array_attributes[:loop.index - 1]:
        free( record->@@array_attr.lower@@ );
        @otf2 endfor
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                             "Could not allocate memory for @@array_attr.name@@ array!" );
    }
    @otf2    endfor
    for ( @@attr.type@@ @@attr.lower@@_i = 0;
          @@attr.lower@@_i < record->@@attr.lower@@;
          @@attr.lower@@_i++ )
    {
        @otf2 for array_attr in attr.array_attributes:

        @otf2  if array_attr.type.errorcheck_on_read:
        ret = OTF2_Buffer_Read@@array_attr.type.otf2@@( reader->buffer,
                                                        record->@@array_attr.lower@@ + @@attr.lower@@_i );
        if ( OTF2_SUCCESS != ret )
        {
            @otf2 for array_attr in attr.array_attributes:
            free( record->@@array_attr.lower@@ );
            @otf2 endfor
            return UTILS_ERROR( ret, "Could not read @@array_attr.name@@ attribute of @@snap.name@@ record. @@array_attr.type.errorcheck_on_read@@" );
        }
        @otf2  else:
        OTF2_Buffer_Read@@array_attr.type.otf2@@( reader->buffer,
                                                  record->@@array_attr.lower@@ + @@attr.lower@@_i );
        @otf2  endif        
        @otf2 endfor
    }
    @otf2   endif
    @otf2  endfor
    @otf2  if v > snap.since:
    }
    else
    {
        otf2_snap_@@snap.lower@@_convert_pre_@@v.major@@_@@v.minor@@( record );
    }
    @otf2  endif

    @otf2 endfor
    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        @otf2 for attr in snap.all_array_attributes:
        free( record->@@attr.lower@@ );
        @otf2 endfor
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    /*
     * If a local snap reader is operated by a global snap reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global snap reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.@@snap.lower@@ )
    {
        interrupt = reader->reader_callbacks.@@snap.lower@@( reader->location_id,
                                                             record->time,
                                                             reader->user_data,
                                                             &reader->attribute_list @@snap.callargs(case='lower', argprefix='record->')@@ );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );

    @otf2 for attr in snap.all_array_attributes:
    free( record->@@attr.lower@@ );
    @otf2 endfor

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}

@otf2 endfor

static inline OTF2_ErrorCode
otf2_snap_reader_read_attribute_list
(
    OTF2_SnapReader* reader
)
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode status;

    /* Get record length and test memory availability */
    uint64_t record_length;
    status = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_length );
    if ( OTF2_SUCCESS != status )
    {
        return UTILS_ERROR( status,
                             "Could not read attribute list. Not enough memory in buffer." );
    }

    /* Get how many attributes are in the list */
    uint32_t number_of_attributes;
    status = OTF2_Buffer_ReadUint32( reader->buffer, &number_of_attributes );
    if ( OTF2_SUCCESS != status )
    {
        return UTILS_ERROR( status, "Could not read number of attributes in list. Invalid compression size." );
    }

    /* Check if number of attributes is consistent with record length */
    if ( record_length > number_of_attributes * OTF2_MAXIMUM_ATTRIBUTE_LIST_ENTRY_SIZE + 5 )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                             "Could not read attribute list. Record length and number of attributes in list are not consistent." );
    }

    for ( uint32_t i = 0; i < number_of_attributes; i++ )
    {
        uint32_t            attribute_id;
        OTF2_Type           type;
        OTF2_AttributeValue value;

        /* Read attribute id from trace */
        status = OTF2_Buffer_ReadUint32( reader->buffer, &attribute_id );
        if ( OTF2_SUCCESS != status )
        {
            return UTILS_ERROR( status, "Could not read attribute from list. Invalid compression size." );
        }

        /* Read the type from trace */
        OTF2_Buffer_ReadUint8( reader->buffer, &type );

        /* Cast the container to the right type and read it from the trace */
        switch ( type )
        {
            @otf2 for entry in Type.entries if entry.type:
            case @@entry.name@@:
                @otf2 if entry.type.errorcheck_on_read
                status = OTF2_Buffer_Read@@entry.type.otf2@@( reader->buffer,
                                                              &value.@@entry.type.lower@@ );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read attribute. @@entry.type.errorcheck_on_read@@" );
                }
                @otf2 else:
                OTF2_Buffer_Read@@entry.type.otf2@@( reader->buffer,
                                                     &value.@@entry.type.lower@@ );
                @otf2 endif
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
                status = OTF2_Buffer_Read@@def.self_attr.type.otf2@@( reader->buffer, &value.@@def.ref_lname@@Ref );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read @@def.name@@ attribute. Invalid compression size." );
                }
                break;

            @otf2 endwith
            @otf2 endfor
            default:
                return UTILS_ERROR( OTF2_ERROR_UNKNOWN_TYPE,
                                     "Type ID is not valid!" );
                break;
        }

        /* Store the new value in the attribute list. */
        OTF2_AttributeList_AddAttribute( &reader->attribute_list,
                                         attribute_id,
                                         type,
                                         value );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_snap_reader_read( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode  status = OTF2_ERROR_INVALID;
    OTF2_GenericSnap* snap  = &reader->current_snap;

    status = OTF2_Buffer_ReadTimeStamp( reader->buffer, &snap->record.time );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Read of timestamp failed!" );
    }

    /* Read record type */
    status = OTF2_Buffer_Guarantee( reader->buffer, 1 );
    if ( OTF2_SUCCESS != status )
    {
        return UTILS_ERROR( status, "Could not read record type." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &snap->type );

    switch ( snap->type )
    {
        case OTF2_BUFFER_END_OF_CHUNK:
            status = OTF2_Buffer_ReadGetNextChunk( reader->buffer );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status, "Load of next chunk failed!" );
            }
            return otf2_snap_reader_read( reader );

        case OTF2_BUFFER_END_OF_FILE:
            return OTF2_ERROR_INDEX_OUT_OF_BOUNDS;

        case OTF2_ATTRIBUTE_LIST:
            status = otf2_snap_reader_read_attribute_list( reader );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status, "Read of additional attributes failed!" );
            }
            return otf2_snap_reader_read( reader );

        @otf2 for snap in snaps:
        case OTF2_SNAP_@@snap.upper@@:
            return otf2_snap_reader_read_@@snap.lower@@( reader );

        @otf2 endfor
        default:
            return otf2_snap_reader_read_unknown( reader );
    }
}

@otf2 for snap in snaps:

OTF2_ErrorCode
OTF2_SnapReaderCallbacks_Set@@snap.name@@Callback(
    OTF2_SnapReaderCallbacks*              snapReaderCallbacks,
    OTF2_SnapReaderCallback_@@snap.name@@ @@snap.lname@@Callback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                             "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->@@snap.lower@@ = @@snap.lname@@Callback;

    return OTF2_SUCCESS;
}

@otf2 endfor
