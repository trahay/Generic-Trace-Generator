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

/**
 *  @file       src/OTF2_MarkerReader.c
 *
 *  @brief      This layer provides read routines for marker records of
 *              an archive.
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

#include "otf2_events_int.h"

#include "OTF2_File.h"
#include "OTF2_Buffer.h"

#include "otf2_marker_int.h"


static inline OTF2_ErrorCode
otf2_marker_reader_read( OTF2_MarkerReader* reader );

static inline OTF2_ErrorCode
otf2_marker_reader_read_def_marker( OTF2_MarkerReader* reader );

static inline OTF2_ErrorCode
otf2_marker_reader_read_marker( OTF2_MarkerReader* reader );

static inline OTF2_ErrorCode
otf2_marker_reader_read_unknown( OTF2_MarkerReader* reader );


OTF2_MarkerReader*
otf2_marker_reader_new( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );

    OTF2_MarkerReader* reader = ( OTF2_MarkerReader* )calloc( 1, sizeof( OTF2_MarkerReader ) );
    if ( !reader )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for reader handle!" );
        return NULL;
    }

    reader->archive = archive;

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
                                      def_chunk_size,
                                      OTF2_BUFFER_READ,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_MARKER,
                                      OTF2_UNDEFINED_LOCATION );

    if ( !reader->buffer )
    {
        UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                     "Creation of buffer handle failed!" );
        free( reader );
        return NULL;
    }

    return reader;
}


OTF2_ErrorCode
otf2_marker_reader_delete( OTF2_MarkerReader* readerHandle )
{
    if ( !readerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Reader deletion failed!" );
    }

    OTF2_ErrorCode ret = OTF2_Buffer_Delete( readerHandle->buffer );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Buffer deletion failed!" );
    }

    free( readerHandle );

    return OTF2_SUCCESS;
}


OTF2_MarkerReaderCallbacks*
OTF2_MarkerReaderCallbacks_New( void )
{
    return calloc( 1, sizeof( OTF2_MarkerReaderCallbacks ) );
}


void
OTF2_MarkerReaderCallbacks_Delete( OTF2_MarkerReaderCallbacks* markerReaderCallbacks )
{
    free( markerReaderCallbacks );
}


void
OTF2_MarkerReaderCallbacks_Clear( OTF2_MarkerReaderCallbacks* markerReaderCallbacks )
{
    if ( markerReaderCallbacks != NULL )
    {
        memset( markerReaderCallbacks, 0, sizeof( OTF2_MarkerReaderCallbacks ) );
    }
}


OTF2_ErrorCode
OTF2_MarkerReaderCallbacks_SetUnknownCallback( OTF2_MarkerReaderCallbacks*       markerReaderCallbacks,
                                               OTF2_MarkerReaderCallback_Unknown unknownCallback )
{
    if ( !markerReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid Marker reader Callbacks argument!" );
    }

    markerReaderCallbacks->unknown = unknownCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_MarkerReaderCallbacks_SetDefMarkerCallback( OTF2_MarkerReaderCallbacks*         markerReaderCallbacks,
                                                 OTF2_MarkerReaderCallback_DefMarker defMarkerCallback )
{
    if ( !markerReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid Marker reader Callbacks argument!" );
    }

    markerReaderCallbacks->def_marker = defMarkerCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_MarkerReaderCallbacks_SetMarkerCallback( OTF2_MarkerReaderCallbacks*      markerReaderCallbacks,
                                              OTF2_MarkerReaderCallback_Marker markerCallback )
{
    if ( !markerReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid Marker reader Callbacks argument!" );
    }

    markerReaderCallbacks->marker = markerCallback;

    return OTF2_SUCCESS;
}


/** @brief Before any marker record could be read, the user
 *         must register a callback function for each record type. This
 *         could be done by the following function.
 *
 *  @param reader     Reader Object.
 *  @param callbacks  Callback pointer struct.
 *  @param userData   Generic pointer to handle users data inside callbacks.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */

OTF2_ErrorCode
OTF2_MarkerReader_SetCallbacks( OTF2_MarkerReader*                reader,
                                const OTF2_MarkerReaderCallbacks* callbacks,
                                void*                             userData )
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

    /* Set internal callback struct and userdata */
    memcpy( &reader->reader_callbacks,
            callbacks,
            sizeof( reader->reader_callbacks ) );
    reader->user_data = userData;

    return OTF2_SUCCESS;
}


