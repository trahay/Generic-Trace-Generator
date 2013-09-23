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
 *  @file       src/OTF2_Buffer.h
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
 */

#ifndef OTF2_BUFFER_H
#define OTF2_BUFFER_H


/* ___ Type definitions. _____________________________________________________ */



/** @brief Defines if a buffer is chunked or not. Please see
    OTF2_ChunkMode_enum for a detailed description. */
typedef uint8_t OTF2_ChunkMode;

/** @brief Defines if a buffer is chunked or not. At the moment all event buffers
    are chunked, all definition buffer are not. */
enum OTF2_ChunkMode_enum
{
    /** The buffer is not chunked (e.g. all definition buffers). */
    OTF2_BUFFER_NOT_CHUNKED = 0,
    /** The buffer is chunked (e.g. all event buffers). */
    OTF2_BUFFER_CHUNKED     = 1
};


/** @brief Defines the kind of interaction with the memory buffer. Please see
    OTF2_BufferMode_enum for a detailed description. */
typedef uint8_t OTF2_BufferMode;

/** @brief Defines the kind of interaction with the memory buffer. */
enum OTF2_BufferMode_enum
{
    /** Buffer is in write-only mode (e.g. while tracing/profiling). */
    OTF2_BUFFER_WRITE  = 0,
    /** Buffer is in modify mode (e.g. while time synchronisation). This mode is
        similar to read mode but with the possibility to modify indivual records
        (e.g. timestamps). If this mode was used, the resulting files are marked
        as 'modified' i.e. the library does not guarantee for correct results.
        So this mode is only for advanced users. */
    OTF2_BUFFER_MODIFY = 1,
    /** Buffer is in read-only mode (e.g. while analysing). */
    OTF2_BUFFER_READ   = 2
};


/** @brief The endiannss modes. */
typedef uint8_t OTF2_EndiannessMode;
enum OTF2_EndiannessMode_enum
{
    OTF2_BIG_ENDIAN    = 0x23,
    OTF2_LITTLE_ENDIAN = 0x42
};


/** @brief Wrapper for the OTF2_InternalBufferEvents_enum
 */
typedef uint8_t OTF2_InternalBufferEvents;

/** @brief Defines internal memory buffer events. */
enum OTF2_InternalBufferEvents_enum
{
    /* Events with value 0 to 4 are used by OTF2 internal buffer events */

    /** OTF2 internal event to indicate the end of a memory chunk */
    OTF2_BUFFER_END_OF_CHUNK  = 0,
    /** OTF2 internal event to indicate the end of memory buffer */
    OTF2_BUFFER_END_OF_BUFFER = 1,
    /** OTF2 internal event to indicate that end of file is reached */
    OTF2_BUFFER_END_OF_FILE   = 2,
    /** OTF2 internal event to indicate internal chunk header */
    OTF2_BUFFER_CHUNK_HEADER  = 3,
    /** OTF2 event to indicate a timestamp event */
    OTF2_BUFFER_TIMESTAMP     = 5,
    /** OTF2 event to indicate an attribute list */
    OTF2_ATTRIBUTE_LIST       = 6
};


/** @brief Keeps all information needed for chunking.
    Please see otf2_chunks_struct for a detailed description. */
typedef struct otf2_chunk_struct otf2_chunk;


/** @brief Keeps all information needed for chunking. */
struct otf2_chunk_struct
{
    /** Pointer to the begin of the actual chunk. */
    uint8_t*    begin;
    /** Pointer to the end of the actual chunk. */
    uint8_t*    end;
    /** Number of the actual chunk (Number of first is One). */
    uint32_t    chunk_num;
    /** Number of the first event in the current chunk. */
    uint64_t    first_event;
    /** Number of the last event in the current chunk. */
    uint64_t    last_event;
    /** Pointer to the previous chunk. */
    otf2_chunk* prev;
    /** Pointer to the next chunk. */
    otf2_chunk* next;
};

/** @brief Keeps all information needed to save rewind points.
    Please see otf2_rewindlist_struct for a detailed description. */
