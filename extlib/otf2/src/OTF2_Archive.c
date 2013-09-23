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


/**
 *  @file       src/OTF2_Archive.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Writing interface for OTF2 archives.
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
#define OTF2_DEBUG_MODULE_NAME ARCHIVE
#include <UTILS_Debug.h>

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "OTF2_File.h"
#include "OTF2_AnchorFile.h"

#include "OTF2_Compression_Zlib.h"


/* ___ Prototypes for internal functions. ___________________________________ */



static inline OTF2_ErrorCode
otf2_anchorfile_version_check( OTF2_Archive* archive );



/* ___ Interactions with the archive handle. ________________________________ */



OTF2_Archive*
OTF2_Archive_Open( const char*              archivePath,
                   const char*              archiveName,
                   const OTF2_FileMode      fileMode,
                   const uint64_t           chunkSizeEvents,
                   const uint64_t           chunkSizeDefs,
                   const OTF2_FileSubstrate fileSubstrate,
                   const OTF2_Compression   compression )
{
    UTILS_DEBUG_ENTRY( "%s/%s", archivePath, archiveName );

    OTF2_Archive*  archive = NULL;
    OTF2_ErrorCode status  = OTF2_SUCCESS;

    /* Validate arguments. */
    if ( NULL == archiveName )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive name!" );
        return NULL;
    }
    if ( NULL == archivePath )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive path!" );
        return NULL;
    }

    /* Check if passed enum value for file mode is valid. */
    if ( fileMode != OTF2_FILEMODE_WRITE && fileMode != OTF2_FILEMODE_READ &&
         fileMode != OTF2_FILEMODE_MODIFY )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid value for the file mode!" );
        return NULL;
    }

    /* Check if passed enum value for file substrate is valid. */
    if ( fileSubstrate != OTF2_SUBSTRATE_POSIX &&
         fileSubstrate != OTF2_SUBSTRATE_SION &&
         fileSubstrate != OTF2_SUBSTRATE_NONE &&
         fileMode == OTF2_FILEMODE_WRITE )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid value for the file substrate!" );
        return NULL;
    }

    /* Get archive handle from internal archive. */
    if ( fileMode == OTF2_FILEMODE_WRITE )
    {
        /* Check the range of chunk sizes. */
        if ( chunkSizeEvents < OTF2_CHUNK_SIZE_MIN ||
             chunkSizeEvents > OTF2_CHUNK_SIZE_MAX )
        {
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "This is no valid event chunk size!" );
            return NULL;
        }
        if ( chunkSizeDefs < OTF2_CHUNK_SIZE_MIN ||
             chunkSizeDefs > OTF2_CHUNK_SIZE_MAX )
        {
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "This is no valid definition chunk size!" );
            return NULL;
        }

        /* Check compression. */
        if ( compression != OTF2_COMPRESSION_NONE &&
             compression != OTF2_COMPRESSION_ZLIB )
        {
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "Unhandled file compression!" );
            return NULL;
        }
        if ( !OTF2_Compression_Zlib_Available() && compression == OTF2_COMPRESSION_ZLIB )
        {
            UTILS_ERROR( OTF2_ERROR_FILE_COMPRESSION_NOT_SUPPORTED,
                         "No compression library found!" );
            return NULL;
        }

        archive = otf2_archive_open( archivePath, archiveName,
                                     fileSubstrate, compression,
                                     chunkSizeEvents, chunkSizeDefs,
                                     fileMode );
    }
    else
    {
        archive = otf2_archive_open( archivePath, archiveName,
                                     OTF2_SUBSTRATE_UNDEFINED,
                                     OTF2_COMPRESSION_UNDEFINED,
                                     OTF2_UNDEFINED_UINT64,
                                     OTF2_UNDEFINED_UINT64,
                                     fileMode );
    }

    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not get new archive handle!" );
        return NULL;
    }

    /* If in reading mode, set archive path and master.
     * And read the anchor file. */
    if ( fileMode == OTF2_FILEMODE_READ )
    {
        status = otf2_archive_set_master_slave_mode( archive, OTF2_MASTER );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status,
                         "Could not set master slave mode!" );
            return NULL;
        }

        status = OTF2_AnchorFile_Load( archive );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status,
                         "Could not read data from anchor file!" );

            return NULL;
        }

        status = otf2_anchorfile_version_check( archive );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status,
                         "The trace version conflicts with current reader version!" );
            otf2_archive_close( archive );
            return NULL;
        }
    }

    return archive;
}



