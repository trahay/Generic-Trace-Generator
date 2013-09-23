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

#include <config.h>

#define __STDC_FORMAT_MACROS    // for PRIu8 in inttypes.h
#include <inttypes.h>

#include <float.h>

#include <string>

#include <iostream>

#include <stdlib.h>
#include <cstring>

#include <UTILS_Error.h>

#include <otf2/otf2.h>

#include "otf2_snapshots.hpp"
#include "otf2_snapshots_parameters.hpp"
#include "otf2_snapshots_events.hpp"
#include "otf2_snapshots_callbacks.hpp"


using namespace std;


/** @brief erases the last matching event in a otf2_snapshots_event list */

static void
erase_event_from_stack( list<otf2_snapshots_event*>* event_stack,
                        otf2_snapshots_event_type    event_type,
                        uint64_t                     event_ID )
{
    bool found = false;

    // reading events starting at top of event stack
    //
    list<otf2_snapshots_event*>::reverse_iterator rit = event_stack->rbegin();
    while ( rit != event_stack->rend() )
    {
        if ( ( *rit )->type == event_type  )
        {
            switch ( event_type )
            {
                case OTF2_SNAPSHOTS_EVENT_ENTER:
                    if ( ( ( otf2_snapshots_event_enter* )( *rit ) )->region == event_ID )
                    {
                        /* Remove any parameter* events which happend after this enter */
                        list<otf2_snapshots_event*>::iterator it = rit.base();
                        while ( it != event_stack->end() )
                        {
                            switch ( ( *it )->type )
                            {
                                case OTF2_SNAPSHOTS_EVENT_PARAMETER_STRING:
                                case OTF2_SNAPSHOTS_EVENT_PARAMETER_INT:
                                case OTF2_SNAPSHOTS_EVENT_PARAMETER_UNSIGNED_INT:
                                    delete *it;
                                    it = event_stack->erase( it );
                                    break;
                                default:
                                    ++it;
                                    break;
                            }
                        }
                        found = true;
                    }
                    else
                    {
                        UTILS_FATAL( "Broken trace! OTF2_SNAPSHOTS_EVENT_ENTER with unexpected region ID!" );
                    }
                    break;
                case OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST:
                    if ( ( ( otf2_snapshots_event_mpi_irecv_request* )( *rit ) )->request_id == event_ID )
                    {
                        found = true;
                        break;
                    }
                case OTF2_SNAPSHOTS_EVENT_MPI_IRECV:
                    if ( ( ( otf2_snapshots_event_mpi_irecv* )( *rit ) )->request_id == event_ID )
                    {
                        found = true;
                        break;
                    }
                case OTF2_SNAPSHOTS_EVENT_OMP_FORK:
                    found = true;
                    break;
                case OTF2_SNAPSHOTS_EVENT_OMP_TASK_CREATE:
                    if ( ( ( otf2_snapshots_event_omp_task_create* )( *rit ) )->task_id == event_ID )
                    {
                        found = true;
                        break;
                    }
                case OTF2_SNAPSHOTS_EVENT_OMP_TASK_SWITCH:
                    if ( ( ( otf2_snapshots_event_omp_task_switch* )( *rit ) )->task_id == event_ID )
                    {
                        found = true;
                        break;
                    }
                case OTF2_SNAPSHOTS_EVENT_OMP_ACQUIRE_LOCK:
                    if ( ( ( otf2_snapshots_event_omp_acquire_lock* )( *rit ) )->lock_id == event_ID )
                    {
                        found = true;
                        break;
                    }
                case OTF2_SNAPSHOTS_EVENT_MEASUREMENT_ON_OFF:
                    found = true;
                    break;
                default:
                    break;
            }
            if ( found )
            {
                delete *rit;
                event_stack->erase( --( rit.base() ) );
                return;
            }
        }
        rit++;
    }
    UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                 "No matching event found in callstack!" );
}

