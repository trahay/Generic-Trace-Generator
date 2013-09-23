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

#ifndef OTF2_SNAPSHOTS_H
#define OTF2_SNAPSHOTS_H

#include <vector>
#include <map>
#include <list>
#include <set>

#include "otf2_snapshots_events.hpp"
#include "otf2_snapshots_parameters.hpp"


/** @brief used to buffer temporary unmatched receive events
 *
 *
 */
struct otf2_snapshots_receive
{
    otf2_snapshots_event_type event_type;
    uint64_t                  receiver_location_id;
    uint32_t                  sender;
    OTF2_CommRef              communicator;
    uint32_t                  msg_tag;
    uint64_t                  msg_length;
    uint64_t                  request_id;
};


/** @brief Keeps all data for the callbacks. */
struct otf2_snapshots_data
{
    /** @brief Archive handle. */
    OTF2_Archive*                                         archive;
    /** @brief List of locations to process. */
    std::vector<uint64_t>                                 locations;
    /** @brief global Offset. */
    uint64_t                                              global_offset;
    /** @brief trace Length. */
    uint64_t                                              trace_length;
    /** @brief Global timer resolution, only necessary if it's the same for all. */
    uint64_t                                              timer_resolution;
    /** @brief Number of break points, which will be used to generate snapshots. */
    uint64_t                                              number_breaks;
    /** @brief Number of ticks between every two breaks. */
    uint64_t                                              tick_rate_for_breaks;
    /** @brief Barrier for the next break in ticks. */
    uint64_t                                              tick_barrier_for_break;
    /** @brief Accumulated number of events for all locations. */
    uint64_t                                              number_events;
    /** @brief Hashtab of locations with pointer to their callstack as values used to save snapshot informations. */
    std::map<uint64_t, std::list<otf2_snapshots_event*> > locations_event_stack;

    std::map<OTF2_MetricRef, OTF2_MetricMemberRef*>       metric_members;

    std::map<OTF2_MetricMemberRef, OTF2_MetricMode>       metric_modes;

    /** @brief maps metric instances to instanced metric class */
    std::map<OTF2_MetricRef, OTF2_MetricRef> metric_instances;

    /** @brief Pointer to the user defined command line options. */
    otf2_snapshots_param* param;

    /** @brief data used for mapping mpi ranks to global location ids */
    std::map<uint32_t, uint32_t>                mpi_comm2group;
    std::map<uint32_t, std::vector< uint64_t> > group_members;
    uint32_t                                    mpi_location_group_id;

    /** @brief unmatched received messages */
    std::map<uint64_t, std::vector<otf2_snapshots_receive> > pending_recvs;

    std::map<OTF2_CommRef, std::list< std::set<uint64_t> > > mpi_collective_ends;


    /** @brief mapping of omp tasks to the location where they were created */
    std::map<uint64_t, uint64_t> omp_task_creator;

    /** @brief mapping of mpi request to kind of request (irecv/isend) for better handling
     * of the request cancels */
    std::map<uint64_t, otf2_snapshots_event_type> mpi_requests;


    /** @brief Number of ticks between every two thumbnail samples. */
    uint64_t                       tick_rate_for_samples;
    /** @brief Barrier for the next break in ticks. */
    uint64_t                       tick_barrier_for_sample;
    /** @brief sample vector */
    std::vector<uint64_t>          samples;
    std::vector<uint64_t>          region_defs;
    std::map< uint32_t, uint32_t > region2sample;
    uint32_t                       sampling_counter;
    OTF2_ThumbWriter*              thumb_writer;

    /** for switching file mode from read to write */
    std::map< OTF2_LocationRef, OTF2_EvtReader*> evt_readers;

    /** maps OTF2_LocationRef to true when event stack contains a MeasurementOnOffRecord */
    std::map< OTF2_LocationRef, bool > measurement_on_off_record;
};


#endif /* OTF2_SNAPSHOTS_H */
