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



static inline OTF2_ErrorCode
otf2_snap_reader_read_snapshot_start( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_SnapshotStart* record = &reader->current_snap.record.snapshot_start;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read SnapshotStart record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->number_of_records );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfRecords attribute of SnapshotStart record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.snapshot_start )
    {
        interrupt = reader->reader_callbacks.snapshot_start( reader->location_id,
                                                             record->time,
                                                             reader->user_data,
                                                             &reader->attribute_list,
                                                             record->number_of_records );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_snapshot_end( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_SnapshotEnd* record = &reader->current_snap.record.snapshot_end;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read SnapshotEnd record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->cont_read_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read contReadPos attribute of SnapshotEnd record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.snapshot_end )
    {
        interrupt = reader->reader_callbacks.snapshot_end( reader->location_id,
                                                           record->time,
                                                           reader->user_data,
                                                           &reader->attribute_list,
                                                           record->cont_read_pos );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_measurement_on_off( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MeasurementOnOffSnap* record = &reader->current_snap.record.measurement_on_off;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MeasurementOnOff record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    OTF2_Buffer_ReadUint8( reader->buffer, &record->measurement_mode );

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.measurement_on_off )
    {
        interrupt = reader->reader_callbacks.measurement_on_off( reader->location_id,
                                                                 record->time,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
                                                                 record->orig_event_time,
                                                                 record->measurement_mode );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_enter( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_EnterSnap* record = &reader->current_snap.record.enter;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read Enter record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->region );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read region attribute of Enter record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.enter )
    {
        interrupt = reader->reader_callbacks.enter( reader->location_id,
                                                    record->time,
                                                    reader->user_data,
                                                    &reader->attribute_list,
                                                    record->orig_event_time,
                                                    record->region );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_mpi_send( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiSendSnap* record = &reader->current_snap.record.mpi_send;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiSend record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->receiver );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read receiver attribute of MpiSend record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->communicator );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read communicator attribute of MpiSend record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->msg_tag );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgTag attribute of MpiSend record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->msg_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgLength attribute of MpiSend record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.mpi_send )
    {
        interrupt = reader->reader_callbacks.mpi_send( reader->location_id,
                                                       record->time,
                                                       reader->user_data,
                                                       &reader->attribute_list,
                                                       record->orig_event_time,
                                                       record->receiver,
                                                       record->communicator,
                                                       record->msg_tag,
                                                       record->msg_length );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_mpi_isend( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiIsendSnap* record = &reader->current_snap.record.mpi_isend;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiIsend record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->receiver );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read receiver attribute of MpiIsend record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->communicator );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read communicator attribute of MpiIsend record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->msg_tag );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgTag attribute of MpiIsend record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->msg_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgLength attribute of MpiIsend record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->request_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read requestID attribute of MpiIsend record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.mpi_isend )
    {
        interrupt = reader->reader_callbacks.mpi_isend( reader->location_id,
                                                        record->time,
                                                        reader->user_data,
                                                        &reader->attribute_list,
                                                        record->orig_event_time,
                                                        record->receiver,
                                                        record->communicator,
                                                        record->msg_tag,
                                                        record->msg_length,
                                                        record->request_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_mpi_isend_complete( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiIsendCompleteSnap* record = &reader->current_snap.record.mpi_isend_complete;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiIsendComplete record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->request_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read requestID attribute of MpiIsendComplete record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.mpi_isend_complete )
    {
        interrupt = reader->reader_callbacks.mpi_isend_complete( reader->location_id,
                                                                 record->time,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
                                                                 record->orig_event_time,
                                                                 record->request_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_mpi_recv( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiRecvSnap* record = &reader->current_snap.record.mpi_recv;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiRecv record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->sender );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read sender attribute of MpiRecv record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->communicator );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read communicator attribute of MpiRecv record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->msg_tag );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgTag attribute of MpiRecv record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->msg_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgLength attribute of MpiRecv record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.mpi_recv )
    {
        interrupt = reader->reader_callbacks.mpi_recv( reader->location_id,
                                                       record->time,
                                                       reader->user_data,
                                                       &reader->attribute_list,
                                                       record->orig_event_time,
                                                       record->sender,
                                                       record->communicator,
                                                       record->msg_tag,
                                                       record->msg_length );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_mpi_irecv_request( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiIrecvRequestSnap* record = &reader->current_snap.record.mpi_irecv_request;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiIrecvRequest record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->request_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read requestID attribute of MpiIrecvRequest record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.mpi_irecv_request )
    {
        interrupt = reader->reader_callbacks.mpi_irecv_request( reader->location_id,
                                                                record->time,
                                                                reader->user_data,
                                                                &reader->attribute_list,
                                                                record->orig_event_time,
                                                                record->request_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_mpi_irecv( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiIrecvSnap* record = &reader->current_snap.record.mpi_irecv;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiIrecv record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->sender );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read sender attribute of MpiIrecv record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->communicator );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read communicator attribute of MpiIrecv record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->msg_tag );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgTag attribute of MpiIrecv record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->msg_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgLength attribute of MpiIrecv record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->request_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read requestID attribute of MpiIrecv record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.mpi_irecv )
    {
        interrupt = reader->reader_callbacks.mpi_irecv( reader->location_id,
                                                        record->time,
                                                        reader->user_data,
                                                        &reader->attribute_list,
                                                        record->orig_event_time,
                                                        record->sender,
                                                        record->communicator,
                                                        record->msg_tag,
                                                        record->msg_length,
                                                        record->request_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_mpi_collective_begin( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiCollectiveBeginSnap* record = &reader->current_snap.record.mpi_collective_begin;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiCollectiveBegin record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.mpi_collective_begin )
    {
        interrupt = reader->reader_callbacks.mpi_collective_begin( reader->location_id,
                                                                   record->time,
                                                                   reader->user_data,
                                                                   &reader->attribute_list,
                                                                   record->orig_event_time );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_mpi_collective_end( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiCollectiveEndSnap* record = &reader->current_snap.record.mpi_collective_end;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiCollectiveEnd record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    OTF2_Buffer_ReadUint8( reader->buffer, &record->collective_op );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->communicator );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read communicator attribute of MpiCollectiveEnd record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->root );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read root attribute of MpiCollectiveEnd record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->size_sent );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read sizeSent attribute of MpiCollectiveEnd record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->size_received );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read sizeReceived attribute of MpiCollectiveEnd record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.mpi_collective_end )
    {
        interrupt = reader->reader_callbacks.mpi_collective_end( reader->location_id,
                                                                 record->time,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
                                                                 record->orig_event_time,
                                                                 record->collective_op,
                                                                 record->communicator,
                                                                 record->root,
                                                                 record->size_sent,
                                                                 record->size_received );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_omp_fork( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpForkSnap* record = &reader->current_snap.record.omp_fork;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpFork record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->number_of_requested_threads );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfRequestedThreads attribute of OmpFork record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.omp_fork )
    {
        interrupt = reader->reader_callbacks.omp_fork( reader->location_id,
                                                       record->time,
                                                       reader->user_data,
                                                       &reader->attribute_list,
                                                       record->orig_event_time,
                                                       record->number_of_requested_threads );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_omp_acquire_lock( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpAcquireLockSnap* record = &reader->current_snap.record.omp_acquire_lock;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpAcquireLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->lock_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lockID attribute of OmpAcquireLock record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->acquisition_order );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read acquisitionOrder attribute of OmpAcquireLock record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.omp_acquire_lock )
    {
        interrupt = reader->reader_callbacks.omp_acquire_lock( reader->location_id,
                                                               record->time,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->orig_event_time,
                                                               record->lock_id,
                                                               record->acquisition_order );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_omp_task_create( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpTaskCreateSnap* record = &reader->current_snap.record.omp_task_create;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpTaskCreate record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->task_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read taskID attribute of OmpTaskCreate record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.omp_task_create )
    {
        interrupt = reader->reader_callbacks.omp_task_create( reader->location_id,
                                                              record->time,
                                                              reader->user_data,
                                                              &reader->attribute_list,
                                                              record->orig_event_time,
                                                              record->task_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_omp_task_switch( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpTaskSwitchSnap* record = &reader->current_snap.record.omp_task_switch;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpTaskSwitch record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->task_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read taskID attribute of OmpTaskSwitch record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.omp_task_switch )
    {
        interrupt = reader->reader_callbacks.omp_task_switch( reader->location_id,
                                                              record->time,
                                                              reader->user_data,
                                                              &reader->attribute_list,
                                                              record->orig_event_time,
                                                              record->task_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_metric( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MetricSnap* record = &reader->current_snap.record.metric;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read Metric record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->metric );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read metric attribute of Metric record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record->number_of_metrics );
    record->type_ids = malloc( record->number_of_metrics * sizeof( OTF2_Type ) );
    if ( record->number_of_metrics > 0 && !record->type_ids )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Could not allocate memory for typeIDs array!" );
    }
    record->metric_values = malloc( record->number_of_metrics * sizeof( OTF2_MetricValue ) );
    if ( record->number_of_metrics > 0 && !record->metric_values )
    {
        free( record->type_ids );
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Could not allocate memory for metricValues array!" );
    }
    for ( uint8_t number_of_metrics_i = 0;
          number_of_metrics_i < record->number_of_metrics;
          number_of_metrics_i++ )
    {
        OTF2_Buffer_ReadUint8( reader->buffer,
                               record->type_ids + number_of_metrics_i );

        ret = OTF2_Buffer_ReadMetricValue( reader->buffer,
                                           record->metric_values + number_of_metrics_i );
        if ( OTF2_SUCCESS != ret )
        {
            free( record->type_ids );
            free( record->metric_values );
            return UTILS_ERROR( ret, "Could not read metricValues attribute of Metric record. Invalid compression size." );
        }
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        free( record->type_ids );
        free( record->metric_values );
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
    if ( reader->reader_callbacks.metric )
    {
        interrupt = reader->reader_callbacks.metric( reader->location_id,
                                                     record->time,
                                                     reader->user_data,
                                                     &reader->attribute_list,
                                                     record->orig_event_time,
                                                     record->metric,
                                                     record->number_of_metrics,
                                                     record->type_ids,
                                                     record->metric_values );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );

    free( record->type_ids );
    free( record->metric_values );

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_parameter_string( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ParameterStringSnap* record = &reader->current_snap.record.parameter_string;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ParameterString record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->parameter );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parameter attribute of ParameterString record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->string );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read string attribute of ParameterString record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.parameter_string )
    {
        interrupt = reader->reader_callbacks.parameter_string( reader->location_id,
                                                               record->time,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->orig_event_time,
                                                               record->parameter,
                                                               record->string );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_parameter_int( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ParameterIntSnap* record = &reader->current_snap.record.parameter_int;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ParameterInt record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->parameter );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parameter attribute of ParameterInt record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadInt64( reader->buffer, &record->value );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read value attribute of ParameterInt record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.parameter_int )
    {
        interrupt = reader->reader_callbacks.parameter_int( reader->location_id,
                                                            record->time,
                                                            reader->user_data,
                                                            &reader->attribute_list,
                                                            record->orig_event_time,
                                                            record->parameter,
                                                            record->value );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_snap_reader_read_parameter_unsigned_int( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ParameterUnsignedIntSnap* record = &reader->current_snap.record.parameter_unsigned_int;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ParameterUnsignedInt record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->orig_event_time );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->parameter );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parameter attribute of ParameterUnsignedInt record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->value );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read value attribute of ParameterUnsignedInt record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
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
    if ( reader->reader_callbacks.parameter_unsigned_int )
    {
        interrupt = reader->reader_callbacks.parameter_unsigned_int( reader->location_id,
                                                                     record->time,
                                                                     reader->user_data,
                                                                     &reader->attribute_list,
                                                                     record->orig_event_time,
                                                                     record->parameter,
                                                                     record->value );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


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
            case OTF2_TYPE_UINT8:
                OTF2_Buffer_ReadUint8( reader->buffer,
                                       &value.uint8 );
                break;

            case OTF2_TYPE_UINT16:
                OTF2_Buffer_ReadUint16( reader->buffer,
                                        &value.uint16 );
                break;

            case OTF2_TYPE_UINT32:
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.uint32 );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_UINT64:
                status = OTF2_Buffer_ReadUint64( reader->buffer,
                                                 &value.uint64 );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_INT8:
                OTF2_Buffer_ReadInt8( reader->buffer,
                                      &value.int8 );
                break;

            case OTF2_TYPE_INT16:
                OTF2_Buffer_ReadInt16( reader->buffer,
                                       &value.int16 );
                break;

            case OTF2_TYPE_INT32:
                status = OTF2_Buffer_ReadInt32( reader->buffer,
                                                &value.int32 );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_INT64:
                status = OTF2_Buffer_ReadInt64( reader->buffer,
                                                &value.int64 );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_FLOAT:
                OTF2_Buffer_ReadFloat( reader->buffer,
                                       &value.float32 );
                break;

            case OTF2_TYPE_DOUBLE:
                OTF2_Buffer_ReadDouble( reader->buffer,
                                        &value.float64 );
                break;

            case OTF2_TYPE_STRING:
                /* OTF2_TYPE_STRING attributes reference a String definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer, &value.stringRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read String attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_ATTRIBUTE:
                /* OTF2_TYPE_ATTRIBUTE attributes reference a Attribute definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer, &value.attributeRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read Attribute attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_LOCATION:
                /* OTF2_TYPE_LOCATION attributes reference a Location definition */
                status = OTF2_Buffer_ReadUint64( reader->buffer, &value.locationRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read Location attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_REGION:
                /* OTF2_TYPE_REGION attributes reference a Region definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer, &value.regionRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read Region attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_GROUP:
                /* OTF2_TYPE_GROUP attributes reference a Group definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer, &value.groupRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read Group attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_METRIC:
                /* OTF2_TYPE_METRIC attributes reference a MetricClass, or a MetricInstance definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer, &value.metricRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read MetricClass attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_COMM:
                /* OTF2_TYPE_COMM attributes reference a Comm definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer, &value.commRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read Comm attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_PARAMETER:
                /* OTF2_TYPE_PARAMETER attributes reference a Parameter definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer, &value.parameterRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read Parameter attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_RMA_WIN:
                /* OTF2_TYPE_RMA_WIN attributes reference a RmaWin definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer, &value.rmaWinRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status, "Could not read RmaWin attribute. Invalid compression size." );
                }
                break;

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

    OTF2_ErrorCode    status = OTF2_ERROR_INVALID;
    OTF2_GenericSnap* snap   = &reader->current_snap;

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

        case OTF2_SNAP_SNAPSHOT_START:
            return otf2_snap_reader_read_snapshot_start( reader );

        case OTF2_SNAP_SNAPSHOT_END:
            return otf2_snap_reader_read_snapshot_end( reader );

        case OTF2_SNAP_MEASUREMENT_ON_OFF:
            return otf2_snap_reader_read_measurement_on_off( reader );

        case OTF2_SNAP_ENTER:
            return otf2_snap_reader_read_enter( reader );

        case OTF2_SNAP_MPI_SEND:
            return otf2_snap_reader_read_mpi_send( reader );

        case OTF2_SNAP_MPI_ISEND:
            return otf2_snap_reader_read_mpi_isend( reader );

        case OTF2_SNAP_MPI_ISEND_COMPLETE:
            return otf2_snap_reader_read_mpi_isend_complete( reader );

        case OTF2_SNAP_MPI_RECV:
            return otf2_snap_reader_read_mpi_recv( reader );

        case OTF2_SNAP_MPI_IRECV_REQUEST:
            return otf2_snap_reader_read_mpi_irecv_request( reader );

        case OTF2_SNAP_MPI_IRECV:
            return otf2_snap_reader_read_mpi_irecv( reader );

        case OTF2_SNAP_MPI_COLLECTIVE_BEGIN:
            return otf2_snap_reader_read_mpi_collective_begin( reader );

        case OTF2_SNAP_MPI_COLLECTIVE_END:
            return otf2_snap_reader_read_mpi_collective_end( reader );

        case OTF2_SNAP_OMP_FORK:
            return otf2_snap_reader_read_omp_fork( reader );

        case OTF2_SNAP_OMP_ACQUIRE_LOCK:
            return otf2_snap_reader_read_omp_acquire_lock( reader );

        case OTF2_SNAP_OMP_TASK_CREATE:
            return otf2_snap_reader_read_omp_task_create( reader );

        case OTF2_SNAP_OMP_TASK_SWITCH:
            return otf2_snap_reader_read_omp_task_switch( reader );

        case OTF2_SNAP_METRIC:
            return otf2_snap_reader_read_metric( reader );

        case OTF2_SNAP_PARAMETER_STRING:
            return otf2_snap_reader_read_parameter_string( reader );

        case OTF2_SNAP_PARAMETER_INT:
            return otf2_snap_reader_read_parameter_int( reader );

        case OTF2_SNAP_PARAMETER_UNSIGNED_INT:
            return otf2_snap_reader_read_parameter_unsigned_int( reader );

        default:
            return otf2_snap_reader_read_unknown( reader );
    }
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetSnapshotStartCallback(
    OTF2_SnapReaderCallbacks*             snapReaderCallbacks,
    OTF2_SnapReaderCallback_SnapshotStart snapshotStartCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->snapshot_start = snapshotStartCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetSnapshotEndCallback(
    OTF2_SnapReaderCallbacks*           snapReaderCallbacks,
    OTF2_SnapReaderCallback_SnapshotEnd snapshotEndCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->snapshot_end = snapshotEndCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMeasurementOnOffCallback(
    OTF2_SnapReaderCallbacks*                snapReaderCallbacks,
    OTF2_SnapReaderCallback_MeasurementOnOff measurementOnOffCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->measurement_on_off = measurementOnOffCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetEnterCallback(
    OTF2_SnapReaderCallbacks*     snapReaderCallbacks,
    OTF2_SnapReaderCallback_Enter enterCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->enter = enterCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMpiSendCallback(
    OTF2_SnapReaderCallbacks*       snapReaderCallbacks,
    OTF2_SnapReaderCallback_MpiSend mpiSendCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->mpi_send = mpiSendCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMpiIsendCallback(
    OTF2_SnapReaderCallbacks*        snapReaderCallbacks,
    OTF2_SnapReaderCallback_MpiIsend mpiIsendCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->mpi_isend = mpiIsendCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMpiIsendCompleteCallback(
    OTF2_SnapReaderCallbacks*                snapReaderCallbacks,
    OTF2_SnapReaderCallback_MpiIsendComplete mpiIsendCompleteCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->mpi_isend_complete = mpiIsendCompleteCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMpiRecvCallback(
    OTF2_SnapReaderCallbacks*       snapReaderCallbacks,
    OTF2_SnapReaderCallback_MpiRecv mpiRecvCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->mpi_recv = mpiRecvCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMpiIrecvRequestCallback(
    OTF2_SnapReaderCallbacks*               snapReaderCallbacks,
    OTF2_SnapReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->mpi_irecv_request = mpiIrecvRequestCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMpiIrecvCallback(
    OTF2_SnapReaderCallbacks*        snapReaderCallbacks,
    OTF2_SnapReaderCallback_MpiIrecv mpiIrecvCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->mpi_irecv = mpiIrecvCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMpiCollectiveBeginCallback(
    OTF2_SnapReaderCallbacks*                  snapReaderCallbacks,
    OTF2_SnapReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->mpi_collective_begin = mpiCollectiveBeginCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMpiCollectiveEndCallback(
    OTF2_SnapReaderCallbacks*                snapReaderCallbacks,
    OTF2_SnapReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->mpi_collective_end = mpiCollectiveEndCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetOmpForkCallback(
    OTF2_SnapReaderCallbacks*       snapReaderCallbacks,
    OTF2_SnapReaderCallback_OmpFork ompForkCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->omp_fork = ompForkCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetOmpAcquireLockCallback(
    OTF2_SnapReaderCallbacks*              snapReaderCallbacks,
    OTF2_SnapReaderCallback_OmpAcquireLock ompAcquireLockCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->omp_acquire_lock = ompAcquireLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetOmpTaskCreateCallback(
    OTF2_SnapReaderCallbacks*             snapReaderCallbacks,
    OTF2_SnapReaderCallback_OmpTaskCreate ompTaskCreateCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->omp_task_create = ompTaskCreateCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetOmpTaskSwitchCallback(
    OTF2_SnapReaderCallbacks*             snapReaderCallbacks,
    OTF2_SnapReaderCallback_OmpTaskSwitch ompTaskSwitchCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->omp_task_switch = ompTaskSwitchCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetMetricCallback(
    OTF2_SnapReaderCallbacks*      snapReaderCallbacks,
    OTF2_SnapReaderCallback_Metric metricCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->metric = metricCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetParameterStringCallback(
    OTF2_SnapReaderCallbacks*               snapReaderCallbacks,
    OTF2_SnapReaderCallback_ParameterString parameterStringCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->parameter_string = parameterStringCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetParameterIntCallback(
    OTF2_SnapReaderCallbacks*            snapReaderCallbacks,
    OTF2_SnapReaderCallback_ParameterInt parameterIntCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->parameter_int = parameterIntCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetParameterUnsignedIntCallback(
    OTF2_SnapReaderCallbacks*                    snapReaderCallbacks,
    OTF2_SnapReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback )
{
    if ( !snapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid SnapReaderCallbacks argument!" );
    }

    snapReaderCallbacks->parameter_unsigned_int = parameterUnsignedIntCallback;

    return OTF2_SUCCESS;
}
