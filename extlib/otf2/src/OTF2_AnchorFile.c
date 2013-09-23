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
 *  @file       src/OTF2_AnchorFile.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer enables writing and parsing to and from anchor files.
 *
 */

#include <config.h>

#if HAVE( CLOCK_GETTIME )
#  ifdef _POSIX_C_SOURCE
#    if _POSIX_C_SOURCE < 199309L
#      undef _POSIX_C_SOURCE
#      define _POSIX_C_SOURCE 199309L
#    endif
#  else
#    define _POSIX_C_SOURCE 199309L
#  endif
#  include <time.h>
#elif HAVE( GETTIMEOFDAY )
#  include <sys/time.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#if HAVE( UNISTD_H )
#  include <unistd.h>
#endif
#if HAVE( SYS_TYPES_H )
#  include <sys/types.h>
#endif
#if HAVE( SYS_SYSINFO_H )
#  include <sys/sysinfo.h>
#endif

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>
#include <UTILS_CStr.h>

#include <jenkins_hash.h>

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "otf2_attic_types.h"

#include "otf2_events_int.h"

#include "OTF2_File.h"
#include "OTF2_Buffer.h"

#include "OTF2_AnchorFile.h"

/* anchor file starts with this string after the chunk header. */
#define OTF2_ANCHOR_MAGIC "OTF2"

/*
 * If we change only the anchor file format, than we can check it with this
 * number and don't need to change the trace format.
 * The anchor file format is only additive and future versions will only
 * append new fields. Therefore the reader of version 1 can also read all
 * future versions.
 */
#define OTF2_ANCHOR_FORMAT 3

/* ___ Implementation of external functions. ________________________________ */



