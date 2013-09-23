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


#ifndef OTF2_GENERAL_DEFINITIONS_H
#define OTF2_GENERAL_DEFINITIONS_H


/**
 *  @file       include/otf2/OTF2_GeneralDefinitions.h
 *  @source     templates/OTF2_GeneralDefinitions.tmpl.h
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This header file provides general definitions which should be
 *              accessible in all internal and external modules.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* ___ OTF2 library version information _____________________________________ */


/** @name OTF2 library version.
 *  @{ */
#define OTF2_VERSION_MAJOR  1
#define OTF2_VERSION_MINOR  2
#define OTF2_VERSION_BUGFIX 1
#define OTF2_VERSION_SUFFIX ""
#define OTF2_VERSION        "1.2.1"
/** @} */


/* ___ Symbols for undefined values _________________________________________ */


/** @name Standard undefined values for basic data types.
 *  @{ */
#define OTF2_UNDEFINED_UINT8  ( ( uint8_t )( ~( ( uint8_t )0u ) ) )
#define OTF2_UNDEFINED_UINT16 ( ( uint16_t )( ~( ( uint16_t )0u ) ) )
#define OTF2_UNDEFINED_UINT32 ( ( uint32_t )( ~( ( uint32_t )0u ) ) )
#define OTF2_UNDEFINED_UINT64 ( ( uint64_t )( ~( ( uint64_t )0u ) ) )
/** @} */


/** Undefined value for enums */
#define OTF2_UNDEFINED_TYPE OTF2_UNDEFINED_UINT8


/** @brief OTF2 time stamp. */
typedef uint64_t OTF2_TimeStamp;
#define OTF2_UNDEFINED_TIMESTAMP OTF2_UNDEFINED_UINT64


/** @brief Type used to indicate a reference to a @ref String definition */
typedef uint32_t OTF2_StringRef;
/** @brief The invalid value for a reference to a @ref String definition */
#define OTF2_UNDEFINED_STRING ( ( OTF2_StringRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref Attribute definition */
typedef uint32_t OTF2_AttributeRef;
/** @brief The invalid value for a reference to a @ref Attribute definition */
#define OTF2_UNDEFINED_ATTRIBUTE ( ( OTF2_AttributeRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref SystemTreeNode definition */
typedef uint32_t OTF2_SystemTreeNodeRef;
/** @brief The invalid value for a reference to a @ref SystemTreeNode definition */
#define OTF2_UNDEFINED_SYSTEM_TREE_NODE ( ( OTF2_SystemTreeNodeRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref LocationGroup definition */
typedef uint32_t OTF2_LocationGroupRef;
/** @brief The invalid value for a reference to a @ref LocationGroup definition */
#define OTF2_UNDEFINED_LOCATION_GROUP ( ( OTF2_LocationGroupRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref Location definition */
typedef uint64_t OTF2_LocationRef;
/** @brief The invalid value for a reference to a @ref Location definition */
#define OTF2_UNDEFINED_LOCATION ( ( OTF2_LocationRef )OTF2_UNDEFINED_UINT64 )


/** @brief Type used to indicate a reference to a @ref Region definition */
typedef uint32_t OTF2_RegionRef;
/** @brief The invalid value for a reference to a @ref Region definition */
#define OTF2_UNDEFINED_REGION ( ( OTF2_RegionRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref Callsite definition */
typedef uint32_t OTF2_CallsiteRef;
/** @brief The invalid value for a reference to a @ref Callsite definition */
#define OTF2_UNDEFINED_CALLSITE ( ( OTF2_CallsiteRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref Callpath definition */
typedef uint32_t OTF2_CallpathRef;
/** @brief The invalid value for a reference to a @ref Callpath definition */
#define OTF2_UNDEFINED_CALLPATH ( ( OTF2_CallpathRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref Group definition */
typedef uint32_t OTF2_GroupRef;
/** @brief The invalid value for a reference to a @ref Group definition */
#define OTF2_UNDEFINED_GROUP ( ( OTF2_GroupRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref MetricMember definition */
typedef uint32_t OTF2_MetricMemberRef;
/** @brief The invalid value for a reference to a @ref MetricMember definition */
#define OTF2_UNDEFINED_METRIC_MEMBER ( ( OTF2_MetricMemberRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref MetricClass, or a @ref MetricInstance definition */
typedef uint32_t OTF2_MetricRef;
/** @brief The invalid value for a reference to a @ref MetricClass, or a @ref MetricInstance definition */
#define OTF2_UNDEFINED_METRIC ( ( OTF2_MetricRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref Comm definition */
typedef uint32_t OTF2_CommRef;
/** @brief The invalid value for a reference to a @ref Comm definition */
#define OTF2_UNDEFINED_COMM ( ( OTF2_CommRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref Parameter definition */
typedef uint32_t OTF2_ParameterRef;
/** @brief The invalid value for a reference to a @ref Parameter definition */
#define OTF2_UNDEFINED_PARAMETER ( ( OTF2_ParameterRef )OTF2_UNDEFINED_UINT32 )


