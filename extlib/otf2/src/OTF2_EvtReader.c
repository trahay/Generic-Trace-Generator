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


/**
 *  @file       src/OTF2_EvtReader.c
 *  @status     beta
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This module reads the local events, which means that the
 *              programmer can select only one location or location group.
 *              In the case of selecting a location group, only the events
 *              of the group (for example for a process) are delivered and no
 *              events from subgroups and sublocations. Rudimentary filtering
 *              could be done by registering only callbacks for the needed
 *              record types. All records which are delivered through this
 *              interface, have already global IDs. This is an external
 *              interface.
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>

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


/** @internal
 *  @brief A clock synchronization interval. */
struct OTF2_ClockInterval_struct
{
    OTF2_ClockInterval* next;
    uint64_t            interval_begin;
    uint64_t            interval_end;
    double              slope;
    int64_t             offset;
};


static inline uint64_t
otf2_evt_reader_map( OTF2_EvtReader*  reader,
                     OTF2_MappingType mapType,
                     uint64_t         localID );

static inline OTF2_ErrorCode
otf2_evt_reader_read_unknown( OTF2_EvtReader* reader );

static inline OTF2_ErrorCode
otf2_evt_reader_skip_record( OTF2_EvtReader* reader );

static inline OTF2_ErrorCode
otf2_evt_reader_skip_compressed( OTF2_EvtReader* reader );

static inline uint64_t
otf2_evt_reader_apply_clock_correction( OTF2_EvtReader* reader,
                                        uint64_t        time );


/* This _must_ be done for the generated part of the OTF2 local reader */
#include "OTF2_EvtReader_inc.c"
/* This _must_ be done for the generated part of the OTF2 local reader */

/** @internal
 *  @brief Create new reader object. This will load a location related
 *         trace file from the filesystem.
 *
 *  @param location     The location from which the records have to be
 *                      read.
 *  @param archivePath  Path to the archive directory.
 *  @param archiveName  Internal archive name.
 *  @param substrate    Which file substrate has to be used for reading.
 *  @param chunksize    Size of the chunks (in bytes).
 *
 *  @return OTF2_EvtReader* on success, and NULL if not
 */

OTF2_EvtReader*
otf2_evt_reader_new( OTF2_Archive*    archive,
                     OTF2_LocationRef location )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( location != OTF2_UNDEFINED_LOCATION );

    OTF2_EvtReader* reader = ( OTF2_EvtReader* )calloc( 1, sizeof( OTF2_EvtReader ) );
    if ( !reader )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for readers handle!" );
        return NULL;
    }

    /* Initialize non-zero members in reader struct. */
    reader->archive                = archive;
    reader->location_id            = location;
    reader->apply_mapping_tables   = true;
    reader->apply_clock_offsets    = true;
    reader->clock_intervals        = NULL;
    reader->tail_clock_intervals   = &reader->clock_intervals;
    reader->current_clock_interval = NULL;
    reader->pending_clock_interval = NULL;

    uint64_t       evt_chunk_size;
    uint64_t       def_chunk_size;
    OTF2_ErrorCode status = otf2_archive_get_chunksize( archive,
                                                        &evt_chunk_size,
                                                        &def_chunk_size );
    if ( status != OTF2_SUCCESS )
    {
        free( reader );
        return NULL;
    }

    /* Generate an internal buffer, and check for
     | success. The buffer is needed to store the
     | events into a memory representation.
     */
    reader->buffer = OTF2_Buffer_New( archive,
                                      reader,
                                      evt_chunk_size,
                                      OTF2_BUFFER_READ,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_EVENTS,
                                      location );

    if ( !reader->buffer )
    {
        UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                     "Creation of buffer handle failed!" );
        free( reader );
        return NULL;
    }

    /* Create attribute list. */
    otf2_attribute_list_init( &reader->attribute_list );

    return reader;
}



