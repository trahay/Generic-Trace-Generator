/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
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


#ifndef OTF2_INTERNAL_EVENTS_H
#define OTF2_INTERNAL_EVENTS_H


/**
 *  @file       src/otf2_events_int.h
 *  @source     templates/otf2_events_int.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This file list all event records supported in OTF2.
 */


typedef uint8_t OTF2_EventType;


enum OTF2_EventType_enum
{
    /* First five entries are reserved for OTF2 internals */

    OTF2_EVENT_BUFFER_FLUSH                 = 10, /** Event record identifier for the BufferFlush event. */
    OTF2_EVENT_MEASUREMENT_ON_OFF           = 11, /** Event record identifier for the MeasurementOnOff event. */
    OTF2_EVENT_ENTER                        = 12, /** Event record identifier for the Enter event. */
    OTF2_EVENT_LEAVE                        = 13, /** Event record identifier for the Leave event. */
    OTF2_EVENT_MPI_SEND                     = 14, /** Event record identifier for the MpiSend event. */
    OTF2_EVENT_MPI_ISEND                    = 15, /** Event record identifier for the MpiIsend event. */
    OTF2_EVENT_MPI_ISEND_COMPLETE           = 16, /** Event record identifier for the MpiIsendComplete event. */
    OTF2_EVENT_MPI_IRECV_REQUEST            = 17, /** Event record identifier for the MpiIrecvRequest event. */
    OTF2_EVENT_MPI_RECV                     = 18, /** Event record identifier for the MpiRecv event. */
    OTF2_EVENT_MPI_IRECV                    = 19, /** Event record identifier for the MpiIrecv event. */
    OTF2_EVENT_MPI_REQUEST_TEST             = 20, /** Event record identifier for the MpiRequestTest event. */
    OTF2_EVENT_MPI_REQUEST_CANCELLED        = 21, /** Event record identifier for the MpiRequestCancelled event. */
    OTF2_EVENT_MPI_COLLECTIVE_BEGIN         = 22, /** Event record identifier for the MpiCollectiveBegin event. */
    OTF2_EVENT_MPI_COLLECTIVE_END           = 23, /** Event record identifier for the MpiCollectiveEnd event. */
    OTF2_EVENT_OMP_FORK                     = 24, /** Event record identifier for the OmpFork event. */
    OTF2_EVENT_OMP_JOIN                     = 25, /** Event record identifier for the OmpJoin event. */
    OTF2_EVENT_OMP_ACQUIRE_LOCK             = 26, /** Event record identifier for the OmpAcquireLock event. */
    OTF2_EVENT_OMP_RELEASE_LOCK             = 27, /** Event record identifier for the OmpReleaseLock event. */
    OTF2_EVENT_OMP_TASK_CREATE              = 28, /** Event record identifier for the OmpTaskCreate event. */
    OTF2_EVENT_OMP_TASK_SWITCH              = 29, /** Event record identifier for the OmpTaskSwitch event. */
    OTF2_EVENT_OMP_TASK_COMPLETE            = 30, /** Event record identifier for the OmpTaskComplete event. */
    OTF2_EVENT_METRIC                       = 31, /** Event record identifier for the Metric event. */
    OTF2_EVENT_PARAMETER_STRING             = 32, /** Event record identifier for the ParameterString event. */
    OTF2_EVENT_PARAMETER_INT                = 33, /** Event record identifier for the ParameterInt event. */
    OTF2_EVENT_PARAMETER_UNSIGNED_INT       = 34, /** Event record identifier for the ParameterUnsignedInt event. */
    OTF2_EVENT_RMA_WIN_CREATE               = 35, /** Event record identifier for the RmaWinCreate event. */
    OTF2_EVENT_RMA_WIN_DESTROY              = 36, /** Event record identifier for the RmaWinDestroy event. */
    OTF2_EVENT_RMA_COLLECTIVE_BEGIN         = 37, /** Event record identifier for the RmaCollectiveBegin event. */
    OTF2_EVENT_RMA_COLLECTIVE_END           = 38, /** Event record identifier for the RmaCollectiveEnd event. */
    OTF2_EVENT_RMA_GROUP_SYNC               = 39, /** Event record identifier for the RmaGroupSync event. */
    OTF2_EVENT_RMA_REQUEST_LOCK             = 40, /** Event record identifier for the RmaRequestLock event. */
    OTF2_EVENT_RMA_ACQUIRE_LOCK             = 41, /** Event record identifier for the RmaAcquireLock event. */
    OTF2_EVENT_RMA_TRY_LOCK                 = 42, /** Event record identifier for the RmaTryLock event. */
    OTF2_EVENT_RMA_RELEASE_LOCK             = 43, /** Event record identifier for the RmaReleaseLock event. */
    OTF2_EVENT_RMA_SYNC                     = 44, /** Event record identifier for the RmaSync event. */
    OTF2_EVENT_RMA_WAIT_CHANGE              = 45, /** Event record identifier for the RmaWaitChange event. */
    OTF2_EVENT_RMA_PUT                      = 46, /** Event record identifier for the RmaPut event. */
    OTF2_EVENT_RMA_GET                      = 47, /** Event record identifier for the RmaGet event. */
    OTF2_EVENT_RMA_ATOMIC                   = 48, /** Event record identifier for the RmaAtomic event. */
    OTF2_EVENT_RMA_OP_COMPLETE_BLOCKING     = 49, /** Event record identifier for the RmaOpCompleteBlocking event. */
    OTF2_EVENT_RMA_OP_COMPLETE_NON_BLOCKING = 50, /** Event record identifier for the RmaOpCompleteNonBlocking event. */
    OTF2_EVENT_RMA_OP_TEST                  = 51, /** Event record identifier for the RmaOpTest event. */
    OTF2_EVENT_RMA_OP_COMPLETE_REMOTE       = 52, /** Event record identifier for the RmaOpCompleteRemote event. */
    OTF2_EVENT_THREAD_FORK                  = 53, /** Event record identifier for the ThreadFork event. */
    OTF2_EVENT_THREAD_JOIN                  = 54, /** Event record identifier for the ThreadJoin event. */
    OTF2_EVENT_THREAD_TEAM_BEGIN            = 55, /** Event record identifier for the ThreadTeamBegin event. */
    OTF2_EVENT_THREAD_TEAM_END              = 56, /** Event record identifier for the ThreadTeamEnd event. */
    OTF2_EVENT_THREAD_ACQUIRE_LOCK          = 57, /** Event record identifier for the ThreadAcquireLock event. */
    OTF2_EVENT_THREAD_RELEASE_LOCK          = 58, /** Event record identifier for the ThreadReleaseLock event. */
    OTF2_EVENT_THREAD_TASK_CREATE           = 59, /** Event record identifier for the ThreadTaskCreate event. */
    OTF2_EVENT_THREAD_TASK_SWITCH           = 60, /** Event record identifier for the ThreadTaskSwitch event. */
    OTF2_EVENT_THREAD_TASK_COMPLETE         = 61, /** Event record identifier for the ThreadTaskComplete event. */

