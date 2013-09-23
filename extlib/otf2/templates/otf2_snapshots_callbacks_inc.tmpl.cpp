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
            @otf2 for event in events | snapshot_events:
            case OTF2_SNAPSHOTS_EVENT_@@event.upper@@:
            {
                otf2_snapshots_event_@@event.lower@@* derived_event = ( otf2_snapshots_event_@@event.lower@@* )event;

                OTF2_SnapWriter_@@event.name@@( writerHandle,
                                                event->attributes,
                                                break_time,
                                                event->time @@event.callargs(case='lower', argprefix='derived_event->')@@ );
            }
            break;

            @otf2 endfor

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

@otf2 for event in events | snapshot_events | exclude( Metric, MpiRecv, MpiIrecv, MpiCollectiveEnd ):

static OTF2_CallbackCode
global_evt_callback_@@event.lower@@( uint64_t   locationID,
                                     uint64_t   time,
                                     void*      userData,
                                     OTF2_AttributeList* attributes @@event.funcargs()@@ )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );

    @otf2 if event == MeasurementOnOff:
    /* remove existing MeasurementOnOffRecord event if there is one */
    if ( data->measurement_on_off_record[locationID] )
    {
        erase_event_from_stack( &data->locations_event_stack[ locationID ],
                                OTF2_SNAPSHOTS_EVENT_MEASUREMENT_ON_OFF,
                                0 );
    }
    else
    {
        data->measurement_on_off_record[locationID] = true;
    }
    @otf2 endif

    otf2_snapshots_event_@@event.lower@@* event = new otf2_snapshots_event_@@event.lower@@( time, attributes @@event.callargs()@@ );

    data->locations_event_stack[ locationID ].push_back( event );

    @otf2 if event == MpiIsend:
    /* keep kind of request */
    data->mpi_requests.insert( make_pair(requestID, OTF2_SNAPSHOTS_EVENT_MPI_ISEND) );
    @otf2 endif

    @otf2 if event == MpiIsendComplete:
    match_pending_recvs( locationID, data );
    @otf2 endif

    @otf2 if event == MpiIrecvRequest:
    /* keep kind of request */
    data->mpi_requests.insert( make_pair(requestID, OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST) );
    @otf2 endif

    @otf2 if event == OmpTaskCreate:
    /* buffer location that created this task to easily remove creation event when task complete */
    data->omp_task_creator.insert( make_pair( taskID, locationID ) );
    @otf2 endif

    @otf2 if event == Enter:
    region_activity_sampling( data, time );
    @otf2 endif

    return OTF2_CALLBACK_SUCCESS;
}

@otf2 endfor

}

/*___ non stack event callbacks.________________________________________________ */

extern "C"
{

@otf2 for event in events | include( Leave, MpiRequestCancelled, OmpJoin, OmpReleaseLock, OmpTaskComplete ):

static OTF2_CallbackCode
global_evt_callback_@@event.lower@@( uint64_t   locationID,
                                     uint64_t   time,
                                     void*      userData,
                                     OTF2_AttributeList* attributes @@event.funcargs()@@ )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );

    @otf2 if event == OmpTaskComplete:
    // erase OMP_switch of location 'locationID' to this task
    // (there's only this one switch since scorep doesn't support more)
    //
    erase_event_from_stack( &data->locations_event_stack[ locationID ],
                            OTF2_SNAPSHOTS_EVENT_OMP_TASK_SWITCH, taskID );
    @otf2 endif

    @otf2 if event == MpiRequestCancelled:
    // try to match pending recvs in case of an Isend request cancel ..
    @otf2 endif

    @otf2 if event == MpiRequestCancelled:
    // try to match pending recvs in case of an Isend request cancel
    //
    if (data->mpi_requests[ requestID ] == OTF2_SNAPSHOTS_EVENT_MPI_ISEND )
    match_pending_recvs( locationID, data);
    @otf2 endif

    @otf2 if event != MpiRecv:
    erase_event_from_stack ( &data->locations_event_stack[ locationID ],
                             @otf2 if event == Leave:
                             OTF2_SNAPSHOTS_EVENT_ENTER,
                             @otf2 endif
                             @otf2 if event == MpiIrecv:
                             OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST,
                             @otf2 endif
                             @otf2 if event == MpiRequestCancelled:
                             data->mpi_requests[ requestID ],
                             @otf2 endif
                             @otf2 if event == MpiCollectiveEnd:
                             OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_BEGIN,
                             @otf2 endif
                             @otf2 if event == OmpJoin:
                             OTF2_SNAPSHOTS_EVENT_OMP_FORK,
                             @otf2 endif
                             @otf2 if event == OmpReleaseLock:
                             OTF2_SNAPSHOTS_EVENT_OMP_ACQUIRE_LOCK,
                             @otf2 endif
                             @otf2 if event == OmpTaskComplete:
                             OTF2_SNAPSHOTS_EVENT_OMP_TASK_CREATE,
                             @otf2 endif

                             @otf2 if event == Leave:
                             region
                             @otf2 elif event == MpiIrecv:
                             requestID
                             @otf2 elif event == MpiRequestCancelled:
                             requestID
                             @otf2 elif event == OmpReleaseLock:
                             lockID
                             @otf2 elif event == OmpTaskComplete:
                             taskID
                             @otf2 else:
                             0
                             @otf2 endif
                             );
    @otf2 endif

    @otf2 if event == Leave:
    region_activity_sampling( data, time );
    @otf2 endif

    return OTF2_CALLBACK_SUCCESS;
}

@otf2 endfor

}


OTF2_GlobalDefReaderCallbacks*
otf2_snapshots_get_global_def_callbacks( void )
{
    OTF2_GlobalDefReaderCallbacks* callbacks = OTF2_GlobalDefReaderCallbacks_New();
    @otf2 for def in defs | include( Location, MetricClass, MetricInstance, MetricMember, ClockProperties, Comm, Group, Region )
    OTF2_GlobalDefReaderCallbacks_Set@@def@@Callback( callbacks, global_def_callback_@@def.lower@@ );
    @otf2 endfor

    return callbacks;
}


OTF2_GlobalEvtReaderCallbacks*
otf2_snapshots_get_event_callbacks( void )
{
    OTF2_GlobalEvtReaderCallbacks* callbacks = OTF2_GlobalEvtReaderCallbacks_New();
    /* stack event callbacks. */
    @otf2 for event in events | snapshot_events:
    OTF2_GlobalEvtReaderCallbacks_Set@@event.name@@Callback( callbacks, global_evt_callback_@@event.lower@@ );
    @otf2 endfor

    /* non stack event callbacks. */
    @otf2 for event in events | include( Leave, MpiRequestCancelled, OmpJoin, OmpReleaseLock, OmpTaskComplete ):
    OTF2_GlobalEvtReaderCallbacks_Set@@event.name@@Callback( callbacks, global_evt_callback_@@event.lower@@ );
    @otf2 endfor

    return callbacks;
}