OTF2_ErrorCode
OTF2_Archive_Close( OTF2_Archive* archive )
{
    OTF2_MasterSlaveMode master    = OTF2_SLAVE;
    OTF2_FileMode        file_mode = OTF2_FILEMODE_READ;
    OTF2_ErrorCode       status    = OTF2_ERROR_INVALID;

    /* Return success if NULL is passed. */
    if ( NULL == archive )
    {
        return OTF2_SUCCESS;
    }

    /* Get master slave mode and file mode.
       If a call fails try to continue anyhow. */
    status = otf2_archive_get_master_slave_mode( archive, &master );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get information if master or slave!" );
    }

    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode information!" );
    }

    /* If master and in writing mode, write a anchor file. */
    if ( master == OTF2_MASTER && file_mode == OTF2_FILEMODE_WRITE )
    {
        status = OTF2_AnchorFile_Save( archive );

        /* Print a warning if writing to the anchor file fails but continue
           anyway. */
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status, "Could not write data from anchor file!" );
        }
    }

    return otf2_archive_close( archive );
}


/* ___ Set additional information. __________________________________________ */



OTF2_ErrorCode
OTF2_Archive_SetMasterSlaveMode( OTF2_Archive*        archive,
                                 OTF2_MasterSlaveMode masterOrSlave )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( masterOrSlave != OTF2_MASTER && masterOrSlave != OTF2_SLAVE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid value for master slave mode!" );
    }

    /* Check if in writing mode. */
    OTF2_FileMode file_mode = OTF2_FILEMODE_READ;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not get file mode!" );
    }

    if ( file_mode != OTF2_FILEMODE_WRITE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This is call is not allowed in reading mode!" );
    }

    /* Check if master/slave is already set. That means work is already done.
       Return success. */
    OTF2_MasterSlaveMode master_or_slave = OTF2_UNDEFINED_TYPE;
    status = otf2_archive_get_master_slave_mode( archive, &master_or_slave );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not get master slave mode!" );
    }

    if ( master_or_slave != OTF2_UNDEFINED_TYPE )
    {
        return OTF2_SUCCESS;
    }


    /* Set master slave mode. */
    status = otf2_archive_set_master_slave_mode( archive, masterOrSlave );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not set master slave mode!" );
    }

    /* Only proceed if location is master or th archive was switched from
     * read mode. */
    if ( masterOrSlave == OTF2_SLAVE
         || archive->prev_file_mode == OTF2_FILEMODE_READ )
    {
        return OTF2_SUCCESS;
    }

    /* Get archive path and archive name. */
    char* archive_path = NULL;
    char* archive_name = NULL;

    status = otf2_archive_get_archive_path( archive, &archive_path );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not get archive name!" );
    }
    status = otf2_archive_get_trace_path( archive, &archive_name );
    if ( status != OTF2_SUCCESS )
    {
        free( archive_path );

        return UTILS_ERROR( status, "Could not get archive name!" );
    }

    /* Create archive main directory and archive trace directory. Ignore error
       if an archive main directory already exists because it is free to the
       user to create it by himself. */
    status = OTF2_File_CreateDirectory( archive, archive_path, false );
    if ( status != OTF2_SUCCESS )
    {
        free( archive_path );
        free( archive_name );

        return UTILS_ERROR( status, "Could not create archive main directory!" );
    }

    /* Please note: It is wanted that the call fails, if the directory already
     * exists. It is to prevent overwriting previous OTF2 archives. */
    status = OTF2_File_CreateDirectory( archive, archive_name, true );
    if ( status != OTF2_SUCCESS )
    {
        free( archive_path );
        free( archive_name );

        return UTILS_ERROR( status, "Could not create archive trace directory!" );
    }

    free( archive_path );
    free( archive_name );

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Archive_SetMachineName( OTF2_Archive* archive,
                             const char*   machineName )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( NULL == machineName )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid machine name!" );
    }

    status = otf2_archive_set_machine_name( archive, machineName );

    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not set machine name!" );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Archive_SetDescription( OTF2_Archive* archive,
                             const char*   description )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( NULL == description )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid description!" );
    }

    status = otf2_archive_set_description( archive, description );

    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not set description!" );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Archive_SetCreator( OTF2_Archive* archive,
                         const char*   creator )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( NULL == creator )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid creator!" );
    }

    status = otf2_archive_set_creator( archive, creator );

    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Could not set creator!" );
    }

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Archive_SetFlushCallbacks( OTF2_Archive*              archive,
                                const OTF2_FlushCallbacks* flushCallbacks,
                                void*                      flushData )
{
    if ( !archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for archive parameter!" );
    }
    if ( !flushCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for memoryCallbacks parameter!" );
    }
    if ( archive->file_mode == OTF2_FILEMODE_READ )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Memory callbacks use in reading mode!" );
    }

    /* guaranteed to process without fault */
    otf2_archive_set_flush_callbacks( archive,
                                      flushCallbacks,
                                      flushData );

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Archive_SetMemoryCallbacks( OTF2_Archive*               archive,
                                 const OTF2_MemoryCallbacks* memoryCallbacks,
                                 void*                       memoryData )
{
    if ( !archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for archive parameter!" );
    }
    if ( !memoryCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for memoryCallbacks parameter!" );
    }

    if ( !memoryCallbacks->otf2_allocate || !memoryCallbacks->otf2_free_all )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Missing function pointers in memoryCallbacks!" );
    }

    if ( archive->file_mode == OTF2_FILEMODE_READ )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Memory callbacks used in reading mode!" );
    }

    /* guaranteed to process without fault */
    otf2_archive_set_memory_callbacks( archive,
                                       memoryCallbacks,
                                       memoryData );

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Archive_SetFileSionCallbacks( OTF2_Archive*                 archive,
                                   const OTF2_FileSionCallbacks* fileSionCallbacks,
                                   void*                         fileSionData )
{
    if ( !archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for archive parameter!" );
    }
    if ( !fileSionCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for fileSionCallbacks parameter!" );
    }

    if ( archive->file_mode == OTF2_FILEMODE_READ )
    {
        /* In reading mode, we need either the get_rank callback, or both the
           open and the close callback */
        if ( !fileSionCallbacks->otf2_file_sion_get_rank &&
             !fileSionCallbacks->otf2_file_sion_open )
        {
            return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                                "Missing rank callback when in read mode!" );
        }

        if ( fileSionCallbacks->otf2_file_sion_open
             && !fileSionCallbacks->otf2_file_sion_close )
        {
            return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                                "Need a close callback when having a open callback!" );
        }
    }
    else
    {
        /* In non-reading mode we need the open and close callback */
        if ( !fileSionCallbacks->otf2_file_sion_open
             || !fileSionCallbacks->otf2_file_sion_close )
        {
            return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                                "Need open and close callback in writing mode!" );
        }
    }

    /* guaranteed to process without fault */
    otf2_archive_set_file_sion_callbacks( archive,
                                          fileSionCallbacks,
                                          fileSionData );

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
OTF2_Archive_SetProperty( OTF2_Archive* archive,
                          const char*   name,
                          const char*   value,
                          bool          overwrite )
{
    /* Validate arguments. */
    if ( !archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for archive parameter!" );
    }
    if ( !name )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for property name parameter!" );
    }
    if ( !value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for property value parameter!" );
    }
    if ( archive->file_mode != OTF2_FILEMODE_WRITE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This is call is not allowed in reading mode!" );
    }

    return otf2_archive_set_property( archive, name, value, overwrite );
}

