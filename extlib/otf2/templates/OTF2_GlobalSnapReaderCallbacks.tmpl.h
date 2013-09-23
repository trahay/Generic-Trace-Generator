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


#ifndef OTF2_GLOBAL_SNAP_READER_CALLBACKS_H
#define OTF2_GLOBAL_SNAP_READER_CALLBACKS_H


/**
 *  @file       include/otf2/OTF2_GlobalSnapReaderCallbacks.h
 *  @source     templates/OTF2_GlobalSnapReaderCallbacks.tmpl.h
 *
 *  @brief      This defines the callbacks for the global snap reader.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>
#include <otf2/OTF2_AttributeList.h>
#include <otf2/OTF2_Events.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Opaque struct which holdes all snap record callbacks.
 * 
 *  @since Version 1.2
 */
typedef struct OTF2_GlobalSnapReaderCallbacks_struct OTF2_GlobalSnapReaderCallbacks;


/** @brief Allocates a new struct for the snap callbacks.
 * 
 *  @since Version 1.2
 *
 *  @return A newly allocated struct of type @ref OTF2_GlobalSnapReaderCallbacks.
 */
OTF2_GlobalSnapReaderCallbacks*
OTF2_GlobalSnapReaderCallbacks_New( void );


/** @brief Deallocates a struct for the global snap callbacks.
 *
 *  @param globalSnapReaderCallbacks Handle to a struct previously allocated
 *                                   with @ref OTF2_GlobalSnapReaderCallbacks_New.
 *  @since Version 1.2
 */
void
OTF2_GlobalSnapReaderCallbacks_Delete( OTF2_GlobalSnapReaderCallbacks* globalSnapReaderCallbacks );


/** @brief Clears a struct for the global snap callbacks.
 *
 *  @param globalSnapReaderCallbacks Handle to a struct previously allocated
 *                                   with @ref OTF2_GlobalSnapReaderCallbacks_New.
 *  @since Version 1.2
 */
void
OTF2_GlobalSnapReaderCallbacks_Clear( OTF2_GlobalSnapReaderCallbacks* globalSnapReaderCallbacks );


/** @brief Callback for an unknown snap record.
 *
 *  @param locationID    The location where this snap happened.
 *  @param snapTime      The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 * 
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_Unknown )( OTF2_LocationRef    locationID,
                                            OTF2_TimeStamp      snapTime,
                                            void*               userData,
                                            OTF2_AttributeList* attributeList );


/** @brief Registers the callback for unknown snaps.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param unknownCallback          Function which should be called for all
 *                                  unknown snaps.
 *  @since Version 1.2
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetUnknownCallback(
    OTF2_GlobalSnapReaderCallbacks*       globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_Unknown unknownCallback );

@otf2 for snap in snaps:

/** @brief Callback for the @@snap.name@@ snap record.
 *
 *  @@snap.doxygendoc()@@
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @@snap.doxygenargs()@@
 *
 *  @since Version @@snap.since@@
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_@@snap.name@@ )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      snapTime,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList @@snap.funcargs()@@ );


/** @brief Registers the callback for the @@snap.name@@ snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param @@snap.lname@@Callback  Function which should be called for all
 *                                  @@snap.name@@ snaps.
 * 
 *  @since Version @@snap.since@@

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_Set@@snap.name@@Callback(
    OTF2_GlobalSnapReaderCallbacks*              globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_@@snap.name@@ @@snap.lname@@Callback );

@otf2 endfor

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GLOBAL_SNAP_READER_CALLBACKS_H */
