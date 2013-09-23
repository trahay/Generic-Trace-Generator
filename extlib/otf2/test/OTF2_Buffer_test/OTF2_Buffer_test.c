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
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Fills the buffer with a defined data set, reads it back and
 *              compares the results.
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>

#include <otf2_internal.h>
#include <otf2_reader.h>
#include <otf2_archive.h>

#include <otf2_events_int.h>
#include <OTF2_File.h>
#include <OTF2_Buffer.h>

/* ___ Global variables. ____________________________________________________ */



/** @brief Defines if debug is turned on (1) or off (0). */
static bool otf2_DEBUG;


/* ___ Prototypes for static functions. _____________________________________ */



static OTF2_FlushType
pre_flush( void*         userData,
           OTF2_FileType fileType,
           uint64_t      locationId,
           void*         callerData,
           bool          final );

static OTF2_TimeStamp
post_flush( void*         userData,
            OTF2_FileType fileType,
            uint64_t      locationId );

static OTF2_FlushCallbacks flush_callbacks =
{
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
};

static uint64_t
get_time( void );

static inline void
check_pointer( void* pointer,
               char* description,
               ... );

static inline void
check_status( OTF2_ErrorCode status,
              char*          description,
              ... );

static inline void
check_boolean( bool  truth_value,
               char* description,
               ... );


/* ___ main _________________________________________________________________ */

#define DO_RECORD( rs )    { .type = OTF2_TYPE_NONE,   .full = 0,  .u.u64 = rs }
#define DO_UINT8( v )      { .type = OTF2_TYPE_UINT8,  .full = 0,  .u.u8  = UINT8_C( v ) }
#define DO_INT8( v )       { .type = OTF2_TYPE_INT8,   .full = 0,  .u.s8  = INT8_C( v ) }
#define DO_UINT8FULL( v )  { .type = OTF2_TYPE_UINT8,  .full = 1,  .u.u8  = UINT8_C( v ) }
#define DO_INT8FULL( v )   { .type = OTF2_TYPE_INT8,   .full = 1,  .u.s8  = INT8_C( v ) }
#define DO_UINT16( v )     { .type = OTF2_TYPE_UINT16, .full = 0,  .u.u16 = UINT16_C( v ) }
#define DO_INT16( v )      { .type = OTF2_TYPE_INT16,  .full = 0,  .u.s16 = INT16_C( v ) }
#define DO_UINT16FULL( v ) { .type = OTF2_TYPE_UINT16, .full = 1,  .u.u16 = UINT16_C( v ) }
#define DO_INT16FULL( v )  { .type = OTF2_TYPE_INT16,  .full = 1,  .u.s16 = INT16_C( v ) }
#define DO_UINT32( v )     { .type = OTF2_TYPE_UINT32, .full = 0,  .u.u32 = UINT32_C( v ) }
#define DO_INT32( v )      { .type = OTF2_TYPE_INT32,  .full = 0,  .u.s32 = INT32_C( v ) }
#define DO_UINT32FULL( v ) { .type = OTF2_TYPE_UINT32, .full = 1,  .u.u32 = UINT32_C( v ) }
#define DO_INT32FULL( v )  { .type = OTF2_TYPE_INT32,  .full = 1,  .u.s32 = INT32_C( v ) }
#define DO_UINT64( v )     { .type = OTF2_TYPE_UINT64, .full = 0,  .u.u64 = UINT64_C( v ) }
#define DO_INT64( v )      { .type = OTF2_TYPE_INT64,  .full = 0,  .u.s64 = INT64_C( v ) }
#define DO_UINT64FULL( v ) { .type = OTF2_TYPE_UINT64, .full = 1,  .u.u64 = UINT64_C( v ) }
#define DO_INT64FULL( v )  { .type = OTF2_TYPE_INT64,  .full = 1,  .u.s64 = INT64_C( v ) }

union buffer_value
{
    uint8_t  u8;
    int8_t   s8;
    uint16_t u16;
    int16_t  s16;
    uint32_t u32;
    int32_t  s32;
    uint64_t u64;
    int64_t  s64;
};