    OTF2_EVENT_MAX_ID
};


/** @brief BufferFlush event.
 */
typedef struct OTF2_BufferFlush_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp stop_time;
} OTF2_BufferFlush;


/** @brief MeasurementOnOff event.
 */
typedef struct OTF2_MeasurementOnOff_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp       time;
    OTF2_MeasurementMode measurement_mode;
} OTF2_MeasurementOnOff;


/** @brief Enter event.
 */
typedef struct OTF2_Enter_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RegionRef region;
} OTF2_Enter;


/** @brief Leave event.
 */
typedef struct OTF2_Leave_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RegionRef region;
} OTF2_Leave;


/** @brief MpiSend event.
 */
typedef struct OTF2_MpiSend_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint32_t       receiver;
    OTF2_CommRef   communicator;
    uint32_t       msg_tag;
    uint64_t       msg_length;
} OTF2_MpiSend;


/** @brief MpiIsend event.
 */
typedef struct OTF2_MpiIsend_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint32_t       receiver;
    OTF2_CommRef   communicator;
    uint32_t       msg_tag;
    uint64_t       msg_length;
    uint64_t       request_id;
} OTF2_MpiIsend;


/** @brief MpiIsendComplete event.
 */
typedef struct OTF2_MpiIsendComplete_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint64_t       request_id;
} OTF2_MpiIsendComplete;


