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

/** @internal
 *
 *  @file       src/OTF2_Buffer.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer is completely internal to the OTF2 library and should
 *              not be exposed to the end user. Its purpose is to abstract the
 *              handling of a memory buffer and to provide basic routines
 *              encapsulating the encoding and decoding of elementary data
 *              types. This layer will even be used when directly reading or
 *              writing from/to a file, since a certain amount of buffering is
 *              required to achieve a reasonable performance by reducing the
 *              number of I/O function calls as well as increasing the size of
 *              the data block to be read or written.
 *
 *
 *
 *  @section buffer_usage_write How to use the memory buffer for writing
 *
 *  To implement correct writing routines for event or definition records you
 *  have to stick to the following pattern:
 *
 *  (1) Call the timestamp writing routine. It internally decides whether a new
 *  timestamp has to be written or not. It also ensures correct chunking and
 *  overflow management.
 *  @code
 *  OTF2_Buffer_WriteTimeStamp( bufferHandle, time );
 *  @endcode
 *
 *  (2) Write the type of the record.
 *  @code
 *  OTF2_Buffer_WriteUint8( bufferHandle, OTF2_EVENT_ENTER );
 *  @endcode
 *
 *  (3) Write the values according to the record type.
 *  @code
 *  OTF2_Buffer_WriteUint32( bufferHandle, regionID );
 *  @endcode
 *
 *
 *
 *  @section buffer_usage_read How to use the memory buffer for reading
 *
 *  To implement correct reading routines for event or definition records you
 *  have to stick to the following pattern:
 *
 *  (1) Call the timestamp reading routine. It checks first if the end of
 *  written data in the buffer is reached and handles this event (Please note:
 *  This is not implemented now, it just returns an error). After this it checks
 *  if the record is a timestamp. If so the timestamp is updated. It always
 *  delivers an actual timestamp in time.
 *  @code
 *  OTF2_Buffer_ReadTimeStamp( bufferHandle, time );
 *  @endcode
 *
 *  (2) Read the type of the record.
 *  @code
 *  OTF2_Buffer_ReadUint8( bufferHandle, recordType );
 *  @endcode
 *
 *  (3) Decide by record type which individual read routines are neccessary and
 *  call them
 *  @code
 *  OTF2_Buffer_ReadUint32( bufferHandle, regionID );
 *  @endcode
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

#include "otf2_local_definitions.h"
#include "otf2_global_definitions.h"
#include "otf2_events_int.h"
#include "otf2_snaps.h"


#include "OTF2_File.h"

#include "OTF2_Buffer.h"


/* ___ Macros _______________________________________________________________ */


/** @brief Size of a chunk header in bytes.
 *  This is neccessary to jump right behind the chunk header.
 *
 *  Please note: This macro directly depends on the implementation of
 *  otf2_buffer_write_header. So if you make any changes to that function
 *  you have to addapt this value accordingly.
 */
#define OTF2_CHUNK_HEADER_SIZE ( uint64_t )18

/** @brief Position of the last event information within the chunk header.
 *  This is neccessary to write the number of the last event in a chunk when the
 *  chunk is completed.
 *
 *  Please note: This macro directly depends on the implementation of
 *  otf2_buffer_write_header. So if you make any changes to that function
 *  you have to addapt this value accordingly.
 */
#define OTF2_CHUNK_HEADER_LAST_EVENT ( uint64_t )10


/** @brief Defines the maximum size that can be allocated by the internal
 *  allocator. */
#define OTF2_ALLOCATE_SIZE_MAX ( uint64_t )( 1024 * 1024 * 128 )

#ifdef WORDS_BIGENDIAN
#define OTF2_HOST_ENDIANNESS OTF2_BIG_ENDIAN
#else
#define OTF2_HOST_ENDIANNESS OTF2_LITTLE_ENDIAN
#endif


/* ___ Prototypes for internal functions. ___________________________________ */

static inline void
otf2_buffer_write_header( OTF2_Buffer* bufferHandle );

static inline OTF2_ErrorCode
otf2_buffer_read_chunk( OTF2_Buffer*  bufferHandle,
                        OTF2_FilePart filePart );

static inline OTF2_ErrorCode
otf2_buffer_read_header( OTF2_Buffer* bufferHandle );

static OTF2_ErrorCode
otf2_buffer_clear_rewind_list( OTF2_Buffer* bufferHandle );

static OTF2_ErrorCode
otf2_buffer_get_file_handle( OTF2_Buffer* bufferHandle );

static OTF2_ErrorCode
otf2_buffer_load_header( OTF2_Buffer* bufferHandle );

static OTF2_ErrorCode
otf2_buffer_load_events( OTF2_Buffer* bufferHandle );

static OTF2_ErrorCode
otf2_buffer_load_head_n_time( OTF2_Buffer* bufferHandle );

static inline void*
otf2_buffer_memory_allocate( OTF2_Buffer* buffer );

void
otf2_buffer_memory_free( OTF2_Buffer* buffer,
                         bool         final );

static inline void
otf2_buffer_free_all_chunks( OTF2_Buffer* bufferHandle );

static inline uint16_t
otf2_swap16( uint16_t v16 );

static inline uint32_t
otf2_swap32( uint32_t v32 );

static inline uint64_t
otf2_swap64( uint64_t v64 );

static inline void
otf2_buffer_finalize( OTF2_Buffer* bufferHandle );



/* ___ Interactions with the buffer handle. _________________________________ */



/** @brief Create a new empty memory buffer.
 *
 *  Creates a new (empty) memory buffer.
 *
 *  @param chunkSize        Size of the internal chunks in bytes.
 *  @param bufferMode       Defines the mode of interaction with the buffer.
 *                          Available values are: 'OTF2_BUFFER_WRITE',
 *                          'OTF2_BUFFER_MODIFY' and 'OTF2_BUFFER_READ'.
 *  @param chunkMode        Defines if a buffer is chunked or not. Buffers
 *                          keeping event records are always chunked, buffers
 *                          keeping definitions are always not chunked.
 *                          Available values are: 'OTF2_BUFFER_NOT_CHUNKED'
 *                          and 'OTF2_BUFFER_CHUNKED'.
 *
 *  @return                 Returns a handle to the buffer if successful, NULL
 *                          otherwise.
 */
OTF2_Buffer*
OTF2_Buffer_New( OTF2_Archive*    archive,
                 void*            operator,
                 uint64_t         chunkSize,
                 OTF2_BufferMode  bufferMode,
                 OTF2_ChunkMode   chunkMode,
                 OTF2_FileType    fileType,
                 OTF2_LocationRef location )
{
    UTILS_ASSERT( archive );

    otf2_chunk*    chunk         = NULL;
    OTF2_Buffer*   buffer_handle = NULL;
    OTF2_ErrorCode status        = OTF2_ERROR_INVALID;

    /* Check if passed enum values are valid. */
    if ( bufferMode != OTF2_BUFFER_WRITE && bufferMode != OTF2_BUFFER_READ &&
         bufferMode != OTF2_BUFFER_MODIFY )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid buffer mode!" );
        return NULL;
    }

    if ( chunkMode != OTF2_BUFFER_NOT_CHUNKED &&
         chunkMode != OTF2_BUFFER_CHUNKED )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid chunk mode!" );
        return NULL;
    }

    if ( fileType != OTF2_FILETYPE_ANCHOR &&
         fileType != OTF2_FILETYPE_GLOBAL_DEFS &&
         fileType != OTF2_FILETYPE_LOCAL_DEFS &&
         fileType != OTF2_FILETYPE_EVENTS &&
         fileType != OTF2_FILETYPE_SNAPSHOTS &&
         fileType != OTF2_FILETYPE_THUMBNAIL &&
         fileType != OTF2_FILETYPE_MARKER )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid file type!" );
        return NULL;
    }

    /*
     * Files which may need a location id can set them later.
     * A location id for the other file types does not make sense.
     *
       if ( !otf2_file_type_needs_location_id( fileType )
         && location != OTF2_UNDEFINED_LOCATION )
       {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid file type location id combination!" );
        return NULL;
       }*/

    /* Check range of chunk size. */
    if ( chunkSize < OTF2_CHUNK_SIZE_MIN || chunkSize > OTF2_CHUNK_SIZE_MAX )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid chunk size!" );
        return NULL;
    }

    /* Allocate memory for handle and validate. */
    buffer_handle = ( OTF2_Buffer* )calloc( 1, sizeof( OTF2_Buffer ) );
    if ( NULL == buffer_handle )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for buffer handle!" );
        return NULL;
    }

    /* Initialize buffer handle data. */
    buffer_handle->archive    = archive;
    buffer_handle->operator   = operator;
    buffer_handle->chunk_size = chunkSize;
    buffer_handle->memory     = OTF2_ALLOCATE_SIZE_MAX;
    /* This always has to be set to OTF2_BUFFER_WRITE at first. Otherwise the
     * OTF2_Buffer_SwitchMode call does not switch the mode correctly. */
    buffer_handle->buffer_mode = OTF2_BUFFER_WRITE;
    buffer_handle->chunk_mode  = chunkMode;

    buffer_handle->file_type   = fileType;
    buffer_handle->location_id = location;

    buffer_handle->time      = 0;
    buffer_handle->time_pos  = NULL;
    buffer_handle->finalized = false;

    /* Set default endianness mode to that of the host,
       the chunk header controls this */
    buffer_handle->endianness_mode = OTF2_HOST_ENDIANNESS;

    buffer_handle->file = NULL;

    /* Allocate memory for a chunk handle and validate. */
    chunk = ( otf2_chunk* )calloc( 1, sizeof( otf2_chunk ) );
    if ( NULL == chunk )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for internal handle!" );
        free( buffer_handle );
        return NULL;
    }

    /* Initialize chunk handle data.
     * In reading mode un-chunked buffers are allocated in
     * otf2_buffer_read_chunk(). */
    chunk->begin = NULL;
    if ( bufferMode == OTF2_BUFFER_WRITE || chunkMode == OTF2_BUFFER_CHUNKED )
    {
        chunk->begin = ( uint8_t* )otf2_buffer_memory_allocate( buffer_handle );
        if ( NULL == chunk->begin )
        {
            UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                         "Could not allocate memory for chunk!" );
            free( chunk );
            free( buffer_handle );
            return NULL;
        }

        chunk->end = chunk->begin + chunkSize;
    }
    chunk->chunk_num   = 1;
    chunk->first_event = 0;
    chunk->last_event  = 0;
    chunk->prev        = NULL;
    chunk->next        = NULL;

    buffer_handle->chunk      = chunk;
    buffer_handle->chunk_list = chunk;

    buffer_handle->write_pos = buffer_handle->chunk->begin;
    buffer_handle->read_pos  = NULL;

    /* Initialize pointer to the rewind list */
    buffer_handle->rewindlist = NULL;

    /* Initialize pointer to the old_chunk_list */
    buffer_handle->old_chunk_list = NULL;

    /* Check if a buffer in modify or read mode is requested and switch mode. */
    if ( bufferMode == OTF2_BUFFER_MODIFY || bufferMode == OTF2_BUFFER_READ )
    {
        OTF2_Buffer_SwitchMode( buffer_handle, bufferMode );
    }

    /* Check if buffer is in writing mode and write headers. */
    if ( bufferMode == OTF2_BUFFER_WRITE )
    {
        otf2_buffer_write_header( buffer_handle );
    }

    /* Check if buffer is in reading mode and load data from file. */
    if ( bufferMode == OTF2_BUFFER_MODIFY || bufferMode == OTF2_BUFFER_READ )
    {
        status = otf2_buffer_read_chunk( buffer_handle, OTF2_FILEPART_NEXT );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status, "Could not read data from file to buffer!" );
            otf2_buffer_memory_free( buffer_handle, true );
            free( chunk );
            free( buffer_handle );
            return NULL;
        }
        status = otf2_buffer_read_header( buffer_handle );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status, "Read of chunk header failed!" );
            otf2_buffer_memory_free( buffer_handle, true );
            free( chunk );
            free( buffer_handle );
            return NULL;
        }
    }

    return buffer_handle;
}


