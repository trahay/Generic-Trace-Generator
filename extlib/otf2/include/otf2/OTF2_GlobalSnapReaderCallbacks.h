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


/** @brief Callback for the SnapshotStart snap record.
 *
 *  This record marks the start of a snapshot.
 *
 *  A snapshot consists of an timestamp and a set of snapshot records. All
 *  these snapshot records have the same snapshot time. A snapshot
 *  starts with one @eref{SnapshotStart} record and closes with one
 *  @eref{SnapshotEnd} record. All snapshot records inbetween are
 *  ordered by the @a origEventTime, which are also less than the
 *  snapshot timestamp. Ie. The timestamp of the next event read from
 *  the event stream is greater or equal to the snapshot time.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param numberOfRecords Number of snapshot event records in this snapshot.
 *                         Excluding the @eref{SnapshotEnd} record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_SnapshotStart )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      snapTime,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  uint64_t            numberOfRecords );


/** @brief Registers the callback for the SnapshotStart snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param snapshotStartCallback  Function which should be called for all
 *                                  SnapshotStart snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetSnapshotStartCallback(
    OTF2_GlobalSnapReaderCallbacks*             globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_SnapshotStart snapshotStartCallback );


/** @brief Callback for the SnapshotEnd snap record.
 *
 *  This record marks the end of a snapshot. It contains the position to
 *  continue reading in the event trace for this location. Use @ref
 *  OTF2_EvtReader_Seek with @a contReadPos as the position.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param contReadPos Position to continue reading in the event trace.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_SnapshotEnd )( OTF2_LocationRef    locationID,
                                                OTF2_TimeStamp      snapTime,
                                                void*               userData,
                                                OTF2_AttributeList* attributeList,
                                                uint64_t            contReadPos );


/** @brief Registers the callback for the SnapshotEnd snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param snapshotEndCallback  Function which should be called for all
 *                                  SnapshotEnd snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetSnapshotEndCallback(
    OTF2_GlobalSnapReaderCallbacks*           globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_SnapshotEnd snapshotEndCallback );


/** @brief Callback for the MeasurementOnOff snap record.
 *
 *  The last occurrence of an @eref{MeasurementOnOff} event of this
 *  location, if any.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime   The original time this event happended.
 *  @param measurementMode Is the measurement turned on
 *                         (@eref{OTF2_MEASUREMENT_ON}) or off
 *                         (@eref{OTF2_MEASUREMENT_OFF})?
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MeasurementOnOff )( OTF2_LocationRef     locationID,
                                                     OTF2_TimeStamp       snapTime,
                                                     void*                userData,
                                                     OTF2_AttributeList*  attributeList,
                                                     OTF2_TimeStamp       origEventTime,
                                                     OTF2_MeasurementMode measurementMode );


/** @brief Registers the callback for the MeasurementOnOff snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param measurementOnOffCallback  Function which should be called for all
 *                                  MeasurementOnOff snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMeasurementOnOffCallback(
    OTF2_GlobalSnapReaderCallbacks*                globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MeasurementOnOff measurementOnOffCallback );


/** @brief Callback for the Enter snap record.
 *
 *  This record exists for each @eref{Enter} event where the corresponding
 *  @eref{Leave} event did not occur before the snapshot.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param region        Needs to be defined in a definition record References a
 *                       @ref Region definition and will be mapped to the
 *                       global definition if a mapping table of type @ref
 *                       OTF2_MAPPING_REGION is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_Enter )( OTF2_LocationRef    locationID,
                                          OTF2_TimeStamp      snapTime,
                                          void*               userData,
                                          OTF2_AttributeList* attributeList,
                                          OTF2_TimeStamp      origEventTime,
                                          OTF2_RegionRef      region );


/** @brief Registers the callback for the Enter snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param enterCallback  Function which should be called for all
 *                                  Enter snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetEnterCallback(
    OTF2_GlobalSnapReaderCallbacks*     globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_Enter enterCallback );


/** @brief Callback for the MpiSend snap record.
 *
 *  This record exists for each @eref{MpiSend} event where the matching
 *  receive message event did not occur on the remote location before
 *  the snapshot. This could either be an @eref{MpiRecv} or an
 *  @eref{MpiIrecv} event. Note that it may so, that a previous
 *  @eref{MpiIsend} with the same envelope than this one is neither
 *  completed not canceled yet, thus the matching receive may already
 *  occurred, but the matching couldn't be done yet.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param receiver      MPI rank of receiver in @a communicator.
 *  @param communicator  Communicator ID. References a @ref Comm definition and
 *                       will be mapped to the global definition if a mapping
 *                       table of type @ref OTF2_MAPPING_COMM is available.
 *  @param msgTag        Message tag
 *  @param msgLength     Message length
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MpiSend )( OTF2_LocationRef    locationID,
                                            OTF2_TimeStamp      snapTime,
                                            void*               userData,
                                            OTF2_AttributeList* attributeList,
                                            OTF2_TimeStamp      origEventTime,
                                            uint32_t            receiver,
                                            OTF2_CommRef        communicator,
                                            uint32_t            msgTag,
                                            uint64_t            msgLength );


/** @brief Registers the callback for the MpiSend snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param mpiSendCallback  Function which should be called for all
 *                                  MpiSend snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiSendCallback(
    OTF2_GlobalSnapReaderCallbacks*       globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiSend mpiSendCallback );


/** @brief Callback for the MpiIsend snap record.
 *
 *  This record exists for each @eref{MpiIsend} event where an
 *  corresponding @eref{MpiIsendComplete} or
 *  @eref{MpiRequestCancelled} event did not occur on this location
 *  before the snapshot. Or the corresponding @eref{MpiIsendComplete}
 *  did occurred (the @eref{MpiIsendCompleteSnap} record exists in the
 *  snapshot) but the matching receive message event did not occur on
 *  the remote location before the snapshot. (This could either be
 *  an@eref{MpiRecv} or an @eref{MpiIrecv} event.)
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param receiver      MPI rank of receiver in @a communicator.
 *  @param communicator  Communicator ID. References a @ref Comm definition and
 *                       will be mapped to the global definition if a mapping
 *                       table of type @ref OTF2_MAPPING_COMM is available.
 *  @param msgTag        Message tag
 *  @param msgLength     Message length
 *  @param requestID     ID of the related request
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MpiIsend )( OTF2_LocationRef    locationID,
                                             OTF2_TimeStamp      snapTime,
                                             void*               userData,
                                             OTF2_AttributeList* attributeList,
                                             OTF2_TimeStamp      origEventTime,
                                             uint32_t            receiver,
                                             OTF2_CommRef        communicator,
                                             uint32_t            msgTag,
                                             uint64_t            msgLength,
                                             uint64_t            requestID );


/** @brief Registers the callback for the MpiIsend snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param mpiIsendCallback  Function which should be called for all
 *                                  MpiIsend snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiIsendCallback(
    OTF2_GlobalSnapReaderCallbacks*        globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiIsend mpiIsendCallback );


/** @brief Callback for the MpiIsendComplete snap record.
 *
 *  This record exists for each @eref{MpiIsend} event where the
 *  corresponding @eref{MpiIsendComplete} event occurred, but where
 *  the matching receive message event did not occur on the remote
 *  location before the snapshot. (This could either be an
 *  @eref{MpiRecv} or an @eref{MpiIrecv} event.) .
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param requestID     ID of the related request
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MpiIsendComplete )( OTF2_LocationRef    locationID,
                                                     OTF2_TimeStamp      snapTime,
                                                     void*               userData,
                                                     OTF2_AttributeList* attributeList,
                                                     OTF2_TimeStamp      origEventTime,
                                                     uint64_t            requestID );


/** @brief Registers the callback for the MpiIsendComplete snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param mpiIsendCompleteCallback  Function which should be called for all
 *                                  MpiIsendComplete snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiIsendCompleteCallback(
    OTF2_GlobalSnapReaderCallbacks*                globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiIsendComplete mpiIsendCompleteCallback );


/** @brief Callback for the MpiRecv snap record.
 *
 *  This record exists for each @eref{MpiRecv} event where the matching
 *  send message event did not occur on the remote location before the
 *  snapshot. This could either be an @eref{MpiSend} or an
 *  @eref{MpiIsendComplete} event. Or an @eref{MpiIrecvRequest}
 *  occurred before this event but the corresponding @eref{MpiIrecv}
 *  event did not occurred before this snapshot. In this case the
 *  message matching couldn't performed yet, because the envelope of
 *  the ongoing @eref{MpiIrecvRequest} is not yet known.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param sender        MPI rank of sender in @a communicator.
 *  @param communicator  Communicator ID. References a @ref Comm definition and
 *                       will be mapped to the global definition if a mapping
 *                       table of type @ref OTF2_MAPPING_COMM is available.
 *  @param msgTag        Message tag
 *  @param msgLength     Message length
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MpiRecv )( OTF2_LocationRef    locationID,
                                            OTF2_TimeStamp      snapTime,
                                            void*               userData,
                                            OTF2_AttributeList* attributeList,
                                            OTF2_TimeStamp      origEventTime,
                                            uint32_t            sender,
                                            OTF2_CommRef        communicator,
                                            uint32_t            msgTag,
                                            uint64_t            msgLength );


/** @brief Registers the callback for the MpiRecv snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param mpiRecvCallback  Function which should be called for all
 *                                  MpiRecv snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiRecvCallback(
    OTF2_GlobalSnapReaderCallbacks*       globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiRecv mpiRecvCallback );


/** @brief Callback for the MpiIrecvRequest snap record.
 *
 *  This record exists for each @eref{MpiIrecvRequest} event where an
 *  corresponding @eref{MpiIrecv} or @eref{MpiRequestCancelled} event
 *  did not occur on this location before the snapshot. Or the
 *  corresponding @eref{MpiIrecv} did occurred (the
 *  @eref{MpiIrecvSnap} record exists in the snapshot) but the
 *  matching receive message event did not occur on the remote
 *  location before the snapshot. This could either be an
 *  @eref{MpiRecv} or an @eref{MpiIrecv} event.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param requestID     ID of the requested receive
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MpiIrecvRequest )( OTF2_LocationRef    locationID,
                                                    OTF2_TimeStamp      snapTime,
                                                    void*               userData,
                                                    OTF2_AttributeList* attributeList,
                                                    OTF2_TimeStamp      origEventTime,
                                                    uint64_t            requestID );


/** @brief Registers the callback for the MpiIrecvRequest snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param mpiIrecvRequestCallback  Function which should be called for all
 *                                  MpiIrecvRequest snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiIrecvRequestCallback(
    OTF2_GlobalSnapReaderCallbacks*               globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiIrecvRequest mpiIrecvRequestCallback );


/** @brief Callback for the MpiIrecv snap record.
 *
 *  This record exists for each @eref{MpiIrecv} event where the matching
 *  send message event did not occur on the remote location before the
 *  snapshot. This could either be an @eref{MpiSend} or an
 *  @eref{MpiIsendComplete} event. Or an @eref{MpiIrecvRequest}
 *  occurred before this event but the corresponding @eref{MpiIrecv}
 *  event did not occurred before this snapshot. In this case the
 *  message matching couldn't performed yet, because the envelope of
 *  the ongoing @eref{MpiIrecvRequest} is not yet known.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param sender        MPI rank of sender in @a communicator.
 *  @param communicator  Communicator ID. References a @ref Comm definition and
 *                       will be mapped to the global definition if a mapping
 *                       table of type @ref OTF2_MAPPING_COMM is available.
 *  @param msgTag        Message tag
 *  @param msgLength     Message length
 *  @param requestID     ID of the related request
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MpiIrecv )( OTF2_LocationRef    locationID,
                                             OTF2_TimeStamp      snapTime,
                                             void*               userData,
                                             OTF2_AttributeList* attributeList,
                                             OTF2_TimeStamp      origEventTime,
                                             uint32_t            sender,
                                             OTF2_CommRef        communicator,
                                             uint32_t            msgTag,
                                             uint64_t            msgLength,
                                             uint64_t            requestID );


/** @brief Registers the callback for the MpiIrecv snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param mpiIrecvCallback  Function which should be called for all
 *                                  MpiIrecv snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiIrecvCallback(
    OTF2_GlobalSnapReaderCallbacks*        globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiIrecv mpiIrecvCallback );


/** @brief Callback for the MpiCollectiveBegin snap record.
 *
 *  Indicates that this location started a collective operation but not
 *  all of the participating locations completed the operation yet,
 *  including this location.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MpiCollectiveBegin )( OTF2_LocationRef    locationID,
                                                       OTF2_TimeStamp      snapTime,
                                                       void*               userData,
                                                       OTF2_AttributeList* attributeList,
                                                       OTF2_TimeStamp      origEventTime );


/** @brief Registers the callback for the MpiCollectiveBegin snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param mpiCollectiveBeginCallback  Function which should be called for all
 *                                  MpiCollectiveBegin snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiCollectiveBeginCallback(
    OTF2_GlobalSnapReaderCallbacks*                  globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiCollectiveBegin mpiCollectiveBeginCallback );


/** @brief Callback for the MpiCollectiveEnd snap record.
 *
 *  Indicates that this location completed a collective operation localy
 *  but not all of the participating locations completed the operation
 *  yet. The corresponding @eref{MpiCollectiveBeginSnaps} record is
 *  still in the snapshot though.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param collectiveOp  Determines which collective operation it is.
 *  @param communicator  Communicator References a @ref Comm definition and will
 *                       be mapped to the global definition if a mapping
 *                       table of type @ref OTF2_MAPPING_COMM is available.
 *  @param root          MPI rank of root in @a communicator.
 *  @param sizeSent      Size of the sent message.
 *  @param sizeReceived  Size of the received message.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_MpiCollectiveEnd )( OTF2_LocationRef    locationID,
                                                     OTF2_TimeStamp      snapTime,
                                                     void*               userData,
                                                     OTF2_AttributeList* attributeList,
                                                     OTF2_TimeStamp      origEventTime,
                                                     OTF2_CollectiveOp   collectiveOp,
                                                     OTF2_CommRef        communicator,
                                                     uint32_t            root,
                                                     uint64_t            sizeSent,
                                                     uint64_t            sizeReceived );


/** @brief Registers the callback for the MpiCollectiveEnd snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param mpiCollectiveEndCallback  Function which should be called for all
 *                                  MpiCollectiveEnd snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMpiCollectiveEndCallback(
    OTF2_GlobalSnapReaderCallbacks*                globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_MpiCollectiveEnd mpiCollectiveEndCallback );


/** @brief Callback for the OmpFork snap record.
 *
 *  This record exists for each @eref{OmpFork} event where the
 *  corresponding @eref{OmpJoin} did not occurred before this
 *  snapshot.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime            The original time this event happended.
 *  @param numberOfRequestedThreads Requested size of the team.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_OmpFork )( OTF2_LocationRef    locationID,
                                            OTF2_TimeStamp      snapTime,
                                            void*               userData,
                                            OTF2_AttributeList* attributeList,
                                            OTF2_TimeStamp      origEventTime,
                                            uint32_t            numberOfRequestedThreads );


/** @brief Registers the callback for the OmpFork snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param ompForkCallback  Function which should be called for all
 *                                  OmpFork snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetOmpForkCallback(
    OTF2_GlobalSnapReaderCallbacks*       globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_OmpFork ompForkCallback );


/** @brief Callback for the OmpAcquireLock snap record.
 *
 *  This record exists for each @eref{OmpAcquireLock} event where the
 *  corresponding @eref{OmpReleaseLock} did not occurred before this
 *  snapshot yet.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime    The original time this event happended.
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
( *OTF2_GlobalSnapReaderCallback_OmpAcquireLock )( OTF2_LocationRef    locationID,
                                                   OTF2_TimeStamp      snapTime,
                                                   void*               userData,
                                                   OTF2_AttributeList* attributeList,
                                                   OTF2_TimeStamp      origEventTime,
                                                   uint32_t            lockID,
                                                   uint32_t            acquisitionOrder );


/** @brief Registers the callback for the OmpAcquireLock snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param ompAcquireLockCallback  Function which should be called for all
 *                                  OmpAcquireLock snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetOmpAcquireLockCallback(
    OTF2_GlobalSnapReaderCallbacks*              globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_OmpAcquireLock ompAcquireLockCallback );


/** @brief Callback for the OmpTaskCreate snap record.
 *
 *  This record exists for each @eref{OmpTaskCreate} event where the
 *  corresponding @eref{OmpTaskComplete} event did not occurred before
 *  this snapshot. Neither on this location nor on any other location
 *  in the current thread team.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param taskID        Identifier of the newly created task instance.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_OmpTaskCreate )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      snapTime,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  OTF2_TimeStamp      origEventTime,
                                                  uint64_t            taskID );


/** @brief Registers the callback for the OmpTaskCreate snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param ompTaskCreateCallback  Function which should be called for all
 *                                  OmpTaskCreate snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetOmpTaskCreateCallback(
    OTF2_GlobalSnapReaderCallbacks*             globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_OmpTaskCreate ompTaskCreateCallback );


/** @brief Callback for the OmpTaskSwitch snap record.
 *
 *  This record exists for each @eref{OmpTaskSwitch} event where the
 *  corresponding @eref{OmpTaskComplete} event did not occurred before
 *  this snapshot. Neither on this location nor on any other location
 *  in the current thread team.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param taskID        Identifier of the now active task instance.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_OmpTaskSwitch )( OTF2_LocationRef    locationID,
                                                  OTF2_TimeStamp      snapTime,
                                                  void*               userData,
                                                  OTF2_AttributeList* attributeList,
                                                  OTF2_TimeStamp      origEventTime,
                                                  uint64_t            taskID );


/** @brief Registers the callback for the OmpTaskSwitch snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param ompTaskSwitchCallback  Function which should be called for all
 *                                  OmpTaskSwitch snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetOmpTaskSwitchCallback(
    OTF2_GlobalSnapReaderCallbacks*             globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_OmpTaskSwitch ompTaskSwitchCallback );


/** @brief Callback for the Metric snap record.
 *
 *  This record exists for each referenced metric class or metric instance
 *  event this location recorded metrics before and provides the last
 *  known recorded metric values.
 *
 *  As an exception for metric classes where the metric mode detontes an
 *  @eref{OTF2_METRIC_VALUE_RELATIVE} mode the value indicates the
 *  accumulation of all previous metric values recorded.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime   The original time this event happended.
 *  @param metric          Could be a metric class or a metric instance.
 *                         References a @ref MetricClass, or a @ref
 *                         MetricInstance definition and will be mapped to
 *                         the global definition if a mapping table of type
 *                         @ref OTF2_MAPPING_METRIC is available.
 *  @param numberOfMetrics Number of metrics with in the set.
 *  @param typeIDs         List of metric types.
 *  @param metricValues    List of metric values.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_Metric )( OTF2_LocationRef        locationID,
                                           OTF2_TimeStamp          snapTime,
                                           void*                   userData,
                                           OTF2_AttributeList*     attributeList,
                                           OTF2_TimeStamp          origEventTime,
                                           OTF2_MetricRef          metric,
                                           uint8_t                 numberOfMetrics,
                                           const OTF2_Type*        typeIDs,
                                           const OTF2_MetricValue* metricValues );


/** @brief Registers the callback for the Metric snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param metricCallback  Function which should be called for all
 *                                  Metric snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetMetricCallback(
    OTF2_GlobalSnapReaderCallbacks*      globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_Metric metricCallback );


/** @brief Callback for the ParameterString snap record.
 *
 *  This record must be included in the snapshot until the leave event for
 *  the enter event occurs which has the greates timestamp less or
 *  equal the timestamp of this record.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param parameter     Parameter ID. References a @ref Parameter definition and
 *                       will be mapped to the global definition if a mapping
 *                       table of type @ref OTF2_MAPPING_PARAMETER is
 *                       available.
 *  @param string        Value: Handle of a string definition References a @ref
 *                       String definition and will be mapped to the global
 *                       definition if a mapping table of type @ref
 *                       OTF2_MAPPING_STRING is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_ParameterString )( OTF2_LocationRef    locationID,
                                                    OTF2_TimeStamp      snapTime,
                                                    void*               userData,
                                                    OTF2_AttributeList* attributeList,
                                                    OTF2_TimeStamp      origEventTime,
                                                    OTF2_ParameterRef   parameter,
                                                    OTF2_StringRef      string );


/** @brief Registers the callback for the ParameterString snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param parameterStringCallback  Function which should be called for all
 *                                  ParameterString snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetParameterStringCallback(
    OTF2_GlobalSnapReaderCallbacks*               globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_ParameterString parameterStringCallback );


/** @brief Callback for the ParameterInt snap record.
 *
 *  This record must be included in the snapshot until the leave event for
 *  the enter event occurs which has the greates timestamp less or
 *  equal the timestamp of this record.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param parameter     Parameter ID. References a @ref Parameter definition and
 *                       will be mapped to the global definition if a mapping
 *                       table of type @ref OTF2_MAPPING_PARAMETER is
 *                       available.
 *  @param value         Value of the recorded parameter.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_ParameterInt )( OTF2_LocationRef    locationID,
                                                 OTF2_TimeStamp      snapTime,
                                                 void*               userData,
                                                 OTF2_AttributeList* attributeList,
                                                 OTF2_TimeStamp      origEventTime,
                                                 OTF2_ParameterRef   parameter,
                                                 int64_t             value );


/** @brief Registers the callback for the ParameterInt snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param parameterIntCallback  Function which should be called for all
 *                                  ParameterInt snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetParameterIntCallback(
    OTF2_GlobalSnapReaderCallbacks*            globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_ParameterInt parameterIntCallback );


/** @brief Callback for the ParameterUnsignedInt snap record.
 *
 *  This record must be included in the snapshot until the leave event for
 *  the enter event occurs which has the greates timestamp less or
 *  equal the timestamp of this record.
 *
 *  @param locationID    The location where this snap happened.
 *  @param time          The time of this snapshot.
 *  @param userData      User data as set by
 *                       @ref OTF2_Reader_RegisterGlobalSnapCallbacks or
 *                       @ref OTF2_GlobalSnapReader_SetCallbacks.
 *  @param attributeList Additional attributes for this snap.
 *
 *  @param origEventTime The original time this event happended.
 *  @param parameter     Parameter ID. References a @ref Parameter definition and
 *                       will be mapped to the global definition if a mapping
 *                       table of type @ref OTF2_MAPPING_PARAMETER is
 *                       available.
 *  @param value         Value of the recorded parameter.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_CALLBACK_SUCCESS} or @eref{OTF2_CALLBACK_INTERRUPT}.
 */
typedef OTF2_CallbackCode
( *OTF2_GlobalSnapReaderCallback_ParameterUnsignedInt )( OTF2_LocationRef    locationID,
                                                         OTF2_TimeStamp      snapTime,
                                                         void*               userData,
                                                         OTF2_AttributeList* attributeList,
                                                         OTF2_TimeStamp      origEventTime,
                                                         OTF2_ParameterRef   parameter,
                                                         uint64_t            value );


/** @brief Registers the callback for the ParameterUnsignedInt snap.
 *
 *  @param globalSnapReaderCallbacks Struct for all callbacks.
 *  @param parameterUnsignedIntCallback  Function which should be called for all
 *                                  ParameterUnsignedInt snaps.
 *
 *  @since Version 1.2

 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INVALID_ARGUMENT,
 *             for an invalid @p defReaderCallbacks argument}
 *  @retend
 */
OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetParameterUnsignedIntCallback(
    OTF2_GlobalSnapReaderCallbacks*                    globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_ParameterUnsignedInt parameterUnsignedIntCallback );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GLOBAL_SNAP_READER_CALLBACKS_H */