typedef struct otf2_rewindlist_struct otf2_rewindlist;

/** @brief Keeps all information needed to save rewind points. */
struct otf2_rewindlist_struct
{
    /** Rewind region id (derived from region handle) to identify the rewind point. */
    uint32_t         id;
    /** Pointer to a saved copy of the OTF2_Buffer_struct at the time of the rewind point. */
    OTF2_Buffer*     buffer;
    /** Pointer to a saved copy of the current otf2_chunk_struct of the OTF2_Buffer_struct. */
    otf2_chunk*      chunk;
    /** Pointer to the previous rewind point. */
    otf2_rewindlist* prev;
    /** Pointer to the next rewind point. */
    otf2_rewindlist* next;
};

/** @brief Keeps all necessary information about the buffer. */
struct OTF2_Buffer_struct
{
    OTF2_Archive* archive;

    /** The controlling object of this buffer. */
    void* operator;

    /** Mode of interaction with the buffer: write, modify or read. */
    OTF2_BufferMode buffer_mode;
    /** Defines if a buffer is chunked or not. Buffers keeping event records are
        always chunked, buffers keeping definitions are always not chunked. */
    OTF2_ChunkMode chunk_mode;

    /** Size of internal chunks in bytes (may be that for events or defs). */
    uint64_t chunk_size;

    /** Pointer to a file the data is stored. */
    OTF2_File* file;

    /** endianness mode of this buffer */
    OTF2_EndiannessMode endianness_mode;

    OTF2_FileType       file_type;
    OTF2_LocationRef    location_id;

    /** Last written timestamp. */
    OTF2_TimeStamp time;
    /** Position of last read timestamp. */
    uint8_t*       time_pos;


    /** Current writing position. */
    uint8_t* write_pos;
    /** Current reading position. */
    uint8_t* read_pos;

    /** Current start of record data. */
    uint8_t* record_data_pos;

    /** Pointer to the list of chunks. */
    otf2_chunk* chunk_list;
    /** Pointer to the actual chunk. */
    otf2_chunk* chunk;

    /** Size of the memory buffer in bytes. */
    uint64_t memory;

    /** Defines if buffer is finalized i.e. writing to buffer is finished. */
    bool finalized;

    /** Storage for the memory system to store per buffer allocator data */
    void* allocator_buffer_data;

    /** Pointer to the list of rewind points. */
    otf2_rewindlist* rewindlist;

    /** Pointer to the list of old chunks */
    otf2_chunk* old_chunk_list;
};


/* ___ Convenience function macros. __________________________________________ */



/* Switch of indention for all function macros. */
/* *INDENT-OFF* */

/** @name Convenience macros for functions where compressed and uncompressed
 *  version does exactly the same i.e. there is no compression possible.
 *  Please note: If you do implement one of these functions you also have to
 *  implement an uncompressed version and remove the macro below.
 *  @{ */
#define OTF2_Buffer_WriteUint8Full(b,v)  OTF2_Buffer_WriteUint8(b,v)
#define OTF2_Buffer_WriteUint16Full(b,v) OTF2_Buffer_WriteUint16(b,v)

#define OTF2_Buffer_WriteInt8Full(b,v)   OTF2_Buffer_WriteInt8(b,v)
#define OTF2_Buffer_WriteInt16Full(b,v)  OTF2_Buffer_WriteInt16(b,v)

#define OTF2_Buffer_WriteFloatFull(b,v)  OTF2_Buffer_WriteFloat(b,v)
#define OTF2_Buffer_WriteDoubleFull(b,v) OTF2_Buffer_WriteDouble(b,v)


#define OTF2_Buffer_ReadUint8Full(b,v)  OTF2_Buffer_ReadUint8(b,v)
#define OTF2_Buffer_ReadUint16Full(b,v) OTF2_Buffer_ReadUint16(b,v)

#define OTF2_Buffer_ReadInt8Full(b,v)   OTF2_Buffer_ReadInt8(b,v)
#define OTF2_Buffer_ReadInt16Full(b,v)  OTF2_Buffer_ReadInt16(b,v)