/** @internal
 *  @brief Delete the passed reader object. An file flush and file
 *         finalization is triggered if this function is called.
 *
 *  @param reader         Reader object which reads the events from its
 *                        buffer.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_evt_reader_delete( OTF2_EvtReader* reader )
{
    /* Check if this is a valid reader object */
    if ( !reader )
    {
        return OTF2_SUCCESS;
    }

    OTF2_ErrorCode ret
        = OTF2_Buffer_Delete( reader->buffer );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( ret, "Buffer deletion failed!" );
    }

    otf2_attribute_list_clear( &reader->attribute_list );

    free( reader->position_table );
    free( reader->timestamp_table );

    for ( uint64_t i = 0; i < OTF2_MAPPING_MAX; i++ )
    {
        OTF2_IdMap_Free( ( OTF2_IdMap* )reader->mapping_tables[ i ] );
        reader->mapping_tables[ i ] = NULL;
    }

    while ( reader->clock_intervals )
    {
        OTF2_ClockInterval* next = reader->clock_intervals->next;
        free( reader->clock_intervals );
        reader->clock_intervals = next;
    }

    if ( reader->pending_clock_interval )
    {
        free( reader->pending_clock_interval );
        reader->pending_clock_interval = NULL;
    }

    /* Delete the object itself */
    free( reader );

    return OTF2_SUCCESS;
}


/** @internal
 *  @brief Tell a local event reader that he is operated by a global reader.
 *
 *  A local event reader must know if he is operated by a global event reader to
 *  handle attribute lists correctly.
 *  Please note: This function should be only available to global event readers.
 *
 *  @param reader           Global event reader handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
void
otf2_evt_reader_operated_by_global_reader( OTF2_EvtReader* reader )
{
    /* Validate arguments. */
    UTILS_ASSERT( reader );

    reader->operated = true;
}


OTF2_ErrorCode
OTF2_EvtReader_GetLocationID( const OTF2_EvtReader* reader,
                              OTF2_LocationRef*     location )
{
    if ( !reader || !location )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    *location = reader->location_id;
    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReader_GetPos( OTF2_EvtReader* reader,
                       uint64_t*       position )
{
    /* Check if this is a valid reader object */
    if ( reader == NULL || position == NULL )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    /* Return the global event position */
    *position = reader->global_event_position;

    return OTF2_SUCCESS;
}


OTF2_EvtReaderCallbacks*
OTF2_EvtReaderCallbacks_New( void )
{
    return calloc( 1, sizeof( OTF2_EvtReaderCallbacks ) );
}


void
OTF2_EvtReaderCallbacks_Delete( OTF2_EvtReaderCallbacks* evtReaderCallbacks )
{
    free( evtReaderCallbacks );
}

void
OTF2_EvtReaderCallbacks_Clear( OTF2_EvtReaderCallbacks* evtReaderCallbacks )
{
    if ( evtReaderCallbacks != NULL )
    {
        memset( evtReaderCallbacks, 0, sizeof( OTF2_EvtReaderCallbacks ) );
    }
}


OTF2_ErrorCode
OTF2_EvtReader_SetCallbacks( OTF2_EvtReader*                reader,
                             const OTF2_EvtReaderCallbacks* callbacks,
                             void*                          userData )
{
    /* Check if this is a valid reader object */
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "No valid reader object!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    /* Set internal callback struct and user_data */
    memcpy( &reader->reader_callbacks, callbacks, sizeof( reader->reader_callbacks ) );
    reader->user_data = userData;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReader_Seek( OTF2_EvtReader* reader,
                     uint64_t        position )
{
    /* Check if this is a valid reader object */
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "No valid reader object!" );
    }

    free( reader->position_table );
    reader->position_table = NULL;
    free( reader->timestamp_table );
    reader->timestamp_table = NULL;

    OTF2_Buffer_ReadSeekChunk( reader->buffer, position );

    /* set global and local events positions */
    reader->global_event_position      = reader->buffer->chunk->first_event - 1;
    reader->chunk_local_event_position = 0;

    /* skip to requested position */
    while ( reader->global_event_position + 1 < position )
    {
        otf2_evt_reader_skip( reader );

        reader->global_event_position++;
        reader->chunk_local_event_position++;
    }

    return OTF2_SUCCESS;
}


