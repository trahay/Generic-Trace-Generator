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


#ifndef OTF2_DEFINITIONS_H
#define OTF2_DEFINITIONS_H


/**
 *  @file       include/otf2/OTF2_Definitions.h
 *  @source     templates/OTF2_Definitions.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Data types used in the definition records.
 */


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Wrapper for enum @ref OTF2_SystemTreeDomain_enum. */
typedef uint8_t OTF2_SystemTreeDomain;

/**
 * @brief
 *
 * @since Version 1.2
 */
enum OTF2_SystemTreeDomain_enum
{
    /** @brief All nodes below a node with this attribute encompass a tightly coupled
     *  HPC system.
     */
    OTF2_SYSTEM_TREE_DOMAIN_MACHINE       = 0,
    /** @brief All nodes below a node with this attribute encompass a system where
     *  processes can communicate via hardware shared memory.
     */
    OTF2_SYSTEM_TREE_DOMAIN_SHARED_MEMORY = 1,
    /** @brief A numa domain. A set of processors around memory which the processors
     *  can directly access.
     */
    OTF2_SYSTEM_TREE_DOMAIN_NUMA          = 2,
    /** @brief Socket, physical package, or chip. In the physical meaning, i.e. that
     *  you can add or remove physically.
     */
    OTF2_SYSTEM_TREE_DOMAIN_SOCKET        = 3,
    /** @brief Cache. Can be L1i, L1d, L2, L3, ...
     */
    OTF2_SYSTEM_TREE_DOMAIN_CACHE         = 4,
    /** @brief Core. A computation unit (may be shared by several logical
     *  processors).
     */
    OTF2_SYSTEM_TREE_DOMAIN_CORE          = 5,
    /** @brief Processing Unit (An non-shared ALU, FPU, ...)
     */
    OTF2_SYSTEM_TREE_DOMAIN_PU            = 6
};


/** @brief Wrapper for enum @ref OTF2_LocationGroupType_enum. */
typedef uint8_t OTF2_LocationGroupType;

/**
 * @brief List of possible definitions of type LocationGroup.
 *
 * @since Version 1.0
 */
enum OTF2_LocationGroupType_enum
{
    /** @brief A location group of unknown type.
     */
    OTF2_LOCATION_GROUP_TYPE_UNKNOWN = 0,
    /** @brief A process.
     */
    OTF2_LOCATION_GROUP_TYPE_PROCESS = 1
};


/** @brief Wrapper for enum @ref OTF2_LocationType_enum. */
typedef uint8_t OTF2_LocationType;

/**
 * @brief List of possible definitions of type Location.
 *
 * @since Version 1.0
 */
enum OTF2_LocationType_enum
{
    /** @brief A location of unknown type.
     */
    OTF2_LOCATION_TYPE_UNKNOWN    = 0,
    /** @brief A CPU thread.
     */
    OTF2_LOCATION_TYPE_CPU_THREAD = 1,
    /** @brief A GPU location.
     */
    OTF2_LOCATION_TYPE_GPU        = 2,
    /** @brief A metric only location e.g. an external device.
     */
    OTF2_LOCATION_TYPE_METRIC     = 3
};


/** @brief Wrapper for enum @ref OTF2_RegionRole_enum. */
typedef uint8_t OTF2_RegionRole;

/**
 * @brief List of possible roles of a Region.
 *
 * @since Version 1.1
 */
