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
 *  @file       src/OTF2_File.c
 *  @status     beta
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer is completely internal to the OTF2 library and should
 *              not be exposed to the end user. Its purpose is to abstract the
 *              file interaction via the specific file substrate.
 *
 *
 *  @section add_new_file_substrate How to add a new file substrate
 *
 *  (1) You have to create a source and a header file containing functions to
 *  open a file, close a file, write to a file, read from a file, seek in file,
 *  stat file, and get position in file. The function to open a file must also
 *  set all the function pointers in the structure correctly.
 *  Also the source file needs to contain an own structure keeping all
 *  neccessary information. Please note: This structure has to keep all elements
 *  of OTF2_File_struct in the exact same order. You can add the file substrates
 *  specific elements after that.
 *  The files and functions have to be named/prefixed OTF2_File_<name>.
 *  You can find an example how to do this in OTF2_File_Raw.c.
 *
 *  (2) You have to include the header file in OTF2_File.c.
 *
 *  (3) You have to modify the switch statement in OTF2_File_Open.
 *
 *  (4) You have to modify the switch statement in OTF2_File_CreateDirectory.
 *
 *  (5) You have to add an enum value to OTF2_FileSubstrate_enum in
 *      templates/OTF2_GeneralDefinitions.tmpl.h.
 *
 *  (6) Please use a 'grep -r OTF2_SUBSTRATE_POSIX .' to see, where
 *      you also have to add the new enum entry in parameter checks.
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
#include <UTILS_IO.h>

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "OTF2_File.h"
#include "otf2_file_substrates.h"

#include "OTF2_Compression_Zlib.h"


/** @brief Internal compression header.
 *  The zlib flush point is fixed to 4 bytes: 0x00 0x00 0xff 0xff. In the two
 *  bytes in front of it is a 3 bit header not equal to zero and 0-7 bits equal
 *  to zero to enable byte alignment.
 *  So by putting 2 zero bytes in front of the header we assure that this header
 *  does not clash with zlib's flush point. By adding 2 one bytes at the end we
 *  ensure that the header does not clash with the header content (This pattern
 *  would only emerge if a compressed block would be of size 4GB which is
 *  impossible). This way the header is distinctive to any other data. */
#define OTF2_COMPRESSION_HEADER UINT64_C( 0x00000000ffffffff )

/** @brief Defines size of an internal compression header. */
#define OTF2_COMPRESSION_HEADER_SIZE ( 20 )

/** @brief Defines size of buffered writes to file system. */
#define OTF2_FILE_BUFFER_SIZE ( 4 * 1024 * 1024 )



/* ___ Prototypes of static functions. ______________________________________ */

static OTF2_ErrorCode
otf2_file_write_compression_header( OTF2_File* file,
                                    uint32_t   compSize,
                                    uint32_t   uncompSize );

static OTF2_ErrorCode
otf2_file_read_compression_header( OTF2_File* file,
                                   uint32_t*  compSize,
                                   uint32_t*  uncompSize );

static inline uint32_t*
otf2_swap32( uint32_t* value );

static OTF2_ErrorCode
otf2_file_write_buffered( OTF2_File*  file,
                          const void* buffer,
                          uint64_t    size
                          );

/* ___ File interaction routines. ___________________________________________ */



/** @brief Open a file.
 *
 *  Open a new file with the given 'fileName'. The passed 'mode' defines whether
 *  the file is opened for write-only access 'OTF2_FILEMODE_WRITE', for
 *  read-only access 'OTF2_FILEMODE_READ', or for read-write access
 *  'OTF2_FILEMODE_MODIFY'. The passed 'substrate' defines the used file
 *  substrate to open and interact with the file.
 *
 *  Calls the specific file open routine defined by the file substrate.
 *
 *  @param fileName         Name of the file.
 *  @param mode             Method to interact with the file. Available values
 *                          are: Available values are: 'OTF2_FILEMODE_WRITE',
 *                          'OTF2_FILEMODE_READ', and 'OTF2_FILEMODE_MODIFY'.
 *  @param substrate        File substrate. Available values at the moment are:
 *                          'OTF2_SUBSTRATE_POSIX' and 'OTF2_SUBSTRATE_NONE'.
 *
 *  @return                 Returns a handle to the file if successful, NULL
 *                          otherwise.
 */