#define OTF2_Buffer_ReadFloatFull(b,v)  OTF2_Buffer_ReadFloat(b,v)
#define OTF2_Buffer_ReadDoubleFull(b,v) OTF2_Buffer_ReadDouble(b,v)

/* redirector for writing and reading an OTF2_IdMap */
#define OTF2_Buffer_ReadIdMap(buffer, id_map)  otf2_id_map_read(buffer, id_map)
#define OTF2_Buffer_WriteIdMap(buffer, id_map)  otf2_id_map_write(buffer, id_map)

/** @} */

/* *INDENT-ON* */



/* ___ Interactions with the buffer handle. _________________________________ */



OTF2_Buffer*
OTF2_Buffer_New( OTF2_Archive*    archive,
                 void*            operator_,
                 uint64_t         chunkSize,
                 OTF2_BufferMode  bufferMode,
                 OTF2_ChunkMode   chunkMode,
                 OTF2_FileType    fileType,
                 OTF2_LocationRef location );

OTF2_ErrorCode
OTF2_Buffer_Delete( OTF2_Buffer* bufferHandle );


OTF2_ErrorCode
OTF2_Buffer_SwitchMode( OTF2_Buffer*    bufferHandle,
                        OTF2_BufferMode bufferMode );

OTF2_ErrorCode
OTF2_Buffer_SetLocationID( OTF2_Buffer*     bufferHandle,
                           OTF2_LocationRef location );

/* ___ Write operations for essential data types. ___________________________ */


/** @name Writing routines for basic data types.
 *
 *  Encode basic data types and write them with an internal compression to the
 *  buffer. Functions ending with 'Full' are writing uncompressed values. The
 *  uncompressed functions call should be used for values that may be altered
 *  later on e.g. timestamps. Other functions (without 'Full') write compressed
 *  if possible.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param value            Value to be written to the buffer.
 *
 *  @{
 */
static inline void
OTF2_Buffer_WriteInt8( OTF2_Buffer* bufferHandle,
                       const int8_t value )
{
    memcpy( bufferHandle->write_pos, &value, 1 );
    bufferHandle->write_pos++;
}

static inline void
OTF2_Buffer_WriteUint8( OTF2_Buffer*  bufferHandle,
                        const uint8_t value )
{
    memcpy( bufferHandle->write_pos, &value, 1 );
    bufferHandle->write_pos++;
}


static inline void
OTF2_Buffer_WriteInt16( OTF2_Buffer*  bufferHandle,
                        const int16_t value )
{
    memcpy( bufferHandle->write_pos, &value, 2 );
    bufferHandle->write_pos += 2;
}


static inline void
OTF2_Buffer_WriteUint16( OTF2_Buffer*   bufferHandle,
                         const uint16_t value )
{
    memcpy( bufferHandle->write_pos, &value, 2 );
    bufferHandle->write_pos += 2;
}


static inline void
OTF2_Buffer_WriteInt32( OTF2_Buffer*  bufferHandle,
                        const int32_t value )
{
    uint8_t size = 0;

    /* Compress only positive integers.
     * For there are different representations of negative integers. There is no
     * compression for negative integers at the moment. */
    if ( value >= 0 )
    {
        /* Get number of used bytes. */
        if ( INT32_C( 0x100 ) > value )
        {
            size = 1;
        }
        else if ( INT32_C( 0x10000 ) > value )
        {
            size = 2;
        }
        else if ( INT32_C( 0x1000000 ) > value )
        {
            size = 3;
        }
        else
        {
            size = 4;
        }
    }
    else
    {
        size = 4;
    }

    *bufferHandle->write_pos++ = size;

#ifdef WORDS_BIGENDIAN /* Store last bytes when big endian. */
    memcpy( bufferHandle->write_pos, ( const uint8_t* )&value + 4 - size, size );
#else /* Store first bytes when little endian. */
    memcpy( bufferHandle->write_pos, &value, size );
#endif      /* WORDS_BIGENDIAN. */

    bufferHandle->write_pos += size;
}