/** @brief erases the *first* matching recv event in a otf2_snapshots_event list */

static void
erase_mpi_recv_event( list<otf2_snapshots_event*>* event_stack,
                      uint32_t                     sender,
                      OTF2_CommRef                 communicator,
                      uint32_t                     msgTag,
                      uint64_t                     msgLength )
{
    list<otf2_snapshots_event*>::iterator it = event_stack->begin();

    while ( it != event_stack->end() )
    {
        if ( ( *it )->type == OTF2_SNAPSHOTS_EVENT_MPI_RECV )
        {
            otf2_snapshots_event_mpi_recv* recv = ( otf2_snapshots_event_mpi_recv* )( *it );

            if (    recv->sender       == sender
                    && recv->communicator == communicator
                    && recv->msg_tag      == msgTag
                    && recv->msg_length   == msgLength )
            {
                delete recv;
                event_stack->erase( it );
                return;
            }
        }
        it++;
    }
    UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                 "No matching event found in callstack!" );
}

/** @brief erases matching collective events in a otf2_snapshots_event list */

static void
erase_mpi_collective_events( list<otf2_snapshots_event*>* event_stack,
                             OTF2_CommRef                 communicator )
{
    /* erase first collective end event with matching communicator
     * buffer latest collective begin */

    list<otf2_snapshots_event*>::iterator it, it_coll_begin = event_stack->end();

    it = event_stack->begin();

    while ( it != event_stack->end() )
    {
        if ( ( *it )->type == OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_BEGIN )
        {
            it_coll_begin = it;
        }

        if ( ( *it )->type == OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_END )
        {
            otf2_snapshots_event_mpi_collective_end* mce = ( otf2_snapshots_event_mpi_collective_end* )( *it );
            if ( mce->communicator == communicator )
            {
                delete mce;
                event_stack->erase( it );
                break;
            }
        }
        it++;
    }
    if ( it_coll_begin == event_stack->end() )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "No MPI_COLLECTIVE_BEGIN event found in callstack!" );
    }

    /* erase latest collective begin */
    otf2_snapshots_event_mpi_collective_begin* mcb = ( otf2_snapshots_event_mpi_collective_begin* )( *it_coll_begin );
    delete mcb;
    event_stack->erase( it_coll_begin );
}


/** map MPI-rank to global location id */

static uint64_t
get_global_location_id( otf2_snapshots_data* ud,
                        uint32_t             communicator,
                        uint32_t             rank )
{
    // communicator -> group id
    //
    uint32_t group_id = ud->mpi_comm2group[ communicator ];

    // group_id + lokal MPI rank -> rank in MPI comm world
    //
    uint64_t rank_comm_world = ud->group_members[ group_id ][ rank ];

    // group MPI LOCATIONS + rank in MPI comm world -> global location id
    //
    return ud->group_members[ ud->mpi_location_group_id ][ rank_comm_world ];
}

/* returns true if message was matched else false
 * */
