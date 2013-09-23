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


#ifndef OTF2_SNAP_WRITER_H
#define OTF2_SNAP_WRITER_H


/**
 *  @file       include/otf2/OTF2_SnapWriter.h
 *  @source     templates/OTF2_SnapWriter.tmpl.h
 *
 *  @brief      This lowest user-visible layer provides write routines to
 *              write snapshot records for a single location.
 */



#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Events.h>
#include <otf2/OTF2_AttributeList.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Keeps all necessary information about the snap writer.
 *  See OTF2_SnapWriter_struct for detailed information.
 *
 *  @since Version 1.2
 *  */
typedef struct OTF2_SnapWriter_struct OTF2_SnapWriter;


/** @brief
 *  Function to get the location ID of a snap writer object.
 *
 *  @param writer       Snap writer object of interest
 *  @param locationID   Pointer to a variable where the ID is returned in
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_GetLocationID( const OTF2_SnapWriter* writer,
                               OTF2_LocationRef*      locationID );



/** @brief Records an SnapshotStart snapshot record.
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
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param numberOfRecords Number of snapshot event records in this snapshot.
 *                         Excluding the @eref{SnapshotEnd} record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_SnapshotStart( OTF2_SnapWriter*    writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      snapTime,
                               uint64_t            numberOfRecords );


/** @brief Records an SnapshotEnd snapshot record.
 *
 *  This record marks the end of a snapshot. It contains the position to
 *  continue reading in the event trace for this location. Use @ref
 *  OTF2_EvtReader_Seek with @a contReadPos as the position.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param contReadPos Position to continue reading in the event trace.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_SnapshotEnd( OTF2_SnapWriter*    writer,
                             OTF2_AttributeList* attributeList,
                             OTF2_TimeStamp      snapTime,
                             uint64_t            contReadPos );


/** @brief Records an MeasurementOnOff snapshot record.
 *
 *  The last occurrence of an @eref{MeasurementOnOff} event of this
 *  location, if any.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param origEventTime   The original time this event happended.
 *  @param measurementMode Is the measurement turned on
 *                         (@eref{OTF2_MEASUREMENT_ON}) or off
 *                         (@eref{OTF2_MEASUREMENT_OFF})?
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MeasurementOnOff( OTF2_SnapWriter*     writer,
                                  OTF2_AttributeList*  attributeList,
                                  OTF2_TimeStamp       snapTime,
                                  OTF2_TimeStamp       origEventTime,
                                  OTF2_MeasurementMode measurementMode );


/** @brief Records an Enter snapshot record.
 *
 *  This record exists for each @eref{Enter} event where the corresponding
 *  @eref{Leave} event did not occur before the snapshot.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param origEventTime The original time this event happended.
 *  @param region        Needs to be defined in a definition record References a
 *                       @ref Region definition and will be mapped to the
 *                       global definition if a mapping table of type @ref
 *                       OTF2_MAPPING_REGION is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_Enter( OTF2_SnapWriter*    writer,
                       OTF2_AttributeList* attributeList,
                       OTF2_TimeStamp      snapTime,
                       OTF2_TimeStamp      origEventTime,
                       OTF2_RegionRef      region );


/** @brief Records an MpiSend snapshot record.
 *
 *  This record exists for each @eref{MpiSend} event where the matching
 *  receive message event did not occur on the remote location before
 *  the snapshot. This could either be an @eref{MpiRecv} or an
 *  @eref{MpiIrecv} event. Note that it may so, that a previous
 *  @eref{MpiIsend} with the same envelope than this one is neither
 *  completed not canceled yet, thus the matching receive may already
 *  occurred, but the matching couldn't be done yet.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MpiSend( OTF2_SnapWriter*    writer,
                         OTF2_AttributeList* attributeList,
                         OTF2_TimeStamp      snapTime,
                         OTF2_TimeStamp      origEventTime,
                         uint32_t            receiver,
                         OTF2_CommRef        communicator,
                         uint32_t            msgTag,
                         uint64_t            msgLength );


/** @brief Records an MpiIsend snapshot record.
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
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MpiIsend( OTF2_SnapWriter*    writer,
                          OTF2_AttributeList* attributeList,
                          OTF2_TimeStamp      snapTime,
                          OTF2_TimeStamp      origEventTime,
                          uint32_t            receiver,
                          OTF2_CommRef        communicator,
                          uint32_t            msgTag,
                          uint64_t            msgLength,
                          uint64_t            requestID );


/** @brief Records an MpiIsendComplete snapshot record.
 *
 *  This record exists for each @eref{MpiIsend} event where the
 *  corresponding @eref{MpiIsendComplete} event occurred, but where
 *  the matching receive message event did not occur on the remote
 *  location before the snapshot. (This could either be an
 *  @eref{MpiRecv} or an @eref{MpiIrecv} event.) .
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param origEventTime The original time this event happended.
 *  @param requestID     ID of the related request
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MpiIsendComplete( OTF2_SnapWriter*    writer,
                                  OTF2_AttributeList* attributeList,
                                  OTF2_TimeStamp      snapTime,
                                  OTF2_TimeStamp      origEventTime,
                                  uint64_t            requestID );


/** @brief Records an MpiRecv snapshot record.
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
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MpiRecv( OTF2_SnapWriter*    writer,
                         OTF2_AttributeList* attributeList,
                         OTF2_TimeStamp      snapTime,
                         OTF2_TimeStamp      origEventTime,
                         uint32_t            sender,
                         OTF2_CommRef        communicator,
                         uint32_t            msgTag,
                         uint64_t            msgLength );


/** @brief Records an MpiIrecvRequest snapshot record.
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
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param origEventTime The original time this event happended.
 *  @param requestID     ID of the requested receive
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MpiIrecvRequest( OTF2_SnapWriter*    writer,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      snapTime,
                                 OTF2_TimeStamp      origEventTime,
                                 uint64_t            requestID );


/** @brief Records an MpiIrecv snapshot record.
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
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MpiIrecv( OTF2_SnapWriter*    writer,
                          OTF2_AttributeList* attributeList,
                          OTF2_TimeStamp      snapTime,
                          OTF2_TimeStamp      origEventTime,
                          uint32_t            sender,
                          OTF2_CommRef        communicator,
                          uint32_t            msgTag,
                          uint64_t            msgLength,
                          uint64_t            requestID );


/** @brief Records an MpiCollectiveBegin snapshot record.
 *
 *  Indicates that this location started a collective operation but not
 *  all of the participating locations completed the operation yet,
 *  including this location.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param origEventTime The original time this event happended.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MpiCollectiveBegin( OTF2_SnapWriter*    writer,
                                    OTF2_AttributeList* attributeList,
                                    OTF2_TimeStamp      snapTime,
                                    OTF2_TimeStamp      origEventTime );


/** @brief Records an MpiCollectiveEnd snapshot record.
 *
 *  Indicates that this location completed a collective operation localy
 *  but not all of the participating locations completed the operation
 *  yet. The corresponding @eref{MpiCollectiveBeginSnaps} record is
 *  still in the snapshot though.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_MpiCollectiveEnd( OTF2_SnapWriter*    writer,
                                  OTF2_AttributeList* attributeList,
                                  OTF2_TimeStamp      snapTime,
                                  OTF2_TimeStamp      origEventTime,
                                  OTF2_CollectiveOp   collectiveOp,
                                  OTF2_CommRef        communicator,
                                  uint32_t            root,
                                  uint64_t            sizeSent,
                                  uint64_t            sizeReceived );


/** @brief Records an OmpFork snapshot record.
 *
 *  This record exists for each @eref{OmpFork} event where the
 *  corresponding @eref{OmpJoin} did not occurred before this
 *  snapshot.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param origEventTime            The original time this event happended.
 *  @param numberOfRequestedThreads Requested size of the team.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_OmpFork( OTF2_SnapWriter*    writer,
                         OTF2_AttributeList* attributeList,
                         OTF2_TimeStamp      snapTime,
                         OTF2_TimeStamp      origEventTime,
                         uint32_t            numberOfRequestedThreads );


/** @brief Records an OmpAcquireLock snapshot record.
 *
 *  This record exists for each @eref{OmpAcquireLock} event where the
 *  corresponding @eref{OmpReleaseLock} did not occurred before this
 *  snapshot yet.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_OmpAcquireLock( OTF2_SnapWriter*    writer,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      snapTime,
                                OTF2_TimeStamp      origEventTime,
                                uint32_t            lockID,
                                uint32_t            acquisitionOrder );


/** @brief Records an OmpTaskCreate snapshot record.
 *
 *  This record exists for each @eref{OmpTaskCreate} event where the
 *  corresponding @eref{OmpTaskComplete} event did not occurred before
 *  this snapshot. Neither on this location nor on any other location
 *  in the current thread team.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param origEventTime The original time this event happended.
 *  @param taskID        Identifier of the newly created task instance.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_OmpTaskCreate( OTF2_SnapWriter*    writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      snapTime,
                               OTF2_TimeStamp      origEventTime,
                               uint64_t            taskID );


/** @brief Records an OmpTaskSwitch snapshot record.
 *
 *  This record exists for each @eref{OmpTaskSwitch} event where the
 *  corresponding @eref{OmpTaskComplete} event did not occurred before
 *  this snapshot. Neither on this location nor on any other location
 *  in the current thread team.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @param origEventTime The original time this event happended.
 *  @param taskID        Identifier of the now active task instance.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_OmpTaskSwitch( OTF2_SnapWriter*    writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      snapTime,
                               OTF2_TimeStamp      origEventTime,
                               uint64_t            taskID );


/** @brief Records an Metric snapshot record.
 *
 *  This record exists for each referenced metric class or metric instance
 *  event this location recorded metrics before and provides the last
 *  known recorded metric values.
 *
 *  As an exception for metric classes where the metric mode detontes an
 *  @eref{OTF2_METRIC_VALUE_RELATIVE} mode the value indicates the
 *  accumulation of all previous metric values recorded.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_Metric( OTF2_SnapWriter*        writer,
                        OTF2_AttributeList*     attributeList,
                        OTF2_TimeStamp          snapTime,
                        OTF2_TimeStamp          origEventTime,
                        OTF2_MetricRef          metric,
                        uint8_t                 numberOfMetrics,
                        const OTF2_Type*        typeIDs,
                        const OTF2_MetricValue* metricValues );


/** @brief Records an ParameterString snapshot record.
 *
 *  This record must be included in the snapshot until the leave event for
 *  the enter event occurs which has the greates timestamp less or
 *  equal the timestamp of this record.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_ParameterString( OTF2_SnapWriter*    writer,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      snapTime,
                                 OTF2_TimeStamp      origEventTime,
                                 OTF2_ParameterRef   parameter,
                                 OTF2_StringRef      string );


/** @brief Records an ParameterInt snapshot record.
 *
 *  This record must be included in the snapshot until the leave event for
 *  the enter event occurs which has the greates timestamp less or
 *  equal the timestamp of this record.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_ParameterInt( OTF2_SnapWriter*    writer,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      snapTime,
                              OTF2_TimeStamp      origEventTime,
                              OTF2_ParameterRef   parameter,
                              int64_t             value );


/** @brief Records an ParameterUnsignedInt snapshot record.
 *
 *  This record must be included in the snapshot until the leave event for
 *  the enter event occurs which has the greates timestamp less or
 *  equal the timestamp of this record.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_ParameterUnsignedInt( OTF2_SnapWriter*    writer,
                                      OTF2_AttributeList* attributeList,
                                      OTF2_TimeStamp      snapTime,
                                      OTF2_TimeStamp      origEventTime,
                                      OTF2_ParameterRef   parameter,
                                      uint64_t            value );



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_SNAP_WRITER_H */
