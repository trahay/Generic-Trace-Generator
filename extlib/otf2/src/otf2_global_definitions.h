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


#ifndef OTF2_INTERNAL_GLOBAL_DEFINITIONS_H
#define OTF2_INTERNAL_GLOBAL_DEFINITIONS_H


/**
 *  @file       src/otf2_global_definitions.h
 *  @source     templates/otf2_global_definitions.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 */


/** @brief Wrapper for the OTF2_GlobalDefinitionType_enum
 */
typedef uint8_t OTF2_GlobalDefinitionType;


/** @brief An enum which holds all values that are possible global definition types.
 */
enum OTF2_GlobalDefinitionType_enum
{
    /* First five entries are reserved for OTF2 internals */

    /* OTF2 Records */
    OTF2_GLOBAL_DEF_CLOCK_PROPERTIES          = 5,  /**< */
    OTF2_GLOBAL_DEF_STRING                    = 10, /**< */
    OTF2_GLOBAL_DEF_ATTRIBUTE                 = 11, /**< */
    OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE          = 12, /**< */
    OTF2_GLOBAL_DEF_LOCATION_GROUP            = 13, /**< */
    OTF2_GLOBAL_DEF_LOCATION                  = 14, /**< */
    OTF2_GLOBAL_DEF_REGION                    = 15, /**< */
    OTF2_GLOBAL_DEF_CALLSITE                  = 16, /**< */
    OTF2_GLOBAL_DEF_CALLPATH                  = 17, /**< */
    OTF2_GLOBAL_DEF_GROUP                     = 18, /**< */
    OTF2_GLOBAL_DEF_METRIC_MEMBER             = 19, /**< */
    OTF2_GLOBAL_DEF_METRIC_CLASS              = 20, /**< */
    OTF2_GLOBAL_DEF_METRIC_INSTANCE           = 21, /**< */
    OTF2_GLOBAL_DEF_COMM                      = 22, /**< */
    OTF2_GLOBAL_DEF_PARAMETER                 = 23, /**< */
    OTF2_GLOBAL_DEF_RMA_WIN                   = 24, /**< */
    OTF2_GLOBAL_DEF_METRIC_CLASS_RECORDER     = 25, /**< */
    OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE_PROPERTY = 26, /**< */
    OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE_DOMAIN   = 27, /**< */

    OTF2_GLOBAL_DEF_MAX_ID
};


/** @brief ClockProperties definition.
 */
typedef struct OTF2_GlobalDefClockProperties_struct
{
    uint64_t timer_resolution;
    uint64_t global_offset;
    uint64_t trace_length;
} OTF2_GlobalDefClockProperties;


/** @brief String definition.
 */
typedef struct OTF2_GlobalDefString_struct
{
    OTF2_StringRef self;
    const char*    string;
} OTF2_GlobalDefString;


/** @brief Attribute definition.
 */
typedef struct OTF2_GlobalDefAttribute_struct
{
    OTF2_AttributeRef self;
    OTF2_StringRef    name;
    OTF2_Type         type;
} OTF2_GlobalDefAttribute;


/** @brief SystemTreeNode definition.
 */
typedef struct OTF2_GlobalDefSystemTreeNode_struct
{
    OTF2_SystemTreeNodeRef self;
    OTF2_StringRef         name;
    OTF2_StringRef         class_name;
    OTF2_SystemTreeNodeRef parent;
} OTF2_GlobalDefSystemTreeNode;


/** @brief LocationGroup definition.
 */
typedef struct OTF2_GlobalDefLocationGroup_struct
{
    OTF2_LocationGroupRef  self;
    OTF2_StringRef         name;
    OTF2_LocationGroupType location_group_type;
    OTF2_SystemTreeNodeRef system_tree_parent;
} OTF2_GlobalDefLocationGroup;


/** @brief Location definition.
 */
typedef struct OTF2_GlobalDefLocation_struct
{
    OTF2_LocationRef      self;
    OTF2_StringRef        name;
    OTF2_LocationType     location_type;
    uint64_t              number_of_events;
    OTF2_LocationGroupRef location_group;
} OTF2_GlobalDefLocation;


/** @brief Region definition.
 */
typedef struct OTF2_GlobalDefRegion_struct
{
    OTF2_RegionRef  self;
    OTF2_StringRef  name;
    OTF2_StringRef  description;
    OTF2_RegionType region_type;
    OTF2_StringRef  source_file;
    uint32_t        begin_line_number;
    uint32_t        end_line_number;
    OTF2_StringRef  canonical_name;
    OTF2_RegionRole region_role;
    OTF2_Paradigm   paradigm;
    OTF2_RegionFlag region_flags;
} OTF2_GlobalDefRegion;


/** @brief Callsite definition.
 */
typedef struct OTF2_GlobalDefCallsite_struct
{
    OTF2_CallsiteRef self;
    OTF2_StringRef   source_file;
    uint32_t         line_number;
    OTF2_RegionRef   entered_region;
    OTF2_RegionRef   left_region;
} OTF2_GlobalDefCallsite;


/** @brief Callpath definition.
 */
typedef struct OTF2_GlobalDefCallpath_struct
{
    OTF2_CallpathRef self;
    OTF2_CallpathRef parent;
    OTF2_RegionRef   region;
} OTF2_GlobalDefCallpath;