/** @brief This function is used to generate a position table, if no such
 *         table exists. This is usually the case, if this function was
 *         not triggered in the current chunk so far. The position table
 *         is afterwards used to be able to step backward in an compressed
 *         buffer.
 *
 *  @param reader         Reader object which reads the events from its
 *                        buffer.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
static OTF2_ErrorCode
otf2_evt_reader_index( OTF2_EvtReader* reader )
{
    OTF2_ErrorCode ret = OTF2_SUCCESS;

    /* Skip if there is already an index available */
    if ( reader->position_table != NULL )
    {
        return OTF2_SUCCESS;
    }

    /* Save the current reading position. This is needed to return
     | to the current reading position after indexing has finnished.
     */
    OTF2_Buffer_GetPosition( reader->buffer, &reader->current_position );

    uint64_t first_event = 0;
    uint64_t last_event  = 0;

    ret = OTF2_Buffer_GetNumberEvents( reader->buffer, &first_event, &last_event );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( ret, "Event number retrieval failed!" );
    }

    uint64_t number_of_elements = ( last_event - first_event ) + 1;

    /* Allocate an index position table */
    reader->position_table = malloc( number_of_elements * sizeof( uint8_t* ) );
    if ( !reader->position_table )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Memory allocation failed!" );
    }

    reader->timestamp_table = malloc( number_of_elements * sizeof( uint8_t* ) );
    if ( !reader->timestamp_table )
    {
        free( reader->position_table );
        reader->position_table = NULL;
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Memory allocation failed!" );
    }

    /* Set the current position to the begin of the chunk and collect
     | some information about the data which is stored in the new chunk.
     | Skip the records (equals not performing the decompression),
     | and save their position into the table
     */
    uint8_t* position;
    OTF2_Buffer_GetBeginOfChunk( reader->buffer, &position );
    OTF2_Buffer_SetPosition( reader->buffer, position );
    uint64_t element_index = 0;

    do
    {
        OTF2_Buffer_GetPosition( reader->buffer,
                                 &reader->position_table[ element_index ] );
        OTF2_Buffer_GetPositionTimeStamp( reader->buffer,
                                          &reader->timestamp_table[ element_index ] );
        element_index++;

        ret = otf2_evt_reader_skip( reader );
        if ( ret == OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
        {
            /* no more records */
            ret = OTF2_SUCCESS;
            break;
        }
        else
        {
            free( reader->position_table );
            reader->position_table = NULL;
            free( reader->timestamp_table );
            reader->timestamp_table = NULL;
            OTF2_Buffer_SetPosition( reader->buffer, reader->current_position );
            return UTILS_ERROR( ret, "Error while scanning through the events." );
        }
    }
    while ( ret == OTF2_SUCCESS );

    /* Restore the current position (was stored at the beginning of this function */
    return OTF2_Buffer_SetPosition( reader->buffer, reader->current_position );
}


OTF2_ErrorCode
OTF2_EvtReader_ReadEvents( OTF2_EvtReader* reader,
                           uint64_t        recordsToRead,
                           uint64_t*       recordsRead )
{
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "No valid reader object!" );
    }

    OTF2_ErrorCode ret = OTF2_SUCCESS;
    uint64_t       read;
    for ( read = 0; read < recordsToRead; read++ )
    {
        ret = otf2_evt_reader_read( reader );
        if ( ret != OTF2_SUCCESS )
        {
            if ( ret == OTF2_ERROR_INTERRUPTED_BY_CALLBACK )
            {
                /* We still delivered a record, count it */
                read++;
            }
            /* This is the end of file, which is not an error. */
            else if ( ret == OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
            {
                ret = OTF2_SUCCESS;
            }
            break;
        }
    }

    *recordsRead = read;
    return ret;
}



