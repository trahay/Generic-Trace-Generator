/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2012,
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
 *  @file       src/OTF2_Compression_Zlib.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer is completely internal to the OTF2 library and should
 *              not be exposed to the end user. Its purpose is to abstract data
 *              compression with a specific compression library.
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

#include "OTF2_Compression_Zlib.h"
#include "zlib.h"


/** @brief Defines compression level for zlib.
 *  Compression levels for zlib are 0 (no compression) to 9. */
#define OTF2_Z_LEVEL 4


/** @brief Defines size of compression buffer reserve.
 *  The size of compressed content does not need to be smaller than the
 *  uncompressed content. In worst case it can even consume more memory the the
 *  uncompressed data. Therefore, all compression buffers have this small
 *  reserve. */
#define OTF2_Z_BUFFER_RESERVE ( 10 * 1024 )



bool
OTF2_Compression_Zlib_Available( void )
{
    return true;
}


/** @brief Compress a memory buffer.
 *
 *  Reads the size of the current and previous compression block.
 *
 *  @param buffer           Memory buffer containing the uncompressed data.
 *  @param size             Size of the uncompressed data.
 *  @param compBuffer       Return pointer to the buffer containing the
 *                          compressed data.
 *  @param compSize         Size of the compressed data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Compression_Zlib_Compress( const void*    buffer,
                                const uint64_t size,
                                void**         compBuffer,
                                uint32_t*      compSize )
{
    /* Initialize zlib stream handle. */
    z_stream stream;


    /* Initialize memory allocation to Z_NULL i.e. use zlib's internal
     * allocation methods. */
    stream.zalloc = Z_NULL;
    stream.zfree  = Z_NULL;
    stream.opaque = Z_NULL;

    int status = deflateInit( &stream, OTF2_Z_LEVEL );
    if ( status != Z_OK )
    {
        return UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                            "Zlib call 'deflateInit()' failed!" );
    }


    /* Define input and output buffers and input and output sizes. */
    *compBuffer = ( void* )malloc( size + OTF2_Z_BUFFER_RESERVE );
    if ( *compBuffer == NULL )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Allocation of compression buffer failed!" );
    }

    stream.next_in   = buffer;
    stream.avail_in  = size;
    stream.next_out  = *compBuffer;
    stream.avail_out = size + OTF2_Z_BUFFER_RESERVE;


    /* Call compression routine. */
    status = deflate( &stream, Z_FULL_FLUSH );

    if ( status == Z_STREAM_ERROR )
    {
        free( *compBuffer );
        return UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                            "Zlib call 'deflate()' failed!" );
    }

    /* Check if all data of the input buffer was compressed. */
    if ( stream.avail_in > 0 )
    {
        free( *compBuffer );
        return UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                            "Zlib call 'deflate()' failed!" );
    }

    *compSize = size + OTF2_Z_BUFFER_RESERVE - stream.avail_out;


    /* Finalize deflate. */
    deflateEnd( &stream );

    return OTF2_SUCCESS;
}


/** @brief Compress a memory buffer.
 *
 *  Reads the size of the current and previous compression block.
 *
 *  @param compBuffer       Memory buffer containing the compressed data.
 *  @param compSize         Size of the compressed data.
 *  @param buffer           Return pointer to the buffer containing the
 *                          uncompressed data.
 *  @param size             Size of buffer the uncompressed data should go to.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_Compression_Zlib_Decompress( const void*    compBuffer,
                                  const uint64_t compSize,
                                  void**         buffer,
                                  uint32_t       size )
{
    /* Initialize zlib stream handle. */
    z_stream stream;


    /* Initialize memory allocation to Z_NULL i.e. use zlib's internal
     * allocation methods. */
    stream.zalloc = Z_NULL;
    stream.zfree  = Z_NULL;
    stream.opaque = Z_NULL;

    int status = inflateInit( &stream );
    if ( status != Z_OK )
    {
        return UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                            "Zlib call 'deflateInit()' failed!" );
    }


    /* Define input and output buffers and input and output sizes. */
    stream.next_in   = compBuffer;
    stream.avail_in  = compSize;
    stream.next_out  = *buffer;
    stream.avail_out = size;


    /* Call compression routine. */
    status = inflate( &stream, Z_FULL_FLUSH );

    if ( status == Z_STREAM_ERROR )
    {
        return UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                            "Zlib call 'deflate()' failed!" );
    }

    /* Check if all data of the input buffer was compressed. */
    if ( stream.avail_out > 0 )
    {
        return UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                            "Zlib call 'deflate()' failed!" );
    }

    /* Finalize deflate. */
    inflateEnd( &stream );

    return OTF2_SUCCESS;
}