/** @brief MpiIrecvRequest event.
 */
typedef struct OTF2_MpiIrecvRequest_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint64_t       request_id;
} OTF2_MpiIrecvRequest;


/** @brief MpiRecv event.
 */
typedef struct OTF2_MpiRecv_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint32_t       sender;
    OTF2_CommRef   communicator;
    uint32_t       msg_tag;
    uint64_t       msg_length;
} OTF2_MpiRecv;


/** @brief MpiIrecv event.
 */
typedef struct OTF2_MpiIrecv_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint32_t       sender;
    OTF2_CommRef   communicator;
    uint32_t       msg_tag;
    uint64_t       msg_length;
    uint64_t       request_id;
} OTF2_MpiIrecv;


/** @brief MpiRequestTest event.
 */
typedef struct OTF2_MpiRequestTest_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint64_t       request_id;
} OTF2_MpiRequestTest;


/** @brief MpiRequestCancelled event.
 */
typedef struct OTF2_MpiRequestCancelled_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint64_t       request_id;
} OTF2_MpiRequestCancelled;


/** @brief MpiCollectiveBegin event.
 */
typedef struct OTF2_MpiCollectiveBegin_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
} OTF2_MpiCollectiveBegin;


/** @brief MpiCollectiveEnd event.
 */
typedef struct OTF2_MpiCollectiveEnd_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp    time;
    OTF2_CollectiveOp collective_op;
    OTF2_CommRef      communicator;
    uint32_t          root;
    uint64_t          size_sent;
    uint64_t          size_received;
} OTF2_MpiCollectiveEnd;


/** @brief OmpFork event.
 */
typedef struct OTF2_OmpFork_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint32_t       number_of_requested_threads;
} OTF2_OmpFork;


/** @brief OmpJoin event.
 */
typedef struct OTF2_OmpJoin_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
} OTF2_OmpJoin;


/** @brief OmpAcquireLock event.
 */
typedef struct OTF2_OmpAcquireLock_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint32_t       lock_id;
    uint32_t       acquisition_order;
} OTF2_OmpAcquireLock;


/** @brief OmpReleaseLock event.
 */
typedef struct OTF2_OmpReleaseLock_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint32_t       lock_id;
    uint32_t       acquisition_order;
} OTF2_OmpReleaseLock;


/** @brief OmpTaskCreate event.
 */
typedef struct OTF2_OmpTaskCreate_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint64_t       task_id;
} OTF2_OmpTaskCreate;


/** @brief OmpTaskSwitch event.
 */
typedef struct OTF2_OmpTaskSwitch_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint64_t       task_id;
} OTF2_OmpTaskSwitch;


/** @brief OmpTaskComplete event.
 */
typedef struct OTF2_OmpTaskComplete_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    uint64_t       task_id;
} OTF2_OmpTaskComplete;


/** @brief Metric event.
 */
typedef struct OTF2_Metric_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp    time;
    OTF2_MetricRef    metric;
    uint8_t           number_of_metrics;
    OTF2_Type*        type_ids;
    OTF2_MetricValue* metric_values;
} OTF2_Metric;


/** @brief ParameterString event.
 */
typedef struct OTF2_ParameterString_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp    time;
    OTF2_ParameterRef parameter;
    OTF2_StringRef    string;
} OTF2_ParameterString;


/** @brief ParameterInt event.
 */
typedef struct OTF2_ParameterInt_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp    time;
    OTF2_ParameterRef parameter;
    int64_t           value;
} OTF2_ParameterInt;


/** @brief ParameterUnsignedInt event.
 */
typedef struct OTF2_ParameterUnsignedInt_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp    time;
    OTF2_ParameterRef parameter;
    uint64_t          value;
} OTF2_ParameterUnsignedInt;


/** @brief RmaWinCreate event.
 */