static inline void
OTF2_Buffer_WriteInt32Full( OTF2_Buffer*  bufferHandle,
                            const int32_t value )
{
    memcpy( bufferHandle->write_pos, &value, 4 );
    bufferHandle->write_pos += 4;
}


static inline void
OTF2_Buffer_WriteUint32( OTF2_Buffer*   bufferHandle,
                         const uint32_t value )
{
    uint8_t size = 0;

    /* special compression for special values */
    if ( value == 0 || value == UINT32_MAX )
    {
        *bufferHandle->write_pos++ = value & 0xff;
        return;
    }

    /* Get number of used bytes. */
    if ( UINT32_C( 0x100 ) > value )
    {
        size = 1;
    }
    else if ( UINT32_C( 0x10000 ) > value )
    {
        size = 2;
    }
    else if ( UINT32_C( 0x1000000 ) > value )
    {
        size = 3;
    }
    else
    {
        size = 4;
    }

    *bufferHandle->write_pos++ = size;

#ifdef WORDS_BIGENDIAN /* Store last bytes when big endian. */
    memcpy( bufferHandle->write_pos, ( const uint8_t* )&value + 4 - size, size );
#else /* Store first bytes when little endian. */
    memcpy( bufferHandle->write_pos, &value, size );
#endif      /* WORDS_BIGENDIAN. */

    bufferHandle->write_pos += size;
}


static inline void
OTF2_Buffer_WriteUint32Full( OTF2_Buffer*   bufferHandle,
                             const uint32_t value )
{
    memcpy( bufferHandle->write_pos, &value, 4 );
    bufferHandle->write_pos += 4;
}


static inline void
OTF2_Buffer_WriteInt64( OTF2_Buffer*  bufferHandle,
                        const int64_t value )
{
    uint8_t size = 0;

    /* Compress only positive integers.
     * For there are different representations of negative integers. There is no
     * compression for negative integers at the moment. */
    if ( value >= 0 )
    {
        /* Get number of used bytes. */
        if ( INT64_C( 0x100 ) > value )
        {
            size = 1;
        }
        else if ( INT64_C( 0x10000 ) > value )
        {
            size = 2;
        }
        else if ( INT64_C( 0x1000000 ) > value )
        {
            size = 3;
        }
        else if ( INT64_C( 0x100000000 ) > value )
        {
            size = 4;
        }
        else if ( INT64_C( 0x10000000000 ) > value )
        {
            size = 5;
        }
        else if ( INT64_C( 0x1000000000000 ) > value )
        {
            size = 6;
        }
        else if ( INT64_C( 0x100000000000000 ) > value )
        {
            size = 7;
        }
        else
        {
            size = 8;
        }
    }
    else
    {
        size = 8;
    }

    *bufferHandle->write_pos++ = size;

#ifdef WORDS_BIGENDIAN /* Store last bytes when big endian. */
    memcpy( bufferHandle->write_pos, ( const uint8_t* )&value + 8 - size, size );
#else /* Store first bytes when little endian. */
    memcpy( bufferHandle->write_pos, &value, size );
#endif      /* WORDS_BIGENDIAN. */

    bufferHandle->write_pos += size;
}


static inline void
OTF2_Buffer_WriteInt64Full( OTF2_Buffer*  bufferHandle,
                            const int64_t value )
{
    memcpy( bufferHandle->write_pos, &value, 8 );
    bufferHandle->write_pos += 8;
}