/** @brief Load an anchor file.
 *
 *  Parses the content of an OTF2 anchor file and passes the information to the
 *  given archive handle. The call definitively fails if a corrupted or
 *  incomplete anchor file is given.
 *
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_AnchorFile_Load( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );

    OTF2_ErrorCode     ret = OTF2_SUCCESS;
    OTF2_ErrorCode     status;
    const char*        tmp_string;
    uint8_t            trace_format;
    uint8_t            version_major;
    uint8_t            version_minor;
    uint8_t            version_bugfix;
    uint64_t           chunk_size_events;
    uint64_t           chunk_size_defs;
    OTF2_FileSubstrate substrate;
    OTF2_Compression   compression;
    uint64_t           locations;
    uint64_t           global_defs;
    char*              machine              = NULL;
    char*              creator              = NULL;
    char*              description          = NULL;
    char**             property_store       = NULL;
    uint32_t           number_of_properties = 0;
    uint64_t           trace_id             = 0;
    uint32_t           number_of_snapshots  = 0;
    uint32_t           number_of_thumbnails = 0;


    OTF2_Buffer* anchor_buffer = OTF2_Buffer_New( archive,
                                                  NULL,
                                                  OTF2_CHUNK_SIZE_MIN,
                                                  OTF2_BUFFER_READ,
                                                  OTF2_BUFFER_NOT_CHUNKED,
                                                  OTF2_FILETYPE_ANCHOR,
                                                  OTF2_UNDEFINED_LOCATION );
    if ( !anchor_buffer )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                            "Can't create buffer for anchor file." );
    }


    /* check magic string */
    status = OTF2_Buffer_ReadString( anchor_buffer, &tmp_string );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Malformed anchor file: expecting string." );
        goto out;
    }
    if ( 0 != strcmp( tmp_string, OTF2_ANCHOR_MAGIC ) )
    {
        ret = UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                           "Malformed anchor file: magic does not match." );
        goto out;
    }

    /* We need 1 byte for the anchor format */
    status = OTF2_Buffer_Guarantee( anchor_buffer, 1 );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Malformed anchor file: unexpected end of file." );
        goto out;
    }

    /* Read anchor format version */
    uint8_t anchor_format;
    OTF2_Buffer_ReadUint8( anchor_buffer, &anchor_format );
    if ( anchor_format == 0 )
    {
        ret = UTILS_ERROR( status, "Malformed anchor file: anchor format mismatch." );
        goto out;
    }

    /* We need 38 bytes for the these values:
     *  * 1 trace_format
     *  * 3 version numbers: major, minor, bugfix
     *  * 8 event chunk size
     *  * 8 def chunk size
     *  * 1 file substrate
     *  * 1 compression
     *  * 8 number of locations
     *  * 8 number of global definitions
     */
    status = OTF2_Buffer_Guarantee( anchor_buffer, 38 );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Malformed anchor file: unexpected end of file." );
        goto out;
    }

    /* Read trace format version */
    OTF2_Buffer_ReadUint8( anchor_buffer, &trace_format );

    /* Read OTF2 version */
    OTF2_Buffer_ReadUint8( anchor_buffer, &version_major );
    OTF2_Buffer_ReadUint8( anchor_buffer, &version_minor );
    OTF2_Buffer_ReadUint8( anchor_buffer, &version_bugfix );

    /* Read chunk sizes */
    OTF2_Buffer_ReadUint64Full( anchor_buffer, &chunk_size_events );
    OTF2_Buffer_ReadUint64Full( anchor_buffer, &chunk_size_defs );

    /* Read file substrate */
    OTF2_Buffer_ReadUint8( anchor_buffer, &substrate );
    switch ( substrate )
    {
        case OTF2_SUBSTRATE_POSIX:
        case OTF2_SUBSTRATE_SION:
        case OTF2_SUBSTRATE_NONE:
            break;
        default:
            ret = UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                               "Unhandled file substrate." );
            goto out;
    }


    /* Read file compression */
    OTF2_Buffer_ReadUint8( anchor_buffer, &compression );
    switch ( compression )
    {
        case OTF2_COMPRESSION_NONE:
        case OTF2_COMPRESSION_ZLIB:
            break;
        default:
            ret = UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                               "Unhandled file compression." );
            goto out;
    }


    /* Read number of locations and global definitions */
    OTF2_Buffer_ReadUint64Full( anchor_buffer, &locations );
    OTF2_Buffer_ReadUint64Full( anchor_buffer, &global_defs );


    /* Read machine. */
    status = OTF2_Buffer_ReadString( anchor_buffer, &tmp_string );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Malformed anchor file: expecting string." );
        goto out;
    }
    machine = UTILS_CStr_dup( tmp_string );
    if ( !machine )
    {
        ret = UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                           "Can't copy string." );
        goto out;
    }


    /* Read creator. */
    status = OTF2_Buffer_ReadString( anchor_buffer, &tmp_string );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Malformed anchor file: expecting string." );
        goto out;
    }
    creator = UTILS_CStr_dup( tmp_string );
    if ( !creator )
    {
        ret = UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                           "Can't copy string." );
        goto out;
    }


    /* Read description. */
    status = OTF2_Buffer_ReadString( anchor_buffer, &tmp_string );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Malformed anchor file: expecting string." );
        goto out;
    }
    description = UTILS_CStr_dup( tmp_string );
    if ( !description )
    {
        ret = UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                           "Can't copy string." );
        goto out;
    }

    /*
     * Since version 2 the anchor file contains additional
     * meta-information called properties and an unique
     * trace identifier.
     */
    if ( anchor_format > 1 )
    {
        /* Read number of properties */
        status = OTF2_Buffer_Guarantee( anchor_buffer, 4 );
        if ( status != OTF2_SUCCESS )
        {
            ret = UTILS_ERROR( status,
                               "Malformed anchor file: unexpected end of file." );
            goto out;
        }
        OTF2_Buffer_ReadUint32Full( anchor_buffer, &number_of_properties );

        if ( number_of_properties > 0 )
        {
            /* Property store will contain name and values of all properties.
             * For example name of i-th property is stored in property_store[ i ]
             * and corresponding value in property_store[ i + 1 ]. */
            property_store = calloc( 2 * number_of_properties, sizeof( char* ) );
            if ( !property_store )
            {
                ret = UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                                   "Can't create property storage." );
                goto out;
            }

            for ( uint32_t property_index = 0; property_index < number_of_properties; property_index++ )
            {
                /*
                 * Read property name
                 */
                status = OTF2_Buffer_ReadString( anchor_buffer, &tmp_string );
                if ( status != OTF2_SUCCESS )
                {
                    ret = UTILS_ERROR( status, "Malformed anchor file: expecting string." );
                    goto out;
                }

                property_store[ 2 * property_index ] = UTILS_CStr_dup( tmp_string );
                if ( !property_store[ 2 * property_index ] )
                {
                    ret = UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                                       "Can't copy string." );
                    goto out;
                }

                /* Read value of current property */
                status = OTF2_Buffer_ReadString( anchor_buffer, &tmp_string );
                if ( status != OTF2_SUCCESS )
                {
                    ret = UTILS_ERROR( status, "Malformed anchor file: expecting string." );
                    goto out;
                }

                property_store[ 2 * property_index + 1 ] = UTILS_CStr_dup( tmp_string );
                if ( !property_store[ 2 * property_index + 1 ] )
                {
                    ret = UTILS_ERROR( OTF2_ERROR_MEM_ALLOC_FAILED,
                                       "Can't copy string." );
                    goto out;
                }
            }
        }

        /* Read trace identifier */
        status = OTF2_Buffer_Guarantee( anchor_buffer, 8 );
        if ( status != OTF2_SUCCESS )
        {
            ret = UTILS_ERROR( status,
                               "Malformed anchor file: unexpected end of file." );
            goto out;
        }
        OTF2_Buffer_ReadUint64Full( anchor_buffer, &trace_id );
    }


    /*
     * Since version 3 anchor files contain number of snapshots and thumbnails
     */
    if ( anchor_format > 2 )
    {
        /* Read number_of_snapshots */
        status = OTF2_Buffer_Guarantee( anchor_buffer, 4 );
        if ( status != OTF2_SUCCESS )
        {
            ret = UTILS_ERROR( status,
                               "Malformed anchor file: unexpected end of file." );
            goto out;
        }
        OTF2_Buffer_ReadUint32Full( anchor_buffer, &number_of_snapshots );

        /* Read number of thumbnails */
        status = OTF2_Buffer_Guarantee( anchor_buffer, 4 );
        if ( status != OTF2_SUCCESS )
        {
            ret = UTILS_ERROR( status,
                               "Malformed anchor file: unexpected end of file." );
            goto out;
        }
        OTF2_Buffer_ReadUint32Full( anchor_buffer, &number_of_thumbnails );
    }

    /*
     * Check for buffer end, only if we have an anchor file from our known format.
     */
    if ( anchor_format == OTF2_ANCHOR_FORMAT )
    {
        /* Read end of OTF2_BUFFER_END_OF_FILE */
        status = OTF2_Buffer_Guarantee( anchor_buffer, 1 );
        if ( status != OTF2_SUCCESS )
        {
            ret = UTILS_ERROR( status, "Malformed anchor file: unexpected end of file." );
            goto out;
        }
        uint8_t end_of_file;
        OTF2_Buffer_ReadUint8( anchor_buffer, &end_of_file );
        if ( end_of_file != OTF2_BUFFER_END_OF_FILE )
        {
            ret = UTILS_ERROR( status, "Malformed anchor file: expecting end of file." );
            goto out;
        }
    }


    /* close buffer */
    OTF2_Buffer_Delete( anchor_buffer );
    anchor_buffer = NULL;


    /* Pass detected values to the archive handle. */

    /* Set trace format version automatically. */
    status = otf2_archive_set_trace_format( archive,
                                            trace_format );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set trace format version!" );
        goto out;
    }

    /* Set OTF2 version automatically. */
    status = otf2_archive_set_version( archive,
                                       version_major,
                                       version_minor,
                                       version_bugfix );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set OTF2 version!" );
        goto out;
    }

    /* Set size of the chunks. */
    status = otf2_archive_set_chunksize( archive,
                                         chunk_size_events,
                                         chunk_size_defs );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set chunk size!" );
        goto out;
    }

    /* Set file substrate. */
    status = otf2_archive_set_file_substrate( archive, substrate );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set file substrate!" );
        goto out;
    }

    /* Set compression mode. */
    status = otf2_archive_set_compression( archive, compression );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set compression!" );
        goto out;
    }

    /* Set number of locations */
    status = otf2_archive_set_number_of_locations( archive, locations );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set locations number!" );
        goto out;
    }

    /* Set number of global definition records */
    status = otf2_archive_set_number_of_global_defs( archive, global_defs );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set global definitions number!" );
        goto out;
    }

    /* Set name of the machine, where the trace is recorded. */
    status = otf2_archive_set_machine_name( archive, machine );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set machine name!" );
        goto out;
    }

    status = otf2_archive_set_creator( archive, creator );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set creator!" );
        goto out;
    }

    /* Set description of the trace. */
    status = otf2_archive_set_description( archive, description );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set description!" );
        goto out;
    }

    /*
     * We don't need to set number of additional meta-information manually.
     * This number is automatically updated while adding properties to archive.
     */

    /* Set additional meta-information */
    for ( uint32_t property_index = 0; property_index < number_of_properties; property_index++ )
    {
        status = otf2_archive_set_property( archive,
                                            property_store[ 2 * property_index ],
                                            property_store[ 2 * property_index + 1 ],
                                            false );
        if ( status != OTF2_SUCCESS )
        {
            ret = UTILS_ERROR( status, "Could not set property!" );
            goto out;
        }
    }

    /* Set trace identifier */
    status = otf2_archive_set_trace_id( archive, trace_id );
    if ( status != OTF2_SUCCESS )
    {
        ret = UTILS_ERROR( status, "Could not set trace identifier!" );
        goto out;
    }

    /* Set number of snaps */
    status = otf2_archive_set_number_of_snapshots( archive, number_of_snapshots );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not set number of snapshots!" );
    }

    /* Set number of thumbnails */
    status = otf2_archive_set_number_of_thumbnails( archive, number_of_thumbnails );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not set number of thumbnails!" );
    }