enum OTF2_RegionRole_enum
{
    /** @brief A region of unknown role.
     */
    OTF2_REGION_ROLE_UNKNOWN          = 0,
    /** @brief An entire function/subroutine.
     */
    OTF2_REGION_ROLE_FUNCTION         = 1,
    /** @brief An API function wrapped by Score-P.
     */
    OTF2_REGION_ROLE_WRAPPER          = 2,
    /** @brief A loop in the code.
     */
    OTF2_REGION_ROLE_LOOP             = 3,
    /** @brief An arbitrary section of code.
     */
    OTF2_REGION_ROLE_CODE             = 4,
    /** @brief E.g. OpenMP "parallel" construct (structured block)
     */
    OTF2_REGION_ROLE_PARALLEL         = 5,
    /** @brief E.g. OpenMP "sections" construct
     */
    OTF2_REGION_ROLE_SECTIONS         = 6,
    /** @brief Individual "section" inside an OpenMP "sections" construct
     */
    OTF2_REGION_ROLE_SECTION          = 7,
    /** @brief E.g. OpenMP "workshare" construct
     */
    OTF2_REGION_ROLE_WORKSHARE        = 8,
    /** @brief E.g. OpenMP "single" construct
     */
    OTF2_REGION_ROLE_SINGLE           = 9,
    /** @brief E.g. OpenMP "single" construct (structured block)
     */
    OTF2_REGION_ROLE_SINGLE_SBLOCK    = 10,
    /** @brief E.g. OpenMP "master" construct
     */
    OTF2_REGION_ROLE_MASTER           = 11,
    /** @brief E.g. OpenMP "critical" construct
     */
    OTF2_REGION_ROLE_CRITICAL         = 12,
    /** @brief E.g. OpenMP "critical" construct (structured block)
     */
    OTF2_REGION_ROLE_CRITICAL_SBLOCK  = 13,
    /** @brief E.g. OpenMP "atomic" construct
     */
    OTF2_REGION_ROLE_ATOMIC           = 14,
    /** @brief Explicit barrier
     */
    OTF2_REGION_ROLE_BARRIER          = 15,
    /** @brief Implicit barrier
     */
    OTF2_REGION_ROLE_IMPLICIT_BARRIER = 16,
    /** @brief E.g. OpenMP "flush" construct
     */
    OTF2_REGION_ROLE_FLUSH            = 17,
    /** @brief E.g. OpenMP "ordered" construct
     */
    OTF2_REGION_ROLE_ORDERED          = 18,
    /** @brief E.g. OpenMP "ordered" construct (structured block)
     */
    OTF2_REGION_ROLE_ORDERED_SBLOCK   = 19,
    /** @brief "task" construct (structured block)
     */
    OTF2_REGION_ROLE_TASK             = 20,
    /** @brief "task" construct (creation)
     */
    OTF2_REGION_ROLE_TASK_CREATE      = 21,
    /** @brief "taskwait" construct
     */
    OTF2_REGION_ROLE_TASK_WAIT        = 22,
    /** @brief Collective 1:N communication operation
     */
    OTF2_REGION_ROLE_COLL_ONE2ALL     = 23,
    /** @brief Collective N:1 communication operation
     */
    OTF2_REGION_ROLE_COLL_ALL2ONE     = 24,
    /** @brief Collective N:N communication operation
     */
    OTF2_REGION_ROLE_COLL_ALL2ALL     = 25,
    /** @brief Collective M:N communication operation
     */
    OTF2_REGION_ROLE_COLL_OTHER       = 26,
    /** @brief Any file I/O operation.
     */
    OTF2_REGION_ROLE_FILE_IO          = 27,
    /** @brief A point-to-point communication function.
     */
    OTF2_REGION_ROLE_POINT2POINT      = 28,
    /** @brief A remote memory access communication operation.
     */
    OTF2_REGION_ROLE_RMA              = 29,
    /** @brief A data transfer operation in memory.
     */
    OTF2_REGION_ROLE_DATA_TRANSFER    = 30,
    /** @brief An artificial region, mostly used by the monitor software. @since
     *  Version 1.2.
     */
    OTF2_REGION_ROLE_ARTIFICIAL       = 31
};


/** @brief Wrapper for enum @ref OTF2_RegionFlag_enum. */
typedef uint32_t OTF2_RegionFlag;

/**
 * @brief List of possible flags to specify special characteristics of a Region.
 *
 * @since Version 1.1
 */