static bool
message_matching( list<otf2_snapshots_event*>* event_stack,
                  uint64_t                     receiverLocationID,
                  OTF2_CommRef                 communicator,
                  uint32_t                     msgTag,
                  uint64_t                     msgLength,
                  otf2_snapshots_data*         data )
{
    map<uint64_t, list<otf2_snapshots_event*>::iterator> matching_isends;

    matching_isends.clear();

    // reading events from the beginning of event list
    //
    list<otf2_snapshots_event*>::iterator it = event_stack->begin();

    bool matching_isend_found = false;

    while (  it != event_stack->end() )
    {
        if ( ( *it )->type == OTF2_SNAPSHOTS_EVENT_MPI_SEND && !matching_isend_found )
        {
            otf2_snapshots_event_mpi_send* mpi_send = ( otf2_snapshots_event_mpi_send* )( *it );

            uint64_t mpi_send_receiver_location_id = get_global_location_id( data, mpi_send->communicator,
                                                                             mpi_send->receiver );

            if ( mpi_send_receiver_location_id == receiverLocationID &&
                 mpi_send->communicator        == communicator &&
                 mpi_send->msg_tag             == msgTag &&
                 mpi_send->msg_length          == msgLength )
            {
                delete *it;
                event_stack->erase( it );
                return true;
            }
        }
        if ( ( *it )->type == OTF2_SNAPSHOTS_EVENT_MPI_ISEND )
        {
            otf2_snapshots_event_mpi_isend* mpi_isend = ( otf2_snapshots_event_mpi_isend* )( *it );

            uint64_t mpi_isend_receiver_location_id = get_global_location_id( data, mpi_isend->communicator,
                                                                              mpi_isend->receiver );

            if ( mpi_isend_receiver_location_id == receiverLocationID &&
                 mpi_isend->communicator        == communicator &&
                 mpi_isend->msg_tag             == msgTag &&
                 mpi_isend->msg_length          == msgLength )
            {
                /* buffer this matching isend event */
                matching_isends.insert( make_pair( mpi_isend->request_id, it ) );
                matching_isend_found = true;
            }
        }
        if ( ( *it )->type == OTF2_SNAPSHOTS_EVENT_MPI_ISEND_COMPLETE &&
             matching_isends.find( ( ( otf2_snapshots_event_mpi_isend_complete* )( *it ) )->request_id ) != matching_isends.end() )
        {
            list<otf2_snapshots_event*>::iterator it_isend = matching_isends[ ( ( otf2_snapshots_event_mpi_isend_complete* )( *it ) )->request_id ];

            /* delete matching OTF2_SNAPSHOTS_EVENT_MPI_ISEND */
            delete *it_isend;
            event_stack->erase( it_isend );

            /* delete OTF2_SNAPSHOTS_EVENT_MPI_ISEND_COMPLETE */
            delete *it;
            event_stack->erase( it );
            return true;
        }
        it++;
    }
    return false;
}


static void
match_pending_recvs( OTF2_LocationRef     senderLocationID,
                     otf2_snapshots_data* data )
{
    vector<otf2_snapshots_receive>* pending_recvs = &data->pending_recvs[ senderLocationID ];

    for ( vector<otf2_snapshots_receive>::iterator it = pending_recvs->begin();
          it != pending_recvs->end(); it++ )
    {
        if ( message_matching( &data->locations_event_stack[ senderLocationID ],
                               it->receiver_location_id,
                               it->communicator,
                               it->msg_tag,
                               it->msg_length,
                               data ) )
        {
            /* message was matched */

            /* erase message related events from receiver event list */

            if ( it->event_type == OTF2_SNAPSHOTS_EVENT_MPI_RECV )
            {
                erase_mpi_recv_event( &data->locations_event_stack[ it->receiver_location_id ],
                                      it->sender, it->communicator, it->msg_tag, it->msg_length );
            }
            else if ( it->event_type == OTF2_SNAPSHOTS_EVENT_MPI_IRECV )
            {
                /* erase OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST event */
                erase_event_from_stack( &data->locations_event_stack[ it->receiver_location_id ],
                                        OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST, it->request_id );

                /* erase OTF2_SNAPSHOTS_EVENT_MPI_IRECV event */
                erase_event_from_stack( &data->locations_event_stack[ it->receiver_location_id ],
                                        OTF2_SNAPSHOTS_EVENT_MPI_IRECV, it->request_id );
            }

            /* erase just matched rcv from 'pending_recvs' */

            it--;

            pending_recvs->erase( it + 1 );
        }
    }
}

/** @brief  get_active_region
 *
 *  @return returns true if a region was found
 */