OTF2_File*
OTF2_File_Open( OTF2_Archive*    archive,
                OTF2_FileMode    fileMode,
                OTF2_FileType    fileType,
                OTF2_LocationRef location )
{
    UTILS_ASSERT( archive );

    /* Validate arguments.
     * If the none substrate is selected, we need a location id for the local
     * defs and event file types. */
    if ( archive->substrate != OTF2_SUBSTRATE_NONE
         && otf2_file_type_needs_location_id( fileType )
         && location == OTF2_UNDEFINED_LOCATION )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid file name!" );
        return NULL;
    }

    /* Special case the ANCHOR file when reading,  */
    OTF2_FileSubstrate substrate = archive->substrate;
    if ( fileType == OTF2_FILETYPE_ANCHOR
         && archive->substrate == OTF2_SUBSTRATE_UNDEFINED )
    {
        substrate = OTF2_SUBSTRATE_POSIX;
    }

    /* If there is a new file substrate the open function has to be added in the
       switch statement. */
    OTF2_File* file = NULL;
    switch ( substrate )
    {
        /* Substrate Posix: Use standard posix file interface. */
        case OTF2_SUBSTRATE_POSIX:
            file = otf2_file_posix_open( archive, fileMode, fileType, location );
            break;

        /* Substrate Sion: Use the interface of the sionlib to write many
           logical files into few physical files. */
        case OTF2_SUBSTRATE_SION:
            #if HAVE( SION_SERIAL )
            file = otf2_file_sion_open( archive, fileMode, fileType, location );
            #endif
            break;

        /* None substrate: Do not use any file interface and just return a NULL
           pointer. */
        case OTF2_SUBSTRATE_NONE:
            file = otf2_file_none_open( archive, fileMode, fileType, location );
            break;

        /* If substrate does not match any of the above, return NULL. */
        default:
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "This is no valid file substrate!" );
            return NULL;
    }

    if ( file )
    {
        /* Initialize substrate data. */
        file->archive     = archive;
        file->compression = archive->compression;
        /* Anchor files are never compressed. */
        if ( otf2_file_type_ignores_compression( fileType ) )
        {
            file->compression = OTF2_COMPRESSION_NONE;
        }
        file->file_type   = fileType;
        file->location_id = location;

        file->buffer      = NULL;
        file->buffer_used = 0;

        file->last_block_size = 0;
    }

    return file;
}


/** @brief Close a file.
 *
 *  Calls the specific file close routine defined by the file substrate.
 *
 *  @param file             OTF2 file handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_File_Close( OTF2_File* file )
{
    /* Validate arguments. */
    if ( NULL == file )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid file handle!" );
    }

    /* Write remaining data in buffer to file. */
    if ( file->buffer_used > 0 )
    {
        OTF2_ErrorCode status = file->write( file, file->buffer, file->buffer_used );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status,
                         "Write of buffered data failed!" );
        }

        free( file->buffer );
    }

    return file->close( file );
}


/** @brief Reset a file.
 *
 *  Resets an opened file, i.e. the file pointer is set to the beginning of the
 *  file and the file size is set to zero.
 *
 *  Calls the specific file reset routine defined by the file substrate.
 *
 *  @param file             OTF2 file handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_File_Reset( OTF2_File* file )
{
    /* Validate arguments. */
    if ( NULL == file )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid file handle!" );
    }

    /* Reset write buffer. */
    free( file->buffer );
    file->buffer_used = 0;

    return file->reset( file );
}


