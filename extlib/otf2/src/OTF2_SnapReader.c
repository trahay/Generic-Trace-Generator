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
 *  @file       src/OTF2_SnapReader.c
 *
 *  @brief      This module reads the local snapshot records.
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

#include "otf2_snaps.h"

#include "otf2_attribute_list.h"

#include "OTF2_File.h"
#include "OTF2_Buffer.h"

#include "otf2_snap_reader.h"


static inline OTF2_ErrorCode
otf2_snap_reader_read_unknown( OTF2_SnapReader* reader );

static inline OTF2_ErrorCode
otf2_snap_reader_skip_record( OTF2_SnapReader* reader );



/* This _must_ be done for the generated part of the OTF2 local reader */
#include "OTF2_SnapReader_inc.c"
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
 *  @return OTF2_SnapReader* on success, and NULL if not
 */

OTF2_SnapReader*
otf2_snap_reader_new( OTF2_Archive*    archive,
                      OTF2_LocationRef location )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( location != OTF2_UNDEFINED_LOCATION );

    OTF2_SnapReader* reader = ( OTF2_SnapReader* )calloc( 1, sizeof( OTF2_SnapReader ) );
    if ( !reader )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for readers handle!" );
        return NULL;
    }

    /* Initialize non-zero members in reader struct. */
    reader->archive     = archive;
    reader->location_id = location;

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

    reader->buffer = OTF2_Buffer_New( archive,
                                      reader,
                                      evt_chunk_size,
                                      OTF2_BUFFER_READ,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_SNAPSHOTS,
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
 *  @param reader Reader object which reads the records from its buffer.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_snap_reader_delete( OTF2_SnapReader* reader )
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

    /* Delete the object itself */
    free( reader );

    return OTF2_SUCCESS;
}


/** @internal
 *  @brief Tell a local snapshot reader that he is operated by a global reader.
 *
 *  A local snapshot reader must know if he is operated by a global snapshot
 *  reader to handle attribute lists correctly.
 *  Please note: This function should be only available to global snapshot readers.
 *
 *  @param reader           Global snapshot reader handle.
 *
 */
void
otf2_snap_reader_operated_by_global_reader( OTF2_SnapReader* reader )
{
    /* Validate arguments. */
    UTILS_ASSERT( reader );

    reader->operated = true;
}


OTF2_ErrorCode
OTF2_SnapReader_GetLocationID( const OTF2_SnapReader* reader,
                               OTF2_LocationRef*      location )
{
    if ( !reader || !location )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    *location = reader->location_id;
    return OTF2_SUCCESS;
}


OTF2_SnapReaderCallbacks*
OTF2_SnapReaderCallbacks_New( void )
{
    return calloc( 1, sizeof( OTF2_SnapReaderCallbacks ) );
}


void
OTF2_SnapReaderCallbacks_Delete( OTF2_SnapReaderCallbacks* evtReaderCallbacks )
{
    free( evtReaderCallbacks );
}

void
OTF2_SnapReaderCallbacks_Clear( OTF2_SnapReaderCallbacks* evtReaderCallbacks )
{
    if ( evtReaderCallbacks != NULL )
    {
        memset( evtReaderCallbacks, 0, sizeof( OTF2_SnapReaderCallbacks ) );
    }
}


OTF2_ErrorCode
OTF2_SnapReader_SetCallbacks( OTF2_SnapReader*                reader,
                              const OTF2_SnapReaderCallbacks* callbacks,
                              void*                           userData )
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


