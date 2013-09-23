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
 *  @file       src/OTF2_GlobalSnapReader_inc.c
 *  @source     templates/OTF2_GlobalSnapReader_inc.tmpl.c
 *
 *  @brief      This module reads the global snapshot records.
 */




static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_snapshot_start( const OTF2_GlobalSnapReader* reader,
                                                OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*   snap   = &snap_reader->current_snap;
    OTF2_SnapshotStart* record = &snap->record.snapshot_start;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.snapshot_start )
    {
        interrupt = reader->reader_callbacks.snapshot_start( snap_reader->location_id,
                                                             record->time,
                                                             reader->user_data,
                                                             &snap_reader->attribute_list,
                                                             record->number_of_records );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_snapshot_end( const OTF2_GlobalSnapReader* reader,
                                              OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap   = &snap_reader->current_snap;
    OTF2_SnapshotEnd* record = &snap->record.snapshot_end;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.snapshot_end )
    {
        interrupt = reader->reader_callbacks.snapshot_end( snap_reader->location_id,
                                                           record->time,
                                                           reader->user_data,
                                                           &snap_reader->attribute_list,
                                                           record->cont_read_pos );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_measurement_on_off( const OTF2_GlobalSnapReader* reader,
                                                    OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*          snap   = &snap_reader->current_snap;
    OTF2_MeasurementOnOffSnap* record = &snap->record.measurement_on_off;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.measurement_on_off )
    {
        interrupt = reader->reader_callbacks.measurement_on_off( snap_reader->location_id,
                                                                 record->time,
                                                                 reader->user_data,
                                                                 &snap_reader->attribute_list,
                                                                 record->orig_event_time,
                                                                 record->measurement_mode );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_enter( const OTF2_GlobalSnapReader* reader,
                                       OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap   = &snap_reader->current_snap;
    OTF2_EnterSnap*   record = &snap->record.enter;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.enter )
    {
        interrupt = reader->reader_callbacks.enter( snap_reader->location_id,
                                                    record->time,
                                                    reader->user_data,
                                                    &snap_reader->attribute_list,
                                                    record->orig_event_time,
                                                    record->region );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_mpi_send( const OTF2_GlobalSnapReader* reader,
                                          OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap   = &snap_reader->current_snap;
    OTF2_MpiSendSnap* record = &snap->record.mpi_send;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_send )
    {
        interrupt = reader->reader_callbacks.mpi_send( snap_reader->location_id,
                                                       record->time,
                                                       reader->user_data,
                                                       &snap_reader->attribute_list,
                                                       record->orig_event_time,
                                                       record->receiver,
                                                       record->communicator,
                                                       record->msg_tag,
                                                       record->msg_length );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_mpi_isend( const OTF2_GlobalSnapReader* reader,
                                           OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*  snap   = &snap_reader->current_snap;
    OTF2_MpiIsendSnap* record = &snap->record.mpi_isend;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_isend )
    {
        interrupt = reader->reader_callbacks.mpi_isend( snap_reader->location_id,
                                                        record->time,
                                                        reader->user_data,
                                                        &snap_reader->attribute_list,
                                                        record->orig_event_time,
                                                        record->receiver,
                                                        record->communicator,
                                                        record->msg_tag,
                                                        record->msg_length,
                                                        record->request_id );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_mpi_isend_complete( const OTF2_GlobalSnapReader* reader,
                                                    OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*          snap   = &snap_reader->current_snap;
    OTF2_MpiIsendCompleteSnap* record = &snap->record.mpi_isend_complete;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_isend_complete )
    {
        interrupt = reader->reader_callbacks.mpi_isend_complete( snap_reader->location_id,
                                                                 record->time,
                                                                 reader->user_data,
                                                                 &snap_reader->attribute_list,
                                                                 record->orig_event_time,
                                                                 record->request_id );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_mpi_recv( const OTF2_GlobalSnapReader* reader,
                                          OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap   = &snap_reader->current_snap;
    OTF2_MpiRecvSnap* record = &snap->record.mpi_recv;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_recv )
    {
        interrupt = reader->reader_callbacks.mpi_recv( snap_reader->location_id,
                                                       record->time,
                                                       reader->user_data,
                                                       &snap_reader->attribute_list,
                                                       record->orig_event_time,
                                                       record->sender,
                                                       record->communicator,
                                                       record->msg_tag,
                                                       record->msg_length );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_mpi_irecv_request( const OTF2_GlobalSnapReader* reader,
                                                   OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*         snap   = &snap_reader->current_snap;
    OTF2_MpiIrecvRequestSnap* record = &snap->record.mpi_irecv_request;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_irecv_request )
    {
        interrupt = reader->reader_callbacks.mpi_irecv_request( snap_reader->location_id,
                                                                record->time,
                                                                reader->user_data,
                                                                &snap_reader->attribute_list,
                                                                record->orig_event_time,
                                                                record->request_id );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_mpi_irecv( const OTF2_GlobalSnapReader* reader,
                                           OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*  snap   = &snap_reader->current_snap;
    OTF2_MpiIrecvSnap* record = &snap->record.mpi_irecv;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_irecv )
    {
        interrupt = reader->reader_callbacks.mpi_irecv( snap_reader->location_id,
                                                        record->time,
                                                        reader->user_data,
                                                        &snap_reader->attribute_list,
                                                        record->orig_event_time,
                                                        record->sender,
                                                        record->communicator,
                                                        record->msg_tag,
                                                        record->msg_length,
                                                        record->request_id );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_mpi_collective_begin( const OTF2_GlobalSnapReader* reader,
                                                      OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*            snap   = &snap_reader->current_snap;
    OTF2_MpiCollectiveBeginSnap* record = &snap->record.mpi_collective_begin;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_collective_begin )
    {
        interrupt = reader->reader_callbacks.mpi_collective_begin( snap_reader->location_id,
                                                                   record->time,
                                                                   reader->user_data,
                                                                   &snap_reader->attribute_list,
                                                                   record->orig_event_time );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_mpi_collective_end( const OTF2_GlobalSnapReader* reader,
                                                    OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*          snap   = &snap_reader->current_snap;
    OTF2_MpiCollectiveEndSnap* record = &snap->record.mpi_collective_end;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.mpi_collective_end )
    {
        interrupt = reader->reader_callbacks.mpi_collective_end( snap_reader->location_id,
                                                                 record->time,
                                                                 reader->user_data,
                                                                 &snap_reader->attribute_list,
                                                                 record->orig_event_time,
                                                                 record->collective_op,
                                                                 record->communicator,
                                                                 record->root,
                                                                 record->size_sent,
                                                                 record->size_received );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_omp_fork( const OTF2_GlobalSnapReader* reader,
                                          OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap   = &snap_reader->current_snap;
    OTF2_OmpForkSnap* record = &snap->record.omp_fork;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.omp_fork )
    {
        interrupt = reader->reader_callbacks.omp_fork( snap_reader->location_id,
                                                       record->time,
                                                       reader->user_data,
                                                       &snap_reader->attribute_list,
                                                       record->orig_event_time,
                                                       record->number_of_requested_threads );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_omp_acquire_lock( const OTF2_GlobalSnapReader* reader,
                                                  OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*        snap   = &snap_reader->current_snap;
    OTF2_OmpAcquireLockSnap* record = &snap->record.omp_acquire_lock;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.omp_acquire_lock )
    {
        interrupt = reader->reader_callbacks.omp_acquire_lock( snap_reader->location_id,
                                                               record->time,
                                                               reader->user_data,
                                                               &snap_reader->attribute_list,
                                                               record->orig_event_time,
                                                               record->lock_id,
                                                               record->acquisition_order );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_omp_task_create( const OTF2_GlobalSnapReader* reader,
                                                 OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*       snap   = &snap_reader->current_snap;
    OTF2_OmpTaskCreateSnap* record = &snap->record.omp_task_create;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.omp_task_create )
    {
        interrupt = reader->reader_callbacks.omp_task_create( snap_reader->location_id,
                                                              record->time,
                                                              reader->user_data,
                                                              &snap_reader->attribute_list,
                                                              record->orig_event_time,
                                                              record->task_id );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_omp_task_switch( const OTF2_GlobalSnapReader* reader,
                                                 OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*       snap   = &snap_reader->current_snap;
    OTF2_OmpTaskSwitchSnap* record = &snap->record.omp_task_switch;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.omp_task_switch )
    {
        interrupt = reader->reader_callbacks.omp_task_switch( snap_reader->location_id,
                                                              record->time,
                                                              reader->user_data,
                                                              &snap_reader->attribute_list,
                                                              record->orig_event_time,
                                                              record->task_id );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_metric( const OTF2_GlobalSnapReader* reader,
                                        OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap   = &snap_reader->current_snap;
    OTF2_MetricSnap*  record = &snap->record.metric;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.metric )
    {
        interrupt = reader->reader_callbacks.metric( snap_reader->location_id,
                                                     record->time,
                                                     reader->user_data,
                                                     &snap_reader->attribute_list,
                                                     record->orig_event_time,
                                                     record->metric,
                                                     record->number_of_metrics,
                                                     record->type_ids,
                                                     record->metric_values );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );

    free( record->type_ids );
    free( record->metric_values );

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_parameter_string( const OTF2_GlobalSnapReader* reader,
                                                  OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*         snap   = &snap_reader->current_snap;
    OTF2_ParameterStringSnap* record = &snap->record.parameter_string;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.parameter_string )
    {
        interrupt = reader->reader_callbacks.parameter_string( snap_reader->location_id,
                                                               record->time,
                                                               reader->user_data,
                                                               &snap_reader->attribute_list,
                                                               record->orig_event_time,
                                                               record->parameter,
                                                               record->string );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_parameter_int( const OTF2_GlobalSnapReader* reader,
                                               OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*      snap   = &snap_reader->current_snap;
    OTF2_ParameterIntSnap* record = &snap->record.parameter_int;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.parameter_int )
    {
        interrupt = reader->reader_callbacks.parameter_int( snap_reader->location_id,
                                                            record->time,
                                                            reader->user_data,
                                                            &snap_reader->attribute_list,
                                                            record->orig_event_time,
                                                            record->parameter,
                                                            record->value );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_parameter_unsigned_int( const OTF2_GlobalSnapReader* reader,
                                                        OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap*              snap   = &snap_reader->current_snap;
    OTF2_ParameterUnsignedIntSnap* record = &snap->record.parameter_unsigned_int;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.parameter_unsigned_int )
    {
        interrupt = reader->reader_callbacks.parameter_unsigned_int( snap_reader->location_id,
                                                                     record->time,
                                                                     reader->user_data,
                                                                     &snap_reader->attribute_list,
                                                                     record->orig_event_time,
                                                                     record->parameter,
                                                                     record->value );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static OTF2_ErrorCode
otf2_global_snap_reader_trigger_callback( const OTF2_GlobalSnapReader* reader,
                                          OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap = &snap_reader->current_snap;

    switch ( snap->type )
    {
        /* An attribute list should not be in the record queue alone. */
        case OTF2_ATTRIBUTE_LIST:
            return OTF2_ERROR_INTEGRITY_FAULT;

        case OTF2_SNAP_SNAPSHOT_START:
            return otf2_global_snap_reader_trigger_snapshot_start( reader,
                                                                   snap_reader );

        case OTF2_SNAP_SNAPSHOT_END:
            return otf2_global_snap_reader_trigger_snapshot_end( reader,
                                                                 snap_reader );

        case OTF2_SNAP_MEASUREMENT_ON_OFF:
            return otf2_global_snap_reader_trigger_measurement_on_off( reader,
                                                                       snap_reader );

        case OTF2_SNAP_ENTER:
            return otf2_global_snap_reader_trigger_enter( reader,
                                                          snap_reader );

        case OTF2_SNAP_MPI_SEND:
            return otf2_global_snap_reader_trigger_mpi_send( reader,
                                                             snap_reader );

        case OTF2_SNAP_MPI_ISEND:
            return otf2_global_snap_reader_trigger_mpi_isend( reader,
                                                              snap_reader );

        case OTF2_SNAP_MPI_ISEND_COMPLETE:
            return otf2_global_snap_reader_trigger_mpi_isend_complete( reader,
                                                                       snap_reader );

        case OTF2_SNAP_MPI_RECV:
            return otf2_global_snap_reader_trigger_mpi_recv( reader,
                                                             snap_reader );

        case OTF2_SNAP_MPI_IRECV_REQUEST:
            return otf2_global_snap_reader_trigger_mpi_irecv_request( reader,
                                                                      snap_reader );

        case OTF2_SNAP_MPI_IRECV:
            return otf2_global_snap_reader_trigger_mpi_irecv( reader,
                                                              snap_reader );

        case OTF2_SNAP_MPI_COLLECTIVE_BEGIN:
            return otf2_global_snap_reader_trigger_mpi_collective_begin( reader,
                                                                         snap_reader );

        case OTF2_SNAP_MPI_COLLECTIVE_END:
            return otf2_global_snap_reader_trigger_mpi_collective_end( reader,
                                                                       snap_reader );

        case OTF2_SNAP_OMP_FORK:
            return otf2_global_snap_reader_trigger_omp_fork( reader,
                                                             snap_reader );

        case OTF2_SNAP_OMP_ACQUIRE_LOCK:
            return otf2_global_snap_reader_trigger_omp_acquire_lock( reader,
                                                                     snap_reader );

        case OTF2_SNAP_OMP_TASK_CREATE:
            return otf2_global_snap_reader_trigger_omp_task_create( reader,
                                                                    snap_reader );

        case OTF2_SNAP_OMP_TASK_SWITCH:
            return otf2_global_snap_reader_trigger_omp_task_switch( reader,
                                                                    snap_reader );

        case OTF2_SNAP_METRIC:
            return otf2_global_snap_reader_trigger_metric( reader,
                                                           snap_reader );

        case OTF2_SNAP_PARAMETER_STRING:
            return otf2_global_snap_reader_trigger_parameter_string( reader,
                                                                     snap_reader );

        case OTF2_SNAP_PARAMETER_INT:
            return otf2_global_snap_reader_trigger_parameter_int( reader,
                                                                  snap_reader );

        case OTF2_SNAP_PARAMETER_UNSIGNED_INT:
            return otf2_global_snap_reader_trigger_parameter_unsigned_int( reader,
                                                                           snap_reader );

        default:
            return otf2_global_snap_reader_trigger_unknown( reader,
                                                            snap_reader );
    }
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetSnapshotStartCallback(
    OTF2_GlobalSnapReaderCallbacks*             globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_SnapshotStart snapshotStartCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->snapshot_start = snapshotStartCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetSnapshotEndCallback(
    OTF2_GlobalSnapReaderCallbacks*           globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_SnapshotEnd snapshotEndCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->snapshot_end = snapshotEndCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMeasurementOnOffCallback(
    OTF2_GlobalSnapReaderCallbacks*                globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MeasurementOnOff measurementOnOffCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->measurement_on_off = measurementOnOffCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetEnterCallback(
    OTF2_GlobalSnapReaderCallbacks*     globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_Enter enterCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->enter = enterCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiSendCallback(
    OTF2_GlobalSnapReaderCallbacks*       globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiSend mpiSendCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->mpi_send = mpiSendCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiIsendCallback(
    OTF2_GlobalSnapReaderCallbacks*        globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiIsend mpiIsendCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->mpi_isend = mpiIsendCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiIsendCompleteCallback(
    OTF2_GlobalSnapReaderCallbacks*                globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiIsendComplete mpiIsendCompleteCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->mpi_isend_complete = mpiIsendCompleteCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiRecvCallback(
    OTF2_GlobalSnapReaderCallbacks*       globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiRecv mpiRecvCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->mpi_recv = mpiRecvCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiIrecvRequestCallback(
    OTF2_GlobalSnapReaderCallbacks*               globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->mpi_irecv_request = mpiIrecvRequestCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiIrecvCallback(
    OTF2_GlobalSnapReaderCallbacks*        globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiIrecv mpiIrecvCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->mpi_irecv = mpiIrecvCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiCollectiveBeginCallback(
    OTF2_GlobalSnapReaderCallbacks*                  globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->mpi_collective_begin = mpiCollectiveBeginCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiCollectiveEndCallback(
    OTF2_GlobalSnapReaderCallbacks*                globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->mpi_collective_end = mpiCollectiveEndCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetOmpForkCallback(
    OTF2_GlobalSnapReaderCallbacks*       globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_OmpFork ompForkCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->omp_fork = ompForkCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetOmpAcquireLockCallback(
    OTF2_GlobalSnapReaderCallbacks*              globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_OmpAcquireLock ompAcquireLockCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->omp_acquire_lock = ompAcquireLockCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetOmpTaskCreateCallback(
    OTF2_GlobalSnapReaderCallbacks*             globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_OmpTaskCreate ompTaskCreateCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->omp_task_create = ompTaskCreateCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetOmpTaskSwitchCallback(
    OTF2_GlobalSnapReaderCallbacks*             globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_OmpTaskSwitch ompTaskSwitchCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->omp_task_switch = ompTaskSwitchCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMetricCallback(
    OTF2_GlobalSnapReaderCallbacks*      globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_Metric metricCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->metric = metricCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetParameterStringCallback(
    OTF2_GlobalSnapReaderCallbacks*               globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_ParameterString parameterStringCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->parameter_string = parameterStringCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetParameterIntCallback(
    OTF2_GlobalSnapReaderCallbacks*            globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_ParameterInt parameterIntCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->parameter_int = parameterIntCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetParameterUnsignedIntCallback(
    OTF2_GlobalSnapReaderCallbacks*                    globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->parameter_unsigned_int = parameterUnsignedIntCallback;

    return OTF2_SUCCESS;
}