OTF2_ErrorCode
OTF2_Archive_SetBoolProperty( OTF2_Archive* archive,
                              const char*   name,
                              bool          value,
                              bool          overwrite )
{
    /* Validate arguments. */
    if ( !archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for archive parameter!" );
    }
    if ( !name )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for property name parameter!" );
    }
    if ( archive->file_mode != OTF2_FILEMODE_WRITE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This is call is not allowed in reading mode!" );
    }

    return otf2_archive_set_property( archive,
                                      name,
                                      value ? "true" : "false",
                                      overwrite );
}

OTF2_ErrorCode
OTF2_Archive_GetPropertyNames( OTF2_Archive* archive,
                               uint32_t*     numberOfProperties,
                               char***       names )
{
    /* Validate arguments. */
    if ( !archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for archive parameter!" );
    }
    if ( !numberOfProperties )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for number of properties parameter!" );
    }
    if ( !names )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for property names parameter!" );
    }

    return otf2_archive_get_property_names( archive, numberOfProperties, names );
}

OTF2_ErrorCode
OTF2_Archive_GetProperty( OTF2_Archive* archive,
                          const char*   name,
                          char**        value )
{
    /* Validate arguments. */
    if ( !archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for archive parameter!" );
    }
    if ( !name )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for property name parameter!" );
    }

    return otf2_archive_get_property( archive, name, value );
}

