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
 *  @file       src/OTF2_EvtReader_inc.c
 *  @source     templates/OTF2_EvtReader_inc.tmpl.c
 *
 *  @status     beta
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 */



static inline OTF2_ErrorCode
otf2_evt_reader_read_buffer_flush( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_BufferFlush* record = &reader->current_event.record.buffer_flush;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read BufferFlush record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    OTF2_Buffer_ReadUint64Full( reader->buffer, &record->stop_time );
    record->stop_time = otf2_evt_reader_apply_clock_correction( reader, record->stop_time );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.buffer_flush )
    {
        interrupt = reader->reader_callbacks.buffer_flush( reader->location_id,
                                                           record->time,
                                                           reader->global_event_position,
                                                           reader->user_data,
                                                           &reader->attribute_list,
                                                           record->stop_time );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_measurement_on_off( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MeasurementOnOff* record = &reader->current_event.record.measurement_on_off;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MeasurementOnOff record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    OTF2_Buffer_ReadUint8( reader->buffer, &record->measurement_mode );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                                 reader->global_event_position,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
                                                                 record->measurement_mode );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_enter( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_Enter* record = &reader->current_event.record.enter;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read Enter record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->region );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read region attribute of Enter record. Invalid compression size." );
    }
    record->region = otf2_evt_reader_map( reader, OTF2_MAPPING_REGION, record->region );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                    reader->global_event_position,
                                                    reader->user_data,
                                                    &reader->attribute_list,
                                                    record->region );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_leave( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_Leave* record = &reader->current_event.record.leave;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read Leave record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->region );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read region attribute of Leave record. Invalid compression size." );
    }
    record->region = otf2_evt_reader_map( reader, OTF2_MAPPING_REGION, record->region );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.leave )
    {
        interrupt = reader->reader_callbacks.leave( reader->location_id,
                                                    record->time,
                                                    reader->global_event_position,
                                                    reader->user_data,
                                                    &reader->attribute_list,
                                                    record->region );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_mpi_send( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiSend* record = &reader->current_event.record.mpi_send;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiSend record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


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
    record->communicator = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->communicator );
    ret                  = OTF2_Buffer_ReadUint32( reader->buffer, &record->msg_tag );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgTag attribute of MpiSend record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->msg_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgLength attribute of MpiSend record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                       reader->global_event_position,
                                                       reader->user_data,
                                                       &reader->attribute_list,
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
otf2_evt_reader_read_mpi_isend( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiIsend* record = &reader->current_event.record.mpi_isend;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiIsend record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


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
    record->communicator = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->communicator );
    ret                  = OTF2_Buffer_ReadUint32( reader->buffer, &record->msg_tag );
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

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                        reader->global_event_position,
                                                        reader->user_data,
                                                        &reader->attribute_list,
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
otf2_evt_reader_read_mpi_isend_complete( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiIsendComplete* record = &reader->current_event.record.mpi_isend_complete;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiIsendComplete record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->request_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read requestID attribute of MpiIsendComplete record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                                 reader->global_event_position,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
                                                                 record->request_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_mpi_irecv_request( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiIrecvRequest* record = &reader->current_event.record.mpi_irecv_request;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiIrecvRequest record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->request_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read requestID attribute of MpiIrecvRequest record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                                reader->global_event_position,
                                                                reader->user_data,
                                                                &reader->attribute_list,
                                                                record->request_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_mpi_recv( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiRecv* record = &reader->current_event.record.mpi_recv;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiRecv record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


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
    record->communicator = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->communicator );
    ret                  = OTF2_Buffer_ReadUint32( reader->buffer, &record->msg_tag );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgTag attribute of MpiRecv record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->msg_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read msgLength attribute of MpiRecv record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                       reader->global_event_position,
                                                       reader->user_data,
                                                       &reader->attribute_list,
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
otf2_evt_reader_read_mpi_irecv( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiIrecv* record = &reader->current_event.record.mpi_irecv;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiIrecv record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


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
    record->communicator = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->communicator );
    ret                  = OTF2_Buffer_ReadUint32( reader->buffer, &record->msg_tag );
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

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                        reader->global_event_position,
                                                        reader->user_data,
                                                        &reader->attribute_list,
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
otf2_evt_reader_read_mpi_request_test( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiRequestTest* record = &reader->current_event.record.mpi_request_test;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiRequestTest record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->request_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read requestID attribute of MpiRequestTest record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_request_test )
    {
        interrupt = reader->reader_callbacks.mpi_request_test( reader->location_id,
                                                               record->time,
                                                               reader->global_event_position,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->request_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_mpi_request_cancelled( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiRequestCancelled* record = &reader->current_event.record.mpi_request_cancelled;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiRequestCancelled record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->request_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read requestID attribute of MpiRequestCancelled record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_request_cancelled )
    {
        interrupt = reader->reader_callbacks.mpi_request_cancelled( reader->location_id,
                                                                    record->time,
                                                                    reader->global_event_position,
                                                                    reader->user_data,
                                                                    &reader->attribute_list,
                                                                    record->request_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_mpi_collective_begin( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiCollectiveBegin* record = &reader->current_event.record.mpi_collective_begin;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiCollectiveBegin record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;



    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                                   reader->global_event_position,
                                                                   reader->user_data,
                                                                   &reader->attribute_list  );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_mpi_collective_end( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_MpiCollectiveEnd* record = &reader->current_event.record.mpi_collective_end;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read MpiCollectiveEnd record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    OTF2_Buffer_ReadUint8( reader->buffer, &record->collective_op );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->communicator );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read communicator attribute of MpiCollectiveEnd record. Invalid compression size." );
    }
    record->communicator = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->communicator );
    ret                  = OTF2_Buffer_ReadUint32( reader->buffer, &record->root );
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

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                                 reader->global_event_position,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
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
otf2_evt_reader_read_omp_fork( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpFork* record = &reader->current_event.record.omp_fork;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpFork record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->number_of_requested_threads );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfRequestedThreads attribute of OmpFork record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                       reader->global_event_position,
                                                       reader->user_data,
                                                       &reader->attribute_list,
                                                       record->number_of_requested_threads );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_omp_join( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpJoin* record = &reader->current_event.record.omp_join;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpJoin record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;



    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.omp_join )
    {
        interrupt = reader->reader_callbacks.omp_join( reader->location_id,
                                                       record->time,
                                                       reader->global_event_position,
                                                       reader->user_data,
                                                       &reader->attribute_list  );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_omp_acquire_lock( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpAcquireLock* record = &reader->current_event.record.omp_acquire_lock;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpAcquireLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


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

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                               reader->global_event_position,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->lock_id,
                                                               record->acquisition_order );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_omp_release_lock( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpReleaseLock* record = &reader->current_event.record.omp_release_lock;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpReleaseLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->lock_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lockID attribute of OmpReleaseLock record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->acquisition_order );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read acquisitionOrder attribute of OmpReleaseLock record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.omp_release_lock )
    {
        interrupt = reader->reader_callbacks.omp_release_lock( reader->location_id,
                                                               record->time,
                                                               reader->global_event_position,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->lock_id,
                                                               record->acquisition_order );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_omp_task_create( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpTaskCreate* record = &reader->current_event.record.omp_task_create;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpTaskCreate record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->task_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read taskID attribute of OmpTaskCreate record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                              reader->global_event_position,
                                                              reader->user_data,
                                                              &reader->attribute_list,
                                                              record->task_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_omp_task_switch( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpTaskSwitch* record = &reader->current_event.record.omp_task_switch;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpTaskSwitch record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->task_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read taskID attribute of OmpTaskSwitch record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                              reader->global_event_position,
                                                              reader->user_data,
                                                              &reader->attribute_list,
                                                              record->task_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_omp_task_complete( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_OmpTaskComplete* record = &reader->current_event.record.omp_task_complete;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    /* Event is singleton (just one attribute of compressed data type) */
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read OmpTaskComplete record. Not enough memory in buffer" );
    }

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->task_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read taskID attribute of OmpTaskComplete record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.omp_task_complete )
    {
        interrupt = reader->reader_callbacks.omp_task_complete( reader->location_id,
                                                                record->time,
                                                                reader->global_event_position,
                                                                reader->user_data,
                                                                &reader->attribute_list,
                                                                record->task_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_metric( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_Metric* record = &reader->current_event.record.metric;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read Metric record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->metric );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read metric attribute of Metric record. Invalid compression size." );
    }
    record->metric = otf2_evt_reader_map( reader, OTF2_MAPPING_METRIC, record->metric );
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

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                     reader->global_event_position,
                                                     reader->user_data,
                                                     &reader->attribute_list,
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
otf2_evt_reader_read_parameter_string( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ParameterString* record = &reader->current_event.record.parameter_string;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ParameterString record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->parameter );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parameter attribute of ParameterString record. Invalid compression size." );
    }
    record->parameter = otf2_evt_reader_map( reader, OTF2_MAPPING_PARAMETER, record->parameter );
    ret               = OTF2_Buffer_ReadUint32( reader->buffer, &record->string );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read string attribute of ParameterString record. Invalid compression size." );
    }
    record->string = otf2_evt_reader_map( reader, OTF2_MAPPING_STRING, record->string );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                               reader->global_event_position,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->parameter,
                                                               record->string );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_parameter_int( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ParameterInt* record = &reader->current_event.record.parameter_int;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ParameterInt record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->parameter );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parameter attribute of ParameterInt record. Invalid compression size." );
    }
    record->parameter = otf2_evt_reader_map( reader, OTF2_MAPPING_PARAMETER, record->parameter );
    ret               = OTF2_Buffer_ReadInt64( reader->buffer, &record->value );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read value attribute of ParameterInt record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                            reader->global_event_position,
                                                            reader->user_data,
                                                            &reader->attribute_list,
                                                            record->parameter,
                                                            record->value );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_parameter_unsigned_int( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ParameterUnsignedInt* record = &reader->current_event.record.parameter_unsigned_int;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ParameterUnsignedInt record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->parameter );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parameter attribute of ParameterUnsignedInt record. Invalid compression size." );
    }
    record->parameter = otf2_evt_reader_map( reader, OTF2_MAPPING_PARAMETER, record->parameter );
    ret               = OTF2_Buffer_ReadUint64( reader->buffer, &record->value );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read value attribute of ParameterUnsignedInt record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
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
                                                                     reader->global_event_position,
                                                                     reader->user_data,
                                                                     &reader->attribute_list,
                                                                     record->parameter,
                                                                     record->value );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_win_create( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaWinCreate* record = &reader->current_event.record.rma_win_create;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaWinCreate record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaWinCreate record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_win_create )
    {
        interrupt = reader->reader_callbacks.rma_win_create( reader->location_id,
                                                             record->time,
                                                             reader->global_event_position,
                                                             reader->user_data,
                                                             &reader->attribute_list,
                                                             record->win );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_win_destroy( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaWinDestroy* record = &reader->current_event.record.rma_win_destroy;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaWinDestroy record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaWinDestroy record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_win_destroy )
    {
        interrupt = reader->reader_callbacks.rma_win_destroy( reader->location_id,
                                                              record->time,
                                                              reader->global_event_position,
                                                              reader->user_data,
                                                              &reader->attribute_list,
                                                              record->win );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_collective_begin( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaCollectiveBegin* record = &reader->current_event.record.rma_collective_begin;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaCollectiveBegin record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;



    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_collective_begin )
    {
        interrupt = reader->reader_callbacks.rma_collective_begin( reader->location_id,
                                                                   record->time,
                                                                   reader->global_event_position,
                                                                   reader->user_data,
                                                                   &reader->attribute_list  );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_collective_end( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaCollectiveEnd* record = &reader->current_event.record.rma_collective_end;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaCollectiveEnd record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    OTF2_Buffer_ReadUint8( reader->buffer, &record->collective_op );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->sync_level );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read syncLevel attribute of RmaCollectiveEnd record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaCollectiveEnd record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->root );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read root attribute of RmaCollectiveEnd record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->bytes_sent );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read bytesSent attribute of RmaCollectiveEnd record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->bytes_received );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read bytesReceived attribute of RmaCollectiveEnd record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_collective_end )
    {
        interrupt = reader->reader_callbacks.rma_collective_end( reader->location_id,
                                                                 record->time,
                                                                 reader->global_event_position,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
                                                                 record->collective_op,
                                                                 record->sync_level,
                                                                 record->win,
                                                                 record->root,
                                                                 record->bytes_sent,
                                                                 record->bytes_received );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_group_sync( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaGroupSync* record = &reader->current_event.record.rma_group_sync;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaGroupSync record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->sync_level );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read syncLevel attribute of RmaGroupSync record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaGroupSync record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->group );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read group attribute of RmaGroupSync record. Invalid compression size." );
    }
    record->group = otf2_evt_reader_map( reader, OTF2_MAPPING_GROUP, record->group );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_group_sync )
    {
        interrupt = reader->reader_callbacks.rma_group_sync( reader->location_id,
                                                             record->time,
                                                             reader->global_event_position,
                                                             reader->user_data,
                                                             &reader->attribute_list,
                                                             record->sync_level,
                                                             record->win,
                                                             record->group );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_request_lock( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaRequestLock* record = &reader->current_event.record.rma_request_lock;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaRequestLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaRequestLock record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->remote );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read remote attribute of RmaRequestLock record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->lock_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lockId attribute of RmaRequestLock record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record->lock_type );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_request_lock )
    {
        interrupt = reader->reader_callbacks.rma_request_lock( reader->location_id,
                                                               record->time,
                                                               reader->global_event_position,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->win,
                                                               record->remote,
                                                               record->lock_id,
                                                               record->lock_type );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_acquire_lock( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaAcquireLock* record = &reader->current_event.record.rma_acquire_lock;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaAcquireLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaAcquireLock record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->remote );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read remote attribute of RmaAcquireLock record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->lock_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lockId attribute of RmaAcquireLock record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record->lock_type );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_acquire_lock )
    {
        interrupt = reader->reader_callbacks.rma_acquire_lock( reader->location_id,
                                                               record->time,
                                                               reader->global_event_position,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->win,
                                                               record->remote,
                                                               record->lock_id,
                                                               record->lock_type );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_try_lock( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaTryLock* record = &reader->current_event.record.rma_try_lock;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaTryLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaTryLock record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->remote );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read remote attribute of RmaTryLock record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->lock_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lockId attribute of RmaTryLock record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record->lock_type );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_try_lock )
    {
        interrupt = reader->reader_callbacks.rma_try_lock( reader->location_id,
                                                           record->time,
                                                           reader->global_event_position,
                                                           reader->user_data,
                                                           &reader->attribute_list,
                                                           record->win,
                                                           record->remote,
                                                           record->lock_id,
                                                           record->lock_type );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_release_lock( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaReleaseLock* record = &reader->current_event.record.rma_release_lock;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaReleaseLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaReleaseLock record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->remote );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read remote attribute of RmaReleaseLock record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->lock_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lockId attribute of RmaReleaseLock record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_release_lock )
    {
        interrupt = reader->reader_callbacks.rma_release_lock( reader->location_id,
                                                               record->time,
                                                               reader->global_event_position,
                                                               reader->user_data,
                                                               &reader->attribute_list,
                                                               record->win,
                                                               record->remote,
                                                               record->lock_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_sync( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaSync* record = &reader->current_event.record.rma_sync;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaSync record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaSync record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->remote );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read remote attribute of RmaSync record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record->sync_type );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_sync )
    {
        interrupt = reader->reader_callbacks.rma_sync( reader->location_id,
                                                       record->time,
                                                       reader->global_event_position,
                                                       reader->user_data,
                                                       &reader->attribute_list,
                                                       record->win,
                                                       record->remote,
                                                       record->sync_type );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_wait_change( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaWaitChange* record = &reader->current_event.record.rma_wait_change;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaWaitChange record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaWaitChange record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_wait_change )
    {
        interrupt = reader->reader_callbacks.rma_wait_change( reader->location_id,
                                                              record->time,
                                                              reader->global_event_position,
                                                              reader->user_data,
                                                              &reader->attribute_list,
                                                              record->win );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_put( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaPut* record = &reader->current_event.record.rma_put;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaPut record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaPut record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->remote );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read remote attribute of RmaPut record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->bytes );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read bytes attribute of RmaPut record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->matching_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read matchingId attribute of RmaPut record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_put )
    {
        interrupt = reader->reader_callbacks.rma_put( reader->location_id,
                                                      record->time,
                                                      reader->global_event_position,
                                                      reader->user_data,
                                                      &reader->attribute_list,
                                                      record->win,
                                                      record->remote,
                                                      record->bytes,
                                                      record->matching_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_get( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaGet* record = &reader->current_event.record.rma_get;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaGet record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaGet record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->remote );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read remote attribute of RmaGet record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->bytes );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read bytes attribute of RmaGet record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->matching_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read matchingId attribute of RmaGet record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_get )
    {
        interrupt = reader->reader_callbacks.rma_get( reader->location_id,
                                                      record->time,
                                                      reader->global_event_position,
                                                      reader->user_data,
                                                      &reader->attribute_list,
                                                      record->win,
                                                      record->remote,
                                                      record->bytes,
                                                      record->matching_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_atomic( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaAtomic* record = &reader->current_event.record.rma_atomic;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaAtomic record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaAtomic record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint32( reader->buffer, &record->remote );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read remote attribute of RmaAtomic record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record->type );
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->bytes_sent );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read bytesSent attribute of RmaAtomic record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->bytes_received );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read bytesReceived attribute of RmaAtomic record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &record->matching_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read matchingId attribute of RmaAtomic record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_atomic )
    {
        interrupt = reader->reader_callbacks.rma_atomic( reader->location_id,
                                                         record->time,
                                                         reader->global_event_position,
                                                         reader->user_data,
                                                         &reader->attribute_list,
                                                         record->win,
                                                         record->remote,
                                                         record->type,
                                                         record->bytes_sent,
                                                         record->bytes_received,
                                                         record->matching_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_op_complete_blocking( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaOpCompleteBlocking* record = &reader->current_event.record.rma_op_complete_blocking;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaOpCompleteBlocking record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaOpCompleteBlocking record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint64( reader->buffer, &record->matching_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read matchingId attribute of RmaOpCompleteBlocking record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_op_complete_blocking )
    {
        interrupt = reader->reader_callbacks.rma_op_complete_blocking( reader->location_id,
                                                                       record->time,
                                                                       reader->global_event_position,
                                                                       reader->user_data,
                                                                       &reader->attribute_list,
                                                                       record->win,
                                                                       record->matching_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_op_complete_non_blocking( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaOpCompleteNonBlocking* record = &reader->current_event.record.rma_op_complete_non_blocking;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaOpCompleteNonBlocking record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaOpCompleteNonBlocking record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint64( reader->buffer, &record->matching_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read matchingId attribute of RmaOpCompleteNonBlocking record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_op_complete_non_blocking )
    {
        interrupt = reader->reader_callbacks.rma_op_complete_non_blocking( reader->location_id,
                                                                           record->time,
                                                                           reader->global_event_position,
                                                                           reader->user_data,
                                                                           &reader->attribute_list,
                                                                           record->win,
                                                                           record->matching_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_op_test( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaOpTest* record = &reader->current_event.record.rma_op_test;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaOpTest record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaOpTest record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint64( reader->buffer, &record->matching_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read matchingId attribute of RmaOpTest record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_op_test )
    {
        interrupt = reader->reader_callbacks.rma_op_test( reader->location_id,
                                                          record->time,
                                                          reader->global_event_position,
                                                          reader->user_data,
                                                          &reader->attribute_list,
                                                          record->win,
                                                          record->matching_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_rma_op_complete_remote( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_RmaOpCompleteRemote* record = &reader->current_event.record.rma_op_complete_remote;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read RmaOpCompleteRemote record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->win );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read win attribute of RmaOpCompleteRemote record. Invalid compression size." );
    }
    record->win = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, record->win );
    ret         = OTF2_Buffer_ReadUint64( reader->buffer, &record->matching_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read matchingId attribute of RmaOpCompleteRemote record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_op_complete_remote )
    {
        interrupt = reader->reader_callbacks.rma_op_complete_remote( reader->location_id,
                                                                     record->time,
                                                                     reader->global_event_position,
                                                                     reader->user_data,
                                                                     &reader->attribute_list,
                                                                     record->win,
                                                                     record->matching_id );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_fork( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadFork* record = &reader->current_event.record.thread_fork;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadFork record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    OTF2_Buffer_ReadUint8( reader->buffer, &record->model );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->number_of_requested_threads );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfRequestedThreads attribute of ThreadFork record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_fork )
    {
        interrupt = reader->reader_callbacks.thread_fork( reader->location_id,
                                                          record->time,
                                                          reader->global_event_position,
                                                          reader->user_data,
                                                          &reader->attribute_list,
                                                          record->model,
                                                          record->number_of_requested_threads );
    }
    else if ( reader->reader_callbacks.omp_fork )
    {
        OTF2_OmpFork omp_fork_record;
        bool         conversion_successful
            = otf2_event_thread_fork_convert_to_omp_fork( record,
                                                          &reader->attribute_list,
                                                          &omp_fork_record );
        if ( conversion_successful )
        {
            interrupt = reader->reader_callbacks.omp_fork( reader->location_id,
                                                           record->time,
                                                           reader->global_event_position,
                                                           reader->user_data,
                                                           &reader->attribute_list,
                                                           omp_fork_record.number_of_requested_threads );
        }
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_join( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadJoin* record = &reader->current_event.record.thread_join;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadJoin record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    OTF2_Buffer_ReadUint8( reader->buffer, &record->model );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_join )
    {
        interrupt = reader->reader_callbacks.thread_join( reader->location_id,
                                                          record->time,
                                                          reader->global_event_position,
                                                          reader->user_data,
                                                          &reader->attribute_list,
                                                          record->model );
    }
    else if ( reader->reader_callbacks.omp_join )
    {
        OTF2_OmpJoin omp_join_record;
        bool         conversion_successful
            = otf2_event_thread_join_convert_to_omp_join( record,
                                                          &reader->attribute_list,
                                                          &omp_join_record );
        if ( conversion_successful )
        {
            interrupt = reader->reader_callbacks.omp_join( reader->location_id,
                                                           record->time,
                                                           reader->global_event_position,
                                                           reader->user_data,
                                                           &reader->attribute_list  );
        }
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_team_begin( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadTeamBegin* record = &reader->current_event.record.thread_team_begin;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadTeamBegin record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->thread_team );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read threadTeam attribute of ThreadTeamBegin record. Invalid compression size." );
    }
    record->thread_team = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->thread_team );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_team_begin )
    {
        interrupt = reader->reader_callbacks.thread_team_begin( reader->location_id,
                                                                record->time,
                                                                reader->global_event_position,
                                                                reader->user_data,
                                                                &reader->attribute_list,
                                                                record->thread_team );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_team_end( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadTeamEnd* record = &reader->current_event.record.thread_team_end;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadTeamEnd record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->thread_team );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read threadTeam attribute of ThreadTeamEnd record. Invalid compression size." );
    }
    record->thread_team = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->thread_team );

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_team_end )
    {
        interrupt = reader->reader_callbacks.thread_team_end( reader->location_id,
                                                              record->time,
                                                              reader->global_event_position,
                                                              reader->user_data,
                                                              &reader->attribute_list,
                                                              record->thread_team );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_acquire_lock( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadAcquireLock* record = &reader->current_event.record.thread_acquire_lock;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadAcquireLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    OTF2_Buffer_ReadUint8( reader->buffer, &record->model );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->lock_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lockID attribute of ThreadAcquireLock record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->acquisition_order );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read acquisitionOrder attribute of ThreadAcquireLock record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_acquire_lock )
    {
        interrupt = reader->reader_callbacks.thread_acquire_lock( reader->location_id,
                                                                  record->time,
                                                                  reader->global_event_position,
                                                                  reader->user_data,
                                                                  &reader->attribute_list,
                                                                  record->model,
                                                                  record->lock_id,
                                                                  record->acquisition_order );
    }
    else if ( reader->reader_callbacks.omp_acquire_lock )
    {
        OTF2_OmpAcquireLock omp_acquire_lock_record;
        bool                conversion_successful
            = otf2_event_thread_acquire_lock_convert_to_omp_acquire_lock( record,
                                                                          &reader->attribute_list,
                                                                          &omp_acquire_lock_record );
        if ( conversion_successful )
        {
            interrupt = reader->reader_callbacks.omp_acquire_lock( reader->location_id,
                                                                   record->time,
                                                                   reader->global_event_position,
                                                                   reader->user_data,
                                                                   &reader->attribute_list,
                                                                   omp_acquire_lock_record.lock_id,
                                                                   omp_acquire_lock_record.acquisition_order );
        }
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_release_lock( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadReleaseLock* record = &reader->current_event.record.thread_release_lock;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadReleaseLock record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    OTF2_Buffer_ReadUint8( reader->buffer, &record->model );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->lock_id );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lockID attribute of ThreadReleaseLock record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->acquisition_order );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read acquisitionOrder attribute of ThreadReleaseLock record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_release_lock )
    {
        interrupt = reader->reader_callbacks.thread_release_lock( reader->location_id,
                                                                  record->time,
                                                                  reader->global_event_position,
                                                                  reader->user_data,
                                                                  &reader->attribute_list,
                                                                  record->model,
                                                                  record->lock_id,
                                                                  record->acquisition_order );
    }
    else if ( reader->reader_callbacks.omp_release_lock )
    {
        OTF2_OmpReleaseLock omp_release_lock_record;
        bool                conversion_successful
            = otf2_event_thread_release_lock_convert_to_omp_release_lock( record,
                                                                          &reader->attribute_list,
                                                                          &omp_release_lock_record );
        if ( conversion_successful )
        {
            interrupt = reader->reader_callbacks.omp_release_lock( reader->location_id,
                                                                   record->time,
                                                                   reader->global_event_position,
                                                                   reader->user_data,
                                                                   &reader->attribute_list,
                                                                   omp_release_lock_record.lock_id,
                                                                   omp_release_lock_record.acquisition_order );
        }
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_task_create( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadTaskCreate* record = &reader->current_event.record.thread_task_create;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadTaskCreate record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->thread_team );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read threadTeam attribute of ThreadTaskCreate record. Invalid compression size." );
    }
    record->thread_team = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->thread_team );
    ret                 = OTF2_Buffer_ReadUint32( reader->buffer, &record->creating_thread );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read creatingThread attribute of ThreadTaskCreate record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->generation_number );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read generationNumber attribute of ThreadTaskCreate record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_task_create )
    {
        interrupt = reader->reader_callbacks.thread_task_create( reader->location_id,
                                                                 record->time,
                                                                 reader->global_event_position,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
                                                                 record->thread_team,
                                                                 record->creating_thread,
                                                                 record->generation_number );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_task_switch( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadTaskSwitch* record = &reader->current_event.record.thread_task_switch;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadTaskSwitch record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->thread_team );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read threadTeam attribute of ThreadTaskSwitch record. Invalid compression size." );
    }
    record->thread_team = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->thread_team );
    ret                 = OTF2_Buffer_ReadUint32( reader->buffer, &record->creating_thread );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read creatingThread attribute of ThreadTaskSwitch record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->generation_number );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read generationNumber attribute of ThreadTaskSwitch record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_task_switch )
    {
        interrupt = reader->reader_callbacks.thread_task_switch( reader->location_id,
                                                                 record->time,
                                                                 reader->global_event_position,
                                                                 reader->user_data,
                                                                 &reader->attribute_list,
                                                                 record->thread_team,
                                                                 record->creating_thread,
                                                                 record->generation_number );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_thread_task_complete( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_ThreadTaskComplete* record = &reader->current_event.record.thread_task_complete;

    record->time = otf2_evt_reader_apply_clock_correction( reader, record->time );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Event contains separate record length information */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read ThreadTaskComplete record. Not enough memory in buffer" );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;


    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->thread_team );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read threadTeam attribute of ThreadTaskComplete record. Invalid compression size." );
    }
    record->thread_team = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, record->thread_team );
    ret                 = OTF2_Buffer_ReadUint32( reader->buffer, &record->creating_thread );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read creatingThread attribute of ThreadTaskComplete record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &record->generation_number );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read generationNumber attribute of ThreadTaskComplete record. Invalid compression size." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

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
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.thread_task_complete )
    {
        interrupt = reader->reader_callbacks.thread_task_complete( reader->location_id,
                                                                   record->time,
                                                                   reader->global_event_position,
                                                                   reader->user_data,
                                                                   &reader->attribute_list,
                                                                   record->thread_team,
                                                                   record->creating_thread,
                                                                   record->generation_number );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_evt_reader_read_attribute_list( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode status;

    /* Get record length and test memory availability */
    uint64_t record_data_length;
    status = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return UTILS_ERROR( status,
                            "Could not read attribute list. Not enough memory in buffer." );
    }
    uint8_t* record_start_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_start_pos );

    /* Get how many attributes are in the list */
    uint32_t number_of_attributes;
    status = OTF2_Buffer_ReadUint32( reader->buffer, &number_of_attributes );
    if ( OTF2_SUCCESS != status )
    {
        return UTILS_ERROR( status,
                            "Could not read number of attributes in list. Invalid compression size." );
    }

    /* Check if number of attributes is consistent with record length */
    if ( record_data_length > number_of_attributes * OTF2_MAXIMUM_ATTRIBUTE_LIST_ENTRY_SIZE + 5 )
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
            return UTILS_ERROR( status,
                                "Could not read attribute from list. Invalid compression size." );
        }
        attribute_id = otf2_evt_reader_map( reader, OTF2_MAPPING_ATTRIBUTE, attribute_id );

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
                    return UTILS_ERROR( status,
                                        "Could not read attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_UINT64:
                status = OTF2_Buffer_ReadUint64( reader->buffer,
                                                 &value.uint64 );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read attribute. Invalid compression size." );
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
                    return UTILS_ERROR( status,
                                        "Could not read attribute. Invalid compression size." );
                }
                break;

            case OTF2_TYPE_INT64:
                status = OTF2_Buffer_ReadInt64( reader->buffer,
                                                &value.int64 );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read attribute. Invalid compression size." );
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
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.stringRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read String attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.stringRef = otf2_evt_reader_map( reader, OTF2_MAPPING_STRING, value.stringRef );
                break;

            case OTF2_TYPE_ATTRIBUTE:
                /* OTF2_TYPE_ATTRIBUTE attributes reference a Attribute definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.attributeRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read Attribute attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.attributeRef = otf2_evt_reader_map( reader, OTF2_MAPPING_ATTRIBUTE, value.attributeRef );
                break;

            case OTF2_TYPE_LOCATION:
                /* OTF2_TYPE_LOCATION attributes reference a Location definition */
                status = OTF2_Buffer_ReadUint64( reader->buffer,
                                                 &value.locationRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read Location attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.locationRef = otf2_evt_reader_map( reader, OTF2_MAPPING_LOCATION, value.locationRef );
                break;

            case OTF2_TYPE_REGION:
                /* OTF2_TYPE_REGION attributes reference a Region definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.regionRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read Region attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.regionRef = otf2_evt_reader_map( reader, OTF2_MAPPING_REGION, value.regionRef );
                break;

            case OTF2_TYPE_GROUP:
                /* OTF2_TYPE_GROUP attributes reference a Group definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.groupRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read Group attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.groupRef = otf2_evt_reader_map( reader, OTF2_MAPPING_GROUP, value.groupRef );
                break;

            case OTF2_TYPE_METRIC:
                /* OTF2_TYPE_METRIC attributes reference a MetricClass, or a MetricInstance definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.metricRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read MetricClass attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.metricRef = otf2_evt_reader_map( reader, OTF2_MAPPING_METRIC, value.metricRef );
                break;

            case OTF2_TYPE_COMM:
                /* OTF2_TYPE_COMM attributes reference a Comm definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.commRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read Comm attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.commRef = otf2_evt_reader_map( reader, OTF2_MAPPING_COMM, value.commRef );
                break;

            case OTF2_TYPE_PARAMETER:
                /* OTF2_TYPE_PARAMETER attributes reference a Parameter definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.parameterRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read Parameter attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.parameterRef = otf2_evt_reader_map( reader, OTF2_MAPPING_PARAMETER, value.parameterRef );
                break;

            case OTF2_TYPE_RMA_WIN:
                /* OTF2_TYPE_RMA_WIN attributes reference a RmaWin definition */
                status = OTF2_Buffer_ReadUint32( reader->buffer,
                                                 &value.rmaWinRef );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read RmaWin attribute. Invalid compression size." );
                }
                /* But we need to map them to the global definition */
                value.rmaWinRef = otf2_evt_reader_map( reader, OTF2_MAPPING_RMA_WIN, value.rmaWinRef );
                break;

            default:
                /*
                 * There can only be more definition references added in the future,
                 * and definition references are guaranteed to be of compressable
                 * size, thus we induce that the value is of compressed type
                 */
                status = OTF2_Buffer_ReadUint64( reader->buffer,
                                                 &value.uint64 );
                if ( OTF2_SUCCESS != status )
                {
                    return UTILS_ERROR( status,
                                        "Could not read attribute of unknown type. Invalid compression size." );
                }
                break;
        }

        /* Store the new value in the attribute list. */
        OTF2_AttributeList_AddAttribute( &reader->attribute_list,
                                         attribute_id,
                                         type,
                                         value );
    }

    /*
     * Always jump to the announced end of the record.
     */
    status = OTF2_Buffer_SetPosition( reader->buffer,
                                      record_start_pos + record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return UTILS_ERROR( status, "Could not read attribute list." );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_evt_reader_read( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode     status = OTF2_ERROR_INVALID;
    OTF2_GenericEvent* event  = &reader->current_event;

    status = OTF2_Buffer_ReadTimeStamp( reader->buffer, &event->record.time );
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
    OTF2_Buffer_ReadUint8( reader->buffer, &event->type );

    switch ( event->type )
    {
        case OTF2_BUFFER_END_OF_CHUNK:
            reader->chunk_local_event_position = 0;
            free( reader->position_table );
            reader->position_table = NULL;
            free( reader->timestamp_table );
            reader->timestamp_table = NULL;
            status                  = OTF2_Buffer_ReadGetNextChunk( reader->buffer );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status, "Load of next chunk failed!" );
            }
            return otf2_evt_reader_read( reader );

        case OTF2_BUFFER_END_OF_FILE:
            return OTF2_ERROR_INDEX_OUT_OF_BOUNDS;

        case OTF2_ATTRIBUTE_LIST:
            status = otf2_evt_reader_read_attribute_list( reader );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status, "Read of additional attributes failed!" );
            }
            return otf2_evt_reader_read( reader );

        case OTF2_EVENT_BUFFER_FLUSH:
            return otf2_evt_reader_read_buffer_flush( reader );

        case OTF2_EVENT_MEASUREMENT_ON_OFF:
            return otf2_evt_reader_read_measurement_on_off( reader );

        case OTF2_EVENT_ENTER:
            return otf2_evt_reader_read_enter( reader );

        case OTF2_EVENT_LEAVE:
            return otf2_evt_reader_read_leave( reader );

        case OTF2_EVENT_MPI_SEND:
            return otf2_evt_reader_read_mpi_send( reader );

        case OTF2_EVENT_MPI_ISEND:
            return otf2_evt_reader_read_mpi_isend( reader );

        case OTF2_EVENT_MPI_ISEND_COMPLETE:
            return otf2_evt_reader_read_mpi_isend_complete( reader );

        case OTF2_EVENT_MPI_IRECV_REQUEST:
            return otf2_evt_reader_read_mpi_irecv_request( reader );

        case OTF2_EVENT_MPI_RECV:
            return otf2_evt_reader_read_mpi_recv( reader );

        case OTF2_EVENT_MPI_IRECV:
            return otf2_evt_reader_read_mpi_irecv( reader );

        case OTF2_EVENT_MPI_REQUEST_TEST:
            return otf2_evt_reader_read_mpi_request_test( reader );

        case OTF2_EVENT_MPI_REQUEST_CANCELLED:
            return otf2_evt_reader_read_mpi_request_cancelled( reader );

        case OTF2_EVENT_MPI_COLLECTIVE_BEGIN:
            return otf2_evt_reader_read_mpi_collective_begin( reader );

        case OTF2_EVENT_MPI_COLLECTIVE_END:
            return otf2_evt_reader_read_mpi_collective_end( reader );

        case OTF2_EVENT_OMP_FORK:
            return otf2_evt_reader_read_omp_fork( reader );

        case OTF2_EVENT_OMP_JOIN:
            return otf2_evt_reader_read_omp_join( reader );

        case OTF2_EVENT_OMP_ACQUIRE_LOCK:
            return otf2_evt_reader_read_omp_acquire_lock( reader );

        case OTF2_EVENT_OMP_RELEASE_LOCK:
            return otf2_evt_reader_read_omp_release_lock( reader );

        case OTF2_EVENT_OMP_TASK_CREATE:
            return otf2_evt_reader_read_omp_task_create( reader );

        case OTF2_EVENT_OMP_TASK_SWITCH:
            return otf2_evt_reader_read_omp_task_switch( reader );

        case OTF2_EVENT_OMP_TASK_COMPLETE:
            return otf2_evt_reader_read_omp_task_complete( reader );

        case OTF2_EVENT_METRIC:
            return otf2_evt_reader_read_metric( reader );

        case OTF2_EVENT_PARAMETER_STRING:
            return otf2_evt_reader_read_parameter_string( reader );

        case OTF2_EVENT_PARAMETER_INT:
            return otf2_evt_reader_read_parameter_int( reader );

        case OTF2_EVENT_PARAMETER_UNSIGNED_INT:
            return otf2_evt_reader_read_parameter_unsigned_int( reader );

        case OTF2_EVENT_RMA_WIN_CREATE:
            return otf2_evt_reader_read_rma_win_create( reader );

        case OTF2_EVENT_RMA_WIN_DESTROY:
            return otf2_evt_reader_read_rma_win_destroy( reader );

        case OTF2_EVENT_RMA_COLLECTIVE_BEGIN:
            return otf2_evt_reader_read_rma_collective_begin( reader );

        case OTF2_EVENT_RMA_COLLECTIVE_END:
            return otf2_evt_reader_read_rma_collective_end( reader );

        case OTF2_EVENT_RMA_GROUP_SYNC:
            return otf2_evt_reader_read_rma_group_sync( reader );

        case OTF2_EVENT_RMA_REQUEST_LOCK:
            return otf2_evt_reader_read_rma_request_lock( reader );

        case OTF2_EVENT_RMA_ACQUIRE_LOCK:
            return otf2_evt_reader_read_rma_acquire_lock( reader );

        case OTF2_EVENT_RMA_TRY_LOCK:
            return otf2_evt_reader_read_rma_try_lock( reader );

        case OTF2_EVENT_RMA_RELEASE_LOCK:
            return otf2_evt_reader_read_rma_release_lock( reader );

        case OTF2_EVENT_RMA_SYNC:
            return otf2_evt_reader_read_rma_sync( reader );

        case OTF2_EVENT_RMA_WAIT_CHANGE:
            return otf2_evt_reader_read_rma_wait_change( reader );

        case OTF2_EVENT_RMA_PUT:
            return otf2_evt_reader_read_rma_put( reader );

        case OTF2_EVENT_RMA_GET:
            return otf2_evt_reader_read_rma_get( reader );

        case OTF2_EVENT_RMA_ATOMIC:
            return otf2_evt_reader_read_rma_atomic( reader );

        case OTF2_EVENT_RMA_OP_COMPLETE_BLOCKING:
            return otf2_evt_reader_read_rma_op_complete_blocking( reader );

        case OTF2_EVENT_RMA_OP_COMPLETE_NON_BLOCKING:
            return otf2_evt_reader_read_rma_op_complete_non_blocking( reader );

        case OTF2_EVENT_RMA_OP_TEST:
            return otf2_evt_reader_read_rma_op_test( reader );

        case OTF2_EVENT_RMA_OP_COMPLETE_REMOTE:
            return otf2_evt_reader_read_rma_op_complete_remote( reader );

        case OTF2_EVENT_THREAD_FORK:
            return otf2_evt_reader_read_thread_fork( reader );

        case OTF2_EVENT_THREAD_JOIN:
            return otf2_evt_reader_read_thread_join( reader );

        case OTF2_EVENT_THREAD_TEAM_BEGIN:
            return otf2_evt_reader_read_thread_team_begin( reader );

        case OTF2_EVENT_THREAD_TEAM_END:
            return otf2_evt_reader_read_thread_team_end( reader );

        case OTF2_EVENT_THREAD_ACQUIRE_LOCK:
            return otf2_evt_reader_read_thread_acquire_lock( reader );

        case OTF2_EVENT_THREAD_RELEASE_LOCK:
            return otf2_evt_reader_read_thread_release_lock( reader );

        case OTF2_EVENT_THREAD_TASK_CREATE:
            return otf2_evt_reader_read_thread_task_create( reader );

        case OTF2_EVENT_THREAD_TASK_SWITCH:
            return otf2_evt_reader_read_thread_task_switch( reader );

        case OTF2_EVENT_THREAD_TASK_COMPLETE:
            return otf2_evt_reader_read_thread_task_complete( reader );

        default:
            return otf2_evt_reader_read_unknown( reader );
    }
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetBufferFlushCallback(
    OTF2_EvtReaderCallbacks*           evtReaderCallbacks,
    OTF2_EvtReaderCallback_BufferFlush bufferFlushCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->buffer_flush = bufferFlushCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMeasurementOnOffCallback(
    OTF2_EvtReaderCallbacks*                evtReaderCallbacks,
    OTF2_EvtReaderCallback_MeasurementOnOff measurementOnOffCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->measurement_on_off = measurementOnOffCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetEnterCallback(
    OTF2_EvtReaderCallbacks*     evtReaderCallbacks,
    OTF2_EvtReaderCallback_Enter enterCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->enter = enterCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetLeaveCallback(
    OTF2_EvtReaderCallbacks*     evtReaderCallbacks,
    OTF2_EvtReaderCallback_Leave leaveCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->leave = leaveCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiSendCallback(
    OTF2_EvtReaderCallbacks*       evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiSend mpiSendCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_send = mpiSendCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiIsendCallback(
    OTF2_EvtReaderCallbacks*        evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiIsend mpiIsendCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_isend = mpiIsendCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiIsendCompleteCallback(
    OTF2_EvtReaderCallbacks*                evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiIsendComplete mpiIsendCompleteCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_isend_complete = mpiIsendCompleteCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiIrecvRequestCallback(
    OTF2_EvtReaderCallbacks*               evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_irecv_request = mpiIrecvRequestCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiRecvCallback(
    OTF2_EvtReaderCallbacks*       evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiRecv mpiRecvCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_recv = mpiRecvCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiIrecvCallback(
    OTF2_EvtReaderCallbacks*        evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiIrecv mpiIrecvCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_irecv = mpiIrecvCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiRequestTestCallback(
    OTF2_EvtReaderCallbacks*              evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiRequestTest mpiRequestTestCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_request_test = mpiRequestTestCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiRequestCancelledCallback(
    OTF2_EvtReaderCallbacks*                   evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiRequestCancelled mpiRequestCancelledCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_request_cancelled = mpiRequestCancelledCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiCollectiveBeginCallback(
    OTF2_EvtReaderCallbacks*                  evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_collective_begin = mpiCollectiveBeginCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMpiCollectiveEndCallback(
    OTF2_EvtReaderCallbacks*                evtReaderCallbacks,
    OTF2_EvtReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->mpi_collective_end = mpiCollectiveEndCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetOmpForkCallback(
    OTF2_EvtReaderCallbacks*       evtReaderCallbacks,
    OTF2_EvtReaderCallback_OmpFork ompForkCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->omp_fork = ompForkCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetOmpJoinCallback(
    OTF2_EvtReaderCallbacks*       evtReaderCallbacks,
    OTF2_EvtReaderCallback_OmpJoin ompJoinCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->omp_join = ompJoinCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetOmpAcquireLockCallback(
    OTF2_EvtReaderCallbacks*              evtReaderCallbacks,
    OTF2_EvtReaderCallback_OmpAcquireLock ompAcquireLockCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->omp_acquire_lock = ompAcquireLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetOmpReleaseLockCallback(
    OTF2_EvtReaderCallbacks*              evtReaderCallbacks,
    OTF2_EvtReaderCallback_OmpReleaseLock ompReleaseLockCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->omp_release_lock = ompReleaseLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetOmpTaskCreateCallback(
    OTF2_EvtReaderCallbacks*             evtReaderCallbacks,
    OTF2_EvtReaderCallback_OmpTaskCreate ompTaskCreateCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->omp_task_create = ompTaskCreateCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetOmpTaskSwitchCallback(
    OTF2_EvtReaderCallbacks*             evtReaderCallbacks,
    OTF2_EvtReaderCallback_OmpTaskSwitch ompTaskSwitchCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->omp_task_switch = ompTaskSwitchCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetOmpTaskCompleteCallback(
    OTF2_EvtReaderCallbacks*               evtReaderCallbacks,
    OTF2_EvtReaderCallback_OmpTaskComplete ompTaskCompleteCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->omp_task_complete = ompTaskCompleteCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetMetricCallback(
    OTF2_EvtReaderCallbacks*      evtReaderCallbacks,
    OTF2_EvtReaderCallback_Metric metricCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->metric = metricCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetParameterStringCallback(
    OTF2_EvtReaderCallbacks*               evtReaderCallbacks,
    OTF2_EvtReaderCallback_ParameterString parameterStringCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->parameter_string = parameterStringCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetParameterIntCallback(
    OTF2_EvtReaderCallbacks*            evtReaderCallbacks,
    OTF2_EvtReaderCallback_ParameterInt parameterIntCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->parameter_int = parameterIntCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetParameterUnsignedIntCallback(
    OTF2_EvtReaderCallbacks*                    evtReaderCallbacks,
    OTF2_EvtReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->parameter_unsigned_int = parameterUnsignedIntCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaWinCreateCallback(
    OTF2_EvtReaderCallbacks*            evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaWinCreate rmaWinCreateCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_win_create = rmaWinCreateCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaWinDestroyCallback(
    OTF2_EvtReaderCallbacks*             evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaWinDestroy rmaWinDestroyCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_win_destroy = rmaWinDestroyCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaCollectiveBeginCallback(
    OTF2_EvtReaderCallbacks*                  evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaCollectiveBegin rmaCollectiveBeginCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_collective_begin = rmaCollectiveBeginCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaCollectiveEndCallback(
    OTF2_EvtReaderCallbacks*                evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaCollectiveEnd rmaCollectiveEndCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_collective_end = rmaCollectiveEndCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaGroupSyncCallback(
    OTF2_EvtReaderCallbacks*            evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaGroupSync rmaGroupSyncCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_group_sync = rmaGroupSyncCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaRequestLockCallback(
    OTF2_EvtReaderCallbacks*              evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaRequestLock rmaRequestLockCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_request_lock = rmaRequestLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaAcquireLockCallback(
    OTF2_EvtReaderCallbacks*              evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaAcquireLock rmaAcquireLockCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_acquire_lock = rmaAcquireLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaTryLockCallback(
    OTF2_EvtReaderCallbacks*          evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaTryLock rmaTryLockCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_try_lock = rmaTryLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaReleaseLockCallback(
    OTF2_EvtReaderCallbacks*              evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaReleaseLock rmaReleaseLockCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_release_lock = rmaReleaseLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaSyncCallback(
    OTF2_EvtReaderCallbacks*       evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaSync rmaSyncCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_sync = rmaSyncCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaWaitChangeCallback(
    OTF2_EvtReaderCallbacks*             evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaWaitChange rmaWaitChangeCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_wait_change = rmaWaitChangeCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaPutCallback(
    OTF2_EvtReaderCallbacks*      evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaPut rmaPutCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_put = rmaPutCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaGetCallback(
    OTF2_EvtReaderCallbacks*      evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaGet rmaGetCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_get = rmaGetCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaAtomicCallback(
    OTF2_EvtReaderCallbacks*         evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaAtomic rmaAtomicCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_atomic = rmaAtomicCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaOpCompleteBlockingCallback(
    OTF2_EvtReaderCallbacks*                     evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaOpCompleteBlocking rmaOpCompleteBlockingCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_op_complete_blocking = rmaOpCompleteBlockingCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaOpCompleteNonBlockingCallback(
    OTF2_EvtReaderCallbacks*                        evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaOpCompleteNonBlocking rmaOpCompleteNonBlockingCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_op_complete_non_blocking = rmaOpCompleteNonBlockingCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaOpTestCallback(
    OTF2_EvtReaderCallbacks*         evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaOpTest rmaOpTestCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_op_test = rmaOpTestCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetRmaOpCompleteRemoteCallback(
    OTF2_EvtReaderCallbacks*                   evtReaderCallbacks,
    OTF2_EvtReaderCallback_RmaOpCompleteRemote rmaOpCompleteRemoteCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->rma_op_complete_remote = rmaOpCompleteRemoteCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadForkCallback(
    OTF2_EvtReaderCallbacks*          evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadFork threadForkCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_fork = threadForkCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadJoinCallback(
    OTF2_EvtReaderCallbacks*          evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadJoin threadJoinCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_join = threadJoinCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadTeamBeginCallback(
    OTF2_EvtReaderCallbacks*               evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadTeamBegin threadTeamBeginCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_team_begin = threadTeamBeginCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadTeamEndCallback(
    OTF2_EvtReaderCallbacks*             evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadTeamEnd threadTeamEndCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_team_end = threadTeamEndCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadAcquireLockCallback(
    OTF2_EvtReaderCallbacks*                 evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadAcquireLock threadAcquireLockCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_acquire_lock = threadAcquireLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadReleaseLockCallback(
    OTF2_EvtReaderCallbacks*                 evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadReleaseLock threadReleaseLockCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_release_lock = threadReleaseLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadTaskCreateCallback(
    OTF2_EvtReaderCallbacks*                evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadTaskCreate threadTaskCreateCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_task_create = threadTaskCreateCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadTaskSwitchCallback(
    OTF2_EvtReaderCallbacks*                evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadTaskSwitch threadTaskSwitchCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_task_switch = threadTaskSwitchCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetThreadTaskCompleteCallback(
    OTF2_EvtReaderCallbacks*                  evtReaderCallbacks,
    OTF2_EvtReaderCallback_ThreadTaskComplete threadTaskCompleteCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->thread_task_complete = threadTaskCompleteCallback;

    return OTF2_SUCCESS;
}