struct buffer_action
{
    /* we use OTF2_TYPE_NONE for a new record */
    OTF2_Type          type;
    /* don't use compression for this type, does not make sense for all types */
    uint8_t            full;
    union buffer_value u;
} test_script[] = {
    DO_RECORD( 4 * 1 ),
    /* 0 bytes significant  */
    DO_UINT8( 0 ),
    DO_UINT8FULL( 0 ),
    /* 1 bytes significant  */
    DO_UINT8( 64 ),
    DO_UINT8FULL( 64 ),

    DO_RECORD( 4 * 1 ),
    /* 0 bytes significant  */
    DO_INT8( 0 ),
    DO_INT8FULL( 0 ),
    /* 1 bytes significant  */
    DO_INT8(  64 ),
    DO_INT8FULL(  64 ),

    DO_RECORD( 2 * 1 ),
    /* 1 bytes significant  */
    DO_INT8( -64 ),
    DO_INT8FULL( -64 ),

    DO_RECORD( 6 * 2 ),
    /* 0 bytes significant  */
    DO_UINT16( 0 ),
    DO_UINT16FULL( 0 ),
    /* 1 bytes significant  */
    DO_UINT16( 128 ),
    DO_UINT16FULL( 128 ),
    /* 2 bytes significant  */
    DO_UINT16( 16512 ),
    DO_UINT16FULL( 16512 ),

    DO_RECORD( 6 * 2 ),
    /* 0 bytes significant  */
    DO_INT16( 0 ),
    DO_INT16FULL( 0 ),
    /* 1 bytes significant  */
    DO_INT16( 128 ),
    DO_INT16FULL( 128 ),
    /* 2 bytes significant  */
    DO_INT16( 16512 ),
    DO_INT16FULL( 16512 ),

    DO_RECORD( 4 * 2 ),
    /* 1 bytes significant  */
    DO_INT16( -128 ),
    DO_INT16FULL( -128 ),
    /* 2 bytes significant  */
    DO_INT16( -16512 ),
    DO_INT16FULL( -16512 ),

    DO_RECORD( 5 * 5 + 5 * 4 ),
    /* 0 bytes significant  */
    DO_UINT32( 0 ),
    DO_UINT32FULL( 0 ),
    /* 1 bytes significant  */
    DO_UINT32( 128 ),
    DO_UINT32FULL( 128 ),
    /* 2 bytes significant  */
    DO_UINT32( 16512 ),
    DO_UINT32FULL( 16512 ),
    /* 3 bytes significant  */
    DO_UINT32( 2113664 ),
    DO_UINT32FULL( 2113664 ),
    /* 4 bytes significant  */
    DO_UINT32( 270549120 ),
    DO_UINT32FULL( 270549120 ),

    DO_RECORD( 5 * 5 + 5 * 4 ),
    /* 0 bytes significant  */
    DO_INT32( 0 ),
    DO_INT32FULL( 0 ),
    /* 1 bytes significant  */
    DO_INT32( 128 ),
    DO_INT32FULL( 128 ),
    /* 2 bytes significant  */
    DO_INT32( 16512 ),
    DO_INT32FULL( 16512 ),
    /* 3 bytes significant  */
    DO_INT32( 2113664 ),
    DO_INT32FULL( 2113664 ),
    /* 4 bytes significant  */
    DO_INT32( 270549120 ),
    DO_INT32FULL( 270549120 ),

    DO_RECORD( 4 * 5 + 4 * 4 ),
    /* 1 bytes significant  */
    DO_INT32( -128 ),
    DO_INT32FULL( -128 ),
    /* 2 bytes significant  */
    DO_INT32( -16512 ),
    DO_INT32FULL( -16512 ),
    /* 3 bytes significant  */
    DO_INT32( -2113664 ),
    DO_INT32FULL( -2113664 ),
    /* 4 bytes significant  */
    DO_INT32( -270549120 ),
    DO_INT32FULL( -270549120 ),

    DO_RECORD( 9 * 9 + 9 * 8 ),
    /* 0 bytes significant  */
    DO_UINT64( 0 ),
    DO_UINT64FULL( 0 ),
    /* 1 bytes significant  */
    DO_UINT64( 128 ),
    DO_UINT64FULL( 128 ),
    /* 2 bytes significant  */
    DO_UINT64( 16512 ),
    DO_UINT64FULL( 16512 ),
    /* 3 bytes significant  */
    DO_UINT64( 2113664 ),
    DO_UINT64FULL( 2113664 ),
    /* 4 bytes significant  */
    DO_UINT64( 270549120 ),
    DO_UINT64FULL( 270549120 ),
    /* 5 bytes significant  */
    DO_UINT64( 34630287488 ),
    DO_UINT64FULL( 34630287488 ),
    /* 6 bytes significant  */
    DO_UINT64( 4432676798592 ),
    DO_UINT64FULL( 4432676798592 ),
    /* 7 bytes significant  */
    DO_UINT64( 567382630219904 ),
    DO_UINT64FULL( 567382630219904 ),
    /* 8 bytes significant  */
    DO_UINT64( 72624976668147840 ),
    DO_UINT64FULL( 72624976668147840 ),

    DO_RECORD( 9 * 9 + 9 * 8 ),
    /* 0 bytes significant  */
    DO_INT64( 0 ),
    DO_INT64FULL( 0 ),
    /* 1 bytes significant  */
    DO_INT64( 128 ),
    DO_INT64FULL( 128 ),
    /* 2 bytes significant  */
    DO_INT64( 16512 ),
    DO_INT64FULL( 16512 ),
    /* 3 bytes significant  */
    DO_INT64( 2113664 ),
    DO_INT64FULL( 2113664 ),
    /* 4 bytes significant  */
    DO_INT64( 270549120 ),
    DO_INT64FULL( 270549120 ),
    /* 5 bytes significant  */
    DO_INT64( 34630287488 ),
    DO_INT64FULL( 34630287488 ),
    /* 6 bytes significant  */
    DO_INT64( 4432676798592 ),
    DO_INT64FULL( 4432676798592 ),
    /* 7 bytes significant  */
    DO_INT64( 567382630219904 ),
    DO_INT64FULL( 567382630219904 ),
    /* 8 bytes significant  */
    DO_INT64( 72624976668147840 ),
    DO_INT64FULL( 72624976668147840 ),

    DO_RECORD( 8 * 9 + 8 * 8 ),
    /* 1 bytes significant  */
    DO_INT64( -128 ),
    DO_INT64FULL( -128 ),
    /* 2 bytes significant  */
    DO_INT64( -16512 ),
    DO_INT64FULL( -16512 ),
    /* 3 bytes significant  */
    DO_INT64( -2113664 ),
    DO_INT64FULL( -2113664 ),
    /* 4 bytes significant  */
    DO_INT64( -270549120 ),
    DO_INT64FULL( -270549120 ),
    /* 5 bytes significant  */
    DO_INT64( -34630287488 ),
    DO_INT64FULL( -34630287488 ),
    /* 6 bytes significant  */
    DO_INT64( -4432676798592 ),
    DO_INT64FULL( -4432676798592 ),
    /* 7 bytes significant  */
    DO_INT64( -567382630219904 ),
    DO_INT64FULL( -567382630219904 ),
    /* 8 bytes significant  */
    DO_INT64( -72624976668147840 ),
    DO_INT64FULL( -72624976668147840 )
};
#define SCRIPT_SIZE ( sizeof( test_script ) / sizeof( test_script[ 0 ] ) )

