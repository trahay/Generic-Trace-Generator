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

/** @file       src/OTF2_GlobalEvtReader.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief
 *
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "otf2_attic_types.h"

#include "otf2_events_int.h"

#include "otf2_attribute_list.h"

#include "OTF2_File.h"
#include "OTF2_Buffer.h"

#include "otf2_evt_reader.h"

#include "otf2_global_evt_reader.h"

/* ___ Global event reader structure. _______________________________________ */


static inline
OTF2_ErrorCode
otf2_global_evt_reader_trigger_unknown( const OTF2_GlobalEvtReader* reader,
                                        OTF2_EvtReader*             evt_reader );


/* This _must_ be done for the generated part of the OTF2 local reader */
#include "OTF2_GlobalEvtReader_inc.c"
/* This _must_ be done for the generated part of the OTF2 local reader */


/* ___ Prototypes for internal functions. ___________________________________ */


static inline bool
compare_evt_reader_lt( OTF2_EvtReader* evt_reader_i,
                       OTF2_EvtReader* evt_reader_j );


static void
percolate_down( OTF2_GlobalEvtReader* reader,
                uint64_t              node );


/* ___ Implementation. ______________________________________________________ */


/** @brief Create new global event reader.
 *
 *  @param localReaders     SCOREP vector containing local event readers.
 *
 *  @return                 Returns a handle to the global event reader if
 *                          successful, NULL otherwise.
 */
OTF2_GlobalEvtReader*
otf2_global_evt_reader_new( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( archive->local_evt_readers );

    /* Create reader. */
    OTF2_GlobalEvtReader* reader = NULL;
    reader = ( OTF2_GlobalEvtReader* )calloc( 1, sizeof( OTF2_GlobalEvtReader )
                                              + ( archive->number_of_evt_readers * sizeof( OTF2_EvtReader* ) ) );
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for reader handle!" );
        return NULL;
    }

    /* Initialize reader. */
    reader->archive = archive;
    memset( &( reader->reader_callbacks ), '\0', sizeof( OTF2_GlobalEvtReaderCallbacks ) );
    reader->user_data = NULL;

    reader->number_of_evt_readers = 0;
    /* Read an event from each local reader and put it in the event queue. */
    for ( OTF2_EvtReader* evt_reader = archive->local_evt_readers;
          evt_reader;
          evt_reader = evt_reader->next )
    {
        /* Tell the local event reader that he is operated by a global event
         * reader and therefore not stand-alone. This is important for the
         * attribute lists. */
        otf2_evt_reader_operated_by_global_reader( evt_reader );

        OTF2_ErrorCode status = otf2_evt_reader_read( evt_reader );
        if ( status != OTF2_SUCCESS )
        {
            /* If there is no event in the local event trace file, continue with
             * next local event reader. */
            if ( status == OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
            {
                continue;
            }

            UTILS_ERROR( status, "Could not read event from location!" );
            otf2_global_evt_reader_delete( reader );
            return NULL;
        }

        reader->event_queue[ reader->number_of_evt_readers++ ] = evt_reader;
    }

    /* build-up the heap */
    for ( uint64_t i = reader->number_of_evt_readers; i--; )
    {
        percolate_down( reader, i );
    }

    return reader;
}


/** @brief Triggers the callback for an unknown event record.
 *
 *  @param reader     Reference to the reader object
 *  @param evt_reader Reference to the event reader object
 *
 *  @return OTF2_ErrorCode, which is OTF2_SUCCESS if everything is fine.
 */
static inline
OTF2_ErrorCode
otf2_global_evt_reader_trigger_unknown( const OTF2_GlobalEvtReader* reader,
                                        OTF2_EvtReader*             evt_reader )
{
    OTF2_GenericEvent* event = &evt_reader->current_event;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.unknown )
    {
        interrupt = reader->reader_callbacks.unknown( evt_reader->location_id,
                                                      event->record.time,
                                                      reader->user_data,
                                                      &evt_reader->attribute_list );
    }

    /* Before the next event is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &evt_reader->attribute_list );

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


/** @brief Delete a global event reader.
 *
 *  Releases all allocated resources. Does nothing if NULL is provided.
 *
 *  @param readerHandle     Global event reader handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_global_evt_reader_delete( OTF2_GlobalEvtReader* readerHandle )
{
    /* Validate argument. */
    if ( NULL == readerHandle )
    {
        return OTF2_SUCCESS;
    }

    while ( readerHandle->number_of_evt_readers-- )
    {
        OTF2_EvtReader* evt_reader = readerHandle->event_queue[ readerHandle->number_of_evt_readers ];
        otf2_archive_close_evt_reader( readerHandle->archive, evt_reader );
    }

    free( readerHandle );

    return OTF2_SUCCESS;
}