/** @brief Delete a memory buffer handle.
 *
 *  Releases the resources associated with the given buffer handle. If in
 *  writing mode it flushes the memory buffer to a file according to the
 *  selected file substrate.
 *  Does nothing if a NULL pointer is provided.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_Delete( OTF2_Buffer* bufferHandle )
{
    OTF2_ErrorCode status_flush = OTF2_SUCCESS;
    OTF2_ErrorCode status_file  = OTF2_SUCCESS;

    /* Validate arguments. */
    if ( NULL == bufferHandle )
    {
        return OTF2_SUCCESS;
    }

    /* Call flush routine if in writing mode.
     * If an error occurs print error message but continue anyway. */
    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        otf2_buffer_finalize( bufferHandle );

        status_flush = OTF2_Buffer_FlushBuffer( bufferHandle );
        if ( status_flush != OTF2_SUCCESS )
        {
            UTILS_ERROR( status_flush,
                         "Buffer was not flushed correctly!" );
        }
    }

    /* Close file handle.
     * If an error occurs print error message but continue anyway. */
    if ( NULL != bufferHandle->file )
    {
        status_file = OTF2_File_Close( bufferHandle->file );
        if ( status_file != OTF2_SUCCESS )
        {
            UTILS_ERROR( status_file,
                         "File was not closed correctly!" );
        }
    }

    /* Free allocated resources. */
    otf2_buffer_memory_free( bufferHandle, true );
    otf2_buffer_free_all_chunks( bufferHandle );

    free( bufferHandle );

    if ( status_flush != OTF2_SUCCESS || status_file != OTF2_SUCCESS )
    {
        return UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                            "Not all operations were handled correctly!" );
    }

    return OTF2_SUCCESS;
}


/** @brief Switch memory buffer mode.
 *
 *  Changes the mode of interaction with the memory buffer. There is a defined
 *  workflow of interaction modes. The following state transitions are allowed:
 *  From 'write' to 'read' or 'modify' and from 'modify' to 'read'.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param bufferMode       Defines the mode of interaction with the buffer.
 *                          Available values are: 'OTF2_BUFFER_WRITE',
 *                          'OTF2_BUFFER_MODIFY' and 'OTF2_BUFFER_READ'.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_SwitchMode( OTF2_Buffer*    bufferHandle,
                        OTF2_BufferMode bufferMode )
{
    UTILS_ASSERT( bufferHandle );

    /* Check if enum values are valid. */
    if ( bufferMode != OTF2_BUFFER_WRITE && bufferMode != OTF2_BUFFER_READ &&
         bufferMode != OTF2_BUFFER_MODIFY )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid value for the buffer mode!" );
    }


    /* If in writing mode change to all other modes. */
    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        if ( bufferMode == OTF2_BUFFER_MODIFY )
        {
            /* Set all remaining bytes in the chunk to OTF2_BUFFER_END_OF_CHUNK. */
            memset( bufferHandle->write_pos, ( char )OTF2_BUFFER_END_OF_CHUNK,
                    bufferHandle->chunk->end - bufferHandle->write_pos );

            bufferHandle->write_pos   = bufferHandle->chunk_list->begin;
            bufferHandle->read_pos    = bufferHandle->write_pos;
            bufferHandle->buffer_mode = bufferMode;

            return OTF2_SUCCESS;
        }
        if ( bufferMode == OTF2_BUFFER_READ )
        {
            /* write_pos may be NULL, when opening a new buffer in READ mode,
             * because the size of the chunk isn't known yet
             */
            if ( bufferHandle->write_pos != NULL )
            {
                /* Set all remaining bytes in the chunk to OTF2_BUFFER_END_OF_CHUNK. */
                memset( bufferHandle->write_pos, ( char )OTF2_BUFFER_END_OF_CHUNK,
                        bufferHandle->chunk->end - bufferHandle->write_pos );
            }

            bufferHandle->write_pos   = NULL;
            bufferHandle->read_pos    = bufferHandle->chunk_list->begin;
            bufferHandle->buffer_mode = bufferMode;

            return OTF2_SUCCESS;
        }

        return OTF2_SUCCESS;
    }

    /* If in modify mode only change to read mode. */
    if ( bufferHandle->buffer_mode == OTF2_BUFFER_MODIFY )
    {
        if ( bufferMode == OTF2_BUFFER_READ )
        {
            bufferHandle->write_pos   = NULL;
            bufferHandle->read_pos    = bufferHandle->chunk_list->begin;
            bufferHandle->buffer_mode = bufferMode;

            return OTF2_SUCCESS;
        }
        if ( bufferMode == OTF2_BUFFER_WRITE )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                                "It is not possible to switch from modify to writing mode!" );
        }

        return OTF2_SUCCESS;
    }

    /* If in read mode only succeed if requested mode is also reading mode. */
    if ( bufferHandle->buffer_mode == OTF2_BUFFER_READ )
    {
        if ( bufferMode == OTF2_BUFFER_READ )
        {
            return OTF2_SUCCESS;
        }

        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "It is not allowed to switch from read mode to another mode!" );
    }

    return UTILS_ERROR( OTF2_ERROR_END_OF_FUNCTION,
                        "Fatal error! A valid call was not processed!" );
}


/** @brief Set the file path.
 *
 *  Sets the file path related to a buffer handle. The file path has to be set
 *  with this function before any data could be written to or read from a file.
 *  The file path could only be set once e.g. only if the buffer handle was
 *  created without a file name.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param location         ID of the location.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_SetLocationID( OTF2_Buffer*     bufferHandle,
                           OTF2_LocationRef location )
{
    UTILS_ASSERT( bufferHandle );

    /* Setting the location id makes only sense for event and local def files. */
    if ( !otf2_file_type_needs_location_id( bufferHandle->file_type ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Can't set location id for this file type!" );
    }

    if ( OTF2_UNDEFINED_LOCATION == location )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid location!" );
    }

    /* Check if call is allowed in currect state. */
    if ( NULL != bufferHandle->file )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "There is already a open file handle!" );
    }

    bufferHandle->location_id = location;

    return OTF2_SUCCESS;
}


/* ___ Write operations for essential data types. ___________________________ */