/** @brief Type used to indicate a reference to a @ref RmaWin definition */
typedef uint32_t OTF2_RmaWinRef;
/** @brief The invalid value for a reference to a @ref RmaWin definition */
#define OTF2_UNDEFINED_RMA_WIN ( ( OTF2_RmaWinRef )OTF2_UNDEFINED_UINT32 )


/* ___ Declarations of opaque OTF2 objects __________________________________ */


/** @brief OTF2 local event reader handle. */
typedef struct OTF2_EvtReader_struct OTF2_EvtReader;


/** @brief OTF2 global event reader handle. */
typedef struct OTF2_GlobalEvtReader_struct OTF2_GlobalEvtReader;


/** @brief OTF2 local definition reader handle. */
typedef struct OTF2_DefReader_struct OTF2_DefReader;


/** @brief OTF2 global definition reader handle. */
typedef struct OTF2_GlobalDefReader_struct OTF2_GlobalDefReader;


/** @brief OTF2 local snap reader handle. */
typedef struct OTF2_SnapReader_struct OTF2_SnapReader;


/** @brief OTF2 global snap reader handle. */
typedef struct OTF2_GlobalSnapReader_struct OTF2_GlobalSnapReader;


/** @brief OTF2 marker reader handle. */
typedef struct OTF2_MarkerReader_struct OTF2_MarkerReader;


/* ___ Definitions related to the file interaction __________________________ */


/** @brief Defines the minimum size of a chunk.*/
#define OTF2_CHUNK_SIZE_MIN ( uint64_t )( 256 * 1024 )


/** @brief Defines the maximum size of a chunk. */
#define OTF2_CHUNK_SIZE_MAX ( uint64_t )( 1024 * 1024 * 16 )


/** @brief Defines which file substrate is used.
 *  Please see @ref OTF2_FileSubstrate_enum for a detailed description. */
typedef uint8_t OTF2_FileSubstrate;


/** @brief Defines which file substrate is used.
 *  Please note: At the moment only the posix and none interfaces are
    implemented. */
enum OTF2_FileSubstrate_enum
{
    /** Undefined. */
    OTF2_SUBSTRATE_UNDEFINED = 0,
    /** Use standard posix file interface. */
    OTF2_SUBSTRATE_POSIX     = 1,
    /** Use the interface of the sionlib to write many logical files into few
     *  physical files. */
    OTF2_SUBSTRATE_SION      = 2,
    /** Do not use any file interface. No data is written to a file. */
    OTF2_SUBSTRATE_NONE      = 3
};


/** @brief Defines how to interact with files.
 *  Please see @ref OTF2_FileMode_enum for a detailed description. */
typedef uint8_t OTF2_FileMode;


/** @brief Defines how to interact with files. */
enum OTF2_FileMode_enum
{
    /** Open a file in write-only mode. */
    OTF2_FILEMODE_WRITE  = 0,
    /** Open a file in read-only mode. */
    OTF2_FILEMODE_READ   = 1,
    /** Open a file in write-read mode. */
    OTF2_FILEMODE_MODIFY = 2
};


/** @brief Defines which compression is used.
 *  Please see @ref OTF2_Compression_enum for a detailed description. */
typedef uint8_t OTF2_Compression;


/** @brief Defines which compression is used. */
enum OTF2_Compression_enum
{
    /** @brief Undefined. */
    OTF2_COMPRESSION_UNDEFINED = 0,
    /** @brief No compression is used. */
    OTF2_COMPRESSION_NONE      = 1,
    /** @brief Use zlib compression. */
    OTF2_COMPRESSION_ZLIB      = 2
};


/** @brief Defines which file type is used.
 *  Please see @ref OTF2_FileType_enum for a detailed description. */
