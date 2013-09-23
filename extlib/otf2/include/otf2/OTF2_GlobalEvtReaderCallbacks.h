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


#ifndef OTF2_GLOBAL_EVT_READER_CALLBACKS_H
#define OTF2_GLOBAL_EVT_READER_CALLBACKS_H


/**
 *  @file       include/otf2/OTF2_GlobalEvtReaderCallbacks.h
 *  @source     templates/OTF2_GlobalEvtReaderCallbacks.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This defines the callbacks for the global event reader.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>
#include <otf2/OTF2_AttributeList.h>
#include <otf2/OTF2_Events.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Opaque struct which holdes all event record callbacks.
 */
typedef struct OTF2_GlobalEvtReaderCallbacks_struct OTF2_GlobalEvtReaderCallbacks;


/** @brief Allocates a new struct for the event callbacks.
 *
 *  @return A newly allocated struct of type @ref OTF2_GlobalEvtReaderCallbacks.
 */
OTF2_GlobalEvtReaderCallbacks*
OTF2_GlobalEvtReaderCallbacks_New( void );


/** @brief Deallocates a struct for the global event callbacks.
 *
 *  @param globalEvtReaderCallbacks Handle to a struct previously allocated
 *                                  with @ref OTF2_GlobalEvtReaderCallbacks_New.
 */
void
OTF2_GlobalEvtReaderCallbacks_Delete( OTF2_GlobalEvtReaderCallbacks* globalEvtReaderCallbacks );


/** @brief Clears a struct for the global event callbacks.
 *
 *  @param globalEvtReaderCallbacks Handle to a struct previously allocated
 *                                  with @ref OTF2_GlobalEvtReaderCallbacks_New.
 */
void
OTF2_GlobalEvtReaderCallbacks_Clear( OTF2_GlobalEvtReaderCallbacks* globalEvtReaderCallbacks );


