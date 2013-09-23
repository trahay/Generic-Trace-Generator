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


#ifndef OTF2_EVENTS_H
#define OTF2_EVENTS_H


/**
 *  @file       include/otf2/OTF2_Events.h
 *  @source     templates/OTF2_Events.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Enums and types used in event records.
 */


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Wrapper for enum @ref OTF2_MeasurementMode_enum. */
typedef uint8_t OTF2_MeasurementMode;

/**
 * @brief Types for use in the MeasurementOnOff event.
 *
 * @since Version 1.0
 */
enum OTF2_MeasurementMode_enum
{
    /** @brief The measurement resumed with event recording.
     */
    OTF2_MEASUREMENT_ON  = 1,
    /** @brief The measurement suspended with event recording.
     */
    OTF2_MEASUREMENT_OFF = 2
};


/** @brief Wrapper for enum @ref OTF2_CollectiveOp_enum. */
typedef uint8_t OTF2_CollectiveOp;

/**
 * @brief Types of collective operations.
 *
 * @since Version 1.0
 */
enum OTF2_CollectiveOp_enum
{
    /** @brief
     */
    OTF2_COLLECTIVE_OP_BARRIER                       = 0,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_BCAST                         = 1,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_GATHER                        = 2,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_GATHERV                       = 3,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_SCATTER                       = 4,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_SCATTERV                      = 5,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_ALLGATHER                     = 6,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_ALLGATHERV                    = 7,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_ALLTOALL                      = 8,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_ALLTOALLV                     = 9,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_ALLTOALLW                     = 10,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_ALLREDUCE                     = 11,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_REDUCE                        = 12,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_REDUCE_SCATTER                = 13,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_SCAN                          = 14,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_EXSCAN                        = 15,
    /** @brief
     */
    OTF2_COLLECTIVE_OP_REDUCE_SCATTER_BLOCK          = 16,
    /** @brief Collectively create a handle (ie. MPI_Win, MPI_Comm, MPI_File).
     */
    OTF2_COLLECTIVE_OP_CREATE_HANDLE                 = 17,
    /** @brief Collectively destroy a handle.
     */
    OTF2_COLLECTIVE_OP_DESTROY_HANDLE                = 18,
    /** @brief Collectively allocate memory.
     */
    OTF2_COLLECTIVE_OP_ALLOCATE                      = 19,
    /** @brief Collectively deallocate memory.
     */
    OTF2_COLLECTIVE_OP_DEALLOCATE                    = 20,
    /** @brief Collectively create a handle and allocate memory.
     */
    OTF2_COLLECTIVE_OP_CREATE_HANDLE_AND_ALLOCATE    = 21,
    /** @brief Collectively destroy a handle and deallocate memory.
     */
    OTF2_COLLECTIVE_OP_DESTROY_HANDLE_AND_DEALLOCATE = 22
};


/** @brief Wrapper for enum @ref OTF2_RmaSyncType_enum. */
typedef uint8_t OTF2_RmaSyncType;

/**
 * @brief Type of direct RMA synchronization call
 *
 * @since Version 1.2
 */
enum OTF2_RmaSyncType_enum
{
    /** @brief Synchronize memory copy.
     */
    OTF2_RMA_SYNC_TYPE_MEMORY     = 0,
    /** @brief Incoming remote notification.
     */
    OTF2_RMA_SYNC_TYPE_NOTIFY_IN  = 1,
    /** @brief Outgoing remote notification.
     */
    OTF2_RMA_SYNC_TYPE_NOTIFY_OUT = 2
};


/** @brief Wrapper for enum @ref OTF2_RmaSyncLevel_enum. */
typedef uint32_t OTF2_RmaSyncLevel;

/**
 * @brief Synchronization level used in RMA synchronization records.
 *
 * @since Version 1.2
 */
enum OTF2_RmaSyncLevel_enum
{
    /** @brief No process synchronization or access completion (e.g., MPI_Win_post,
     *  MPI_Win_start).
     */
    OTF2_RMA_SYNC_LEVEL_NONE    = 0,
    /** @brief Synchronize processes (e.g., MPI_Win_create/free).
     */
    OTF2_RMA_SYNC_LEVEL_PROCESS = ( 1 << 0 ),
    /** @brief Complete memory accesses (e.g., MPI_Win_complete, MPI_Win_wait).
     */
    OTF2_RMA_SYNC_LEVEL_MEMORY  = ( 1 << 1 )
};


/** @brief Wrapper for enum @ref OTF2_LockType_enum. */
typedef uint8_t OTF2_LockType;

/**
 * @brief General Lock Type.
 *
 * @since Version 1.2
 */
enum OTF2_LockType_enum
{
    /** @brief Exclusive lock. No other lock will be granted.
     */
    OTF2_LOCK_EXCLUSIVE = 0,
    /** @brief Shared lock. Other shared locks will be granted, but no exclusive
     *  locks.
     */
    OTF2_LOCK_SHARED    = 1
};


/** @brief Wrapper for enum @ref OTF2_RmaAtomicType_enum. */
typedef uint8_t OTF2_RmaAtomicType;

/**
 * @brief RMA Atomic Operation Type.
 *
 * @since Version 1.2
 */
enum OTF2_RmaAtomicType_enum
{
    /** @brief
     */
    OTF2_RMA_ATOMIC_TYPE_ACCUMULATE       = 0,
    /** @brief
     */
    OTF2_RMA_ATOMIC_TYPE_INCREMENT        = 1,
    /** @brief
     */
    OTF2_RMA_ATOMIC_TYPE_TEST_AND_SET     = 2,
    /** @brief
     */
    OTF2_RMA_ATOMIC_TYPE_COMPARE_AND_SWAP = 3
};


/** @brief Metric value. */
typedef union OTF2_MetricValue_union OTF2_MetricValue;
union OTF2_MetricValue_union
{
    int64_t  signed_int;
    uint64_t unsigned_int;
    double   floating_point;
};


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* OTF2_EVENTS_H */