/** @brief Requests a new chunk for new events.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param time             Current timestamp.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_RequestNewChunk( OTF2_Buffer*   bufferHandle,
                             OTF2_TimeStamp time )
{
    UTILS_ASSERT( bufferHandle );

    OTF2_ErrorCode status;

    /* Write number of the last event in the chunk header. */
    memcpy( bufferHandle->chunk->begin + OTF2_CHUNK_HEADER_LAST_EVENT,
            &( bufferHandle->chunk->first_event ), 8 );

    /* Set all remaining bytes in the chunk to OTF2_BUFFER_END_OF_CHUNK. */
    memset( bufferHandle->write_pos, ( char )OTF2_BUFFER_END_OF_CHUNK,
            bufferHandle->chunk->end - bufferHandle->write_pos );

    /* Use an old chunk if available */
    otf2_chunk* new_chunk = bufferHandle->old_chunk_list;

    /* Update old_chunk_list */
    if ( new_chunk )
    {
        bufferHandle->old_chunk_list = new_chunk->next;
    }
    else
    /* Allocate memory for a new chunk handle and validate. */
    {
        new_chunk = ( otf2_chunk* )calloc( 1, sizeof( otf2_chunk ) );

        if ( NULL == new_chunk )
        {
            return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                                "Could not allocate memory for internal handle!" );
        }

        new_chunk->begin = otf2_buffer_memory_allocate( bufferHandle );
    }


    if ( NULL != new_chunk->begin )
    {
        /* Initialize new chunk handle data. */
        new_chunk->end         = new_chunk->begin + bufferHandle->chunk_size;
        new_chunk->chunk_num   = bufferHandle->chunk->chunk_num + 1;
        new_chunk->first_event = bufferHandle->chunk->first_event;
        new_chunk->last_event  = 0;
        new_chunk->prev        = bufferHandle->chunk;
        new_chunk->next        = NULL;

        /* Update buffer handle data. */
        bufferHandle->chunk->next = new_chunk;
        bufferHandle->chunk       = new_chunk;
        bufferHandle->write_pos   = bufferHandle->chunk->begin;

        /* Write chunk header. */
        otf2_buffer_write_header( bufferHandle );

        /* Write timestamp and update time. */
        if ( otf2_file_type_has_timestamps( bufferHandle->file_type ) )
        {
            OTF2_Buffer_WriteUint8( bufferHandle, OTF2_BUFFER_TIMESTAMP );
            OTF2_Buffer_WriteUint64Full( bufferHandle, time );
            bufferHandle->time = time;
        }
    }
    else
    {
        /* Save the number of events. */
        uint64_t first_event = bufferHandle->chunk->first_event;

        /* Call the buffer flush routine. */
        status = OTF2_Buffer_FlushBuffer( bufferHandle );

        if ( status != OTF2_SUCCESS )
        {
            free( new_chunk );
            return UTILS_ERROR( status,
                                "Buffer flush failed!" );
        }

        /* Initialize new chunk handle data. */
        new_chunk->begin = otf2_buffer_memory_allocate( bufferHandle );
        if ( NULL == new_chunk->begin )
        {
            free( new_chunk );
            return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                                "Failed to allocate memory for chunk!" );
        }
        new_chunk->end         = new_chunk->begin + bufferHandle->chunk_size;
        new_chunk->chunk_num   = 1;
        new_chunk->first_event = first_event;
        new_chunk->last_event  = 0;
        new_chunk->prev        = NULL;
        new_chunk->next        = NULL;

        /* Update buffer handle data. */
        bufferHandle->chunk      = new_chunk;
        bufferHandle->chunk_list = new_chunk;
        bufferHandle->write_pos  = bufferHandle->chunk->begin;

        /* Write chunk header. */
        otf2_buffer_write_header( bufferHandle );

        /* Write timestamp and update time. */
        if ( otf2_file_type_has_timestamps( bufferHandle->file_type ) )
        {
            OTF2_Buffer_WriteUint8( bufferHandle, OTF2_BUFFER_TIMESTAMP );
            OTF2_Buffer_WriteUint64Full( bufferHandle, time );
            bufferHandle->time = time;

            if ( bufferHandle->archive->flush_callbacks->otf2_post_flush != NULL )
            {
                /* Get a timestamp from the user and write flush record. */
                OTF2_TimeStamp flush_time = bufferHandle->archive->flush_callbacks
                                            ->otf2_post_flush( bufferHandle->archive->flush_data,
                                                               bufferHandle->file_type,
                                                               bufferHandle->location_id );

                /* Write event type */
                OTF2_Buffer_WriteUint8( bufferHandle, OTF2_EVENT_BUFFER_FLUSH );
                /* Write record length (in bytes) */
                OTF2_Buffer_WriteUint8( bufferHandle, 8 );
                /* Write record data */
                OTF2_Buffer_WriteUint64Full( bufferHandle, flush_time );

                /* count the BUFFER_FLUSH as an event */
                bufferHandle->chunk->first_event++;
            }
        }
    }

    return OTF2_SUCCESS;
}



/** @brief Write chunk header.
 *
 *  Writes the chunk header with the number of the first event in the current
 *  chunk and reserves space for the number of the last event.
 *
 *  Please note: The macros OTF2_CHUNK_HEADER_SIZE and
 *  OTF2_CHUNK_HEADER_LAST_EVENT directly depend on the implementation of this
 *  function. So if you make any changes to this function you have to addapt
 *  these macros accordingly.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 */
static inline void
otf2_buffer_write_header( OTF2_Buffer* bufferHandle )
{
    UTILS_ASSERT( bufferHandle );

    OTF2_Buffer_WriteUint8( bufferHandle, OTF2_BUFFER_CHUNK_HEADER );
    OTF2_Buffer_WriteUint8( bufferHandle, OTF2_HOST_ENDIANNESS );

    /* In chunked mode we write also the first and last event number */
    if ( bufferHandle->chunk_mode == OTF2_BUFFER_CHUNKED )
    {
        OTF2_Buffer_WriteUint64Full( bufferHandle, bufferHandle->chunk->first_event + 1 );
        OTF2_Buffer_WriteUint64Full( bufferHandle, 0 );
    }
}



/** @brief Initiate an immediate buffer flush.
 *
 *  If the buffer is full the choosen buffer overflow method is automatically
 *  triggered. Sometimes it is desired to trigger this method at a special point
 *  within the program flow. This is also done with this call.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_FlushBuffer( OTF2_Buffer* bufferHandle )
{
    UTILS_ASSERT( bufferHandle );

    OTF2_FlushType flush  = OTF2_FLUSH;
    OTF2_ErrorCode status = OTF2_SUCCESS;

    /* Only for event writers is the default to not flush */
    if ( bufferHandle->file_type == OTF2_FILETYPE_EVENTS )
    {
        flush = OTF2_NO_FLUSH;
    }

    /* Remove all previous stored rewind points from rewind list. */
    otf2_buffer_clear_rewind_list( bufferHandle );

    /* Get flush information from pre flush callback. */
    if ( bufferHandle->archive->flush_callbacks
         && bufferHandle->archive->flush_callbacks->otf2_pre_flush )
    {
        flush = bufferHandle->archive->flush_callbacks->otf2_pre_flush(
            bufferHandle->archive->flush_data,
            bufferHandle->file_type,
            bufferHandle->location_id,
            bufferHandle->operator,
            bufferHandle->finalized );
    }

    if ( flush == OTF2_NO_FLUSH )
    {
        return OTF2_SUCCESS;
    }

    /* If there is no file handle open one. */
    if ( bufferHandle->file == NULL )
    {
        /* Check if we need and have a location. */
        if ( otf2_file_type_needs_location_id( bufferHandle->file_type )
             && bufferHandle->location_id == OTF2_UNDEFINED_LOCATION )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                                "Could not open file. No location id given." );
        }

        bufferHandle->file =
            OTF2_File_Open( bufferHandle->archive,
                            bufferHandle->buffer_mode == OTF2_BUFFER_WRITE  ? OTF2_FILEMODE_WRITE :
                            bufferHandle->buffer_mode == OTF2_BUFFER_MODIFY ? OTF2_FILEMODE_MODIFY :
                            OTF2_FILEMODE_READ,
                            bufferHandle->file_type,
                            bufferHandle->location_id );
        if ( NULL == bufferHandle->file )
        {
            return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                                "Could not open file." );
        }
    }

    /* If a buffer is chunked write all previous chunks and complete current
     * chunk. Otherwise just write the used part.  */
    if ( bufferHandle->chunk_mode == OTF2_BUFFER_CHUNKED )
    {
        /* Write number of the last event in the chunk header. */
        memcpy( bufferHandle->chunk->begin + OTF2_CHUNK_HEADER_LAST_EVENT,
                &( bufferHandle->chunk->first_event ), 8 );

        /* Set all remaining bytes in the chunk to OTF2_BUFFER_END_OF_CHUNK. */
        memset( bufferHandle->write_pos, ( char )OTF2_BUFFER_END_OF_CHUNK,
                bufferHandle->chunk->end - bufferHandle->write_pos );


        /* Flush the buffer by looping over all chunks. */
        otf2_chunk* chunk = bufferHandle->chunk_list;

        status = OTF2_SUCCESS;
        while ( status == OTF2_SUCCESS && chunk != NULL )
        {
            /* If it is the last chunk of this location, write only the actually
             * used part to file. */
            if ( bufferHandle->finalized && chunk->next == NULL )
            {
                status = OTF2_File_Write( bufferHandle->file,
                                          chunk->begin,
                                          bufferHandle->write_pos
                                          - bufferHandle->chunk->begin );
            }
            else
            {
                status = OTF2_File_Write( bufferHandle->file,
                                          chunk->begin,
                                          bufferHandle->chunk_size );
            }

            chunk = chunk->next;
        }
    }
    else
    {
        /* Set an OTF2_BUFFER_END_OF_CHUNK byte. */
        memset( bufferHandle->write_pos, ( char )OTF2_BUFFER_END_OF_CHUNK, 1 );
        bufferHandle->write_pos++;

        status = OTF2_File_Write( bufferHandle->file,
                                  bufferHandle->chunk->begin,
                                  bufferHandle->write_pos
                                  - bufferHandle->chunk->begin );
    }
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed to write to the file!" );
    }

    /* Free all allocated chunks. */
    otf2_buffer_memory_free( bufferHandle, false );
    otf2_buffer_free_all_chunks( bufferHandle );

    return OTF2_SUCCESS;
}



