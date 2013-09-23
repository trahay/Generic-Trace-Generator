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


#ifndef OTF2_INTERNAL_SNAPS_H
#define OTF2_INTERNAL_SNAPS_H


/**
 *  @file       src/otf2_snaps.h
 *  @source     templates/otf2_snaps.tmpl.h
 *
 *  @brief      This file list all snap records supported in OTF2.
 */


typedef uint8_t OTF2_SnapType;


enum OTF2_SnapType_enum
{
    /* First five entries are reserved for OTF2 internals */

    /* Snapshots */
    OTF2_SNAP_SNAPSHOT_START         = 10, /** snap record identifier for the SnapshotStart record. */
    OTF2_SNAP_SNAPSHOT_END           = 11, /** snap record identifier for the SnapshotEnd record. */
    OTF2_SNAP_MEASUREMENT_ON_OFF     = 12, /** snap record identifier for the MeasurementOnOffSnap record. */
    OTF2_SNAP_ENTER                  = 13, /** snap record identifier for the EnterSnap record. */
    OTF2_SNAP_MPI_SEND               = 14, /** snap record identifier for the MpiSendSnap record. */
    OTF2_SNAP_MPI_ISEND              = 15, /** snap record identifier for the MpiIsendSnap record. */
    OTF2_SNAP_MPI_ISEND_COMPLETE     = 16, /** snap record identifier for the MpiIsendCompleteSnap record. */
    OTF2_SNAP_MPI_RECV               = 17, /** snap record identifier for the MpiRecvSnap record. */
    OTF2_SNAP_MPI_IRECV_REQUEST      = 18, /** snap record identifier for the MpiIrecvRequestSnap record. */
    OTF2_SNAP_MPI_IRECV              = 19, /** snap record identifier for the MpiIrecvSnap record. */
    OTF2_SNAP_MPI_COLLECTIVE_BEGIN   = 20, /** snap record identifier for the MpiCollectiveBeginSnap record. */
    OTF2_SNAP_MPI_COLLECTIVE_END     = 21, /** snap record identifier for the MpiCollectiveEndSnap record. */
    OTF2_SNAP_OMP_FORK               = 22, /** snap record identifier for the OmpForkSnap record. */
    OTF2_SNAP_OMP_ACQUIRE_LOCK       = 23, /** snap record identifier for the OmpAcquireLockSnap record. */
    OTF2_SNAP_OMP_TASK_CREATE        = 24, /** snap record identifier for the OmpTaskCreateSnap record. */
    OTF2_SNAP_OMP_TASK_SWITCH        = 25, /** snap record identifier for the OmpTaskSwitchSnap record. */
    OTF2_SNAP_METRIC                 = 26, /** snap record identifier for the MetricSnap record. */
    OTF2_SNAP_PARAMETER_STRING       = 27, /** snap record identifier for the ParameterStringSnap record. */
    OTF2_SNAP_PARAMETER_INT          = 28, /** snap record identifier for the ParameterIntSnap record. */
    OTF2_SNAP_PARAMETER_UNSIGNED_INT = 29, /** snap record identifier for the ParameterUnsignedIntSnap record. */

    OTF2_SNAP_MAX_ID
};


/** @brief SnapshotStart snapshot record.
 */
typedef struct OTF2_SnapshotStart_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    uint64_t       number_of_records;
} OTF2_SnapshotStart;


/** @brief SnapshotEnd snapshot record.
 */
typedef struct OTF2_SnapshotEnd_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    uint64_t       cont_read_pos;
} OTF2_SnapshotEnd;


/** @brief MeasurementOnOff snapshot record.
 */
typedef struct OTF2_MeasurementOnOffSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp       time;
    OTF2_TimeStamp       orig_event_time;
    OTF2_MeasurementMode measurement_mode;
} OTF2_MeasurementOnOffSnap;


/** @brief Enter snapshot record.
 */
typedef struct OTF2_EnterSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    OTF2_RegionRef region;
} OTF2_EnterSnap;


/** @brief MpiSend snapshot record.
 */
typedef struct OTF2_MpiSendSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint32_t       receiver;
    OTF2_CommRef   communicator;
    uint32_t       msg_tag;
    uint64_t       msg_length;
} OTF2_MpiSendSnap;


/** @brief MpiIsend snapshot record.
 */
typedef struct OTF2_MpiIsendSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint32_t       receiver;
    OTF2_CommRef   communicator;
    uint32_t       msg_tag;
    uint64_t       msg_length;
    uint64_t       request_id;
} OTF2_MpiIsendSnap;