#define OTF2_BUFFER_TEST_EVENT 0xff

static void
apply_script_to_buffer( OTF2_Buffer* buffer, uint32_t repeatCount )
{
    OTF2_ErrorCode status;

    for ( uint32_t r = 0; r < repeatCount; r++ )
    {
        for ( size_t i = 0; i < SCRIPT_SIZE; i++ )
        {
            switch ( test_script[ i ].type )
            {
                case OTF2_TYPE_NONE:
                    status = OTF2_Buffer_WriteTimeStamp( buffer,
                                                         get_time(),
                                                         test_script[ i ].u.u64 );
                    check_status( status, "Write timestamp" );

                    /* write artificial event type, to be able to recognize the end of chunk */
                    OTF2_Buffer_WriteUint8( buffer, OTF2_BUFFER_TEST_EVENT );

                    break;

                case OTF2_TYPE_UINT8:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_WriteUint8Full( buffer, test_script[ i ].u.u8 );
                    }
                    else
                    {
                        OTF2_Buffer_WriteUint8( buffer, test_script[ i ].u.u8 );
                    }
                    break;

                case OTF2_TYPE_INT8:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_WriteInt8Full( buffer, test_script[ i ].u.s8 );
                    }
                    else
                    {
                        OTF2_Buffer_WriteInt8( buffer, test_script[ i ].u.s8 );
                    }
                    break;

                case OTF2_TYPE_UINT16:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_WriteUint16Full( buffer, test_script[ i ].u.u16 );
                    }
                    else
                    {
                        OTF2_Buffer_WriteUint16( buffer, test_script[ i ].u.u16 );
                    }
                    break;

                case OTF2_TYPE_INT16:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_WriteInt16Full( buffer, test_script[ i ].u.s16 );
                    }
                    else
                    {
                        OTF2_Buffer_WriteInt16( buffer, test_script[ i ].u.s16 );
                    }
                    break;

                case OTF2_TYPE_UINT32:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_WriteUint32Full( buffer, test_script[ i ].u.u32 );
                    }
                    else
                    {
                        OTF2_Buffer_WriteUint32( buffer, test_script[ i ].u.u32 );
                    }
                    break;

                case OTF2_TYPE_INT32:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_WriteInt32Full( buffer, test_script[ i ].u.s32 );
                    }
                    else
                    {
                        OTF2_Buffer_WriteInt32( buffer, test_script[ i ].u.s32 );
                    }
                    break;

                case OTF2_TYPE_UINT64:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_WriteUint64Full( buffer, test_script[ i ].u.u64 );
                    }
                    else
                    {
                        OTF2_Buffer_WriteUint64( buffer, test_script[ i ].u.u64 );
                    }
                    break;

                case OTF2_TYPE_INT64:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_WriteInt64Full( buffer, test_script[ i ].u.s64 );
                    }
                    else
                    {
                        OTF2_Buffer_WriteInt64( buffer, test_script[ i ].u.s64 );
                    }
                    break;
            }
        }
    }
}