/** @brief Create a directory.
 *
 *  Please Note: The given file substrate must match the file substrate used for
 *  opening and interacting with files. Otherwise the behaviour is undefined.
 *
 *  Developer Info: This call may be called before an OTF2 file is opened.
 *  Therefore, this function call can not be handled with a suitable function
 *  pointer stored in the file structure, because there may not be one.
 *
 *  @param substrate        File substrate. Available values at the moment are:
 *                          'OTF2_SUBSTRATE_POSIX' and 'OTF2_SUBSTRATE_NONE'.
 *  @param mainPath         Path to the head of the directory.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_File_CreateDirectory( OTF2_Archive* archive,
                           const char*   mainPath,
                           bool          failIfLastExists )
{
    UTILS_ASSERT( archive );

    OTF2_ErrorCode status = OTF2_SUCCESS;

    /* Validate arguments. */
    if ( NULL == mainPath )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid file path!" );
    }

    char* path_copy = UTILS_CStr_dup( mainPath );
    if ( !path_copy )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Can't duplicate path" );
    }
    UTILS_IO_SimplifyPath( path_copy );

    /* The +1 skips absolute paths, starting with '/' */
    char* slash = strchr( path_copy + 1, '/' );
    if ( !slash )
    {
        slash = path_copy + strlen( path_copy );
    }
    while ( true )
    {
        bool is_last = *slash == '\0';
        *slash = '\0';

        /* If there is a new file substrate the create directory function has to be
           added in the switch statement below. */
        switch ( archive->substrate )
        {
            /* Substrate Posix: Use standard posix file interface. */
            case OTF2_SUBSTRATE_POSIX:
                status = otf2_file_posix_create_directory( path_copy );
                break;

            /* Substrate Sion: Use the interface of the sionlib to write many
               logical files into few physical files. */
            case OTF2_SUBSTRATE_SION:
            #if HAVE( SION_SERIAL )
                status = otf2_file_sion_create_directory( path_copy );
            #else
                status = OTF2_ERROR_FILE_INTERACTION;
            #endif
                break;

            /* None substrate: Do not use any file interface and just return a NULL
               pointer. */
            case OTF2_SUBSTRATE_NONE:
                status = otf2_file_none_create_directory( path_copy );
                break;

            /* If substrate does not match any of the above, return error. */
            default:
                status = UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                                      "Invalid file substrate!" );
                break;
        }
        if ( status != OTF2_SUCCESS )
        {
            if ( status != OTF2_ERROR_EEXIST )
            {
                goto out;
            }
            if ( is_last && !failIfLastExists )
            {
                status = OTF2_SUCCESS;
            }
        }

        if ( is_last )
        {
            break;
        }

        *slash = '/';
        slash  = strchr( slash + 1, '/' );
        if ( !slash )
        {
            slash = path_copy + strlen( path_copy );
        }
    }

out:
    free( path_copy );

    return status;
}

/** @brief Write to a file.
 *
 *  Writes 'size' bytes from the given 'buffer' to the 'file'.
 *
 *  Calls the specific file write routine defined by the file substrate.
 *
 *  @param file             OTF2 file handle.
 *  @param buffer           Pointer to the buffer that should be written.
 *  @param size             Size of the buffer to be written in bytes.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_File_Write( OTF2_File*  file,
                 const void* buffer,
                 uint64_t    size )
{
    /* Validate arguments. */
    if ( NULL == file )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid file handle!" );
    }

    if ( NULL == buffer )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid buffer pointer!" );
    }

    if ( 0 == size )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Zero bytes to write!" );
    }

    /* If no compression is selected, write uncompresed data and return. */
    if ( file->compression == OTF2_COMPRESSION_NONE )
    {
        return otf2_file_write_buffered( file, buffer, size );
    }

    if ( OTF2_Compression_Zlib_Available()
         && file->compression == OTF2_COMPRESSION_NONE )
    {
        /* Call compression method. */
        void*    comp_buffer = NULL;
        uint32_t comp_size   = 0;

        OTF2_ErrorCode status = OTF2_Compression_Zlib_Compress( buffer, size, &comp_buffer, &comp_size );
        if ( status != OTF2_SUCCESS )
        {
            return status;
        }

        status = otf2_file_write_compression_header( file, comp_size, size );
        if ( status != OTF2_SUCCESS )
        {
            free( comp_buffer );
            return status;
        }
        status = otf2_file_write_buffered( file, comp_buffer, size );
        if ( status != OTF2_SUCCESS )
        {
            free( comp_buffer );
            return status;
        }

        file->last_block_size = comp_size;
        free( comp_buffer );

        return status;
    }

    return UTILS_ERROR( OTF2_ERROR_FILE_COMPRESSION_NOT_SUPPORTED,
                        "Requesting to operate on a compressed file without library support." );
}


/** @brief Read from a file.
 *
 *  Reads 'size' bytes from the given 'file' to the 'buffer'.
 *
 *  Calls the specific file read routine defined by the file substrate.
 *
 *  @param file             OTF2 file handle.
 *  @param buffer           Pointer to the buffer the data should be written to.
 *  @param size             Size of the buffer to be read in bytes.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_File_Read( OTF2_File* file,
                void*      buffer,
                uint64_t   size )
{
    /* Validate arguments. */
    if ( NULL == file )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid file handle!" );
    }

    if ( NULL == buffer )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid buffer pointer!" );
    }

    if ( 0 == size )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Zero bytes to read!" );
    }

    /* If no compression is selected, read uncompresed data and return. */
    if ( file->compression == OTF2_COMPRESSION_NONE )
    {
        return file->read( file, buffer, size );
    }

    if ( OTF2_Compression_Zlib_Available()
         && file->compression == OTF2_COMPRESSION_NONE )
    {
        /* Call compression method. */
        void*    comp_buffer = NULL;
        uint32_t comp_size   = 0;

        OTF2_ErrorCode status = otf2_file_read_compression_header( file, &comp_size, ( uint32_t* )&size );
        if ( status != OTF2_SUCCESS )
        {
            return status;
        }

        comp_buffer = ( void* )malloc( comp_size );

        status = file->read( file, comp_buffer, comp_size );
        if ( status != OTF2_SUCCESS )
        {
            free( comp_buffer );
            return status;
        }

        status = OTF2_Compression_Zlib_Decompress( comp_buffer, comp_size, &buffer, size );
        free( comp_buffer );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Can't decompress from buffer." );
        }

        return status;
    }

    return UTILS_ERROR( OTF2_ERROR_FILE_COMPRESSION_NOT_SUPPORTED,
                        "Requesting to operate on a compressed file without library support." );
}


