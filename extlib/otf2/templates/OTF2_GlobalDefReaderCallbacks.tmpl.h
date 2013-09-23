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

@otf2 for def in defs|global_defs:

/** @brief Function pointer definition for the callback which is triggered by
 *         a @ref @@def.name@@ definition record.
 *
 *  @@def.doxygendoc()@@
 *
 *  @param userData User data as set by
 *                  @ref OTF2_Reader_RegisterGlobalDefCallbacks or
 *                  @ref OTF2_GlobalDefReader_SetCallbacks.
 *
 *  @@def.doxygenargs()@@
 *
 *  @since Version @@def.since@@
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalDefReaderCallback_@@def.name@@ )( void* userData @@def.funcargs()@@ );


/** @brief Registers the callback for the @ref @@def.name@@ definition.
 *
 *  @param globalDefReaderCallbacks Struct for all callbacks.
 *  @param @@def.lname@@Callback    Function which should be called for all
 *                                  @ref @@def.name@@ definitions.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_Set@@def.name@@Callback(
    OTF2_GlobalDefReaderCallbacks*            globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_@@def.name@@ @@def.lname@@Callback );

@otf2 endfor

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GLOBAL_DEF_READER_CALLBACKS_H */
