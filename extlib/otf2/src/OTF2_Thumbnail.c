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
 * @file        src/OTF2_Thumbnail.c
 *
 * @brief       This layer provides write routines to store event records of a
 *              single location to an OTF2 memory buffer.
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

#include <UTILS_CStr.h>

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "otf2_events_int.h"

#include "OTF2_File.h"
#include "OTF2_Buffer.h"

#include "otf2_thumbnail.h"


/** @brief Create a new thumb writer.
 *
 *  @param location         The Location ID is needed to name the thumb file.
 *  @param thumbID          Thumb ID.
 *
 *  @return                 Returns a handle to the thumb writer if successful,
 *                          NULL otherwise.
 */
OTF2_ThumbWriter*
otf2_thumb_writer_new( OTF2_Archive* archive,
                       uint32_t      thumbID )
{
    UTILS_ASSERT( archive );

    OTF2_ThumbWriter* writer = NULL;

    /* Allocate new writer */
    writer = ( OTF2_ThumbWriter* )calloc( 1, sizeof( OTF2_ThumbWriter ) );
    if ( !writer )
    {
        goto clean_mem_new;
    }

    /* Allocate the buffer module */
    writer->buffer = OTF2_Buffer_New( archive,
                                      writer,
                                      OTF2_CHUNK_SIZE_THUMB,
                                      OTF2_BUFFER_WRITE,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_THUMBNAIL,
                                      thumbID );

    if ( !writer->buffer )
    {
        goto clean_mem_new;
    }

    /* Initialize the variables */
    writer->archive        = archive;
    writer->thumb_id       = thumbID;
    writer->sample_counter = 0;

    return writer;

clean_mem_new:

    free( writer );

    return NULL;
}


/** @brief
 *  Releases the resources associated with the given thumb writer handle.
 *  Does nothing if a NULL pointer is provided.
 *
 *  @param writer   Writer object which has to be deleted.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_thumb_writer_delete( OTF2_ThumbWriter* writer )
{
    if ( !writer )
    {
        return OTF2_SUCCESS;
    }
    OTF2_Buffer_WriteUint8( writer->buffer, OTF2_BUFFER_END_OF_FILE );

    OTF2_ErrorCode ret = OTF2_Buffer_Delete( writer->buffer );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( ret,
                            "Buffer deletion failed!" );
    }

    if ( writer->sample_counter != writer->number_of_samples )
    {
        ret = UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                           "Inconsistent number of written thumbnail samples: %" PRIu64 " != %" PRIu64,
                           writer->sample_counter, writer->number_of_samples );
    }
    free( writer );

    return ret;
}


/** @brief Create a new thumb reader.
 *
 *  @param location         The Location ID is needed to name the thumb file.
 *  @param thumbID          Thumb ID.
 *
 *  @return                 Returns a handle to the thumb reader if successful,
 *                          NULL otherwise.
 */
OTF2_ThumbReader*
otf2_thumb_reader_new( OTF2_Archive* archive,
                       uint8_t       thumbID )
{
    UTILS_ASSERT( archive );

    OTF2_ThumbReader* reader = NULL;

    /* Allocate new writer */
    reader = ( OTF2_ThumbReader* )calloc( 1, sizeof( OTF2_ThumbReader ) );
    if ( !reader )
    {
        goto clean_mem_new;
    }

    /* Allocate the buffer module */
    reader->buffer = OTF2_Buffer_New( archive,
                                      reader,
                                      OTF2_CHUNK_SIZE_THUMB,
                                      OTF2_BUFFER_READ,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_THUMBNAIL,
                                      thumbID );

    if ( !reader->buffer )
    {
        goto clean_mem_new;
    }

    /* Initialize the variables */
    reader->archive  = archive;
    reader->thumb_id = thumbID;

    /* read thumb header */
    OTF2_ErrorCode status = otf2_thumb_reader_read_header( reader );
    if ( status != OTF2_SUCCESS )
    {
        otf2_thumb_reader_delete( reader );
        return NULL;
    }

    return reader;

clean_mem_new:

    free( reader );

    return NULL;
}