static void
check_script_with_buffer( OTF2_Buffer* buffer, uint32_t repeatCount )
{
    OTF2_ErrorCode     status;
    uint8_t*           read_pos;
    union buffer_value u;

    for ( uint32_t r = 0; r < repeatCount; r++ )
    {
        for ( size_t i = 0; i < SCRIPT_SIZE; i++ )
        {
            switch ( test_script[ i ].type )
            {
                case OTF2_TYPE_NONE:

                    OTF2_Buffer_GetPosition( buffer, &read_pos );

                    if ( *read_pos == OTF2_BUFFER_TIMESTAMP )
                    {
                        status = OTF2_Buffer_ReadTimeStamp( buffer,
                                                            &u.u64 );
                        check_status( status, "Read timestamp" );
                        /* can't check timestamp, because of flushing */

                        OTF2_Buffer_ReadUint8( buffer, &u.u8 );
                        if ( u.u8 == OTF2_EVENT_BUFFER_FLUSH )
                        {
                            OTF2_Buffer_ReadUint8( buffer, &u.u8 );
                            OTF2_Buffer_ReadUint64Full( buffer, &u.u64 );
                            OTF2_Buffer_ReadUint8( buffer, &u.u8 );
                        }
                        if ( u.u8 != OTF2_BUFFER_TEST_EVENT )
                        {
                            check_status( OTF2_ERROR_PROCESSED_WITH_FAULTS, "Read event type" );
                        }
                    }
                    else if ( *read_pos == OTF2_EVENT_BUFFER_FLUSH )
                    {
                        /* consume the event type */
                        OTF2_Buffer_ReadUint8( buffer, &u.u8 );
                        OTF2_Buffer_ReadUint64Full( buffer, &u.u64 );
                    }
                    else if ( *read_pos == OTF2_BUFFER_END_OF_CHUNK )
                    {
                        status = OTF2_Buffer_ReadGetNextChunk( buffer );
                        check_status( status, "Loading next chunk" );
                        /* redo the current step, ie. read in the timestamp */
                        --i;
                        continue;
                    }
                    else
                    {
                        check_status( OTF2_ERROR_PROCESSED_WITH_FAULTS, "Read event type" );
                    }

                    break;

                case OTF2_TYPE_UINT8:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_ReadUint8Full( buffer, &u.u8 );
                        check_boolean( test_script[ i ].u.u8 == u.u8,
                                       "Full uint8_t mismatch: "
                                       "got %" PRIu64 ", expected %" PRIu64 ".",
                                       u.u8, test_script[ i ].u.u8 );
                    }
                    else
                    {
                        OTF2_Buffer_ReadUint8( buffer, &u.u8 );
                        check_boolean( test_script[ i ].u.u8 == u.u8,
                                       "uint8_t mismatch: "
                                       "got %" PRIu8 ", expected %" PRIu8 ".",
                                       u.u8, test_script[ i ].u.u8 );
                    }
                    break;

                case OTF2_TYPE_INT8:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_ReadInt8Full( buffer, &u.s8 );
                        check_boolean( test_script[ i ].u.s8 == u.s8,
                                       "Full int8_t mismatch: "
                                       "got %" PRId8 ", expected %" PRId8 ".",
                                       u.s8, test_script[ i ].u.s8 );
                    }
                    else
                    {
                        OTF2_Buffer_ReadInt8( buffer, &u.s8 );
                        check_boolean( test_script[ i ].u.s8 == u.s8,
                                       "int8_t mismatch: "
                                       "got %" PRId8 ", expected %" PRId8 ".",
                                       u.s8, test_script[ i ].u.s8 );
                    }
                    break;

                case OTF2_TYPE_UINT16:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_ReadUint16Full( buffer, &u.u16 );
                        check_boolean( test_script[ i ].u.u16 == u.u16,
                                       "Full uint16_t mismatch: "
                                       "got %" PRIu16 ", expected %" PRIu16 ".",
                                       u.u16, test_script[ i ].u.u16 );
                    }
                    else
                    {
                        OTF2_Buffer_ReadUint16( buffer, &u.u16 );
                        check_boolean( test_script[ i ].u.u16 == u.u16,
                                       "uint16_t mismatch: "
                                       "got %" PRIu16 ", expected %" PRIu16 ".",
                                       u.u16, test_script[ i ].u.u16 );
                    }
                    break;

                case OTF2_TYPE_INT16:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_ReadInt16Full( buffer, &u.s16 );
                        check_boolean( test_script[ i ].u.s16 == u.s16,
                                       "Full int16_t mismatch: "
                                       "got %" PRId16 ", expected %" PRId16 ".",
                                       u.s16, test_script[ i ].u.s16 );
                    }
                    else
                    {
                        OTF2_Buffer_ReadInt16( buffer, &u.s16 );
                        check_boolean( test_script[ i ].u.s16 == u.s16,
                                       "int16_t mismatch: "
                                       "got %" PRId16 ", expected %" PRId16 ".",
                                       u.s16, test_script[ i ].u.s16 );
                    }
                    break;

                case OTF2_TYPE_UINT32:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_ReadUint32Full( buffer, &u.u32 );
                        check_boolean( test_script[ i ].u.u32 == u.u32,
                                       "Full uint32_t mismatch: "
                                       "got %" PRIu32 ", expected %" PRIu32 ".",
                                       u.u32, test_script[ i ].u.u32 );
                    }
                    else
                    {
                        OTF2_Buffer_ReadUint32( buffer, &u.u32 );
                        check_boolean( test_script[ i ].u.u32 == u.u32,
                                       "uint32_t mismatch: "
                                       "got %" PRIu32 ", expected %" PRIu32 ".",
                                       u.u32, test_script[ i ].u.u32 );
                    }
                    break;

                case OTF2_TYPE_INT32:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_ReadInt32Full( buffer, &u.s32 );
                        check_boolean( test_script[ i ].u.s32 == u.s32,
                                       "Full int32_t mismatch: "
                                       "got %" PRId32 ", expected %" PRId32 ".",
                                       u.s32, test_script[ i ].u.s32 );
                    }
                    else
                    {
                        OTF2_Buffer_ReadInt32( buffer, &u.s32 );
                        check_boolean( test_script[ i ].u.s32 == u.s32,
                                       "int32_t mismatch: "
                                       "got %" PRId32 ", expected %" PRId32 ".",
                                       u.s32, test_script[ i ].u.s32 );
                    }
                    break;

                case OTF2_TYPE_UINT64:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_ReadUint64Full( buffer, &u.u64 );
                        check_boolean( test_script[ i ].u.u64 == u.u64,
                                       "Full uint64_t mismatch: "
                                       "got %" PRIu64 ", expected %" PRIu64 ".",
                                       u.u64, test_script[ i ].u.u64 );
                    }
                    else
                    {
                        OTF2_Buffer_ReadUint64( buffer, &u.u64 );
                        check_boolean( test_script[ i ].u.u64 == u.u64,
                                       "uint64_t mismatch: "
                                       "got %" PRIu64 ", expected %" PRIu64 ".",
                                       u.u64, test_script[ i ].u.u64 );
                    }
                    break;

                case OTF2_TYPE_INT64:
                    if ( test_script[ i ].full )
                    {
                        OTF2_Buffer_ReadInt64Full( buffer, &u.s64 );
                        check_boolean( test_script[ i ].u.s64 == u.s64,
                                       "Full int64_t mismatch: "
                                       "got %" PRId64 ", expected %" PRId64 ".",
                                       u.s64, test_script[ i ].u.s64 );
                    }
                    else
                    {
                        OTF2_Buffer_ReadInt64( buffer, &u.s64 );
                        check_boolean( test_script[ i ].u.s64 == u.s64,
                                       "int64_t mismatch: "
                                       "got %" PRId64 ", expected %" PRId64 ".",
                                       u.s64, test_script[ i ].u.s64 );
                    }
                    break;
            }
        }
    }
}