OTF2_ErrorCode
OTF2_Archive_GetTraceId( OTF2_Archive* archive,
                         uint64_t*     id )
{
    /* Validate arguments. */
    if ( !archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for archive parameter!" );
    }
    if ( !id )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid argument for id parameter!" );
    }
    if ( archive->file_mode != OTF2_FILEMODE_READ )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                            "This is call is allowed in reading mode only!" );
    }

    return otf2_archive_get_trace_id( archive, id );
}

OTF2_ErrorCode
OTF2_Archive_GetMasterSlaveMode( OTF2_Archive*         archive,
                                 OTF2_MasterSlaveMode* masterOrSlave )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !masterOrSlave )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid masterOrSlave argument!" );
    }

    return otf2_archive_get_master_slave_mode( archive, masterOrSlave );
}


OTF2_ErrorCode
OTF2_Archive_GetNumberOfLocations( OTF2_Archive* archive,
                                   uint64_t*     numberOfLocations )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !numberOfLocations )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid numberOfLocations argument!" );
    }

    return otf2_archive_get_number_of_locations( archive,
                                                 numberOfLocations );
}


OTF2_ErrorCode
OTF2_Archive_GetNumberOfGlobalDefinitions( OTF2_Archive* archive,
                                           uint64_t*     numberOfGlobalDefinitions )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !numberOfGlobalDefinitions )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid numberOfGlobalDefinitions argument!" );
    }

    return otf2_archive_get_number_of_global_defs( archive,
                                                   numberOfGlobalDefinitions );
}


OTF2_ErrorCode
OTF2_Archive_GetMachineName( OTF2_Archive* archive,
                             char**        machineName )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !machineName )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid machineName argument!" );
    }

    return otf2_archive_get_machine_name( archive, machineName );
}


OTF2_ErrorCode
OTF2_Archive_GetDescription( OTF2_Archive* archive,
                             char**        description )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !description )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid description argument!" );
    }

    return otf2_archive_get_description( archive, description );
}


OTF2_ErrorCode
OTF2_Archive_GetCreator( OTF2_Archive* archive,
                         char**        creator )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !creator )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid creator argument!" );
    }

    return otf2_archive_get_creator( archive, creator );
}


OTF2_ErrorCode
OTF2_Archive_GetVersion( OTF2_Archive* archive,
                         uint8_t*      major,
                         uint8_t*      minor,
                         uint8_t*      bugfix )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !major || !minor || !bugfix )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    return otf2_archive_get_version( archive, major, minor, bugfix );
}



OTF2_ErrorCode
OTF2_Archive_GetChunkSize( OTF2_Archive* archive,
                           uint64_t*     chunkSizeEvents,
                           uint64_t*     chunkSizeDefs )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !chunkSizeEvents || !chunkSizeDefs )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    return otf2_archive_get_chunksize( archive, chunkSizeEvents, chunkSizeDefs );
}