/** @brief
 *  Releases the resources associated with the given thumb reader handle.
 *  Does nothing if a NULL pointer is provided.
 *
 *  @param writer   Reader object which has to be deleted.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_thumb_reader_delete( OTF2_ThumbReader* reader )
{
    if ( !reader )
    {
        return OTF2_SUCCESS;
    }

    OTF2_ErrorCode ret = OTF2_Buffer_Delete( reader->buffer );
    if ( ret != OTF2_SUCCESS )
    {
        UTILS_ERROR( ret, "Buffer deletion failed!" );
    }

    free( reader->name );
    free( reader->description );
    free( reader->refs_to_defs );
    free( reader );

    return ret;
}


OTF2_ErrorCode
otf2_thumb_writer_write_header( OTF2_ThumbWriter*  writerHandle,
                                const char*        name,
                                const char*        description,
                                OTF2_ThumbnailType type,
                                uint32_t           numberOfSamples,
                                uint32_t           numberOfMetrics,
                                const uint64_t*    refsToDefs )
{
    UTILS_ASSERT( writerHandle );
    UTILS_ASSERT( name );
    UTILS_ASSERT( description );
    UTILS_ASSERT( numberOfMetrics > 0 );
    UTILS_ASSERT( refsToDefs );

    OTF2_ErrorCode ret;

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

    record_data_length += strlen( name ) + 1;                           /* name */
    record_data_length += strlen( description ) + 1;                    /* description */
    record_data_length += sizeof( OTF2_ThumbnailType );                 /* type */
    record_data_length += sizeof( uint32_t ) + 1;                       /* numberOfSamples */
    record_data_length += sizeof( uint32_t ) + 1;                       /* numberOfMetrics */
    record_data_length += numberOfMetrics * ( sizeof( uint64_t ) + 1 ); /* refsToDefs */
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

    ret = OTF2_Buffer_WriteMemoryRequest( writerHandle->buffer, record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    /* Write record type */
    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_THUMB_HEADER );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteString( writerHandle->buffer, name );
    OTF2_Buffer_WriteString( writerHandle->buffer, description );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, type );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, numberOfSamples );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, numberOfMetrics );
    for ( uint32_t i = 0; i < numberOfMetrics; i++ )
    {
        OTF2_Buffer_WriteUint64( writerHandle->buffer,
                                 refsToDefs[ i ] );
    }

    /* buffer some header data to check number of written samples */
    writerHandle->number_of_samples = numberOfSamples;
    writerHandle->number_of_metrics = numberOfMetrics;

    return OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
}


OTF2_ErrorCode
OTF2_ThumbReader_GetHeader( OTF2_ThumbReader*   reader,
                            char** const        name,
                            char** const        description,
                            OTF2_ThumbnailType* type,
                            uint32_t*           numberOfSamples,
                            uint32_t*           numberOfMetrics,
                            uint64_t**          refsToDefs )
{
    /* provide already buffered header */

    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader handle." );
    }
    if ( !name || !description || !type
         || !numberOfSamples || !numberOfMetrics
         || !refsToDefs )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments." );
    }

    *name            = UTILS_CStr_dup( reader->name );
    *description     = UTILS_CStr_dup( reader->description );
    *type            = reader->type;
    *numberOfSamples = reader->number_of_samples;
    *numberOfMetrics = reader->number_of_metrics;
    *refsToDefs      = malloc( sizeof( *reader->refs_to_defs ) * reader->number_of_metrics );

    if ( !*name || !*description || !*refsToDefs )
    {
        free( *name );
        free( *description );
        free( *refsToDefs );
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Insufficient memory to fulfill request." );
    }

    memcpy( *refsToDefs, reader->refs_to_defs,
            sizeof( *reader->refs_to_defs ) * reader->number_of_metrics );

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
otf2_thumb_reader_read_header( OTF2_ThumbReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    uint8_t        record_type;
    OTF2_ErrorCode ret;
    ret = OTF2_Buffer_Guarantee( reader->buffer, 1 );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record type." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record_type );

    if ( record_type != OTF2_THUMB_HEADER )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Expected thumbnail header record." );
    }

    uint64_t record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read header record." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    const char* tmp_string;
    ret = OTF2_Buffer_ReadString( reader->buffer, &tmp_string );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    reader->name = UTILS_CStr_dup( tmp_string );
    if ( !reader->name )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't allocate memory for name." );
    }

    ret = OTF2_Buffer_ReadString( reader->buffer, &tmp_string );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    reader->description = UTILS_CStr_dup( tmp_string );
    if ( !reader->description )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't allocate memory for description." );
    }

    OTF2_Buffer_ReadUint8( reader->buffer, &reader->type );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &reader->number_of_samples );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfSamples attribute of ThumbnailHeader record. Invalid compression size." );
    }

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &reader->number_of_metrics );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfMetrics attribute of ThumbnailHeader record. Invalid compression size." );
    }

    reader->refs_to_defs = malloc( reader->number_of_metrics * sizeof( uint64_t ) );
    if ( reader->number_of_metrics > 0 && !reader->refs_to_defs )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Could not allocate memory for refsToDefs array!" );
    }
    for ( uint32_t i = 0; i < reader->number_of_metrics; i++ )
    {
        ret = OTF2_Buffer_ReadUint64( reader->buffer,
                                      reader->refs_to_defs + i );
        if ( OTF2_SUCCESS != ret )
        {
            return UTILS_ERROR( ret, "Could not read refsToDefs attribute of ThumbnailHeader record. Invalid compression size." );
        }
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
OTF2_ThumbWriter_WriteSample( OTF2_ThumbWriter* writer,
                              uint64_t          baseline,
                              uint32_t          numberOfMetrics,
                              const uint64_t*   metricSamples )
{
    if ( !writer )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writer handle." );
    }
    if ( numberOfMetrics != writer->number_of_metrics )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Number of metrics does not match header." );
    }
    if ( !metricSamples )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid metricSamples argument." );
    }

    OTF2_ErrorCode ret;

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

    record_data_length += sizeof( uint64_t ) + 1;                       /* baseline */
    record_data_length += sizeof( uint32_t ) + 1;                       /* numberOfMetrics */
    record_data_length += numberOfMetrics * ( sizeof( uint64_t ) + 1 ); /* metricSamples */
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

    ret = OTF2_Buffer_WriteMemoryRequest( writer->buffer, record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writer->buffer, OTF2_THUMB_SAMPLE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writer->buffer, record_data_length );

    OTF2_Buffer_WriteUint64( writer->buffer, baseline );
    OTF2_Buffer_WriteUint32( writer->buffer, numberOfMetrics );
    for ( uint32_t i = 0; i < numberOfMetrics; i++ )
    {
        OTF2_Buffer_WriteUint64( writer->buffer, metricSamples[ i ] );
    }

    writer->sample_counter++;

    /* Write real record length */
    return OTF2_Buffer_WriteFinalRecordLength( writer->buffer, record_data_length );
}