/** @internal
 *  @brief This functions reads one event backwards from the current
 *  position, and sets the current position to this event.
 *
 *  @param reader   Reader object which reads the events from its
 *                  buffer.
 *  @param record   Generic event handle. Please pass a NULL here,
 *                  if you use this interface directly.
 *  @param type     Return the type of the read event.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_evt_reader_read_backward( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode ret = OTF2_SUCCESS;
    uint64_t       first_event;

    /* If the new position is in the previous chunk, the new chunk has to be loaded */
    if ( reader->chunk_local_event_position == 0 )
    {
        ret = OTF2_Buffer_ReadGetPreviousChunk( reader->buffer );
        if ( ret != OTF2_SUCCESS )
        {
            return UTILS_ERROR( ret, "Can't step back!" );
        }

        /* Get the correct chunk local position */
        OTF2_Buffer_GetNumberEvents( reader->buffer,
                                     &first_event,
                                     &reader->chunk_local_event_position );

        reader->chunk_local_event_position =
            reader->chunk_local_event_position - first_event;

        /* Clear the index tables */
        free( reader->position_table );
        reader->position_table = NULL;

        free( reader->timestamp_table );
        reader->timestamp_table = NULL;
    }

    /* The index is needed (function skips indexing if there is already an index) */
    ret = otf2_evt_reader_index( reader );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( ret, "Unable to get index!" );
    }

    uint8_t*       act_pos = NULL; /** @review  only used in branch, declare there */
    OTF2_EventType record_type;
    /* Find out if the record has a attribute list
     | (used a codeblock to make indention possible against
     | the code uglyfier)
     */
    {
        /** @review Please check error codes of following functions */
        /* Get the actual position to be able to restore afterwards */
        OTF2_Buffer_GetPosition( reader->buffer, &act_pos );

        /* Set the position to two records behind
         | (first position = record, second position = attribute list )
         */
        OTF2_Buffer_SetPosition( reader->buffer,
                                 reader->position_table[ reader->chunk_local_event_position - 2 ] );

        /* Read the type field */
        OTF2_Buffer_ReadUint8( reader->buffer, &record_type );

        /* Reset the position to the previous */
        OTF2_Buffer_SetPosition( reader->buffer, act_pos );
    }


    uint64_t diff_pos = 0;
    if ( record_type == OTF2_ATTRIBUTE_LIST )
    {
        diff_pos = 2;
    }
    else
    {
        diff_pos = 1;
    }

    reader->chunk_local_event_position = reader->chunk_local_event_position - diff_pos;
    reader->global_event_position      = reader->global_event_position - diff_pos;

    OTF2_Buffer_SetPosition
        ( reader->buffer, reader->position_table[ reader->chunk_local_event_position ] );

    OTF2_Buffer_SetPositionTimeStamp( reader->buffer,
                                      reader->timestamp_table[ reader->chunk_local_event_position ] );

    ret = otf2_evt_reader_read( reader );
    if ( ret != OTF2_SUCCESS )
    {
        if ( ret == OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
        {
            return OTF2_SUCCESS;
        }
        if ( ret == OTF2_ERROR_INTERRUPTED_BY_CALLBACK )
        {
            return ret;
        }
        return UTILS_ERROR( ret, "Error when reading record!" );
    }
    if ( record_type == OTF2_ATTRIBUTE_LIST )
    {
        ret = otf2_evt_reader_read( reader );
        if ( ret != OTF2_SUCCESS )
        {
            if ( ret == OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
            {
                return OTF2_SUCCESS;
            }
            if ( ret == OTF2_ERROR_INTERRUPTED_BY_CALLBACK )
            {
                return ret;
            }
            return UTILS_ERROR( ret, "Error when reading record!" );
        }
    }

    return OTF2_Buffer_SetPosition( reader->buffer,
                                    reader->position_table[ reader->chunk_local_event_position ] );
}


OTF2_ErrorCode
OTF2_EvtReader_ReadEventsBackward( OTF2_EvtReader* reader,
                                   uint64_t        recordsToRead,
                                   uint64_t*       recordsRead )
{
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "No valid reader object!" );
    }

    OTF2_ErrorCode ret = OTF2_SUCCESS;
    uint64_t       read;
    for ( read = 0; read < recordsToRead; read++ )
    {
        ret = otf2_evt_reader_read_backward( reader );
        if ( ret != OTF2_SUCCESS )
        {
            break;
        }
    }

    *recordsRead = read;
    return ret;
}


OTF2_ErrorCode
OTF2_EvtReader_TimeStampRewrite( OTF2_EvtReader* reader,
                                 OTF2_TimeStamp  time )
{
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "No valid reader object!" );
    }

    return OTF2_Buffer_RewriteTimeStamp( reader->buffer, time );
}


OTF2_ErrorCode
OTF2_EvtReader_ApplyMappingTables( OTF2_EvtReader* reader,
                                   bool            action )
{
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader object!" );
    }

    reader->apply_mapping_tables = action;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_EvtReader_ApplyClockOffsets( OTF2_EvtReader* reader,
                                  bool            action )
{
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader object!" );
    }

    reader->apply_clock_offsets = action;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_evt_reader_set_mapping_table( OTF2_EvtReader*   reader,
                                   OTF2_MappingType  mapType,
                                   const OTF2_IdMap* iDMap )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( iDMap );

    if ( mapType >= OTF2_MAPPING_MAX )
    {
        /* We need to ignore unknown mapping types */
        return OTF2_SUCCESS;
    }

    if ( reader->mapping_tables[ mapType ] != NULL )
    {
        return UTILS_ERROR( OTF2_ERROR_DUPLICATE_MAPPING_TABLE,
                            "Mapping %hhu was already set!", mapType );
    }

    reader->mapping_tables[ mapType ] = iDMap;

    return OTF2_SUCCESS;
}