out:
    if ( anchor_buffer )
    {
        OTF2_Buffer_Delete( anchor_buffer );
    }
    free( machine );
    free( creator );
    free( description );
    if ( property_store )
    {
        for ( uint32_t i = 0; i < 2 * number_of_properties; i++ )
        {
            free( property_store[ i ] );
        }
    }
    free( property_store );

    return ret;
}


/** @brief Save archive data to an anchor file.
 *
 *  Stores necessary archive data in an anchor file.
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_AnchorFile_Save( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );

    OTF2_ErrorCode     status;
    OTF2_Buffer*       anchor_buffer = NULL;
    uint8_t            version_major;
    uint8_t            version_minor;
    uint8_t            version_bugfix;
    uint8_t            trace_format;
    uint64_t           chunk_size_events;
    uint64_t           chunk_size_defs;
    OTF2_FileSubstrate substrate;
    OTF2_Compression   compression;
    uint64_t           locations;
    uint64_t           global_defs;
    char*              machine        = NULL;
    char*              description    = NULL;
    char*              creator        = NULL;
    char**             property_names = NULL;
    char*              property_value = NULL;
    char               empty_string[] = "";
    uint32_t           number_of_properties;
    /* Position in buffer where characteristics of the trace file
     * (e.g.chunk_size_events) are stored. This pointer will be passed
     * to a hashing algorithm in order to generate trace identifier. */
    uint8_t* hashing_start_pos;
    uint8_t* hashing_end_pos;
    size_t   hashing_length;
    uint64_t trace_id;
    uint32_t number_of_snapshots;
    uint32_t number_of_thumbnails;