#define OTF2_BUFFER_TEST_CHUNK_SIZE ( 256 * 1024 )
#define OTF2_BUFFER_TEST_NUMBER_OF_CHUNKS 4
struct otf2_chunk_pool
{
    uint8_t chunks[ OTF2_BUFFER_TEST_NUMBER_OF_CHUNKS ]
    [ OTF2_BUFFER_TEST_CHUNK_SIZE ];
    size_t  next_free;
};

static void*
allocate_chunk( void*         userData,
                OTF2_FileType fileType,
                uint64_t      locationId,
                void**        perBufferData,
                uint64_t      chunkSize )
{
    check_boolean( chunkSize == OTF2_BUFFER_TEST_CHUNK_SIZE,
                   "Requested chunk size match." );

    if ( !*perBufferData )
    {
        *perBufferData = calloc( 1, sizeof( struct otf2_chunk_pool ) );
        check_pointer( *perBufferData, "Allocating chunk pool for buffer." );
    }
    struct otf2_chunk_pool* chunk_pool = *perBufferData;

    if ( chunk_pool->next_free == OTF2_BUFFER_TEST_NUMBER_OF_CHUNKS )
    {
        return NULL;
    }

    return chunk_pool->chunks[ chunk_pool->next_free++ ];
}

static void
free_all_chunks( void*         userData,
                 OTF2_FileType fileType,
                 uint64_t      locationId,
                 void**        perBufferData,
                 bool          final )
{
    check_pointer( *perBufferData, "Buffer chunk pool." );

    struct otf2_chunk_pool* chunk_pool = *perBufferData;

    chunk_pool->next_free = 0;

    if ( final )
    {
        free( chunk_pool );
        *perBufferData = NULL;
    }
}