/** @brief Generic map routine, to map an ID while reading.
 *
 *  @param reader   Reader object which reads the events from its buffer.
 *  @param mapType  Says which type if ID has to be mapped.
 *  @param localID  Local ID
 *
 *  @return Global ID
 */
uint64_t
otf2_evt_reader_map( OTF2_EvtReader*  reader,
                     OTF2_MappingType mapType,
                     uint64_t         localID )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( mapType < OTF2_MAPPING_MAX );

    /* Do nothing, if the user do not want the mapping tables,
     * but ignore the request if this EvtReader is operated by an
     * GlobalEvtReader. */
    if ( !reader->operated && !reader->apply_mapping_tables )
    {
        return localID;
    }

    const OTF2_IdMap* iDMap = reader->mapping_tables[ mapType ];
    if ( iDMap == NULL )
    {
        return localID;
    }

    uint64_t       global_id;
    OTF2_ErrorCode status =
        OTF2_IdMap_GetGlobalId( iDMap, localID, &global_id );

    if ( status != OTF2_SUCCESS )
    {
        return localID;
    }
    return global_id;
}


/** @brief Reads an event record of unknown type from the event buffer.
 *
 *  @param reader   Reference to the reader object
 *  @param record   Generic handle for returning the record. This is at
 *                  least needed for the global reader.
 *  @param time     Timestamp
 */

static inline OTF2_ErrorCode
otf2_evt_reader_read_unknown( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_GenericEvent* event = &reader->current_event;

    event->record.time = otf2_evt_reader_apply_clock_correction( reader,
                                                                 event->record.time );

    OTF2_ErrorCode ret;
    /* Get record length and test memory availability */
    uint64_t record_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    /* Skip record according to its length */
    ret = OTF2_Buffer_Skip( reader->buffer, record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Error while skipping unknown record." );
    }

    reader->global_event_position++;
    reader->chunk_local_event_position++;

    /*
     * If a local event reader is operated by a global event reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global event reader
     * will clean up the attribute list after he has triggered his callback.
     */
    if ( reader->operated )
    {
        return OTF2_SUCCESS;
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.unknown )
    {
        interrupt = reader->reader_callbacks.unknown( reader->location_id,
                                                      event->record.time,
                                                      reader->global_event_position,
                                                      reader->user_data,
                                                      &reader->attribute_list );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


/** @brief Skip an event record.
 *
 *  @param reader Reference to the reader object
 *
 *  @return ignored
 */
OTF2_ErrorCode
otf2_evt_reader_skip_record( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode ret;
    /* Get record length and test memory availability */
    uint64_t record_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Error while skipping record. Buffer does not provide enough memory." );
    }

    /* Skip record according to its length */
    return OTF2_Buffer_Skip( reader->buffer, record_length );
}

/** @brief Skip an singleton event record (has only one compressed attribute).
 *
 *  @param reader Reference to the reader object
 *
 *  @return ignored
 */
OTF2_ErrorCode
otf2_evt_reader_skip_compressed( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode ret;
    /* Get record length and test memory availability */
    uint64_t record_length;
    ret = OTF2_Buffer_GuaranteeCompressed( reader->buffer );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Error while skipping record. Buffer does not provide enough memory." );
    }

    /* Skip record according to its length */
    return OTF2_Buffer_SkipCompressed( reader->buffer );
}

OTF2_ErrorCode
otf2_evt_reader_add_clock_offset( OTF2_EvtReader* reader,
                                  uint64_t        time,
                                  int64_t         offset,
                                  double          stdDev )
{
    UTILS_ASSERT( reader );

    /* if there is a pending timer synchronization interval, complete it */
    if ( reader->pending_clock_interval != NULL )
    {
        /* complete the pending timer synchronization interval */
        OTF2_ClockInterval* new_interval = reader->pending_clock_interval;
        reader->pending_clock_interval = NULL;

        /* Multiple timer synchronization at the same time are a fault */
        if ( new_interval->interval_begin >= time )
        {
            free( new_interval );
            return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                                "Multiple timer synchronization at the same time" );
        }

        new_interval->interval_end = time;
        new_interval->slope        = ( ( double )offset - ( double )new_interval->offset )
                                     / ( ( double )time - ( double )new_interval->interval_begin );
        *reader->tail_clock_intervals = new_interval;
        reader->tail_clock_intervals  = &new_interval->next;

        /* if we got here, we have at least one interval */
        reader->current_clock_interval = reader->clock_intervals;
    }

    /* store this time synchronization in a pending interval */
    OTF2_ClockInterval* new_pending_interval = calloc( 1, sizeof( *new_pending_interval ) );
    if ( !new_pending_interval )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Can't allocate memory for timer interval" );
    }
    new_pending_interval->next           = NULL;
    new_pending_interval->interval_begin = time;
    new_pending_interval->interval_end   = 0;
    new_pending_interval->offset         = offset;
    new_pending_interval->slope          = 0.0;

    reader->pending_clock_interval = new_pending_interval;

    return OTF2_SUCCESS;
}