/* ___ Get all necessary information from the internal archive handle. _____ */


    /* Trace format version, which is set automatically. */
    status = otf2_archive_get_trace_format( archive, &trace_format );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Unable to get trace format version!" );
    }

    /* OTF2 version, which is set automatically. */
    status = otf2_archive_get_version( archive,
                                       &version_major,
                                       &version_minor,
                                       &version_bugfix );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Unable to get trace format version!" );
    }

    /* Size of the chunks which are used in an event trace */
    status = otf2_archive_get_chunksize( archive,
                                         &chunk_size_events,
                                         &chunk_size_defs );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Unable to get chunk size!" );
    }

    /* Determines how the trace should be written */
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Unable to get file substrate!" );
    }

    /* Determines if trace data is compressed. */
    status = otf2_archive_get_compression( archive, &compression );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Unable to get file substrate!" );
    }

    /* Location ID */
    status = otf2_archive_get_number_of_locations( archive, &locations );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Unable to get number of locations!" );
    }

    /* Number of global definition records */
    status = otf2_archive_get_number_of_global_defs( archive,
                                                     &global_defs );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Unable to get number of global definitions!" );
    }

    /* Name of the machine, where the trace is recorded (optional) */
    status = otf2_archive_get_machine_name( archive, &machine );
    if ( !machine )
    {
        machine = empty_string;
    }

    /* Name of the creator (optional) */
    status = otf2_archive_get_creator( archive, &creator );
    if ( !creator )
    {
        creator = empty_string;
    }

    /* Description of the trace (optional) */
    status = otf2_archive_get_description( archive, &description );
    if ( !description )
    {
        description = empty_string;
    }

    /* Number of additional meta-information (optional) */
    status = otf2_archive_get_property_names( archive, &number_of_properties, &property_names );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Unable to get property names!" );
        goto out;
    }

    status = otf2_archive_get_trace_id( archive, &trace_id );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Unable to get trace id!" );
        goto out;
    }

    status = otf2_archive_get_number_of_snapshots( archive, &number_of_snapshots );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Unable to get availability of snapshots!" );
        goto out;
    }

    status = otf2_archive_get_number_of_thumbnails( archive, &number_of_thumbnails );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Unable to get number of thumbnails!" );
        goto out;
    }