OTF2_GlobalEvtReaderCallbacks*
OTF2_GlobalEvtReaderCallbacks_New( void )
{
    return calloc( 1, sizeof( OTF2_GlobalEvtReaderCallbacks ) );
}


void
OTF2_GlobalEvtReaderCallbacks_Delete( OTF2_GlobalEvtReaderCallbacks* globalEvtReaderCallbacks )
{
    free( globalEvtReaderCallbacks );
}


void
OTF2_GlobalEvtReaderCallbacks_Clear( OTF2_GlobalEvtReaderCallbacks* globalEvtReaderCallbacks )
{
    if ( globalEvtReaderCallbacks != NULL )
    {
        memset( globalEvtReaderCallbacks, 0, sizeof( OTF2_GlobalEvtReaderCallbacks ) );
    }
}


OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_SetUnknownCallback(
    OTF2_GlobalEvtReaderCallbacks*       globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_Unknown unknownCallback )
{
    if ( !globalEvtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalEvtReaderCallback argument!" );
    }

    globalEvtReaderCallbacks->unknown = unknownCallback;

    return OTF2_SUCCESS;
}


/** @brief Set user callbacks.
 *
 *  @param readerHandle     Global event reader handle.
 *  @param callbacks        Structure keeping all defined callbacks.
 *  @param userData         Additional user data provided in the callbacks.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalEvtReader_SetCallbacks( OTF2_GlobalEvtReader*                readerHandle,
                                   const OTF2_GlobalEvtReaderCallbacks* callbacks,
                                   void*                                userData )
{
    /* Validate arguments. */
    if ( NULL == readerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid global event reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    /* Set internal callback struct and userdata */
    memcpy( &readerHandle->reader_callbacks,
            callbacks,
            sizeof( readerHandle->reader_callbacks ) );
    readerHandle->user_data = userData;

    return OTF2_SUCCESS;
}



/* ___ Read routines. _______________________________________________________ */



/** @brief Read an event.
 *
 *  @param readerHandle     Global event reader handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalEvtReader_ReadEvent( OTF2_GlobalEvtReader* readerHandle )
{
    /* Validate arguments. */
    if ( NULL == readerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid global event reader handle!" );
    }

    /* If the heap is empty, all records where already read. */
    if ( 0 == readerHandle->number_of_evt_readers )
    {
        return OTF2_SUCCESS;
    }

    /* Get event with the least timestamp. */
    OTF2_EvtReader* evt_reader = readerHandle->event_queue[ 0 ];

    /* Trigger according callback. */
    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    OTF2_ErrorCode    status    = otf2_global_evt_reader_trigger_callback( readerHandle,
                                                                           evt_reader );
    if ( OTF2_ERROR_INTERRUPTED_BY_CALLBACK == status )
    {
        interrupt = !OTF2_CALLBACK_SUCCESS;
        status    = OTF2_SUCCESS;
    }
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Triggering event callback failed!" );
    }

    /*
     * Read next event from the according local reader and push it to queue.
     * This can't return OTF2_ERROR_INTERRUPTED_BY_CALLBACK, because no
     * callback will be triggered.
     */
    status = otf2_evt_reader_read( evt_reader );
    if ( status != OTF2_SUCCESS )
    {
        /* This error code means that all events from the according buffer are
         * already read. So this is no error. */
        if ( status != OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
        {
            /* This error overrules an callback interruption. */
            return UTILS_ERROR( status, "Could not read event from location!" );
        }

        otf2_archive_close_evt_reader( readerHandle->archive, evt_reader );

        /* remove event reader from queue */
        readerHandle->number_of_evt_readers--;

        if ( 0 == readerHandle->number_of_evt_readers )
        {
            /*
             * Queue is empty.
             * But still pass an !OTF2_CALLBACK_SUCCESS to the caller.
             */
            goto out;
        }

        /* move event reader from the last leaf to the top */
        readerHandle->event_queue[ 0 ] = readerHandle->event_queue[ readerHandle->number_of_evt_readers ];
    }

    /* update queue with new event in top event reader */
    percolate_down( readerHandle, 0 );

out:
    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


/** @brief Has more events.
 *
 *  @param readerHandle     Global event reader handle.
 *  @param flag             In case of success, the flag will be set to 1
 *                          when there is at least more more event to read.
 *                          To 0 if not. Otherwise the value is undefined.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalEvtReader_HasEvent( OTF2_GlobalEvtReader* readerHandle,
                               int*                  flag )
{
    /* Validate arguments. */
    if ( NULL == readerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid global event reader handle!" );
    }

    /* If the heap is empty, all records where already read. */
    *flag = !!readerHandle->number_of_evt_readers;

    return OTF2_SUCCESS;
}