typedef uint8_t OTF2_FileType;

/** @brief Defines which file type is used.
 */
enum OTF2_FileType_enum
{
    /** @brief Represents the type for the anchor file (.otf2).
     *  Does has a undefined location ID
     */
    OTF2_FILETYPE_ANCHOR      = 0,

    /** @brief Represents the type for the global definition file (.def).
     *  Does has a undefined location ID
     */
    OTF2_FILETYPE_GLOBAL_DEFS = 1,

    /** @brief Represents the type for a local definition file (.def). */
    OTF2_FILETYPE_LOCAL_DEFS  = 2,

    /** @brief Represents the type for a event file (.evt). */
    OTF2_FILETYPE_EVENTS      = 3,

    /** @brief Represents the type for a snapshot file (.snap). */
    OTF2_FILETYPE_SNAPSHOTS   = 4,

    /** @brief Represents the type for a thumb file (.thumb). */
    OTF2_FILETYPE_THUMBNAIL   = 5,

    /** @brief Represents the type for a marker file (.marker). */
    OTF2_FILETYPE_MARKER      = 6
};


/** @brief Return value to indicate that the record reading should be
 *         interrupted.
 *
 * Returning @eref{OTF2_CALLBACK_INTERRUPT} will stop reading more events, if functions
 * like:
 * - @ref OTF2_Reader_ReadLocalEvents
 * - @ref OTF2_Reader_ReadAllLocalEvents
 * - @ref OTF2_Reader_ReadLocalEventsBackward
 * - @ref OTF2_Reader_ReadGlobalEvents
 * - @ref OTF2_Reader_ReadAllGlobalEvents
 * - @ref OTF2_Reader_ReadLocalDefinitions
 * - @ref OTF2_Reader_ReadAllLocalDefinitions
 * - @ref OTF2_Reader_ReadGlobalDefinitions
 * - @ref OTF2_Reader_ReadAllGlobalDefinitions
 * where called. The return value for these functions is
 * @eref{OTF2_ERROR_INTERRUPTED_BY_CALLBACK} in this case. It is valid
 * to call any reader functions in such a condition again.
 */
typedef enum OTF2_CallbackCode_enum
{
    /** @brief Record reading can continue. */
    OTF2_CALLBACK_SUCCESS   = 0,

    /** @brief Interrupt record reading.
     *
     *  Control returns to the caller of the read function
     *  with error OTF2_ERROR_INTERRUPTED_BY_CALLBACK to signal this.
     *  The actual value can be any except OTF2_CALLBACK_SUCCESS.
     */
    OTF2_CALLBACK_INTERRUPT = !OTF2_CALLBACK_SUCCESS
} OTF2_CallbackCode;


/* ___ Definitions related to the memory buffer ______________________________ */


/** @brief Defines whether the recorded data is flushed to a file or not. Please
    see OTF2_Fllushtype_enum for a detailed description. */
typedef uint8_t OTF2_FlushType;


/** @brief Defines whether the recorded data is flushed to a file or not. */
enum OTF2_FlushType_enum
{
    /** @brief Flushing will be supressed when running out of memory. */
    OTF2_NO_FLUSH = 0,
    /** @brief Recorded data is flushed when running out of memory. */
    OTF2_FLUSH    = 1
};


/** @brief Wrapper for enum @ref OTF2_MappingType_enum. */
typedef uint8_t OTF2_MappingType;

/**
 * @brief Possible mappings from local to global identifiers.
 *
 * @since Version 1.0
 */
enum OTF2_MappingType_enum
{
    /** @brief Mapping of string identifiers.
     */
    OTF2_MAPPING_STRING    = 0,
    /** @brief Mapping of attribute identifiers.
     */
    OTF2_MAPPING_ATTRIBUTE = 1,
    /** @brief Mapping of location identifiers.
     */
    OTF2_MAPPING_LOCATION  = 2,
    /** @brief Mapping of region identifiers.
     */
    OTF2_MAPPING_REGION    = 3,
    /** @brief Mapping of group identifiers.
     */
    OTF2_MAPPING_GROUP     = 4,
    /** @brief Mapping of metric identifiers.
     */
    OTF2_MAPPING_METRIC    = 5,
    /** @brief Mapping of MPI communicator identifiers.
     */
    OTF2_MAPPING_COMM      = 6,
    /** @brief Mapping of parameter identifiers.
     */
    OTF2_MAPPING_PARAMETER = 7,
    /** @brief Mapping of RMA window identifiers.
     */
    OTF2_MAPPING_RMA_WIN   = 8,

