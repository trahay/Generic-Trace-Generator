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


#ifndef OTF2_GLOBAL_SNAP_READER_H
#define OTF2_GLOBAL_SNAP_READER_H


/**
 *  @file       include/otf2/OTF2_GlobalSnapReader.h
 *
 *  @brief      This is the global snapshot event reader.
 *
 *  @since      Version 1.2
 *
 *  Used to read from multiple local snap event readers, and provide them in a timely
 *  ordered sequence.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_SnapReader.h>


#include <otf2/OTF2_GlobalSnapReaderCallbacks.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Sets the callback functions for the given reader object. Everytime
 *         when OTF2 reads a record, a callback function is called and the records
 *         data is passed to this function. Therefore the programmer needs to set
 *         function pointers at the "callbacks" struct for the record type he wants
 *         to read.
 *
 *  @param reader          Reader object which reads the snap events from its
 *                         buffer.
 *  @param callbacks       Struct which holds a function pointer for each
 *                         record type. @ref OTF2_GlobalSnapReaderCallbacks_New.
 *  @param userData        Data passed as argument @a userData to the record
 *                         callbacks.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 */
OTF2_ErrorCode
OTF2_GlobalSnapReader_SetCallbacks( OTF2_GlobalSnapReader*                reader,
                                    const OTF2_GlobalSnapReaderCallbacks* callbacks,
                                    void*                                 userData );


/** @brief Reads the given number of records from the global snap event reader.
 *
 *  @param reader           The records of this reader will be read when the function is issued.
 *  @param recordsToRead    This variable tells the reader how much records it has to read.
 *  @param[out] recordsRead This is a pointer to variable where the amount of actually read
 *                          records is returned. This may differ to the given recordsToRead if
 *                          there are no more records left in the trace. In this case the programmer
 *                          can easily check that the reader has finnished his job by checking
 *                          recordsRead < recordsToRead.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 */
OTF2_ErrorCode
OTF2_GlobalSnapReader_ReadSnapshots( OTF2_GlobalSnapReader* reader,
                                     uint64_t               recordsToRead,
                                     uint64_t*              recordsRead );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GLOBAL_EVT_READER */