/* ___ Read operations for essential data types. ____________________________ */



/** @brief Read a timestamp if neccesary.
 *
 *  It checks first if the end of written data in the buffer is reached and
 *  handles this event (Please note: This is not implemented now, it just
 *  returns an error). After this it checks if the record is a timestamp. If so
 *  the timestamp is updated. It always delivers an actual timestamp in time.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param time             Pointer to the time that should be updated.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_ReadTimeStamp( OTF2_Buffer*    bufferHandle,
                           OTF2_TimeStamp* time )
{
    UTILS_ASSERT( bufferHandle );

    /* Check if end of buffer is reached. */
    if ( bufferHandle->read_pos >= bufferHandle->chunk->end )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "Tried to read after end of internal chunk !" );
    }

    /* Check if the next record is a timestamp. */
    if ( OTF2_BUFFER_TIMESTAMP == *bufferHandle->read_pos )
    {
        bufferHandle->read_pos++;
        OTF2_Buffer_ReadUint64Full( bufferHandle, &( bufferHandle->time ) );
        bufferHandle->time_pos = bufferHandle->read_pos
                                 - OTF2_BUFFER_TIMESTAMP_SIZE;
    }

    *time = bufferHandle->time;

    return OTF2_SUCCESS;
}


/** @name Reading routines for basic data types.
 *
 *  Read and decode basic data types from the buffer. Functions ending with
 *  'Full' are reading uncompressed values. The uncompressed functions call
 *  should be used for values that are per default not compressed e.g.
 *  timestamps.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param returnValue      Pointer to the read value.
 *
 *  @{
 */

void
OTF2_Buffer_ReadUint8( OTF2_Buffer* bufferHandle,
                       uint8_t*     returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    memcpy( returnValue, bufferHandle->read_pos, 1 );
    bufferHandle->read_pos++;
}


void
OTF2_Buffer_ReadInt8( OTF2_Buffer* bufferHandle,
                      int8_t*      returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    memcpy( returnValue, bufferHandle->read_pos, 1 );
    bufferHandle->read_pos++;
}


void
OTF2_Buffer_ReadUint16( OTF2_Buffer* bufferHandle,
                        uint16_t*    returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    memcpy( returnValue, bufferHandle->read_pos, 2 );
    bufferHandle->read_pos += 2;

    if ( bufferHandle->endianness_mode != OTF2_HOST_ENDIANNESS )
    {
        *returnValue = otf2_swap16( *returnValue );
    }
}


void
OTF2_Buffer_ReadInt16( OTF2_Buffer* bufferHandle,
                       int16_t*     returnValue )
{
    union
    {
        uint16_t u16;
        int16_t  s16;
    } u;

    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    OTF2_Buffer_ReadUint16( bufferHandle, &u.u16 );
    *returnValue = u.s16;
}