OTF2_ErrorCode
OTF2_Archive_GetFileSubstrate( OTF2_Archive*       archive,
                               OTF2_FileSubstrate* substrate )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !substrate )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid substrate argument!" );
    }

    return otf2_archive_get_file_substrate( archive, substrate );
}


OTF2_ErrorCode
OTF2_Archive_GetCompression( OTF2_Archive*     archive,
                             OTF2_Compression* compression )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !compression )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid compression argument!" );
    }

    return otf2_archive_get_compression( archive, compression );
}


OTF2_EvtWriter*
OTF2_Archive_GetEvtWriter( OTF2_Archive*    archive,
                           OTF2_LocationRef location )
{
    OTF2_EvtWriter* writer = NULL;
    OTF2_ErrorCode  status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode == OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a event writer in read mode!" );
        return NULL;
    }

    /* Flush callbacks are mandatory for event writers */
    if ( !archive->flush_callbacks ||
         !archive->flush_callbacks->otf2_pre_flush )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a event writer without flush callbacks!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    /* location == OTF2_UNDEFINED_LOCATION is valid */

    status = otf2_archive_get_evt_writer( archive, location, &writer );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get local event writer" );

        return NULL;
    }

    return writer;
}


OTF2_DefWriter*
OTF2_Archive_GetDefWriter( OTF2_Archive*    archive,
                           OTF2_LocationRef location )
{
    OTF2_DefWriter* writer = NULL;
    OTF2_ErrorCode  status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    if ( location == OTF2_UNDEFINED_LOCATION )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid location ID given!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode == OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a definition writer in read mode!" );
        return NULL;
    }

    /* A pre_flush callback is mandatory for definition writers */
    if ( !archive->flush_callbacks ||
         !archive->flush_callbacks->otf2_pre_flush )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a definition writer without pre_flush callback!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    status = otf2_archive_get_def_writer( archive, location, &writer );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get local definition writer" );
        return NULL;
    }

    return writer;
}


OTF2_GlobalDefWriter*
OTF2_Archive_GetGlobalDefWriter( OTF2_Archive* archive )
{
    OTF2_GlobalDefWriter* writer = NULL;
    OTF2_ErrorCode        status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode == OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a global definition writer in read mode!" );
        return NULL;
    }

    /* A pre_flush callback is mandatory for global definition writers */
    if ( !archive->flush_callbacks ||
         !archive->flush_callbacks->otf2_pre_flush )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a global definition writer without pre_flush callback!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    status = otf2_archive_get_global_def_writer( archive, &writer );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get global definition writer" );
        return NULL;
    }

    return writer;
}


OTF2_SnapWriter*
OTF2_Archive_GetSnapWriter( OTF2_Archive*    archive,
                            OTF2_LocationRef location )
{
    OTF2_SnapWriter* writer = NULL;
    OTF2_ErrorCode   status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode == OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a event writer in read mode!" );
        return NULL;
    }

    /* Flush callbacks are mandatory for event writers */
    if ( !archive->flush_callbacks ||
         !archive->flush_callbacks->otf2_pre_flush )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a event writer without flush callbacks!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    /* location == OTF2_UNDEFINED_LOCATION is valid */

    status = otf2_archive_get_snap_writer( archive, location, &writer );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get local event writer" );

        return NULL;
    }

    return writer;
}