enum OTF2_RegionFlag_enum
{
    /** @brief A region without special characterization.
     */
    OTF2_REGION_FLAG_NONE    = 0,
    /** @brief Each time this region is entered it will get an individual call path
     *  in the profile.
     */
    OTF2_REGION_FLAG_DYNAMIC = ( 1 << 0 ),
    /** @brief Each time this region is entered it will get an individual root node
     *  in the profile.
     */
    OTF2_REGION_FLAG_PHASE   = ( 1 << 1 )
};


/** @brief Wrapper for enum @ref OTF2_GroupType_enum. */
typedef uint8_t OTF2_GroupType;

/**
 * @brief
 *
 * @since Version 1.2
 */
enum OTF2_GroupType_enum
{
    /** @brief Group of unknown type.
     */
    OTF2_GROUP_TYPE_UNKNOWN        = 0,
    /** @brief Group of locations.
     */
    OTF2_GROUP_TYPE_LOCATIONS      = 1,
    /** @brief Group of regions.
     */
    OTF2_GROUP_TYPE_REGIONS        = 2,
    /** @brief Group of metrics.
     */
    OTF2_GROUP_TYPE_METRIC         = 3,
    /** @brief List of location IDs, which are MPI ranks.
     *
     *  The size of this group should match the size of MPI_COMM_WORLD. Each
     *  entry in the list is a location ID, where the index of the
     *  entry is equal to the rank in MPI_COMM_WORLD. (Ie. rank i
     *  corresponds to location members[i])
     *
     *  Also, if this definition is present, the location group ids of
     *  locations with type OTF2_LOCATION_TYPE_CPU_THREAD should match
     *  The MPI rank.
     *
     *  This group needs to be defined, before any group of type
     *  @eref{OTF2_GROUP_TYPE_MPI_GROUP}.
     *
     *  Note: This does not makes sense in local definitions.
     */
    OTF2_GROUP_TYPE_COMM_LOCATIONS = 4,
    /** @brief MPI group.
     */
    OTF2_GROUP_TYPE_COMM_GROUP     = 5,
    /** @brief Special group type to efficiently handle MPI self-like communicators.
     */
    OTF2_GROUP_TYPE_COMM_SELF      = 6
};


/** @brief Wrapper for enum @ref OTF2_GroupFlag_enum. */
typedef uint32_t OTF2_GroupFlag;

/**
 * @brief List of possible flags to specify special characteristics of a Group.
 *
 * @since Version 1.2
 */
enum OTF2_GroupFlag_enum
{
    /** @brief A group without special characterization.
     */
    OTF2_GROUP_FLAG_NONE           = 0,
    /** @brief No translation needs to be done when a group of type
     *  @eref{OTF2_GROUP_TYPE_COMM_GROUP} has this flag.
     */
    OTF2_GROUP_FLAG_GLOBAL_MEMBERS = ( 1 << 0 )
};


/** @brief Wrapper for enum @ref OTF2_MetricOccurrence_enum. */
typedef uint8_t OTF2_MetricOccurrence;

/**
 * @brief Metric occurrence.
 *
 * @since Version 1.0
 */
enum OTF2_MetricOccurrence_enum
{
    /** @brief Metric occurs at every region enter and leave.
     */
    OTF2_METRIC_SYNCHRONOUS_STRICT = 0,
    /** @brief Metric occurs only at a region enter and leave, but does not need to
     *  occur at every enter/leave.
     */
    OTF2_METRIC_SYNCHRONOUS        = 1,
    /** @brief Metric can occur at any place i.e. it is not related to region enter
     *  and leaves.
     */
    OTF2_METRIC_ASYNCHRONOUS       = 2
};


/** @brief Wrapper for enum @ref OTF2_MetricType_enum. */
typedef uint8_t OTF2_MetricType;

/**
 * @brief
 *
 * @since Version 1.0
 */
enum OTF2_MetricType_enum
{
    /** @brief Any metric of a type not explicitly listed below.
     */
    OTF2_METRIC_TYPE_OTHER  = 0,
    /** @brief PAPI counter.
     */
    OTF2_METRIC_TYPE_PAPI   = 1,
    /** @brief Resource usage counter.
     */
    OTF2_METRIC_TYPE_RUSAGE = 2,
    /** @brief User metrics.
     */
    OTF2_METRIC_TYPE_USER   = 3
};