/** @brief MpiIsendComplete snapshot record.
 */
typedef struct OTF2_MpiIsendCompleteSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint64_t       request_id;
} OTF2_MpiIsendCompleteSnap;


/** @brief MpiRecv snapshot record.
 */
typedef struct OTF2_MpiRecvSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint32_t       sender;
    OTF2_CommRef   communicator;
    uint32_t       msg_tag;
    uint64_t       msg_length;
} OTF2_MpiRecvSnap;


/** @brief MpiIrecvRequest snapshot record.
 */
typedef struct OTF2_MpiIrecvRequestSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint64_t       request_id;
} OTF2_MpiIrecvRequestSnap;


/** @brief MpiIrecv snapshot record.
 */
typedef struct OTF2_MpiIrecvSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint32_t       sender;
    OTF2_CommRef   communicator;
    uint32_t       msg_tag;
    uint64_t       msg_length;
    uint64_t       request_id;
} OTF2_MpiIrecvSnap;


/** @brief MpiCollectiveBegin snapshot record.
 */
typedef struct OTF2_MpiCollectiveBeginSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
} OTF2_MpiCollectiveBeginSnap;


/** @brief MpiCollectiveEnd snapshot record.
 */
typedef struct OTF2_MpiCollectiveEndSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp    time;
    OTF2_TimeStamp    orig_event_time;
    OTF2_CollectiveOp collective_op;
    OTF2_CommRef      communicator;
    uint32_t          root;
    uint64_t          size_sent;
    uint64_t          size_received;
} OTF2_MpiCollectiveEndSnap;


/** @brief OmpFork snapshot record.
 */
typedef struct OTF2_OmpForkSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint32_t       number_of_requested_threads;
} OTF2_OmpForkSnap;


/** @brief OmpAcquireLock snapshot record.
 */
typedef struct OTF2_OmpAcquireLockSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint32_t       lock_id;
    uint32_t       acquisition_order;
} OTF2_OmpAcquireLockSnap;


/** @brief OmpTaskCreate snapshot record.
 */
typedef struct OTF2_OmpTaskCreateSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint64_t       task_id;
} OTF2_OmpTaskCreateSnap;


/** @brief OmpTaskSwitch snapshot record.
 */
typedef struct OTF2_OmpTaskSwitchSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    OTF2_TimeStamp orig_event_time;
    uint64_t       task_id;
} OTF2_OmpTaskSwitchSnap;


/** @brief Metric snapshot record.
 */
typedef struct OTF2_MetricSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp    time;
    OTF2_TimeStamp    orig_event_time;
    OTF2_MetricRef    metric;
    uint8_t           number_of_metrics;
    OTF2_Type*        type_ids;
    OTF2_MetricValue* metric_values;
} OTF2_MetricSnap;


/** @brief ParameterString snapshot record.
 */
typedef struct OTF2_ParameterStringSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp    time;
    OTF2_TimeStamp    orig_event_time;
    OTF2_ParameterRef parameter;
    OTF2_StringRef    string;
} OTF2_ParameterStringSnap;


/** @brief ParameterInt snapshot record.
 */
typedef struct OTF2_ParameterIntSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp    time;
    OTF2_TimeStamp    orig_event_time;
    OTF2_ParameterRef parameter;
    int64_t           value;
} OTF2_ParameterIntSnap;


/** @brief ParameterUnsignedInt snapshot record.
 */
typedef struct OTF2_ParameterUnsignedIntSnap_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp    time;
    OTF2_TimeStamp    orig_event_time;
    OTF2_ParameterRef parameter;
    uint64_t          value;
} OTF2_ParameterUnsignedIntSnap;


typedef struct OTF2_GenericSnap_struct OTF2_GenericSnap;


struct OTF2_GenericSnap_struct
{
    OTF2_SnapType type;
    union
    {
        OTF2_TimeStamp                time; /**< Timestamp, which is exactly here in every record */
        OTF2_SnapshotStart            snapshot_start;
        OTF2_SnapshotEnd              snapshot_end;
        OTF2_MeasurementOnOffSnap     measurement_on_off;
        OTF2_EnterSnap                enter;
        OTF2_MpiSendSnap              mpi_send;
        OTF2_MpiIsendSnap             mpi_isend;
        OTF2_MpiIsendCompleteSnap     mpi_isend_complete;
        OTF2_MpiRecvSnap              mpi_recv;
        OTF2_MpiIrecvRequestSnap      mpi_irecv_request;
        OTF2_MpiIrecvSnap             mpi_irecv;
        OTF2_MpiCollectiveBeginSnap   mpi_collective_begin;
        OTF2_MpiCollectiveEndSnap     mpi_collective_end;
        OTF2_OmpForkSnap              omp_fork;
        OTF2_OmpAcquireLockSnap       omp_acquire_lock;
        OTF2_OmpTaskCreateSnap        omp_task_create;
        OTF2_OmpTaskSwitchSnap        omp_task_switch;
        OTF2_MetricSnap               metric;
        OTF2_ParameterStringSnap      parameter_string;
        OTF2_ParameterIntSnap         parameter_int;
        OTF2_ParameterUnsignedIntSnap parameter_unsigned_int;
    } record;
};