static OTF2_ErrorCode
otf2_snap_reader_seek_snapshot_start( OTF2_SnapReader* reader,
                                      uint64_t         req_time,
                                      bool*            found )
{
    OTF2_ErrorCode ret;

    *found = false;

    /* skip to first snapshot record with largest time less equal then
     * requested time */
    uint8_t*       read_position, * snapshot_start;
    OTF2_TimeStamp evt_time;
    do
    {
        /* store current read position */
        OTF2_Buffer_GetPosition( reader->buffer, &read_position );

        ret = OTF2_Buffer_ReadTimeStamp( reader->buffer, &evt_time );
        if ( OTF2_SUCCESS != ret )
        {
            return UTILS_ERROR( ret, "No timestamp at expected position!" );
        }

        if ( evt_time <= req_time )
        {
            /* Read record type */
            uint8_t record_type;
            ret = OTF2_Buffer_Guarantee( reader->buffer, 1 );
            if ( OTF2_SUCCESS != ret )
            {
                return UTILS_ERROR( ret,
                                    "Insufficient buffer size for reading record type." );
            }
            OTF2_Buffer_ReadUint8( reader->buffer, &record_type );

            if ( record_type == OTF2_SNAP_SNAPSHOT_START )
            {
                snapshot_start = read_position;
                *found         = true;
            }

            if ( record_type == OTF2_BUFFER_END_OF_CHUNK )
            {
                break;
            }

            /* skip record */
            ret = otf2_snap_reader_skip_record( reader );
            if ( OTF2_SUCCESS != ret )
            {
                return UTILS_ERROR( ret,
                                    "Invalid record data while skiping." );
            }
        }
    }
    while ( evt_time < req_time );

    if ( *found )
    {
        /* set read position to latest snapshot start */
        OTF2_Buffer_SetPosition( reader->buffer, snapshot_start );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_SnapReader_Seek( OTF2_SnapReader* reader,
                      uint64_t         req_time,
                      bool*            found )
{
    /* Check if this is a valid reader object */
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "No valid reader object!" );
    }
    if ( !found )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "No valid reader object!" );
    }

    OTF2_ErrorCode ret = OTF2_Buffer_ReadSeekChunkTime( reader->buffer,
                                                        req_time,
                                                        found );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Seeking failed!" );
    }

    if ( !*found )
    {
        return OTF2_SUCCESS;
    }

    bool start_found = false;

    ret = otf2_snap_reader_seek_snapshot_start( reader, req_time, &start_found );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( ret, "Can't seek snapshot start!" );
    }

    while ( !start_found )
    {
        /* there was no snaphot start record before requested time
         * so we have to look for it in the previous chunk */
        ret = OTF2_Buffer_ReadGetPreviousChunk( reader->buffer );
        if ( ret != OTF2_SUCCESS )
        {
            return UTILS_ERROR( ret, "Can't step back!" );
        }

        ret = otf2_snap_reader_seek_snapshot_start( reader, req_time, &start_found );
        if ( ret != OTF2_SUCCESS )
        {
            return UTILS_ERROR( ret, "Can't seek snapshot start!" );
        }
    }

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
OTF2_SnapReader_ReadSnapshots( OTF2_SnapReader* reader,
                               uint64_t         recordsToRead,
                               uint64_t*        recordsRead )
{
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "No valid reader object!" );
    }

    OTF2_ErrorCode ret = OTF2_SUCCESS;
    uint64_t       read;
    for ( read = 0; read < recordsToRead; read++ )
    {
        ret = otf2_snap_reader_read( reader );
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



/** @brief Reads an snapshot record of unknown type from the buffer.
 *
 *  @param reader   Reference to the reader object
 *  @param record   Generic handle for returning the record. This is at
 *                  least needed for the global reader.
 *  @param time     Timestamp
 */

static inline OTF2_ErrorCode
otf2_snap_reader_read_unknown( OTF2_SnapReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_GenericSnap* snap = &reader->current_snap;

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

    /*
     * If a local snapshot reader is operated by a global snapshot reader,
     * don't call our callback, don't clean up the attribute list and
     * don't free any attribute arrays. Otherwise the global snapshot reader
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
                                                      snap->record.time,
                                                      reader->user_data,
                                                      &reader->attribute_list );
    }

    otf2_attribute_list_remove_all_attributes( &reader->attribute_list );

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


/** @brief Skip one snapshot record.
 *
 *  @param reader Reference to the reader object
 *
 *  @return ignored
 */
OTF2_ErrorCode
otf2_snap_reader_skip_record( OTF2_SnapReader* reader )
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



OTF2_ErrorCode
otf2_snap_reader_skip( OTF2_SnapReader* reader )
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
            status = otf2_snap_reader_skip_record( reader );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status, "Read of additional attributes failed!" );
            }
            return otf2_snap_reader_skip( reader );

        default:
            return otf2_snap_reader_skip_record( reader );
    }
}

OTF2_ErrorCode
OTF2_SnapReaderCallbacks_SetUnknownCallback( OTF2_SnapReaderCallbacks*       evtReaderCallbacks,
                                             OTF2_SnapReaderCallback_Unknown unknownCallback )
{
    if ( !evtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid evtReaderCallbacks argument!" );
    }

    evtReaderCallbacks->unknown = unknownCallback;

    return OTF2_SUCCESS;
}
