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


#ifndef OTF2_MARKER_READER_CALLBACKS_H
#define OTF2_MARKER_READER_CALLBACKS_H


/**
 *  @file       include/otf2/OTF2_MarkerReaderCallbacks.h
 *
 *  @brief      This defines the callbacks for the marker reader.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>
#include <otf2/OTF2_Definitions.h>
#include <otf2/OTF2_IdMap.h>


#include <otf2/OTF2_Marker.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Opaque struct which holdes all definition record callbacks.
 */
typedef struct OTF2_MarkerReaderCallbacks_struct OTF2_MarkerReaderCallbacks;


/** @brief Allocates a new struct for the marker callbacks.
 *
 *  @since Version 1.2
 *
 *  @return A newly allocated struct of type @ref OTF2_MarkerReaderCallbacks.
 */
OTF2_MarkerReaderCallbacks*
OTF2_MarkerReaderCallbacks_New( void );


/** @brief Deallocates a struct for the marker callbacks.
 *
 *  @since Version 1.2
 *
 *  @param markerReaderCallbacks Handle to a struct previously allocated
 *                               with @ref OTF2_MarkerReaderCallbacks_New.
 */
void
OTF2_MarkerReaderCallbacks_Delete( OTF2_MarkerReaderCallbacks* markerReaderCallbacks );


/** @brief Clears a struct for the marker callbacks.
 *
 *  @since Version 1.2
 *
 *  @param markerReaderCallbacks Handle to a struct previously allocated
 *                               with @ref OTF2_MarkerReaderCallbacks_New.
 */
void
OTF2_MarkerReaderCallbacks_Clear( OTF2_MarkerReaderCallbacks* markerReaderCallbacks );


/** @brief Function pointer definition for the callback which is
 *         triggered for an unknown marker.
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterMarkerCallbacks or
 *                  @ref OTF2_MarkerReader_SetCallbacks.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_MarkerReaderCallback_Unknown )( void* userData );


/** @brief Registers the callback for an unknown marker.
 *
 *  @param markerReaderCallbacks Struct for all callbacks.
 *  @param unknownCallback       Function which should be called for all
 *                               unknown definitions.
 *
 *  @since Version 1.2
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_MarkerReaderCallbacks_SetUnknownCallback(
    OTF2_MarkerReaderCallbacks*       markerReaderCallbacks,
    OTF2_MarkerReaderCallback_Unknown unknownCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Marker definition record.
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterMarkerCallbacks or
 *                  @ref OTF2_MarkerReader_SetCallbacks.
 *
 *  @param self             Reference to this marker defintion.
 *  @param markerGroup      Group name, e.g., "MUST", ...
 *  @param markerCategory   Category, e.g., "Argument type error", ...
 *                          The tuple (markerGroup, markerCategory) must be
 *                          unique over all marker definitions.
 *  @param severity         The severity for this marker category.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_MarkerReaderCallback_DefMarker )( void*               userData,
                                          OTF2_MarkerRef      self,
                                          const char*         markerGroup,
                                          const char*         markerCategory,
                                          OTF2_MarkerSeverity severity );


/** @brief Registers the callback for the @ref Marker definition.
 *
 *  @param markerReaderCallbacks    Struct for all callbacks.
 *  @param defMarkerCallback        Function which should be called for all
 *                                  @ref Marker definitions.
 *  @since Version 1.2
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_MarkerReaderCallbacks_SetDefMarkerCallback(
    OTF2_MarkerReaderCallbacks*         markerReaderCallbacks,
    OTF2_MarkerReaderCallback_DefMarker defMarkerCallback );


/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref Marker record.
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterMarkerCallbacks or
 *                  @ref OTF2_MarkerReader_SetCallbacks.
 *
 *  @param timestamp        Timestamp of the marker.
 *  @param duration         Duration the marker applies.
 *  @param marker           Reference to the marker defintion.
 *  @param scope            The type of scope of this marker instance.
 *  @param scopeRef         The reference to an element of the scope of this
 *                          marker. Depends on @ref scope.
 *  @param text             A textual description for this marker.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_MarkerReaderCallback_Marker )( void*            userData,
                                       OTF2_TimeStamp   timestamp,
                                       OTF2_TimeStamp   duration,
                                       OTF2_MarkerRef   marker,
                                       OTF2_MarkerScope scope,
                                       uint64_t         scopeRef,
                                       const char*      text );


/** @brief Registers the callback for the @ref Marker record.
 *
 *  @param markerReaderCallbacks    Struct for all callbacks.
 *  @param markerCallback           Function which should be called for all
 *                                  @ref Marker records.
 *  @since Version 1.2
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_MarkerReaderCallbacks_SetMarkerCallback(
    OTF2_MarkerReaderCallbacks*      markerReaderCallbacks,
    OTF2_MarkerReaderCallback_Marker markerCallback );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_MARKER_READER_CALLBACKS_H */
