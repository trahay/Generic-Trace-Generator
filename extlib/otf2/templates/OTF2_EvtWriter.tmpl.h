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


#ifndef OTF2_EVT_WRITER_H
#define OTF2_EVT_WRITER_H


/**
 *  @file       include/otf2/OTF2_EvtWriter.h
 *  @source     templates/OTF2_EvtWriter.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This lowest user-visible layer provides write routines to
 *              write event data of a single location.
 */



#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Events.h>
#include <otf2/OTF2_AttributeList.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Keeps all necessary information about the event writer.
 *  See OTF2_EvtWriter_struct for detailed information. */
typedef struct OTF2_EvtWriter_struct OTF2_EvtWriter;


/** @brief
 *  Function to get the location ID of a writer object.
 *
 *  @param writer       Writer object which has to be deleted
 *  @param locationID   Pointer to a variable where the ID is returned in
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_GetLocationID( const OTF2_EvtWriter* writer,
                              OTF2_LocationRef*     locationID );


/** @brief
 *  Function to set user defined data to a writer object.
 *
 *  @param writer   Writer object.
 *  @param userData User provided data. Can be queried with
 *                  @ref OTF2_EvtWriter_GetUserData.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_SetUserData( OTF2_EvtWriter* writer,
                            void*           userData );


/** @brief
 *  Function to get the location of a writer object.
 *
 *  @param writer        Writer object.
 *  @param[out] userData Pointer to a variable where the pointer to the location is returned in.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_GetUserData( const OTF2_EvtWriter* writer,
                            void**                userData );


/** @brief
 *  The location ID is not always known on measurment start, and only needed
 *  on the first buffer flush to generate the file name. This function enables
 *  setting of the location ID after generating the buffer object.
 *
 *  @param writer      Writer object.
 *  @param location    Location ID.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_SetLocationID( OTF2_EvtWriter*  writer,
                              OTF2_LocationRef location );


/** @brief Get the number of events.
 *
 *  Get the number of events written with this event writer. You should call
 *  this function right before closing the event writer to get the correct
 *  number of stored event records.
 *
 *  @param writer              Writer object.
 *  @param[out] numberOfEvents Return pointer to the number of events.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_GetNumberOfEvents( OTF2_EvtWriter* writer,
                                  uint64_t*       numberOfEvents );

@otf2 for event in events:

/** @brief Records an @@event.name@@ event.
 *
 *  @@event.doxygendoc()@@
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @@event.doxygenargs()@@
 *
 *  @since Version @@event.since@@
@otf2  if event.deprecated:
 *
 *  @deprecated In version @@event.deprecated@@
@otf2  endif
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_@@event.name@@( OTF2_EvtWriter*     writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time @@event.funcargs()@@ );

@otf2 endfor

/** @brief Please give me a documantation.
 *
 *  @param writer   Writer object.
 *  @param rewindId Generic attributes for the event.
 *
 *  @since Version 1.1
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_StoreRewindPoint( OTF2_EvtWriter* writer,
                                 uint32_t        rewindId );


/** @brief Please give me a documantation.
 *
 *  @param writer   Writer object.
 *  @param rewindId Generic attributes for the event.
 *
 *  @since Version 1.1
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_Rewind( OTF2_EvtWriter* writer,
                       uint32_t        rewindId );


/** @brief Please give me a documantation.
 *
 *  @param writer   Writer object.
 *  @param rewindId Generic attributes for the event.
 *
 *  @since Version 1.1
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ClearRewindPoint( OTF2_EvtWriter* writer,
                                 uint32_t        rewindId );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_EVT_WRITER_H */