static inline void
OTF2_Buffer_WriteUint64( OTF2_Buffer*   bufferHandle,
                         const uint64_t value )
{
    uint8_t size = 0;

    /* special compression for special values */
    if ( value == 0 || value == UINT64_MAX )
    {
        *bufferHandle->write_pos++ = value & 0xff;
        return;
    }

    /* Get number of used bytes. */
    if ( UINT64_C( 0x100 ) > value )
    {
        size = 1;
    }
    else if ( UINT64_C( 0x10000 ) > value )
    {
        size = 2;
    }
    else if ( UINT64_C( 0x1000000 ) > value )
    {
        size = 3;
    }
    else if ( UINT64_C( 0x100000000 ) > value )
    {
        size = 4;
    }
    else if ( UINT64_C( 0x10000000000 ) > value )
    {
        size = 5;
    }
    else if ( UINT64_C( 0x1000000000000 ) > value )
    {
        size = 6;
    }
    else if ( UINT64_C( 0x100000000000000 ) > value )
    {
        size = 7;
    }
    else
    {
        size = 8;
    }

    *bufferHandle->write_pos++ = size;

#ifdef WORDS_BIGENDIAN /* Store last bytes when big endian. */
    memcpy( bufferHandle->write_pos, ( const uint8_t* )&value + 8 - size, size );
#else /* Store first bytes when little endian. */
    memcpy( bufferHandle->write_pos, &value, size );
#endif      /* WORDS_BIGENDIAN. */

    bufferHandle->write_pos += size;
}


static inline void
OTF2_Buffer_WriteUint64Full( OTF2_Buffer*   bufferHandle,
                             const uint64_t value )
{
    memcpy( bufferHandle->write_pos, &value, 8 );
    bufferHandle->write_pos += 8;
}


static inline void
OTF2_Buffer_WriteMetricValue( OTF2_Buffer*     bufferHandle,
                              OTF2_MetricValue value )
{
    UTILS_ASSERT( bufferHandle );
    OTF2_Buffer_WriteUint64( bufferHandle, value.unsigned_int );
}


static inline void
OTF2_Buffer_WriteFloat( OTF2_Buffer* bufferHandle,
                        const float  value )
{
    memcpy( bufferHandle->write_pos, &value, 4 );
    bufferHandle->write_pos += 4;
}


static inline void
OTF2_Buffer_WriteDouble( OTF2_Buffer* bufferHandle,
                         const double value )
{
    memcpy( bufferHandle->write_pos, &value, 8 );
    bufferHandle->write_pos += 8;
}


static inline void
OTF2_Buffer_WriteString( OTF2_Buffer* bufferHandle,
                         const char*  value )
{
    uint32_t str_length = strlen( value ) + 1;

    memcpy( bufferHandle->write_pos, value, str_length );

    bufferHandle->write_pos += str_length;
}
/** @} */


OTF2_ErrorCode
OTF2_Buffer_RequestNewChunk( OTF2_Buffer*   bufferHandle,
                             OTF2_TimeStamp time );

/** @brief Check if the next record fits into the current chunk.
 *
 *  There are 3 possibilities:
 *
 *  (1) It fits: Do nothing.
 *
 *  (2) It does not fit and is not the last chunk: Go to next chunk, write the
 *      chunk header, and a current timestamp.
 *
 *  (3) It does not fit and is the last chunk: Flush and reset the buffer.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param time             Current timestamp.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static inline OTF2_ErrorCode
OTF2_Buffer_RecordRequest( OTF2_Buffer*   bufferHandle,
                           OTF2_TimeStamp time,
                           uint64_t       recordLength )
{
    /* Add space for timestamp, if type implies a timestamp. */
    uint8_t size_time = 0;
    if ( otf2_file_type_has_timestamps( bufferHandle->file_type ) )
    {
        size_time = OTF2_BUFFER_TIMESTAMP_SIZE;
    }

    /* Check if record fits in the chunk. Furthermore ensure that there
     * is at least one byte left to write OTF2_BUFFER_END_OF_CHUNK. That's why
     * ( size_time + recordLength ) needs to be smaller than free_memory. */
    uint64_t free_memory = bufferHandle->chunk->end - bufferHandle->write_pos;
    if ( ( size_time + recordLength ) < free_memory )
    {
        return OTF2_SUCCESS;
    }

    return OTF2_Buffer_RequestNewChunk( bufferHandle, time );
}

