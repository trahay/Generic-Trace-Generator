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


/** @brief Records an BufferFlush event.
 *
 *  This event signals that the internal buffer was flushed at the given
 *  time.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param stopTime The time the buffer flush finished.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_BufferFlush( OTF2_EvtWriter*     writer,
                            OTF2_AttributeList* attributeList,
                            OTF2_TimeStamp      time,
                            OTF2_TimeStamp      stopTime );


/** @brief Records an MeasurementOnOff event.
 *
 *  This event signals where the measurement system turned measurement on
 *  or off.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param measurementMode Is the measurement turned on
 *                         (@eref{OTF2_MEASUREMENT_ON}) or off
 *                         (@eref{OTF2_MEASUREMENT_OFF})?
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MeasurementOnOff( OTF2_EvtWriter*      writer,
                                 OTF2_AttributeList*  attributeList,
                                 OTF2_TimeStamp       time,
                                 OTF2_MeasurementMode measurementMode );


/** @brief Records an Enter event.
 *
 *  An enter record indicates that the program enters a code region.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param region Needs to be defined in a definition record References a @ref
 *                Region definition and will be mapped to the global
 *                definition if a mapping table of type @ref
 *                OTF2_MAPPING_REGION is available.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_Enter( OTF2_EvtWriter*     writer,
                      OTF2_AttributeList* attributeList,
                      OTF2_TimeStamp      time,
                      OTF2_RegionRef      region );


/** @brief Records an Leave event.
 *
 *  A leave record indicates that the program leaves a code region.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param region Needs to be defined in a definition record References a @ref
 *                Region definition and will be mapped to the global
 *                definition if a mapping table of type @ref
 *                OTF2_MAPPING_REGION is available.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_Leave( OTF2_EvtWriter*     writer,
                      OTF2_AttributeList* attributeList,
                      OTF2_TimeStamp      time,
                      OTF2_RegionRef      region );


/** @brief Records an MpiSend event.
 *
 *  A MpiSend record indicates that a MPI message send process was
 *  initiated (MPI_SEND). It keeps the necessary information for this
 *  event: receiver of the message, communicator, and the message tag.
 *  You can optionally add further information like the message length
 *  (size of the send buffer).
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiSend( OTF2_EvtWriter*     writer,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time,
                        uint32_t            receiver,
                        OTF2_CommRef        communicator,
                        uint32_t            msgTag,
                        uint64_t            msgLength );


/** @brief Records an MpiIsend event.
 *
 *  A MpiIsend record indicates that a MPI message send process was
 *  initiated (MPI_ISEND). It keeps the necessary information for this
 *  event: receiver of the message, communicator, and the message tag.
 *  You can optionally add further information like the message length
 *  (size of the send buffer).
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiIsend( OTF2_EvtWriter*     writer,
                         OTF2_AttributeList* attributeList,
                         OTF2_TimeStamp      time,
                         uint32_t            receiver,
                         OTF2_CommRef        communicator,
                         uint32_t            msgTag,
                         uint64_t            msgLength,
                         uint64_t            requestID );


/** @brief Records an MpiIsendComplete event.
 *
 *  Signals the completion of non-blocking send request.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param requestID ID of the related request
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiIsendComplete( OTF2_EvtWriter*     writer,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 uint64_t            requestID );


/** @brief Records an MpiIrecvRequest event.
 *
 *  Signals the request of an receive, which can be completed later.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param requestID ID of the requested receive
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiIrecvRequest( OTF2_EvtWriter*     writer,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      time,
                                uint64_t            requestID );


/** @brief Records an MpiRecv event.
 *
 *  A MpiRecv record indicates that a MPI message was received (MPI_RECV).
 *  It keeps the necessary information for this event: sender of the
 *  message, communicator, and the message tag. You can optionally add
 *  further information like the message length (size of the receive
 *  buffer).
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiRecv( OTF2_EvtWriter*     writer,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time,
                        uint32_t            sender,
                        OTF2_CommRef        communicator,
                        uint32_t            msgTag,
                        uint64_t            msgLength );


/** @brief Records an MpiIrecv event.
 *
 *  A MpiIrecv record indicates that a MPI message was received
 *  (MPI_IRECV). It keeps the necessary information for this event:
 *  sender of the message, communicator, and the message tag. You can
 *  optionally add further information like the message length (size
 *  of the receive buffer).
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiIrecv( OTF2_EvtWriter*     writer,
                         OTF2_AttributeList* attributeList,
                         OTF2_TimeStamp      time,
                         uint32_t            sender,
                         OTF2_CommRef        communicator,
                         uint32_t            msgTag,
                         uint64_t            msgLength,
                         uint64_t            requestID );


/** @brief Records an MpiRequestTest event.
 *
 *  This events appears if the program tests if a request has already
 *  completed but the test failed.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param requestID ID of the related request
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiRequestTest( OTF2_EvtWriter*     writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               uint64_t            requestID );


/** @brief Records an MpiRequestCancelled event.
 *
 *  This events appears if the program canceled a request.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param requestID ID of the related request
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiRequestCancelled( OTF2_EvtWriter*     writer,
                                    OTF2_AttributeList* attributeList,
                                    OTF2_TimeStamp      time,
                                    uint64_t            requestID );


/** @brief Records an MpiCollectiveBegin event.
 *
 *  A MpiCollectiveBegin record marks the begin of an MPI collective
 *  operation (MPI_GATHER, MPI_SCATTER etc.).
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiCollectiveBegin( OTF2_EvtWriter*     writer,
                                   OTF2_AttributeList* attributeList,
                                   OTF2_TimeStamp      time  );


/** @brief Records an MpiCollectiveEnd event.
 *
 *  A MpiCollectiveEnd record marks the end of an MPI collective operation
 *  (MPI_GATHER, MPI_SCATTER etc.). It keeps the necessary information
 *  for this event: type of collective operation, communicator, the
 *  root of this collective operation. You can optionally add further
 *  information like sent and received bytes.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_MpiCollectiveEnd( OTF2_EvtWriter*     writer,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 OTF2_CollectiveOp   collectiveOp,
                                 OTF2_CommRef        communicator,
                                 uint32_t            root,
                                 uint64_t            sizeSent,
                                 uint64_t            sizeReceived );


/** @brief Records an OmpFork event.
 *
 *  An OmpFork record marks that an OpenMP Thread forks a thread team.
 *
 *  This event record is superseded by the @eref{ThreadFork} event record
 *  and should not be used when the @eref{ThreadFork} event record is
 *  in use.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param numberOfRequestedThreads Requested size of the team.
 *
 *  @since Version 1.0
 *
 *  @deprecated In version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_OmpFork( OTF2_EvtWriter*     writer,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time,
                        uint32_t            numberOfRequestedThreads );


/** @brief Records an OmpJoin event.
 *
 *  An OmpJoin record marks that a team of threads is joint and only the
 *  master thread continues execution.
 *
 *  This event record is superseded by the @eref{ThreadJoin} event record
 *  and should not be used when the @eref{ThreadJoin} event record is
 *  in use.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *
 *
 *  @since Version 1.0
 *
 *  @deprecated In version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_OmpJoin( OTF2_EvtWriter*     writer,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time  );


/** @brief Records an OmpAcquireLock event.
 *
 *  An OmpAcquireLock record marks that a thread acquires an OpenMP lock.
 *
 *  This event record is superseded by the @eref{ThreadAcquireLock} event
 *  record and should not be used when the @eref{ThreadAcquireLock}
 *  event record is in use record.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @deprecated In version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_OmpAcquireLock( OTF2_EvtWriter*     writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               uint32_t            lockID,
                               uint32_t            acquisitionOrder );


/** @brief Records an OmpReleaseLock event.
 *
 *  An OmpReleaseLock record marks that a thread releases an OpenMP lock.
 *
 *  This event record is superseded by the @eref{ThreadReleaseLock} event
 *  record and should not be used when the @eref{ThreadReleaseLock}
 *  event record is in use.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @deprecated In version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_OmpReleaseLock( OTF2_EvtWriter*     writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               uint32_t            lockID,
                               uint32_t            acquisitionOrder );


/** @brief Records an OmpTaskCreate event.
 *
 *  An OmpTaskCreate record marks that an OpenMP Task was/will be created
 *  in the current region.
 *
 *  This event record is superseded by the @eref{ThreadTaskCreate} event
 *  record and should not be used when the @eref{ThreadTaskCreate}
 *  event record is in use.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param taskID Identifier of the newly created task instance.
 *
 *  @since Version 1.0
 *
 *  @deprecated In version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_OmpTaskCreate( OTF2_EvtWriter*     writer,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              uint64_t            taskID );


/** @brief Records an OmpTaskSwitch event.
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
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param taskID Identifier of the now active task instance.
 *
 *  @since Version 1.0
 *
 *  @deprecated In version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_OmpTaskSwitch( OTF2_EvtWriter*     writer,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              uint64_t            taskID );


/** @brief Records an OmpTaskComplete event.
 *
 *  An OmpTaskComplete record indicates that the execution of an OpenMP
 *  task has finished.
 *
 *  This event record is superseded by the @eref{ThreadTaskComplete} event
 *  record and should not be used when the @eref{ThreadTaskComplete}
 *  event record is in use.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param taskID Identifier of the completed task instance.
 *
 *  @since Version 1.0
 *
 *  @deprecated In version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_OmpTaskComplete( OTF2_EvtWriter*     writer,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      time,
                                uint64_t            taskID );


/** @brief Records an Metric event.
 *
 *  A metric event is always stored at the location that recorded the
 *  metric. A metric event can reference a metric class or metric
 *  instance. Therefore, metric classes and instances share same ID
 *  space. Synchronous metrics are always located right before the
 *  according enter and leave event.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_Metric( OTF2_EvtWriter*         writer,
                       OTF2_AttributeList*     attributeList,
                       OTF2_TimeStamp          time,
                       OTF2_MetricRef          metric,
                       uint8_t                 numberOfMetrics,
                       const OTF2_Type*        typeIDs,
                       const OTF2_MetricValue* metricValues );


/** @brief Records an ParameterString event.
 *
 *  A ParameterString record marks that in the current region, the
 *  specified string parameter has the specified value.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ParameterString( OTF2_EvtWriter*     writer,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      time,
                                OTF2_ParameterRef   parameter,
                                OTF2_StringRef      string );


/** @brief Records an ParameterInt event.
 *
 *  A ParameterInt record marks that in the current region, the specified
 *  integer parameter has the specified value.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param parameter Parameter ID. References a @ref Parameter definition and
 *                   will be mapped to the global definition if a mapping
 *                   table of type @ref OTF2_MAPPING_PARAMETER is available.
 *  @param value     Value of the recorded parameter.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ParameterInt( OTF2_EvtWriter*     writer,
                             OTF2_AttributeList* attributeList,
                             OTF2_TimeStamp      time,
                             OTF2_ParameterRef   parameter,
                             int64_t             value );


/** @brief Records an ParameterUnsignedInt event.
 *
 *  A ParameterUnsignedInt record marks that in the current region, the
 *  specified unsigned integer parameter has the specified value.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param parameter Parameter ID. References a @ref Parameter definition and
 *                   will be mapped to the global definition if a mapping
 *                   table of type @ref OTF2_MAPPING_PARAMETER is available.
 *  @param value     Value of the recorded parameter.
 *
 *  @since Version 1.0
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ParameterUnsignedInt( OTF2_EvtWriter*     writer,
                                     OTF2_AttributeList* attributeList,
                                     OTF2_TimeStamp      time,
                                     OTF2_ParameterRef   parameter,
                                     uint64_t            value );


/** @brief Records an RmaWinCreate event.
 *
 *  An RmaWinCreate record denotes the creation of an RMA window.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param win ID of the window created. References a @ref RmaWin definition and
 *             will be mapped to the global definition if a mapping table of
 *             type @ref OTF2_MAPPING_RMA_WIN is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaWinCreate( OTF2_EvtWriter*     writer,
                             OTF2_AttributeList* attributeList,
                             OTF2_TimeStamp      time,
                             OTF2_RmaWinRef      win );


/** @brief Records an RmaWinDestroy event.
 *
 *  An RmaWinDestroy record denotes the destruction of an RMA window.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param win ID of the window destructed. References a @ref RmaWin definition
 *             and will be mapped to the global definition if a mapping table
 *             of type @ref OTF2_MAPPING_RMA_WIN is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaWinDestroy( OTF2_EvtWriter*     writer,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              OTF2_RmaWinRef      win );


/** @brief Records an RmaCollectiveBegin event.
 *
 *  An RmaCollectiveBegin record denotes the beginnig of a collective RMA
 *  operation.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaCollectiveBegin( OTF2_EvtWriter*     writer,
                                   OTF2_AttributeList* attributeList,
                                   OTF2_TimeStamp      time  );


/** @brief Records an RmaCollectiveEnd event.
 *
 *  "An RmaCollectiveEnd record denotes the end of a collective RMA
 *  operation.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaCollectiveEnd( OTF2_EvtWriter*     writer,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 OTF2_CollectiveOp   collectiveOp,
                                 OTF2_RmaSyncLevel   syncLevel,
                                 OTF2_RmaWinRef      win,
                                 uint32_t            root,
                                 uint64_t            bytesSent,
                                 uint64_t            bytesReceived );


/** @brief Records an RmaGroupSync event.
 *
 *  An RmaGroupSync record denotes the synchronization with a subgroup of
 *  processes on a window.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaGroupSync( OTF2_EvtWriter*     writer,
                             OTF2_AttributeList* attributeList,
                             OTF2_TimeStamp      time,
                             OTF2_RmaSyncLevel   syncLevel,
                             OTF2_RmaWinRef      win,
                             OTF2_GroupRef       group );


/** @brief Records an RmaRequestLock event.
 *
 *  An RmaRequestLock record denotes the time a lock was requested and
 *  with it the earliest time it could have been granted.  It is used
 *  to mark (possibly) non-blocking lock request, as defined by the
 *  MPI standard.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaRequestLock( OTF2_EvtWriter*     writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               OTF2_RmaWinRef      win,
                               uint32_t            remote,
                               uint64_t            lockId,
                               OTF2_LockType       lockType );


/** @brief Records an RmaAcquireLock event.
 *
 *  An RmaAcquireLock record denotes the time a lock was aquired by the
 *  process.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaAcquireLock( OTF2_EvtWriter*     writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               OTF2_RmaWinRef      win,
                               uint32_t            remote,
                               uint64_t            lockId,
                               OTF2_LockType       lockType );


/** @brief Records an RmaTryLock event.
 *
 *  An RmaTryLock record denotes the time of an unsuccessful attempt to
 *  acquire the lock.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaTryLock( OTF2_EvtWriter*     writer,
                           OTF2_AttributeList* attributeList,
                           OTF2_TimeStamp      time,
                           OTF2_RmaWinRef      win,
                           uint32_t            remote,
                           uint64_t            lockId,
                           OTF2_LockType       lockType );


/** @brief Records an RmaReleaseLock event.
 *
 *  An RmaReleaseLock record denotes the time the lock was released.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaReleaseLock( OTF2_EvtWriter*     writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      time,
                               OTF2_RmaWinRef      win,
                               uint32_t            remote,
                               uint64_t            lockId );


/** @brief Records an RmaSync event.
 *
 *  An RmaSync record denotes the direct synchronization with a possibly
 *  remote process.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaSync( OTF2_EvtWriter*     writer,
                        OTF2_AttributeList* attributeList,
                        OTF2_TimeStamp      time,
                        OTF2_RmaWinRef      win,
                        uint32_t            remote,
                        OTF2_RmaSyncType    syncType );


/** @brief Records an RmaWaitChange event.
 *
 *  An RmaWaitChange record denotes the change of a window that was waited
 *  for.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param win ID of the window used for this operation. References a @ref RmaWin
 *             definition and will be mapped to the global definition if a
 *             mapping table of type @ref OTF2_MAPPING_RMA_WIN is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaWaitChange( OTF2_EvtWriter*     writer,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              OTF2_RmaWinRef      win );


/** @brief Records an RmaPut event.
 *
 *  An RmaPut record denotes the time a put operation was issued.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaPut( OTF2_EvtWriter*     writer,
                       OTF2_AttributeList* attributeList,
                       OTF2_TimeStamp      time,
                       OTF2_RmaWinRef      win,
                       uint32_t            remote,
                       uint64_t            bytes,
                       uint64_t            matchingId );


/** @brief Records an RmaGet event.
 *
 *  An RmaGet record denotes the time a put operation was issued.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaGet( OTF2_EvtWriter*     writer,
                       OTF2_AttributeList* attributeList,
                       OTF2_TimeStamp      time,
                       OTF2_RmaWinRef      win,
                       uint32_t            remote,
                       uint64_t            bytes,
                       uint64_t            matchingId );


/** @brief Records an RmaAtomic event.
 *
 *  An RmaAtomic record denotes the time a atomic operation was issued.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaAtomic( OTF2_EvtWriter*     writer,
                          OTF2_AttributeList* attributeList,
                          OTF2_TimeStamp      time,
                          OTF2_RmaWinRef      win,
                          uint32_t            remote,
                          OTF2_RmaAtomicType  type,
                          uint64_t            bytesSent,
                          uint64_t            bytesReceived,
                          uint64_t            matchingId );


/** @brief Records an RmaOpCompleteBlocking event.
 *
 *  An RmaOpCompleteBlocking record denotes the local completion of a
 *  blocking RMA operation.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaOpCompleteBlocking( OTF2_EvtWriter*     writer,
                                      OTF2_AttributeList* attributeList,
                                      OTF2_TimeStamp      time,
                                      OTF2_RmaWinRef      win,
                                      uint64_t            matchingId );


/** @brief Records an RmaOpCompleteNonBlocking event.
 *
 *  An RmaOpCompleteNonBlocking record denotes the local completion of a
 *  non-blocking RMA operation.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaOpCompleteNonBlocking( OTF2_EvtWriter*     writer,
                                         OTF2_AttributeList* attributeList,
                                         OTF2_TimeStamp      time,
                                         OTF2_RmaWinRef      win,
                                         uint64_t            matchingId );


/** @brief Records an RmaOpTest event.
 *
 *  An RmaOpTest record denotes that a non-blocking RMA operation has been
 *  tested for completion unsuccessfully.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaOpTest( OTF2_EvtWriter*     writer,
                          OTF2_AttributeList* attributeList,
                          OTF2_TimeStamp      time,
                          OTF2_RmaWinRef      win,
                          uint64_t            matchingId );


/** @brief Records an RmaOpCompleteRemote event.
 *
 *  An RmaOpCompleteRemote record denotes the local completion of an RMA
 *  operation.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param win        ID of the window used for this operation. References a @ref
 *                    RmaWin definition and will be mapped to the global
 *                    definition if a mapping table of type @ref
 *                    OTF2_MAPPING_RMA_WIN is available.
 *  @param matchingId ID used for matching the appropriate completion record.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_RmaOpCompleteRemote( OTF2_EvtWriter*     writer,
                                    OTF2_AttributeList* attributeList,
                                    OTF2_TimeStamp      time,
                                    OTF2_RmaWinRef      win,
                                    uint64_t            matchingId );


/** @brief Records an ThreadFork event.
 *
 *  An ThreadFork record marks that an thread forks a thread team.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *
 *  @param numberOfRequestedThreads Requested size of the team.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadFork( OTF2_EvtWriter*     writer,
                           OTF2_AttributeList* attributeList,
                           OTF2_TimeStamp      time,
                           OTF2_Paradigm       model,
                           uint32_t            numberOfRequestedThreads );


/** @brief Records an ThreadJoin event.
 *
 *  An ThreadJoin record marks that a team of threads is joint and only
 *  the master thread continues execution.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadJoin( OTF2_EvtWriter*     writer,
                           OTF2_AttributeList* attributeList,
                           OTF2_TimeStamp      time,
                           OTF2_Paradigm       model );


/** @brief Records an ThreadTeamBegin event.
 *
 *
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param threadTeam Thread team References a @ref Comm definition and will be
 *                    mapped to the global definition if a mapping table of
 *                    type @ref OTF2_MAPPING_COMM is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadTeamBegin( OTF2_EvtWriter*     writer,
                                OTF2_AttributeList* attributeList,
                                OTF2_TimeStamp      time,
                                OTF2_CommRef        threadTeam );


/** @brief Records an ThreadTeamEnd event.
 *
 *
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
 *
 *  @param threadTeam Thread team References a @ref Comm definition and will be
 *                    mapped to the global definition if a mapping table of
 *                    type @ref OTF2_MAPPING_COMM is available.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadTeamEnd( OTF2_EvtWriter*     writer,
                              OTF2_AttributeList* attributeList,
                              OTF2_TimeStamp      time,
                              OTF2_CommRef        threadTeam );


/** @brief Records an ThreadAcquireLock event.
 *
 *  An ThreadAcquireLock record marks that a thread acquires an lock.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadAcquireLock( OTF2_EvtWriter*     writer,
                                  OTF2_AttributeList* attributeList,
                                  OTF2_TimeStamp      time,
                                  OTF2_Paradigm       model,
                                  uint32_t            lockID,
                                  uint32_t            acquisitionOrder );


/** @brief Records an ThreadReleaseLock event.
 *
 *  An ThreadReleaseLock record marks that a thread releases an lock.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadReleaseLock( OTF2_EvtWriter*     writer,
                                  OTF2_AttributeList* attributeList,
                                  OTF2_TimeStamp      time,
                                  OTF2_Paradigm       model,
                                  uint32_t            lockID,
                                  uint32_t            acquisitionOrder );


/** @brief Records an ThreadTaskCreate event.
 *
 *  An ThreadTaskCreate record marks that an task in was/will be created
 *  and will be processed by the specified thread team.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadTaskCreate( OTF2_EvtWriter*     writer,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 OTF2_CommRef        threadTeam,
                                 uint32_t            creatingThread,
                                 uint32_t            generationNumber );


/** @brief Records an ThreadTaskSwitch event.
 *
 *  An ThreadTaskSwitch record indicates that the execution of the current
 *  task will be suspended and another task starts/restarts its
 *  execution. Please note that this may change the current call stack
 *  of the executing location.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadTaskSwitch( OTF2_EvtWriter*     writer,
                                 OTF2_AttributeList* attributeList,
                                 OTF2_TimeStamp      time,
                                 OTF2_CommRef        threadTeam,
                                 uint32_t            creatingThread,
                                 uint32_t            generationNumber );


/** @brief Records an ThreadTaskComplete event.
 *
 *  An ThreadTaskComplete record indicates that the execution of an OpenMP
 *  task has finished.
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the event.
 *  @param time          The time for this event.
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
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtWriter_ThreadTaskComplete( OTF2_EvtWriter*     writer,
                                   OTF2_AttributeList* attributeList,
                                   OTF2_TimeStamp      time,
                                   OTF2_CommRef        threadTeam,
                                   uint32_t            creatingThread,
                                   uint32_t            generationNumber );


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
