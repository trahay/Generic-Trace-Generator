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

/** @brief Write location event stack to file.
 *
 *  @param location_event_stack Current active location event stack struct in the point of time where the break point is reached.
 *
 * @{
 */
void
location_event_stack_write( OTF2_SnapWriter*            writerHandle,
                            list<otf2_snapshots_event*> location_event_stack,
                            OTF2_TimeStamp              break_time )
{
    for ( list<otf2_snapshots_event*>::iterator it = location_event_stack.begin();
          it != location_event_stack.end();
          it++ )
    {
        otf2_snapshots_event* event = *it;

        switch ( event->type )
        {
            case OTF2_SNAPSHOTS_EVENT_MEASUREMENT_ON_OFF:
            {
                otf2_snapshots_event_measurement_on_off* derived_event = ( otf2_snapshots_event_measurement_on_off* )event;

                OTF2_SnapWriter_MeasurementOnOff( writerHandle,
                                                  event->attributes,
                                                  break_time,
                                                  event->time,
                                                  derived_event->measurement_mode );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_ENTER:
            {
                otf2_snapshots_event_enter* derived_event = ( otf2_snapshots_event_enter* )event;

                OTF2_SnapWriter_Enter( writerHandle,
                                       event->attributes,
                                       break_time,
                                       event->time,
                                       derived_event->region );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_MPI_SEND:
            {
                otf2_snapshots_event_mpi_send* derived_event = ( otf2_snapshots_event_mpi_send* )event;

                OTF2_SnapWriter_MpiSend( writerHandle,
                                         event->attributes,
                                         break_time,
                                         event->time,
                                         derived_event->receiver,
                                         derived_event->communicator,
                                         derived_event->msg_tag,
                                         derived_event->msg_length );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_MPI_ISEND:
            {
                otf2_snapshots_event_mpi_isend* derived_event = ( otf2_snapshots_event_mpi_isend* )event;

                OTF2_SnapWriter_MpiIsend( writerHandle,
                                          event->attributes,
                                          break_time,
                                          event->time,
                                          derived_event->receiver,
                                          derived_event->communicator,
                                          derived_event->msg_tag,
                                          derived_event->msg_length,
                                          derived_event->request_id );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_MPI_ISEND_COMPLETE:
            {
                otf2_snapshots_event_mpi_isend_complete* derived_event = ( otf2_snapshots_event_mpi_isend_complete* )event;

                OTF2_SnapWriter_MpiIsendComplete( writerHandle,
                                                  event->attributes,
                                                  break_time,
                                                  event->time,
                                                  derived_event->request_id );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST:
            {
                otf2_snapshots_event_mpi_irecv_request* derived_event = ( otf2_snapshots_event_mpi_irecv_request* )event;

                OTF2_SnapWriter_MpiIrecvRequest( writerHandle,
                                                 event->attributes,
                                                 break_time,
                                                 event->time,
                                                 derived_event->request_id );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_MPI_RECV:
            {
                otf2_snapshots_event_mpi_recv* derived_event = ( otf2_snapshots_event_mpi_recv* )event;

                OTF2_SnapWriter_MpiRecv( writerHandle,
                                         event->attributes,
                                         break_time,
                                         event->time,
                                         derived_event->sender,
                                         derived_event->communicator,
                                         derived_event->msg_tag,
                                         derived_event->msg_length );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_MPI_IRECV:
            {
                otf2_snapshots_event_mpi_irecv* derived_event = ( otf2_snapshots_event_mpi_irecv* )event;

                OTF2_SnapWriter_MpiIrecv( writerHandle,
                                          event->attributes,
                                          break_time,
                                          event->time,
                                          derived_event->sender,
                                          derived_event->communicator,
                                          derived_event->msg_tag,
                                          derived_event->msg_length,
                                          derived_event->request_id );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_BEGIN:
            {
                otf2_snapshots_event_mpi_collective_begin* derived_event = ( otf2_snapshots_event_mpi_collective_begin* )event;

                OTF2_SnapWriter_MpiCollectiveBegin( writerHandle,
                                                    event->attributes,
                                                    break_time,
                                                    event->time  );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_END:
            {
                otf2_snapshots_event_mpi_collective_end* derived_event = ( otf2_snapshots_event_mpi_collective_end* )event;

                OTF2_SnapWriter_MpiCollectiveEnd( writerHandle,
                                                  event->attributes,
                                                  break_time,
                                                  event->time,
                                                  derived_event->collective_op,
                                                  derived_event->communicator,
                                                  derived_event->root,
                                                  derived_event->size_sent,
                                                  derived_event->size_received );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_OMP_FORK:
            {
                otf2_snapshots_event_omp_fork* derived_event = ( otf2_snapshots_event_omp_fork* )event;

                OTF2_SnapWriter_OmpFork( writerHandle,
                                         event->attributes,
                                         break_time,
                                         event->time,
                                         derived_event->number_of_requested_threads );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_OMP_ACQUIRE_LOCK:
            {
                otf2_snapshots_event_omp_acquire_lock* derived_event = ( otf2_snapshots_event_omp_acquire_lock* )event;

                OTF2_SnapWriter_OmpAcquireLock( writerHandle,
                                                event->attributes,
                                                break_time,
                                                event->time,
                                                derived_event->lock_id,
                                                derived_event->acquisition_order );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_OMP_TASK_CREATE:
            {
                otf2_snapshots_event_omp_task_create* derived_event = ( otf2_snapshots_event_omp_task_create* )event;

                OTF2_SnapWriter_OmpTaskCreate( writerHandle,
                                               event->attributes,
                                               break_time,
                                               event->time,
                                               derived_event->task_id );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_OMP_TASK_SWITCH:
            {
                otf2_snapshots_event_omp_task_switch* derived_event = ( otf2_snapshots_event_omp_task_switch* )event;

                OTF2_SnapWriter_OmpTaskSwitch( writerHandle,
                                               event->attributes,
                                               break_time,
                                               event->time,
                                               derived_event->task_id );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_METRIC:
            {
                otf2_snapshots_event_metric* derived_event = ( otf2_snapshots_event_metric* )event;

                OTF2_SnapWriter_Metric( writerHandle,
                                        event->attributes,
                                        break_time,
                                        event->time,
                                        derived_event->metric,
                                        derived_event->number_of_metrics,
                                        derived_event->type_ids,
                                        derived_event->metric_values );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_PARAMETER_STRING:
            {
                otf2_snapshots_event_parameter_string* derived_event = ( otf2_snapshots_event_parameter_string* )event;

                OTF2_SnapWriter_ParameterString( writerHandle,
                                                 event->attributes,
                                                 break_time,
                                                 event->time,
                                                 derived_event->parameter,
                                                 derived_event->string );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_PARAMETER_INT:
            {
                otf2_snapshots_event_parameter_int* derived_event = ( otf2_snapshots_event_parameter_int* )event;

                OTF2_SnapWriter_ParameterInt( writerHandle,
                                              event->attributes,
                                              break_time,
                                              event->time,
                                              derived_event->parameter,
                                              derived_event->value );
            }
            break;

            case OTF2_SNAPSHOTS_EVENT_PARAMETER_UNSIGNED_INT:
            {
                otf2_snapshots_event_parameter_unsigned_int* derived_event = ( otf2_snapshots_event_parameter_unsigned_int* )event;

                OTF2_SnapWriter_ParameterUnsignedInt( writerHandle,
                                                      event->attributes,
                                                      break_time,
                                                      event->time,
                                                      derived_event->parameter,
                                                      derived_event->value );
            }
            break;


            default:
                UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                             "Can't process unexpected event type." );
                break;
        }
    }
}

/*___ stack event callbacks._____________________________________________ */

extern "C"
{
static OTF2_CallbackCode
global_evt_callback_measurement_on_off( uint64_t             locationID,
                                        uint64_t             time,
                                        void*                userData,
                                        OTF2_AttributeList*  attributes,
                                        OTF2_MeasurementMode measurementMode )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );

    /* remove existing MeasurementOnOffRecord event if there is one */
    if ( data->measurement_on_off_record[ locationID ] )
    {
        erase_event_from_stack( &data->locations_event_stack[ locationID ],
                                OTF2_SNAPSHOTS_EVENT_MEASUREMENT_ON_OFF,
                                0 );
    }
    else
    {
        data->measurement_on_off_record[ locationID ] = true;
    }

    otf2_snapshots_event_measurement_on_off* event = new otf2_snapshots_event_measurement_on_off( time, attributes,
                                                                                                  measurementMode );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_enter( uint64_t            locationID,
                           uint64_t            time,
                           void*               userData,
                           OTF2_AttributeList* attributes,
                           OTF2_RegionRef      region )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_enter* event = new otf2_snapshots_event_enter( time, attributes,
                                                                        region );

    data->locations_event_stack[ locationID ].push_back( event );





    region_activity_sampling( data, time );

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_mpi_send( uint64_t            locationID,
                              uint64_t            time,
                              void*               userData,
                              OTF2_AttributeList* attributes,
                              uint32_t            receiver,
                              OTF2_CommRef        communicator,
                              uint32_t            msgTag,
                              uint64_t            msgLength )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_mpi_send* event = new otf2_snapshots_event_mpi_send( time, attributes,
                                                                              receiver,
                                                                              communicator,
                                                                              msgTag,
                                                                              msgLength );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_mpi_isend( uint64_t            locationID,
                               uint64_t            time,
                               void*               userData,
                               OTF2_AttributeList* attributes,
                               uint32_t            receiver,
                               OTF2_CommRef        communicator,
                               uint32_t            msgTag,
                               uint64_t            msgLength,
                               uint64_t            requestID )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_mpi_isend* event = new otf2_snapshots_event_mpi_isend( time, attributes,
                                                                                receiver,
                                                                                communicator,
                                                                                msgTag,
                                                                                msgLength,
                                                                                requestID );

    data->locations_event_stack[ locationID ].push_back( event );

    /* keep kind of request */
    data->mpi_requests.insert( make_pair( requestID, OTF2_SNAPSHOTS_EVENT_MPI_ISEND ) );





    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_mpi_isend_complete( uint64_t            locationID,
                                        uint64_t            time,
                                        void*               userData,
                                        OTF2_AttributeList* attributes,
                                        uint64_t            requestID )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_mpi_isend_complete* event = new otf2_snapshots_event_mpi_isend_complete( time, attributes,
                                                                                                  requestID );

    data->locations_event_stack[ locationID ].push_back( event );


    match_pending_recvs( locationID, data );




    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_mpi_irecv_request( uint64_t            locationID,
                                       uint64_t            time,
                                       void*               userData,
                                       OTF2_AttributeList* attributes,
                                       uint64_t            requestID )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_mpi_irecv_request* event = new otf2_snapshots_event_mpi_irecv_request( time, attributes,
                                                                                                requestID );

    data->locations_event_stack[ locationID ].push_back( event );



    /* keep kind of request */
    data->mpi_requests.insert( make_pair( requestID, OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST ) );



    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_mpi_collective_begin( uint64_t            locationID,
                                          uint64_t            time,
                                          void*               userData,
                                          OTF2_AttributeList* attributes  )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_mpi_collective_begin* event = new otf2_snapshots_event_mpi_collective_begin( time, attributes  );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_omp_fork( uint64_t            locationID,
                              uint64_t            time,
                              void*               userData,
                              OTF2_AttributeList* attributes,
                              uint32_t            numberOfRequestedThreads )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_omp_fork* event = new otf2_snapshots_event_omp_fork( time, attributes,
                                                                              numberOfRequestedThreads );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_omp_acquire_lock( uint64_t            locationID,
                                      uint64_t            time,
                                      void*               userData,
                                      OTF2_AttributeList* attributes,
                                      uint32_t            lockID,
                                      uint32_t            acquisitionOrder )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_omp_acquire_lock* event = new otf2_snapshots_event_omp_acquire_lock( time, attributes,
                                                                                              lockID,
                                                                                              acquisitionOrder );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_omp_task_create( uint64_t            locationID,
                                     uint64_t            time,
                                     void*               userData,
                                     OTF2_AttributeList* attributes,
                                     uint64_t            taskID )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_omp_task_create* event = new otf2_snapshots_event_omp_task_create( time, attributes,
                                                                                            taskID );

    data->locations_event_stack[ locationID ].push_back( event );




    /* buffer location that created this task to easily remove creation event when task complete */
    data->omp_task_creator.insert( make_pair( taskID, locationID ) );


    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_omp_task_switch( uint64_t            locationID,
                                     uint64_t            time,
                                     void*               userData,
                                     OTF2_AttributeList* attributes,
                                     uint64_t            taskID )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_omp_task_switch* event = new otf2_snapshots_event_omp_task_switch( time, attributes,
                                                                                            taskID );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_parameter_string( uint64_t            locationID,
                                      uint64_t            time,
                                      void*               userData,
                                      OTF2_AttributeList* attributes,
                                      OTF2_ParameterRef   parameter,
                                      OTF2_StringRef      string )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_parameter_string* event = new otf2_snapshots_event_parameter_string( time, attributes,
                                                                                              parameter,
                                                                                              string );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_parameter_int( uint64_t            locationID,
                                   uint64_t            time,
                                   void*               userData,
                                   OTF2_AttributeList* attributes,
                                   OTF2_ParameterRef   parameter,
                                   int64_t             value )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_parameter_int* event = new otf2_snapshots_event_parameter_int( time, attributes,
                                                                                        parameter,
                                                                                        value );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_parameter_unsigned_int( uint64_t            locationID,
                                            uint64_t            time,
                                            void*               userData,
                                            OTF2_AttributeList* attributes,
                                            OTF2_ParameterRef   parameter,
                                            uint64_t            value )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    otf2_snapshots_event_parameter_unsigned_int* event = new otf2_snapshots_event_parameter_unsigned_int( time, attributes,
                                                                                                          parameter,
                                                                                                          value );

    data->locations_event_stack[ locationID ].push_back( event );






    return OTF2_CALLBACK_SUCCESS;
}
}

/*___ non stack event callbacks.________________________________________________ */

extern "C"
{
static OTF2_CallbackCode
global_evt_callback_leave( uint64_t            locationID,
                           uint64_t            time,
                           void*               userData,
                           OTF2_AttributeList* attributes,
                           OTF2_RegionRef      region )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );




    erase_event_from_stack( &data->locations_event_stack[ locationID ],
                            OTF2_SNAPSHOTS_EVENT_ENTER,

                            region
                            );

    region_activity_sampling( data, time );

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_mpi_request_cancelled( uint64_t            locationID,
                                           uint64_t            time,
                                           void*               userData,
                                           OTF2_AttributeList* attributes,
                                           uint64_t            requestID )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );


    // try to match pending recvs in case of an Isend request cancel ..

    // try to match pending recvs in case of an Isend request cancel
    //
    if ( data->mpi_requests[ requestID ] == OTF2_SNAPSHOTS_EVENT_MPI_ISEND )
    {
        match_pending_recvs( locationID, data );
    }

    erase_event_from_stack( &data->locations_event_stack[ locationID ],
                            data->mpi_requests[ requestID ],

                            requestID
                            );


    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_omp_join( uint64_t            locationID,
                              uint64_t            time,
                              void*               userData,
                              OTF2_AttributeList* attributes  )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );




    erase_event_from_stack( &data->locations_event_stack[ locationID ],
                            OTF2_SNAPSHOTS_EVENT_OMP_FORK,

                            0
                            );


    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_omp_release_lock( uint64_t            locationID,
                                      uint64_t            time,
                                      void*               userData,
                                      OTF2_AttributeList* attributes,
                                      uint32_t            lockID,
                                      uint32_t            acquisitionOrder )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );




    erase_event_from_stack( &data->locations_event_stack[ locationID ],
                            OTF2_SNAPSHOTS_EVENT_OMP_ACQUIRE_LOCK,

                            lockID
                            );


    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_omp_task_complete( uint64_t            locationID,
                                       uint64_t            time,
                                       void*               userData,
                                       OTF2_AttributeList* attributes,
                                       uint64_t            taskID )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );

    // erase OMP_switch of location 'locationID' to this task
    // (there's only this one switch since scorep doesn't support more)
    //
    erase_event_from_stack( &data->locations_event_stack[ locationID ],
                            OTF2_SNAPSHOTS_EVENT_OMP_TASK_SWITCH, taskID );



    erase_event_from_stack( &data->locations_event_stack[ locationID ],
                            OTF2_SNAPSHOTS_EVENT_OMP_TASK_CREATE,

                            taskID
                            );


    return OTF2_CALLBACK_SUCCESS;
}
}


OTF2_GlobalDefReaderCallbacks*
otf2_snapshots_get_global_def_callbacks( void )
{
    OTF2_GlobalDefReaderCallbacks* callbacks = OTF2_GlobalDefReaderCallbacks_New();
    OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback( callbacks, global_def_callback_clock_properties );
    OTF2_GlobalDefReaderCallbacks_SetLocationCallback( callbacks, global_def_callback_location );
    OTF2_GlobalDefReaderCallbacks_SetRegionCallback( callbacks, global_def_callback_region );
    OTF2_GlobalDefReaderCallbacks_SetGroupCallback( callbacks, global_def_callback_group );
    OTF2_GlobalDefReaderCallbacks_SetMetricMemberCallback( callbacks, global_def_callback_metric_member );
    OTF2_GlobalDefReaderCallbacks_SetMetricClassCallback( callbacks, global_def_callback_metric_class );
    OTF2_GlobalDefReaderCallbacks_SetMetricInstanceCallback( callbacks, global_def_callback_metric_instance );
    OTF2_GlobalDefReaderCallbacks_SetCommCallback( callbacks, global_def_callback_comm );

    return callbacks;
}


OTF2_GlobalEvtReaderCallbacks*
otf2_snapshots_get_event_callbacks( void )
{
    OTF2_GlobalEvtReaderCallbacks* callbacks = OTF2_GlobalEvtReaderCallbacks_New();
    /* stack event callbacks. */
    OTF2_GlobalEvtReaderCallbacks_SetMeasurementOnOffCallback( callbacks, global_evt_callback_measurement_on_off );
    OTF2_GlobalEvtReaderCallbacks_SetEnterCallback( callbacks, global_evt_callback_enter );
    OTF2_GlobalEvtReaderCallbacks_SetMpiSendCallback( callbacks, global_evt_callback_mpi_send );
    OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCallback( callbacks, global_evt_callback_mpi_isend );
    OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCompleteCallback( callbacks, global_evt_callback_mpi_isend_complete );
    OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvRequestCallback( callbacks, global_evt_callback_mpi_irecv_request );
    OTF2_GlobalEvtReaderCallbacks_SetMpiRecvCallback( callbacks, global_evt_callback_mpi_recv );
    OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvCallback( callbacks, global_evt_callback_mpi_irecv );
    OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveBeginCallback( callbacks, global_evt_callback_mpi_collective_begin );
    OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveEndCallback( callbacks, global_evt_callback_mpi_collective_end );
    OTF2_GlobalEvtReaderCallbacks_SetOmpForkCallback( callbacks, global_evt_callback_omp_fork );
    OTF2_GlobalEvtReaderCallbacks_SetOmpAcquireLockCallback( callbacks, global_evt_callback_omp_acquire_lock );
    OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCreateCallback( callbacks, global_evt_callback_omp_task_create );
    OTF2_GlobalEvtReaderCallbacks_SetOmpTaskSwitchCallback( callbacks, global_evt_callback_omp_task_switch );
    OTF2_GlobalEvtReaderCallbacks_SetMetricCallback( callbacks, global_evt_callback_metric );
    OTF2_GlobalEvtReaderCallbacks_SetParameterStringCallback( callbacks, global_evt_callback_parameter_string );
    OTF2_GlobalEvtReaderCallbacks_SetParameterIntCallback( callbacks, global_evt_callback_parameter_int );
    OTF2_GlobalEvtReaderCallbacks_SetParameterUnsignedIntCallback( callbacks, global_evt_callback_parameter_unsigned_int );

    /* non stack event callbacks. */
    OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback( callbacks, global_evt_callback_leave );
    OTF2_GlobalEvtReaderCallbacks_SetMpiRequestCancelledCallback( callbacks, global_evt_callback_mpi_request_cancelled );
    OTF2_GlobalEvtReaderCallbacks_SetOmpJoinCallback( callbacks, global_evt_callback_omp_join );
    OTF2_GlobalEvtReaderCallbacks_SetOmpReleaseLockCallback( callbacks, global_evt_callback_omp_release_lock );
    OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCompleteCallback( callbacks, global_evt_callback_omp_task_complete );

    return callbacks;
}