static bool
get_active_region( otf2_snapshots_data* user_data,
                   OTF2_LocationRef     locationID,
                   OTF2_RegionRef*      region )
{
    // reading events starting at top of event stack
    //
    list<otf2_snapshots_event*>::reverse_iterator rit = user_data->locations_event_stack[ locationID ].rbegin();
    while ( rit != user_data->locations_event_stack[ locationID ].rend() )
    {
        if ( ( *rit )->type == OTF2_SNAPSHOTS_EVENT_ENTER )
        {
            *region = ( ( otf2_snapshots_event_enter* )( *rit ) )->region;
            return true;
        }
        rit++;
    }
    return false;
}

/** @brief sampling for region activity
 *
 *  @param user_data Self-defined user data.
 *  @param time      Current timestamp of an event.
 */
static void
region_activity_sampling( otf2_snapshots_data* user_data,
                          OTF2_TimeStamp       time )
{
    if ( user_data->tick_barrier_for_sample <= time &&
         user_data->sampling_counter < OTF2_SNAPSHOTS_NUMBER_SAMPLES )
    {
        /* Update sample */
        user_data->samples.assign( user_data->samples.size(), 0 );
        /* count number of processes that are executing a certain region */
        for ( vector< uint64_t>::iterator it = user_data->locations.begin();
              it != user_data->locations.end(); it++ )
        {
            OTF2_RegionRef active_region;
            if ( get_active_region( user_data, *it, &active_region ) )
            {
                user_data->samples[ user_data->region2sample[ active_region ] ]++;
            }
        }

        /*
         * Write new sample, and also sample which we missed
         * because no enter/leave happend
         */
        uint64_t number_missed = ( time - user_data->tick_barrier_for_sample ) / user_data->tick_rate_for_samples;
        for ( uint64_t i = 0; i <= number_missed; i++ )
        {
            OTF2_ThumbWriter_WriteSample( user_data->thumb_writer,
                                          user_data->locations.size(),
                                          user_data->samples.size(),
                                          &*user_data->samples.begin() );

            user_data->sampling_counter++;
        }

        /* update tick_barrier_for_sample */
        user_data->tick_barrier_for_sample += ( number_missed + 1 ) * user_data->tick_rate_for_samples;
    }
}


/** @brief Write location event stack to file.
 *
 *  @param location_event_stack Current active location event stack struct in the point of time where the break point is reached.
 *
 * @{
 */
static void
location_event_stack_write( OTF2_SnapWriter*            writerHandle,
                            list<otf2_snapshots_event*> location_event_stack,
                            OTF2_TimeStamp              break_time );

/** @} */



/** @brief Test whether a break should be performed for a given timestamp and
 *         the snapshots are written or not.
 *
 *  @param user_data Self-defined user data.
 *  @param time      Current timestamp of an event.
 */
static void
analyse_state( otf2_snapshots_data* user_data,
               OTF2_TimeStamp       time )
{
    uint64_t tick_barrier_for_break = OTF2_UNDEFINED_UINT64;

    /* Test, if its time to write a new snapshot. */
    bool leave = false;
    if ( time < user_data->tick_barrier_for_break )
    {
        return;
    }

    /* Set parameter for function adjust_break_time(...). */
    tick_barrier_for_break = user_data->tick_barrier_for_break;
    /* Update tick_barrier_for_break for the next snapshot */
    while ( user_data->tick_barrier_for_break <= time )
    {
        user_data->tick_barrier_for_break += user_data->tick_rate_for_breaks;
    }
    user_data->number_breaks++;

    /* Write a snapshot for each locationID. */
    if ( user_data->param->verbose )
    {
        printf( "\nWrite a new break for all locationID at break timestamp: %" PRIu64 "\n", tick_barrier_for_break );
    }
    for ( vector< uint64_t>::iterator it = user_data->locations.begin();
          it < user_data->locations.end(); ++it )
    {
        /* ___ Write snapshot to file ________________________________*/

        OTF2_SnapWriter* snap_writer = OTF2_Archive_GetSnapWriter( user_data->archive, *it );

        /* write SnaphotStart event */

        OTF2_SnapWriter_SnapshotStart( snap_writer,
                                       NULL,
                                       tick_barrier_for_break,
                                       user_data->locations_event_stack[ *it ].size() );

        location_event_stack_write( snap_writer, user_data->locations_event_stack[ *it ], tick_barrier_for_break );


        /* write SnaphotEnd event */

        //OTF2_EvtReader* evt_reader = OTF2_Archive_GetEvtReader( user_data->archive, *it );

        /* we can't use OTF2_Archive_GetEvtReader in write file mode... */

        OTF2_EvtReader* evt_reader = user_data->evt_readers[ *it ];

        uint64_t position;

        OTF2_EvtReader_GetPos( evt_reader, &position );

        OTF2_SnapWriter_SnapshotEnd( snap_writer, NULL, tick_barrier_for_break, position++ );
    }
}