/** @brief Request memory from buffer.
 *
 *  Before a record is written, it must be ensured that there is enough space
 *  left in the buffer. If there is not enough memory left, it is tried to
 *  allocate more memory or flush the buffer. Therefore, this call should always
 *  succeed even if a buffer is full. If the call returns an error, buffer
 *  handling failed and the recording should be aborted.
 *
 *  Please note: This function must only be called from definition writers. For
 *  event writers this functionality is included in OTF2_Buffer_WriteTimeStamp.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param recordLength     Size of requested record in bytes.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static inline OTF2_ErrorCode
OTF2_Buffer_WriteMemoryRequest( OTF2_Buffer* bufferHandle,
                                uint64_t     recordLength )
{
    /* Handle chunking and buffer overflow. */
    OTF2_ErrorCode status = OTF2_Buffer_RecordRequest( bufferHandle,
                                                       0, recordLength );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Chunk handling failed!" );
    }

    return OTF2_SUCCESS;
}

/** @brief Write a timestamp if neccesary.
 *
 *  Checks if the the passed timestamp is newer than the last written one. If so
 *  it writes the new timestamp to the buffer.
 *
 *  In addition it requests memory from the buffer.
 *  Before a record is written, it must be ensured that there is enough space
 *  left in the buffer. If there is not enough memory left, it is tried to
 *  allocate more memory or flush the buffer. Therefore, this call should always
 *  succeed even if a buffer is full. If the call returns an error, buffer
 *  handling failed and the recording should be aborted.
 *
 *  Please note: This function must only be called from event writers. For
 *  definition writers this functionality is included in
 *  OTF2_Buffer_WriteMemoryRequest.
 *
 *  @param bufferHandle     Initialized OTF2_Buffer instance.
 *  @param time             Time the event took place.
 *  @param recordLength     Size of requested record in bytes.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static inline OTF2_ErrorCode
OTF2_Buffer_WriteTimeStamp( OTF2_Buffer*         bufferHandle,
                            const OTF2_TimeStamp time,
                            uint64_t             recordLength )
{
    OTF2_ErrorCode status = OTF2_SUCCESS;

    /* Check the time order early, because we may write
       timestamps in OTF2_Buffer_RecordRequest, too */
    if ( time < bufferHandle->time )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Passed timestamp is smaller than last written one! (%" PRIu64 " < %" PRIu64 ")",
                            time, bufferHandle->time );
    }

    /* Handle chunking and buffer overflow. */
    status = OTF2_Buffer_RecordRequest( bufferHandle, time, recordLength );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Chunk handling failed!" );
    }

    /* Check if actual timestamp is newer than the last written one. If so write
     * it to the buffer otherwise return.
     * Enable to write a timestamp zero at the beginning of the buffer. */
    if ( ( time > bufferHandle->time ) ||
         ( time == 0 && time == bufferHandle->time ) )
    {
        OTF2_Buffer_WriteUint8( bufferHandle, OTF2_BUFFER_TIMESTAMP );
        OTF2_Buffer_WriteUint64Full( bufferHandle, time );

        bufferHandle->time = time;
    }


    /* Increment number of events. In writing mode only one counter is
       necessary. So only 'first_event' is incremented. */
    bufferHandle->chunk->first_event++;

    return OTF2_SUCCESS;
}


/** @brief Write initial value to record length byte.
 *
 *  This function must be called when a record length byte should be written.
 *  For the moment this functions sets the record length byte to zero, because
 *  correct record length is not known at this point. After writing the complete
 *  record OTF2_Buffer_WriteFinalRecordLength must be called to write correct
 *  record length.
 *
 *  @param bufferHandle          Initialized OTF2_Buffer instance.
 *  @param estimatedRecordLength Expected record length.
 */