static OTF2_MemoryCallbacks memory_callbacks =
{
    .otf2_allocate = allocate_chunk,
    .otf2_free_all = free_all_chunks
};

static void
write_buffer( OTF2_Archive* archive,
              uint32_t      repeatCount )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* Open a new buffer handle. */
    OTF2_Buffer* buffer = OTF2_Buffer_New( archive,
                                           NULL,
                                           OTF2_BUFFER_TEST_CHUNK_SIZE,
                                           OTF2_BUFFER_WRITE,
                                           OTF2_BUFFER_CHUNKED,
                                           OTF2_FILETYPE_EVENTS,
                                           repeatCount );
    check_pointer( buffer, "Create new buffer handle" );

    /* Fill the buffer with some data.
     * Can not run over chunk border, because switching to read fails then. */
    apply_script_to_buffer( buffer, repeatCount );

    /* Switch to reading and read data back in. */
    status = OTF2_Buffer_Delete( buffer );
    check_status( status, "Close buffer" );
}

static void
write_archive( const char*     archivePath,
               const char*     archiveName,
               const uint32_t* repeats,
               uint32_t        nRepeats )
{
    OTF2_ErrorCode status;

    OTF2_Archive* archive = OTF2_Archive_Open( archivePath,
                                               archiveName,
                                               OTF2_FILEMODE_WRITE,
                                               OTF2_BUFFER_TEST_CHUNK_SIZE,
                                               4 * 1024 * 1024,
                                               OTF2_SUBSTRATE_POSIX,
                                               OTF2_COMPRESSION_NONE );
    check_pointer( archive, "Create writer archive handle" );

    status = OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
    check_status( status, "Set flush callbacks." );
    status = OTF2_Archive_SetMemoryCallbacks( archive, &memory_callbacks, NULL );
    check_status( status, "Set memory callbacks." );
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set archive to master mode." );

    status = otf2_archive_set_number_of_locations( archive, nRepeats );
    check_status( status, "Set number of locations." );
    status = otf2_archive_set_number_of_global_defs( archive, 0 );
    check_status( status, "Set number of global definitions." );

    for ( size_t i = 0; i < nRepeats; i++ )
    {
        printf( "Writing %u in %s\n", repeats[ i ], archiveName );
        write_buffer( archive, repeats[ i ] );
    }

    OTF2_Archive_Close( archive );
}

