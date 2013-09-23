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
 *  @file       src/OTF2_File_Posix.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
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
#include <sys/stat.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "OTF2_File.h"
#include "otf2_file_substrates.h"


/** @brief File handle to interact with posix files.
 *  Please see otf2_file_posix_struct for a detailed description. */
typedef struct otf2_file_posix_struct otf2_file_posix;

/** @brief File handle to interact with posix files. */
struct otf2_file_posix_struct
{
    OTF2_File super;

    /* ___ Information owned by the POSIX file substrate. ___ */

    /** Own copy of the filename. */
    const char* file_path;

    /** Actual OS file handle. It is NULL if file is currently closed. */
    FILE* file;
    /** Keep file pos when the real file is closed. Is undefined while file is
     *  open, equals 0 before opened for the first time. */
    uint64_t position;
};


static OTF2_ErrorCode
otf2_file_posix_close( OTF2_File* file );


static OTF2_ErrorCode
otf2_file_posix_reset( OTF2_File* file );


static OTF2_ErrorCode
otf2_file_posix_write( OTF2_File*  file,
                       const void* buffer,
                       uint64_t    size );


static OTF2_ErrorCode
otf2_file_posix_read( OTF2_File* file,
                      void*      buffer,
                      uint64_t   size );


static OTF2_ErrorCode
otf2_file_posix_seek( OTF2_File*    file,
                      int64_t       offset,
                      OTF2_FileSeek origin );

static OTF2_ErrorCode
otf2_file_posix_get_file_size( OTF2_File* file,
                               uint64_t*  size );


/** @brief Register a handle for file substrate.
 *
 *  Allocates a handle, sets the according values and registers the function
 *  pointers to its file operations.
 *
 *  @return                 Returns a handle to the file if successful, NULL
 *                          otherwise.
 */
OTF2_File*
otf2_file_posix_open( OTF2_Archive*    archive,
                      OTF2_FileMode    fileMode,
                      OTF2_FileType    fileType,
                      OTF2_LocationRef location )
{
    char* file_path = otf2_archive_get_file_path( archive,
                                                  fileType,
                                                  location );
    if ( !file_path )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Can't determine file path!" );
        return NULL;
    }

    /* Initialize substrate specific data. */
    FILE* os_file = NULL;
    switch ( fileMode )
    {
        case OTF2_FILEMODE_WRITE:
            os_file = fopen( file_path, "wb" );
            break;
        case OTF2_FILEMODE_READ:
            os_file = fopen( file_path, "rb" );
            break;
        case OTF2_FILEMODE_MODIFY:
            os_file = fopen( file_path, "rb+" );
            break;
        default:
            os_file = fopen( file_path, "rb" );
            break;
    }

    if ( os_file == NULL )
    {
        UTILS_ERROR_POSIX( "%s", file_path );
        free( file_path );
        return NULL;
    }

    otf2_file_posix* file = ( otf2_file_posix* )calloc( 1, sizeof( otf2_file_posix ) );

    if ( file == NULL )
    {
        UTILS_ERROR_POSIX();
        fclose( os_file );
        free( file_path );
        return NULL;
    }

    /* Register function pointers. */
    file->super.close         = otf2_file_posix_close;
    file->super.reset         = otf2_file_posix_reset;
    file->super.write         = otf2_file_posix_write;
    file->super.read          = otf2_file_posix_read;
    file->super.seek          = otf2_file_posix_seek;
    file->super.get_file_size = otf2_file_posix_get_file_size;

    file->file_path = file_path;
    file->file      = os_file;
    file->position  = 0;

    return &file->super;
}


/** @name File operations for Posix file substrate.
 *  Please see the standard interface functions for a detailed description.
 *  @{ */
OTF2_ErrorCode
otf2_file_posix_close( OTF2_File* file )
{
    /* Get the original substrate structure. */
    otf2_file_posix* posix_file = ( otf2_file_posix* )file;

    int status = fclose( posix_file->file );

    free( ( void* )posix_file->file_path );
    free( posix_file );

    if ( status == 0 )
    {
        return OTF2_SUCCESS;
    }

    /* Print out additional error information from posix error handle. */
    perror( "In function 'otf2_file_posix_close'" );

    return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                        "Posix call 'fclose()' failed!" );
}


OTF2_ErrorCode
otf2_file_posix_reset( OTF2_File* file )
{
    /* Get the original substrate structure. */
    otf2_file_posix* posix_file = ( otf2_file_posix* )file;

    /* Reset is done by closing the file and reopen it in writing mode. */
    int status = fclose( posix_file->file );
    if ( status != 0 )
    {
        return UTILS_ERROR_POSIX( "%s", posix_file->file_path );
    }

    posix_file->file = fopen( posix_file->file_path, "wb" );
    if ( posix_file->file == NULL )
    {
        return UTILS_ERROR_POSIX( "%s", posix_file->file_path );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_file_posix_write( OTF2_File*  file,
                       const void* buffer,
                       uint64_t    size )
{
    /* Get the original substrate structure. */
    otf2_file_posix* posix_file = ( otf2_file_posix* )file;

    size_t status = fwrite( buffer, size, 1, posix_file->file );

    if ( status != 1 )
    {
        return UTILS_ERROR_POSIX( "%s", posix_file->file_path );
    }

    posix_file->position += size;
    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_file_posix_read( OTF2_File* file,
                      void*      buffer,
                      uint64_t   size )
{
    /* Get the original substrate structure. */
    otf2_file_posix* posix_file = ( otf2_file_posix* )file;

    size_t bytes = fread( buffer, 1, size, posix_file->file );

    if ( ferror( posix_file->file ) )
    {
        return UTILS_ERROR_POSIX( "%s", posix_file->file_path );
    }

    posix_file->position += bytes;
    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_file_posix_seek( OTF2_File*    file,
                      int64_t       offset,
                      OTF2_FileSeek origin )
{
    int posix_origin = SEEK_SET;

    /* Get the original substrate structure. */
    otf2_file_posix* posix_file = ( otf2_file_posix* )file;

    switch ( origin )
    {
        case OTF2_SEEK_SET:
            posix_origin = SEEK_SET;
            break;
        case OTF2_SEEK_CUR:
            posix_origin = SEEK_CUR;
            break;
        case OTF2_SEEK_END:
            posix_origin = SEEK_END;
            break;
        default:
            posix_origin = SEEK_SET;
    }

    int status = fseek( posix_file->file, offset, posix_origin );

    if ( status != 0 )
    {
        return UTILS_ERROR_POSIX( "%s", posix_file->file_path );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_file_posix_get_file_size( OTF2_File* file,
                               uint64_t*  size )
{
    /* Get the original substrate structure. */
    otf2_file_posix* posix_file = ( otf2_file_posix* )file;

    struct stat file_stat;
    int         status = stat( posix_file->file_path, &file_stat );

    *size = ( uint64_t )file_stat.st_size;

    if ( status != 0 )
    {
        return UTILS_ERROR_POSIX( "%s", posix_file->file_path );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_file_posix_create_directory( const char* mainPath )
{
    int status = mkdir( mainPath, 0777 );

    /* ignore already existing directories */
    if ( status != 0 )
    {
        if ( errno == EEXIST  )
        {
            return OTF2_ERROR_EEXIST;
        }
        return UTILS_ERROR_POSIX( "%s", mainPath );
    }

    return OTF2_SUCCESS;
}


/** @} */