typedef struct OTF2_RmaWinCreate_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
} OTF2_RmaWinCreate;


/** @brief RmaWinDestroy event.
 */
typedef struct OTF2_RmaWinDestroy_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
} OTF2_RmaWinDestroy;


/** @brief RmaCollectiveBegin event.
 */
typedef struct OTF2_RmaCollectiveBegin_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
} OTF2_RmaCollectiveBegin;


/** @brief RmaCollectiveEnd event.
 */
typedef struct OTF2_RmaCollectiveEnd_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp    time;
    OTF2_CollectiveOp collective_op;
    OTF2_RmaSyncLevel sync_level;
    OTF2_RmaWinRef    win;
    uint32_t          root;
    uint64_t          bytes_sent;
    uint64_t          bytes_received;
} OTF2_RmaCollectiveEnd;


/** @brief RmaGroupSync event.
 */
typedef struct OTF2_RmaGroupSync_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp    time;
    OTF2_RmaSyncLevel sync_level;
    OTF2_RmaWinRef    win;
    OTF2_GroupRef     group;
} OTF2_RmaGroupSync;


/** @brief RmaRequestLock event.
 */
typedef struct OTF2_RmaRequestLock_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint32_t       remote;
    uint64_t       lock_id;
    OTF2_LockType  lock_type;
} OTF2_RmaRequestLock;


/** @brief RmaAcquireLock event.
 */
typedef struct OTF2_RmaAcquireLock_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint32_t       remote;
    uint64_t       lock_id;
    OTF2_LockType  lock_type;
} OTF2_RmaAcquireLock;


/** @brief RmaTryLock event.
 */
typedef struct OTF2_RmaTryLock_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint32_t       remote;
    uint64_t       lock_id;
    OTF2_LockType  lock_type;
} OTF2_RmaTryLock;


/** @brief RmaReleaseLock event.
 */
typedef struct OTF2_RmaReleaseLock_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint32_t       remote;
    uint64_t       lock_id;
} OTF2_RmaReleaseLock;


/** @brief RmaSync event.
 */
typedef struct OTF2_RmaSync_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp   time;
    OTF2_RmaWinRef   win;
    uint32_t         remote;
    OTF2_RmaSyncType sync_type;
} OTF2_RmaSync;


/** @brief RmaWaitChange event.
 */
typedef struct OTF2_RmaWaitChange_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
} OTF2_RmaWaitChange;


/** @brief RmaPut event.
 */
typedef struct OTF2_RmaPut_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint32_t       remote;
    uint64_t       bytes;
    uint64_t       matching_id;
} OTF2_RmaPut;


/** @brief RmaGet event.
 */
typedef struct OTF2_RmaGet_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint32_t       remote;
    uint64_t       bytes;
    uint64_t       matching_id;
} OTF2_RmaGet;


/** @brief RmaAtomic event.
 */
typedef struct OTF2_RmaAtomic_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp     time;
    OTF2_RmaWinRef     win;
    uint32_t           remote;
    OTF2_RmaAtomicType type;
    uint64_t           bytes_sent;
    uint64_t           bytes_received;
    uint64_t           matching_id;
} OTF2_RmaAtomic;


/** @brief RmaOpCompleteBlocking event.
 */
typedef struct OTF2_RmaOpCompleteBlocking_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint64_t       matching_id;
} OTF2_RmaOpCompleteBlocking;


/** @brief RmaOpCompleteNonBlocking event.
 */
typedef struct OTF2_RmaOpCompleteNonBlocking_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint64_t       matching_id;
} OTF2_RmaOpCompleteNonBlocking;


/** @brief RmaOpTest event.
 */
typedef struct OTF2_RmaOpTest_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint64_t       matching_id;
} OTF2_RmaOpTest;


/** @brief RmaOpCompleteRemote event.
 */
typedef struct OTF2_RmaOpCompleteRemote_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_RmaWinRef win;
    uint64_t       matching_id;
} OTF2_RmaOpCompleteRemote;


/** @brief ThreadFork event.
 */