OTF2_ErrorCode
OTF2_Buffer_ReadUint32( OTF2_Buffer* bufferHandle,
                        uint32_t*    returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    uint8_t size = *bufferHandle->read_pos++;

    if ( size == 0 )
    {
        *returnValue = 0;
        return OTF2_SUCCESS;
    }

    if ( size == 0xff )
    {
        *returnValue = UINT32_MAX;
        return OTF2_SUCCESS;
    }

    if ( size > 4 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Invalid size in compressed length byte." );
    }

    /* Initialize all bytes with zero. */
    *returnValue = ( uint32_t )0;

    uint8_t* write_pos = ( uint8_t* )returnValue;
    if ( bufferHandle->endianness_mode == OTF2_BIG_ENDIAN )
    {
        /* Store value in last bytes when big endian. */
        write_pos += 4 - size;
    }

    memcpy( write_pos, bufferHandle->read_pos, size );

    bufferHandle->read_pos += size;

    if ( bufferHandle->endianness_mode != OTF2_HOST_ENDIANNESS )
    {
        *returnValue = otf2_swap32( *returnValue );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Buffer_ReadInt32( OTF2_Buffer* bufferHandle,
                       int32_t*     returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    union
    {
        uint32_t u32;
        int32_t  s32;
    } u;

    OTF2_ErrorCode status = OTF2_Buffer_ReadUint32( bufferHandle, &u.u32 );
    if ( status != OTF2_SUCCESS )
    {
        return status;
    }
    *returnValue = u.s32;

    return OTF2_SUCCESS;
}


void
OTF2_Buffer_ReadUint32Full( OTF2_Buffer* bufferHandle,
                            uint32_t*    returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    memcpy( returnValue, bufferHandle->read_pos, 4 );
    bufferHandle->read_pos += 4;

    if ( bufferHandle->endianness_mode != OTF2_HOST_ENDIANNESS )
    {
        *returnValue = otf2_swap32( *returnValue );
    }
}


void
OTF2_Buffer_ReadInt32Full( OTF2_Buffer* bufferHandle,
                           int32_t*     returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    union
    {
        uint32_t u32;
        int32_t  s32;
    } u;

    OTF2_Buffer_ReadUint32Full( bufferHandle, &u.u32 );
    *returnValue = u.s32;
}


OTF2_ErrorCode
OTF2_Buffer_ReadUint64( OTF2_Buffer* bufferHandle,
                        uint64_t*    returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    uint8_t size = *bufferHandle->read_pos++;

    if ( size == 0 )
    {
        *returnValue = 0;
        return OTF2_SUCCESS;
    }

    if ( size == 0xff )
    {
        *returnValue = UINT64_MAX;
        return OTF2_SUCCESS;
    }

    if ( size > 8 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Invalid size in compressed length byte." );
    }

    /* Initialize all bytes with zero. */
    *returnValue = ( uint64_t )0;

    uint8_t* write_pos = ( uint8_t* )returnValue;
    if ( bufferHandle->endianness_mode == OTF2_BIG_ENDIAN )
    {
        /* Store value in last bytes when big endian. */
        write_pos += 8 - size;
    }

    memcpy( write_pos, bufferHandle->read_pos, size );

    bufferHandle->read_pos += size;

    if ( bufferHandle->endianness_mode != OTF2_HOST_ENDIANNESS )
    {
        *returnValue = otf2_swap64( *returnValue );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Buffer_ReadInt64( OTF2_Buffer* bufferHandle,
                       int64_t*     returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    union
    {
        uint64_t u64;
        int64_t  s64;
    } u;

    OTF2_ErrorCode status = OTF2_Buffer_ReadUint64( bufferHandle, &u.u64 );
    if ( status != OTF2_SUCCESS )
    {
        return status;
    }
    *returnValue = u.s64;

    return OTF2_SUCCESS;
}


void
OTF2_Buffer_ReadUint64Full( OTF2_Buffer* bufferHandle,
                            uint64_t*    returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    memcpy( returnValue, bufferHandle->read_pos, 8 );
    bufferHandle->read_pos += 8;

    if ( bufferHandle->endianness_mode != OTF2_HOST_ENDIANNESS )
    {
        *returnValue = otf2_swap64( *returnValue );
    }
}


OTF2_ErrorCode
OTF2_Buffer_ReadMetricValue( OTF2_Buffer*      bufferHandle,
                             OTF2_MetricValue* returnValue )
{
    UTILS_ASSERT( returnValue );
    return OTF2_Buffer_ReadUint64( bufferHandle, &returnValue->unsigned_int );
}


void
OTF2_Buffer_ReadInt64Full( OTF2_Buffer* bufferHandle,
                           int64_t*     returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    union
    {
        uint64_t u64;
        int64_t  s64;
    } u;

    OTF2_Buffer_ReadUint64Full( bufferHandle, &u.u64 );
    *returnValue = u.s64;
}


void
OTF2_Buffer_ReadFloat( OTF2_Buffer* bufferHandle,
                       float*       returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    union
    {
        uint32_t u32;
        float    f;
    } u;

    OTF2_Buffer_ReadUint32Full( bufferHandle, &u.u32 );
    *returnValue = u.f;
}


void
OTF2_Buffer_ReadDouble( OTF2_Buffer* bufferHandle,
                        double*      returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    union
    {
        uint64_t u64;
        double   d;
    } u;

    OTF2_Buffer_ReadUint64Full( bufferHandle, &u.u64 );
    *returnValue = u.d;
}


OTF2_ErrorCode
OTF2_Buffer_ReadString( OTF2_Buffer* bufferHandle,
                        const char** returnValue )
{
    UTILS_ASSERT( bufferHandle );
    UTILS_ASSERT( returnValue );

    if ( NULL == memchr( bufferHandle->read_pos, '\0', bufferHandle->chunk->end - bufferHandle->read_pos ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Searching for string's terminating null byte failed!" );
    }

    *returnValue            = ( const char* )bufferHandle->read_pos;
    bufferHandle->read_pos += strlen( *returnValue ) + 1;

    return OTF2_SUCCESS;
}
/** @} */

/* ___ Skip operations for essential data types. ____________________________ */



/** @brief Skip an uncompressed data type.
 *
 *  This function is triggered by macros defined in the header named
 *  OTF2_Buffer_Skip<type> (e.g. unit8 or int64). Please refer to them
 *  to get detailed information on how to use them.
 */
OTF2_ErrorCode
OTF2_Buffer_Skip( OTF2_Buffer* bufferHandle,
                  uint64_t     size )
{
    UTILS_ASSERT( bufferHandle );

    if ( size >= ( bufferHandle->chunk->end - bufferHandle->read_pos ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Could not skip. Invalid size argument." );
    }

    bufferHandle->read_pos += size;

    return OTF2_SUCCESS;
}


/** @brief Skip a compressed data type.
 *
 *  Warning: This function is not allowed to be used by any other module even
 *  though it is defined external. This function is triggered by macros defined
 *  in the header named OTF2_Buffer_Skip<type> (e.g. unit8 or int64). Please
 *  refer to them to get detailed information on how to use them.
 */
OTF2_ErrorCode
OTF2_Buffer_SkipCompressed( OTF2_Buffer* bufferHandle )
{
    UTILS_ASSERT( bufferHandle );

    /* Check if there is 1 byte available that contains length information */
    if ( 1 >= ( bufferHandle->chunk->end - bufferHandle->read_pos ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Reading length information of compressed data type failed!" );
    }

    /* Get number of bytes used for data payload */
    uint8_t size = bufferHandle->read_pos[ 0 ];

    /* 0xff is the special compression value for OTF2_UNDEFINED_UINT(32|64)
       nothing to skip here */
    if ( size == 0xff )
    {
        size = 0;
    }

    /* Valid compression size values never exceed 8 bytes */
    if ( size > 8 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Invalid size in compressed length byte." );
    }

    /* Check if there is enough memory for data payload (including length byte) */
    if ( size + 1 >= ( bufferHandle->chunk->end - bufferHandle->read_pos ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Buffer does not contain enough memory to read compressed data type!" );
    }

    /* Skip compressed data type */
    bufferHandle->read_pos += size + 1;

    return OTF2_SUCCESS;
}
/** @} */



/* ___ Additional get and set routines. _____________________________________ */



/** @brief Get next chunk.
 *
 *  The next function is needed to make it obsolete for the buffer in reading
 *  mode to check every time if the the requested date is in the actual chunk.
 *  If a local reader reads a NOOP event in forward reading mode, it has to
 *  trigger the ReadGetNextChunk() function.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_ReadGetNextChunk( OTF2_Buffer* bufferHandle )
{
    UTILS_ASSERT( bufferHandle );

    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This call is not allowed in writing mode!" );
    }

    /* If there is no next chunk available, allocate new one and load data from
     * file. */
    if ( bufferHandle->chunk->next == NULL )
    {
        otf2_chunk* new_chunk = NULL;

        /* Reuse previous chunk if available. */
        if ( bufferHandle->chunk->prev != NULL )
        {
            new_chunk = bufferHandle->chunk->prev;

            /* Remove previous chunk from chunk list. */
            bufferHandle->chunk->prev = NULL;
            bufferHandle->chunk_list  = bufferHandle->chunk;
        }
        else
        {
            /* Allocate memory for a new chunk handle and validate. */
            new_chunk = ( otf2_chunk* )calloc( 1, sizeof( otf2_chunk ) );
            if ( NULL == new_chunk )
            {
                return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                                    "Could not allocate memory for handle!" );
            }

            new_chunk->begin = ( uint8_t* )malloc( bufferHandle->chunk_size );
            if ( NULL == new_chunk->begin )
            {
                free( new_chunk );
                return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                                    "Could not allocate memory for chunk!" );
            }
        }

        /* Initialize new chunk handle data. */
        new_chunk->end         = new_chunk->begin + bufferHandle->chunk_size;
        new_chunk->chunk_num   = bufferHandle->chunk->chunk_num + 1;
        new_chunk->first_event = 0;
        new_chunk->last_event  = 0;
        new_chunk->prev        = bufferHandle->chunk;
        new_chunk->next        = NULL;

        /* Pack chunk in the list of chunks. */
        bufferHandle->chunk->next = new_chunk;
        bufferHandle->chunk       = new_chunk;

        status = otf2_buffer_read_chunk( bufferHandle, OTF2_FILEPART_NEXT );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status, "Read of next chunk failed!" );
        }
    }
    else
    {
        bufferHandle->chunk = bufferHandle->chunk->next;
    }
    bufferHandle->read_pos = bufferHandle->chunk->begin;

    status = otf2_buffer_read_header( bufferHandle );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Read of chunk header failed!" );
    }

    return OTF2_SUCCESS;
}


/** @brief Get previous chunk.
 *
 *  The previous function is needed to make it obsolete for the buffer in reading
 *  mode to check every time if the the requested date is in the actual chunk.
 *  If a local gets a further delivery request after delivering the first event
 *  record of the actual chunk in backward reading mode, it has to trigger the
 *  ReadGetPrevoiusChunk() function.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_ReadGetPreviousChunk( OTF2_Buffer* bufferHandle )
{
    UTILS_ASSERT( bufferHandle );

    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This call is not allowed in writing mode!" );
    }

    if ( bufferHandle->chunk->chunk_num == 1 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "There's no previous chunk!" );
    }

    /* If there is no previous chunk available, allocate new one and load data from
     * file. */
    if ( bufferHandle->chunk->prev == NULL )
    {
        otf2_chunk* new_chunk = NULL;

        /* Reuse next chunk if available. */
        if ( bufferHandle->chunk->next != NULL )
        {
            new_chunk = bufferHandle->chunk->next;

            /* Remove next chunk from chunk list. */
            bufferHandle->chunk->next = NULL;
        }
        else
        {
            /* Allocate memory for a chunk handle and validate. */
            new_chunk = ( otf2_chunk* )calloc( 1, sizeof( otf2_chunk ) );
            if ( NULL == new_chunk )
            {
                return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                                    "Could not allocate memory for handle!" );
            }

            new_chunk->begin = ( uint8_t* )malloc( bufferHandle->chunk_size );
            if ( NULL == new_chunk->begin )
            {
                return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                                    "Could not allocate memory for chunk!" );
            }
        }

        /* Initialize chunk handle data. */
        new_chunk->end         = new_chunk->begin + bufferHandle->chunk_size;
        new_chunk->chunk_num   = bufferHandle->chunk->chunk_num - 1;
        new_chunk->first_event = 0;
        new_chunk->last_event  = 0;
        new_chunk->prev        = NULL;
        new_chunk->next        = bufferHandle->chunk;

        /* Pack chunk in the list of chunks. */
        bufferHandle->chunk->prev = new_chunk;
        bufferHandle->chunk       = new_chunk;

        status = otf2_buffer_read_chunk( bufferHandle, OTF2_FILEPART_PREV );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status, "Read of previous chunk failed!" );
        }
    }
    else
    {
        bufferHandle->chunk = bufferHandle->chunk->prev;
    }

    bufferHandle->read_pos = bufferHandle->chunk->begin;

    status = otf2_buffer_read_header( bufferHandle );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Read of chunk header failed!" );
    }

    return OTF2_SUCCESS;
}

/** @brief Seek to chunk containing a given position in binary fashion.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @param position         Position we are looking for.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */

OTF2_ErrorCode
OTF2_Buffer_ReadSeekChunk( OTF2_Buffer* bufferHandle,
                           uint64_t     position )
{
    OTF2_ErrorCode status;

    /* Validate arguments. */
    if ( NULL == bufferHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid memory buffer handle!" );
    }
    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This call is not allowed in writing mode!" );
    }

    /* look for position in loaded chunks */
    otf2_chunk* chunk;
    bool        chunk_found = false;

    for ( chunk = bufferHandle->chunk_list; chunk != NULL; chunk = chunk->next )
    {
        if ( chunk->first_event < position && position < chunk->last_event )
        {
            chunk_found         = true;
            bufferHandle->chunk = chunk;
            break;
        }
    }

    if ( !chunk_found )
    {
        /* free next and previous chunk */
        chunk = bufferHandle->chunk;
        if ( chunk->prev )
        {
            free( chunk->prev->begin );
            chunk->prev = NULL;
        }
        if ( chunk->next )
        {
            free( chunk->next->begin );
            chunk->next = NULL;
        }

        /* binary search */
        uint64_t file_size, number_of_chunks;

        status = OTF2_File_GetSizeUnchunked( bufferHandle->file, &file_size );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Couldn't get decompressed file size!" );
        }

        number_of_chunks = file_size / bufferHandle->chunk_size + 1;

        uint64_t lower_bound = 1, upper_bound = number_of_chunks, center;
        uint64_t firstEvent, lastEvent;

        do
        {
            center = ( lower_bound + upper_bound ) / 2;

            /* jump to chunk 'center' in file */
            status = OTF2_File_SeekChunk( bufferHandle->file, center, bufferHandle->chunk_size );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status,
                                    "Failed seeking chunk!" );
            }

            /* load header to start of chunk */
            status = otf2_buffer_load_header( bufferHandle );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status,
                                    "Failed to load chunk header!" );
            }

            /* set position to start of chunk */
            status = OTF2_Buffer_SetPosition( bufferHandle, bufferHandle->chunk->begin );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status,
                                    "Failed to set buffer position!" );
            }


            /* read header */
            status = otf2_buffer_read_header( bufferHandle );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status,
                                    "Failed to read chunk header!" );
            }
            status = OTF2_Buffer_GetNumberEvents( bufferHandle, &firstEvent, &lastEvent );
            if ( status != OTF2_SUCCESS )
            {
                return UTILS_ERROR( status,
                                    "Couldn't get number of first and last event in chunk!" );
            }

            if ( firstEvent <= position && position <= lastEvent )
            {
                chunk_found = true;
            }
            else
            {
                if ( firstEvent > position )
                {
                    upper_bound = center - 1;
                }
                else
                {
                    lower_bound = center + 1;
                }
            }
        }
        while ( !chunk_found && lower_bound <= upper_bound );

        if ( !chunk_found )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT, "Requested position doesn't exist! \n" );
        }

        /* load chunk events without header */
        status = otf2_buffer_load_events( bufferHandle );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to load events!" );
        }

        /* update chunk number */
        bufferHandle->chunk->chunk_num = center;
    }

    /* set buffer read position to begin of events */
    uint8_t* new_position;
    status = OTF2_Buffer_GetBeginOfChunk( bufferHandle, &new_position );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed to get begin of chunk!" );
    }

    status = OTF2_Buffer_SetPosition( bufferHandle, new_position );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed to set buffer position!" );
    }

    return OTF2_SUCCESS;
}