/** @brief Wrapper for enum @ref OTF2_MetricValueProperty_enum. */
typedef uint8_t OTF2_MetricValueProperty;

/**
 * @brief Information about whether the metric value is accumulated, absolute,
 * or relative. Used for the lower half-byte of OTF2_MetricMode.
 *
 * @since Version 1.0
 */
enum OTF2_MetricValueProperty_enum
{
    /** @brief Accumulated metric is monotonously increasing (i.e., PAPI counter for
     *  number of executed floating point operations).
     */
    OTF2_METRIC_VALUE_ACCUMULATED = 0,
    /** @brief Absolute metric (i.e., temperature, rate, mean value, etc.).
     */
    OTF2_METRIC_VALUE_ABSOLUTE    = 1,
    /** @brief Relative metric.
     */
    OTF2_METRIC_VALUE_RELATIVE    = 2,

    /** @brief This mask can be used to get lower half-byte in OTF2_MetricMode that
     *  is used to indicate metric value property.
     */
    OTF2_METRIC_VALUE_MASK        = 15 /* xxxx1111 */
};


/** @brief Wrapper for enum @ref OTF2_MetricTiming_enum. */
typedef uint8_t OTF2_MetricTiming;

/**
 * @brief Determines when the values have been collected or for which interval
 * of time they are valid. Used for the upper half-byte of
 * OTF2_MetricMode.
 *
 * @since Version 1.0
 */
enum OTF2_MetricTiming_enum
{
    /** @brief Metric value belongs to the time interval since the beginning of the
     *  measurement.
     */
    OTF2_METRIC_TIMING_START = 0,
    /** @brief Metric value is only valid at a point in time but not necessarily for
     *  any interval of time.
     */
    OTF2_METRIC_TIMING_POINT = 1 << 4,
    /** @brief Metric value is related to the time interval since the last counter
     *  sample of the same metric, i.e. the immediate past.
     */
    OTF2_METRIC_TIMING_LAST  = 2 << 4,
    /** @brief Metric value is valid from now until the next counter sample, i.e. the
     *  future right ahead.
     */
    OTF2_METRIC_TIMING_NEXT  = 3 << 4,

    /** @brief This mask can be used to get the upper half-byte in OTF2_MetricMode
     *  that is used to indicate metric timing information.
     */
    OTF2_METRIC_TIMING_MASK  = 240 /* 1111xxxx */
};


/** @brief Wrapper for enum @ref OTF2_MetricMode_enum. */
typedef uint8_t OTF2_MetricMode;

/**
 * @brief Metric mode is a combination of value property and timing information.
 *
 * @since Version 1.0
 */
enum OTF2_MetricMode_enum
{
    /** @brief Accumulated metric, 'START' timing.
     */
    OTF2_METRIC_ACCUMULATED_START = OTF2_METRIC_VALUE_ACCUMULATED | OTF2_METRIC_TIMING_START,
    /** @brief Accumulated metric, 'POINT' timing.
     */
    OTF2_METRIC_ACCUMULATED_POINT = OTF2_METRIC_VALUE_ACCUMULATED | OTF2_METRIC_TIMING_POINT,
    /** @brief Accumulated metric, 'LAST' timing.
     */
    OTF2_METRIC_ACCUMULATED_LAST  = OTF2_METRIC_VALUE_ACCUMULATED | OTF2_METRIC_TIMING_LAST,
    /** @brief Accumulated metric, 'NEXT' timing.
     */
    OTF2_METRIC_ACCUMULATED_NEXT  = OTF2_METRIC_VALUE_ACCUMULATED | OTF2_METRIC_TIMING_NEXT,
    /** @brief Absolute metric, 'POINT' timing.
     */
    OTF2_METRIC_ABSOLUTE_POINT    = OTF2_METRIC_VALUE_ABSOLUTE | OTF2_METRIC_TIMING_POINT,
    /** @brief Absolute metric, 'LAST' timing.
     */
    OTF2_METRIC_ABSOLUTE_LAST     = OTF2_METRIC_VALUE_ABSOLUTE | OTF2_METRIC_TIMING_LAST,
    /** @brief Absolute metric, 'NEXT' timing.
     */
    OTF2_METRIC_ABSOLUTE_NEXT     = OTF2_METRIC_VALUE_ABSOLUTE | OTF2_METRIC_TIMING_NEXT,
    /** @brief Relative metric, 'POINT' timing.
     */
    OTF2_METRIC_RELATIVE_POINT    = OTF2_METRIC_VALUE_RELATIVE | OTF2_METRIC_TIMING_POINT,
    /** @brief Relative metric, 'LAST' timing.
     */
    OTF2_METRIC_RELATIVE_LAST     = OTF2_METRIC_VALUE_RELATIVE | OTF2_METRIC_TIMING_LAST,
    /** @brief Relative metric, 'NEXT' timing.
     */
    OTF2_METRIC_RELATIVE_NEXT     = OTF2_METRIC_VALUE_RELATIVE | OTF2_METRIC_TIMING_NEXT
};