OTF2_ThumbWriter*
OTF2_Archive_GetThumbWriter( OTF2_Archive*      archive,
                             const char*        name,
                             const char*        description,
                             OTF2_ThumbnailType type,
                             uint32_t           numberOfSamples,
                             uint32_t           numberOfMetrics,
                             const uint64_t*    refsToDefs )
{
    OTF2_ThumbWriter* writer = NULL;
    OTF2_ErrorCode    status;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid archive handle!" );
        return NULL;
    }
    if ( !name || !description
         || 0 == numberOfSamples || 0 == numberOfMetrics
         || !refsToDefs )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid arguments!" );
        return NULL;
    }
    if ( type != OTF2_THUMBNAIL_TYPE_REGION
         && type != OTF2_THUMBNAIL_TYPE_METRIC
         && type != OTF2_THUMBNAIL_TYPE_ATTRIBUTES )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid thumbnail type!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode == OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a thumbnail writer in read mode!" );
        return NULL;
    }

    status = otf2_archive_get_thumb_writer( archive,
                                            &writer,
                                            name,
                                            description,
                                            type,
                                            numberOfSamples,
                                            numberOfMetrics,
                                            refsToDefs );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get local event writer" );

        return NULL;
    }

    return writer;
}


OTF2_MarkerWriter*
OTF2_Archive_GetMarkerWriter( OTF2_Archive* archive )
{
    OTF2_MarkerWriter* writer = NULL;
    OTF2_ErrorCode     status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    /* Check if archive is in slave mode (default when requested by the
     * OTF2_Reader) to prevent that the archive, in particular the anchor file,
     * can be modified. */
    OTF2_MasterSlaveMode master_or_slave;
    status = otf2_archive_get_file_mode( archive, &master_or_slave );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get master-slave mode!" );
        return NULL;
    }

    if ( master_or_slave != OTF2_MASTER )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a marker writer in slave mode!" );
        return NULL;
    }

    status = otf2_archive_get_marker_writer( archive, &writer );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get marker writer" );
        return NULL;
    }

    return writer;
}


OTF2_EvtReader*
OTF2_Archive_GetEvtReader( OTF2_Archive*    archive,
                           OTF2_LocationRef location )
{
    OTF2_EvtReader* reader = NULL;
    OTF2_ErrorCode  status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    if ( location == OTF2_UNDEFINED_LOCATION )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid location ID given!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode != OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a event reader in writing mode!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    status = otf2_archive_get_evt_reader( archive, location, &reader );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get local event reader" );

        return NULL;
    }

    return reader;
}


/** @internal
 *  @brief Get a global event reader.
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns a global event reader handle if successful,
 *                          NULL if an error occurs.
 */
OTF2_GlobalEvtReader*
OTF2_Archive_GetGlobalEvtReader( OTF2_Archive* archive )
{
    OTF2_GlobalEvtReader* reader = NULL;
    OTF2_ErrorCode        status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    /* Validate argument. */
    if ( NULL == archive->local_evt_readers )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "No event reader selected!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode != OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a global event reader in writing mode!" );
        return NULL;
    }

    status = otf2_archive_get_global_evt_reader( archive, &reader );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get global event reader" );

        return NULL;
    }

    return reader;
}


OTF2_DefReader*
OTF2_Archive_GetDefReader( OTF2_Archive*    archive,
                           OTF2_LocationRef location )
{
    OTF2_DefReader* reader = NULL;
    OTF2_ErrorCode  status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    if ( location == OTF2_UNDEFINED_LOCATION )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid location ID given!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode != OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a definition reader in writing mode!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    status = otf2_archive_get_def_reader( archive, location, &reader );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get local definition reader" );
        return NULL;
    }

    return reader;
}


OTF2_GlobalDefReader*
OTF2_Archive_GetGlobalDefReader( OTF2_Archive* archive )
{
    OTF2_GlobalDefReader* reader = NULL;
    OTF2_ErrorCode        status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode != OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a global definition reader in writing mode!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    status = otf2_archive_get_global_def_reader( archive, &reader );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get global definition reader" );

        return NULL;
    }

    return reader;
}


OTF2_SnapReader*
OTF2_Archive_GetSnapReader( OTF2_Archive*    archive,
                            OTF2_LocationRef location )
{
    OTF2_SnapReader* reader = NULL;
    OTF2_ErrorCode   status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    if ( location == OTF2_UNDEFINED_LOCATION )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid location ID given!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode != OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a snap reader in writing mode!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    status = otf2_archive_get_snap_reader( archive, location, &reader );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get local snap reader" );

        return NULL;
    }

    return reader;
}