static void
read_buffer( OTF2_Archive* archive, uint32_t repeatCount )
{
    OTF2_ErrorCode status;

    /* Open a new buffer handle. */
    OTF2_Buffer* buffer = OTF2_Buffer_New( archive,
                                           NULL,
                                           OTF2_BUFFER_TEST_CHUNK_SIZE,
                                           OTF2_BUFFER_READ,
                                           OTF2_BUFFER_CHUNKED,
                                           OTF2_FILETYPE_EVENTS,
                                           repeatCount );
    check_pointer( buffer, "Create buffer handle in read mode" );

    check_script_with_buffer( buffer, repeatCount );

    /* Delete buffer. */
    status = OTF2_Buffer_Delete( buffer );
    check_status( status, "Delete buffer." );
}

static void
read_archive( const char*     archivePath,
              const char*     archiveName,
              const uint32_t* repeats,
              uint32_t        nRepeats )
{
    OTF2_Archive* archive = OTF2_Archive_Open( archivePath,
                                               archiveName,
                                               OTF2_FILEMODE_READ,
                                               OTF2_UNDEFINED_UINT64,
                                               OTF2_UNDEFINED_UINT64,
                                               OTF2_SUBSTRATE_UNDEFINED,
                                               OTF2_COMPRESSION_UNDEFINED );
    check_pointer( archive, "Create reader archive handle" );

    /* generate our own files and read them back */
    for ( size_t i = 0; i < nRepeats; i++ )
    {
        printf( "Reading %u in %s\n", repeats[ i ], archiveName );
        read_buffer( archive, repeats[ i ] );
    }

    OTF2_Archive_Close( archive );
}

int
main( int argc, char* argv[] )
{
    otf2_DEBUG = !!getenv( "OTF2_DEBUG_TESTS" );

    OTF2_ErrorCode status;

    const char* source_path = NULL;

    /*
     * We test three different cases:
     *  1) all fits into one chunk
     *  2) all fits into OTF2_BUFFER_TEST_NUMBER_OF_CHUNKS chunks
     *     (ie. no flushing)
     *  3) we need more than OTF2_BUFFER_TEST_NUMBER_OF_CHUNKS chunks
     *     (ie. we do flush)
     *
     *  The repeat counts are the knobs to reach this goal depending on the
     *  chunk size given by OTF2_BUFFER_TEST_CHUNK_SIZE.
     *  ( Ie. if you change the chunks size, the number of chunks before
     *    flushing, or the test_script you may need to adjust the repeat counts.
     *    Also you need to regenerate the little and big endian files. )
     */
    uint32_t repeat_counts[] = { 128, 512, 2048 };
    #define N_REPEAT_COUNTS \
    ( sizeof( repeat_counts ) / sizeof( repeat_counts[ 0 ] ) )

    /*
     * Call the program with no arguments to generate the foreign input files
     * to test also the byte order
     */
    if ( argc == 1 )
    {
        /* go into generate mode */

        const char* endiannes_name;
        #ifdef WORDS_BIGENDIAN
        endiannes_name = "be";
        #else
        endiannes_name = "le";
        #endif

        write_archive( ".",
                       endiannes_name,
                       repeat_counts,
                       N_REPEAT_COUNTS );

        return 0;
    }

    if ( argc != 2 )
    {
        fprintf( stderr, "usage: %s [source path]\n", argv[ 0 ] );

        return 1;
    }

    source_path = argv[ 1 ];

    if ( otf2_DEBUG )
    {
        printf( "=== OTF2_BUFFER_TEST [DEBUG MODE] ===\n" );
    }
    else
    {
        printf( "=== OTF2_BUFFER_TEST ===\n" );
    }

    /* Write files in antive order */
    write_archive( ".",
                   "native",
                   repeat_counts,
                   N_REPEAT_COUNTS );

    /* Read files in native order */
    read_archive( ".",
                  "native",
                  repeat_counts,
                  N_REPEAT_COUNTS );

    /* Read files in big-endian order */
    read_archive( source_path,
                  "be",
                  repeat_counts,
                  N_REPEAT_COUNTS );

    /* Read files in little-endian order */
    read_archive( source_path,
                  "le",
                  repeat_counts,
                  N_REPEAT_COUNTS );

    /* Read files in big-endian order of trace format 1 */
    read_archive( source_path,
                  "be_1",
                  repeat_counts,
                  N_REPEAT_COUNTS );

    /* Read files in little-endian order of trace format 1 */
    read_archive( source_path,
                  "le_1",
                  repeat_counts,
                  N_REPEAT_COUNTS );

    /* This is just to add a message to the debug output. */
    check_status( OTF2_SUCCESS, "Programm finished successfully." );

    return EXIT_SUCCESS;
}