static OTF2_ErrorCode
otf2_thumb_reader_read_sample( OTF2_ThumbReader* reader,
                               uint64_t*         baseline,
                               uint32_t          numberOfMetrics,
                               uint64_t*         metricSamples )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );
    UTILS_ASSERT( baseline );
    UTILS_ASSERT( numberOfMetrics );
    UTILS_ASSERT( metricSamples );

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint64( reader->buffer, baseline );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read baseline attribute of ThumbnailSample record. Invalid compression size." );
    }
    uint32_t number_of_metrics;
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &number_of_metrics );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfMetrics attribute of ThumbnailSample record. Invalid compression size." );
    }
    if ( number_of_metrics != numberOfMetrics )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Number of metrics in sample do not match with header." );
    }
    for ( uint64_t i = 0; i < number_of_metrics; i++ )
    {
        ret = OTF2_Buffer_ReadUint64( reader->buffer, metricSamples + i );
        if ( OTF2_SUCCESS != ret )
        {
            return UTILS_ERROR( ret, "Could not read metricSamples attribute of ThumbnailSample record. Invalid compression size." );
        }
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
OTF2_ThumbReader_ReadSample( OTF2_ThumbReader* reader,
                             uint64_t*         baseline,
                             uint32_t          numberOfMetrics,
                             uint64_t*         metricSamples )
{
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writer handle." );
    }
    if ( !baseline )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid baseline argument." );
    }
    if ( numberOfMetrics != reader->number_of_metrics )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid number of metrics." );
    }
    if ( !metricSamples )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid metricSamples argument." );
    }

    /* Read record type */
    uint8_t        record_type = OTF2_BUFFER_END_OF_CHUNK;
    OTF2_ErrorCode ret;
    /* Do this loop at most twice. */
    while ( record_type == OTF2_BUFFER_END_OF_CHUNK )
    {
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
                if ( OTF2_SUCCESS != ret )
                {
                    return UTILS_ERROR( ret, "Load of next chunk failed!" );
                }
                /* Restart with new chunk. */
                continue;

            case OTF2_BUFFER_END_OF_FILE:
                /* This is the end of file, which could mean the user called us
                   more than numberOfSamples, or there are not enough samples
                   in the thumbnail. Both are errors. */
                return OTF2_ERROR_INDEX_OUT_OF_BOUNDS;

            case OTF2_THUMB_SAMPLE:
                return otf2_thumb_reader_read_sample( reader,
                                                      baseline,
                                                      numberOfMetrics,
                                                      metricSamples );

            default:
                break;
        }
    }

    return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT, "Couldn't read thumbnail!" );
}
