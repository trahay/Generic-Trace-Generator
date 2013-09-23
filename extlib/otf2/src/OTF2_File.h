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

#ifndef OTF2_FILE_H
#define OTF2_FILE_H

/** @brief Keeps all necessary information about the file handle.
 *  Please see OTF2_File_struct for a detailed description. */
typedef struct OTF2_File_struct OTF2_File;

/** @brief Defines which part of the buffer is to be read next.
*  Please see OTF2_FilePart_enum for a detailed description. */
typedef uint8_t OTF2_FilePart;

/** @brief Defines which part of the buffer is to be read next. */
enum OTF2_FilePart_enum
{
    /** Read next part of the file. */
    OTF2_FILEPART_NEXT = 0,
    /** Read previous part of the file. */
    OTF2_FILEPART_PREV = 1
};


/** @brief Defines start position for seeking.
 *  Please see OTF2_FileSeek_enum for a detailed description. */
typedef uint8_t OTF2_FileSeek;

/** @brief Defines start position for seeking. */
enum OTF2_FileSeek_enum
{
    /** Seek from the start of the file. */
    OTF2_SEEK_SET = 0,
    /** Seek from the current position. */
    OTF2_SEEK_CUR = 1,
    /** Seek from the end of the file. */
    OTF2_SEEK_END = 2
};


OTF2_File*
OTF2_File_Open( OTF2_Archive*    archive,
                OTF2_FileMode    fileMode,
                OTF2_FileType    fileType,
                OTF2_LocationRef location );

OTF2_ErrorCode
OTF2_File_Close( OTF2_File* file );


OTF2_ErrorCode
OTF2_File_Reset( OTF2_File* file );


OTF2_ErrorCode
OTF2_File_CreateDirectory( OTF2_Archive* archive,
                           const char*   mainPath,
                           bool          failIfLastExists );


OTF2_ErrorCode
OTF2_File_Write( OTF2_File*  file,
                 const void* buffer,
                 uint64_t    size );

OTF2_ErrorCode
OTF2_File_Read( OTF2_File* file,
                void*      buffer,
                uint64_t   size );

OTF2_ErrorCode
OTF2_File_GetSizeUnchunked( OTF2_File* file,
                            uint64_t*  size );

OTF2_ErrorCode
OTF2_File_SeekPrevChunk( OTF2_File* file,
                         uint64_t   chunkSize );

OTF2_ErrorCode
OTF2_File_SeekChunk( OTF2_File* file,
                     uint64_t   chunkNumber,
                     uint64_t   chunkSize );

#endif /* !OTF2_FILE_H */