/** @brief Get size of (uncompressed) unchunked file content.
 *
 *  This function is used by the buffer to determine the size that has to be
 *  allocated for a definition buffer. For uncompressed file content this
 *  function simply provides the file size. For compressed data it provides the
 *  size the decompressed data.
 *
 *  @param file             OTF2 file handle.
 *  @param size             Return value for the file content size.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_File_GetSizeUnchunked( OTF2_File* file,
                            uint64_t*  size )
{
    if ( !file || !size )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    if ( file->compression == OTF2_COMPRESSION_NONE )
    {
        return file->get_file_size( file, size );
    }

    if ( OTF2_Compression_Zlib_Available()
         && file->compression == OTF2_COMPRESSION_NONE )
    {
        uint32_t       comp_size   = 0;
        uint32_t       uncomp_size = 0;
        OTF2_ErrorCode status      =
            otf2_file_read_compression_header( file, &comp_size, &uncomp_size );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to read compression header!" );
        }
        status = file->seek( file, 0, OTF2_SEEK_SET );
        if ( status != OTF2_SUCCESS )
        {
            return UTILS_ERROR( status,
                                "Failed to seek to head of file!" );
        }

        *size = ( uint64_t )uncomp_size;
        return OTF2_SUCCESS;
    }

    return UTILS_ERROR( OTF2_ERROR_FILE_COMPRESSION_NOT_SUPPORTED,
                        "Requesting to operate on a compressed file without library support." );
}


/** @brief Seek to the begin of the previous chunk in a file.
 *
 *  For uncompressed data the file pointer is just set twice the chunk size
 *  back. For compressed data, the compression header is evaluated and the file
 *  pointer is set back the size of the compressed data. This is also done
 *  twice.
 *
 *  @param file             OTF2 file handle.
 *  @param chunkSize        Size of uncompressed chunks.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_File_SeekPrevChunk( OTF2_File* file,
                         uint64_t   chunkSize )
{
    if ( !file || !chunkSize )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    if ( file->compression == OTF2_COMPRESSION_NONE )
    {
        return file->seek( file, ( -2 ) * chunkSize, OTF2_SEEK_CUR );
    }

    if ( OTF2_Compression_Zlib_Available()
         && file->compression == OTF2_COMPRESSION_NONE )
    {
        uint32_t comp_size   = 0;
        uint32_t uncomp_size = 0;
        /* TODO: check return values */
        OTF2_ErrorCode status = otf2_file_read_compression_header( file, &comp_size, &uncomp_size );
        status = file->seek( file, ( -1 ) * file->last_block_size + OTF2_COMPRESSION_HEADER_SIZE, OTF2_SEEK_CUR );
        status = otf2_file_read_compression_header( file, &comp_size, &uncomp_size );
        status = file->seek( file, ( -1 ) * file->last_block_size + OTF2_COMPRESSION_HEADER_SIZE, OTF2_SEEK_CUR );

        return status;
    }

    return UTILS_ERROR( OTF2_ERROR_FILE_COMPRESSION_NOT_SUPPORTED,
                        "Requesting to operate on a compressed file without library support." );
}


OTF2_ErrorCode
OTF2_File_SeekChunk( OTF2_File* file,
                     uint64_t   chunkNumber,
                     uint64_t   chunkSize )
{
    if ( !file || !chunkSize )
    {
        return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                            "Invalid arguments!" );
    }

    if ( file->compression == OTF2_COMPRESSION_NONE )
    {
        return file->seek( file, ( chunkNumber - 1 ) * chunkSize, OTF2_SEEK_SET );
    }

    return UTILS_ERROR( OTF2_ERROR_FILE_INTERACTION,
                        "Operation not supported for compressed trace files!" );
}

/* ___ Implementation of static functions. __________________________________ */