/** @brief Callback for an unknown event record.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_Unknown )( OTF2_LocationRef    locationID,
                                           OTF2_TimeStamp      time,
                                           void*               userData,
                                           OTF2_AttributeList* attributeList );


/** @brief Registers the callback for unknown events.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param unknownCallback          Function which should be called for all
 *                                  unknown events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetUnknownCallback(
    OTF2_GlobalEvtReaderCallbacks*       globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_Unknown unknownCallback );


/** @brief Callback for the BufferFlush event record.
 *
 *  This event signals that the internal buffer was flushed at the given
 *  time.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param stopTime The time the buffer flush finished.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_BufferFlush )( OTF2_LocationRef    locationID,
                                               OTF2_TimeStamp      time,
                                               void*               userData,
                                               OTF2_AttributeList* attributeList,
                                               OTF2_TimeStamp      stopTime );


/** @brief Registers the callback for the BufferFlush event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param bufferFlushCallback  Function which should be called for all
 *                                  BufferFlush events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetBufferFlushCallback(
    OTF2_GlobalEvtReaderCallbacks*           globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_BufferFlush bufferFlushCallback );


/** @brief Callback for the MeasurementOnOff event record.
 *
 *  This event signals where the measurement system turned measurement on
 *  or off.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param measurementMode Is the measurement turned on
 *                         (@eref{OTF2_MEASUREMENT_ON}) or off
 *                         (@eref{OTF2_MEASUREMENT_OFF})?
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MeasurementOnOff )( OTF2_LocationRef     locationID,
                                                    OTF2_TimeStamp       time,
                                                    void*                userData,
                                                    OTF2_AttributeList*  attributeList,
                                                    OTF2_MeasurementMode measurementMode );


/** @brief Registers the callback for the MeasurementOnOff event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param measurementOnOffCallback  Function which should be called for all
 *                                  MeasurementOnOff events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMeasurementOnOffCallback(
    OTF2_GlobalEvtReaderCallbacks*                globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MeasurementOnOff measurementOnOffCallback );


/** @brief Callback for the Enter event record.
 *
 *  An enter record indicates that the program enters a code region.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param region Needs to be defined in a definition record References a @ref
 *                Region definition and will be mapped to the global
 *                definition if a mapping table of type @ref
 *                OTF2_MAPPING_REGION is available.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_Enter )( OTF2_LocationRef    locationID,
                                         OTF2_TimeStamp      time,
                                         void*               userData,
                                         OTF2_AttributeList* attributeList,
                                         OTF2_RegionRef      region );


/** @brief Registers the callback for the Enter event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param enterCallback  Function which should be called for all
 *                                  Enter events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetEnterCallback(
    OTF2_GlobalEvtReaderCallbacks*     globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_Enter enterCallback );


/** @brief Callback for the Leave event record.
 *
 *  A leave record indicates that the program leaves a code region.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param region Needs to be defined in a definition record References a @ref
 *                Region definition and will be mapped to the global
 *                definition if a mapping table of type @ref
 *                OTF2_MAPPING_REGION is available.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_Leave )( OTF2_LocationRef    locationID,
                                         OTF2_TimeStamp      time,
                                         void*               userData,
                                         OTF2_AttributeList* attributeList,
                                         OTF2_RegionRef      region );


/** @brief Registers the callback for the Leave event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param leaveCallback  Function which should be called for all
 *                                  Leave events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback(
    OTF2_GlobalEvtReaderCallbacks*     globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_Leave leaveCallback );


/** @brief Callback for the MpiSend event record.
 *
 *  A MpiSend record indicates that a MPI message send process was
 *  initiated (MPI_SEND). It keeps the necessary information for this
 *  event: receiver of the message, communicator, and the message tag.
 *  You can optionally add further information like the message length
 *  (size of the send buffer).
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param receiver     MPI rank of receiver in @a communicator.
 *  @param communicator Communicator ID. References a @ref Comm definition and
 *                      will be mapped to the global definition if a mapping
 *                      table of type @ref OTF2_MAPPING_COMM is available.
 *  @param msgTag       Message tag
 *  @param msgLength    Message length
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiSend )( OTF2_LocationRef    locationID,
                                           OTF2_TimeStamp      time,
                                           void*               userData,
                                           OTF2_AttributeList* attributeList,
                                           uint32_t            receiver,
                                           OTF2_CommRef        communicator,
                                           uint32_t            msgTag,
                                           uint64_t            msgLength );


/** @brief Registers the callback for the MpiSend event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiSendCallback  Function which should be called for all
 *                                  MpiSend events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiSendCallback(
    OTF2_GlobalEvtReaderCallbacks*       globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiSend mpiSendCallback );


/** @brief Callback for the MpiIsend event record.
 *
 *  A MpiIsend record indicates that a MPI message send process was
 *  initiated (MPI_ISEND). It keeps the necessary information for this
 *  event: receiver of the message, communicator, and the message tag.
 *  You can optionally add further information like the message length
 *  (size of the send buffer).
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param receiver     MPI rank of receiver in @a communicator.
 *  @param communicator Communicator ID. References a @ref Comm definition and
 *                      will be mapped to the global definition if a mapping
 *                      table of type @ref OTF2_MAPPING_COMM is available.
 *  @param msgTag       Message tag
 *  @param msgLength    Message length
 *  @param requestID    ID of the related request
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiIsend )( OTF2_LocationRef    locationID,
                                            OTF2_TimeStamp      time,
                                            void*               userData,
                                            OTF2_AttributeList* attributeList,
                                            uint32_t            receiver,
                                            OTF2_CommRef        communicator,
                                            uint32_t            msgTag,
                                            uint64_t            msgLength,
                                            uint64_t            requestID );


/** @brief Registers the callback for the MpiIsend event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiIsendCallback  Function which should be called for all
 *                                  MpiIsend events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCallback(
    OTF2_GlobalEvtReaderCallbacks*        globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiIsend mpiIsendCallback );


/** @brief Callback for the MpiIsendComplete event record.
 *
 *  Signals the completion of non-blocking send request.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param requestID ID of the related request
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiIsendComplete )( OTF2_LocationRef    locationID,
                                                    OTF2_TimeStamp      time,
                                                    void*               userData,
                                                    OTF2_AttributeList* attributeList,
                                                    uint64_t            requestID );


/** @brief Registers the callback for the MpiIsendComplete event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiIsendCompleteCallback  Function which should be called for all
 *                                  MpiIsendComplete events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCompleteCallback(
    OTF2_GlobalEvtReaderCallbacks*                globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiIsendComplete mpiIsendCompleteCallback );


/** @brief Callback for the MpiIrecvRequest event record.
 *
 *  Signals the request of an receive, which can be completed later.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param requestID ID of the requested receive
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiIrecvRequest )( OTF2_LocationRef    locationID,
                                                   OTF2_TimeStamp      time,
                                                   void*               userData,
                                                   OTF2_AttributeList* attributeList,
                                                   uint64_t            requestID );


/** @brief Registers the callback for the MpiIrecvRequest event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiIrecvRequestCallback  Function which should be called for all
 *                                  MpiIrecvRequest events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvRequestCallback(
    OTF2_GlobalEvtReaderCallbacks*               globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback );


/** @brief Callback for the MpiRecv event record.
 *
 *  A MpiRecv record indicates that a MPI message was received (MPI_RECV).
 *  It keeps the necessary information for this event: sender of the
 *  message, communicator, and the message tag. You can optionally add
 *  further information like the message length (size of the receive
 *  buffer).
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param sender       MPI rank of sender in @a communicator.
 *  @param communicator Communicator ID. References a @ref Comm definition and
 *                      will be mapped to the global definition if a mapping
 *                      table of type @ref OTF2_MAPPING_COMM is available.
 *  @param msgTag       Message tag
 *  @param msgLength    Message length
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiRecv )( OTF2_LocationRef    locationID,
                                           OTF2_TimeStamp      time,
                                           void*               userData,
                                           OTF2_AttributeList* attributeList,
                                           uint32_t            sender,
                                           OTF2_CommRef        communicator,
                                           uint32_t            msgTag,
                                           uint64_t            msgLength );


/** @brief Registers the callback for the MpiRecv event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiRecvCallback  Function which should be called for all
 *                                  MpiRecv events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiRecvCallback(
    OTF2_GlobalEvtReaderCallbacks*       globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiRecv mpiRecvCallback );


/** @brief Callback for the MpiIrecv event record.
 *
 *  A MpiIrecv record indicates that a MPI message was received
 *  (MPI_IRECV). It keeps the necessary information for this event:
 *  sender of the message, communicator, and the message tag. You can
 *  optionally add further information like the message length (size
 *  of the receive buffer).
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param sender       MPI rank of sender in @a communicator.
 *  @param communicator Communicator ID. References a @ref Comm definition and
 *                      will be mapped to the global definition if a mapping
 *                      table of type @ref OTF2_MAPPING_COMM is available.
 *  @param msgTag       Message tag
 *  @param msgLength    Message length
 *  @param requestID    ID of the related request
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiIrecv )( OTF2_LocationRef    locationID,
                                            OTF2_TimeStamp      time,
                                            void*               userData,
                                            OTF2_AttributeList* attributeList,
                                            uint32_t            sender,
                                            OTF2_CommRef        communicator,
                                            uint32_t            msgTag,
                                            uint64_t            msgLength,
                                            uint64_t            requestID );


/** @brief Registers the callback for the MpiIrecv event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiIrecvCallback  Function which should be called for all
 *                                  MpiIrecv events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvCallback(
    OTF2_GlobalEvtReaderCallbacks*        globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiIrecv mpiIrecvCallback );


/** @brief Callback for the MpiRequestTest event record.
 *
 *  This events appears if the program tests if a request has already
 *  completed but the test failed.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param requestID ID of the related request
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiRequestTest )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      time,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  uint64_t            requestID );


/** @brief Registers the callback for the MpiRequestTest event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiRequestTestCallback  Function which should be called for all
 *                                  MpiRequestTest events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiRequestTestCallback(
    OTF2_GlobalEvtReaderCallbacks*              globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiRequestTest mpiRequestTestCallback );


/** @brief Callback for the MpiRequestCancelled event record.
 *
 *  This events appears if the program canceled a request.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param requestID ID of the related request
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiRequestCancelled )( OTF2_LocationRef    locationID,
                                                       OTF2_TimeStamp      time,
                                                       void*               userData,
                                                       OTF2_AttributeList* attributeList,
                                                       uint64_t            requestID );


/** @brief Registers the callback for the MpiRequestCancelled event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiRequestCancelledCallback  Function which should be called for all
 *                                  MpiRequestCancelled events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiRequestCancelledCallback(
    OTF2_GlobalEvtReaderCallbacks*                   globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiRequestCancelled mpiRequestCancelledCallback );


/** @brief Callback for the MpiCollectiveBegin event record.
 *
 *  A MpiCollectiveBegin record marks the begin of an MPI collective
 *  operation (MPI_GATHER, MPI_SCATTER etc.).
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiCollectiveBegin )( OTF2_LocationRef    locationID,
                                                      OTF2_TimeStamp      time,
                                                      void*               userData,
                                                      OTF2_AttributeList* attributeList  );


/** @brief Registers the callback for the MpiCollectiveBegin event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiCollectiveBeginCallback  Function which should be called for all
 *                                  MpiCollectiveBegin events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveBeginCallback(
    OTF2_GlobalEvtReaderCallbacks*                  globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback );


/** @brief Callback for the MpiCollectiveEnd event record.
 *
 *  A MpiCollectiveEnd record marks the end of an MPI collective operation
 *  (MPI_GATHER, MPI_SCATTER etc.). It keeps the necessary information
 *  for this event: type of collective operation, communicator, the
 *  root of this collective operation. You can optionally add further
 *  information like sent and received bytes.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param collectiveOp Determines which collective operation it is.
 *  @param communicator Communicator References a @ref Comm definition and will
 *                      be mapped to the global definition if a mapping table
 *                      of type @ref OTF2_MAPPING_COMM is available.
 *  @param root         MPI rank of root in @a communicator.
 *  @param sizeSent     Size of the sent message.
 *  @param sizeReceived Size of the received message.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_MpiCollectiveEnd )( OTF2_LocationRef    locationID,
                                                    OTF2_TimeStamp      time,
                                                    void*               userData,
                                                    OTF2_AttributeList* attributeList,
                                                    OTF2_CollectiveOp   collectiveOp,
                                                    OTF2_CommRef        communicator,
                                                    uint32_t            root,
                                                    uint64_t            sizeSent,
                                                    uint64_t            sizeReceived );


/** @brief Registers the callback for the MpiCollectiveEnd event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param mpiCollectiveEndCallback  Function which should be called for all
 *                                  MpiCollectiveEnd events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveEndCallback(
    OTF2_GlobalEvtReaderCallbacks*                globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback );


/** @brief Callback for the OmpFork event record.
 *
 *  An OmpFork record marks that an OpenMP Thread forks a thread team.
 *
 *  This event record is superseded by the @eref{ThreadFork} event record
 *  and should not be used when the @eref{ThreadFork} event record is
 *  in use.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param numberOfRequestedThreads Requested size of the team.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_OmpFork )( OTF2_LocationRef    locationID,
                                           OTF2_TimeStamp      time,
                                           void*               userData,
                                           OTF2_AttributeList* attributeList,
                                           uint32_t            numberOfRequestedThreads );


/** @brief Registers the callback for the OmpFork event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param ompForkCallback  Function which should be called for all
 *                                  OmpFork events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetOmpForkCallback(
    OTF2_GlobalEvtReaderCallbacks*       globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_OmpFork ompForkCallback );


/** @brief Callback for the OmpJoin event record.
 *
 *  An OmpJoin record marks that a team of threads is joint and only the
 *  master thread continues execution.
 *
 *  This event record is superseded by the @eref{ThreadJoin} event record
 *  and should not be used when the @eref{ThreadJoin} event record is
 *  in use.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_OmpJoin )( OTF2_LocationRef    locationID,
                                           OTF2_TimeStamp      time,
                                           void*               userData,
                                           OTF2_AttributeList* attributeList  );


/** @brief Registers the callback for the OmpJoin event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param ompJoinCallback  Function which should be called for all
 *                                  OmpJoin events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetOmpJoinCallback(
    OTF2_GlobalEvtReaderCallbacks*       globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_OmpJoin ompJoinCallback );


/** @brief Callback for the OmpAcquireLock event record.
 *
 *  An OmpAcquireLock record marks that a thread acquires an OpenMP lock.
 *
 *  This event record is superseded by the @eref{ThreadAcquireLock} event
 *  record and should not be used when the @eref{ThreadAcquireLock}
 *  event record is in use record.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param lockID           ID of the lock.
 *  @param acquisitionOrder A monotonically increasing number to determine the
 *                          order of lock acquisitions (with unsynchronized
 *                          clocks this is otherwise not possible).
 *                          Corresponding acquire-release events have same
 *                          number.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_OmpAcquireLock )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      time,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  uint32_t            lockID,
                                                  uint32_t            acquisitionOrder );


/** @brief Registers the callback for the OmpAcquireLock event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param ompAcquireLockCallback  Function which should be called for all
 *                                  OmpAcquireLock events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetOmpAcquireLockCallback(
    OTF2_GlobalEvtReaderCallbacks*              globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_OmpAcquireLock ompAcquireLockCallback );


/** @brief Callback for the OmpReleaseLock event record.
 *
 *  An OmpReleaseLock record marks that a thread releases an OpenMP lock.
 *
 *  This event record is superseded by the @eref{ThreadReleaseLock} event
 *  record and should not be used when the @eref{ThreadReleaseLock}
 *  event record is in use.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param lockID           ID of the lock.
 *  @param acquisitionOrder A monotonically increasing number to determine the
 *                          order of lock acquisitions (with unsynchronized
 *                          clocks this is otherwise not possible).
 *                          Corresponding acquire-release events have same
 *                          number.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_OmpReleaseLock )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      time,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  uint32_t            lockID,
                                                  uint32_t            acquisitionOrder );


/** @brief Registers the callback for the OmpReleaseLock event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param ompReleaseLockCallback  Function which should be called for all
 *                                  OmpReleaseLock events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetOmpReleaseLockCallback(
    OTF2_GlobalEvtReaderCallbacks*              globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_OmpReleaseLock ompReleaseLockCallback );


/** @brief Callback for the OmpTaskCreate event record.
 *
 *  An OmpTaskCreate record marks that an OpenMP Task was/will be created
 *  in the current region.
 *
 *  This event record is superseded by the @eref{ThreadTaskCreate} event
 *  record and should not be used when the @eref{ThreadTaskCreate}
 *  event record is in use.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param taskID Identifier of the newly created task instance.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_OmpTaskCreate )( OTF2_LocationRef    locationID,
                                                 OTF2_TimeStamp      time,
                                                 void*               userData,
                                                 OTF2_AttributeList* attributeList,
                                                 uint64_t            taskID );


/** @brief Registers the callback for the OmpTaskCreate event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param ompTaskCreateCallback  Function which should be called for all
 *                                  OmpTaskCreate events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCreateCallback(
    OTF2_GlobalEvtReaderCallbacks*             globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_OmpTaskCreate ompTaskCreateCallback );


/** @brief Callback for the OmpTaskSwitch event record.
 *
 *  An OmpTaskSwitch record indicates that the execution of the current
 *  task will be suspended and another task starts/restarts its
 *  execution. Please note that this may change the current call stack
 *  of the executing location.
 *
 *  This event record is superseded by the @eref{ThreadTaskSwitch} event
 *  record and should not be used when the @eref{ThreadTaskSwitch}
 *  event record is in use.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param taskID Identifier of the now active task instance.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_OmpTaskSwitch )( OTF2_LocationRef    locationID,
                                                 OTF2_TimeStamp      time,
                                                 void*               userData,
                                                 OTF2_AttributeList* attributeList,
                                                 uint64_t            taskID );


/** @brief Registers the callback for the OmpTaskSwitch event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param ompTaskSwitchCallback  Function which should be called for all
 *                                  OmpTaskSwitch events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetOmpTaskSwitchCallback(
    OTF2_GlobalEvtReaderCallbacks*             globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_OmpTaskSwitch ompTaskSwitchCallback );


/** @brief Callback for the OmpTaskComplete event record.
 *
 *  An OmpTaskComplete record indicates that the execution of an OpenMP
 *  task has finished.
 *
 *  This event record is superseded by the @eref{ThreadTaskComplete} event
 *  record and should not be used when the @eref{ThreadTaskComplete}
 *  event record is in use.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param taskID Identifier of the completed task instance.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_OmpTaskComplete )( OTF2_LocationRef    locationID,
                                                   OTF2_TimeStamp      time,
                                                   void*               userData,
                                                   OTF2_AttributeList* attributeList,
                                                   uint64_t            taskID );


/** @brief Registers the callback for the OmpTaskComplete event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param ompTaskCompleteCallback  Function which should be called for all
 *                                  OmpTaskComplete events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCompleteCallback(
    OTF2_GlobalEvtReaderCallbacks*               globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_OmpTaskComplete ompTaskCompleteCallback );


/** @brief Callback for the Metric event record.
 *
 *  A metric event is always stored at the location that recorded the
 *  metric. A metric event can reference a metric class or metric
 *  instance. Therefore, metric classes and instances share same ID
 *  space. Synchronous metrics are always located right before the
 *  according enter and leave event.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param metric          Could be a metric class or a metric instance.
 *                         References a @ref MetricClass, or a @ref
 *                         MetricInstance definition and will be mapped to
 *                         the global definition if a mapping table of type
 *                         @ref OTF2_MAPPING_METRIC is available.
 *  @param numberOfMetrics Number of metrics with in the set.
 *  @param typeIDs         List of metric types.
 *  @param metricValues    List of metric values.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_Metric )( OTF2_LocationRef        locationID,
                                          OTF2_TimeStamp          time,
                                          void*                   userData,
                                          OTF2_AttributeList*     attributeList,
                                          OTF2_MetricRef          metric,
                                          uint8_t                 numberOfMetrics,
                                          const OTF2_Type*        typeIDs,
                                          const OTF2_MetricValue* metricValues );


/** @brief Registers the callback for the Metric event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param metricCallback  Function which should be called for all
 *                                  Metric events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetMetricCallback(
    OTF2_GlobalEvtReaderCallbacks*      globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_Metric metricCallback );


/** @brief Callback for the ParameterString event record.
 *
 *  A ParameterString record marks that in the current region, the
 *  specified string parameter has the specified value.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param parameter Parameter ID. References a @ref Parameter definition and
 *                   will be mapped to the global definition if a mapping
 *                   table of type @ref OTF2_MAPPING_PARAMETER is available.
 *  @param string    Value: Handle of a string definition References a @ref
 *                   String definition and will be mapped to the global
 *                   definition if a mapping table of type @ref
 *                   OTF2_MAPPING_STRING is available.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ParameterString )( OTF2_LocationRef    locationID,
                                                   OTF2_TimeStamp      time,
                                                   void*               userData,
                                                   OTF2_AttributeList* attributeList,
                                                   OTF2_ParameterRef   parameter,
                                                   OTF2_StringRef      string );


/** @brief Registers the callback for the ParameterString event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param parameterStringCallback  Function which should be called for all
 *                                  ParameterString events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetParameterStringCallback(
    OTF2_GlobalEvtReaderCallbacks*               globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ParameterString parameterStringCallback );


/** @brief Callback for the ParameterInt event record.
 *
 *  A ParameterInt record marks that in the current region, the specified
 *  integer parameter has the specified value.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param parameter Parameter ID. References a @ref Parameter definition and
 *                   will be mapped to the global definition if a mapping
 *                   table of type @ref OTF2_MAPPING_PARAMETER is available.
 *  @param value     Value of the recorded parameter.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ParameterInt )( OTF2_LocationRef    locationID,
                                                OTF2_TimeStamp      time,
                                                void*               userData,
                                                OTF2_AttributeList* attributeList,
                                                OTF2_ParameterRef   parameter,
                                                int64_t             value );


/** @brief Registers the callback for the ParameterInt event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param parameterIntCallback  Function which should be called for all
 *                                  ParameterInt events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetParameterIntCallback(
    OTF2_GlobalEvtReaderCallbacks*            globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ParameterInt parameterIntCallback );


/** @brief Callback for the ParameterUnsignedInt event record.
 *
 *  A ParameterUnsignedInt record marks that in the current region, the
 *  specified unsigned integer parameter has the specified value.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param parameter Parameter ID. References a @ref Parameter definition and
 *                   will be mapped to the global definition if a mapping
 *                   table of type @ref OTF2_MAPPING_PARAMETER is available.
 *  @param value     Value of the recorded parameter.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ParameterUnsignedInt )( OTF2_LocationRef    locationID,
                                                        OTF2_TimeStamp      time,
                                                        void*               userData,
                                                        OTF2_AttributeList* attributeList,
                                                        OTF2_ParameterRef   parameter,
                                                        uint64_t            value );


/** @brief Registers the callback for the ParameterUnsignedInt event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param parameterUnsignedIntCallback  Function which should be called for all
 *                                  ParameterUnsignedInt events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetParameterUnsignedIntCallback(
    OTF2_GlobalEvtReaderCallbacks*                    globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback );


/** @brief Callback for the RmaWinCreate event record.
 *
 *  An RmaWinCreate record denotes the creation of an RMA window.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win ID of the window created. References a @ref RmaWin definition and
 *             will be mapped to the global definition if a mapping table of
 *             type @ref OTF2_MAPPING_RMA_WIN is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaWinCreate )( OTF2_LocationRef    locationID,
                                                OTF2_TimeStamp      time,
                                                void*               userData,
                                                OTF2_AttributeList* attributeList,
                                                OTF2_RmaWinRef      win );


/** @brief Registers the callback for the RmaWinCreate event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaWinCreateCallback  Function which should be called for all
 *                                  RmaWinCreate events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaWinCreateCallback(
    OTF2_GlobalEvtReaderCallbacks*            globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaWinCreate rmaWinCreateCallback );


/** @brief Callback for the RmaWinDestroy event record.
 *
 *  An RmaWinDestroy record denotes the destruction of an RMA window.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win ID of the window destructed. References a @ref RmaWin definition
 *             and will be mapped to the global definition if a mapping table
 *             of type @ref OTF2_MAPPING_RMA_WIN is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaWinDestroy )( OTF2_LocationRef    locationID,
                                                 OTF2_TimeStamp      time,
                                                 void*               userData,
                                                 OTF2_AttributeList* attributeList,
                                                 OTF2_RmaWinRef      win );


/** @brief Registers the callback for the RmaWinDestroy event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaWinDestroyCallback  Function which should be called for all
 *                                  RmaWinDestroy events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaWinDestroyCallback(
    OTF2_GlobalEvtReaderCallbacks*             globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaWinDestroy rmaWinDestroyCallback );


/** @brief Callback for the RmaCollectiveBegin event record.
 *
 *  An RmaCollectiveBegin record denotes the beginnig of a collective RMA
 *  operation.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaCollectiveBegin )( OTF2_LocationRef    locationID,
                                                      OTF2_TimeStamp      time,
                                                      void*               userData,
                                                      OTF2_AttributeList* attributeList  );


/** @brief Registers the callback for the RmaCollectiveBegin event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaCollectiveBeginCallback  Function which should be called for all
 *                                  RmaCollectiveBegin events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaCollectiveBeginCallback(
    OTF2_GlobalEvtReaderCallbacks*                  globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaCollectiveBegin rmaCollectiveBeginCallback );


/** @brief Callback for the RmaCollectiveEnd event record.
 *
 *  "An RmaCollectiveEnd record denotes the end of a collective RMA
 *  operation.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param collectiveOp  Determines which collective operation it is.
 *  @param syncLevel     Synchronization level of this collective operation.
 *  @param win           ID of the window used for this operation. References a
 *                       @ref RmaWin definition and will be mapped to the
 *                       global definition if a mapping table of type @ref
 *                       OTF2_MAPPING_RMA_WIN is available.
 *  @param root          Root process for this operation.
 *  @param bytesSent     Bytes sent in operation.
 *  @param bytesReceived Bytes receives in operation.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaCollectiveEnd )( OTF2_LocationRef    locationID,
                                                    OTF2_TimeStamp      time,
                                                    void*               userData,
                                                    OTF2_AttributeList* attributeList,
                                                    OTF2_CollectiveOp   collectiveOp,
                                                    OTF2_RmaSyncLevel   syncLevel,
                                                    OTF2_RmaWinRef      win,
                                                    uint32_t            root,
                                                    uint64_t            bytesSent,
                                                    uint64_t            bytesReceived );


/** @brief Registers the callback for the RmaCollectiveEnd event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaCollectiveEndCallback  Function which should be called for all
 *                                  RmaCollectiveEnd events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaCollectiveEndCallback(
    OTF2_GlobalEvtReaderCallbacks*                globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaCollectiveEnd rmaCollectiveEndCallback );


/** @brief Callback for the RmaGroupSync event record.
 *
 *  An RmaGroupSync record denotes the synchronization with a subgroup of
 *  processes on a window.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param syncLevel Synchronization level of this collective operation.
 *  @param win       ID of the window used for this operation. References a @ref
 *                   RmaWin definition and will be mapped to the global
 *                   definition if a mapping table of type @ref
 *                   OTF2_MAPPING_RMA_WIN is available.
 *  @param group     Group of remote processes involved in synchronization.
 *                   References a @ref Group definition and will be mapped to
 *                   the global definition if a mapping table of type @ref
 *                   OTF2_MAPPING_GROUP is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaGroupSync )( OTF2_LocationRef    locationID,
                                                OTF2_TimeStamp      time,
                                                void*               userData,
                                                OTF2_AttributeList* attributeList,
                                                OTF2_RmaSyncLevel   syncLevel,
                                                OTF2_RmaWinRef      win,
                                                OTF2_GroupRef       group );


/** @brief Registers the callback for the RmaGroupSync event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaGroupSyncCallback  Function which should be called for all
 *                                  RmaGroupSync events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaGroupSyncCallback(
    OTF2_GlobalEvtReaderCallbacks*            globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaGroupSync rmaGroupSyncCallback );


/** @brief Callback for the RmaRequestLock event record.
 *
 *  An RmaRequestLock record denotes the time a lock was requested and
 *  with it the earliest time it could have been granted.  It is used
 *  to mark (possibly) non-blocking lock request, as defined by the
 *  MPI standard.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win      ID of the window used for this operation. References a @ref
 *                  RmaWin definition and will be mapped to the global
 *                  definition if a mapping table of type @ref
 *                  OTF2_MAPPING_RMA_WIN is available.
 *  @param remote   Rank of the locked remote process.
 *  @param lockId   ID of the lock aquired, if multiple locks are defined on a
 *                  window.
 *  @param lockType Type of lock aquired.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaRequestLock )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      time,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  OTF2_RmaWinRef      win,
                                                  uint32_t            remote,
                                                  uint64_t            lockId,
                                                  OTF2_LockType       lockType );


/** @brief Registers the callback for the RmaRequestLock event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaRequestLockCallback  Function which should be called for all
 *                                  RmaRequestLock events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaRequestLockCallback(
    OTF2_GlobalEvtReaderCallbacks*              globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaRequestLock rmaRequestLockCallback );


/** @brief Callback for the RmaAcquireLock event record.
 *
 *  An RmaAcquireLock record denotes the time a lock was aquired by the
 *  process.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win      ID of the window used for this operation. References a @ref
 *                  RmaWin definition and will be mapped to the global
 *                  definition if a mapping table of type @ref
 *                  OTF2_MAPPING_RMA_WIN is available.
 *  @param remote   Rank of the locked remote process.
 *  @param lockId   ID of the lock aquired, if multiple locks are defined on a
 *                  window.
 *  @param lockType Type of lock aquired.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaAcquireLock )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      time,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  OTF2_RmaWinRef      win,
                                                  uint32_t            remote,
                                                  uint64_t            lockId,
                                                  OTF2_LockType       lockType );


/** @brief Registers the callback for the RmaAcquireLock event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaAcquireLockCallback  Function which should be called for all
 *                                  RmaAcquireLock events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaAcquireLockCallback(
    OTF2_GlobalEvtReaderCallbacks*              globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaAcquireLock rmaAcquireLockCallback );


/** @brief Callback for the RmaTryLock event record.
 *
 *  An RmaTryLock record denotes the time of an unsuccessful attempt to
 *  acquire the lock.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win      ID of the window used for this operation. References a @ref
 *                  RmaWin definition and will be mapped to the global
 *                  definition if a mapping table of type @ref
 *                  OTF2_MAPPING_RMA_WIN is available.
 *  @param remote   Rank of the locked remote process.
 *  @param lockId   ID of the lock aquired, if multiple locks are defined on a
 *                  window.
 *  @param lockType Type of lock aquired.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaTryLock )( OTF2_LocationRef    locationID,
                                              OTF2_TimeStamp      time,
                                              void*               userData,
                                              OTF2_AttributeList* attributeList,
                                              OTF2_RmaWinRef      win,
                                              uint32_t            remote,
                                              uint64_t            lockId,
                                              OTF2_LockType       lockType );


/** @brief Registers the callback for the RmaTryLock event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaTryLockCallback  Function which should be called for all
 *                                  RmaTryLock events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaTryLockCallback(
    OTF2_GlobalEvtReaderCallbacks*          globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaTryLock rmaTryLockCallback );


/** @brief Callback for the RmaReleaseLock event record.
 *
 *  An RmaReleaseLock record denotes the time the lock was released.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win    ID of the window used for this operation. References a @ref
 *                RmaWin definition and will be mapped to the global
 *                definition if a mapping table of type @ref
 *                OTF2_MAPPING_RMA_WIN is available.
 *  @param remote Rank of the locked remote process.
 *  @param lockId ID of the lock released, if multiple locks are defined on a
 *                window.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaReleaseLock )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      time,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  OTF2_RmaWinRef      win,
                                                  uint32_t            remote,
                                                  uint64_t            lockId );


/** @brief Registers the callback for the RmaReleaseLock event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaReleaseLockCallback  Function which should be called for all
 *                                  RmaReleaseLock events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaReleaseLockCallback(
    OTF2_GlobalEvtReaderCallbacks*              globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaReleaseLock rmaReleaseLockCallback );


/** @brief Callback for the RmaSync event record.
 *
 *  An RmaSync record denotes the direct synchronization with a possibly
 *  remote process.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win      ID of the window used for this operation. References a @ref
 *                  RmaWin definition and will be mapped to the global
 *                  definition if a mapping table of type @ref
 *                  OTF2_MAPPING_RMA_WIN is available.
 *  @param remote   Rank of the locked remote process.
 *  @param syncType Type of synchronization.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaSync )( OTF2_LocationRef    locationID,
                                           OTF2_TimeStamp      time,
                                           void*               userData,
                                           OTF2_AttributeList* attributeList,
                                           OTF2_RmaWinRef      win,
                                           uint32_t            remote,
                                           OTF2_RmaSyncType    syncType );


/** @brief Registers the callback for the RmaSync event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaSyncCallback  Function which should be called for all
 *                                  RmaSync events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaSyncCallback(
    OTF2_GlobalEvtReaderCallbacks*       globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaSync rmaSyncCallback );


/** @brief Callback for the RmaWaitChange event record.
 *
 *  An RmaWaitChange record denotes the change of a window that was waited
 *  for.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win ID of the window used for this operation. References a @ref RmaWin
 *             definition and will be mapped to the global definition if a
 *             mapping table of type @ref OTF2_MAPPING_RMA_WIN is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaWaitChange )( OTF2_LocationRef    locationID,
                                                 OTF2_TimeStamp      time,
                                                 void*               userData,
                                                 OTF2_AttributeList* attributeList,
                                                 OTF2_RmaWinRef      win );


/** @brief Registers the callback for the RmaWaitChange event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaWaitChangeCallback  Function which should be called for all
 *                                  RmaWaitChange events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaWaitChangeCallback(
    OTF2_GlobalEvtReaderCallbacks*             globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaWaitChange rmaWaitChangeCallback );


/** @brief Callback for the RmaPut event record.
 *
 *  An RmaPut record denotes the time a put operation was issued.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param remote     Rank of the target process.
 *  @param bytes      Bytes sent to target.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaPut )( OTF2_LocationRef    locationID,
                                          OTF2_TimeStamp      time,
                                          void*               userData,
                                          OTF2_AttributeList* attributeList,
                                          OTF2_RmaWinRef      win,
                                          uint32_t            remote,
                                          uint64_t            bytes,
                                          uint64_t            matchingId );


/** @brief Registers the callback for the RmaPut event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaPutCallback  Function which should be called for all
 *                                  RmaPut events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaPutCallback(
    OTF2_GlobalEvtReaderCallbacks*      globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaPut rmaPutCallback );


/** @brief Callback for the RmaGet event record.
 *
 *  An RmaGet record denotes the time a put operation was issued.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param remote     Rank of the target process.
 *  @param bytes      Bytes received from target.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaGet )( OTF2_LocationRef    locationID,
                                          OTF2_TimeStamp      time,
                                          void*               userData,
                                          OTF2_AttributeList* attributeList,
                                          OTF2_RmaWinRef      win,
                                          uint32_t            remote,
                                          uint64_t            bytes,
                                          uint64_t            matchingId );


/** @brief Registers the callback for the RmaGet event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaGetCallback  Function which should be called for all
 *                                  RmaGet events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaGetCallback(
    OTF2_GlobalEvtReaderCallbacks*      globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaGet rmaGetCallback );


/** @brief Callback for the RmaAtomic event record.
 *
 *  An RmaAtomic record denotes the time a atomic operation was issued.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win           ID of the window used for this operation. References a
 *                       @ref RmaWin definition and will be mapped to the
 *                       global definition if a mapping table of type @ref
 *                       OTF2_MAPPING_RMA_WIN is available.
 *  @param remote        Rank of the target process.
 *  @param type          Type of atomic operation.
 *  @param bytesSent     Bytes sent to target.
 *  @param bytesReceived Bytes received from target.
 *  @param matchingId    ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaAtomic )( OTF2_LocationRef    locationID,
                                             OTF2_TimeStamp      time,
                                             void*               userData,
                                             OTF2_AttributeList* attributeList,
                                             OTF2_RmaWinRef      win,
                                             uint32_t            remote,
                                             OTF2_RmaAtomicType  type,
                                             uint64_t            bytesSent,
                                             uint64_t            bytesReceived,
                                             uint64_t            matchingId );


/** @brief Registers the callback for the RmaAtomic event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaAtomicCallback  Function which should be called for all
 *                                  RmaAtomic events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaAtomicCallback(
    OTF2_GlobalEvtReaderCallbacks*         globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaAtomic rmaAtomicCallback );


/** @brief Callback for the RmaOpCompleteBlocking event record.
 *
 *  An RmaOpCompleteBlocking record denotes the local completion of a
 *  blocking RMA operation.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaOpCompleteBlocking )( OTF2_LocationRef    locationID,
                                                         OTF2_TimeStamp      time,
                                                         void*               userData,
                                                         OTF2_AttributeList* attributeList,
                                                         OTF2_RmaWinRef      win,
                                                         uint64_t            matchingId );


/** @brief Registers the callback for the RmaOpCompleteBlocking event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaOpCompleteBlockingCallback  Function which should be called for all
 *                                  RmaOpCompleteBlocking events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaOpCompleteBlockingCallback(
    OTF2_GlobalEvtReaderCallbacks*                     globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaOpCompleteBlocking rmaOpCompleteBlockingCallback );


/** @brief Callback for the RmaOpCompleteNonBlocking event record.
 *
 *  An RmaOpCompleteNonBlocking record denotes the local completion of a
 *  non-blocking RMA operation.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaOpCompleteNonBlocking )( OTF2_LocationRef    locationID,
                                                            OTF2_TimeStamp      time,
                                                            void*               userData,
                                                            OTF2_AttributeList* attributeList,
                                                            OTF2_RmaWinRef      win,
                                                            uint64_t            matchingId );


/** @brief Registers the callback for the RmaOpCompleteNonBlocking event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaOpCompleteNonBlockingCallback  Function which should be called for all
 *                                  RmaOpCompleteNonBlocking events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaOpCompleteNonBlockingCallback(
    OTF2_GlobalEvtReaderCallbacks*                        globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaOpCompleteNonBlocking rmaOpCompleteNonBlockingCallback );


/** @brief Callback for the RmaOpTest event record.
 *
 *  An RmaOpTest record denotes that a non-blocking RMA operation has been
 *  tested for completion unsuccessfully.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaOpTest )( OTF2_LocationRef    locationID,
                                             OTF2_TimeStamp      time,
                                             void*               userData,
                                             OTF2_AttributeList* attributeList,
                                             OTF2_RmaWinRef      win,
                                             uint64_t            matchingId );


/** @brief Registers the callback for the RmaOpTest event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaOpTestCallback  Function which should be called for all
 *                                  RmaOpTest events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaOpTestCallback(
    OTF2_GlobalEvtReaderCallbacks*         globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaOpTest rmaOpTestCallback );


/** @brief Callback for the RmaOpCompleteRemote event record.
 *
 *  An RmaOpCompleteRemote record denotes the local completion of an RMA
 *  operation.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_RmaOpCompleteRemote )( OTF2_LocationRef    locationID,
                                                       OTF2_TimeStamp      time,
                                                       void*               userData,
                                                       OTF2_AttributeList* attributeList,
                                                       OTF2_RmaWinRef      win,
                                                       uint64_t            matchingId );


/** @brief Registers the callback for the RmaOpCompleteRemote event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param rmaOpCompleteRemoteCallback  Function which should be called for all
 *                                  RmaOpCompleteRemote events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetRmaOpCompleteRemoteCallback(
    OTF2_GlobalEvtReaderCallbacks*                   globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_RmaOpCompleteRemote rmaOpCompleteRemoteCallback );


/** @brief Callback for the ThreadFork event record.
 *
 *  An ThreadFork record marks that an thread forks a thread team.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *
 *  @param numberOfRequestedThreads Requested size of the team.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadFork )( OTF2_LocationRef    locationID,
                                              OTF2_TimeStamp      time,
                                              void*               userData,
                                              OTF2_AttributeList* attributeList,
                                              OTF2_Paradigm       model,
                                              uint32_t            numberOfRequestedThreads );


/** @brief Registers the callback for the ThreadFork event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadForkCallback  Function which should be called for all
 *                                  ThreadFork events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadForkCallback(
    OTF2_GlobalEvtReaderCallbacks*          globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadFork threadForkCallback );


/** @brief Callback for the ThreadJoin event record.
 *
 *  An ThreadJoin record marks that a team of threads is joint and only
 *  the master thread continues execution.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadJoin )( OTF2_LocationRef    locationID,
                                              OTF2_TimeStamp      time,
                                              void*               userData,
                                              OTF2_AttributeList* attributeList,
                                              OTF2_Paradigm       model );


/** @brief Registers the callback for the ThreadJoin event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadJoinCallback  Function which should be called for all
 *                                  ThreadJoin events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadJoinCallback(
    OTF2_GlobalEvtReaderCallbacks*          globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadJoin threadJoinCallback );


/** @brief Callback for the ThreadTeamBegin event record.
 *
 *
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param threadTeam Thread team References a @ref Comm definition and will be
 *                    mapped to the global definition if a mapping table of
 *                    type @ref OTF2_MAPPING_COMM is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadTeamBegin )( OTF2_LocationRef    locationID,
                                                   OTF2_TimeStamp      time,
                                                   void*               userData,
                                                   OTF2_AttributeList* attributeList,
                                                   OTF2_CommRef        threadTeam );


/** @brief Registers the callback for the ThreadTeamBegin event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadTeamBeginCallback  Function which should be called for all
 *                                  ThreadTeamBegin events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadTeamBeginCallback(
    OTF2_GlobalEvtReaderCallbacks*               globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadTeamBegin threadTeamBeginCallback );


/** @brief Callback for the ThreadTeamEnd event record.
 *
 *
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param threadTeam Thread team References a @ref Comm definition and will be
 *                    mapped to the global definition if a mapping table of
 *                    type @ref OTF2_MAPPING_COMM is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadTeamEnd )( OTF2_LocationRef    locationID,
                                                 OTF2_TimeStamp      time,
                                                 void*               userData,
                                                 OTF2_AttributeList* attributeList,
                                                 OTF2_CommRef        threadTeam );


/** @brief Registers the callback for the ThreadTeamEnd event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadTeamEndCallback  Function which should be called for all
 *                                  ThreadTeamEnd events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadTeamEndCallback(
    OTF2_GlobalEvtReaderCallbacks*             globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadTeamEnd threadTeamEndCallback );


/** @brief Callback for the ThreadAcquireLock event record.
 *
 *  An ThreadAcquireLock record marks that a thread acquires an lock.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *
 *  @param lockID           ID of the lock.
 *  @param acquisitionOrder A monotonically increasing number to determine the
 *                          order of lock acquisitions (with unsynchronized
 *                          clocks this is otherwise not possible).
 *                          Corresponding acquire-release events have same
 *                          number.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadAcquireLock )( OTF2_LocationRef    locationID,
                                                     OTF2_TimeStamp      time,
                                                     void*               userData,
                                                     OTF2_AttributeList* attributeList,
                                                     OTF2_Paradigm       model,
                                                     uint32_t            lockID,
                                                     uint32_t            acquisitionOrder );


/** @brief Registers the callback for the ThreadAcquireLock event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadAcquireLockCallback  Function which should be called for all
 *                                  ThreadAcquireLock events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadAcquireLockCallback(
    OTF2_GlobalEvtReaderCallbacks*                 globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadAcquireLock threadAcquireLockCallback );


/** @brief Callback for the ThreadReleaseLock event record.
 *
 *  An ThreadReleaseLock record marks that a thread releases an lock.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *
 *  @param lockID           ID of the lock.
 *  @param acquisitionOrder A monotonically increasing number to determine the
 *                          order of lock acquisitions (with unsynchronized
 *                          clocks this is otherwise not possible).
 *                          Corresponding acquire-release events have same
 *                          number.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadReleaseLock )( OTF2_LocationRef    locationID,
                                                     OTF2_TimeStamp      time,
                                                     void*               userData,
                                                     OTF2_AttributeList* attributeList,
                                                     OTF2_Paradigm       model,
                                                     uint32_t            lockID,
                                                     uint32_t            acquisitionOrder );


/** @brief Registers the callback for the ThreadReleaseLock event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadReleaseLockCallback  Function which should be called for all
 *                                  ThreadReleaseLock events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadReleaseLockCallback(
    OTF2_GlobalEvtReaderCallbacks*                 globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadReleaseLock threadReleaseLockCallback );


/** @brief Callback for the ThreadTaskCreate event record.
 *
 *  An ThreadTaskCreate record marks that an task in was/will be created
 *  and will be processed by the specified thread team.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param threadTeam       Thread team References a @ref Comm definition and
 *                          will be mapped to the global definition if a
 *                          mapping table of type @ref OTF2_MAPPING_COMM is
 *                          available.
 *  @param creatingThread   Creating thread of this task. (This is redundant,
 *                          remove?)
 *  @param generationNumber Thread-private generation number of task's creating
 *                          thread.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadTaskCreate )( OTF2_LocationRef    locationID,
                                                    OTF2_TimeStamp      time,
                                                    void*               userData,
                                                    OTF2_AttributeList* attributeList,
                                                    OTF2_CommRef        threadTeam,
                                                    uint32_t            creatingThread,
                                                    uint32_t            generationNumber );


/** @brief Registers the callback for the ThreadTaskCreate event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadTaskCreateCallback  Function which should be called for all
 *                                  ThreadTaskCreate events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadTaskCreateCallback(
    OTF2_GlobalEvtReaderCallbacks*                globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadTaskCreate threadTaskCreateCallback );


/** @brief Callback for the ThreadTaskSwitch event record.
 *
 *  An ThreadTaskSwitch record indicates that the execution of the current
 *  task will be suspended and another task starts/restarts its
 *  execution. Please note that this may change the current call stack
 *  of the executing location.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param threadTeam       Thread team References a @ref Comm definition and
 *                          will be mapped to the global definition if a
 *                          mapping table of type @ref OTF2_MAPPING_COMM is
 *                          available.
 *  @param creatingThread   Creating thread of this task.
 *  @param generationNumber Thread-private generation number of task's creating
 *                          thread.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadTaskSwitch )( OTF2_LocationRef    locationID,
                                                    OTF2_TimeStamp      time,
                                                    void*               userData,
                                                    OTF2_AttributeList* attributeList,
                                                    OTF2_CommRef        threadTeam,
                                                    uint32_t            creatingThread,
                                                    uint32_t            generationNumber );


/** @brief Registers the callback for the ThreadTaskSwitch event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadTaskSwitchCallback  Function which should be called for all
 *                                  ThreadTaskSwitch events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadTaskSwitchCallback(
    OTF2_GlobalEvtReaderCallbacks*                globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadTaskSwitch threadTaskSwitchCallback );


/** @brief Callback for the ThreadTaskComplete event record.
 *
 *  An ThreadTaskComplete record indicates that the execution of an OpenMP
 *  task has finished.
 *
 *  @param locationID    The location where this event happened.
 *  @param time          The time when this event happened.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalEvtCallbacks or
 *                       @ref OTF2_GlobalEvtReader_SetCallbacks.
 *  @param attributeList Additional attributes for this event.
 *
 *  @param threadTeam       Thread team References a @ref Comm definition and
 *                          will be mapped to the global definition if a
 *                          mapping table of type @ref OTF2_MAPPING_COMM is
 *                          available.
 *  @param creatingThread   Creating thread of this task.
 *  @param generationNumber Thread-private generation number of task's creating
 *                          thread.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalEvtReaderCallback_ThreadTaskComplete )( OTF2_LocationRef    locationID,
                                                      OTF2_TimeStamp      time,
                                                      void*               userData,
                                                      OTF2_AttributeList* attributeList,
                                                      OTF2_CommRef        threadTeam,
                                                      uint32_t            creatingThread,
                                                      uint32_t            generationNumber );


/** @brief Registers the callback for the ThreadTaskComplete event.
 *
 *  @param globalEvtReaderCallbacks Struct for all callbacks.
 *  @param threadTaskCompleteCallback  Function which should be called for all
 *                                  ThreadTaskComplete events.
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetThreadTaskCompleteCallback(
    OTF2_GlobalEvtReaderCallbacks*                  globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_ThreadTaskComplete threadTaskCompleteCallback );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GLOBAL_EVT_READER_CALLBACKS_H */