/* ___ Implementation of static functions ___________________________________ */



/** @brief Pre flush callback.
 *
 *  @param evtWriter        Ignored.
 *  @param evtReader        Ignored.
 *
 *  @return                 Returns always OTF2_NO_FLUSH.
 */
static OTF2_FlushType
pre_flush( void*         userData,
           OTF2_FileType fileType,
           uint64_t      locationId,
           void*         callerData,
           bool          final )
{
    return OTF2_FLUSH;
}


static OTF2_TimeStamp
post_flush( void*         userData,
            OTF2_FileType fileType,
            uint64_t      locationId )
{
    return get_time();
}


/** @brief Get generic timestamp.
 *
 *  @return                 Returns consecutive timestamps 0,1,2 ...
 */
static inline uint64_t
get_time( void )
{
    static uint64_t sequence;
    return sequence++;
}


/** @brief Check if pointer is NULL.
 *
 *  Checks if a pointer is NULL. If so it prints an error with the passed
 *  description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param pointer          Pointer to be checked.
 *  @param description      Description for error/debug message.
 *  @param ...              Variable argument list like e.g. in printf.
 */
static inline void
check_pointer( void* pointer,
               char* description,
               ... )
{
    va_list va;
    va_start( va, description );

    if ( pointer == NULL )
    {
        printf( "==ERROR== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
        exit( EXIT_FAILURE );
    }

    if ( otf2_DEBUG )
    {
        printf( "==DEBUG== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
    }

    va_end( va );
}


/** @brief Check if status is not OTF2_SUCCESS.
 *
 *  Checks if status is not OTF2_SUCCESS. If so it prints an error with the
 *  passed description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param pointer          Status to be checked.
 *  @param description      Description for error/debug message.
 *  @param ...              Variable argument list like e.g. in printf.
 */
static inline void
check_status( OTF2_ErrorCode status,
              char*          description,
              ... )
{
    va_list va;
    va_start( va, description );

    if ( status != OTF2_SUCCESS )
    {
        printf( "==ERROR== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
        exit( EXIT_FAILURE );
    }

    if ( otf2_DEBUG )
    {
        printf( "==DEBUG== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
    }

    va_end( va );
}


/** @brief Check if boolean is not true.
 *
 *  Checks if status is not OTF2_SUCCESS. If so it prints an error with the
 *  passed description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param truth_value      Variable to be checked.
 *  @param description      Description for error/debug message.
 *  @param ...              Variable argument list like e.g. in printf.
 */
static inline void
check_boolean( bool  truth_value,
               char* description,
               ... )
{
    va_list va;
    va_start( va, description );

    if ( !truth_value )
    {
        printf( "==ERROR== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
        exit( EXIT_FAILURE );
    }

    if ( otf2_DEBUG )
    {
        printf( "==DEBUG== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
    }

    va_end( va );
}
