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


#ifndef OTF2_GLOBAL_DEF_READER_CALLBACKS_H
#define OTF2_GLOBAL_DEF_READER_CALLBACKS_H


/**
 *  @file       include/otf2/OTF2_GlobalDefReaderCallbacks.h
 *  @source     templates/OTF2_GlobalDefReaderCallbacks.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This defines the callbacks for the global definition reader.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>
#include <otf2/OTF2_Definitions.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Opaque struct which holdes all global definition record callbacks.
 */
typedef struct OTF2_GlobalDefReaderCallbacks_struct OTF2_GlobalDefReaderCallbacks;


/** @brief Allocates a new struct for the global definition callbacks.
 *
 *  @return A newly allocated struct of type @ref OTF2_GlobalDefReaderCallbacks.
 */
OTF2_GlobalDefReaderCallbacks*
OTF2_GlobalDefReaderCallbacks_New( void );


/** @brief Deallocates a struct for the global definition callbacks.
 *
 *  @param globalDefReaderCallbacks Handle to a struct previously allocated
 *                                  with @ref OTF2_GlobalDefReaderCallbacks_New.
 */
void
OTF2_GlobalDefReaderCallbacks_Delete( OTF2_GlobalDefReaderCallbacks* globalDefReaderCallbacks );


/** @brief Clears a struct for the global definition callbacks.
 *
 *  @param globalDefReaderCallbacks Handle to a struct previously allocated
 *                                  with @ref OTF2_GlobalDefReaderCallbacks_New.
 */
void
OTF2_GlobalDefReaderCallbacks_Clear( OTF2_GlobalDefReaderCallbacks* globalDefReaderCallbacks );