    /** @brief Max entry.
     */
    OTF2_MAPPING_MAX       = 9
};


/** @brief Wrapper for enum @ref OTF2_Type_enum. */
typedef uint8_t OTF2_Type;

/**
 * @brief OTF2 basic data types.
 *
 * @since Version 1.0
 */
enum OTF2_Type_enum
{
    /** @brief Undefined type.
     */
    OTF2_TYPE_NONE      = 0,
    /** @brief Unsigned 8-bit integer.
     */
    OTF2_TYPE_UINT8     = 1,
    /** @brief Unsigned 16-bit integer.
     */
    OTF2_TYPE_UINT16    = 2,
    /** @brief Unsigned 32-bit integer.
     */
    OTF2_TYPE_UINT32    = 3,
    /** @brief Unsigned 64-bit integer.
     */
    OTF2_TYPE_UINT64    = 4,
    /** @brief Signed 8-bit integer.
     */
    OTF2_TYPE_INT8      = 5,
    /** @brief Signed 16-bit integer.
     */
    OTF2_TYPE_INT16     = 6,
    /** @brief Signed 32-bit integer.
     */
    OTF2_TYPE_INT32     = 7,
    /** @brief Signed 64-bit integer.
     */
    OTF2_TYPE_INT64     = 8,
    /** @brief 32-bit floating point value.
     */
    OTF2_TYPE_FLOAT     = 9,
    /** @brief 64-bit floating point value.
     */
    OTF2_TYPE_DOUBLE    = 10,
    /** @brief
     */
    OTF2_TYPE_STRING    = 11,
    /** @brief
     */
    OTF2_TYPE_ATTRIBUTE = 12,
    /** @brief
     */
    OTF2_TYPE_LOCATION  = 13,
    /** @brief
     */
    OTF2_TYPE_REGION    = 14,
    /** @brief
     */
    OTF2_TYPE_GROUP     = 15,
    /** @brief
     */
    OTF2_TYPE_METRIC    = 16,
    /** @brief
     */
    OTF2_TYPE_COMM      = 17,
    /** @brief
     */
    OTF2_TYPE_PARAMETER = 18,
    /** @brief
     */
    OTF2_TYPE_RMA_WIN   = 19
};


/** @brief Wrapper for enum @ref OTF2_Paradigm_enum. */
typedef uint8_t OTF2_Paradigm;

/**
 * @brief List of known paradigms.
 *
 * @since Version 1.1
 */
enum OTF2_Paradigm_enum
{
    /** @brief An unknown paradigm.
     */
    OTF2_PARADIGM_UNKNOWN            = 0,
    /** @brief Regions generated through user instrumentation.
     */
    OTF2_PARADIGM_USER               = 1,
    /** @brief Regions generated through compiler instrumentation.
     */
    OTF2_PARADIGM_COMPILER           = 2,
    /** @brief Regions referring to OpenMP directives and API functions.
     */
    OTF2_PARADIGM_OPENMP             = 3,
    /** @brief Regions referring to MPI functions.
     */
    OTF2_PARADIGM_MPI                = 4,
    /** @brief Regions referring to CUDA API functions.
     */
    OTF2_PARADIGM_CUDA               = 5,
    /** @brief Regions used by the measurement software. @since Version 1.2.
     */
    OTF2_PARADIGM_MEASUREMENT_SYSTEM = 6
};


/** @brief Wrapper for enum @ref OTF2_ThumbnailType_enum. */
typedef uint8_t OTF2_ThumbnailType;

/**
 * @brief Type of definitions used as metric in an thumbnail.
 *
 * @since Version 1.2
 */
enum OTF2_ThumbnailType_enum
{
    /** @brief The referenced definitions are of type @eref{Region}.
     */
    OTF2_THUMBNAIL_TYPE_REGION     = 0,
    /** @brief The referenced definitions are of type @eref{MetricMember}.
     */
    OTF2_THUMBNAIL_TYPE_METRIC     = 1,
    /** @brief The referenced definitions are of type @eref{Attribute}.
     */
    OTF2_THUMBNAIL_TYPE_ATTRIBUTES = 2
};


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GENERAL_DEFINITIONS_H */