/** @internal
 *  @brief Get a global snap reader.
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns a global snap reader handle if successful,
 *                          NULL if an error occurs.
 */
OTF2_GlobalSnapReader*
OTF2_Archive_GetGlobalSnapReader( OTF2_Archive* archive )
{
    OTF2_GlobalSnapReader* reader = NULL;
    OTF2_ErrorCode         status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    /* Validate argument. */
    if ( NULL == archive->local_snap_readers )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "No snap reader selected!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode != OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a global snap reader in writing mode!" );
        return NULL;
    }

    status = otf2_archive_get_global_snap_reader( archive, &reader );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get global event reader" );

        return NULL;
    }

    return reader;
}


OTF2_ThumbReader*
OTF2_Archive_GetThumbReader( OTF2_Archive* archive,
                             uint32_t      thumbID )
{
    OTF2_ThumbReader* reader = NULL;
    OTF2_ErrorCode    status = OTF2_ERROR_INVALID;

    /* Validate arguments. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    if ( !( thumbID < archive->number_of_thumbnails ) )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "Invalid thumb ID given!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode != OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a event reader in writing mode!" );
        return NULL;
    }

    OTF2_FileSubstrate substrate;
    status = otf2_archive_get_file_substrate( archive, &substrate );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file substrate!" );
        return NULL;
    }

    if ( substrate == OTF2_SUBSTRATE_SION && !archive->file_sion_callbacks )
    {
        UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                     "SION archive without SION callbacks!" );
        return NULL;
    }

    status = otf2_archive_get_thumb_reader( archive, thumbID, &reader );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get local event reader" );

        return NULL;
    }

    return reader;
}


OTF2_MarkerReader*
OTF2_Archive_GetMarkerReader( OTF2_Archive* archive )
{
    OTF2_MarkerReader* reader = NULL;
    OTF2_ErrorCode     status = OTF2_ERROR_INVALID;

    /* Validate argument. */
    if ( NULL == archive )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid archive handle!" );
        return NULL;
    }

    OTF2_FileMode file_mode;
    status = otf2_archive_get_file_mode( archive, &file_mode );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get file mode!" );
        return NULL;
    }

    if ( file_mode != OTF2_FILEMODE_READ )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_CALL,
                     "Requesting a marker reader in writing mode!" );
        return NULL;
    }

    status = otf2_archive_get_marker_reader( archive, &reader );

    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Could not get marker reader" );

        return NULL;
    }

    return reader;
}


OTF2_ErrorCode
OTF2_Archive_CloseEvtWriter( OTF2_Archive*   archive,
                             OTF2_EvtWriter* writer )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }

    if ( writer == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_evt_writer( archive, writer );
}


OTF2_ErrorCode
OTF2_Archive_CloseDefWriter( OTF2_Archive*   archive,
                             OTF2_DefWriter* writer )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }

    if ( writer == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_def_writer( archive, writer );
}


OTF2_ErrorCode
OTF2_Archive_CloseSnapWriter( OTF2_Archive*    archive,
                              OTF2_SnapWriter* writer )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }

    if ( writer == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_snap_writer( archive, writer );
}


OTF2_ErrorCode
OTF2_Archive_CloseMarkerWriter( OTF2_Archive*      archive,
                                OTF2_MarkerWriter* writer )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }

    if ( writer == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_marker_writer( archive, writer );
}


OTF2_ErrorCode
OTF2_Archive_CloseEvtReader( OTF2_Archive*   archive,
                             OTF2_EvtReader* reader )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }

    if ( reader == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_evt_reader( archive, reader );
}


OTF2_ErrorCode
OTF2_Archive_CloseThumbReader( OTF2_Archive*     archive,
                               OTF2_ThumbReader* reader )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }

    if ( reader == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_thumb_reader( archive, reader );
}

OTF2_ErrorCode
OTF2_Archive_CloseGlobalEvtReader( OTF2_Archive*         archive,
                                   OTF2_GlobalEvtReader* globalEvtReader )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid archive handle!" );
    }

    if ( globalEvtReader == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_global_evt_reader( archive, globalEvtReader );
}