/** @brief Wrapper for enum @ref OTF2_MetricBase_enum. */
typedef uint8_t OTF2_MetricBase;

/**
 * @brief Metric base types.
 *
 * @since Version 1.0
 */
enum OTF2_MetricBase_enum
{
    /** @brief Binary base.
     */
    OTF2_BASE_BINARY  = 0,
    /** @brief Decimal base.
     */
    OTF2_BASE_DECIMAL = 1
};


/** @brief Wrapper for enum @ref OTF2_MetricScope_enum. */
typedef uint8_t OTF2_MetricScope;

/**
 * @brief
 *
 * @since Version 1.0
 */
enum OTF2_MetricScope_enum
{
    /** @brief Scope of a metric is another location.
     */
    OTF2_SCOPE_LOCATION         = 0,
    /** @brief Scope of a metric is a location group.
     */
    OTF2_SCOPE_LOCATION_GROUP   = 1,
    /** @brief Scope of a metric is a system tree node.
     */
    OTF2_SCOPE_SYSTEM_TREE_NODE = 2,
    /** @brief Scope of a metric is a generic group of locations.
     */
    OTF2_SCOPE_GROUP            = 3
};


/** @brief Wrapper for enum @ref OTF2_RecorderKind_enum. */
typedef uint8_t OTF2_RecorderKind;

/**
 * @brief List of possible kinds a MetricClass can be recorded by.
 *
 * @since Version 1.2
 */
enum OTF2_RecorderKind_enum
{
    /** @brief No specific kind of recorder.
     */
    OTF2_RECORDER_KIND_UNKNOWN  = 0,
    /** @brief Only @eref{MetricInstances} will record this metric class.
     */
    OTF2_RECORDER_KIND_ABSTRACT = 1,
    /** @brief This metric class will only be recored by locations of type
     *  @eref{OTF2_LOCATION_TYPE_CPU_THREAD}.
     */
    OTF2_RECORDER_KIND_CPU      = 2,
    /** @brief This metric class will only be recored by locations of type
     *  @eref{OTF2_LOCATION_TYPE_GPU}.
     */
    OTF2_RECORDER_KIND_GPU      = 3
};


/** @brief Wrapper for enum @ref OTF2_ParameterType_enum. */
typedef uint8_t OTF2_ParameterType;

/**
 * @brief List of possible for definitions of type Parameter.
 *
 * @since Version 1.0
 */
enum OTF2_ParameterType_enum
{
    /** @brief Parameter is of type string.
     */
    OTF2_PARAMETER_TYPE_STRING = 0,
    /** @brief Parameter is of type signed 8-byte integer.
     */
    OTF2_PARAMETER_TYPE_INT64  = 1,
    /** @brief Parameter is of type unsigned 8-byte integer.
     */
    OTF2_PARAMETER_TYPE_UINT64 = 2
};


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* OTF2_DEFINITIONS_H */