typedef struct OTF2_ThreadFork_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_Paradigm  model;
    uint32_t       number_of_requested_threads;
} OTF2_ThreadFork;


/** @brief ThreadJoin event.
 */
typedef struct OTF2_ThreadJoin_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_Paradigm  model;
} OTF2_ThreadJoin;


/** @brief ThreadTeamBegin event.
 */
typedef struct OTF2_ThreadTeamBegin_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_CommRef   thread_team;
} OTF2_ThreadTeamBegin;


/** @brief ThreadTeamEnd event.
 */
typedef struct OTF2_ThreadTeamEnd_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_CommRef   thread_team;
} OTF2_ThreadTeamEnd;


/** @brief ThreadAcquireLock event.
 */
typedef struct OTF2_ThreadAcquireLock_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_Paradigm  model;
    uint32_t       lock_id;
    uint32_t       acquisition_order;
} OTF2_ThreadAcquireLock;


/** @brief ThreadReleaseLock event.
 */
typedef struct OTF2_ThreadReleaseLock_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_Paradigm  model;
    uint32_t       lock_id;
    uint32_t       acquisition_order;
} OTF2_ThreadReleaseLock;


/** @brief ThreadTaskCreate event.
 */
typedef struct OTF2_ThreadTaskCreate_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_CommRef   thread_team;
    uint32_t       creating_thread;
    uint32_t       generation_number;
} OTF2_ThreadTaskCreate;


/** @brief ThreadTaskSwitch event.
 */
typedef struct OTF2_ThreadTaskSwitch_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_CommRef   thread_team;
    uint32_t       creating_thread;
    uint32_t       generation_number;
} OTF2_ThreadTaskSwitch;


/** @brief ThreadTaskComplete event.
 */
typedef struct OTF2_ThreadTaskComplete_struct
{
    /** Timestamp, which is exactly here in every event record */
    OTF2_TimeStamp time;
    OTF2_CommRef   thread_team;
    uint32_t       creating_thread;
    uint32_t       generation_number;
} OTF2_ThreadTaskComplete;


typedef struct OTF2_GenericEvent_struct OTF2_GenericEvent;


struct OTF2_GenericEvent_struct
{
    OTF2_EventType type;
    union
    {
        OTF2_TimeStamp                time; /**< Timestamp, which is exactly here in every record */
        OTF2_BufferFlush              buffer_flush;
        OTF2_MeasurementOnOff         measurement_on_off;
        OTF2_Enter                    enter;
        OTF2_Leave                    leave;
        OTF2_MpiSend                  mpi_send;
        OTF2_MpiIsend                 mpi_isend;
        OTF2_MpiIsendComplete         mpi_isend_complete;
        OTF2_MpiIrecvRequest          mpi_irecv_request;
        OTF2_MpiRecv                  mpi_recv;
        OTF2_MpiIrecv                 mpi_irecv;
        OTF2_MpiRequestTest           mpi_request_test;
        OTF2_MpiRequestCancelled      mpi_request_cancelled;
        OTF2_MpiCollectiveBegin       mpi_collective_begin;
        OTF2_MpiCollectiveEnd         mpi_collective_end;
        OTF2_OmpFork                  omp_fork;
        OTF2_OmpJoin                  omp_join;
        OTF2_OmpAcquireLock           omp_acquire_lock;
        OTF2_OmpReleaseLock           omp_release_lock;
        OTF2_OmpTaskCreate            omp_task_create;
        OTF2_OmpTaskSwitch            omp_task_switch;
        OTF2_OmpTaskComplete          omp_task_complete;
        OTF2_Metric                   metric;
        OTF2_ParameterString          parameter_string;
        OTF2_ParameterInt             parameter_int;
        OTF2_ParameterUnsignedInt     parameter_unsigned_int;
        OTF2_RmaWinCreate             rma_win_create;
        OTF2_RmaWinDestroy            rma_win_destroy;
        OTF2_RmaCollectiveBegin       rma_collective_begin;
        OTF2_RmaCollectiveEnd         rma_collective_end;
        OTF2_RmaGroupSync             rma_group_sync;
        OTF2_RmaRequestLock           rma_request_lock;
        OTF2_RmaAcquireLock           rma_acquire_lock;
        OTF2_RmaTryLock               rma_try_lock;
        OTF2_RmaReleaseLock           rma_release_lock;
        OTF2_RmaSync                  rma_sync;
        OTF2_RmaWaitChange            rma_wait_change;
        OTF2_RmaPut                   rma_put;
        OTF2_RmaGet                   rma_get;
        OTF2_RmaAtomic                rma_atomic;
        OTF2_RmaOpCompleteBlocking    rma_op_complete_blocking;
        OTF2_RmaOpCompleteNonBlocking rma_op_complete_non_blocking;
        OTF2_RmaOpTest                rma_op_test;
        OTF2_RmaOpCompleteRemote      rma_op_complete_remote;
        OTF2_ThreadFork               thread_fork;
        OTF2_ThreadJoin               thread_join;
        OTF2_ThreadTeamBegin          thread_team_begin;
        OTF2_ThreadTeamEnd            thread_team_end;
        OTF2_ThreadAcquireLock        thread_acquire_lock;
        OTF2_ThreadReleaseLock        thread_release_lock;
        OTF2_ThreadTaskCreate         thread_task_create;
        OTF2_ThreadTaskSwitch         thread_task_switch;
        OTF2_ThreadTaskComplete       thread_task_complete;
    } record;
};