/** @brief Function pointer definition for the callback which is triggered by
 *         an unknown definition record.
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Unknown )( void* userData );


/** @brief Registers the callback for an unknown definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param unknownCallback          Function which should be called for all
 *                                  Unknown definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetUnknownCallback(
    OTF2_GlobalDefReaderCallbacks*       globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Unknown unknownCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref ClockProperties definition record.
 *
 *  Defines the timer resolution and time range of this trace. There will
 *  be no event with a timestamp less than @a globalOffset, and no
 *  event with timestamp greater than (@a globalOffset + @a
 *  traceLength).
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @param timerResolution Ticks per seconds.
 *  @param globalOffset    A timestamp smaller than all event timestamps.
 *  @param traceLength     A timespan which includes the timespan between the
 *                         smallest and greatest timestamp of all event
 *                         timestamps.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_ClockProperties )( void*    userData,
                                                   uint64_t timerResolution,
                                                   uint64_t globalOffset,
                                                   uint64_t traceLength );


/** @brief Registers the callback for the @ref ClockProperties definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param clockPropertiesCallback    Function which should be called for all
 *                                  @ref ClockProperties definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback(
    OTF2_GlobalDefReaderCallbacks*               globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_ClockProperties clockPropertiesCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref String definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @param self   The unique identifier for this @ref String definition.
 *  @param string The string, null terminated.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_String )( void*          userData,
                                          OTF2_StringRef self,
                                          const char*    string );


/** @brief Registers the callback for the @ref String definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param stringCallback    Function which should be called for all
 *                                  @ref String definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetStringCallback(
    OTF2_GlobalDefReaderCallbacks*      globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_String stringCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Attribute definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @param self The unique identifier for this @ref Attribute definition.
 *  @param name Name of the attribute. References a @ref String definition.
 *  @param type Type of the attribute value.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Attribute )( void*             userData,
                                             OTF2_AttributeRef self,
                                             OTF2_StringRef    name,
                                             OTF2_Type         type );


/** @brief Registers the callback for the @ref Attribute definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param attributeCallback    Function which should be called for all
 *                                  @ref Attribute definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetAttributeCallback(
    OTF2_GlobalDefReaderCallbacks*         globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Attribute attributeCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref SystemTreeNode definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_SystemTreeNode )( void*                  userData,
                                                  OTF2_SystemTreeNodeRef self,
                                                  OTF2_StringRef         name,
                                                  OTF2_StringRef         className,
                                                  OTF2_SystemTreeNodeRef parent );


/** @brief Registers the callback for the @ref SystemTreeNode definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param systemTreeNodeCallback    Function which should be called for all
 *                                  @ref SystemTreeNode definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback(
    OTF2_GlobalDefReaderCallbacks*              globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_SystemTreeNode systemTreeNodeCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref LocationGroup definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_LocationGroup )( void*                  userData,
                                                 OTF2_LocationGroupRef  self,
                                                 OTF2_StringRef         name,
                                                 OTF2_LocationGroupType locationGroupType,
                                                 OTF2_SystemTreeNodeRef systemTreeParent );


/** @brief Registers the callback for the @ref LocationGroup definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param locationGroupCallback    Function which should be called for all
 *                                  @ref LocationGroup definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback(
    OTF2_GlobalDefReaderCallbacks*             globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_LocationGroup locationGroupCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Location definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Location )( void*                 userData,
                                            OTF2_LocationRef      self,
                                            OTF2_StringRef        name,
                                            OTF2_LocationType     locationType,
                                            uint64_t              numberOfEvents,
                                            OTF2_LocationGroupRef locationGroup );


/** @brief Registers the callback for the @ref Location definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param locationCallback    Function which should be called for all
 *                                  @ref Location definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetLocationCallback(
    OTF2_GlobalDefReaderCallbacks*        globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Location locationCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Region definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Region )( void*           userData,
                                          OTF2_RegionRef  self,
                                          OTF2_StringRef  name,
                                          OTF2_StringRef  canonicalName,
                                          OTF2_StringRef  description,
                                          OTF2_RegionRole regionRole,
                                          OTF2_Paradigm   paradigm,
                                          OTF2_RegionFlag regionFlags,
                                          OTF2_StringRef  sourceFile,
                                          uint32_t        beginLineNumber,
                                          uint32_t        endLineNumber );


/** @brief Registers the callback for the @ref Region definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param regionCallback    Function which should be called for all
 *                                  @ref Region definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetRegionCallback(
    OTF2_GlobalDefReaderCallbacks*      globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Region regionCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Callsite definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Callsite )( void*            userData,
                                            OTF2_CallsiteRef self,
                                            OTF2_StringRef   sourceFile,
                                            uint32_t         lineNumber,
                                            OTF2_RegionRef   enteredRegion,
                                            OTF2_RegionRef   leftRegion );


/** @brief Registers the callback for the @ref Callsite definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param callsiteCallback    Function which should be called for all
 *                                  @ref Callsite definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetCallsiteCallback(
    OTF2_GlobalDefReaderCallbacks*        globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Callsite callsiteCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Callpath definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @param self   The unique identifier for this @ref Callpath definition.
 *  @param parent  References a @ref Callpath definition.
 *  @param region  References a @ref Region definition.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Callpath )( void*            userData,
                                            OTF2_CallpathRef self,
                                            OTF2_CallpathRef parent,
                                            OTF2_RegionRef   region );


/** @brief Registers the callback for the @ref Callpath definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param callpathCallback    Function which should be called for all
 *                                  @ref Callpath definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetCallpathCallback(
    OTF2_GlobalDefReaderCallbacks*        globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Callpath callpathCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Group definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Group )( void*           userData,
                                         OTF2_GroupRef   self,
                                         OTF2_StringRef  name,
                                         OTF2_GroupType  groupType,
                                         OTF2_Paradigm   paradigm,
                                         OTF2_GroupFlag  groupFlags,
                                         uint32_t        numberOfMembers,
                                         const uint64_t* members );


/** @brief Registers the callback for the @ref Group definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param groupCallback    Function which should be called for all
 *                                  @ref Group definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetGroupCallback(
    OTF2_GlobalDefReaderCallbacks*     globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Group groupCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref MetricMember definition record.
 *
 *  A metric is defined by a metric member definition. A metric member is
 *  always a member of a metric class. Therefore, a single metric is a
 *  special case of a metric class with only one member. It is not
 *  allowed to reference a metric member id in a metric event, but
 *  only metric class IDs.
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_MetricMember )( void*                userData,
                                                OTF2_MetricMemberRef self,
                                                OTF2_StringRef       name,
                                                OTF2_StringRef       description,
                                                OTF2_MetricType      metricType,
                                                OTF2_MetricMode      metricMode,
                                                OTF2_Type            valueType,
                                                OTF2_MetricBase      metricBase,
                                                int64_t              exponent,
                                                OTF2_StringRef       unit );


/** @brief Registers the callback for the @ref MetricMember definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param metricMemberCallback    Function which should be called for all
 *                                  @ref MetricMember definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetMetricMemberCallback(
    OTF2_GlobalDefReaderCallbacks*            globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_MetricMember metricMemberCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref MetricClass definition record.
 *
 *  For a metric class it is implicitly given that the event stream that
 *  records the metric is also the scope. A metric class can contain
 *  multiple different metrics.
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_MetricClass )( void*                       userData,
                                               OTF2_MetricRef              self,
                                               uint8_t                     numberOfMetrics,
                                               const OTF2_MetricMemberRef* metricMembers,
                                               OTF2_MetricOccurrence       metricOccurrence,
                                               OTF2_RecorderKind           recorderKind );


/** @brief Registers the callback for the @ref MetricClass definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param metricClassCallback    Function which should be called for all
 *                                  @ref MetricClass definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetMetricClassCallback(
    OTF2_GlobalDefReaderCallbacks*           globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_MetricClass metricClassCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref MetricInstance definition record.
 *
 *  A metric instance is used to define metrics that are recorded at one
 *  location for multiple locations or for another location. The
 *  occurrence of a metric instance is implicitly of type
 *  @eref{OTF2_METRIC_ASYNCHRONOUS}.
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_MetricInstance )( void*            userData,
                                                  OTF2_MetricRef   self,
                                                  OTF2_MetricRef   metricClass,
                                                  OTF2_LocationRef recorder,
                                                  OTF2_MetricScope metricScope,
                                                  uint64_t         scope );


/** @brief Registers the callback for the @ref MetricInstance definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param metricInstanceCallback    Function which should be called for all
 *                                  @ref MetricInstance definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetMetricInstanceCallback(
    OTF2_GlobalDefReaderCallbacks*              globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_MetricInstance metricInstanceCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Comm definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Comm )( void*          userData,
                                        OTF2_CommRef   self,
                                        OTF2_StringRef name,
                                        OTF2_GroupRef  group,
                                        OTF2_CommRef   parent );


/** @brief Registers the callback for the @ref Comm definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param commCallback    Function which should be called for all
 *                                  @ref Comm definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetCommCallback(
    OTF2_GlobalDefReaderCallbacks*    globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Comm commCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Parameter definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_Parameter )( void*              userData,
                                             OTF2_ParameterRef  self,
                                             OTF2_StringRef     name,
                                             OTF2_ParameterType parameterType );


/** @brief Registers the callback for the @ref Parameter definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param parameterCallback    Function which should be called for all
 *                                  @ref Parameter definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetParameterCallback(
    OTF2_GlobalDefReaderCallbacks*         globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Parameter parameterCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref RmaWin definition record.
 *
 *  A window defines the communication context for any remote-memory
 *  access operation.
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @param self The unique identifier for this @ref RmaWin definition.
 *  @param name Name, e.g. 'GASPI Queue 1', 'NVidia Card 2', etc.. References a
 *              @ref String definition.
 *  @param comm Communicator object used to create the window. References a @ref
 *              Comm definition.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_RmaWin )( void*          userData,
                                          OTF2_RmaWinRef self,
                                          OTF2_StringRef name,
                                          OTF2_CommRef   comm );


/** @brief Registers the callback for the @ref RmaWin definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param rmaWinCallback    Function which should be called for all
 *                                  @ref RmaWin definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetRmaWinCallback(
    OTF2_GlobalDefReaderCallbacks*      globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_RmaWin rmaWinCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref MetricClassRecorder definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @param metricClass Parent @ref MetricClass definition to which this one is a
 *                     supplementary definition. References a @ref
 *                     MetricClass definition.
 *  @param recorder    The location which recorded the referenced metric class.
 *                     References a @ref Location definition.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_MetricClassRecorder )( void*            userData,
                                                       OTF2_MetricRef   metricClass,
                                                       OTF2_LocationRef recorder );


/** @brief Registers the callback for the @ref MetricClassRecorder definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param metricClassRecorderCallback    Function which should be called for all
 *                                  @ref MetricClassRecorder definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetMetricClassRecorderCallback(
    OTF2_GlobalDefReaderCallbacks*                   globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_MetricClassRecorder metricClassRecorderCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref SystemTreeNodeProperty definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
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
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_SystemTreeNodeProperty )( void*                  userData,
                                                          OTF2_SystemTreeNodeRef systemTreeNode,
                                                          OTF2_StringRef         name,
                                                          OTF2_StringRef         value );


/** @brief Registers the callback for the @ref SystemTreeNodeProperty definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param systemTreeNodePropertyCallback    Function which should be called for all
 *                                  @ref SystemTreeNodeProperty definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodePropertyCallback(
    OTF2_GlobalDefReaderCallbacks*                      globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_SystemTreeNodeProperty systemTreeNodePropertyCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref SystemTreeNodeDomain definition record.
 *
 *
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @param systemTreeNode   Parent @ref SystemTreeNode definition to which this
 *                          one is a supplementary definition. References a
 *                          @ref SystemTreeNode definition.
 *
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_SystemTreeNodeDomain )( void*                  userData,
                                                        OTF2_SystemTreeNodeRef systemTreeNode,
                                                        OTF2_SystemTreeDomain  systemTreeDomain );


/** @brief Registers the callback for the @ref SystemTreeNodeDomain definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param systemTreeNodeDomainCallback    Function which should be called for all
 *                                  @ref SystemTreeNodeDomain definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeDomainCallback(
    OTF2_GlobalDefReaderCallbacks*                    globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_SystemTreeNodeDomain systemTreeNodeDomainCallback );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GLOBAL_DEF_READER_CALLBACKS_H */
