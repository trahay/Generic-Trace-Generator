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


#ifndef OTF2_GLOBAL_EVT_READER_H
#define OTF2_GLOBAL_EVT_READER_H


/**
 *  @file       include/otf2/OTF2_GlobalEvtReader.h
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This is the global event reader.
 *
 *  Used to read from multiple local event readers, and provide them in a timely
 *  ordered sequence.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_EvtReader.h>


#include <otf2/OTF2_GlobalEvtReaderCallbacks.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Sets the callback functions for the given reader object. Everytime
 *         when OTF2 reads a record, a callback function is called and the records
 *         data is passed to this function. Therefore the programmer needs to set
 *         function pointers at the "callbacks" struct for the record type he wants
 *         to read.
 *
 *  @param reader          Reader object which reads the events from its
 *                         buffer.
 *  @param callbacks       Struct which holds a function pointer for each
 *                         record type. @ref OTF2_GlobalEvtReaderCallbacks_New.
 *  @param userData        Data passed as argument @a userData to the record
 *                         callbacks.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalEvtReader_SetCallbacks( OTF2_GlobalEvtReader*                reader,
                                   const OTF2_GlobalEvtReaderCallbacks* callbacks,
                                   void*                                userData );


/** @brief Triggers the callback for the next event record.
 *
 *  @param reader          Reader object which reads the events from its
 *                         buffer.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalEvtReader_ReadEvent( OTF2_GlobalEvtReader* reader );


/** @brief Has more events.
 *
 *  @param reader     Global event reader handle.
 *  @param[out] flag  In case of success, the flag will be set to 1
 *                    when there is at least more more event to read.
 *                    To 0 if not. Otherwise the value is undefined.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalEvtReader_HasEvent( OTF2_GlobalEvtReader* reader,
                               int*                  flag );

/** @brief Reads the given number of records from the global event reader.
 *
 *  @param reader           The records of this reader will be read when the function is issued.
 *  @param recordsToRead    This variable tells the reader how much records it has to read.
 *  @param[out] recordsRead This is a pointer to variable where the amount of actually read
 *                          records is returned. This may differ to the given recordsToRead if
 *                          there are no more records left in the trace. In this case the programmer
 *                          can easily check that the reader has finnished his job by checking
 *                          recordsRead < recordsToRead.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalEvtReader_ReadEvents( OTF2_GlobalEvtReader* reader,
                                 uint64_t              recordsToRead,
                                 uint64_t*             recordsRead );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_GLOBAL_EVT_READER */