/** @brief Seek to latest chunk starting with a timestamp <= than a given time.
 *
 *  This is used to find the best matching snapshot to a given time in a binary fashion.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @param req_time         Time we are looking for.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */

OTF2_ErrorCode
OTF2_Buffer_ReadSeekChunkTime( OTF2_Buffer* bufferHandle,
                               uint64_t     req_time,
                               bool*        found )
{
    OTF2_ErrorCode status;

    UTILS_ASSERT( bufferHandle );

    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This call is not allowed in writing mode!" );
    }
    /* free next and previous chunk */
    otf2_chunk* chunk = bufferHandle->chunk;
    if ( chunk->prev )
    {
        free( chunk->prev->begin );
        chunk->prev = NULL;
    }
    if ( chunk->next )
    {
        free( chunk->next->begin );
        chunk->next = NULL;
    }
    /* binary search .. */
    uint64_t file_size, number_of_chunks;

    status = OTF2_File_GetSizeUnchunked( bufferHandle->file, &file_size );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Couldn't get decompressed file size!" );
    }

    number_of_chunks = file_size / bufferHandle->chunk_size + 1;

    uint64_t       lower_bound = 1, upper_bound = number_of_chunks, center;
    OTF2_TimeStamp time1;
    *found = false;

    do
    {
        center = ( lower_bound + upper_bound + 1 ) / 2;

        /* jump to chunk 'center' in file */
        status = OTF2_File_SeekChunk( bufferHandle->file, center, bufferHandle->chunk_size );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed seeking chunk!" );
        }

        /* load header and first timestamp to start of chunk */
        status = otf2_buffer_load_head_n_time( bufferHandle );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to load chunk header and time!" );
        }

        status = OTF2_Buffer_SetPosition( bufferHandle, bufferHandle->chunk->begin );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to jump to the chunk begin!" );
        }

        status = otf2_buffer_read_header( bufferHandle );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to read chunk header!" );
        }

        status = OTF2_Buffer_ReadTimeStamp( bufferHandle, &time1 );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to read timestamp!" );
        }

        if ( time1 > req_time )
        {
            upper_bound = center - 1;
        }
        else
        {
            lower_bound = center;
            if ( lower_bound == upper_bound )
            {
                *found = true;              // requested chunk found
            }
        }
    }
    while ( lower_bound <= upper_bound && !*found );

    if ( !*found )              // all chunks start later than req_time
    {
        return OTF2_SUCCESS;
    }

    /* jump to chunk 'center' in file */
    status = OTF2_File_SeekChunk( bufferHandle->file, center, bufferHandle->chunk_size );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed seeking chunk!" );
    }

    /* load the whole chunk */
    status = otf2_buffer_read_chunk( bufferHandle, OTF2_FILEPART_NEXT );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed reading chunk!" );
    }

    status = OTF2_Buffer_SetPosition( bufferHandle, bufferHandle->chunk->begin );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed to set buffer position!" );
    }

    status = otf2_buffer_read_header( bufferHandle );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed to read chunk header!" );
    }

    /* update chunk number */
    bufferHandle->chunk->chunk_num = center;

    /* set buffer read position to begin of events */
    uint8_t* new_position;
    status = OTF2_Buffer_GetBeginOfChunk( bufferHandle, &new_position );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed to get begin of chunk!" );
    }

    status = OTF2_Buffer_SetPosition( bufferHandle, new_position );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Failed to set buffer position!" );
    }

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
otf2_buffer_get_file_handle( OTF2_Buffer* bufferHandle )
{
    /* Check if we need and have a locationID. */
    if ( otf2_file_type_needs_location_id( bufferHandle->file_type )
         && bufferHandle->location_id == OTF2_UNDEFINED_UINT64 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "Could not open file. No location id given." );
    }

    bufferHandle->file =
        OTF2_File_Open( bufferHandle->archive,
                        bufferHandle->buffer_mode == OTF2_BUFFER_WRITE  ? OTF2_FILEMODE_WRITE :
                        bufferHandle->buffer_mode == OTF2_BUFFER_MODIFY ? OTF2_FILEMODE_MODIFY :
                        OTF2_FILEMODE_READ,
                        bufferHandle->file_type,
                        bufferHandle->location_id );
    if ( NULL == bufferHandle->file )
    {
        return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                            "Could not open file." );
    }
    return OTF2_SUCCESS;
}

/** @brief Read data from file to the recent chunk.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param filePart         Defines whether to read next or previous chunk of a
 *                          file. Available values are "OTF2_FILEPART_NEXT" and
 *                          "OTF2_FILEPART_PREV".
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_buffer_read_chunk( OTF2_Buffer*  bufferHandle,
                        OTF2_FilePart filePart )
{
    UTILS_ASSERT( bufferHandle );

    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* If there is no file handle, open one. */
    if ( bufferHandle->file == NULL  )
    {
        status = otf2_buffer_get_file_handle( bufferHandle );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to get file handle!" );
        }
    }

    /* If it is an unchunked buffer, read in the whole file. */
    if ( bufferHandle->chunk_mode == OTF2_BUFFER_NOT_CHUNKED )
    {
        status = OTF2_File_GetSizeUnchunked( bufferHandle->file,
                                             &( bufferHandle->chunk_size ) );

        /* Allocate memory buffer. */
        bufferHandle->chunk->begin = ( uint8_t* )malloc( bufferHandle->chunk_size );
        if ( NULL == bufferHandle->chunk->begin )
        {
            return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                                "Could not allocate memory for chunk!" );
        }

        bufferHandle->chunk->end = bufferHandle->chunk->begin + bufferHandle->chunk_size;
        bufferHandle->read_pos   = bufferHandle->chunk->begin;
    }

    /* If the previous chunk in the file is requested, jump to chunk sizes
     * backwards. */
    if ( filePart == OTF2_FILEPART_PREV &&
         bufferHandle->chunk_mode == OTF2_BUFFER_CHUNKED )
    {
        status = OTF2_File_SeekPrevChunk( bufferHandle->file,
                                          bufferHandle->chunk_size );
    }

    status = OTF2_File_Read( bufferHandle->file, bufferHandle->chunk->begin,
                             bufferHandle->chunk_size );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Could not read from file." );
    }

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
otf2_buffer_load_header( OTF2_Buffer* bufferHandle )
{
    OTF2_ErrorCode status;

    /* If there is no file handle, open one. */
    if ( bufferHandle->file == NULL  )
    {
        status = otf2_buffer_get_file_handle( bufferHandle );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to get file handle!" );
        }
    }

    status = OTF2_File_Read( bufferHandle->file, bufferHandle->chunk->begin, OTF2_CHUNK_HEADER_SIZE );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Could not read from file!" );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_buffer_load_events( OTF2_Buffer* bufferHandle )
{
    OTF2_ErrorCode status;
    uint8_t*       chunk_begin;

    status = OTF2_Buffer_GetBeginOfChunk(  bufferHandle, &chunk_begin ); // begin of events!

    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Could not get begin of chunk!" );
    }

    status = OTF2_File_Read( bufferHandle->file,
                             chunk_begin,
                             bufferHandle->chunk_size - OTF2_CHUNK_HEADER_SIZE );

    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Could not read from file!" );
    }

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
otf2_buffer_load_head_n_time( OTF2_Buffer* bufferHandle )
{
    OTF2_ErrorCode status;

    /* If there is no file handle, open one. */
    if ( bufferHandle->file == NULL  )
    {
        status = otf2_buffer_get_file_handle( bufferHandle );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to get file handle!" );
        }
    }

    return OTF2_File_Read( bufferHandle->file,
                           bufferHandle->chunk->begin,
                           OTF2_CHUNK_HEADER_SIZE + sizeof( OTF2_TimeStamp ) );
}


/** @brief Read chunk header.
 *
 *  Reads the chunk header with the number of the first and last event in the
 *  current chunk.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static inline OTF2_ErrorCode
otf2_buffer_read_header( OTF2_Buffer* bufferHandle )
{
    UTILS_ASSERT( bufferHandle );

    uint8_t event_type = 0;

    OTF2_Buffer_ReadUint8( bufferHandle, &event_type );

    if ( event_type != OTF2_BUFFER_CHUNK_HEADER )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "This is no chunk header!" );
    }

    uint8_t endianness_mode;
    OTF2_Buffer_ReadUint8( bufferHandle, &endianness_mode );

    /* Set buffers read routine to the correct endianness mode, before
       reading in the chunk header part */

    if ( endianness_mode != OTF2_BIG_ENDIAN &&
         endianness_mode != OTF2_LITTLE_ENDIAN )
    {
        /* invalid endianness byte */
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Invalid endianness byte %hhx",
                            endianness_mode );
    }

    bufferHandle->endianness_mode = endianness_mode;

    if ( bufferHandle->chunk_mode == OTF2_BUFFER_CHUNKED )
    {
        OTF2_Buffer_ReadUint64Full( bufferHandle,
                                    &( bufferHandle->chunk->first_event ) );
        OTF2_Buffer_ReadUint64Full( bufferHandle,
                                    &( bufferHandle->chunk->last_event ) );
    }

    return OTF2_SUCCESS;
}


