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

/*___derived stack event class con- and destructors._____________________________________________ */


otf2_snapshots_event_measurement_on_off::
otf2_snapshots_event_measurement_on_off( uint64_t             time,
                                         OTF2_AttributeList*  attributes,
                                         OTF2_MeasurementMode measurementMode )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MEASUREMENT_ON_OFF, time, attributes )
{
    /* initialize event specific variables */

    this->measurement_mode = measurementMode;
};


otf2_snapshots_event_enter::
otf2_snapshots_event_enter( uint64_t            time,
                            OTF2_AttributeList* attributes,
                            OTF2_RegionRef      region )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_ENTER, time, attributes )
{
    /* initialize event specific variables */

    this->region = region;
};


otf2_snapshots_event_mpi_send::
otf2_snapshots_event_mpi_send( uint64_t            time,
                               OTF2_AttributeList* attributes,
                               uint32_t            receiver,
                               OTF2_CommRef        communicator,
                               uint32_t            msgTag,
                               uint64_t            msgLength )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MPI_SEND, time, attributes )
{
    /* initialize event specific variables */

    this->receiver     = receiver;
    this->communicator = communicator;
    this->msg_tag      = msgTag;
    this->msg_length   = msgLength;
};


otf2_snapshots_event_mpi_isend::
otf2_snapshots_event_mpi_isend( uint64_t            time,
                                OTF2_AttributeList* attributes,
                                uint32_t            receiver,
                                OTF2_CommRef        communicator,
                                uint32_t            msgTag,
                                uint64_t            msgLength,
                                uint64_t            requestID )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MPI_ISEND, time, attributes )
{
    /* initialize event specific variables */

    this->receiver     = receiver;
    this->communicator = communicator;
    this->msg_tag      = msgTag;
    this->msg_length   = msgLength;
    this->request_id   = requestID;
};


otf2_snapshots_event_mpi_isend_complete::
otf2_snapshots_event_mpi_isend_complete( uint64_t            time,
                                         OTF2_AttributeList* attributes,
                                         uint64_t            requestID )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MPI_ISEND_COMPLETE, time, attributes )
{
    /* initialize event specific variables */

    this->request_id = requestID;
};


otf2_snapshots_event_mpi_irecv_request::
otf2_snapshots_event_mpi_irecv_request( uint64_t            time,
                                        OTF2_AttributeList* attributes,
                                        uint64_t            requestID )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MPI_IRECV_REQUEST, time, attributes )
{
    /* initialize event specific variables */

    this->request_id = requestID;
};


otf2_snapshots_event_mpi_recv::
otf2_snapshots_event_mpi_recv( uint64_t            time,
                               OTF2_AttributeList* attributes,
                               uint32_t            sender,
                               OTF2_CommRef        communicator,
                               uint32_t            msgTag,
                               uint64_t            msgLength )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MPI_RECV, time, attributes )
{
    /* initialize event specific variables */

    this->sender       = sender;
    this->communicator = communicator;
    this->msg_tag      = msgTag;
    this->msg_length   = msgLength;
};


otf2_snapshots_event_mpi_irecv::
otf2_snapshots_event_mpi_irecv( uint64_t            time,
                                OTF2_AttributeList* attributes,
                                uint32_t            sender,
                                OTF2_CommRef        communicator,
                                uint32_t            msgTag,
                                uint64_t            msgLength,
                                uint64_t            requestID )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MPI_IRECV, time, attributes )
{
    /* initialize event specific variables */

    this->sender       = sender;
    this->communicator = communicator;
    this->msg_tag      = msgTag;
    this->msg_length   = msgLength;
    this->request_id   = requestID;
};


otf2_snapshots_event_mpi_collective_begin::
otf2_snapshots_event_mpi_collective_begin( uint64_t            time,
                                           OTF2_AttributeList* attributes  )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_BEGIN, time, attributes )
{
    /* initialize event specific variables */
};


