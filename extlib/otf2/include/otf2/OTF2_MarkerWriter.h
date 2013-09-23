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


#ifndef OTF2_MARKER_WRITER_H
#define OTF2_MARKER_WRITER_H


/**
 *  @file       include/otf2/OTF2_MarkerWriter.h
 *
 *  @brief      This file provides all routines that write marker records.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Definitions.h>


#include <otf2/OTF2_Marker.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Handle definition for the external marker writer. */
typedef struct OTF2_MarkerWriter_struct OTF2_MarkerWriter;


/** @brief Write a marker definition.
 *
 *  @param writerHandle     Marker writer handle.
 *  @param self             Reference to this marker definition.
 *  @param markerGroup      Group name e.g. "MUST".
 *  @param markerCategory   Category name e.g "Argument type error".
 *                          The tuple (markerGroup, markerCategory) must be unique
 *                          over all marker definitions.
 *  @param severity         The severity for this marker category.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_MarkerWriter_WriteDefMarker( OTF2_MarkerWriter*  writerHandle,
                                  OTF2_MarkerRef      self,
                                  const char*         markerGroup,
                                  const char*         markerCategory,
                                  OTF2_MarkerSeverity severity );

/** @brief Write a marker record.
 *
 *  @param writerHandle     Marker writer handle.
 *  @param time             Time of the marker.
 *  @param duration         A possible duration of this marker. May be 0.
 *  @param marker           Reference to a marker definition.
 *  @param scope            The type of scope of this marker instance:
 *                          @eref{OTF2_MARKER_SCOPE_GLOBAL},
 *                          @eref{OTF2_MARKER_SCOPE_LOCATION},
 *                          @eref{OTF2_MARKER_SCOPE_LOCATION_GROUP},
 *                          @eref{OTF2_MARKER_SCOPE_SYSTEM_TREE_NODE},
 *                          @eref{OTF2_MARKER_SCOPE_GROUP}, or
 *                          @eref{OTF2_MARKER_SCOPE_COMM}.
 *  @param scopeRef         The reference to an element of the scope of this
 *                          marker. Depends on @ref scope.
 *  @param text             A textual description for this marker.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_MarkerWriter_WriteMarker( OTF2_MarkerWriter* writerHandle,
                               uint64_t           time,
                               uint64_t           duration,
                               OTF2_MarkerRef     marker,
                               OTF2_MarkerScope   scope,
                               uint64_t           scopeRef,
                               const char*        text );

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_MARKER_WRITER_H */
