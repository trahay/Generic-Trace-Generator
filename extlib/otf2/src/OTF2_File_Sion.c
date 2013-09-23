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
 *  @file       src/OTF2_File_Sion.c
 *  @status     alpha
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer is completely internal to the OTF2 library and should
 *              not be exposed to the end user. Its purpose is to abstract the
 *              file interaction via the specific file substrate.
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

#if HAVE( SION_SERIAL )
#include <sion.h>
#else
#define SION_CURRENT_RANK 0
#define SION_ABSOLUTE_POS 0
#define SION_CURRENT_POS  0
#define sion_int64 int64_t
static inline int
sion_open_rank( const char* file_path, const char* mode, long long int* chunksize, int* fs_blocksize, int* rank_id, FILE** file_ptr )
{
    return -1;
}
static inline int
sion_close( int sid )
{
    return 0;
}
static inline int
sion_fwrite( const void* buffer, size_t size, size_t nmembs, int sid )
{
    return 0;
}
static inline int
sion_fread( void* buffer, size_t size, size_t nmembs, int sid )
{
    return 0;
}
static inline int
sion_seek( int sid, int rank, int where, int64_t offset )
{
    return 0;
}
static inline int
sion_get_current_location( int sid, int* currentblock, sion_int64* currentpos, int* maxchunks, sion_int64** chunksizes )
{
    return 0;
}
#endif

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "OTF2_File.h"
#include "otf2_file_substrates.h"


/** @brief File handle to interact with sion files.
 *  Please see otf2_file_sion_struct for a detailed description. */
typedef struct otf2_file_sion_struct otf2_file_sion;

/** @brief File handle to interact with Sion files. */
struct otf2_file_sion_struct
{
    OTF2_File super;

    /* ___ Information owned by the SION file substrate. ___ */

    /** Current Sion file handle. */
    int sid;
    /** Keep file pos when the real file is closed. Is undefined while file is
     *  open, equals 0 before opened for the first time. */
    uint64_t position;

    /** File pointer to the file SION uses */
    FILE* sfileptr;
};


static OTF2_ErrorCode
otf2_file_sion_close( OTF2_File* file );

static OTF2_ErrorCode
otf2_file_sion_reset( OTF2_File* file );

static OTF2_ErrorCode
otf2_file_sion_write( OTF2_File*  file,
                      const void* buffer,
                      uint64_t    size );

static OTF2_ErrorCode
otf2_file_sion_read( OTF2_File* file,
                     void*      buffer,
                     uint64_t   size );

static OTF2_ErrorCode
otf2_file_sion_seek( OTF2_File*    file,
                     int64_t       offset,
                     OTF2_FileSeek origin );

static OTF2_ErrorCode
otf2_file_sion_get_file_size( OTF2_File* file,
                              uint64_t*  size );


/** @brief Register a handle for file substrate.
 *
 *  Allocates a handle, sets the according values and registers the function
 *  pointers to its file operations.
 *
 *  @param fileName         Name of the file.
 *  @param mode             Method to interact with the file.
 *  @param substrate        File substrate.
 *
 *  @return                 Returns a handle to the file if successful, NULL
 *                          otherwise.
 */
OTF2_File*
otf2_file_sion_open( OTF2_Archive*    archive,
                     OTF2_FileMode    fileMode,
                     OTF2_FileType    fileType,
                     OTF2_LocationRef location )
{
    UTILS_ASSERT( archive );

    /*
     * we handle only file types which are per-location
     * lets the POSIX substrate handle the rest
     */
    if ( !otf2_file_type_needs_location_id( fileType ) )
    {
        return otf2_file_posix_open( archive, fileMode, fileType, location );
    }

    /* Determine the file mode.*/
    const char* mode_str = "rb";
    switch ( fileMode )
    {
        case OTF2_FILEMODE_WRITE:
            mode_str = "wb";
            break;
        case OTF2_FILEMODE_READ:
            mode_str = "rb";
            break;
        case OTF2_FILEMODE_MODIFY:
            UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                         "SION does not support the modify modus" );
            return NULL;
            break;
        default:
            UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                         "Unknown file mode." );
            return NULL;
            break;
    }

    otf2_file_sion* file =
        ( otf2_file_sion* )calloc( 1, sizeof( otf2_file_sion ) );

    if ( file == NULL )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for file handle!" );
        return NULL;
    }

    char* file_path = otf2_archive_get_file_path_with_name_prefix( archive,
                                                                   fileType,
                                                                   "sion" );
    if ( !file_path )
    {
        free( file );
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Can't determine file path!" );
        return NULL;
    }

    /* Some variables which are needed for the open command */
    long long int chunksize    = OTF2_CHUNK_SIZE_MAX;
    int           fs_blocksize = -1;
    int           sid          = -1;
    FILE*         file_ptr     = NULL;

    /* Open the SION file handle */
    if ( archive->file_sion_callbacks->otf2_file_sion_open )
    {
        sid = archive->file_sion_callbacks
              ->otf2_file_sion_open( archive->file_sion_data,
                                     fileType,
                                     location,
                                     file_path,
                                     mode_str,
                                     &chunksize,
                                     &fs_blocksize,
                                     &file_ptr );
    }
    else
    {
        int32_t        rank_id;
        OTF2_ErrorCode status = archive->file_sion_callbacks
                                ->otf2_file_sion_get_rank( archive->file_sion_data,
                                                           fileType,
                                                           location,
                                                           &rank_id );
        if ( OTF2_SUCCESS != status )
        {
            UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION, "No rank for location!" );
            free( file );
            free( file_path );
            return NULL;
        }

        sid = sion_open_rank( file_path,
                              mode_str,
                              &chunksize,
                              &fs_blocksize,
                              &rank_id,
                              &file_ptr );
    }

    if ( sid == -1 )
    {
        UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                     "Open call failed! (File name: %s)", file_path );
        free( file );
        free( file_path );
        return NULL;
    }
    free( file_path );

    /* Register function pointers, for real SION files */
    file->super.close         = otf2_file_sion_close;
    file->super.reset         = otf2_file_sion_reset;
    file->super.write         = otf2_file_sion_write;
    file->super.read          = otf2_file_sion_read;
    file->super.seek          = otf2_file_sion_seek;
    file->super.get_file_size = otf2_file_sion_get_file_size;

    file->sid      = sid;
    file->position = 0;
    file->sfileptr = file_ptr;

    return &file->super;
}