struct OTF2_SnapReaderCallbacks_struct
{
    OTF2_SnapReaderCallback_Unknown              unknown;
    OTF2_SnapReaderCallback_SnapshotStart        snapshot_start;
    OTF2_SnapReaderCallback_SnapshotEnd          snapshot_end;
    OTF2_SnapReaderCallback_MeasurementOnOff     measurement_on_off;
    OTF2_SnapReaderCallback_Enter                enter;
    OTF2_SnapReaderCallback_MpiSend              mpi_send;
    OTF2_SnapReaderCallback_MpiIsend             mpi_isend;
    OTF2_SnapReaderCallback_MpiIsendComplete     mpi_isend_complete;
    OTF2_SnapReaderCallback_MpiRecv              mpi_recv;
    OTF2_SnapReaderCallback_MpiIrecvRequest      mpi_irecv_request;
    OTF2_SnapReaderCallback_MpiIrecv             mpi_irecv;
    OTF2_SnapReaderCallback_MpiCollectiveBegin   mpi_collective_begin;
    OTF2_SnapReaderCallback_MpiCollectiveEnd     mpi_collective_end;
    OTF2_SnapReaderCallback_OmpFork              omp_fork;
    OTF2_SnapReaderCallback_OmpAcquireLock       omp_acquire_lock;
    OTF2_SnapReaderCallback_OmpTaskCreate        omp_task_create;
    OTF2_SnapReaderCallback_OmpTaskSwitch        omp_task_switch;
    OTF2_SnapReaderCallback_Metric               metric;
    OTF2_SnapReaderCallback_ParameterString      parameter_string;
    OTF2_SnapReaderCallback_ParameterInt         parameter_int;
    OTF2_SnapReaderCallback_ParameterUnsignedInt parameter_unsigned_int;
};


struct OTF2_GlobalSnapReaderCallbacks_struct
{
    OTF2_GlobalSnapReaderCallback_Unknown              unknown;
    OTF2_GlobalSnapReaderCallback_SnapshotStart        snapshot_start;
    OTF2_GlobalSnapReaderCallback_SnapshotEnd          snapshot_end;
    OTF2_GlobalSnapReaderCallback_MeasurementOnOff     measurement_on_off;
    OTF2_GlobalSnapReaderCallback_Enter                enter;
    OTF2_GlobalSnapReaderCallback_MpiSend              mpi_send;
    OTF2_GlobalSnapReaderCallback_MpiIsend             mpi_isend;
    OTF2_GlobalSnapReaderCallback_MpiIsendComplete     mpi_isend_complete;
    OTF2_GlobalSnapReaderCallback_MpiRecv              mpi_recv;
    OTF2_GlobalSnapReaderCallback_MpiIrecvRequest      mpi_irecv_request;
    OTF2_GlobalSnapReaderCallback_MpiIrecv             mpi_irecv;
    OTF2_GlobalSnapReaderCallback_MpiCollectiveBegin   mpi_collective_begin;
    OTF2_GlobalSnapReaderCallback_MpiCollectiveEnd     mpi_collective_end;
    OTF2_GlobalSnapReaderCallback_OmpFork              omp_fork;
    OTF2_GlobalSnapReaderCallback_OmpAcquireLock       omp_acquire_lock;
    OTF2_GlobalSnapReaderCallback_OmpTaskCreate        omp_task_create;
    OTF2_GlobalSnapReaderCallback_OmpTaskSwitch        omp_task_switch;
    OTF2_GlobalSnapReaderCallback_Metric               metric;
    OTF2_GlobalSnapReaderCallback_ParameterString      parameter_string;
    OTF2_GlobalSnapReaderCallback_ParameterInt         parameter_int;
    OTF2_GlobalSnapReaderCallback_ParameterUnsignedInt parameter_unsigned_int;
};

#endif /* OTF2_INTERNAL_SNAPS_H */