uint64_t
otf2_evt_reader_apply_clock_correction( OTF2_EvtReader* reader,
                                        uint64_t        time )
{
    UTILS_ASSERT( reader );

    /* Do nothing when there are no intervals */
    if ( !reader || !reader->clock_intervals )
    {
        return time;
    }

    /* Do nothing, if the user do not want the clock corrections,
     * but ignore the request if this EvtReader is operated by an
     * GlobalEvtReader. */
    if ( !reader->operated && !reader->apply_clock_offsets )
    {
        return time;
    }

    /*
     * search the interval for this timestamp
     * we can assume that the timestamp increase monotonical,
     * ie. we search from the current interval on forward
     */
    while ( true )
    {
        /* timestamps past the last interval use the last one */
        if ( !reader->current_clock_interval->next )
        {
            break;
        }

        if ( time > reader->current_clock_interval->interval_end )
        {
            reader->current_clock_interval = reader->current_clock_interval->next;
            continue;
        }
        break;
    }

    /* apply linear interpolation */
    double result = time + reader->current_clock_interval->slope
                    * ( ( double )time - ( double )reader->current_clock_interval->interval_begin )
                    + reader->current_clock_interval->offset;
    return rint( result );
}


OTF2_ErrorCode
otf2_evt_reader_skip( OTF2_EvtReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode status;

    OTF2_TimeStamp time;
    status = OTF2_Buffer_ReadTimeStamp( reader->buffer, &time );
    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    /* Read record type */
    status = OTF2_Buffer_Guarantee( reader->buffer, 1 );
    if ( OTF2_SUCCESS != status )
    {
        return UTILS_ERROR( status, "Could not read record type." );
    }
    uint8_t record_type;
    OTF2_Buffer_ReadUint8( reader->buffer, &record_type );

    switch ( record_type )
    {
        case OTF2_BUFFER_END_OF_CHUNK:
            return OTF2_ERROR_INDEX_OUT_OF_BOUNDS;

        case OTF2_ATTRIBUTE_LIST:
            status = otf2_evt_reader_skip_record( reader );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status, "Read of additional attributes failed!" );
            }
            return otf2_evt_reader_skip( reader );

        /*
         * Known singlton event records at 1.1, there wont be any after 1.1
         * They need special handling when skipping, because the UNDEFINED value
         * is encoded as a single 0xff byte, this conflicts with the 0xff marker
         * of the record legth byte, when the record length is >= 256.
         * Because than, the actuall record length is stored in the following
         * 8 bytes.
         */
        case OTF2_EVENT_ENTER:
        case OTF2_EVENT_LEAVE:
        case OTF2_EVENT_MPI_ISEND_COMPLETE:
        case OTF2_EVENT_MPI_IRECV_REQUEST:
        case OTF2_EVENT_MPI_REQUEST_TEST:
        case OTF2_EVENT_MPI_REQUEST_CANCELLED:
        case OTF2_EVENT_OMP_FORK:
        case OTF2_EVENT_OMP_TASK_CREATE:
        case OTF2_EVENT_OMP_TASK_SWITCH:
        case OTF2_EVENT_OMP_TASK_COMPLETE:
            return otf2_evt_reader_skip_compressed( reader );

        default:
            return otf2_evt_reader_skip_record( reader );
    }
}

OTF2_ErrorCode
OTF2_EvtReaderCallbacks_SetUnknownCallback(
    OTF2_EvtReaderCallbacks*       evtReaderCallbacks,
    OTF2_EvtReaderCallback_Unknown unknownCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->unknown = unknownCallback;

    return OTF2_SUCCESS;
}
