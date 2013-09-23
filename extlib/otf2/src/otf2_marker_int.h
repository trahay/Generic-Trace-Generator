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

#ifndef OTF2_INTERNAL_MARKER_H
#define OTF2_INTERNAL_MARKER_H


#include <otf2/OTF2_Marker.h>
#include <otf2/OTF2_MarkerWriter.h>
#include <otf2/OTF2_MarkerReader.h>


/** @brief Wrapper for the OTF2_MarkerType_enum
 */
typedef uint8_t OTF2_MarkerType;

/** @brief An enum which holds all values that are possible marker types.
 */
enum OTF2_MarkerType_enum
{
    /* First five entries are reserved for OTF2 internals */

    /* OTF2 Marker Records */
    OTF2_MARKER_DEF    = 5,  /**< Marker defintion record. */
    OTF2_MARKER_MARKER = 6,  /**< Marker record. */

    OTF2_MARKER_MAX_ID
};


/** Group markers by name and severity. */
typedef struct OTF2_DefMarker_struct
{
    OTF2_MarkerRef self;

    /** Group name. "MUST", ... */
    const char* marker_group;

    /** Marker name. "Argument type error", ... */
    const char* marker_category;

    /** The severity for these markers. */
    OTF2_MarkerSeverity severity;
} OTF2_DefMarker;


/** A user marker instance, with implied time stamp. */
typedef struct OTF2_Marker_struct
{
    OTF2_TimeStamp timestamp;

    /** A possible duration of this marker. May be 0. */
    OTF2_TimeStamp duration;

    /** Groups this marker by name and severity. */
    OTF2_MarkerRef marker;

    /** The type of scope of this marker instance.

        @eref{OTF2_MARKER_SCOPE_GLOBAL}::
            The scope is global for the complete trace. The value of @ref scope
            should be @eref{OTF2_UNDEFINED_UINT64}.

        @eref{OTF2_MARKER_SCOPE_LOCATION}::
            The scope is a location and the value of @ref scope should be
            a valid reference to a @eref{Location} definition.

        @eref{OTF2_MARKER_SCOPE_LOCATION_GROUP}::
            The scope is a location group and the value of @ref scope should be
            a valid reference to a @eref{LocationGroup} definition.

        @eref{OTF2_MARKER_SCOPE_SYSTEM_TREE_NODE}::
            The scope is a system tree node and the value of @ref scope should be
            a valid reference to a @eref{SystemTreeNode} definition.

        @eref{OTF2_MARKER_SCOPE_GROUP}::
            The scope is a group and the value of @ref scope should be
            a valid reference to a @eref{Group} definition, which needs to be
            resolvable to a group of locations (directly or indirectly).

        @eref{OTF2_MARKER_SCOPE_COMM}::
            The scope is a communicator and the value of @ref scope should be
            a valid reference to a @eref{MpiComm} definition.
     */
    OTF2_MarkerScope marker_scope;

    /** The scope instance of this marker. Depends on @ref marker_scope. */
    uint64_t scope;

    /** A textual description for this marker. */
    const char* text;
} OTF2_Marker;

/** @brief Marker reader callbacks handle. */
struct OTF2_MarkerReaderCallbacks_struct
{
    /** @brief Callback for an unknown record. */
    OTF2_MarkerReaderCallback_Unknown   unknown;
    /** @brief Callback for a marker defintion record. */
    OTF2_MarkerReaderCallback_DefMarker def_marker;
    /** @brief Callback for a marker record. */
    OTF2_MarkerReaderCallback_Marker    marker;
};

/** @brief Keeps all necessary information about the marker writer. */
struct OTF2_MarkerWriter_struct
{
    /** Archive handle */
    OTF2_Archive*      archive;
    /** Buffer handle */
    OTF2_Buffer*       buffer;
    /** Chain to next marker writer, used in OTF2_Archive */
    OTF2_MarkerWriter* next;
};

/** @brief Keeps all necessary information about the marker reader. */
struct OTF2_MarkerReader_struct
{
    /** Archive handle */
    OTF2_Archive*              archive;
    /** Buffer handle */
    OTF2_Buffer*               buffer;
    /** Chain to next definition reader, used in OTF2_Archive */
    OTF2_MarkerReader*         next;
    /** Callbacks, which are triggered on read.*/
    OTF2_MarkerReaderCallbacks reader_callbacks;
    /** Generic pointer to the userdata, to handle them inside a callback. */
    void*                      user_data;
};


/** @brief Creates a new marker writer instance.
 *
 *  @param archive      Archive handle.
 *
 *  @return             Returns a handle to the marker writer if successful,
 *                      NULL otherwise.
 */
OTF2_MarkerWriter*
otf2_marker_writer_new( OTF2_Archive* archive );


/** @brief Releases resources associated with the given marker writer handle.
 *
 *  @param writerHandle Writer object which has to be deleted.
 *
 *  @return             OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_marker_writer_delete( OTF2_MarkerWriter* writerHandle );


/** @brief Creates a new marker reader instance.
 *
 *  @param archive      Archive handle.
 *
 *  @return             Returns a handle to the marker reader if successful,
 *                      NULL otherwise.
 */
OTF2_MarkerReader*
otf2_marker_reader_new( OTF2_Archive* archive );


/** @brief Releases resources associated with the given marker reader handle.
 *
 *  @param readerHandle Writer object which has to be deleted.
 *
 *  @return             OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_marker_reader_delete( OTF2_MarkerReader* readerHandle );


#endif /* OTF2_INTERNAL_MARKER_H */