/** @brief Get position.
 *
 *  Checks the mode of interaction with the buffer and stores either the actual
 *  write or read position in the given pointer.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param position         Pointer the returned position.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
void
OTF2_Buffer_GetPosition( OTF2_Buffer* bufferHandle,
                         uint8_t**    position )
{
    UTILS_ASSERT( bufferHandle );

    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        *position = bufferHandle->write_pos;
    }
    else
    {
        *position = bufferHandle->read_pos;
    }
}


/** @brief Set position.
 *
 *  Checks the mode of interaction with the buffer and sets either the actual
 *  write or read position or both to the given position.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param position         Pointer to the position.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_SetPosition( OTF2_Buffer* bufferHandle,
                         uint8_t*     position )
{
    UTILS_ASSERT( bufferHandle );

    if ( ( position < bufferHandle->chunk->begin ) ||
         ( position >= bufferHandle->chunk->end ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Position points outside the buffer's memory!" );
    }


    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        bufferHandle->write_pos = position;
        return OTF2_SUCCESS;
    }

    if ( bufferHandle->buffer_mode == OTF2_BUFFER_MODIFY )
    {
        bufferHandle->write_pos = position;
        bufferHandle->read_pos  = position;
        return OTF2_SUCCESS;
    }

    bufferHandle->read_pos = position;
    return OTF2_SUCCESS;
}


/** @brief Get position of the begin of the current chunk.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param position         Pointer to the returned position.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_GetBeginOfChunk( OTF2_Buffer* bufferHandle,
                             uint8_t**    position )
{
    UTILS_ASSERT( bufferHandle );

    *position = bufferHandle->chunk->begin + OTF2_CHUNK_HEADER_SIZE;

    return OTF2_SUCCESS;
}


/** @brief Get the number of first and last event in the current chunk.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param firstEvent       Pointer to the returned number of the first event.
 *  @param lastEvent        Pointer to the returned number of the last event.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_GetNumberEvents( OTF2_Buffer* bufferHandle,
                             uint64_t*    firstEvent,
                             uint64_t*    lastEvent )
{
    UTILS_ASSERT( bufferHandle );

    *firstEvent = bufferHandle->chunk->first_event;
    *lastEvent  = bufferHandle->chunk->last_event;

    return OTF2_SUCCESS;
}


/** @brief Get position of last read timestamp.
 *
 *  The position of the last read timestamp is the valid timestamp for the
 *  current position as well.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param timePos          Pointer to the returned position.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_GetPositionTimeStamp( OTF2_Buffer* bufferHandle,
                                  uint8_t**    timePos )
{
    UTILS_ASSERT( bufferHandle );

    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This is not allowed in writing mode!" );
    }

    *timePos = bufferHandle->time_pos;

    return OTF2_SUCCESS;
}


/** @brief Set position of last read timestamp.
 *
 *  The position of the last read timestamp is the valid timestamp for the
 *  currect position as well.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param timePos          Position of the timestamp.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_SetPositionTimeStamp( OTF2_Buffer* bufferHandle,
                                  uint8_t*     timePos )
{
    UTILS_ASSERT( bufferHandle );

    bufferHandle->time_pos = timePos;
    memcpy( &( bufferHandle->time ), timePos + 1, 8 );

    return OTF2_SUCCESS;
}


/** @brief Rewrite timestamp.
 *
 *  Checks if there is a timestamp at the passed position and rewrites the
 *  recorded timestamp with the passed timestamp.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param time             New timestamp.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_RewriteTimeStamp( OTF2_Buffer*   bufferHandle,
                              OTF2_TimeStamp time )
{
    UTILS_ASSERT( bufferHandle );

    /* Rewrite timestamp. */
    memcpy( bufferHandle->time_pos + 1, &time, 8 );
    bufferHandle->time = time;

    return OTF2_SUCCESS;
}



/* ___ Routines to manage the rewind stack. _________________________________ */



/** @brief Clear all rewind points saved before.
 *
 *  Deallocate all saved rewind points in the rewind list and
 *  assign a NULL pointer to the rewind list.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_buffer_clear_rewind_list( OTF2_Buffer* bufferHandle )
{
    /* iterate over rewind points and deallocate all memory. */
    while ( bufferHandle->rewindlist )
    {
        otf2_rewindlist* next = bufferHandle->rewindlist->next;

        free( bufferHandle->rewindlist->buffer );
        free( bufferHandle->rewindlist->chunk );
        free( bufferHandle->rewindlist );

        bufferHandle->rewindlist = next;
    }

    return OTF2_SUCCESS;
}


/** @brief Store a buffer state in the rewind list.
 *
 *  Saves the current buffer state in a double-linked list of rewind points.
 *  If the same rewind region id is already in the list, then the old
 *  rewind point will be overwritten, else a new rewind point is allocated
 *  at the end of the list.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param id               Identification number of the rewind region handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_StoreRewindPoint( OTF2_Buffer* bufferHandle,
                              uint32_t     id )
{
    UTILS_ASSERT( bufferHandle && bufferHandle->buffer_mode == OTF2_BUFFER_WRITE );

    otf2_rewindlist* rewindlistitem = bufferHandle->rewindlist;

    /* If rewind list is empty, then store buffer state as the first rewind point */
    if ( !rewindlistitem )
    {
        rewindlistitem         = ( otf2_rewindlist* )malloc( sizeof( otf2_rewindlist ) );
        rewindlistitem->id     = id;
        rewindlistitem->buffer = ( OTF2_Buffer* )malloc( sizeof( OTF2_Buffer ) );
        rewindlistitem->chunk  = ( otf2_chunk* )malloc( sizeof( otf2_chunk ) );
        rewindlistitem->prev   = NULL;
        rewindlistitem->next   = NULL;

        memcpy( rewindlistitem->buffer, bufferHandle, sizeof( OTF2_Buffer ) );
        memcpy( rewindlistitem->chunk, bufferHandle->chunk, sizeof( otf2_chunk ) );

        bufferHandle->rewindlist = rewindlistitem;

        return OTF2_SUCCESS;
    }

    /* else, search for a previous saved rewind point with the same id. */
    while ( rewindlistitem->id != id && rewindlistitem->next )
    {
        rewindlistitem = rewindlistitem->next;
    }

    /* If there is no previous saved rewind point, then save as a new rewind point. */
    if ( rewindlistitem->id != id )
    {
        rewindlistitem->next       = ( otf2_rewindlist* )malloc( sizeof( otf2_rewindlist ) );
        rewindlistitem->next->prev = rewindlistitem;
        rewindlistitem->next->next = NULL;
        rewindlistitem             = rewindlistitem->next;

        rewindlistitem->id     = id;
        rewindlistitem->buffer = ( OTF2_Buffer* )malloc( sizeof( OTF2_Buffer ) );
        rewindlistitem->chunk  = ( otf2_chunk* )malloc( sizeof( otf2_chunk ) );
    }

    /* And copy the buffer state. */
    memcpy( rewindlistitem->buffer, bufferHandle, sizeof( OTF2_Buffer ) );
    memcpy( rewindlistitem->chunk, bufferHandle->chunk, sizeof( otf2_chunk ) );

    return OTF2_SUCCESS;
}


/** @brief Clear one specific rewind point, which was saved before.
 *
 *  Deallocate all buffers reserved for a specific rewind point in the
 *  rewind list and remove the rewind point from the double-linked rewind list.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param id               Identification number of the rewind region handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs or the id isn't in the list.
 */
OTF2_ErrorCode
OTF2_Buffer_ClearRewindPoint( OTF2_Buffer* bufferHandle,
                              uint32_t     id )
{
    UTILS_ASSERT( bufferHandle && bufferHandle->buffer_mode == OTF2_BUFFER_WRITE );

    otf2_rewindlist* rewindlistitem = bufferHandle->rewindlist;
    otf2_rewindlist* prev           = NULL;
    otf2_rewindlist* next           = NULL;

    /* Search for the rewind region id in the rewind list. */
    while ( rewindlistitem && rewindlistitem->id != id )
    {
        rewindlistitem = rewindlistitem->next;
    }

    /* If the id was found in the rewind list, then */
    if ( rewindlistitem )
    {
        /* Renew the prev and next pointers for the neighbour elements in the list,
           to safely remove the rewind point. */
        prev = rewindlistitem->prev;
        next = rewindlistitem->next;
        if ( prev )
        {
            prev->next = next;
        }
        if ( next )
        {
            next->prev = prev;
        }

        /* Check if deleted rewind point was the first element (and maybe the last,
           too) in the list */
        if ( !prev && !next )
        {
            bufferHandle->rewindlist = NULL;
        }
        else if ( !prev && next )
        {
            bufferHandle->rewindlist = next;
        }

        /* And, delete previous allocated memory. */
        free( rewindlistitem->buffer );
        free( rewindlistitem->chunk );
        free( rewindlistitem );
    }
    else
    {
        /* else, the id is not in the list because of an previous buffer flush
         * or a programming error. */
        UTILS_WARNING( "Rewind id not found in rewind list!" );
    }

    return OTF2_SUCCESS;
}