/* ___ Write all information to the anchor file. ____________________________ */

    anchor_buffer = OTF2_Buffer_New( archive,
                                     NULL,
                                     OTF2_CHUNK_SIZE_MIN,
                                     OTF2_BUFFER_WRITE,
                                     OTF2_BUFFER_NOT_CHUNKED,
                                     OTF2_FILETYPE_ANCHOR,
                                     OTF2_UNDEFINED_LOCATION );
    if ( !anchor_buffer )
    {
        goto out;
    }

    /* Write OTF2 magic and format
     * No need to guarantee anything, OTF2_CHUNK_SIZE_MIN is big enough.
     * This holds until we write the first string, ie. up to global_defs.
     */
    OTF2_Buffer_WriteString( anchor_buffer, OTF2_ANCHOR_MAGIC );
    OTF2_Buffer_WriteUint8( anchor_buffer, OTF2_ANCHOR_FORMAT );

    /* Write trace format version */
    OTF2_Buffer_WriteUint8( anchor_buffer, trace_format );

    /* Write OTF2 version */
    OTF2_Buffer_WriteUint8( anchor_buffer, version_major );
    OTF2_Buffer_WriteUint8( anchor_buffer, version_minor );
    OTF2_Buffer_WriteUint8( anchor_buffer, version_bugfix );

    /* Get position in buffer (used for hashing) */
    OTF2_Buffer_GetPosition( anchor_buffer, &hashing_start_pos  );

    /* Write chunk sizes */
    OTF2_Buffer_WriteUint64Full( anchor_buffer, chunk_size_events );
    OTF2_Buffer_WriteUint64Full( anchor_buffer, chunk_size_defs );

    /* Write file substrate */
    OTF2_Buffer_WriteUint8( anchor_buffer, substrate );

    /* Write file compression */
    OTF2_Buffer_WriteUint8( anchor_buffer, compression );

    /* Write number of locations and global definitions */
    OTF2_Buffer_WriteUint64Full( anchor_buffer, locations );
    OTF2_Buffer_WriteUint64Full( anchor_buffer, global_defs );

    /* From here on, we need to check every write with an guaranteed length */

    /* Write machine */
    status = OTF2_Buffer_Guarantee( anchor_buffer, strlen( machine ) + 1 );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Machine name too long." );
        goto out;
    }
    OTF2_Buffer_WriteString( anchor_buffer, machine );


    /* Write creator */
    status = OTF2_Buffer_Guarantee( anchor_buffer, strlen( creator ) + 1 );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Creator name too long." );
        goto out;
    }
    OTF2_Buffer_WriteString( anchor_buffer, creator );


    /* Write description */
    status = OTF2_Buffer_Guarantee( anchor_buffer, strlen( description ) + 1 );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Description name too long." );
        goto out;
    }
    OTF2_Buffer_WriteString( anchor_buffer, description );

    /* Write number of additional meta-information */
    status = OTF2_Buffer_Guarantee( anchor_buffer, 4 );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Insufficient buffer space for anchor file." );
        goto out;
    }
    OTF2_Buffer_WriteUint32Full( anchor_buffer, number_of_properties );
    /* Write all meta-information */
    for ( uint32_t property_index = 0; property_index < number_of_properties; property_index++ )
    {
        status = otf2_archive_get_property( archive, property_names[ property_index ], &property_value );
        if ( status != OTF2_SUCCESS || property_value == NULL )
        {
            UTILS_ERROR( status, "Unable to get property value." );
            free( property_value );
            goto out;
        }

        status = OTF2_Buffer_Guarantee( anchor_buffer, strlen( property_names[ property_index ] ) + 1 );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status, "Property name too long." );
            free( property_value );
            goto out;
        }
        OTF2_Buffer_WriteString( anchor_buffer, property_names[ property_index ] );

        status = OTF2_Buffer_Guarantee( anchor_buffer, strlen( property_value ) + 1 );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status, "Property value too long." );
            free( property_value );
            goto out;
        }
        OTF2_Buffer_WriteString( anchor_buffer, property_value );

        free( property_value );
    }

    /* Generate trace identifier, if not done so already */
    if ( trace_id == 0 )
    {
        uint32_t hash_lower;
        uint32_t hash_upper;

        /* Get position in buffer (used for hashing) */
        OTF2_Buffer_GetPosition( anchor_buffer, &hashing_end_pos  );
        hashing_length = ( size_t )( hashing_end_pos - hashing_start_pos );

        /* Seeding the hash. */
        hash_lower = 1;
#if HAVE( GETPID )
        hash_upper = getpid();
#else
        hash_upper = 23;
#endif

        jenkins_hashlittle2( archive->archive_path,
                             strlen( archive->archive_path ),
                             &hash_lower,
                             &hash_upper );
        jenkins_hashlittle2( archive->archive_name,
                             strlen( archive->archive_name ),
                             &hash_lower,
                             &hash_upper );
        jenkins_hashlittle2( hashing_start_pos,
                             hashing_length,
                             &hash_lower,
                             &hash_upper );

#if HAVE( SYSINFO )
        struct sysinfo info;
        if ( 0 == sysinfo( &info ) )
        {
            /* add the uptime, or the complete struct? */
            jenkins_hashlittle2( &info.uptime,
                                 sizeof( info.uptime ),
                                 &hash_lower,
                                 &hash_upper );
        }
#endif

#if HAVE( CLOCK_GETTIME )
        struct timespec tp;
        clock_gettime( CLOCK_REALTIME, &tp );
        jenkins_hashlittle2( &tp,
                             sizeof( tp ),
                             &hash_lower,
                             &hash_upper );
#elif HAVE( GETTIMEOFDAY )
        struct timeval tv;
        gettimeofday( &tv, NULL );
        jenkins_hashlittle2( &tv,
                             sizeof( tv ),
                             &hash_lower,
                             &hash_upper );
#endif

        /* make a 64bit hash value out of the two 32bit ones. */
        trace_id = ( ( uint64_t )hash_upper << 32 ) + hash_lower;
    }

    /* Write trace identifier */
    status = OTF2_Buffer_Guarantee( anchor_buffer, 8 );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Insufficient buffer space for anchor file." );
        goto out;
    }
    OTF2_Buffer_WriteUint64Full( anchor_buffer, trace_id );

    /* Write number_of_snapshots */
    status = OTF2_Buffer_Guarantee( anchor_buffer, 4 );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Insufficient buffer space for anchor file." );
        goto out;
    }
    OTF2_Buffer_WriteUint32Full( anchor_buffer, number_of_snapshots );

    /* Write number of thumbnails */
    status = OTF2_Buffer_Guarantee( anchor_buffer, 4 );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Insufficient buffer space for anchor file." );
        goto out;
    }
    OTF2_Buffer_WriteUint32Full( anchor_buffer, number_of_thumbnails );

    /* Write OTF2_BUFFER_END_OF_FILE */
    status = OTF2_Buffer_Guarantee( anchor_buffer, 1 );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Can't write END_OF_FILE." );
        goto out;
    }
    OTF2_Buffer_WriteUint8( anchor_buffer, OTF2_BUFFER_END_OF_FILE );

    status = OTF2_Buffer_Delete( anchor_buffer );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Can't close buffer." );
    }
    anchor_buffer = NULL;

out:
    if ( anchor_buffer )
    {
        OTF2_Buffer_Delete( anchor_buffer );
    }

    /* Release allocated memory for strings. */
    if ( machine != empty_string )
    {
        free( machine );
    }
    if ( description != empty_string )
    {
        free( description );
    }
    if ( creator != empty_string )
    {
        free( creator );
    }
    free( property_names );

    return status;
}