static inline void
OTF2_Buffer_WriteInitialRecordLength( OTF2_Buffer* bufferHandle,
                                      uint64_t     estimatedRecordLength )
{
    /* Decision if we will use one byte or nine bytes to encode record length
     * depends on estimate of real record length */
    if ( estimatedRecordLength < UINT8_MAX )
    {
        /* Records with a length ranging from 0 to 254 bytes can be encoded in one byte */
        OTF2_Buffer_WriteUint8( bufferHandle, 0 );
    }
    else
    {
        /* For records larger than 254 bytes the record length byte will be
         * set to 0xFFh and the record length is encoded in the immediately
         * following eight bytes. */
        OTF2_Buffer_WriteUint8( bufferHandle, UINT8_MAX );
        OTF2_Buffer_WriteUint64Full( bufferHandle, 0 );
    }
    /* Store position where current record data starts */
    bufferHandle->record_data_pos = bufferHandle->write_pos;
}

/** @brief Write final value to record length byte.
 *
 *  This function must be called after initializing record length byte by
 *  calling OTF2_Buffer_WriteInitialRecordLength. This funtion will calculate
 *  record length and write the result to record length byte if the record
 *  uses less than 255 bytes. For records with a length of 255 bytes or
 *  larger the initial value of record length byte is left unchanged and
 *  length information is encoded in the immediately following 8 bytes.
 *
 *  @param bufferHandle          Initialized OTF2_Buffer instance.
 *  @param estimatedRecordLength Expected record length.
 */