/** @brief Read multiple events.
 *
 *  @param readerHandle     Global event reader handle.
 *  @param recordsToRead    Number of record to read.
 *  @param recordsRead      Number of records read.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalEvtReader_ReadEvents( OTF2_GlobalEvtReader* readerHandle,
                                 const uint64_t        recordsToRead,
                                 uint64_t*             recordsRead )
{
    /* Validate arguments. */
    if ( NULL == readerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "This is no valid global event reader handle!" );
    }

    OTF2_ErrorCode status = OTF2_SUCCESS;
    uint64_t       read   = 0;
    for ( read = 0; read < recordsToRead; read++ )
    {
        int flag;

        /* Check, if we have a new events */
        status = OTF2_GlobalEvtReader_HasEvent( readerHandle, &flag );
        if ( status != OTF2_SUCCESS || !flag )
        {
            break;
        }

        status = OTF2_GlobalEvtReader_ReadEvent( readerHandle );
        if ( status != OTF2_SUCCESS )
        {
            if ( status == OTF2_ERROR_INTERRUPTED_BY_CALLBACK )
            {
                /* We still delivered a record, count it */
                read++;
            }
            break;
        }
    }

    *recordsRead = read;
    return status;
}


/* ___ Implementation of internal functions. ________________________________ */



/** @brief Compare two event readers events by timestamp.
 *
 *  @param evt_reader_i First generic event to be compared.
 *  @param evt_reader_j Second generic event to be compared.
 *
 *  @return             True, if evt_reader_i has a smaller timestamp than
 *                      evt_reader_j.
 */
bool
compare_evt_reader_lt( OTF2_EvtReader* evt_reader_i,
                       OTF2_EvtReader* evt_reader_j )
{
    if ( evt_reader_i->current_event.record.time < evt_reader_j->current_event.record.time )
    {
        return true;
    }
    else if ( evt_reader_i->current_event.record.time == evt_reader_j->current_event.record.time )
    {
        /* If timestamps equal, compare the location IDs. This guaranties that
         * events with same timestamps on one location are delivered right after
         * each other without events of other locations with same timestamp
         * between them.
         * Note, there are at most one event for each location in the queue,
         * thus the locations are never equal. */
        if ( evt_reader_i->location_id < evt_reader_j->location_id )
        {
            return true;
        }
    }

    return false;
}

void
percolate_down( OTF2_GlobalEvtReader* reader,
                uint64_t              node )
{
    OTF2_EvtReader** event_queue = reader->event_queue;
    uint64_t         n           = reader->number_of_evt_readers;

    while ( node < n )
    {
        uint64_t smallest = node;

        /* check left child */
        uint64_t child = node * 2 + 1;
        if ( child < n
             && compare_evt_reader_lt( event_queue[ child ],
                                       event_queue[ smallest ] ) )
        {
            smallest = child;
        }

        /* check right child */
        child++;
        if ( child < n
             && compare_evt_reader_lt( event_queue[ child ],
                                       event_queue[ smallest ] ) )
        {
            smallest = child;
        }

        if ( smallest == node )
        {
            return;
        }

        /* swap down, and continue with child */
        OTF2_EvtReader* tmp = event_queue[ smallest ];
        event_queue[ smallest ] = event_queue[ node ];
        event_queue[ node ]     = tmp;
        node                    = smallest;
    }
}
