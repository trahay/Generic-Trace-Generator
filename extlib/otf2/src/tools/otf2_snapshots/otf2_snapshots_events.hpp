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

#ifndef OTF2_SNAPSHOTS_EVENTS_H
#define OTF2_SNAPSHOTS_EVENTS_H


enum otf2_snapshots_event_type
{
    OTF2_SNAPSHOTS_EVENT_MEASUREMENT_ON_OFF     = 11,
    OTF2_SNAPSHOTS_EVENT_ENTER                  = 12,
    OTF2_SNAPSHOTS_EVENT_MPI_SEND               = 14,
    OTF2_SNAPSHOTS_EVENT_MPI_ISEND              = 15,
    OTF2_SNAPSHOTS_EVENT_MPI_ISEND_COMPLETE     = 16,
    OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST      = 17,
    OTF2_SNAPSHOTS_EVENT_MPI_RECV               = 18,
    OTF2_SNAPSHOTS_EVENT_MPI_IRECV              = 19,
    OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_BEGIN   = 22,
    OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_END     = 23,
    OTF2_SNAPSHOTS_EVENT_OMP_FORK               = 24,
    OTF2_SNAPSHOTS_EVENT_OMP_ACQUIRE_LOCK       = 26,
    OTF2_SNAPSHOTS_EVENT_OMP_TASK_CREATE        = 28,
    OTF2_SNAPSHOTS_EVENT_OMP_TASK_SWITCH        = 29,
    OTF2_SNAPSHOTS_EVENT_METRIC                 = 31,
    OTF2_SNAPSHOTS_EVENT_PARAMETER_STRING       = 32,
    OTF2_SNAPSHOTS_EVENT_PARAMETER_INT          = 33,
    OTF2_SNAPSHOTS_EVENT_PARAMETER_UNSIGNED_INT = 34,
};

class otf2_snapshots_event
{
public:

    otf2_snapshots_event_type type;
    uint64_t                  time;
    OTF2_AttributeList*       attributes;

    otf2_snapshots_event( otf2_snapshots_event_type type,
                          uint64_t                  time,
                          OTF2_AttributeList*       attributes );
    virtual
    ~otf2_snapshots_event();
};

/*___derived event classes._____________________________________________ */


class otf2_snapshots_event_measurement_on_off : public otf2_snapshots_event
{
public:

    OTF2_MeasurementMode measurement_mode;

    otf2_snapshots_event_measurement_on_off( uint64_t             time,
                                             OTF2_AttributeList*  attributes,
                                             OTF2_MeasurementMode measurementMode );
};


class otf2_snapshots_event_enter : public otf2_snapshots_event
{
public:

    OTF2_RegionRef region;

    otf2_snapshots_event_enter( uint64_t            time,
                                OTF2_AttributeList* attributes,
                                OTF2_RegionRef      region );
};


class otf2_snapshots_event_mpi_send : public otf2_snapshots_event
{
public:

    uint32_t     receiver;
    OTF2_CommRef communicator;
    uint32_t     msg_tag;
    uint64_t     msg_length;

    otf2_snapshots_event_mpi_send( uint64_t            time,
                                   OTF2_AttributeList* attributes,
                                   uint32_t            receiver,
                                   OTF2_CommRef        communicator,
                                   uint32_t            msgTag,
                                   uint64_t            msgLength );
};


class otf2_snapshots_event_mpi_isend : public otf2_snapshots_event
{
public:

    uint32_t     receiver;
    OTF2_CommRef communicator;
    uint32_t     msg_tag;
    uint64_t     msg_length;
    uint64_t     request_id;

    otf2_snapshots_event_mpi_isend( uint64_t            time,
                                    OTF2_AttributeList* attributes,
                                    uint32_t            receiver,
                                    OTF2_CommRef        communicator,
                                    uint32_t            msgTag,
                                    uint64_t            msgLength,
                                    uint64_t            requestID );
};


class otf2_snapshots_event_mpi_isend_complete : public otf2_snapshots_event
{
public:

    uint64_t request_id;

    otf2_snapshots_event_mpi_isend_complete( uint64_t            time,
                                             OTF2_AttributeList* attributes,
                                             uint64_t            requestID );
};


class otf2_snapshots_event_mpi_irecv_request : public otf2_snapshots_event
{
public:

    uint64_t request_id;

    otf2_snapshots_event_mpi_irecv_request( uint64_t            time,
                                            OTF2_AttributeList* attributes,
                                            uint64_t            requestID );
};


class otf2_snapshots_event_mpi_recv : public otf2_snapshots_event
{
public:

    uint32_t     sender;
    OTF2_CommRef communicator;
    uint32_t     msg_tag;
    uint64_t     msg_length;

