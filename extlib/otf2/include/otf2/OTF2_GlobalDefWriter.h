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


#ifndef OTF2_GLOBAL_DEF_WRITER_H
#define OTF2_GLOBAL_DEF_WRITER_H


/**
 *  @file       include/otf2/OTF2_GlobalDefWriter.h
 *  @source     templates/OTF2_GlobalDefWriter.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer always writes globally defined OTF2 definition
 *              records and is used to write either the global definitions
 *              in addition to local definitions or write all definitions as
 *              globally valid in combination with OTF2_GlobalEventWriter.
 *              Global definitions are stored in one global definition file,
 *              which makes it nearly impossible to write them in a distributed
 *              manner. It is therefore only allowed to get such a writer from
 *              an OTF2_ArchiveHandler which is marked as OTF2_MASTER.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Definitions.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Typedef of the struct which keeps all necessary information of
           a global definition writer. Can be used to reference these structs
           from external
 */
typedef struct OTF2_GlobalDefWriter_struct OTF2_GlobalDefWriter;


/** @brief Returns the current number of written definitions of a global
 *         definition writer.
 *
 *  @param writerHandle             Handle to the global definition writer.
 *  @param[out] numberOfDefinitions Storage for the number of definitions.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_GetNumberOfDefinitions( OTF2_GlobalDefWriter* writerHandle,
                                             uint64_t*             numberOfDefinitions );


/** @brief Returns the current number of written location definitions
 *         of a global definition writer.
 *
 *  @param writerHandle           Handle to the global definition writer.
 *  @param[out] numberOfLocations Storage for the number of locations.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_GetNumberOfLocations( OTF2_GlobalDefWriter* writerHandle,
                                           uint64_t*             numberOfLocations );


/** @brief Writes a @ref ClockProperties definition record into the GlobalDefWriter.
 *
 *  Defines the timer resolution and time range of this trace. There will
 *  be no event with a timestamp less than @a globalOffset, and no
 *  event with timestamp greater than (@a globalOffset + @a
 *  traceLength).
 *
 *  @param writerHandle The writer handle.
 *
 *  @param timerResolution Ticks per seconds.
 *  @param globalOffset    A timestamp smaller than all event timestamps.
 *  @param traceLength     A timespan which includes the timespan between the
 *                         smallest and greatest timestamp of all event
 *                         timestamps.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteClockProperties( OTF2_GlobalDefWriter* writerHandle,
                                           uint64_t              timerResolution,
                                           uint64_t              globalOffset,
                                           uint64_t              traceLength );


/** @brief Writes a @ref String definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self   The unique identifier for this @ref String definition.
 *  @param string The string, null terminated.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteString( OTF2_GlobalDefWriter* writerHandle,
                                  OTF2_StringRef        self,
                                  const char*           string );


/** @brief Writes a @ref Attribute definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self The unique identifier for this @ref Attribute definition.
 *  @param name Name of the attribute. References a @ref String definition.
 *  @param type Type of the attribute value.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteAttribute( OTF2_GlobalDefWriter* writerHandle,
                                     OTF2_AttributeRef     self,
                                     OTF2_StringRef        name,
                                     OTF2_Type             type );


/** @brief Writes a @ref SystemTreeNode definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self      The unique identifier for this @ref SystemTreeNode
 *                   definition.
 *  @param name      Free form instance name of this node. References a @ref
 *                   String definition.
 *  @param className Free form class name of this node References a @ref String
 *                   definition.
 *  @param parent    Parent id of this node. May be
 *                   @eref{OTF2_UNDEFINED_SYSTEM_TREE_NODE} to indicate that
 *                   there is no parent. References a @ref SystemTreeNode
 *                   definition.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteSystemTreeNode( OTF2_GlobalDefWriter*  writerHandle,
                                          OTF2_SystemTreeNodeRef self,
                                          OTF2_StringRef         name,
                                          OTF2_StringRef         className,
                                          OTF2_SystemTreeNodeRef parent );


/** @brief Writes a @ref LocationGroup definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self              The unique identifier for this @ref LocationGroup
 *                           definition.
 *  @param name              Name of the group. References a @ref String
 *                           definition.
 *  @param locationGroupType Type of this group.
 *  @param systemTreeParent  Parent of this location group in the system tree.
 *                           References a @ref SystemTreeNode definition.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteLocationGroup( OTF2_GlobalDefWriter*  writerHandle,
                                         OTF2_LocationGroupRef  self,
                                         OTF2_StringRef         name,
                                         OTF2_LocationGroupType locationGroupType,
                                         OTF2_SystemTreeNodeRef systemTreeParent );


/** @brief Writes a @ref Location definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self           The unique identifier for this @ref Location
 *                        definition.
 *  @param name           Name of the location References a @ref String
 *                        definition.
 *  @param locationType   Location type.
 *  @param numberOfEvents Number of events this location has recorded.
 *  @param locationGroup  Location group which includes this location. References
 *                        a @ref LocationGroup definition.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteLocation( OTF2_GlobalDefWriter* writerHandle,
                                    OTF2_LocationRef      self,
                                    OTF2_StringRef        name,
                                    OTF2_LocationType     locationType,
                                    uint64_t              numberOfEvents,
                                    OTF2_LocationGroupRef locationGroup );


/** @brief Writes a @ref Region definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self            The unique identifier for this @ref Region definition.
 *  @param name            Name of the region (demangled name if available).
 *                         References a @ref String definition.
 *  @param canonicalName   Alternative name of the region (e.g. mangled name).
 *                         References a @ref String definition. Since version
 *                         1.1.
 *  @param description     A more detailed description of this region. References
 *                         a @ref String definition.
 *  @param regionRole      Region role. Since version 1.1.
 *  @param paradigm        Paradigm. Since version 1.1.
 *  @param regionFlags     Region flags. Since version 1.1.
 *  @param sourceFile      The source file where this region was declared.
 *                         References a @ref String definition.
 *  @param beginLineNumber Starting line number of this region in the source
 *                         file.
 *  @param endLineNumber   Ending line number of this region in the source file.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteRegion( OTF2_GlobalDefWriter* writerHandle,
                                  OTF2_RegionRef        self,
                                  OTF2_StringRef        name,
                                  OTF2_StringRef        canonicalName,
                                  OTF2_StringRef        description,
                                  OTF2_RegionRole       regionRole,
                                  OTF2_Paradigm         paradigm,
                                  OTF2_RegionFlag       regionFlags,
                                  OTF2_StringRef        sourceFile,
                                  uint32_t              beginLineNumber,
                                  uint32_t              endLineNumber );


/** @brief Writes a @ref Callsite definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self          The unique identifier for this @ref Callsite definition.
 *  @param sourceFile    The source file where this call was made. References a
 *                       @ref String definition.
 *  @param lineNumber    Line number in the source file where this call was made.
 *  @param enteredRegion The region which was called. References a @ref Region
 *                       definition.
 *  @param leftRegion    The region which made the call. References a @ref Region
 *                       definition.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteCallsite( OTF2_GlobalDefWriter* writerHandle,
                                    OTF2_CallsiteRef      self,
                                    OTF2_StringRef        sourceFile,
                                    uint32_t              lineNumber,
                                    OTF2_RegionRef        enteredRegion,
                                    OTF2_RegionRef        leftRegion );


/** @brief Writes a @ref Callpath definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self   The unique identifier for this @ref Callpath definition.
 *  @param parent  References a @ref Callpath definition.
 *  @param region  References a @ref Region definition.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteCallpath( OTF2_GlobalDefWriter* writerHandle,
                                    OTF2_CallpathRef      self,
                                    OTF2_CallpathRef      parent,
                                    OTF2_RegionRef        region );


/** @brief Writes a @ref Group definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self            The unique identifier for this @ref Group definition.
 *  @param name            Name of this group References a @ref String
 *                         definition.
 *  @param groupType       The type of this group. Since version 1.2.
 *  @param paradigm        The paradigm of this communication group. Since
 *                         version 1.2.
 *  @param groupFlags      Flags for this group. Since version 1.2.
 *  @param numberOfMembers The number of members in this group.
 *  @param members         The identifiers of the group members.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteGroup( OTF2_GlobalDefWriter* writerHandle,
                                 OTF2_GroupRef         self,
                                 OTF2_StringRef        name,
                                 OTF2_GroupType        groupType,
                                 OTF2_Paradigm         paradigm,
                                 OTF2_GroupFlag        groupFlags,
                                 uint32_t              numberOfMembers,
                                 const uint64_t*       members );


/** @brief Writes a @ref MetricMember definition record into the GlobalDefWriter.
 *
 *  A metric is defined by a metric member definition. A metric member is
 *  always a member of a metric class. Therefore, a single metric is a
 *  special case of a metric class with only one member. It is not
 *  allowed to reference a metric member id in a metric event, but
 *  only metric class IDs.
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self        The unique identifier for this @ref MetricMember
 *                     definition.
 *  @param name        Name of the metric. References a @ref String definition.
 *  @param description Description of the metric. References a @ref String
 *                     definition.
 *  @param metricType  Metric type: PAPI, etc.
 *  @param metricMode  Metric mode: accumulative, fix, relative, etc.
 *  @param valueType   Type of the value: int64_t, uint64_t, or double.
 *  @param metricBase  The recorded values should be handled in this given base,
 *                     either binary or decimal. This information can be used
 *                     if the value needs to be scaled.
 *  @param exponent    The values inside the Metric events should be scaled by
 *                     the factor base^exponent, to get the value in its base
 *                     unit. For example, if the metric values come in as
 *                     KiBi, than the base should be @eref{OTF2_BASE_BINARY}
 *                     and the exponent 10. Than the writer does not need to
 *                     scale the values up to bytes, but can directly write
 *                     the KiBi values into the Metric event. At reading
 *                     time, the reader can apply the scaling factor to get
 *                     the value in its base unit, ie. in bytes.
 *  @param unit        Unit of the metric. This needs to be the scale free base
 *                     unit, ie. "bytes", "operations", or "seconds". In
 *                     particular this unit should not have any scale prefix.
 *                     References a @ref String definition.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteMetricMember( OTF2_GlobalDefWriter* writerHandle,
                                        OTF2_MetricMemberRef  self,
                                        OTF2_StringRef        name,
                                        OTF2_StringRef        description,
                                        OTF2_MetricType       metricType,
                                        OTF2_MetricMode       metricMode,
                                        OTF2_Type             valueType,
                                        OTF2_MetricBase       metricBase,
                                        int64_t               exponent,
                                        OTF2_StringRef        unit );


/** @brief Writes a @ref MetricClass definition record into the GlobalDefWriter.
 *
 *  For a metric class it is implicitly given that the event stream that
 *  records the metric is also the scope. A metric class can contain
 *  multiple different metrics.
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self             The unique identifier for this @ref MetricClass
 *                          definition.
 *  @param numberOfMetrics  Number of metrics within the set.
 *  @param metricMembers    List of metric members. References a @ref
 *                          MetricMember definition.
 *  @param metricOccurrence Defines occurrence of a metric set.
 *  @param recorderKind     What kind of locations will record this metric class,
 *                          or will this metric class only be recorded by
 *                          metric instances. Since version 1.2.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteMetricClass( OTF2_GlobalDefWriter*       writerHandle,
                                       OTF2_MetricRef              self,
                                       uint8_t                     numberOfMetrics,
                                       const OTF2_MetricMemberRef* metricMembers,
                                       OTF2_MetricOccurrence       metricOccurrence,
                                       OTF2_RecorderKind           recorderKind );


/** @brief Writes a @ref MetricInstance definition record into the GlobalDefWriter.
 *
 *  A metric instance is used to define metrics that are recorded at one
 *  location for multiple locations or for another location. The
 *  occurrence of a metric instance is implicitly of type
 *  @eref{OTF2_METRIC_ASYNCHRONOUS}.
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self        The unique identifier for this @ref MetricClass
 *                     definition.
 *  @param metricClass The instanced @eref{MetricClass}. This metric class must
 *                     be of kind @eref{OTF2_RECORDER_KIND_ABSTRACT}.
 *                     References a @ref MetricClass definition.
 *  @param recorder    Recorder of the metric: location ID. References a @ref
 *                     Location definition.
 *  @param metricScope Defines type of scope: location, location group, system
 *                     tree node, or a generic group of locations.
 *  @param scope       Scope of metric: ID of a location, location group, system
 *                     tree node, or a generic group of locations.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteMetricInstance( OTF2_GlobalDefWriter* writerHandle,
                                          OTF2_MetricRef        self,
                                          OTF2_MetricRef        metricClass,
                                          OTF2_LocationRef      recorder,
                                          OTF2_MetricScope      metricScope,
                                          uint64_t              scope );


/** @brief Writes a @ref Comm definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self   The unique identifier for this @ref Comm definition.
 *  @param name   The name given by calling MPI_Comm_set_name on this
 *                communicator. Or the empty name to indicate that no name
 *                was given. References a @ref String definition.
 *  @param group  The describing MPI group of this MPI communicator  The group
 *                needs to be of type @eref{OTF2_GROUP_TYPE_MPI_GROUP} or
 *                @eref{OTF2_GROUP_TYPE_MPI_COMM_SELF}. References a @ref
 *                Group definition.
 *  @param parent The parent MPI communicator from which this communicator was
 *                created, if any. Use @eref{OTF2_UNDEFINED_COMM} to indicate
 *                no parent. References a @ref Comm definition.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteComm( OTF2_GlobalDefWriter* writerHandle,
                                OTF2_CommRef          self,
                                OTF2_StringRef        name,
                                OTF2_GroupRef         group,
                                OTF2_CommRef          parent );


/** @brief Writes a @ref Parameter definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self          The unique identifier for this @ref Parameter
 *                       definition.
 *  @param name          Name of the parameter (variable name etc.) References a
 *                       @ref String definition.
 *  @param parameterType Type of the parameter, @eref{OTF2_ParameterType} for
 *                       possible types.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteParameter( OTF2_GlobalDefWriter* writerHandle,
                                     OTF2_ParameterRef     self,
                                     OTF2_StringRef        name,
                                     OTF2_ParameterType    parameterType );


/** @brief Writes a @ref RmaWin definition record into the GlobalDefWriter.
 *
 *  A window defines the communication context for any remote-memory
 *  access operation.
 *
 *  @param writerHandle The writer handle.
 *
 *  @param self The unique identifier for this @ref RmaWin definition.
 *  @param name Name, e.g. 'GASPI Queue 1', 'NVidia Card 2', etc.. References a
 *              @ref String definition.
 *  @param comm Communicator object used to create the window. References a @ref
 *              Comm definition.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteRmaWin( OTF2_GlobalDefWriter* writerHandle,
                                  OTF2_RmaWinRef        self,
                                  OTF2_StringRef        name,
                                  OTF2_CommRef          comm );


/** @brief Writes a @ref MetricClassRecorder definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param metricClass Parent @ref MetricClass definition to which this one is a
 *                     supplementary definition. References a @ref
 *                     MetricClass definition.
 *  @param recorder    The location which recorded the referenced metric class.
 *                     References a @ref Location definition.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteMetricClassRecorder( OTF2_GlobalDefWriter* writerHandle,
                                               OTF2_MetricRef        metricClass,
                                               OTF2_LocationRef      recorder );


/** @brief Writes a @ref SystemTreeNodeProperty definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param systemTreeNode Parent @ref SystemTreeNode definition to which this one
 *                        is a supplementary definition. References a @ref
 *                        SystemTreeNode definition.
 *  @param name           Name of the property. References a @ref String
 *                        definition.
 *  @param value          Property value. References a @ref String definition.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteSystemTreeNodeProperty( OTF2_GlobalDefWriter*  writerHandle,
                                                  OTF2_SystemTreeNodeRef systemTreeNode,
                                                  OTF2_StringRef         name,
                                                  OTF2_StringRef         value );


/** @brief Writes a @ref SystemTreeNodeDomain definition record into the GlobalDefWriter.
 *
 *
 *
 *  @param writerHandle The writer handle.
 *
 *  @param systemTreeNode   Parent @ref SystemTreeNode definition to which this
 *                          one is a supplementary definition. References a
 *                          @ref SystemTreeNode definition.
 *
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteSystemTreeNodeDomain( OTF2_GlobalDefWriter*  writerHandle,
                                                OTF2_SystemTreeNodeRef systemTreeNode,
                                                OTF2_SystemTreeDomain  systemTreeDomain );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GLOBAL_DEF_WRITER_H */