/** @brief Group definition.
 */
typedef struct OTF2_GlobalDefGroup_struct
{
    OTF2_GroupRef       self;
    OTF2_StringRef      name;
    OTF2_GroupTypePre12 group_type_pre12;
    uint32_t            number_of_members;
    uint64_t*           members;
    OTF2_GroupType      group_type;
    OTF2_Paradigm       paradigm;
    OTF2_GroupFlag      group_flags;
} OTF2_GlobalDefGroup;


/** @brief MetricMember definition.
 */
typedef struct OTF2_GlobalDefMetricMember_struct
{
    OTF2_MetricMemberRef self;
    OTF2_StringRef       name;
    OTF2_StringRef       description;
    OTF2_MetricType      metric_type;
    OTF2_MetricMode      metric_mode;
    OTF2_Type            value_type;
    OTF2_MetricBase      metric_base;
    int64_t              exponent;
    OTF2_StringRef       unit;
} OTF2_GlobalDefMetricMember;


/** @brief MetricClass definition.
 */
typedef struct OTF2_GlobalDefMetricClass_struct
{
    OTF2_MetricRef        self;
    uint8_t               number_of_metrics;
    OTF2_MetricMemberRef* metric_members;
    OTF2_MetricOccurrence metric_occurrence;
    OTF2_RecorderKind     recorder_kind;
} OTF2_GlobalDefMetricClass;


/** @brief MetricInstance definition.
 */
typedef struct OTF2_GlobalDefMetricInstance_struct
{
    OTF2_MetricRef   self;
    OTF2_MetricRef   metric_class;
    OTF2_LocationRef recorder;
    OTF2_MetricScope metric_scope;
    uint64_t         scope;
} OTF2_GlobalDefMetricInstance;


/** @brief Comm definition.
 */
typedef struct OTF2_GlobalDefComm_struct
{
    OTF2_CommRef   self;
    OTF2_StringRef name;
    OTF2_GroupRef  group;
    OTF2_CommRef   parent;
} OTF2_GlobalDefComm;


/** @brief Parameter definition.
 */
typedef struct OTF2_GlobalDefParameter_struct
{
    OTF2_ParameterRef  self;
    OTF2_StringRef     name;
    OTF2_ParameterType parameter_type;
} OTF2_GlobalDefParameter;


/** @brief RmaWin definition.
 */
typedef struct OTF2_GlobalDefRmaWin_struct
{
    OTF2_RmaWinRef self;
    OTF2_StringRef name;
    OTF2_CommRef   comm;
} OTF2_GlobalDefRmaWin;


/** @brief MetricClassRecorder definition.
 */
typedef struct OTF2_GlobalDefMetricClassRecorder_struct
{
    OTF2_MetricRef   metric_class;
    OTF2_LocationRef recorder;
} OTF2_GlobalDefMetricClassRecorder;


/** @brief SystemTreeNodeProperty definition.
 */
typedef struct OTF2_GlobalDefSystemTreeNodeProperty_struct
{
    OTF2_SystemTreeNodeRef system_tree_node;
    OTF2_StringRef         name;
    OTF2_StringRef         value;
} OTF2_GlobalDefSystemTreeNodeProperty;


/** @brief SystemTreeNodeDomain definition.
 */
typedef struct OTF2_GlobalDefSystemTreeNodeDomain_struct
{
    OTF2_SystemTreeNodeRef system_tree_node;
    OTF2_SystemTreeDomain  system_tree_domain;
} OTF2_GlobalDefSystemTreeNodeDomain;


struct OTF2_GlobalDefReaderCallbacks_struct
{
    OTF2_GlobalDefReaderCallback_Unknown                unknown;
    OTF2_GlobalDefReaderCallback_ClockProperties        clock_properties;
    OTF2_GlobalDefReaderCallback_String                 string;
    OTF2_GlobalDefReaderCallback_Attribute              attribute;
    OTF2_GlobalDefReaderCallback_SystemTreeNode         system_tree_node;
    OTF2_GlobalDefReaderCallback_LocationGroup          location_group;
    OTF2_GlobalDefReaderCallback_Location               location;
    OTF2_GlobalDefReaderCallback_Region                 region;
    OTF2_GlobalDefReaderCallback_Callsite               callsite;
    OTF2_GlobalDefReaderCallback_Callpath               callpath;
    OTF2_GlobalDefReaderCallback_Group                  group;
    OTF2_GlobalDefReaderCallback_MetricMember           metric_member;
    OTF2_GlobalDefReaderCallback_MetricClass            metric_class;
    OTF2_GlobalDefReaderCallback_MetricInstance         metric_instance;
    OTF2_GlobalDefReaderCallback_Comm                   comm;
    OTF2_GlobalDefReaderCallback_Parameter              parameter;
    OTF2_GlobalDefReaderCallback_RmaWin                 rma_win;
    OTF2_GlobalDefReaderCallback_MetricClassRecorder    metric_class_recorder;
    OTF2_GlobalDefReaderCallback_SystemTreeNodeProperty system_tree_node_property;
    OTF2_GlobalDefReaderCallback_SystemTreeNodeDomain   system_tree_node_domain;
};


#endif /* OTF2_INTERNAL_GLOBAL_DEFINITIONS_H */
