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
 *  @file       src/otf2_compression_no_zlib.c
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @brief      Empty compression layer for zlib.
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


bool
OTF2_Compression_Zlib_Available( void )
{
    return false;
}


OTF2_ErrorCode
OTF2_Compression_Zlib_Compress( const void*    buffer,
                                const uint64_t size,
                                void**         compBuffer,
                                uint32_t*      compSize )
{
    return UTILS_ERROR( OTF2_ERROR_FILE_COMPRESSION_NOT_SUPPORTED,
                        "Using zlib without library support." );
}


OTF2_ErrorCode
OTF2_Compression_Zlib_Decompress( const void*    compBuffer,
                                  const uint64_t compSize,
                                  void**         buffer,
                                  uint32_t       size )
{
    return UTILS_ERROR( OTF2_ERROR_FILE_COMPRESSION_NOT_SUPPORTED,
                        "Using zlib without library support." );
}
