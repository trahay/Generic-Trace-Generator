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
@otf2 with v = version
#define OTF2_VERSION_MAJOR  @@v.major@@
#define OTF2_VERSION_MINOR  @@v.minor@@
#define OTF2_VERSION_BUGFIX @@v.bugfix@@
#define OTF2_VERSION_SUFFIX "@@v.suffix@@"
#define OTF2_VERSION        "@@v@@"
@otf2 endwith
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

@otf2 for def in defs|referable_defs|first_born_defs:

@otf2 if def is shared_def:
/** @brief Type used to indicate a reference to a @ref @@def.siblings|join(', or a @ref ')@@ definition */
@otf2 else:
/** @brief Type used to indicate a reference to a @ref @@def.name@@ definition */
@otf2 endif
typedef @@def.self_attr.type.c_orig@@ @@def.self_attr.type@@;
@otf2 if def is shared_def:
/** @brief The invalid value for a reference to a @ref @@def.siblings|join(', or a @ref ')@@ definition */
@otf2 else:
/** @brief The invalid value for a reference to a @ref @@def.name@@ definition */
@otf2 endif
#define OTF2_UNDEFINED_@@def.ref_upper@@ ( ( @@def.self_attr.type@@ )OTF2_UNDEFINED_@@def.self_attr.type.otf2|upper@@ )

@otf2 endfor

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
    OTF2_FILETYPE_ANCHOR = 0,

    /** @brief Represents the type for the global definition file (.def).
     *  Does has a undefined location ID
     */
    OTF2_FILETYPE_GLOBAL_DEFS = 1,

    /** @brief Represents the type for a local definition file (.def). */
    OTF2_FILETYPE_LOCAL_DEFS = 2,

    /** @brief Represents the type for a event file (.evt). */
    OTF2_FILETYPE_EVENTS = 3,

    /** @brief Represents the type for a snapshot file (.snap). */
    OTF2_FILETYPE_SNAPSHOTS = 4,

    /** @brief Represents the type for a thumb file (.thumb). */
    OTF2_FILETYPE_THUMBNAIL = 5,

    /** @brief Represents the type for a marker file (.marker). */
    OTF2_FILETYPE_MARKER = 6
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
    OTF2_CALLBACK_SUCCESS = 0,

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

@otf2 for enum in enums|general_enums:

/** @brief Wrapper for enum @ref @@enum.type@@_enum. */
typedef @@enum.type.c_orig@@ @@enum.type@@;

/**
 * @brief @@enum.doxygendoc(prefix=' * ')@@
 *
 * @since Version @@enum.since@@
 */
enum @@enum.type@@_enum
{
    @otf2 for entry in enum.entries:
    /** @brief @@entry.doxygendoc(prefix='     *  ')@@
     */
    @@entry.name@@ = @@entry.value@@@@enum.need_comma(entry)@@
    @otf2 endfor
    @otf2 if enum has terminator:

    /** @brief @@enum.terminator.doxygendoc(prefix='     *  ')@@
     */
    @@enum.terminator.name@@ = @@enum.terminator.value@@
    @otf2 endif
};

@otf2 endfor

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GENERAL_DEFINITIONS_H */