/** @brief Write compression header.
 *
 *  Writes the size of the current and previous compression block.
 *
 *  @param file             OTF2 file handle.
 *  @param compSize         Size of following compressed data block.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_file_write_compression_header( OTF2_File* file,
                                    uint32_t   compSize,
                                    uint32_t   uncompSize )
{
    /* This is needed to find a header later on in the file.
     * See also definition of OTF2_COMPRESSION_HEADER. */
    uint64_t header_id = OTF2_COMPRESSION_HEADER;

    OTF2_ErrorCode status = otf2_file_write_buffered( file, &header_id, 8 );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Writing of compression header failed!" );
    }
    status = otf2_file_write_buffered( file, otf2_swap32( &compSize ), 4 );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Writing of compression header failed!" );
    }
    status = otf2_file_write_buffered( file, otf2_swap32( &uncompSize ), 4 );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Writing of compression header failed!" );
    }
    status = otf2_file_write_buffered( file, otf2_swap32( &( file->last_block_size ) ), 4 );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Writing of compression header failed!" );
    }

    return OTF2_SUCCESS;
}


/** @brief Read compression header.
 *
 *  Reads the size of the current and previous compression block.
 *
 *  @param file             OTF2 file handle.
 *  @param compSize         Size of following compressed data block.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_file_read_compression_header( OTF2_File* file,
                                   uint32_t*  compSize,
                                   uint32_t*  uncompSize )
{
    uint64_t header_id = 0;

    OTF2_ErrorCode status = file->read( file, &header_id, 8 );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Reading of compression header failed!" );
    }

    if ( header_id != OTF2_COMPRESSION_HEADER )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "This is no valid compression header!" );
    }

    status = file->read( file, compSize, 4 );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Reading of compression header failed!" );
    }
    status = file->read( file, uncompSize, 4 );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Reading of compression header failed!" );
    }
    status = file->read( file, &( file->last_block_size ), 4 );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status,
                            "Reading of compression header failed!" );
    }

    compSize              = otf2_swap32( compSize );
    uncompSize            = otf2_swap32( uncompSize );
    file->last_block_size = *( otf2_swap32( compSize ) );

    return OTF2_SUCCESS;
}


#ifdef WORDS_BIGENDIAN
/** Swap the bytes in a 4-byte buffer. */
static uint32_t*
otf2_swap32( uint32_t* value )
{
    uint8_t* bytes = ( uint8_t* )value;
    uint8_t  temp  = bytes[ 0 ];
    bytes[ 0 ] = bytes[ 3 ];
    bytes[ 3 ] = temp;
    temp       = bytes[ 1 ];
    bytes[ 1 ] = bytes[ 2 ];
    bytes[ 2 ] = temp;

    return ( uint32_t* )bytes;
}
#else
static uint32_t*
otf2_swap32( uint32_t* value )
{
    return value;
}
#endif /* WORDS_BIGENDIAN */


/** @brief Enables buffered writing with fixed size.
 *
 *  To enable writing to file system with a fixed size the data is buffered
 *  until enough data is collected to write it to the file system.
 *
 *  @param file             OTF2 file handle.
 *  @param buffer           Address of buffer containing the data.
 *  @param size             Size of the content in buffer.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_file_write_buffered( OTF2_File*  file,
                          const void* buffer,
                          uint64_t    size )
{
    /* If size is big enough just write data to file. */
    if ( size >= OTF2_FILE_BUFFER_SIZE )
    {
        return file->write( file, buffer, size );
    }

    if ( file->buffer == NULL )
    {
        file->buffer = ( void* )malloc( OTF2_FILE_BUFFER_SIZE );
        /* If malloc fails write directly to file. */
        if ( file->buffer == NULL )
        {
            return file->write( file, buffer, size );
        }
    }

    uint32_t buffer_free = OTF2_FILE_BUFFER_SIZE - file->buffer_used;

    if ( size < buffer_free )
    {
        memcpy( ( uint8_t* )file->buffer + file->buffer_used, buffer, size );
        file->buffer_used += size;
    }
    else
    {
        /* Copy first part of content so the buffer is filled and write complete
         * buffer to file system. */
        memcpy( ( uint8_t* )file->buffer + file->buffer_used, buffer, buffer_free );
        OTF2_ErrorCode status = file->write( file, file->buffer, OTF2_FILE_BUFFER_SIZE );
        if ( status != OTF2_SUCCESS )
        {
            free( file->buffer );
            return UTILS_ERROR( status,
                                "Write to file failed!" );
        }

        /* Copy second half of content to buffer. */
        memcpy( file->buffer, ( uint8_t* )buffer + buffer_free, size - buffer_free );
        file->buffer_used = size - buffer_free;
    }

    return OTF2_SUCCESS;
}
