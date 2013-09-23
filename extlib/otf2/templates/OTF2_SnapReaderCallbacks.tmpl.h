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


#ifndef OTF2_SNAP_READER_CALLBACKS_H
#define OTF2_SNAP_READER_CALLBACKS_H


/**
 *  @file       include/otf2/OTF2_SnapReaderCallbacks.h
 *  @source     templates/OTF2_SnapReaderCallbacks.tmpl.h
 *
 *  @brief      This defines the callbacks for the snap reader.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>
#include <otf2/OTF2_AttributeList.h>
#include <otf2/OTF2_Events.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Opaque struct which holdes all snap event record callbacks.
 * 
 *  @since Version 1.2
 */
typedef struct OTF2_SnapReaderCallbacks_struct OTF2_SnapReaderCallbacks;


/** @brief Allocates a new struct for the snap event callbacks.
 * 
 *  @since Version 1.2
 *
 *  @return A newly allocated struct of type @ref OTF2_SnapReaderCallbacks.
 */
OTF2_SnapReaderCallbacks*
OTF2_SnapReaderCallbacks_New( void );


/** @brief Deallocates a struct for the snap event callbacks.
 *
 *  @param snapReaderCallbacks Handle to a struct previously allocated
 *                            with @ref OTF2_SnapReaderCallbacks_New.
 * 
 *  @since Version 1.2
 */
void
OTF2_SnapReaderCallbacks_Delete( OTF2_SnapReaderCallbacks* snapReaderCallbacks );


/** @brief Clears a struct for the sanp event callbacks.
 *
 *  @param snapReaderCallbacks Handle to a struct previously allocated
 *                            with @ref OTF2_SnapReaderCallbacks_New.
 * 
 *  @since Version 1.2
 */
void
OTF2_SnapReaderCallbacks_Clear( OTF2_SnapReaderCallbacks* snapReaderCallbacks );


/** @brief Callback for an unknown snap event record.
 *
 *  @param location      The location where this event happened.
 *  @param time          Snapshot time.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterSnapCallbacks or
 *                       @ref OTF2_SnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 * 
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_SnapReaderCallback_Unknown )( OTF2_LocationRef    location,
                                      OTF2_TimeStamp      snapTime,
                                      void*               userData,
                                      OTF2_AttributeList* attributeList );


/** @brief Registers the callback for the Unknown snap event.
 *
 *  @param snapReaderCallbacks Struct for all callbacks.
 *  @param unknownCallback    Function which should be called for all
 *                            unknown snap events.
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
OTF2_SnapReaderCallbacks_SetUnknownCallback(
    OTF2_SnapReaderCallbacks*       snapReaderCallbacks,
    OTF2_SnapReaderCallback_Unknown unknownCallback );

@otf2 for snap in snaps:

/** @brief Callback for the @@snap.name@@ snap event record.
 *
 *  @@snap.doxygendoc()@@
 *
 *  @param location      The location where this snap event happened.
 *  @param snapTime      Snapshot time.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterSnapCallbacks or
 *                       @ref OTF2_SnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @@snap.doxygenargs()@@
 *
 *  @since Version @@snap.since@@
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_SnapReaderCallback_@@snap.name@@ )( OTF2_LocationRef    location,
                                            OTF2_TimeStamp      snapTime,
                                            void*               userData,
                                            OTF2_AttributeList* attributeList @@snap.funcargs()@@ );


/** @brief Registers the callback for the @@snap.name@@ snap event.
 *
 *  @param snapReaderCallbacks      Struct for all callbacks.
 *  @param @@snap.lname@@Callback Function which should be called for all
 *                                 @@snap.name@@ events.
 * 
 *  @since Version @@snap.since@@
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_SnapReaderCallbacks_Set@@snap.name@@Callback(
    OTF2_SnapReaderCallbacks*              snapReaderCallbacks,
    OTF2_SnapReaderCallback_@@snap.name@@ @@snap.lname@@Callback );

@otf2 endfor

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_SNAP_READER_CALLBACKS_H */
