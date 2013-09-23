/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
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


#ifndef OTF2_EVT_READER_H
#define OTF2_EVT_READER_H


/**
 *  @file       include/otf2/OTF2_EvtReader.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This is the local event reader, which reads events from one
 *              location.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Events.h>
#include <otf2/OTF2_Definitions.h>
#include <otf2/OTF2_AttributeList.h>
#include <otf2/OTF2_EvtReaderCallbacks.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Return the location ID of the reading related location.
 *
 *  @param reader         Reader object which reads the events from its
 *                        buffer.
 *  @param[out] location  ID of the location.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_GetLocationID( const OTF2_EvtReader* reader,
                              OTF2_LocationRef*     location );


/** @brief The following function can be used to get the position
 *         (number of the event in the stream) of last read event.
 *
 *  @param reader         Reader object which reads the events from its
 *                        buffer.
 *  @param[out] position  Number of the event in the stream.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_GetPos( OTF2_EvtReader* reader,
                       uint64_t*       position );


/** @brief Sets the callback functions for the given reader object. Everytime
 *         when OTF2 reads a record, a callback function is called and the records
 *         data is passed to this function. Therefore the programmer needs to set
 *         function pointers at the "callbacks" struct for the record type he wants
 *         to read.
 *
 *  These callbacks are ignored, if the events are read by an global event reader.
 *
 *  @param reader          Reader object which reads the events from its
 *                         buffer.
 *  @param callbacks       Struct which holds a function pointer for each
 *                         record type. @ref OTF2_EvtReaderCallbacks_New.
 *  @param userData        Data passed as argument @a userData to the record
 *                         callbacks.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_SetCallbacks( OTF2_EvtReader*                reader,
                             const OTF2_EvtReaderCallbacks* callbacks,
                             void*                          userData );


/** @brief Seek jumps to an event position.
 *
 *  @param reader         Reader object which reads the events from its
 *                        buffer.
 *  @param position       Number of the event, where the reader has to
 *                        jump.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_Seek( OTF2_EvtReader* reader,
                     uint64_t        position );


/** @brief After callback registration, the local events could be read
 *         with the following function. Readn reads @a recordsToRead records.
 *         The reader indicates that it reached the end of the trace by just
 *         reading less records than requested.
 *
 *  @param reader           Reader object which reads the events from its buffer.
 *  @param recordsToRead    How many records can be read next.
 *  @param[out] recordsRead Return how many records where really read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_ReadEvents( OTF2_EvtReader* reader,
                           uint64_t        recordsToRead,
                           uint64_t*       recordsRead );


/** @brief The following function rewrites the timestamp from the event on the
 *  actual reading position if the buffer is in OTF2_BUFFER_MODIFY mode. It also
 *  modifies the timestamp for all other events in the same timestamp bundle. This
 *  function also has to keep track that not only the last timestamp, but all records
 *  equal to the last timestamp has to be modified. This is done by a position list,
 *  if there has no seek appeared before. In this case a position list can be easily
 *  generated because of that the reader has seen all related timestamps before.
 *  This not the case if there has a seek appeared before. In this case the related
 *  timestamp positions are generated by a linear search.
 *
 *
 *  @param reader  Reader object which reads the events from its buffer.
 *  @param time    New timestamp
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_TimeStampRewrite( OTF2_EvtReader* reader,
                                 OTF2_TimeStamp  time );


/** @brief This functions reads recordsRead events backwards from the current
 *  position.
 *
 *  @param reader           Reader object which reads the events from its buffer.
 *  @param recordsToRead    How many records can be read next.
 *  @param[out] recordsRead Return how many records where really read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_ReadEventsBackward( OTF2_EvtReader* reader,
                                   uint64_t        recordsToRead,
                                   uint64_t*       recordsRead );


/** @brief Enable or disable applying of the mapping tabes to events read
 *         from this event reader.
 *
 *  This setting has no effect if the eventes are read by an global event
 *  reader.
 *
 *  @param reader  Reader object.
 *  @param action  Truth value whether the mappings should be applied or not.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_ApplyMappingTables( OTF2_EvtReader* reader,
                                   bool            action );


/** @brief Enable or disable applying of the clock offset to event timestamps
 *         read from this event reader.
 *
 *  This setting has no effect if the eventes are read by an global event
 *  reader.
 *
 *  @param reader  Reader object.
 *  @param action  Truth value whether the clock offsets should be applied or
 *                 not.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_EvtReader_ApplyClockOffsets( OTF2_EvtReader* reader,
                                  bool            action );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_EVT_READER_H */