OTF2_ErrorCode
OTF2_Archive_CloseDefReader( OTF2_Archive*   archive,
                             OTF2_DefReader* reader )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }

    if ( reader == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_def_reader( archive, reader );
}


OTF2_ErrorCode
OTF2_Archive_CloseGlobalDefReader( OTF2_Archive*         archive,
                                   OTF2_GlobalDefReader* globalDefReader )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid archive handle!" );
    }

    if ( globalDefReader == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_global_def_reader( archive, globalDefReader );
}


OTF2_ErrorCode
OTF2_Archive_CloseSnapReader( OTF2_Archive*    archive,
                              OTF2_SnapReader* reader )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }

    if ( reader == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_snap_reader( archive, reader );
}


OTF2_ErrorCode
OTF2_Archive_CloseGlobalSnapReader( OTF2_Archive*          archive,
                                    OTF2_GlobalSnapReader* globalSnapReader )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid archive handle!" );
    }

    if ( globalSnapReader == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_global_snap_reader( archive, globalSnapReader );
}


OTF2_ErrorCode
OTF2_Archive_CloseMarkerReader( OTF2_Archive*      archive,
                                OTF2_MarkerReader* markerReader )
{
    /* Validate argument. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid archive handle!" );
    }

    if ( markerReader == NULL )
    {
        return OTF2_SUCCESS;
    }

    return otf2_archive_close_marker_reader( archive, markerReader );
}


/** @internal
 *  @brief Check version of trace archive.
 *
 *  Checks if the version of the trace archive works with the version of the
 *  current reader.
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static inline OTF2_ErrorCode
otf2_anchorfile_version_check( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );

    uint8_t        version = 0;
    OTF2_ErrorCode status  = otf2_archive_get_trace_format( archive, &version );
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Unable to get trace format version!" );
    }

    /*
     * We are currently only able to read version 1 and 2 of the trace format.
     */
    if ( version <= OTF2_TRACE_FORMAT )
    {
        return OTF2_SUCCESS;
    }

    return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                        "The trace version conflicts with current reader version!" );
}


/** @brief Switch from read to write mode.
 *
 *  @param archive          Archive handle.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_SwitchFileMode( OTF2_Archive* archive,
                             OTF2_FileMode newFileMode )
{
    /* validate arguments */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid archive handle!" );
    }

    /* Check if passed enum value for file mode is valid. */
    if ( newFileMode != OTF2_FILEMODE_WRITE
         && newFileMode != OTF2_FILEMODE_READ
         && newFileMode != OTF2_FILEMODE_MODIFY )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid value for the file mode!" );
    }

    if ( !( archive->file_mode == OTF2_FILEMODE_READ
            && newFileMode == OTF2_FILEMODE_WRITE
            && archive->substrate == OTF2_SUBSTRATE_POSIX ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_FILE_MODE_TRANSITION,
                            "File mode transition not permitted!" );
    }

    archive->master_or_slave = OTF2_UNDEFINED_TYPE;
    archive->prev_file_mode  = archive->file_mode;
    archive->file_mode       = newFileMode;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_Archive_GetNumberOfThumbnails( OTF2_Archive* archive,
                                    uint32_t*     number )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !number )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid 'number' argument!" );
    }

    return otf2_archive_get_number_of_thumbnails( archive, number );
}


OTF2_ErrorCode
OTF2_Archive_SetNumberOfSnapshots( OTF2_Archive* archive,
                                   uint32_t      number )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    return otf2_archive_set_number_of_snapshots( archive, number );
}


OTF2_ErrorCode
OTF2_Archive_GetNumberOfSnapshots( OTF2_Archive* archive,
                                   uint32_t*     number )
{
    /* Validate arguments. */
    if ( NULL == archive )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid archive handle!" );
    }
    if ( !number )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid 'number' argument!" );
    }
    return otf2_archive_get_number_of_snapshots( archive, number );
}