struct OTF2_EvtReaderCallbacks_struct
{
    OTF2_EvtReaderCallback_Unknown                  unknown;
    OTF2_EvtReaderCallback_BufferFlush              buffer_flush;
    OTF2_EvtReaderCallback_MeasurementOnOff         measurement_on_off;
    OTF2_EvtReaderCallback_Enter                    enter;
    OTF2_EvtReaderCallback_Leave                    leave;
    OTF2_EvtReaderCallback_MpiSend                  mpi_send;
    OTF2_EvtReaderCallback_MpiIsend                 mpi_isend;
    OTF2_EvtReaderCallback_MpiIsendComplete         mpi_isend_complete;
    OTF2_EvtReaderCallback_MpiIrecvRequest          mpi_irecv_request;
    OTF2_EvtReaderCallback_MpiRecv                  mpi_recv;
    OTF2_EvtReaderCallback_MpiIrecv                 mpi_irecv;
    OTF2_EvtReaderCallback_MpiRequestTest           mpi_request_test;
    OTF2_EvtReaderCallback_MpiRequestCancelled      mpi_request_cancelled;
    OTF2_EvtReaderCallback_MpiCollectiveBegin       mpi_collective_begin;
    OTF2_EvtReaderCallback_MpiCollectiveEnd         mpi_collective_end;
    OTF2_EvtReaderCallback_OmpFork                  omp_fork;
    OTF2_EvtReaderCallback_OmpJoin                  omp_join;
    OTF2_EvtReaderCallback_OmpAcquireLock           omp_acquire_lock;
    OTF2_EvtReaderCallback_OmpReleaseLock           omp_release_lock;
    OTF2_EvtReaderCallback_OmpTaskCreate            omp_task_create;
    OTF2_EvtReaderCallback_OmpTaskSwitch            omp_task_switch;
    OTF2_EvtReaderCallback_OmpTaskComplete          omp_task_complete;
    OTF2_EvtReaderCallback_Metric                   metric;
    OTF2_EvtReaderCallback_ParameterString          parameter_string;
    OTF2_EvtReaderCallback_ParameterInt             parameter_int;
    OTF2_EvtReaderCallback_ParameterUnsignedInt     parameter_unsigned_int;
    OTF2_EvtReaderCallback_RmaWinCreate             rma_win_create;
    OTF2_EvtReaderCallback_RmaWinDestroy            rma_win_destroy;
    OTF2_EvtReaderCallback_RmaCollectiveBegin       rma_collective_begin;
    OTF2_EvtReaderCallback_RmaCollectiveEnd         rma_collective_end;
    OTF2_EvtReaderCallback_RmaGroupSync             rma_group_sync;
    OTF2_EvtReaderCallback_RmaRequestLock           rma_request_lock;
    OTF2_EvtReaderCallback_RmaAcquireLock           rma_acquire_lock;
    OTF2_EvtReaderCallback_RmaTryLock               rma_try_lock;
    OTF2_EvtReaderCallback_RmaReleaseLock           rma_release_lock;
    OTF2_EvtReaderCallback_RmaSync                  rma_sync;
    OTF2_EvtReaderCallback_RmaWaitChange            rma_wait_change;
    OTF2_EvtReaderCallback_RmaPut                   rma_put;
    OTF2_EvtReaderCallback_RmaGet                   rma_get;
    OTF2_EvtReaderCallback_RmaAtomic                rma_atomic;
    OTF2_EvtReaderCallback_RmaOpCompleteBlocking    rma_op_complete_blocking;
    OTF2_EvtReaderCallback_RmaOpCompleteNonBlocking rma_op_complete_non_blocking;
    OTF2_EvtReaderCallback_RmaOpTest                rma_op_test;
    OTF2_EvtReaderCallback_RmaOpCompleteRemote      rma_op_complete_remote;
    OTF2_EvtReaderCallback_ThreadFork               thread_fork;
    OTF2_EvtReaderCallback_ThreadJoin               thread_join;
    OTF2_EvtReaderCallback_ThreadTeamBegin          thread_team_begin;
    OTF2_EvtReaderCallback_ThreadTeamEnd            thread_team_end;
    OTF2_EvtReaderCallback_ThreadAcquireLock        thread_acquire_lock;
    OTF2_EvtReaderCallback_ThreadReleaseLock        thread_release_lock;
    OTF2_EvtReaderCallback_ThreadTaskCreate         thread_task_create;
    OTF2_EvtReaderCallback_ThreadTaskSwitch         thread_task_switch;
    OTF2_EvtReaderCallback_ThreadTaskComplete       thread_task_complete;
};


