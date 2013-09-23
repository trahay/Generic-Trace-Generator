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

#ifndef OTF2_FILE_SUBSTRATES_H
#define OTF2_FILE_SUBSTRATES_H

/** @brief Keeps all necessary information about the file handle. */
struct OTF2_File_struct
{
    /** Archive handle */
    OTF2_Archive* archive;

    /** Used compression for this file. */
    OTF2_Compression compression;

    /** The type of this file */
    OTF2_FileType file_type;
    /** The location id of this file, if appropriate */
    uint64_t      location_id;

    /** Keeps data for buffered writing. */
    void*    buffer;
    /** Keeps size of data already in buffer. */
    uint32_t buffer_used;

    /** Keeps size of last written block. Used in zlib compression layer. */
    uint32_t last_block_size;

    /** @name Function pointer to the actual file operations according to the
     *  file substrate.
     *  @{ */
    OTF2_ErrorCode ( * close )( OTF2_File* file );
    OTF2_ErrorCode ( * reset )( OTF2_File* file );
    OTF2_ErrorCode ( * write )( OTF2_File*  file,
                                const void* buffer,
                                uint64_t    size );
    OTF2_ErrorCode ( * read )( OTF2_File* file,
                               void*      buffer,
                               uint64_t   size );
    OTF2_ErrorCode ( * seek )( OTF2_File*    file,
                               int64_t       offset,
                               OTF2_FileSeek origin );
    OTF2_ErrorCode ( * get_file_size )( OTF2_File* file,
                                        uint64_t*  size );
    /** @} */
};

/* Substrate: NONE */

OTF2_File*
otf2_file_none_open( OTF2_Archive*    archive,
                     OTF2_FileMode    fileMode,
                     OTF2_FileType    fileType,
                     OTF2_LocationRef locationID );

OTF2_ErrorCode
otf2_file_none_create_directory( const char* mainPath );

/* Substrate: POSIX */

OTF2_File*
otf2_file_posix_open( OTF2_Archive*    archive,
                      OTF2_FileMode    fileMode,
                      OTF2_FileType    fileType,
                      OTF2_LocationRef locationID );


OTF2_ErrorCode
otf2_file_posix_create_directory( const char* mainPath );


/* Substrate: SION */

OTF2_File*
otf2_file_sion_open( OTF2_Archive*    archive,
                     OTF2_FileMode    fileMode,
                     OTF2_FileType    fileType,
                     OTF2_LocationRef locationID );


OTF2_ErrorCode
otf2_file_sion_create_directory( const char* mainPath );

#endif /* OTF2_FILE_SUBSTRATES_H */