OTF2_ErrorCode
otf2_file_sion_create_directory( const char* mainPath )
{
    return otf2_file_posix_create_directory( mainPath );
}


/** @name File operations for Sion file substrate.
 *  Please see the standard interface functions for a detailed description.
 *  @{ */
OTF2_ErrorCode
otf2_file_sion_close( OTF2_File* file )
{
    otf2_file_sion* sion_file = ( otf2_file_sion* )file;

    int ret = 0;

    if ( file->archive->file_sion_callbacks->otf2_file_sion_close )
    {
        ret = file->archive->file_sion_callbacks
              ->otf2_file_sion_close( file->archive->file_sion_data,
                                      file->file_type,
                                      file->location_id,
                                      sion_file->sid );
    }
    else
    {
        ret = sion_close( sion_file->sid );
    }

    if ( ret == 1 )
    {
        free( sion_file );
        return OTF2_SUCCESS;
    }

    return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                        "Sion call 'sion_parclose_mpi()' failed!" );
}


OTF2_ErrorCode
otf2_file_sion_reset( OTF2_File* file )
{
    return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                        "Not supported!" );
}


OTF2_ErrorCode
otf2_file_sion_write( OTF2_File*  file,
                      const void* buffer,
                      uint64_t    size )
{
    /* Get the original substrate structure. */
    otf2_file_sion* sion_file = ( otf2_file_sion* )file;

    int status =
        sion_fwrite( buffer, ( size_t )size, 1, sion_file->sid );

    if ( status == 1 )
    {
        sion_file->position += size;
        return OTF2_SUCCESS;
    }

    return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                        "Sion call 'sion_fwrite()' failed!" );
}


OTF2_ErrorCode
otf2_file_sion_read( OTF2_File* file,
                     void*      buffer,
                     uint64_t   size )
{
    /* Get the original substrate structure. */
    otf2_file_sion* sion_file = ( otf2_file_sion* )file;

    int status =
        sion_fread(  buffer, ( size_t )1, size, sion_file->sid );

    /* There has to be a better check, to verify a correct read. This is just so
     * it will work temporarily. */
    if ( status >= 1 )
    {
        sion_file->position += size;
        return OTF2_SUCCESS;
    }

    return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                        "Sion call to 'sion_fread()' failed!" );
}


OTF2_ErrorCode
otf2_file_sion_seek( OTF2_File*    file,
                     int64_t       offset,
                     OTF2_FileSeek origin )
{
    /* Get the original substrate structure. */
    otf2_file_sion* sion_file = ( otf2_file_sion* )file;

    int status = 1;
    switch ( origin )
    {
        case OTF2_SEEK_SET:
            status =
                sion_seek(
                    sion_file->sid, SION_CURRENT_RANK, SION_ABSOLUTE_POS, offset );
            break;
        case OTF2_SEEK_CUR:
            status =
                sion_seek(
                    sion_file->sid, SION_CURRENT_RANK, SION_CURRENT_POS, offset );
            break;
        default:
            status = -1;
    }

    if ( status != 1 )
    {
        return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                            "Sion call 'sion_seek()' failed!" );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_file_sion_get_file_size( OTF2_File* file,
                              uint64_t*  size )
{
    /* Get the original substrate structure. */
    otf2_file_sion* sion_file = ( otf2_file_sion* )file;

    int         currentblock;
    sion_int64  currentpos;
    int         maxchunks;
    sion_int64* chunksizes;

    int status =
        sion_get_current_location(
            sion_file->sid, &currentblock, &currentpos, &maxchunks, &chunksizes );

    if ( status != 1 )
    {
        return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                            "Sion call 'sion_getFileSize()' failed!" );
    }

    *size = 0;
    for ( uint32_t i = 0; i < maxchunks; i++ )
    {
        *size = *size + chunksizes[ i ];
    }

    // free() chunksizes? because it was allocated by sion_get_current_location()!

    return OTF2_SUCCESS;
}
/** @} */
