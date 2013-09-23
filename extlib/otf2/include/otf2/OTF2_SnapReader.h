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


#ifndef OTF2_SNAP_READER_H
#define OTF2_SNAP_READER_H


/**
 *  @file       include/otf2/OTF2_SnapReader.h
 *
 *  @brief      This is the local snap reader, which reads snapshot events from one
 *              location.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Events.h>
#include <otf2/OTF2_Definitions.h>
#include <otf2/OTF2_AttributeList.h>
#include <otf2/OTF2_SnapReaderCallbacks.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Return the location ID of the reading related location.
 *
 *  @param reader         Reader object which reads the snapshot events from its
 *                        buffer.
 *  @param[out] location  ID of the location.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapReader_GetLocationID( const OTF2_SnapReader* reader,
                               OTF2_LocationRef*      location );


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
 *                         record type. @ref OTF2_SnapReaderCallbacks_New.
 *  @param userData        Data passed as argument @a userData to the record
 *                         callbacks.
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapReader_SetCallbacks( OTF2_SnapReader*                reader,
                              const OTF2_SnapReaderCallbacks* callbacks,
                              void*                           userData );


/** @brief Seek jumps to start of latest snaphot that was made
 *         before a given time 'req_time'.
 *
 *  @param reader         Reader object which reads the events from its
 *                        buffer.
 *  @param req_time       Requested time (see above)
 *  @param found          returns if a matching snapshot was found
 *
 *  @since Version 1.2
 *
 *  @return OTF2_Error_Code with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
OTF2_SnapReader_Seek( OTF2_SnapReader* reader,
                      uint64_t         req_time,
                      bool*            found );


/** @brief After callback registration, the local events could be read
 *         with the following function. Readn reads @a recordsToRead records.
 *         The reader indicates that it reached the end of the trace by just
 *         reading less records than requested.
 *
 *  @param reader           Reader object which reads the events from its buffer.
 *  @param recordsToRead    How many records can be read next.
 *  @param[out] recordsRead Return how many records where really read.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapReader_ReadSnapshots( OTF2_SnapReader* reader,
                               uint64_t         recordsToRead,
                               uint64_t*        recordsRead );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_SNAP_READER_H */