/** @brief After callback registration, the markers could be
 *         read with the following function. The user of this
 *         function tells the system how many markers it is
 *         able to handle (recordsToRead) and the function returns
 *         how many markers where in the stream (recordsRead).
 *         It should usually be the case that both values are the
 *         same. If this is not the case, then there where less
 *         records than requested in the stream.
 *
 *  @param reader         Reader Object.
 *  @param recordsToRead  How many records have to be read next.
 *  @param recordsRead    How many records where read?
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */

OTF2_ErrorCode
OTF2_MarkerReader_ReadMarkers( OTF2_MarkerReader* reader,
                               uint64_t           recordsToRead,
                               uint64_t*          recordsRead )
{
    /* Check if this is a valid reader object */
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "No valid reader object!" );
    }

    OTF2_ErrorCode ret = OTF2_SUCCESS;
    uint64_t       read;
    for ( read = 0; read < recordsToRead; read++ )
    {
        ret = otf2_marker_reader_read( reader );
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


static inline OTF2_ErrorCode
otf2_marker_reader_read( OTF2_MarkerReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode ret;

    /* Read record type */
    uint8_t record_type;
    ret = OTF2_Buffer_Guarantee( reader->buffer, 1 );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record type." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record_type );

    switch ( record_type )
    {
        case OTF2_BUFFER_END_OF_CHUNK:
            ret = OTF2_Buffer_ReadGetNextChunk( reader->buffer );
            if ( ret != OTF2_SUCCESS )
            {
                return UTILS_ERROR( ret, "Load of next chunk failed!" );
            }
            return otf2_marker_reader_read( reader );

        case OTF2_BUFFER_END_OF_FILE:
            return OTF2_ERROR_INDEX_OUT_OF_BOUNDS;

        case OTF2_MARKER_DEF:
            return otf2_marker_reader_read_def_marker( reader );

        case OTF2_MARKER_MARKER:
            return otf2_marker_reader_read_marker( reader );

        default:
            return otf2_marker_reader_read_unknown( reader );
    }
}


/** @brief Reads a DefMarker record and triggers the callback.
 *
 *  @param reader   Reference to the reader object
 *
 *  @return OTF2_SUCCESS
 */
static inline OTF2_ErrorCode
otf2_marker_reader_read_def_marker( OTF2_MarkerReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_DefMarker static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_start_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_start_pos );

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of DefMarker record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadString( reader->buffer, &static_record.marker_group );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read markerGroup attribute of DefMarker record. Can't read string from buffer." );
    }
    ret = OTF2_Buffer_ReadString( reader->buffer, &static_record.marker_category );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read markerCategory attribute of DefMarker record. Can't read string from buffer." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.severity );

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer,
                                   record_start_pos + record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.def_marker )
    {
        interrupt = reader->reader_callbacks.def_marker( reader->user_data,
                                                         static_record.self,
                                                         static_record.marker_group,
                                                         static_record.marker_category,
                                                         static_record.severity );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


/** @brief Reads a UserMarker record and triggers the callback.
 *
 *  @param reader   Reference to the reader object
 *
 *  @return OTF2_SUCCESS
 */
static inline OTF2_ErrorCode
otf2_marker_reader_read_marker( OTF2_MarkerReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_Marker static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_start_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_start_pos );

    OTF2_Buffer_ReadUint64( reader->buffer, &static_record.timestamp );
    OTF2_Buffer_ReadUint64( reader->buffer, &static_record.duration );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.marker );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read marker attribute of Marker record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.marker_scope );
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.scope );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read scope attribute of Marker record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadString( reader->buffer, &static_record.text );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read text attribute of Marker record. Can't read string from buffer." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer,
                                   record_start_pos + record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.marker )
    {
        interrupt = reader->reader_callbacks.marker( reader->user_data,
                                                     static_record.timestamp,
                                                     static_record.duration,
                                                     static_record.marker,
                                                     static_record.marker_scope,
                                                     static_record.scope,
                                                     static_record.text );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


/** @brief Reads a marker record of unknown type and triggers the callback.
 *
 *  @param reader   Reference to the reader object
 *
 *  @return OTF2_SUCCESS
 */
static inline OTF2_ErrorCode
otf2_marker_reader_read_unknown( OTF2_MarkerReader* reader )
{
    OTF2_ErrorCode ret;

    /* Get record length and test memory availability */
    uint64_t record_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read marker record of unknown type." );
    }

    /* Skip record */
    ret = OTF2_Buffer_Skip( reader->buffer, record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not skip marker record of unknown type." );
    }

    /* Trigger callback */
    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.unknown )
    {
        interrupt = reader->reader_callbacks.unknown( reader->user_data );
    }

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}