/* ___ Implementation of global definition callbacks ________________________ */

extern "C"
{
/** @brief Global callback for definition.
 *         Please see OTF2_GlobDefReaderCallback_...() for a detailed
 *         description.
 * @{
 */
static OTF2_CallbackCode
global_def_callback_location( void*                 userData,
                              OTF2_LocationRef      self,
                              OTF2_StringRef        name,
                              OTF2_LocationType     locationType,
                              uint64_t              numberOfEvents,
                              OTF2_LocationGroupRef locationGroup )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;

    /* Open a definition reader and read all local definitions. */
    OTF2_DefReader* def_reader = OTF2_Archive_GetDefReader( data->archive, self );

    uint64_t definitions_read = 0;
    OTF2_DefReader_ReadDefinitions( def_reader, OTF2_UNDEFINED_UINT64, &definitions_read );

    /* Open an event reader to be able to read events with a global event reader */
    OTF2_EvtReader* evt_reader = OTF2_Archive_GetEvtReader( data->archive, self );

    /* buffer evt_reader */
    data->evt_readers.insert( make_pair( self, evt_reader ) );

    /* Add location to the list of locations and to a hash table to read/write events for the location. */
    data->locations.push_back( self );

    list<otf2_snapshots_event*> empty_stack;

    data->locations_event_stack.insert( make_pair( self, empty_stack ) );

    /* Count the number of events. */
    data->number_events += numberOfEvents;

    /* initialize measurement_on_off_record value for the new location*/
    data->measurement_on_off_record.insert( make_pair( self, false ) );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
global_def_callback_metric_class( void*                       userData,
                                  OTF2_MetricRef              self,
                                  uint8_t                     numberOfMetrics,
                                  const OTF2_MetricMemberRef* metricMembers,
                                  OTF2_MetricOccurrence       metricOccurrence,
                                  OTF2_RecorderKind           kind )
{
    /* copy metric members*/

    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;

    OTF2_MetricMemberRef* members = new OTF2_MetricMemberRef[ numberOfMetrics ];

    memcpy( members, metricMembers, numberOfMetrics * sizeof( OTF2_MetricMemberRef ) );

    data->metric_members.insert( make_pair( self, members ) );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
global_def_callback_metric_instance( void*            userData,
                                     OTF2_MetricRef   self,
                                     OTF2_MetricRef   metricClass,
                                     OTF2_LocationRef recorder,
                                     OTF2_MetricScope metricScope,
                                     uint64_t         scope )
{
    ( ( otf2_snapshots_data* )userData )->metric_instances.insert( make_pair( self, metricClass ) );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
global_def_callback_metric_member( void*                userData,
                                   OTF2_MetricMemberRef self,
                                   OTF2_StringRef       name,
                                   OTF2_StringRef       description,
                                   OTF2_MetricType      metricType,
                                   OTF2_MetricMode      metricMode,
                                   OTF2_Type            valueType,
                                   OTF2_MetricBase      metricBase,
                                   int64_t              exponent,
                                   OTF2_StringRef       unit )
{
    ( ( otf2_snapshots_data* )userData )->metric_modes.insert( make_pair( self, metricMode ) );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
global_def_callback_clock_properties( void*    userData,
                                      uint64_t timerResolution,
                                      uint64_t global_offset,
                                      uint64_t trace_length )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;

    data->global_offset    = global_offset;
    data->trace_length     = trace_length;
    data->timer_resolution = timerResolution;

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
global_def_callback_comm( void*          userData,
                          OTF2_CommRef   self,
                          OTF2_StringRef ame,
                          OTF2_GroupRef  group,
                          OTF2_CommRef   parent )
{
    otf2_snapshots_data* ud = ( otf2_snapshots_data* )userData;

    /* add Comm to global Comms */

    ud->mpi_comm2group.insert( make_pair( self, group ) );

    list< set<uint64_t> > empty_list;
    ud->mpi_collective_ends.insert( make_pair( self, empty_list ) );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
global_def_callback_group( void*           userData,
                           OTF2_GroupRef   self,
                           OTF2_StringRef  name,
                           OTF2_GroupType  groupType,
                           OTF2_Paradigm   paradigm,
                           OTF2_GroupFlag  groupFlags,
                           uint32_t        numberOfMembers,
                           const uint64_t* members )
{
    otf2_snapshots_data* ud = ( otf2_snapshots_data* )userData;

    /* copy group to vector */
    vector<uint64_t> members_copy( numberOfMembers, 0 );
    for ( uint32_t i = 0; i < numberOfMembers; i++ )
    {
        members_copy[ i ] = members[ i ];
    }

    /* add copy to global groups */
    ud->group_members.insert( make_pair( self, members_copy ) );

    /* catch mpi_location_group */
    if ( groupType == OTF2_GROUP_TYPE_COMM_LOCATIONS
         && paradigm == OTF2_PARADIGM_MPI )
    {
        ud->mpi_location_group_id = self;
    }

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_def_callback_region( void*           userData,
                            OTF2_RegionRef  self,
                            OTF2_StringRef  name,
                            OTF2_StringRef  canonicalName,
                            OTF2_StringRef  description,
                            OTF2_RegionRole regionRole,
                            OTF2_Paradigm   paradigm,
                            OTF2_RegionFlag regionFlags,
                            OTF2_StringRef  sourceFile,
                            uint32_t        beginLineNumber,
                            uint32_t        endLineNumber )
{
    otf2_snapshots_data* ud = ( otf2_snapshots_data* )userData;

    ud->region2sample.insert( make_pair( self, ( uint32_t )ud->region_defs.size() ) );
    ud->region_defs.push_back( self );

    return OTF2_CALLBACK_SUCCESS;
}
}

/** @} */


/*___ stack event callbacks._____________________________________________ */

extern "C"
{
static OTF2_CallbackCode
global_evt_callback_mpi_recv( uint64_t            locationID,
                              uint64_t            time,
                              void*               userData,
                              OTF2_AttributeList* attributes,
                              uint32_t            sender,
                              OTF2_CommRef        communicator,
                              uint32_t            msgTag,
                              uint64_t            msgLength )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );

    // map MPI rank of sender to global location ID
    //
    uint64_t sender_global_lid = get_global_location_id( data, communicator, sender );

    // erase matching send events in stack of sender
    //
    if ( !message_matching( &data->locations_event_stack[ sender_global_lid ],
                            locationID, communicator, msgTag, msgLength, data ) )
    {
        /* -> recv wasn't matched */

        /* -> buffer unmatched receive */
        otf2_snapshots_receive receive;

        receive.event_type           = OTF2_SNAPSHOTS_EVENT_MPI_RECV;
        receive.receiver_location_id = locationID;
        receive.sender               = sender;
        receive.communicator         = communicator;
        receive.msg_tag              = msgTag;
        receive.msg_length           = msgLength;

        data->pending_recvs[ sender_global_lid ].push_back( receive );

        /* write recv event to receiver event buffer */
        otf2_snapshots_event_mpi_recv* event =
            new otf2_snapshots_event_mpi_recv( time,
                                               attributes,
                                               sender,
                                               communicator,
                                               msgTag,
                                               msgLength );

        data->locations_event_stack[ locationID ].push_back( event );
    }

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_mpi_irecv( uint64_t            locationID,
                               uint64_t            time,
                               void*               userData,
                               OTF2_AttributeList* attributes,
                               uint32_t            sender,
                               OTF2_CommRef        communicator,
                               uint32_t            msgTag,
                               uint64_t            msgLength,
                               uint64_t            requestID )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );

    // map MPI rank of sender to global location ID
    //
    uint64_t sender_global_lid = get_global_location_id( data, communicator, sender );

    // erase matching send events in stack of sender
    //
    if ( !message_matching( &data->locations_event_stack[ sender_global_lid ],
                            locationID, communicator, msgTag, msgLength, data ) )
    {
        /* -> irecv wasn't matched */

        /* -> buffer unmatched irecv */
        otf2_snapshots_receive receive;

        receive.event_type           = OTF2_SNAPSHOTS_EVENT_MPI_IRECV;
        receive.request_id           = requestID;
        receive.receiver_location_id = locationID;
        receive.communicator         = communicator;
        receive.msg_tag              = msgTag;
        receive.msg_length           = msgLength;

        data->pending_recvs[ sender_global_lid ].push_back( receive );

        /* write irecv event to receiver event buffer */
        otf2_snapshots_event_mpi_irecv* event =
            new otf2_snapshots_event_mpi_irecv( time,
                                                attributes,
                                                sender,
                                                communicator,
                                                msgTag,
                                                msgLength,
                                                requestID );

        data->locations_event_stack[ locationID ].push_back( event );
    }
    else
    {
        // erase irecv request from mpi_requests
        //
        data->mpi_requests.erase( requestID );
        erase_event_from_stack( &data->locations_event_stack[ locationID ],
                                OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST,
                                requestID );
    }

    return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
global_evt_callback_metric( uint64_t                locationID,
                            uint64_t                time,
                            void*                   userData,
                            OTF2_AttributeList*     attributes,
                            OTF2_MetricRef          metric,
                            uint8_t                 numberOfMetrics,
                            const OTF2_Type*        typeIDs,
                            const OTF2_MetricValue* metricValues )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );

    list<otf2_snapshots_event*>* event_stack = &data->locations_event_stack[ locationID ];

    /* If this metric is already on the stack, then update metric values and change event position on stack. */
    for ( list<otf2_snapshots_event*>::iterator it = event_stack->begin();
          it != event_stack->end();
          it++ )
    {
        if ( ( *it )->type == OTF2_SNAPSHOTS_EVENT_METRIC  &&
             ( ( otf2_snapshots_event_metric* )( *it ) )->metric == metric )
        {
            OTF2_MetricRef metric_class = metric;

            /* check for metric instance */
            map<OTF2_MetricRef, OTF2_MetricRef>::iterator it_mi = data->metric_instances.find( metric );
            if ( it_mi != data->metric_instances.end() )
            {
                metric_class = it_mi->second;
            }

            /* update values */
            for ( uint8_t i = 0; i < numberOfMetrics; i++ )
            {
                /* get OTF2_MetricMode of current metric member */
                OTF2_MetricMemberRef metric_member_ref = data->metric_members[ metric_class ][ i ];
                OTF2_MetricMode      metric_mode       = data->metric_modes[ metric_member_ref ];

                if ( ( metric_mode & OTF2_METRIC_VALUE_MASK ) == OTF2_METRIC_VALUE_RELATIVE )
                {
                    /* compute new metric value future values should relate on:
                     * new value = value to relate on (last) + current value */
                    switch ( typeIDs[ i ] )
                    {
                        case OTF2_TYPE_INT64:
                            ( ( otf2_snapshots_event_metric* )( *it ) )->metric_values[ i ].signed_int += metricValues[ i ].signed_int;
                            break;
                        case OTF2_TYPE_UINT64:
                            ( ( otf2_snapshots_event_metric* )( *it ) )->metric_values[ i ].unsigned_int += metricValues[ i ].unsigned_int;
                            break;
                        case OTF2_TYPE_DOUBLE:
                            ( ( otf2_snapshots_event_metric* )( *it ) )->metric_values[ i ].floating_point += metricValues[ i ].floating_point;
                            break;
                    }
                }
                else
                {
                    /* adopt new value */
                    ( ( otf2_snapshots_event_metric* )( *it ) )->metric_values[ i ] = metricValues[ i ];
                }
            }

            /* update event position in stack */
            event_stack->push_back( *it );
            event_stack->erase( it );

            //cout << "metric event for location " << locationID << " with ref " << metric << endl;
            //print_stack( &data->locations_event_stack[ locationID ] );

            return OTF2_CALLBACK_SUCCESS;
        }
    }

    /* else push a new metric event on stack */
    otf2_snapshots_event_metric* event =
        new otf2_snapshots_event_metric( time,
                                         attributes,
                                         metric,
                                         numberOfMetrics,
                                         typeIDs,
                                         metricValues );

    data->locations_event_stack[ locationID ].push_back( event );

    //cout << "metric event for location " << locationID << " with ref " << metric << endl;
    //print_stack( &data->locations_event_stack[ locationID ] );

    return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
global_evt_callback_mpi_collective_end( uint64_t            locationID,
                                        uint64_t            time,
                                        void*               userData,
                                        OTF2_AttributeList* attributes,
                                        OTF2_CollectiveOp   collectiveOp,
                                        OTF2_CommRef        communicator,
                                        uint32_t            root,
                                        uint64_t            sizeSent,
                                        uint64_t            sizeReceived )
{
    otf2_snapshots_data* data = ( otf2_snapshots_data* )userData;
    analyse_state( data, time );

    /* ___ buffer rank that finished the collective __________________ */

    list< set<uint64_t> >* sets = &data->mpi_collective_ends[ communicator ];
    {
        list< set<uint64_t> >::iterator it = sets->begin();

        /* try to insert rank into existing sets */
        while ( it != sets->end() )
        {
            pair< set< uint64_t >:: iterator, bool > ret;
            ret = it->insert( locationID );
            if ( ret.second == true )
            {
                break;
            }
            it++;
        }

        if ( it == sets->end() )
        {
            /* insert rank into new set */
            set<uint64_t> s;
            s.insert( locationID );
            sets->push_back( s );
        }
    }

    // get mpi group for the communicator
    //
    vector< uint64_t>* mpi_group = &data->group_members[ data->mpi_comm2group[ communicator ] ];

    // anyone not yet completed this collective?
    //
    if ( data->mpi_collective_ends[ communicator ].front().size() < mpi_group->size() )
    {
        /* just write an mpi_collective_end snap record for the location */

        otf2_snapshots_event_mpi_collective_end* event =
            new otf2_snapshots_event_mpi_collective_end( time,
                                                         attributes,
                                                         collectiveOp,
                                                         communicator,
                                                         root,
                                                         sizeSent,
                                                         sizeReceived );

        data->locations_event_stack[ locationID ].push_back( event );
    }
    else
    {
        // for each rank of the mpi group
        //
        for ( vector< uint64_t>::iterator it = mpi_group->begin();
              it != mpi_group->end();
              it++ )
        {
            // get location id
            //
            uint64_t coll_loc_id = data->group_members[ data->mpi_location_group_id ][ *it ];

            //  remove events related to this collective
            //
            erase_mpi_collective_events( &data->locations_event_stack[ coll_loc_id ],
                                         communicator );
        }

        /* erase full set of ranks to update the current collective */
        sets->pop_front();
    }

    return OTF2_CALLBACK_SUCCESS;
}
} // extern c

#include "otf2_snapshots_callbacks_inc.cpp"