    otf2_snapshots_event_mpi_recv( uint64_t            time,
                                   OTF2_AttributeList* attributes,
                                   uint32_t            sender,
                                   OTF2_CommRef        communicator,
                                   uint32_t            msgTag,
                                   uint64_t            msgLength );
};


class otf2_snapshots_event_mpi_irecv : public otf2_snapshots_event
{
public:

    uint32_t     sender;
    OTF2_CommRef communicator;
    uint32_t     msg_tag;
    uint64_t     msg_length;
    uint64_t     request_id;

    otf2_snapshots_event_mpi_irecv( uint64_t            time,
                                    OTF2_AttributeList* attributes,
                                    uint32_t            sender,
                                    OTF2_CommRef        communicator,
                                    uint32_t            msgTag,
                                    uint64_t            msgLength,
                                    uint64_t            requestID );
};


class otf2_snapshots_event_mpi_collective_begin : public otf2_snapshots_event
{
public:


    otf2_snapshots_event_mpi_collective_begin( uint64_t            time,
                                               OTF2_AttributeList* attributes  );
};


class otf2_snapshots_event_mpi_collective_end : public otf2_snapshots_event
{
public:

    OTF2_CollectiveOp collective_op;
    OTF2_CommRef      communicator;
    uint32_t          root;
    uint64_t          size_sent;
    uint64_t          size_received;

    otf2_snapshots_event_mpi_collective_end( uint64_t            time,
                                             OTF2_AttributeList* attributes,
                                             OTF2_CollectiveOp   collectiveOp,
                                             OTF2_CommRef        communicator,
                                             uint32_t            root,
                                             uint64_t            sizeSent,
                                             uint64_t            sizeReceived );
};


class otf2_snapshots_event_omp_fork : public otf2_snapshots_event
{
public:

    uint32_t number_of_requested_threads;

    otf2_snapshots_event_omp_fork( uint64_t            time,
                                   OTF2_AttributeList* attributes,
                                   uint32_t            numberOfRequestedThreads );
};


class otf2_snapshots_event_omp_acquire_lock : public otf2_snapshots_event
{
public:

    uint32_t lock_id;
    uint32_t acquisition_order;

    otf2_snapshots_event_omp_acquire_lock( uint64_t            time,
                                           OTF2_AttributeList* attributes,
                                           uint32_t            lockID,
                                           uint32_t            acquisitionOrder );
};


class otf2_snapshots_event_omp_task_create : public otf2_snapshots_event
{
public:

    uint64_t task_id;

    otf2_snapshots_event_omp_task_create( uint64_t            time,
                                          OTF2_AttributeList* attributes,
                                          uint64_t            taskID );
};


class otf2_snapshots_event_omp_task_switch : public otf2_snapshots_event
{
public:

    uint64_t task_id;

    otf2_snapshots_event_omp_task_switch( uint64_t            time,
                                          OTF2_AttributeList* attributes,
                                          uint64_t            taskID );
};


class otf2_snapshots_event_metric : public otf2_snapshots_event
{
public:

    OTF2_MetricRef    metric;
    uint8_t           number_of_metrics;
    OTF2_Type*        type_ids;
    OTF2_MetricValue* metric_values;

    otf2_snapshots_event_metric( uint64_t                time,
                                 OTF2_AttributeList*     attributes,
                                 OTF2_MetricRef          metric,
                                 uint8_t                 numberOfMetrics,
                                 const OTF2_Type*        typeIDs,
                                 const OTF2_MetricValue* metricValues );

    ~otf2_snapshots_event_metric();
};


class otf2_snapshots_event_parameter_string : public otf2_snapshots_event
{
public:

    OTF2_ParameterRef parameter;
    OTF2_StringRef    string;

    otf2_snapshots_event_parameter_string( uint64_t            time,
                                           OTF2_AttributeList* attributes,
                                           OTF2_ParameterRef   parameter,
                                           OTF2_StringRef      string );
};


class otf2_snapshots_event_parameter_int : public otf2_snapshots_event
{
public:

    OTF2_ParameterRef parameter;
    int64_t           value;

    otf2_snapshots_event_parameter_int( uint64_t            time,
                                        OTF2_AttributeList* attributes,
                                        OTF2_ParameterRef   parameter,
                                        int64_t             value );
};


class otf2_snapshots_event_parameter_unsigned_int : public otf2_snapshots_event
{
public:

    OTF2_ParameterRef parameter;
    uint64_t          value;

    otf2_snapshots_event_parameter_unsigned_int( uint64_t            time,
                                                 OTF2_AttributeList* attributes,
                                                 OTF2_ParameterRef   parameter,
                                                 uint64_t            value );
};


#endif /* OTF2_SNAPSHOTS_EVENTS_H */