/** @brief Make a rewind of the current buffer state to a previous saved state.
 *
 *  Forget all saved OTF2 events after a defined point in time. This will be
 *  done thru a rewind of the buffer state to a previous saved buffer state.
 *  If there was a buffer flush in the meantime, then there won't be a old
 *  buffer state in the rewind list.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param id               Identification number of the rewind region handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs or the id isn't in the list.
 */
OTF2_ErrorCode
OTF2_Buffer_Rewind( OTF2_Buffer* bufferHandle,
                    uint32_t     id )
{
    UTILS_ASSERT( bufferHandle && bufferHandle->buffer_mode == OTF2_BUFFER_WRITE );

    otf2_rewindlist* rewindlistitem = bufferHandle->rewindlist;
    otf2_rewindlist* dummy          = NULL;
    otf2_chunk*      chunk          = NULL;

    /* Search for the rewind region id in the rewind list. */
    while ( rewindlistitem && rewindlistitem->id != id )
    {
        rewindlistitem = rewindlistitem->next;
    }

    /* If the id was found in the rewind list, then */
    if ( rewindlistitem )
    {
        /* link all chunks, which were allocated after the rewind point
         * with the old_chunk_list */
        bufferHandle->chunk->next = bufferHandle->old_chunk_list;

        /* save the new start of the old_chunk_list */
        chunk = rewindlistitem->buffer->chunk->next;

        /* Rewind buffer state (but save the current pointer to the
         * rewind list). */
        dummy = bufferHandle->rewindlist;
        memcpy( bufferHandle, rewindlistitem->buffer, sizeof( OTF2_Buffer ) );
        memcpy( bufferHandle->chunk, rewindlistitem->chunk, sizeof( otf2_chunk ) );
        bufferHandle->rewindlist = dummy;

        /* set the new start of old_chunk_list in the rewinded buffer */
        bufferHandle->old_chunk_list = chunk;
    }
    else
    {
        /* else, an error code will be returned. */
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Key not found in rewind list!" );
    }

    return OTF2_SUCCESS;
}



/* ___ Routines to check buffer consistency . _______________________________ */



/** @brief Read record length information and check if buffer contains enough
 *         memory according to this information.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param recordLength     Returns length of recent record. Set this argument to
 *                          NULL if you are not interested in this information.
 *
 *  @return                 Returns OTF2_SUCCESS if successful. If recordLength
 *                          parameter is not NULL, then record length information
 *                          read for recent record will be set to this variable.
 *                          Otherwise an error code will be returned.
 */
OTF2_ErrorCode
OTF2_Buffer_GuaranteeRecord( OTF2_Buffer* bufferHandle,
                             uint64_t*    recordLength )
{
    UTILS_ASSERT( bufferHandle );

    /* Check if there is enough memory for record length byte */
    if ( bufferHandle->chunk->end - bufferHandle->read_pos <= 1 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Could not read record. Not enough memory left in buffer." );
    }

    /* Read first record length byte */
    uint8_t small_record_length;
    OTF2_Buffer_ReadUint8( bufferHandle, &small_record_length );
    if ( small_record_length != UINT8_MAX )
    {
        /* Record length is encoded in this byte */

        /* Check if the buffer contains enough bytes according to record length information */
        if ( bufferHandle->chunk->end - bufferHandle->read_pos <= small_record_length )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                                "Could not read record. Not enough memory left in buffer." );
        }

        if ( recordLength != NULL )
        {
            *recordLength = small_record_length;
        }
    }
    else
    {
        /* Length of large records is encoded in additional 8 bytes */

        /* Read additional 8 bytes to get length of large records */
        if ( bufferHandle->chunk->end - bufferHandle->read_pos <= 8 )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                                "Could not read record. Not enough memory left in buffer." );
        }

        uint64_t large_record_length;
        OTF2_Buffer_ReadUint64Full( bufferHandle, &large_record_length );

        /* Check if the buffer contains enough bytes according to record length information */
        if ( bufferHandle->chunk->end - bufferHandle->read_pos <= large_record_length )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                                "Could not read record. Not enough memory left in buffer." );
        }

        if ( recordLength != NULL )
        {
            *recordLength = large_record_length;
        }
    }

    return OTF2_SUCCESS;
}



/** @brief Check if the buffer contains enough memory according to read a (compressed) integer value.
 *         The read position in the buffer is not changed by this function.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_GuaranteeCompressed( OTF2_Buffer* bufferHandle )
{
    UTILS_ASSERT( bufferHandle );

    /* Check if there is enough memory for compressed header byte */
    if ( bufferHandle->chunk->end - bufferHandle->read_pos <= 1 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Could not read record. Not enough memory left in buffer." );
    }

    uint8_t size = bufferHandle->read_pos[ 0 ];

    /* 0xff is the special compression value for OTF2_UNDEFINED_UINT(32|64)
       nothing to guarantee here */
    if ( size == 0xff )
    {
        size = 0;
    }

    /* Valid compression size values never exceed 8 bytes */
    if ( size > 8 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Invalid size in compressed length byte." );
    }

    /* Check if there is enough memory for record payload (including compression size byte ) */
    if ( bufferHandle->chunk->end - bufferHandle->read_pos <= size + 1 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Could not read record. Not enough memory left in buffer." );
    }

    return OTF2_SUCCESS;
}



/** @brief Check if the buffer contains memory according to read or write
 *         data of the requested size.
 *
 *         The read position in the buffer is not changed by this function.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param size             Requested memory in bytes.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Buffer_Guarantee( OTF2_Buffer* bufferHandle,
                       uint64_t     size )
{
    /* Validate arguments. */
    UTILS_ASSERT( bufferHandle );

    /* Check if there is enough memory */
    if ( bufferHandle->buffer_mode == OTF2_BUFFER_WRITE )
    {
        if ( bufferHandle->chunk->end - bufferHandle->write_pos <= size )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_SIZE_GIVEN,
                                "Not enough memory left in buffer." );
        }
    }
    else
    {
        if ( bufferHandle->chunk->end - bufferHandle->read_pos < size )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                                "Could not read record. Not enough memory left in buffer." );
        }
    }

    return OTF2_SUCCESS;
}



/* ___ Allocator ____________________________________________________________ */



/** @brief Internal memory allocate.
 *
 *  @param bufferHandle     Buffer handle.
 *
 *  @return                 Returns a void pointer to the allocated memory on
 *                          success, NULL if an error occurs.
 */
void*
otf2_buffer_memory_allocate( OTF2_Buffer* buffer )
{
    if ( buffer->archive->allocator_callbacks )
    {
        return buffer->archive->allocator_callbacks
               ->otf2_allocate( buffer->archive->allocator_data,
                                buffer->file_type,
                                buffer->location_id,
                                &buffer->allocator_buffer_data,
                                buffer->chunk_size );
    }

    if ( buffer->memory >= buffer->chunk_size )
    {
        buffer->memory -= buffer->chunk_size;
        return malloc( buffer->chunk_size );
    }

    return NULL;
}


/** @brief Internal memory free.
 *
 *  Frees all memory buffer associated with chunks.
 *
 *  @param bufferHandle     Buffer handle.
 */
void
otf2_buffer_memory_free( OTF2_Buffer* buffer,
                         bool         final )
{
    if ( buffer->archive->allocator_callbacks )
    {
        buffer->archive->allocator_callbacks->otf2_free_all(
            buffer->archive->allocator_data,
            buffer->file_type,
            buffer->location_id,
            &buffer->allocator_buffer_data,
            final );
        return;
    }

    otf2_chunk* chunk = buffer->chunk_list;
    while ( chunk != NULL )
    {
        free( chunk->begin );
        chunk = chunk->next;
    }

    buffer->memory = OTF2_ALLOCATE_SIZE_MAX;
}


/** @brief Free all chunks.
 *
 *  @param bufferHandle     Buffer handle.
 */
void
otf2_buffer_free_all_chunks( OTF2_Buffer* buffer )
{
    otf2_chunk* temp_chunk = NULL;

    while ( buffer->chunk_list != NULL )
    {
        temp_chunk         = buffer->chunk_list;
        buffer->chunk_list = buffer->chunk_list->next;
        free( temp_chunk );
    }

    buffer->chunk      = NULL;
    buffer->chunk_list = NULL;
}


/** Swap the bytes in an 2-byte word. */
static inline uint16_t
otf2_swap16( uint16_t v16 )
{
    return ( ( v16 >> 8 ) & 0xFFu ) | ( ( v16 & 0xFFu ) << 8 );
}

/** Swap the bytes in an 4-byte word. */
static inline uint32_t
otf2_swap32( uint32_t v32 )
{
    return otf2_swap16( ( v32 >> 16 ) & 0xFFFFu ) |
           ( ( uint32_t )otf2_swap16( v32 & 0xFFFFu ) << 16 );
}

/** Swap the bytes in an 8-byte word. */
static inline uint64_t
otf2_swap64( uint64_t v64 )
{
    return otf2_swap32( ( v64 >> 32 ) & 0xFFFFFFFFu ) |
           ( ( uint64_t )otf2_swap32( v64 & 0xFFFFFFFFu ) << 32 );
}

/** Finalize the buffer */
static inline void
otf2_buffer_finalize( OTF2_Buffer* bufferHandle )
{
    OTF2_Buffer_WriteUint8( bufferHandle, OTF2_BUFFER_END_OF_BUFFER );
    bufferHandle->finalized = true;
}
