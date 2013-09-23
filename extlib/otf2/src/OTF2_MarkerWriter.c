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
 *  @file       src/OTF2_MarkerWriter.c
 *
 *  @brief      This layer provides write routines to store marker records of
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


OTF2_MarkerWriter*
otf2_marker_writer_new( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );

    OTF2_MarkerWriter* writer = ( OTF2_MarkerWriter* )calloc( 1, sizeof( OTF2_MarkerWriter ) );
    if ( !writer )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for writer handle!" );
        return NULL;
    }

    writer->archive = archive;

    uint64_t       evt_chunk_size;
    uint64_t       def_chunk_size;
    OTF2_ErrorCode status = otf2_archive_get_chunksize( archive,
                                                        &evt_chunk_size,
                                                        &def_chunk_size );
    if ( status != OTF2_SUCCESS )
    {
        free( writer );
        return NULL;
    }

    writer->buffer = OTF2_Buffer_New( archive,
                                      writer,
                                      def_chunk_size,
                                      OTF2_BUFFER_WRITE,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_MARKER,
                                      OTF2_UNDEFINED_LOCATION );

    if ( !writer->buffer )
    {
        UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                     "Creation of buffer handle failed!" );
        free( writer );
        return NULL;
    }

    return writer;
}


OTF2_ErrorCode
otf2_marker_writer_delete( OTF2_MarkerWriter* writerHandle )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Writer deletion failed!" );
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_BUFFER_END_OF_FILE );

    OTF2_ErrorCode ret = OTF2_Buffer_Delete( writerHandle->buffer );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Buffer deletion failed!" );
    }

    free( writerHandle );

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_MarkerWriter_WriteDefMarker( OTF2_MarkerWriter*  writerHandle,
                                  OTF2_MarkerRef      self,
                                  const char*         markerGroup,
                                  const char*         markerCategory,
                                  OTF2_MarkerSeverity severity )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode status;

    /*
     * Maximum memory usage of pure record data (excluding type ID
     * and record length information).
     */
    uint64_t record_data_length = 0;
    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes.
     */
    uint64_t record_length = 1;

    record_data_length += sizeof( OTF2_MarkerRef ) + 1;  /* self */
    record_data_length += strlen( markerGroup ) + 1;     /* markerGroup */
    record_data_length += strlen( markerCategory ) + 1;  /* markerCategory */
    record_data_length += sizeof( OTF2_MarkerSeverity ); /* severity */

    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional byte to store real record length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    status = OTF2_Buffer_WriteMemoryRequest( writerHandle->buffer, record_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_MARKER_DEF );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteString( writerHandle->buffer, markerGroup );
    OTF2_Buffer_WriteString( writerHandle->buffer, markerCategory );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, severity );

    /* Write real record length */
    return OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
}


OTF2_ErrorCode
OTF2_MarkerWriter_WriteMarker( OTF2_MarkerWriter* writerHandle,
                               uint64_t           time,
                               uint64_t           duration,
                               OTF2_MarkerRef     marker,
                               OTF2_MarkerScope   markerScope,
                               uint64_t           scope,
                               const char*        text )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode status;

    /*
     * Maximum memory usage of pure record data (excluding type ID
     * and record length information).
     */
    uint64_t record_data_length = 0;
    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes.
     */
    uint64_t record_length = 1;

    record_data_length += sizeof( uint64_t ) + 1;       /* time */
    record_data_length += sizeof( uint64_t ) + 1;       /* duration */
    record_data_length += sizeof( OTF2_MarkerRef ) + 1; /* marker */
    record_data_length += sizeof( OTF2_MarkerScope );   /* markerScope */
    record_data_length += sizeof( uint64_t ) + 1;       /* scope */
    record_data_length += strlen( text ) + 1;           /* text */

    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional byte to store real record length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    status = OTF2_Buffer_WriteMemoryRequest( writerHandle->buffer, record_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_MARKER_MARKER );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, time );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, duration );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, marker );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, markerScope );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, scope );
    OTF2_Buffer_WriteString( writerHandle->buffer, text );

    /* Write real record length */
    return OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
}