struct OTF2_GlobalEvtReaderCallbacks_struct
{
    OTF2_GlobalEvtReaderCallback_Unknown                  unknown;
    OTF2_GlobalEvtReaderCallback_BufferFlush              buffer_flush;
    OTF2_GlobalEvtReaderCallback_MeasurementOnOff         measurement_on_off;
    OTF2_GlobalEvtReaderCallback_Enter                    enter;
    OTF2_GlobalEvtReaderCallback_Leave                    leave;
    OTF2_GlobalEvtReaderCallback_MpiSend                  mpi_send;
    OTF2_GlobalEvtReaderCallback_MpiIsend                 mpi_isend;
    OTF2_GlobalEvtReaderCallback_MpiIsendComplete         mpi_isend_complete;
    OTF2_GlobalEvtReaderCallback_MpiIrecvRequest          mpi_irecv_request;
    OTF2_GlobalEvtReaderCallback_MpiRecv                  mpi_recv;
    OTF2_GlobalEvtReaderCallback_MpiIrecv                 mpi_irecv;
    OTF2_GlobalEvtReaderCallback_MpiRequestTest           mpi_request_test;
    OTF2_GlobalEvtReaderCallback_MpiRequestCancelled      mpi_request_cancelled;
    OTF2_GlobalEvtReaderCallback_MpiCollectiveBegin       mpi_collective_begin;
    OTF2_GlobalEvtReaderCallback_MpiCollectiveEnd         mpi_collective_end;
    OTF2_GlobalEvtReaderCallback_OmpFork                  omp_fork;
    OTF2_GlobalEvtReaderCallback_OmpJoin                  omp_join;
    OTF2_GlobalEvtReaderCallback_OmpAcquireLock           omp_acquire_lock;
    OTF2_GlobalEvtReaderCallback_OmpReleaseLock           omp_release_lock;
    OTF2_GlobalEvtReaderCallback_OmpTaskCreate            omp_task_create;
    OTF2_GlobalEvtReaderCallback_OmpTaskSwitch            omp_task_switch;
    OTF2_GlobalEvtReaderCallback_OmpTaskComplete          omp_task_complete;
    OTF2_GlobalEvtReaderCallback_Metric                   metric;
    OTF2_GlobalEvtReaderCallback_ParameterString          parameter_string;
    OTF2_GlobalEvtReaderCallback_ParameterInt             parameter_int;
    OTF2_GlobalEvtReaderCallback_ParameterUnsignedInt     parameter_unsigned_int;
    OTF2_GlobalEvtReaderCallback_RmaWinCreate             rma_win_create;
    OTF2_GlobalEvtReaderCallback_RmaWinDestroy            rma_win_destroy;
    OTF2_GlobalEvtReaderCallback_RmaCollectiveBegin       rma_collective_begin;
    OTF2_GlobalEvtReaderCallback_RmaCollectiveEnd         rma_collective_end;
    OTF2_GlobalEvtReaderCallback_RmaGroupSync             rma_group_sync;
    OTF2_GlobalEvtReaderCallback_RmaRequestLock           rma_request_lock;
    OTF2_GlobalEvtReaderCallback_RmaAcquireLock           rma_acquire_lock;
    OTF2_GlobalEvtReaderCallback_RmaTryLock               rma_try_lock;
    OTF2_GlobalEvtReaderCallback_RmaReleaseLock           rma_release_lock;
    OTF2_GlobalEvtReaderCallback_RmaSync                  rma_sync;
    OTF2_GlobalEvtReaderCallback_RmaWaitChange            rma_wait_change;
    OTF2_GlobalEvtReaderCallback_RmaPut                   rma_put;
    OTF2_GlobalEvtReaderCallback_RmaGet                   rma_get;
    OTF2_GlobalEvtReaderCallback_RmaAtomic                rma_atomic;
    OTF2_GlobalEvtReaderCallback_RmaOpCompleteBlocking    rma_op_complete_blocking;
    OTF2_GlobalEvtReaderCallback_RmaOpCompleteNonBlocking rma_op_complete_non_blocking;
    OTF2_GlobalEvtReaderCallback_RmaOpTest                rma_op_test;
    OTF2_GlobalEvtReaderCallback_RmaOpCompleteRemote      rma_op_complete_remote;
    OTF2_GlobalEvtReaderCallback_ThreadFork               thread_fork;
    OTF2_GlobalEvtReaderCallback_ThreadJoin               thread_join;
    OTF2_GlobalEvtReaderCallback_ThreadTeamBegin          thread_team_begin;
    OTF2_GlobalEvtReaderCallback_ThreadTeamEnd            thread_team_end;
    OTF2_GlobalEvtReaderCallback_ThreadAcquireLock        thread_acquire_lock;
    OTF2_GlobalEvtReaderCallback_ThreadReleaseLock        thread_release_lock;
    OTF2_GlobalEvtReaderCallback_ThreadTaskCreate         thread_task_create;
    OTF2_GlobalEvtReaderCallback_ThreadTaskSwitch         thread_task_switch;
    OTF2_GlobalEvtReaderCallback_ThreadTaskComplete       thread_task_complete;
};


bool
otf2_event_thread_fork_convert_to_omp_fork( const OTF2_ThreadFork* threadForkRecord,
                                            OTF2_AttributeList*    attributeList,
                                            OTF2_OmpFork*          ompForkRecord );


bool
otf2_event_thread_join_convert_to_omp_join( const OTF2_ThreadJoin* threadJoinRecord,
                                            OTF2_AttributeList*    attributeList,
                                            OTF2_OmpJoin*          ompJoinRecord );


bool
otf2_event_thread_acquire_lock_convert_to_omp_acquire_lock( const OTF2_ThreadAcquireLock* threadAcquireLockRecord,
                                                            OTF2_AttributeList*           attributeList,
                                                            OTF2_OmpAcquireLock*          ompAcquireLockRecord );


bool
otf2_event_thread_release_lock_convert_to_omp_release_lock( const OTF2_ThreadReleaseLock* threadReleaseLockRecord,
                                                            OTF2_AttributeList*           attributeList,
                                                            OTF2_OmpReleaseLock*          ompReleaseLockRecord );


#endif /* OTF2_INTERNAL_EVENTS_H */