otf2_snapshots_event_mpi_collective_end::
otf2_snapshots_event_mpi_collective_end( uint64_t            time,
                                         OTF2_AttributeList* attributes,
                                         OTF2_CollectiveOp   collectiveOp,
                                         OTF2_CommRef        communicator,
                                         uint32_t            root,
                                         uint64_t            sizeSent,
                                         uint64_t            sizeReceived )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_MPI_COLLECTIVE_END, time, attributes )
{
    /* initialize event specific variables */

    this->collective_op = collectiveOp;
    this->communicator  = communicator;
    this->root          = root;
    this->size_sent     = sizeSent;
    this->size_received = sizeReceived;
};


otf2_snapshots_event_omp_fork::
otf2_snapshots_event_omp_fork( uint64_t            time,
                               OTF2_AttributeList* attributes,
                               uint32_t            numberOfRequestedThreads )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_OMP_FORK, time, attributes )
{
    /* initialize event specific variables */

    this->number_of_requested_threads = numberOfRequestedThreads;
};


otf2_snapshots_event_omp_acquire_lock::
otf2_snapshots_event_omp_acquire_lock( uint64_t            time,
                                       OTF2_AttributeList* attributes,
                                       uint32_t            lockID,
                                       uint32_t            acquisitionOrder )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_OMP_ACQUIRE_LOCK, time, attributes )
{
    /* initialize event specific variables */

    this->lock_id           = lockID;
    this->acquisition_order = acquisitionOrder;
};


otf2_snapshots_event_omp_task_create::
otf2_snapshots_event_omp_task_create( uint64_t            time,
                                      OTF2_AttributeList* attributes,
                                      uint64_t            taskID )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_OMP_TASK_CREATE, time, attributes )
{
    /* initialize event specific variables */

    this->task_id = taskID;
};


otf2_snapshots_event_omp_task_switch::
otf2_snapshots_event_omp_task_switch( uint64_t            time,
                                      OTF2_AttributeList* attributes,
                                      uint64_t            taskID )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_OMP_TASK_SWITCH, time, attributes )
{
    /* initialize event specific variables */

    this->task_id = taskID;
};


otf2_snapshots_event_metric::
otf2_snapshots_event_metric( uint64_t                time,
                             OTF2_AttributeList*     attributes,
                             OTF2_MetricRef          metric,
                             uint8_t                 numberOfMetrics,
                             const OTF2_Type*        typeIDs,
                             const OTF2_MetricValue* metricValues )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_METRIC, time, attributes )
{
    /* initialize event specific variables */

    this->metric            = metric;
    this->number_of_metrics = numberOfMetrics;
    this->type_ids          = new OTF2_Type[ number_of_metrics ];
    memcpy( this->type_ids,
            type_ids,
            number_of_metrics * sizeof( OTF2_Type ) );
    this->metric_values = new OTF2_MetricValue[ number_of_metrics ];
    memcpy( this->metric_values,
            metric_values,
            number_of_metrics * sizeof( OTF2_MetricValue ) );
};


otf2_snapshots_event_metric::
~otf2_snapshots_event_metric()
{
    delete[] this->type_ids;
    delete[] this->metric_values;
}


otf2_snapshots_event_parameter_string::
otf2_snapshots_event_parameter_string( uint64_t            time,
                                       OTF2_AttributeList* attributes,
                                       OTF2_ParameterRef   parameter,
                                       OTF2_StringRef      string )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_PARAMETER_STRING, time, attributes )
{
    /* initialize event specific variables */

    this->parameter = parameter;
    this->string    = string;
};


otf2_snapshots_event_parameter_int::
otf2_snapshots_event_parameter_int( uint64_t            time,
                                    OTF2_AttributeList* attributes,
                                    OTF2_ParameterRef   parameter,
                                    int64_t             value )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_PARAMETER_INT, time, attributes )
{
    /* initialize event specific variables */

    this->parameter = parameter;
    this->value     = value;
};


otf2_snapshots_event_parameter_unsigned_int::
otf2_snapshots_event_parameter_unsigned_int( uint64_t            time,
                                             OTF2_AttributeList* attributes,
                                             OTF2_ParameterRef   parameter,
                                             uint64_t            value )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_PARAMETER_UNSIGNED_INT, time, attributes )
{
    /* initialize event specific variables */

    this->parameter = parameter;
    this->value     = value;
};