static inline OTF2_ErrorCode
OTF2_Buffer_WriteFinalRecordLength( OTF2_Buffer* bufferHandle,
                                    uint64_t     estimatedRecordLength )
{
    /* Determine record length. */
    uint64_t record_length = bufferHandle->write_pos - bufferHandle->record_data_pos;

    if ( estimatedRecordLength < UINT8_MAX )
    {
        if ( record_length >= UINT8_MAX )
        {
            /* Wrong estimate of record length */
            return OTF2_ERROR_E2BIG;
        }
        /* Write record length byte (as uint8_t) one byte before the record
         * data was written. */
        uint8_t record_length_byte = record_length;
        memcpy( bufferHandle->record_data_pos - 1, &record_length_byte, 1 );
    }
    else
    {
        /* Special handling of records larger than 254 bytes,
         * record length byte remains unchanged,
         * length is encoded (as uint64_t) in the additional eight bytes
         * before the record data was written */
        memcpy( bufferHandle->record_data_pos - 8, &record_length, 8 );
    }
    /* Reset record data position */
    bufferHandle->record_data_pos = NULL;

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
OTF2_Buffer_FlushBuffer( OTF2_Buffer* bufferHandle );



/* ___ Read operations for essential data types. ____________________________ */



OTF2_ErrorCode
OTF2_Buffer_ReadTimeStamp( OTF2_Buffer*    bufferHandle,
                           OTF2_TimeStamp* req_time );

/** @{ */
void
OTF2_Buffer_ReadInt8( OTF2_Buffer* bufferHandle,
                      int8_t*      returnValue );

void
OTF2_Buffer_ReadUint8( OTF2_Buffer* bufferHandle,
                       uint8_t*     returnValue );

void
OTF2_Buffer_ReadInt16( OTF2_Buffer* bufferHandle,
                       int16_t*     returnValue );

void
OTF2_Buffer_ReadUint16( OTF2_Buffer* bufferHandle,
                        uint16_t*    returnValue );

OTF2_ErrorCode
OTF2_Buffer_ReadInt32( OTF2_Buffer* bufferHandle,
                       int32_t*     returnValue );

void
OTF2_Buffer_ReadInt32Full( OTF2_Buffer* bufferHandle,
                           int32_t*     returnValue );

OTF2_ErrorCode
OTF2_Buffer_ReadUint32( OTF2_Buffer* bufferHandle,
                        uint32_t*    returnValue );

void
OTF2_Buffer_ReadUint32Full( OTF2_Buffer* bufferHandle,
                            uint32_t*    returnValue );

OTF2_ErrorCode
OTF2_Buffer_ReadInt64( OTF2_Buffer* bufferHandle,
                       int64_t*     returnValue );

void
OTF2_Buffer_ReadInt64Full( OTF2_Buffer* bufferHandle,
                           int64_t*     returnValue );

OTF2_ErrorCode
OTF2_Buffer_ReadUint64( OTF2_Buffer* bufferHandle,
                        uint64_t*    returnValue );

void
OTF2_Buffer_ReadUint64Full( OTF2_Buffer* bufferHandle,
                            uint64_t*    returnValue );

void
OTF2_Buffer_ReadFloat( OTF2_Buffer* bufferHandle,
                       float*       returnValue );

void
OTF2_Buffer_ReadDouble( OTF2_Buffer* bufferHandle,
                        double*      returnValue );

OTF2_ErrorCode
OTF2_Buffer_ReadMetricValue( OTF2_Buffer*      bufferHandle,
                             OTF2_MetricValue* returnValue );

OTF2_ErrorCode
OTF2_Buffer_ReadString( OTF2_Buffer* bufferHandle,
                        const char** returnValue );

/** @} */

/* ___ Skip operations for essential data types. ____________________________ */



OTF2_ErrorCode
OTF2_Buffer_Skip( OTF2_Buffer* bufferHandle,
                  uint64_t     size );

OTF2_ErrorCode
OTF2_Buffer_SkipCompressed( OTF2_Buffer* bufferHandle );

/* ___ Additional get and set routines. _____________________________________ */



OTF2_ErrorCode
OTF2_Buffer_ReadGetNextChunk( OTF2_Buffer* bufferHandle );

OTF2_ErrorCode
OTF2_Buffer_ReadGetPreviousChunk( OTF2_Buffer* bufferHandle );

OTF2_ErrorCode
OTF2_Buffer_ReadSeekChunk( OTF2_Buffer* bufferHandle,
                           uint64_t     position );

OTF2_ErrorCode
OTF2_Buffer_ReadSeekChunkTime( OTF2_Buffer* bufferHandle,
                               uint64_t     time,
                               bool*        found );

void
OTF2_Buffer_GetPosition( OTF2_Buffer* bufferHandle,
                         uint8_t**    position );

OTF2_ErrorCode
OTF2_Buffer_SetPosition( OTF2_Buffer* bufferHandle,
                         uint8_t*     position );


OTF2_ErrorCode
OTF2_Buffer_GetBeginOfChunk( OTF2_Buffer* bufferHandle,
                             uint8_t**    position );

OTF2_ErrorCode
OTF2_Buffer_GetNumberEvents( OTF2_Buffer* bufferHandle,
                             uint64_t*    firstEvent,
                             uint64_t*    lastEvent );


OTF2_ErrorCode
OTF2_Buffer_GetPositionTimeStamp( OTF2_Buffer* bufferHandle,
                                  uint8_t**    timePos );

OTF2_ErrorCode
OTF2_Buffer_SetPositionTimeStamp( OTF2_Buffer* bufferHandle,
                                  uint8_t*     timePos );

OTF2_ErrorCode
OTF2_Buffer_RewriteTimeStamp( OTF2_Buffer*   bufferHandle,
                              OTF2_TimeStamp time );



/* ___ Routines to manage the rewind list. __________________________________ */


OTF2_ErrorCode
OTF2_Buffer_StoreRewindPoint( OTF2_Buffer* bufferHandle,
                              uint32_t     id );

OTF2_ErrorCode
OTF2_Buffer_ClearRewindPoint( OTF2_Buffer* bufferHandle,
                              uint32_t     id );

OTF2_ErrorCode
OTF2_Buffer_Rewind( OTF2_Buffer* bufferHandle,
                    uint32_t     id );


/* ___ Routines to check buffer consistency . _______________________________ */


OTF2_ErrorCode
OTF2_Buffer_GuaranteeRecord( OTF2_Buffer* bufferHandle,
                             uint64_t*    record_length );

OTF2_ErrorCode
OTF2_Buffer_GuaranteeCompressed( OTF2_Buffer* bufferHandle );

OTF2_ErrorCode
OTF2_Buffer_Guarantee( OTF2_Buffer* bufferHandle,
                       uint64_t     size );


#endif /* !OTF2_BUFFER_H */
