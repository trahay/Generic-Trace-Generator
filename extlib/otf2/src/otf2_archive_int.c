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
 *  @file       src/otf2_archive_int.c
 *  @status     alpha
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief This module is an internal representation of the opened
 *         archive. It is mainly used to organize the archive related
 *         meta-data.
 *
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>
#include <UTILS_CStr.h>
#include <UTILS_IO.h>

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "otf2_attic_types.h"

#include "otf2_local_definitions.h"
#include "otf2_global_definitions.h"
#include "otf2_events_int.h"
#include "otf2_snaps.h"

#include "otf2_attribute_list.h"
#include "otf2_id_map.h"

#include "otf2_def_reader.h"
#include "otf2_def_writer.h"
#include "otf2_evt_reader.h"
#include "otf2_evt_writer.h"
#include "otf2_snap_reader.h"
#include "otf2_snap_writer.h"

#include "otf2_global_def_reader.h"
#include "otf2_global_def_writer.h"
#include "otf2_global_evt_reader.h"
#include "otf2_global_snap_reader.h"

#include "otf2_thumbnail.h"

#include "otf2_marker_int.h"

#define VALID_PROPERTY_NAME_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"

/*__SET_FUNCTIONS____________________________________________*/



/** @brief Create a new internal archive.
 *
 *  This function generates the basic datastructure for an internal archive representation,
 *  which is mainly used to hold the metadata on runtime. Every member variable can be set
 *  or get with related get and set routines.
 *
 *  @param archivePath      Path to the archive i.e. the directory where the
 *                          anchor file is located.
 *  @param archiveName      Name of the archive. It is used to generate sub
 *                          pathes e.g. 'archiveName.otf2'.
 *  @param substrate        Determines which file substrate should be used in
 *                          writing mode. In reading mode this value is ignored
 *                          because the correct file substrated is extracted
 *                          from the anchor file.
 *  @param chunksize        Requested size of the internal chunks in writing
 *                          mode. In reading mode this value is ignored because
 *                          the correct chunk size is extracted from the
 *                          anchor file.
 *  @param file_mode        Determines if in reading, writing or modify mode.
 *
 *  @return                 Returns an archive handle if successful, NULL
 *                          otherwise.
 */

OTF2_Archive*
otf2_archive_open( const char*        archivePath,
                   const char*        archiveName,
                   OTF2_FileSubstrate substrate,
                   OTF2_Compression   compression,
                   uint64_t           chunkSizeEvents,
                   uint64_t           chunkSizeDefs,
                   OTF2_FileMode      fileMode )
{
    OTF2_ErrorCode ret_error = OTF2_ERROR_INVALID;

    UTILS_ASSERT( archivePath );
    UTILS_ASSERT( archiveName );

    /* Generate the initial structure and do some controling */
    OTF2_Archive* ret = calloc( 1, sizeof( *ret ) );

    if ( !ret )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Alloc failed!" );
        return NULL;
    }

    /* Set initial values */
    ret->master_or_slave   = OTF2_UNDEFINED_TYPE;
    ret->file_mode         = OTF2_UNDEFINED_TYPE;
    ret->prev_file_mode    = OTF2_UNDEFINED_TYPE;
    ret->substrate         = OTF2_SUBSTRATE_UNDEFINED;
    ret->compression       = OTF2_COMPRESSION_UNDEFINED;
    ret->chunk_size_events = OTF2_UNDEFINED_UINT64;
    ret->chunk_size_defs   = OTF2_UNDEFINED_UINT64;

    ret->number_of_locations   = OTF2_UNDEFINED_UINT64;
    ret->number_of_global_defs = OTF2_UNDEFINED_UINT64;

    ret->number_of_thumbnails = 0;
    ret->number_of_snapshots  = 0;

    /* Already known values */

    if ( chunkSizeEvents != OTF2_UNDEFINED_UINT64 &&
         chunkSizeDefs != OTF2_UNDEFINED_UINT64 )
    {
        ret_error = otf2_archive_set_chunksize( ret, chunkSizeEvents, chunkSizeDefs );

        if ( ret_error != OTF2_SUCCESS )
        {
            otf2_archive_close( ret );
            UTILS_ERROR( ret_error, "Set of chunk sizes failed!" );
            return NULL;
        }
    }

    ret_error = otf2_archive_set_file_mode( ret, fileMode );
    if ( ret_error != OTF2_SUCCESS )
    {
        otf2_archive_close( ret );
        UTILS_ERROR( ret_error, "Set failed!" );
        return NULL;
    }

    if ( substrate != OTF2_SUBSTRATE_UNDEFINED )
    {
        ret_error = otf2_archive_set_file_substrate( ret, substrate );

        if ( ret_error != OTF2_SUCCESS )
        {
            otf2_archive_close( ret );
            UTILS_ERROR( ret_error, "Set failed!" );
            return NULL;
        }
    }

    if ( compression != OTF2_COMPRESSION_UNDEFINED )
    {
        ret_error = otf2_archive_set_compression( ret, compression );

        if ( ret_error != OTF2_SUCCESS )
        {
            otf2_archive_close( ret );
            UTILS_ERROR( ret_error, "Set failed!" );
            return NULL;
        }
    }

    /* Set the archive path */
    ret_error = otf2_archive_set_archive_path( ret, archivePath );
    if ( ret_error != OTF2_SUCCESS )
    {
        otf2_archive_close( ret );
        UTILS_ERROR( ret_error, "Set failed!" );
        return NULL;
    }


    /* Set the archive name */
    ret_error = otf2_archive_set_archive_name( ret, archiveName );
    if ( ret_error != OTF2_SUCCESS )
    {
        otf2_archive_close( ret );
        UTILS_ERROR( ret_error, "Set failed!" );
        return NULL;
    }

    /* Set the OTF2 and trace format versions */
    if ( fileMode != OTF2_FILEMODE_READ )
    {
        ret_error = otf2_archive_set_version( ret,
                                              OTF2_VERSION_MAJOR,
                                              OTF2_VERSION_MINOR,
                                              OTF2_VERSION_BUGFIX );
        if ( ret_error != OTF2_SUCCESS )
        {
            otf2_archive_close( ret );
            UTILS_ERROR( ret_error, "Set failed!" );
            return NULL;
        }

        ret_error = otf2_archive_set_trace_format( ret, OTF2_TRACE_FORMAT );
        if ( ret_error != OTF2_SUCCESS )
        {
            otf2_archive_close( ret );
            UTILS_ERROR( ret_error, "Set failed!" );
            return NULL;
        }
    }

    /* Initialize the lists where already allocated readers/writers are stored */
    ret->local_evt_readers = NULL;
    ret->local_def_readers = NULL;
    ret->local_evt_writers = NULL;
    ret->local_def_writers = NULL;

    ret->local_snap_readers = NULL;
    ret->local_snap_writers = NULL;

    ret->thumb_writers = NULL;
    ret->thumb_readers = NULL;

    ret->marker_reader = NULL;
    ret->marker_writer = NULL;

    return ret;
}



/** @brief Deconstruct a processed archive.
 *
 *  This is more a finalization step. If this function is called, the archive will be
 *  finalized and completly written to disk. If this function is succsessful, a valid
 *  OTF2 archive is written to disk. All included reader and writer objects will be
 *  finalized and destroid. Their data is automatically written to disk.
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns a writer object or NULL in case of an error.
 */

OTF2_ErrorCode
otf2_archive_close( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );

    OTF2_ErrorCode ret;

    /* All writers must be closed etc. */
    while ( archive->local_evt_writers )
    {
        OTF2_EvtWriter* next = archive->local_evt_writers->next;
        otf2_evt_writer_delete( archive->local_evt_writers );
        archive->local_evt_writers = next;
    }

    while ( archive->local_def_writers )
    {
        OTF2_DefWriter* next = archive->local_def_writers->next;
        otf2_def_writer_delete( archive->local_def_writers );
        archive->local_def_writers = next;
    }

    if ( archive->global_def_writer )
    {
        ret = otf2_global_def_writer_delete( archive->global_def_writer );
        if ( ret != OTF2_SUCCESS )
        {
            UTILS_ERROR( ret, "Free of global definition writer failed!" );
        }
    }

    while ( archive->local_snap_writers )
    {
        OTF2_SnapWriter* next = archive->local_snap_writers->next;
        otf2_snap_writer_delete( archive->local_snap_writers );
        archive->local_snap_writers = next;
    }

    while ( archive->thumb_writers )
    {
        OTF2_ThumbWriter* next = archive->thumb_writers->next;
        otf2_thumb_writer_delete( archive->thumb_writers );
        archive->thumb_writers = next;
    }

    if ( archive->marker_writer )
    {
        ret = otf2_marker_writer_delete( archive->marker_writer );
        if ( ret != OTF2_SUCCESS )
        {
            UTILS_ERROR( ret, "Free of marker writer failed!" );
        }
    }

    /* Readers must be freed also */
    if ( archive->global_evt_reader )
    {
        ret = otf2_global_evt_reader_delete( archive->global_evt_reader );
        if ( ret != OTF2_SUCCESS )
        {
            UTILS_ERROR( ret, "Free of global event reader failed!" );
        }
    }

    while ( archive->local_evt_readers )
    {
        OTF2_EvtReader* next = archive->local_evt_readers->next;
        otf2_evt_reader_delete( archive->local_evt_readers );
        archive->local_evt_readers = next;
    }

    while ( archive->local_def_readers )
    {
        OTF2_DefReader* next = archive->local_def_readers->next;
        otf2_def_reader_delete( archive->local_def_readers );
        archive->local_def_readers = next;
    }

    if ( archive->global_def_reader )
    {
        ret = otf2_global_def_reader_delete( archive->global_def_reader );
        if ( ret != OTF2_SUCCESS )
        {
            UTILS_ERROR( ret, "Free of global definition reader failed!" );
        }
    }

    if ( archive->global_snap_reader )
    {
        ret = otf2_global_snap_reader_delete( archive->global_snap_reader );
        if ( ret != OTF2_SUCCESS )
        {
            UTILS_ERROR( ret, "Free of global snpshot reader failed!" );
        }
    }

    while ( archive->local_snap_readers )
    {
        OTF2_SnapReader* next = archive->local_snap_readers->next;
        otf2_snap_reader_delete( archive->local_snap_readers );
        archive->local_snap_readers = next;
    }

    while ( archive->thumb_readers )
    {
        OTF2_ThumbReader* next = archive->thumb_readers->next;
        otf2_thumb_reader_delete( archive->thumb_readers );
        archive->thumb_readers = next;
    }

    if ( archive->marker_reader )
    {
        ret = otf2_marker_reader_delete( archive->marker_reader );
        if ( ret != OTF2_SUCCESS )
        {
            UTILS_ERROR( ret, "Free of marker reader failed!" );
        }
    }

    /* Free properties */
    otf2_archive_property* current_property = archive->properties;
    otf2_archive_property* next_property;
    while ( current_property != NULL )
    {
        next_property = current_property->next;
        free( current_property->name );
        free( current_property->value );
        free( current_property );
        current_property = next_property;
    }

    /* Free all path buffers */
    free( archive->archive_path );
    free( archive->archive_name );
    free( archive->machine_name );
    free( archive->description );
    free( archive->creator );
    free( archive );

    return OTF2_SUCCESS;
}

/*_set_functions_____________________________________________________________*/



/** @brief Set the path to the folder where the archive is stored.
 *
 *
 *  @param archive      Archive handle.
 *  @param archivePath  Path of the archives folder.
 *
 *  @return             OTF2_SUCCESS, or error.
 */

OTF2_ErrorCode
otf2_archive_set_archive_path( OTF2_Archive* archive,
                               const char*   archivePath )
{
    /* Be sure that valid pointers where passed */
    if ( !archive || !archivePath )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT, "No valid pointer!" );
    }

    /* File path can never be changed and set only one time */
    if ( archive->archive_path )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT, "Archive path already set!" );
    }

    /* Get enough space for the path */
    archive->archive_path = UTILS_CStr_dup( archivePath );
    if ( !archive->archive_path )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Can't allocate filepath!" );
    }

    return OTF2_SUCCESS;
}



/** @brief Set the archive name which is partly used to generate the file path
 *         to the folder where the archive is stored.
 *
 *
 *  @param archive          Archive handle.
 *  @param archiveName      Archive name, which is partly used to generate sub path.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */

OTF2_ErrorCode
otf2_archive_set_archive_name( OTF2_Archive* archive,
                               const char*   archiveName )
{
    /* Test for valid pointers */
    UTILS_ASSERT( archive );
    UTILS_ASSERT( archiveName );

    /* Test if there is already a path set */
    if ( archive->archive_name )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Archive name is already set." );
    }

    /* The input string must be checked for '/' */
    if ( strchr( archiveName, '/' ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Archive name must not contain '/'!" );
    }

    /* Make a copy of the archive name */
    archive->archive_name = UTILS_CStr_dup( archiveName );
    if ( !archive->archive_name )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Memory allocation failed!" );
    }

    return OTF2_SUCCESS;
}



/** @brief Set the OTF2 version
 *
 *  @param archive Archive handle.
 *  @param majorn  OTF2 version identifier
 *  @param minori  OTF2 version identifier
 *  @param bugfix  OTF2 version identifier
 *
 *  @return        Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_version( OTF2_Archive* archive,
                          uint8_t       major,
                          uint8_t       minor,
                          uint8_t       bugfix )
{
    UTILS_ASSERT( archive );

    archive->version[ 0 ] = major;
    archive->version[ 1 ] = minor;
    archive->version[ 2 ] = bugfix;

    return OTF2_SUCCESS;
}



/** @brief Set the trace format version
 *         The trace format version is stored to the anchor file, to make format
 *         incompatibilities detectable.
 *
 *  @param archive Archive handle.
 *  @param version Trace format version identifier
 *
 *  @return        Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_trace_format( OTF2_Archive* archive,
                               uint8_t       version )
{
    UTILS_ASSERT( archive );

    archive->trace_format = version;

    return OTF2_SUCCESS;
}


/** @brief Set the chunksize.
 *         The chunksize for written event traces must be set globally. Therefore
 *         this size must be saved to the archive management.
 *
 *  @param archive          Archive handle.
 *  @param chunksize        Chunksize
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_chunksize( OTF2_Archive* archive,
                            uint64_t      chunkSizeEvents,
                            uint64_t      chunkSizeDefs )
{
    UTILS_ASSERT( archive );

    if ( chunkSizeEvents == OTF2_UNDEFINED_UINT64 ||
         chunkSizeDefs == OTF2_UNDEFINED_UINT64 )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't allocate chunks with undefined length!" );
    }

    if ( archive->chunk_size_events != OTF2_UNDEFINED_UINT64 ||
         archive->chunk_size_defs != OTF2_UNDEFINED_UINT64 )
    {
        return UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                            "Set failed, because value is already set!" );
    }

    archive->chunk_size_events = chunkSizeEvents;
    archive->chunk_size_defs   = chunkSizeDefs;

    return OTF2_SUCCESS;
}



/** @brief Set the name of the machine where the trace was recorded. This is optional!
 *
 *  @param archive          Archive handle.
 *  @param machineName      Machine name.
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_machine_name( OTF2_Archive* archive,
                               const char*   machineName )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( machineName );

    /* Make a copy of the string */
    archive->machine_name = UTILS_CStr_dup( machineName );
    if ( !archive->machine_name )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Allocation failed!" );
    }

    return OTF2_SUCCESS;
}



/** @brief Set the description of the trace. This is optional!
 *
 *  @param archive          Archive handle.
 *  @param description      Description of the performance experiment which is stored in this trace.
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_description( OTF2_Archive* archive,
                              const char*   description )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( description );

    /* Make a copy of the string */
    archive->description = UTILS_CStr_dup( description );
    if ( !archive->description )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Allocation failed!" );
    }

    return OTF2_SUCCESS;
}


/** @brief To determine who has recorded the trace. This is optional!
 *
 *  @param archive          Archive handle.
 *  @param creator          Creator of the OTF2 trace archive.
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_creator( OTF2_Archive* archive,
                          const char*   creator )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( creator );

    /* Make a copy of the string */
    archive->creator = UTILS_CStr_dup( creator );
    if ( !archive->creator )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Allocation failed!" );
    }

    return OTF2_SUCCESS;
}


/** @brief Set the number locations.
 *
 *  @param archive          Archive handle.
 *  @param locationNumber   Number of locations.
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_number_of_locations( OTF2_Archive* archive,
                                      uint64_t      numberOfLocations )
{
    UTILS_ASSERT( archive );

    archive->number_of_locations = numberOfLocations;

    return OTF2_SUCCESS;
}


/** @brief Set the number of global definition records, which are stored in this trace.
 *         This can later be used for progress calculation.
 *
 *  @param archive                 Archive handle.
 *  @param globalDefRecordsNumber  Number of global definition records.
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_number_of_global_defs( OTF2_Archive* archive,
                                        uint64_t      numberOfGlobalDefs )
{
    UTILS_ASSERT( archive );

    archive->number_of_global_defs = numberOfGlobalDefs;

    return OTF2_SUCCESS;
}



/** @brief The master or slave flag determines which role the actual archive instance
 *         plays. A master stores the metadata into the anchor file and the global
 *         definition file. Furthermore does the master establish the basic folder
 *         hirarchy.
 *
 *  @param archive        Archive handle.
 *  @param masterOrSlave  Information about the mode of this archive instance
 *
 *  @return               Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_master_slave_mode( OTF2_Archive*        archive,
                                    OTF2_MasterSlaveMode masterOrSlave )
{
    UTILS_ASSERT( archive );

    /* The variable master_or_slave can only set once and is initialised with 128 */
    if ( archive->master_or_slave != OTF2_UNDEFINED_TYPE )
    {
        if ( archive->master_or_slave != masterOrSlave )
        {
            return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                                "Master or slave already set!" );
        }
    }

    /* If the argument isn't 0 or 1 ther is something going wrong */
    if ( ( masterOrSlave != 0 ) && ( masterOrSlave != 1 ) )
    {
        return UTILS_ERROR( OTF2_ERROR_INDEX_OUT_OF_BOUNDS,
                            "Value is not master or slave!" );
    }

    archive->master_or_slave = masterOrSlave;

    return OTF2_SUCCESS;
}



/** @brief Set the interaction mode (reading, writing, modification)
 *
 *  @param archive    Archive handle.
 *  @param fileMode   Reading, writing or modification mode?
 *
 *  @return           Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_file_mode( OTF2_Archive* archive,
                            OTF2_FileMode fileMode )
{
    UTILS_ASSERT( archive );

    /* Check that a possible value is passed as argument */
    if ( !( ( fileMode == OTF2_FILEMODE_READ ) ||
            ( fileMode == OTF2_FILEMODE_WRITE ) ||
            ( fileMode == OTF2_FILEMODE_MODIFY ) )
         )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Filemode argument is not valid!" );
    }

    if ( archive->file_mode != OTF2_UNDEFINED_TYPE )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Filemode is already set!" );
    }

    archive->file_mode = fileMode;

    return OTF2_SUCCESS;
}


/** @brief Set the file substrate (posix, sion, none)
 *
 *  @param archive    Archive handle.
 *  @param Substrate  Used file substrate: OTF2_SUBSTRATE_POSIX,
 *                    OTF2_SUBSTRATE_SION, or OTF2_SUBSTRATE_NONE
 *
 *  @return           Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_set_file_substrate( OTF2_Archive*      archive,
                                 OTF2_FileSubstrate substrate )
{
    UTILS_ASSERT( archive );


    /* File substrate can always given only once */
    if ( archive->substrate != OTF2_SUBSTRATE_UNDEFINED )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Substrate is already set!" );
    }

    /* Check that a possible value is passed as argument */
    if ( !( ( substrate == OTF2_SUBSTRATE_POSIX ) ||
            ( substrate == OTF2_SUBSTRATE_SION ) ||
            ( substrate == OTF2_SUBSTRATE_NONE ) )
         )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "The substrate is not correct!" );
    }

    archive->substrate = substrate;

    return OTF2_SUCCESS;
}


/** @brief Set the compression (none, zlib)
 *
 *  @param archive      Archive handle.
 *  @param compression  Compression mode: OTF2_COMPRESSION_NONE or
 *                      OTF2_COMPRESSION_ZLIB.
 *
 *  @return             Returns an error code if something was going wrong.
 */

OTF2_ErrorCode
otf2_archive_set_compression( OTF2_Archive*    archive,
                              OTF2_Compression compression )
{
    UTILS_ASSERT( archive );


    /* Compression can always given only once */
    if ( archive->compression != OTF2_COMPRESSION_UNDEFINED )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Compression is already set!" );
    }

    /* Check that a possible value is passed as argument */
    if ( !( ( compression == OTF2_COMPRESSION_NONE ) ||
            ( compression == OTF2_COMPRESSION_ZLIB ) )
         )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid compression mode!" );
    }

    archive->compression = compression;

    return OTF2_SUCCESS;
}

void
otf2_archive_set_flush_callbacks( OTF2_Archive*              archive,
                                  const OTF2_FlushCallbacks* flushCallbacks,
                                  void*                      flushData )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( flushCallbacks );
    UTILS_ASSERT( archive->file_mode == OTF2_FILEMODE_WRITE );

    archive->flush_callbacks = flushCallbacks;
    archive->flush_data      = flushData;
}


void
otf2_archive_set_memory_callbacks( OTF2_Archive*               archive,
                                   const OTF2_MemoryCallbacks* memoryCallbacks,
                                   void*                       memoryData )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( memoryCallbacks );
    UTILS_ASSERT( memoryCallbacks->otf2_allocate );
    UTILS_ASSERT( memoryCallbacks->otf2_free_all );
    UTILS_ASSERT( archive->file_mode == OTF2_FILEMODE_WRITE );

    archive->allocator_callbacks = memoryCallbacks;
    archive->allocator_data      = memoryData;
}


void
otf2_archive_set_file_sion_callbacks( OTF2_Archive*                 archive,
                                      const OTF2_FileSionCallbacks* fileSionCallbacks,
                                      void*                         fileSionData )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( fileSionCallbacks );

    archive->file_sion_callbacks = fileSionCallbacks;
    archive->file_sion_data      = fileSionData;
}

OTF2_ErrorCode
otf2_archive_set_property( OTF2_Archive* archive,
                           const char*   name,
                           const char*   value,
                           bool          overwrite )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( name );
    UTILS_ASSERT( value );

    bool remove_property = false;

    /* Validate property name syntax */
    const char* name_check_pos = name;
    const char* name_sep       = "";
    do
    {
        name_sep = strstr( name_check_pos, "::" );
        if ( !name_sep )
        {
            /* If this was the first iteration and we did not found a namespace
             * separator, than we fail */
            if ( name_check_pos == name )
            {
                return UTILS_ERROR( OTF2_ERROR_PROPERTY_NAME_INVALID,
                                    "Property name does not contain a namespace "
                                    "separator." );
            }
            /* Last name component expands till the end of the string */
            name_sep = name_check_pos + strlen( name_check_pos );
        }

        /* Check if the name component has a none-zero length */
        size_t name_len = name_sep - name_check_pos;
        if ( name_len == 0 )
        {
            return UTILS_ERROR( OTF2_ERROR_PROPERTY_NAME_INVALID,
                                "Namespace component of property name is empty." );
        }

        /* Check for valid characters in the name component */
        if ( strspn( name_check_pos, VALID_PROPERTY_NAME_CHARS ) != name_len )
        {
            return UTILS_ERROR( OTF2_ERROR_PROPERTY_NAME_INVALID,
                                "Property name contains invalid characters. "
                                "Please use only [A-Z0-9_]: '%.*s'",
                                ( int )name_len, name_check_pos );
        }

        /* Skip :: and advance check position */
        name_check_pos = name_sep + 2;
    }
    while ( *name_sep );

    /* Convert property name to upper case */
    char* property_name = UTILS_CStr_dup( name );
    for ( char* ptr = property_name; *ptr; ptr++ )
    {
        *ptr = toupper( *ptr );
    }

    /* Removing a trace file property is done by passing "" in the
    * value parameter. The overwrite parameter is ignored than. */
    if ( strlen( value ) == 0 )
    {
        remove_property = true;
    }

    if ( archive->properties == NULL )
    {
        /* Set head element of list */

        if ( remove_property )
        {
            free( property_name );
            return OTF2_SUCCESS;
        }

        archive->properties = malloc( sizeof( otf2_archive_property ) );
        if ( !archive->properties )
        {
            free( property_name );
            UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
            return OTF2_ERROR_MEM_FAULT;
        }

        archive->properties->name  = property_name;
        archive->properties->value = UTILS_CStr_dup( value );
        archive->properties->next  = archive->properties->previous  = NULL;
        archive->number_of_properties++;
    }
    else
    {
        /* At least one element in list */

        otf2_archive_property* current_property = archive->properties;
        while ( true )
        {
            /* Look for an already existing element with the same name */
            if ( strcmp( current_property->name, property_name ) == 0 )
            {
                /* Already existing element with same name found */
                if ( remove_property )
                {
                    /* Free memory for strings */
                    free( current_property->name );
                    free( current_property->value );

                    if ( current_property == archive->properties )
                    {
                        /* Remove first element in list */
                        if ( current_property->next == NULL )
                        {
                            /* It's the one and only element in list */
                            archive->properties = NULL;
                        }
                        else
                        {
                            archive->properties           = current_property->next;
                            archive->properties->previous = NULL;
                        }
                    }
                    else if ( current_property->next == NULL )
                    {
                        /* Remove last element in list */
                        current_property->previous->next = NULL;
                    }
                    else
                    {
                        /* Free element in the middle of the list */
                        current_property->previous->next = current_property->next;
                        current_property->next->previous = current_property->previous;
                    }
                    free( current_property );
                    free( property_name );
                    archive->number_of_properties--;
                    return OTF2_SUCCESS;
                }

                /* Property is already available */
                if ( overwrite )
                {
                    /* Delete old value */
                    free( current_property->value );
                    /* Write new value */
                    current_property->value = UTILS_CStr_dup( value );

                    free( property_name );
                    return OTF2_SUCCESS;
                }

                /* Return with error code if property is already available but should not be overwritten */
                free( property_name );
                return OTF2_ERROR_PROPERTY_EXISTS;
            }

            /* Continue with next element in list */
            if ( current_property->next == NULL )
            {
                break;
            }
            else
            {
                current_property = current_property->next;
            }
        }

        if ( remove_property )
        {
            /* Property requested to remove but not found. */
            free( property_name );
            return OTF2_ERROR_PROPERTY_NOT_FOUND;
        }

        /* Property is not available in list yet, add new one */
        current_property->next = malloc( sizeof( otf2_archive_property ) );
        if ( !current_property->next )
        {
            UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
            return OTF2_ERROR_MEM_FAULT;
        }

        current_property->next->name     = property_name;
        current_property->next->value    = UTILS_CStr_dup( value );
        current_property->next->next     = NULL;
        current_property->next->previous = current_property;
        archive->number_of_properties++;
    }

    return OTF2_SUCCESS;
}


/** @brief Set the identifier of the trace file.
 *
 *  @param archive          Archive handle.
 *  @param id               Trace identifier.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_set_trace_id( OTF2_Archive* archive,
                           uint64_t      id )
{
    UTILS_ASSERT( archive );

    archive->trace_id = id;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_set_number_of_snapshots( OTF2_Archive* archive,
                                      uint32_t      number )
{
    UTILS_ASSERT( archive );

    archive->number_of_snapshots = number;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_set_number_of_thumbnails( OTF2_Archive* archive,
                                       uint32_t      numberOfThumbnails )
{
    UTILS_ASSERT( archive );

    archive->number_of_thumbnails = numberOfThumbnails;

    return OTF2_SUCCESS;
}


/*__get_functions__________________________________________________*/



/** @brief Get the OTF2 version. This was hopefully set by
 *         otf2_archive_set_version.
 *
 *  @param archive          Archive handle.
 *  @param major            OTF2 version identifier
 *  @param minor            OTF2 version identifier
 *  @param bugfix           OTF2 version identifier
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_version( OTF2_Archive* archive,
                          uint8_t*      major,
                          uint8_t*      minor,
                          uint8_t*      bugfix )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( major );
    UTILS_ASSERT( minor );
    UTILS_ASSERT( bugfix );

    *major  = archive->version[ 0 ];
    *minor  = archive->version[ 1 ];
    *bugfix = archive->version[ 2 ];

    return OTF2_SUCCESS;
}



/** @brief Get the trace format version. This was hopefully set by
 *         otf2_archive_set_trace_format.
 *
 *  @param archive          Archive handle.
 *  @param trace_format     Trace format version identifier
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_trace_format( OTF2_Archive* archive,
                               uint8_t*      version )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( version );

    *version = archive->trace_format;

    return OTF2_SUCCESS;
}



/** @brief Get the chunksize.
 *         The chunksize for written event traces must be set globally. Therefore
 *         this size must be saved to the archive management.
 *
 *  @param archive          Archive handle.
 *  @param chunksize        Chunksize
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_chunksize( OTF2_Archive* archive,
                            uint64_t*     chunkSizeEvents,
                            uint64_t*     chunkSizeDefs )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( chunkSizeEvents );
    UTILS_ASSERT( chunkSizeDefs );

    *chunkSizeEvents = archive->chunk_size_events;
    *chunkSizeDefs   = archive->chunk_size_defs;

    return OTF2_SUCCESS;
}



/** @brief Get the name of the machine where the trace was recorded. This is optional!
 *
 *  @param archive          Archive handle.
 *  @param machineName      Name of the machine the trace was created.
 *
 *  @return                 Returns an error code if something was going wrong,
 *                          or there was no name set before.
 */

OTF2_ErrorCode
otf2_archive_get_machine_name( OTF2_Archive* archive,
                               char**        machineName )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( machineName );

    /* Check if there is no name set, return an error */
    if ( !archive->machine_name )
    {
        return OTF2_ERROR_MEM_FAULT;
    }

    /* Make a copy of the string */
    *machineName = UTILS_CStr_dup( archive->machine_name );
    if ( !*machineName )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
    }

    return OTF2_SUCCESS;
}



/** @brief Get the description of the trace. This is optional!
 *
 *  @param archive          Archive handle.
 *  @param description      Description of the performance experiment which is stored in this trace.
 *
 *  @return                 Returns an error code if something was going wrong,
 *                          or there was no name set before.
 */

OTF2_ErrorCode
otf2_archive_get_description( OTF2_Archive* archive,
                              char**        description )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( description );

    /* Check if there is no description set, return an error */
    if ( !archive->description )
    {
        return OTF2_ERROR_MEM_FAULT;
    }

    /* Make a copy of the string */
    *description = UTILS_CStr_dup( archive->description );
    if ( !*description )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
    }

    return OTF2_SUCCESS;
}



/** @brief Get the name of the creator, to determine who has recordet the trace.
 *         This is optional!
 *
 *  @param archive          Archive handle.
 *  @param creator          Place for the name of the author of the trace
 *
 *  @return                 Returns an error code if something was going wrong,
 *                          or there was no name set before.
 */

OTF2_ErrorCode
otf2_archive_get_creator( OTF2_Archive* archive,
                          char**        creator )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( creator );

    /* Check if there is no name set, return an error */
    if ( !archive->creator )
    {
        return OTF2_ERROR_MEM_FAULT;
    }

    /* Make a copy of the string */
    *creator = UTILS_CStr_dup( archive->creator );
    if ( !*creator )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
    }

    return OTF2_SUCCESS;
}



/** @brief Get the number of locations.
 *
 *  @param archive          Archive handle.
 *  @param locationNumber   Number of global definition records.
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_number_of_locations( OTF2_Archive* archive,
                                      uint64_t*     numberOfLocations )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( numberOfLocations );

    /* If in writing mode, get the number from the global definition writer
     * first. */
    if ( NULL != archive->global_def_writer )
    {
        OTF2_ErrorCode status = OTF2_GlobalDefWriter_GetNumberOfLocations(
            archive->global_def_writer,
            &( archive->number_of_locations ) );
        if ( OTF2_SUCCESS != status )
        {
            return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                                "Could not get number of locations!" );
        }
    }

    *numberOfLocations = archive->number_of_locations;

    return OTF2_SUCCESS;
}



/** @brief Get the number of global definition records, which are stored in this trace.
 *         This can be used for progress calculation.
 *
 *  @param archive                 Archive handle.
 *  @param globalDefRecordsNumber  Number of global definition records.
 *
 *  @return                 Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_number_of_global_defs( OTF2_Archive* archive,
                                        uint64_t*     numberOfGlobalDefs )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( numberOfGlobalDefs );

    /* If in writing mode, get the number from the global definition writer
     * first. */
    if ( NULL != archive->global_def_writer )
    {
        OTF2_ErrorCode status = OTF2_GlobalDefWriter_GetNumberOfDefinitions(
            archive->global_def_writer,
            &( archive->number_of_global_defs ) );
        if ( OTF2_SUCCESS != status )
        {
            return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                                "Could not get number of global definitions!" );
        }
    }

    *numberOfGlobalDefs = archive->number_of_global_defs;

    return OTF2_SUCCESS;
}



/** @brief Get the master or slave flag of this archive.
 *         The master or slave flag determines which role the actual archive instance
 *         plays. A master stores the metadata into the anchor file and the global
 *         definition file. Furthermore does the master establish the basic folder
 *         hirarchy.
 *
 *  @param archive        Archive handle.
 *  @param masterOrSlave  Information about the mode of this archive instance
 *
 *  @return               Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_master_slave_mode( OTF2_Archive*         archive,
                                    OTF2_MasterSlaveMode* masterOrSlave )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( masterOrSlave );

    /* Check if the state of this flag has a valid state */
    if ( ( archive->master_or_slave != 0 ) &&
         ( archive->master_or_slave != 1 ) &&
         ( archive->master_or_slave != OTF2_UNDEFINED_TYPE )
         )
    {
        return UTILS_ERROR( OTF2_ERROR_INDEX_OUT_OF_BOUNDS,
                            "State is not valid!" );
    }

    /* Copy the actual state */
    *masterOrSlave = archive->master_or_slave;

    return OTF2_SUCCESS;
}



/** @brief Get the file substrate (posix, sion, none)
 *
 *  @param archive    Archive handle.
 *  @param Substrate  Return pointer to file substrate:
 *                    OTF2_SUBSTRATE_POSIX, OTF2_SUBSTRATE_SION,
 *                    or OTF2_SUBSTRATE_NONE
 *
 *  @return           Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_file_substrate( OTF2_Archive*       archive,
                                 OTF2_FileSubstrate* substrate )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( substrate );

    /* Check if this flag was already set. If not return an error */
    if ( archive->substrate == OTF2_SUBSTRATE_UNDEFINED )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Value is not set!" );
    }

    /* Check if the state of this flag has a valid state */
    if ( !( ( archive->substrate == OTF2_SUBSTRATE_POSIX ) ||
            ( archive->substrate == OTF2_SUBSTRATE_SION ) ||
            ( archive->substrate == OTF2_SUBSTRATE_NONE ) )
         )
    {
        return UTILS_ERROR( OTF2_ERROR_INDEX_OUT_OF_BOUNDS,
                            "State is not valid!" );
    }

    /* Copy the actual state */
    *substrate = archive->substrate;

    return OTF2_SUCCESS;
}



/** @brief Get compression mode (none or zlib)
 *
 *  @param archive      Archive handle.
 *  @param compression  Return pointer to compression mode.
 *
 *  @return             Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_compression( OTF2_Archive*     archive,
                              OTF2_Compression* compression )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( compression );

    /* Check if this flag was already set. If not return an error */
    if ( archive->compression == OTF2_COMPRESSION_UNDEFINED )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Value is not set!" );
    }

    /* Check if the state of this flag has a valid state */
    if ( !( ( archive->compression == OTF2_COMPRESSION_NONE ) ||
            ( archive->compression == OTF2_COMPRESSION_ZLIB ) )
         )
    {
        return UTILS_ERROR( OTF2_ERROR_INDEX_OUT_OF_BOUNDS,
                            "State is not valid!" );
    }

    /* Copy the actual state */
    *compression = archive->compression;

    return OTF2_SUCCESS;
}



/** @brief Get the interaction mode (reading, writing, modification)
 *
 *  @param archive    Archive handle.
 *  @param fileMode   Reading, writing or modification mode?
 *
 *  @return           Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_file_mode( OTF2_Archive*  archive,
                            OTF2_FileMode* fileMode )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( fileMode );

    /* Check if this flag was already set. If not return an error */
    if ( archive->file_mode == OTF2_UNDEFINED_TYPE )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Value is not set!" );
    }

    /* Check if the state of this flag has a valid state */
    if ( !( ( archive->file_mode == OTF2_FILEMODE_READ ) ||
            ( archive->file_mode == OTF2_FILEMODE_WRITE ) ||
            ( archive->file_mode == OTF2_FILEMODE_MODIFY ) )
         )
    {
        return UTILS_ERROR( OTF2_ERROR_INDEX_OUT_OF_BOUNDS,
                            "State is not valid!" );
    }

    /* Copy the actual state */
    *fileMode = archive->file_mode;

    return OTF2_SUCCESS;
}



/** @brief Get the archive path.
 *
 *  @param  archive     Archive handle.
 *  @param  ArchivePath Path to the archive
 *
 *  @return Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_archive_path( OTF2_Archive* archive,
                               char**        archivePath )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( archivePath );

    /* Copy archive name */
    *archivePath = UTILS_CStr_dup( archive->archive_path );
    if ( !*archivePath )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
    }

    return OTF2_SUCCESS;
}



/** @brief Get the path to the trace files (ie. per location files).
 *
 *  @param  archive     Archive handle.
 *  @param  tracePath   Path to the trace files
 *
 *  @return Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_trace_path( OTF2_Archive* archive,
                             char**        tracePath )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( tracePath );

    /* join the archive name and the file path to get the full path */
    *tracePath = UTILS_IO_JoinPath( 2,
                                    archive->archive_path,
                                    archive->archive_name );
    if ( !*tracePath )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
    }

    return OTF2_SUCCESS;
}



/** @brief Get the names of all trace file properties.
 *
 *  @param archive              Archive handle.
 *  @param numberOfProperties   Number of properties.
 *  @param names                Pointer to a string where the property name will be stored, will be malloc'ed.
 *                              Just pass *names to free.
 *
 *  @return                     Returns OTF2_SUCCESS if successful, an error code
 *                              if an error occurs.
 */

OTF2_ErrorCode
otf2_archive_get_property_names( OTF2_Archive* archive,
                                 uint32_t*     numberOfProperties,
                                 char***       names )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( numberOfProperties );
    UTILS_ASSERT( names );

    uint32_t               string_length_sum = 0;
    uint32_t               current_index     = 0;
    otf2_archive_property* property          = archive->properties;

    *numberOfProperties = archive->number_of_properties;
    if ( *numberOfProperties == 0 )
    {
        ( *names ) = NULL;
        return OTF2_SUCCESS;
    }

    while ( property != NULL )
    {
        if ( current_index >= *numberOfProperties )
        {
            return OTF2_ERROR_INDEX_OUT_OF_BOUNDS;
        }

        string_length_sum += strlen( property->name ) + 1;

        property = property->next;
        current_index++;
    }

    /* Array of pointer to immediately following strings */
    char** name_array = malloc( *numberOfProperties * sizeof( char* ) + string_length_sum * sizeof( char ) );
    if ( name_array == NULL )
    {
        return OTF2_ERROR_MEM_FAULT;
    }

    current_index = 0;
    property      = archive->properties;
    uint32_t string_start = *numberOfProperties * sizeof( char* );
    while ( property != NULL )
    {
        name_array[ current_index ] = ( char* )name_array + string_start;
        strcpy( ( char* )name_array + string_start, property->name );
        string_start += strlen( property->name ) + 1;

        property = property->next;
        current_index++;
    }

    *names = name_array;
    return OTF2_SUCCESS;
}



/** @brief Get the value of the named trace file property.
 *
 *  @param archive          Archive handle.
 *  @param name             Name of the property.
 *  @param value            Pointer to a string where the value will be stored, will be malloc'ed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */

OTF2_ErrorCode
otf2_archive_get_property( OTF2_Archive* archive,
                           const char*   name,
                           char**        value )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( name );
    UTILS_ASSERT( value );

    otf2_archive_property* property = archive->properties;
    while ( property != NULL )
    {
        if ( strcmp( property->name, name ) == 0 )
        {
            *value = UTILS_CStr_dup( property->value );
            return OTF2_SUCCESS;
        }

        property = property->next;
    }

    *value = NULL;
    return OTF2_ERROR_PROPERTY_NOT_FOUND;
}



/** @brief Get the identifier of the trace file.
 *
 *  @note This call is only allowed when the archive was opened with mode
 *        OTF2_FILEMODE_READ.
 *
 *  @param archive          Archive handle.
 *  @param id               Trace identifier.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_get_trace_id( OTF2_Archive* archive,
                           uint64_t*     id )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( id );

    *id = archive->trace_id;
    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_get_number_of_snapshots( OTF2_Archive* archive,
                                      uint32_t*     number )
{
    UTILS_ASSERT( archive );
    *number = archive->number_of_snapshots;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_get_number_of_thumbnails( OTF2_Archive* archive,
                                       uint32_t*     numberOfThumbnails )
{
    UTILS_ASSERT( archive );

    *numberOfThumbnails = archive->number_of_thumbnails;

    return OTF2_SUCCESS;
}


/** @brief The get_local_event_writer interface does internally lookup for
 *         already existing event writer instances (local_evt_writers)
 *         or if the requested writer was not requested before, it generates a
 *         new one for the requested location and puts it into the internal
 *         vector.
 *
 *  @param archive          Archive handle.
 *  @param writer           Returned local event writer object.
 *  @param locationID       Location id for which the writer is requested.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */

OTF2_ErrorCode
otf2_archive_get_evt_writer( OTF2_Archive*    archive,
                             OTF2_LocationRef location,
                             OTF2_EvtWriter** writer )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( writer );

    /* Search writer in existing list */
    if ( location != OTF2_UNDEFINED_LOCATION )
    {
        for ( *writer = archive->local_evt_writers;
              *writer;
              *writer = ( *writer )->next )
        {
            if ( ( *writer )->location_id == location )
            {
                return OTF2_SUCCESS;
            }
        }
    }

    /* Create new writer */
    *writer = otf2_evt_writer_new( archive, location );
    if ( !*writer )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create event writer!" );
    }

    /* Put new writer into the list of open writers */
    ( *writer )->next          = archive->local_evt_writers;
    archive->local_evt_writers = *writer;

    return OTF2_SUCCESS;
}


/** @brief  This interface gets the global definition writer, if the archive handle
 *          is in master mode.
 *
 *  @param archive          Archive handle.
 *  @param writer           Returned global definition writer object.
 *  @param preFlush         Function pointer to the users pre flush callback to
 *                          hook in before the flush.
 *  @param postFlush        Function pointer to the users post flush callback
 *                          that provides an actual timestamp.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */

OTF2_ErrorCode
otf2_archive_get_global_def_writer(  OTF2_Archive*          archive,
                                     OTF2_GlobalDefWriter** writer )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( writer );

    if ( archive->master_or_slave != OTF2_MASTER )
    {
        return OTF2_ERROR_INVALID_CALL;
    }

    if ( archive->global_def_writer )
    {
        *writer = archive->global_def_writer;
        return OTF2_SUCCESS;
    }

    archive->global_def_writer =
        otf2_global_def_writer_new( archive );

    if ( !archive->global_def_writer )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create global definition writer!" );
    }

    *writer = archive->global_def_writer;

    return OTF2_SUCCESS;
}


/** @brief The get_local_definition_writer interface does internally lookup for
 *         already existing definition writer instances (local_def_writers)
 *         or if the requested writer was not requested before, it generates a
 *         new one for the requested location and puts it into the internal
 *         vector.
 *
 *  @param archive          Archive handle.
 *  @param writer           Returned local definition writer object.
 *  @param location         Location id for which the writer is requested.
 *  @param preFlush         Function pointer to the users pre flush callback to
 *                          hook in before the flush.
 *  @param postFlush        Function pointer to the users post flush callback
 *                          that provides an actual timestamp.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */

OTF2_ErrorCode
otf2_archive_get_def_writer( OTF2_Archive*    archive,
                             OTF2_LocationRef location,
                             OTF2_DefWriter** writer )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( location != OTF2_UNDEFINED_LOCATION );
    UTILS_ASSERT( writer );

    *writer = NULL;

    /* Search writer in existing list */
    *writer = archive->local_def_writers;
    while ( *writer )
    {
        if ( ( *writer )->location_id == location )
        {
            return OTF2_SUCCESS;
        }
        *writer = ( *writer )->next;
    }

    /* Create new writer */
    *writer = otf2_def_writer_new( archive, location );
    if ( !*writer )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create definition writer!" );
    }

    /* Put new writer into the list of open writers */
    ( *writer )->next          = archive->local_def_writers;
    archive->local_def_writers = *writer;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_get_snap_writer( OTF2_Archive*     archive,
                              OTF2_LocationRef  location,
                              OTF2_SnapWriter** writer )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( writer );

    /* Search writer in existing list */
    if ( location != OTF2_UNDEFINED_LOCATION )
    {
        for ( *writer = archive->local_snap_writers;
              *writer;
              *writer = ( *writer )->next )
        {
            if ( ( *writer )->location_id == location )
            {
                return OTF2_SUCCESS;
            }
        }
    }

    /* Create new writer */
    *writer = otf2_snap_writer_new( archive, location );

    if ( !*writer )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create snap writer!" );
    }

    /* Put new writer into the list of open writers */
    ( *writer )->next           = archive->local_snap_writers;
    archive->local_snap_writers = *writer;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_get_thumb_writer( OTF2_Archive*      archive,
                               OTF2_ThumbWriter** writer,
                               const char*        name,
                               const char*        description,
                               OTF2_ThumbnailType type,
                               uint32_t           numberOfSamples,
                               uint32_t           numberOfMetrics,
                               const uint64_t*    refsToDefs )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( writer );
    UTILS_ASSERT( name );
    UTILS_ASSERT( description );
    UTILS_ASSERT( numberOfSamples > 0 );
    UTILS_ASSERT( numberOfMetrics > 0 );
    UTILS_ASSERT( refsToDefs );

    *writer = NULL;

    /* Create new writer */
    *writer = otf2_thumb_writer_new( archive, archive->number_of_thumbnails );
    if ( !*writer )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create definition writer!" );
    }

    /* write thumb header */
    OTF2_ErrorCode ret;
    ret = otf2_thumb_writer_write_header( *writer,
                                          name,
                                          description,
                                          type,
                                          numberOfSamples,
                                          numberOfMetrics,
                                          refsToDefs );
    if ( ret != OTF2_SUCCESS )
    {
        otf2_thumb_writer_delete( *writer );
        return UTILS_ERROR( ret, "Can't write thumbnail header." );
    }

    /* Put new writer into the list of open writers */
    ( *writer )->next      = archive->thumb_writers;
    archive->thumb_writers = *writer;

    archive->number_of_thumbnails++;

    return OTF2_SUCCESS;
}


/** @brief  This interface gets the marker writer, if the archive handle
 *          is in master mode.
 *
 *  @param archive          Archive handle.
 *  @param writer           Returned marker writer object.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_get_marker_writer( OTF2_Archive*       archive,
                                OTF2_MarkerWriter** writer )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( writer );

    if ( archive->marker_writer )
    {
        *writer = archive->marker_writer;
        return OTF2_SUCCESS;
    }
    else
    {
        archive->marker_writer = otf2_marker_writer_new( archive );
        *writer                = archive->marker_writer;
        if ( archive->marker_writer )
        {
            return OTF2_SUCCESS;
        }
    }

    return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                        "Marker writer creation failed!" );
}


/** @brief The get_local_event_reader interface does internally lookup for
 *         already existing reader instances (local_evt_readers)
 *         or if the requested reader was not requested before, it generates a
 *         new one for the requested location and puts it into the internal
 *         vector.
 *
 *  @param  archive            Archive handle.
 *  @param  reader             Returned local event reader object.
 *  @param  location           Location id for which the reader is requested.
 *
 *  @return Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_evt_reader( OTF2_Archive*    archive,
                             OTF2_LocationRef location,
                             OTF2_EvtReader** reader )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( location != OTF2_UNDEFINED_LOCATION );
    UTILS_ASSERT( reader );

    /* Search reader in existing list */
    *reader = archive->local_evt_readers;
    while ( *reader )
    {
        if ( ( *reader )->location_id == location )
        {
            return OTF2_SUCCESS;
        }
        *reader = ( *reader )->next;
    }

    /* Create new reader */
    *reader = otf2_evt_reader_new( archive, location );
    if ( !*reader )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create event reader!" );
    }

    /* Put new reader into the list of open readers */
    ( *reader )->next          = archive->local_evt_readers;
    archive->local_evt_readers = *reader;
    archive->number_of_evt_readers++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_get_global_evt_reader( OTF2_Archive*          archive,
                                    OTF2_GlobalEvtReader** reader )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( reader );

    if ( archive->global_evt_reader )
    {
        *reader = archive->global_evt_reader;
        return OTF2_SUCCESS;
    }
    else
    {
        archive->global_evt_reader = otf2_global_evt_reader_new( archive );
        *reader                    = archive->global_evt_reader;
        if ( archive->global_evt_reader )
        {
            return OTF2_SUCCESS;
        }
    }

    return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                        "Can't create global event reader!" );
}


/** @brief The get_local_definition_reader interface does internally lookup for
 *         already existing reader instances (local_def_readers)
 *         or if the requested reader was not requested before, it generates a
 *         new one for the requested location and puts it into the internal
 *         vector.
 *
 *  @param  archive            Archive handle.
 *  @param  reader             Returned local definition reader object.
 *  @param  location           Location id for which the reader is requested.
 *
 *  @return Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_def_reader( OTF2_Archive*    archive,
                             OTF2_LocationRef location,
                             OTF2_DefReader** reader )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( location != OTF2_UNDEFINED_LOCATION );
    UTILS_ASSERT( reader );

    /* Search reader in existing list */
    *reader = archive->local_def_readers;
    while ( *reader )
    {
        if ( ( *reader )->location_id == location )
        {
            return OTF2_SUCCESS;
        }
        *reader = ( *reader )->next;
    }

    /* Create new reader */
    *reader = otf2_def_reader_new( archive, location );
    if ( !*reader )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create definition reader!" );
    }

    /* Put new reader into the list of open readers */
    ( *reader )->next          = archive->local_def_readers;
    archive->local_def_readers = *reader;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_get_global_def_reader( OTF2_Archive*          archive,
                                    OTF2_GlobalDefReader** reader )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( reader );

    if ( archive->global_def_reader )
    {
        *reader = archive->global_def_reader;
        return OTF2_SUCCESS;
    }
    else
    {
        archive->global_def_reader = otf2_global_def_reader_new( archive );
        *reader                    = archive->global_def_reader;
        if ( archive->global_def_reader )
        {
            return OTF2_SUCCESS;
        }
    }

    return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                        "Global definition reader creation failed!" );
}


/** @brief The get_local_event_reader interface does internally lookup for
 *         already existing reader instances (local_evt_readers)
 *         or if the requested reader was not requested before, it generates a
 *         new one for the requested location and puts it into the internal
 *         vector.
 *
 *  @param  archive            Archive handle.
 *  @param  reader             Returned local event reader object.
 *  @param  location           Location id for which the reader is requested.
 *
 *  @return Returns an error code if something was going wrong
 */

OTF2_ErrorCode
otf2_archive_get_snap_reader( OTF2_Archive*     archive,
                              OTF2_LocationRef  location,
                              OTF2_SnapReader** reader )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( location != OTF2_UNDEFINED_LOCATION );
    UTILS_ASSERT( reader );

    /* Search reader in existing list */
    *reader = archive->local_snap_readers;
    while ( *reader )
    {
        if ( ( *reader )->location_id == location )
        {
            return OTF2_SUCCESS;
        }
        *reader = ( *reader )->next;
    }

    /* Create new reader */
    *reader = otf2_snap_reader_new( archive, location );
    if ( !*reader )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create snap reader!" );
    }

    /* Put new reader into the list of open readers */
    ( *reader )->next           = archive->local_snap_readers;
    archive->local_snap_readers = *reader;
    archive->number_of_snap_readers++;


    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_get_global_snap_reader( OTF2_Archive*           archive,
                                     OTF2_GlobalSnapReader** reader )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( reader );

    if ( archive->global_snap_reader )
    {
        *reader = archive->global_snap_reader;
        return OTF2_SUCCESS;
    }
    else
    {
        archive->global_snap_reader = otf2_global_snap_reader_new( archive );
        *reader                     = archive->global_snap_reader;
        if ( archive->global_snap_reader )
        {
            return OTF2_SUCCESS;
        }
    }

    return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                        "Can't create global snap reader!" );
}


OTF2_ErrorCode
otf2_archive_get_thumb_reader( OTF2_Archive*      archive,
                               uint32_t           thumbID,
                               OTF2_ThumbReader** reader )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( reader );

    /* Search reader in existing list */
    *reader = archive->thumb_readers;
    while ( *reader )
    {
        if ( ( *reader )->thumb_id == thumbID )
        {
            return OTF2_SUCCESS;
        }
        *reader = ( *reader )->next;
    }

    /* Create new reader */
    *reader = otf2_thumb_reader_new( archive, thumbID );
    if ( !*reader )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Can't create thumb reader!" );
    }

    /* Put new reader into the list of open readers */
    ( *reader )->next      = archive->thumb_readers;
    archive->thumb_readers = *reader;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
otf2_archive_get_marker_reader( OTF2_Archive*       archive,
                                OTF2_MarkerReader** reader )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( reader );

    if ( archive->marker_reader )
    {
        *reader = archive->marker_reader;
        return OTF2_SUCCESS;
    }
    else
    {
        archive->marker_reader = otf2_marker_reader_new( archive );
        *reader                = archive->marker_reader;
        if ( archive->marker_reader )
        {
            return OTF2_SUCCESS;
        }
    }

    return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                        "Marker reader creation failed!" );
}


/* ___ Close functions for local reader and writer. _________________________ */

/** @brief Close an opened local event writer.
 *
 *  @param archive          Archive handle.
 *  @param writer           Writer handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_evt_writer( OTF2_Archive*   archive,
                               OTF2_EvtWriter* writer )
{
    UTILS_ASSERT( archive );

    if ( writer == NULL )
    {
        return OTF2_SUCCESS;
    }

    /* Search the writer in the list of open writers. */
    OTF2_EvtWriter** writer_it;
    for ( writer_it = &archive->local_evt_writers;
          *writer_it;
          writer_it = &( *writer_it )->next )
    {
        if ( *writer_it == writer )
        {
            break;
        }
    }
    if ( !*writer_it )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Can't find event writer." );
    }
    /* Remove the writer from the list */
    *writer_it = writer->next;

    /* Close event writer. */
    otf2_evt_writer_delete( writer );

    return OTF2_SUCCESS;
}


/** @brief Close an opened local definition writer.
 *
 *  @param archive          Archive handle.
 *  @param writer           Writer handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_def_writer( OTF2_Archive*   archive,
                               OTF2_DefWriter* writer )
{
    UTILS_ASSERT( archive );

    if ( writer == NULL )
    {
        return OTF2_SUCCESS;
    }

    /* Search the writer in the list of open writers. */
    OTF2_DefWriter** writer_it;
    for ( writer_it = &archive->local_def_writers;
          *writer_it;
          writer_it = &( *writer_it )->next )
    {
        if ( *writer_it == writer )
        {
            break;
        }
    }
    if ( !*writer_it )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Can't find definition writer." );
    }
    /* Remove the writer from the list */
    *writer_it = writer->next;

    /* Close definition writer. */
    otf2_def_writer_delete( writer );

    return OTF2_SUCCESS;
}


/** @brief Close an opened local snap writer.
 *
 *  @param archive          Archive handle.
 *  @param writer           Writer handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_snap_writer( OTF2_Archive*    archive,
                                OTF2_SnapWriter* writer )
{
    UTILS_ASSERT( archive );

    if ( writer == NULL )
    {
        return OTF2_SUCCESS;
    }

    /* Search the writer in the list of open writers. */
    OTF2_SnapWriter** writer_it;
    for ( writer_it = &archive->local_snap_writers;
          *writer_it;
          writer_it = &( *writer_it )->next )
    {
        if ( *writer_it == writer )
        {
            break;
        }
    }
    if ( !*writer_it )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Can't find snap writer." );
    }
    /* Remove the writer from the list */
    *writer_it = writer->next;

    /* Close event writer. */
    otf2_snap_writer_delete( writer );

    return OTF2_SUCCESS;
}


/** @brief Closes the marker writer.
 *
 *  @param archive          Archive handle.
 *  @param markerWriter     Writer handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_marker_writer( OTF2_Archive*      archive,
                                  OTF2_MarkerWriter* markerWriter )
{
    UTILS_ASSERT( archive );

    if ( markerWriter == NULL )
    {
        return OTF2_SUCCESS;
    }

    if ( archive->marker_writer != markerWriter )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Marker writer does not match with that of the archive." );
    }
    archive->marker_writer = NULL;

    /* Close global definition reader object. */
    return otf2_marker_writer_delete( markerWriter );
}


/** @brief Close an opened local event reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_evt_reader( OTF2_Archive*   archive,
                               OTF2_EvtReader* reader )
{
    UTILS_ASSERT( archive );

    if ( reader == NULL )
    {
        return OTF2_SUCCESS;
    }

    /* Search the reader in the list of open readers. */
    OTF2_EvtReader** reader_it;
    for ( reader_it = &archive->local_evt_readers;
          *reader_it;
          reader_it = &( *reader_it )->next )
    {
        if ( *reader_it == reader )
        {
            break;
        }
    }
    if ( !*reader_it )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Can't find event reader." );
    }
    /* Remove the reader from the list */
    *reader_it = reader->next;
    archive->number_of_evt_readers--;

    /* Close event reader. */
    otf2_evt_reader_delete( reader );

    return OTF2_SUCCESS;
}


/** @brief Closes the global event reader.
 *
 *  @param archive          Archive handle.
 *  @param globalEvtReader  Reader handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_global_evt_reader( OTF2_Archive*         archive,
                                      OTF2_GlobalEvtReader* globalEvtReader )
{
    UTILS_ASSERT( archive );

    if ( globalEvtReader == NULL )
    {
        return OTF2_SUCCESS;
    }

    if ( archive->global_evt_reader != globalEvtReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Global event reader does not match with that of "
                            "the archive." );
    }
    archive->global_evt_reader = NULL;

    /* Close global definition reader object. */
    return otf2_global_evt_reader_delete( globalEvtReader );
}


/** @brief Close an opened local definition reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_def_reader( OTF2_Archive*   archive,
                               OTF2_DefReader* reader )
{
    UTILS_ASSERT( archive );

    if ( reader == NULL )
    {
        return OTF2_SUCCESS;
    }

    /* Search the reader in the list of open readers. */
    OTF2_DefReader** reader_it;
    for ( reader_it = &archive->local_def_readers;
          *reader_it;
          reader_it = &( *reader_it )->next )
    {
        if ( *reader_it == reader )
        {
            break;
        }
    }
    if ( !*reader_it )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Can't find definition reader." );
    }
    /* Remove the reader from the list */
    *reader_it = reader->next;

    /* Close definition reader. */
    otf2_def_reader_delete( reader );

    return OTF2_SUCCESS;
}


/** @brief Closes the global definition reader.
 *
 *  @param archive          Archive handle.
 *  @param globalDefReader  Reader handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_global_def_reader( OTF2_Archive*         archive,
                                      OTF2_GlobalDefReader* globalDefReader )
{
    UTILS_ASSERT( archive );

    if ( globalDefReader == NULL )
    {
        return OTF2_SUCCESS;
    }

    if ( archive->global_def_reader != globalDefReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Global definition reader does not match with that "
                            "of the archive." );
    }
    archive->global_def_reader = NULL;

    /* Close global definition reader object. */
    return otf2_global_def_reader_delete( globalDefReader );
}


/** @brief Close an opened local snap reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_snap_reader( OTF2_Archive*    archive,
                                OTF2_SnapReader* reader )
{
    UTILS_ASSERT( archive );

    if ( reader == NULL )
    {
        return OTF2_SUCCESS;
    }

    /* Search the reader in the list of open readers. */
    OTF2_SnapReader** reader_it;
    for ( reader_it = &archive->local_snap_readers;
          *reader_it;
          reader_it = &( *reader_it )->next )
    {
        if ( *reader_it == reader )
        {
            break;
        }
    }
    if ( !*reader_it )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Can't find event reader." );
    }
    /* Remove the reader from the list */
    *reader_it = reader->next;
    archive->number_of_snap_readers--;

    /* Close event reader. */
    otf2_snap_reader_delete( reader );

    return OTF2_SUCCESS;
}


/** @brief Closes the global snapshot reader.
 *
 *  @param archive          Archive handle.
 *  @param globalSnapReader Reader handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_global_snap_reader( OTF2_Archive*          archive,
                                       OTF2_GlobalSnapReader* globalSnapReader )
{
    UTILS_ASSERT( archive );

    if ( globalSnapReader == NULL )
    {
        return OTF2_SUCCESS;
    }

    if ( archive->global_snap_reader != globalSnapReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Global snapshot reader does not match with that of "
                            "the archive." );
    }
    archive->global_snap_reader = NULL;

    /* Close global definition reader object. */
    return otf2_global_snap_reader_delete( globalSnapReader );
}


/** @brief Close an opened thumb reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_thumb_reader( OTF2_Archive*     archive,
                                 OTF2_ThumbReader* reader )
{
    UTILS_ASSERT( archive );

    if ( reader == NULL )
    {
        return OTF2_SUCCESS;
    }

    /* Search the reader in the list of open readers. */
    OTF2_ThumbReader** reader_it;
    for ( reader_it = &archive->thumb_readers;
          *reader_it;
          reader_it = &( *reader_it )->next )
    {
        if ( *reader_it == reader )
        {
            break;
        }
    }
    if ( !*reader_it )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Can't find thumbnail reader." );
    }
    /* Remove the reader from the list */
    *reader_it = reader->next;

    /* Close thumb reader. */
    otf2_thumb_reader_delete( reader );

    return OTF2_SUCCESS;
}


/** @brief Closes the marker reader.
 *
 *  @param archive          Archive handle.
 *  @param markerReader     Reader handle to be closed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_archive_close_marker_reader( OTF2_Archive*      archive,
                                  OTF2_MarkerReader* markerReader )
{
    UTILS_ASSERT( archive );

    if ( markerReader == NULL )
    {
        return OTF2_SUCCESS;
    }

    if ( archive->marker_reader != markerReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Marker reader does not match with that of the archive." );
    }
    archive->marker_reader = NULL;

    /* Close global definition reader object. */
    return otf2_marker_reader_delete( markerReader );
}


/* ___ Register function pointers. __________________________________________ */

static const otf2_reader_archive_impl otf2_archive_reader_impl =
{
    .delete_archive                 = ( OTF2_FP_DeleteArchive               )OTF2_Archive_Close,
    .set_file_sion_callbacks        = ( OTF2_FP_SetFileSionCallbacks        )OTF2_Archive_SetFileSionCallbacks,
    .register_evt_callbacks         = ( OTF2_FP_RegisterEvtCallbacks        )OTF2_EvtReader_SetCallbacks,
    .register_global_evt_callbacks  = ( OTF2_FP_RegisterGlobalEvtCallbacks  )OTF2_GlobalEvtReader_SetCallbacks,
    .register_def_callbacks         = ( OTF2_FP_RegisterDefCallbacks        )OTF2_DefReader_SetCallbacks,
    .register_global_def_callbacks  = ( OTF2_FP_RegisterGlobalDefCallbacks  )OTF2_GlobalDefReader_SetCallbacks,
    .register_snap_callbacks        = ( OTF2_FP_RegisterSnapCallbacks       )OTF2_SnapReader_SetCallbacks,
    .register_global_snap_callbacks = ( OTF2_FP_RegisterGlobalSnapCallbacks )OTF2_GlobalSnapReader_SetCallbacks,
    .register_marker_callbacks      = ( OTF2_FP_RegisterMarkerCallbacks    )OTF2_MarkerReader_SetCallbacks,
    .read_local_events              = ( OTF2_FP_ReadLocalEvents             )OTF2_EvtReader_ReadEvents,
    .read_global_event              = ( OTF2_FP_ReadGlobalEvent             )OTF2_GlobalEvtReader_ReadEvent,
    .has_global_event               = ( OTF2_FP_HasGlobalEvent              )OTF2_GlobalEvtReader_HasEvent,
    .read_global_events             = ( OTF2_FP_ReadGlobalEvents            )OTF2_GlobalEvtReader_ReadEvents,
    .read_local_events_backward     = ( OTF2_FP_ReadLocalEvents             )OTF2_EvtReader_ReadEventsBackward,
    .read_local_definitions         = ( OTF2_FP_ReadLocalDefinitions        )OTF2_DefReader_ReadDefinitions,
    .read_global_definitions        = ( OTF2_FP_ReadGlobalDefinitions       )OTF2_GlobalDefReader_ReadDefinitions,
    .read_local_snapshots           = ( OTF2_FP_ReadLocalSnapshots          )OTF2_SnapReader_ReadSnapshots,
    .read_global_snapshots          = ( OTF2_FP_ReadGlobalSnapshots         )OTF2_GlobalSnapReader_ReadSnapshots,
    .read_markers                   = ( OTF2_FP_ReadMarkers                )OTF2_MarkerReader_ReadMarkers,
    .get_evt_reader                 = ( OTF2_FP_GetEvtReader                )OTF2_Archive_GetEvtReader,
    .get_snap_reader                = ( OTF2_FP_GetSnapReader               )OTF2_Archive_GetSnapReader,
    .get_def_reader                 = ( OTF2_FP_GetDefReader                )OTF2_Archive_GetDefReader,
    .get_global_evt_reader          = ( OTF2_FP_GetGlobalEvtReader          )OTF2_Archive_GetGlobalEvtReader,
    .get_global_snap_reader         = ( OTF2_FP_GetGlobalSnapReader         )OTF2_Archive_GetGlobalSnapReader,
    .get_global_def_reader          = ( OTF2_FP_GetGlobalDefReader          )OTF2_Archive_GetGlobalDefReader,
    .get_thumb_reader               = ( OTF2_FP_GetThumbReader              )OTF2_Archive_GetThumbReader,
    .get_marker_reader              = ( OTF2_FP_GetMarkerReader            )OTF2_Archive_GetMarkerReader,
    .get_marker_writer              = ( OTF2_FP_GetMarkerWriter            )OTF2_Archive_GetMarkerWriter,
    .get_version                    = ( OTF2_FP_GetVersion                  )OTF2_Archive_GetVersion,
    .get_chunk_size                 = ( OTF2_FP_GetChunkSize                )OTF2_Archive_GetChunkSize,
    .get_file_substrate             = ( OTF2_FP_GetFileSubstrate            )OTF2_Archive_GetFileSubstrate,
    .get_compression                = ( OTF2_FP_GetCompression              )OTF2_Archive_GetCompression,
    .get_number_of_locations        = ( OTF2_FP_GetNumLocations             )OTF2_Archive_GetNumberOfLocations,
    .get_number_of_global_defs      = ( OTF2_FP_GetNumGlobalDefs            )OTF2_Archive_GetNumberOfGlobalDefinitions,
    .get_machine_name               = ( OTF2_FP_GetMachineName              )OTF2_Archive_GetMachineName,
    .get_creator                    = ( OTF2_FP_GetCreator                  )OTF2_Archive_GetCreator,
    .get_description                = ( OTF2_FP_GetDescription              )OTF2_Archive_GetDescription,
    .get_property_names             = ( OTF2_FP_GetPropertyNames            )OTF2_Archive_GetPropertyNames,
    .get_property                   = ( OTF2_FP_GetProperty                 )OTF2_Archive_GetProperty,
    .get_trace_id                   = ( OTF2_FP_GetTraceId                  )OTF2_Archive_GetTraceId,
    .get_number_of_snapshots        = ( OTF2_FP_GetNumberOfSnapshots        )OTF2_Archive_GetNumberOfSnapshots,
    .get_number_of_thumbnails       = ( OTF2_FP_GetNumberOfThumbnails       )OTF2_Archive_GetNumberOfThumbnails,
    .close_evt_reader               = ( OTF2_FP_CloseEvtReader              )OTF2_Archive_CloseEvtReader,
    .close_global_evt_reader        = ( OTF2_FP_CloseGlobalEvtReader        )OTF2_Archive_CloseGlobalEvtReader,
    .close_def_reader               = ( OTF2_FP_CloseDefReader              )OTF2_Archive_CloseDefReader,
    .close_global_def_reader        = ( OTF2_FP_CloseGlobalDefReader        )OTF2_Archive_CloseGlobalDefReader,
    .close_snap_reader              = ( OTF2_FP_CloseSnapReader             )OTF2_Archive_CloseSnapReader,
    .close_global_snap_reader       = ( OTF2_FP_CloseGlobalSnapReader       )OTF2_Archive_CloseGlobalSnapReader,
    .close_thumb_reader             = ( OTF2_FP_CloseThumbReader            )OTF2_Archive_CloseThumbReader,
    .close_marker_reader            = ( OTF2_FP_CloseMarkerReader          )OTF2_Archive_CloseMarkerReader,
    .close_marker_writer            = ( OTF2_FP_CloseMarkerWriter          )OTF2_Archive_CloseMarkerWriter
};


const otf2_reader_archive_impl*
otf2_archive_get_reader_impl()
{
    return &otf2_archive_reader_impl;
}

/* ___ Static helper callbacks. __________________________________________ */

static char*
otf2_archive_get_file_name( const char*   base,
                            OTF2_FileType fileType )
{
    const char* suffix = "";
    switch ( fileType )
    {
        case OTF2_FILETYPE_ANCHOR:
            suffix = "otf2";
            break;

        case OTF2_FILETYPE_GLOBAL_DEFS:
            suffix = "def";
            break;

        case OTF2_FILETYPE_LOCAL_DEFS:
            suffix = "def";
            break;

        case OTF2_FILETYPE_EVENTS:
            suffix = "evt";
            break;

        case OTF2_FILETYPE_SNAPSHOTS:
            suffix = "snap";
            break;

        case OTF2_FILETYPE_THUMBNAIL:
            suffix = "thumb";
            break;

        case OTF2_FILETYPE_MARKER:
            suffix = "marker";
            break;
    }

    char* file_name = malloc( strlen( base ) + 1 + strlen( suffix ) + 1 );
    if ( !file_name )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
        return NULL;
    }

    /* Build-up the name */
    sprintf( file_name, "%s.%s", base, suffix );

    return file_name;
}

char*
otf2_archive_get_file_path( OTF2_Archive*    archive,
                            OTF2_FileType    fileType,
                            OTF2_LocationRef location )
{
    UTILS_ASSERT( archive );

    char  location_name[ 32 ];
    char* name_prefix = NULL;

    /* pre-location files have the location as name_prefix */
    if ( otf2_file_type_needs_location_id( fileType ) ||
         fileType == OTF2_FILETYPE_THUMBNAIL )
    {
        UTILS_ASSERT( OTF2_UNDEFINED_LOCATION != location );

        sprintf( location_name, "%" PRIu64, location );
        name_prefix = location_name;
    }
    else
    {
        UTILS_ASSERT( OTF2_UNDEFINED_LOCATION == location );
    }

    return otf2_archive_get_file_path_with_name_prefix( archive,
                                                        fileType,
                                                        name_prefix );
}

char*
otf2_archive_get_file_path_with_name_prefix( OTF2_Archive* archive,
                                             OTF2_FileType fileType,
                                             const char*   name_prefix )
{
    UTILS_ASSERT( archive );
    //UTILS_ASSERT( ( otf2_file_type_needs_location_id( fileType ) && name_prefix )
    //              || ( !otf2_file_type_needs_location_id( fileType ) && !name_prefix ) );

    char* path_component_1;
    char* path_component_2;
    char* to_free = NULL;

    if ( otf2_file_type_needs_location_id( fileType ) )
    {
        /* per-location files reside in the archive_name directory */

        path_component_1 = archive->archive_name;
        path_component_2 = otf2_archive_get_file_name( name_prefix,
                                                       fileType );
        if ( !path_component_2 )
        {
            UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
            return NULL;
        }
        to_free = path_component_2;
    }
    else if ( fileType == OTF2_FILETYPE_THUMBNAIL )
    {
        char* thumb_name_base = malloc( strlen( archive->archive_name ) + 1
                                        + strlen( name_prefix ) + 1 );

        sprintf( thumb_name_base, "%s.%s", archive->archive_name, name_prefix );

        path_component_1 = otf2_archive_get_file_name( thumb_name_base,
                                                       fileType );
        free( thumb_name_base );
        path_component_2 = "";
    }
    else
    {
        /*
         * global files have the archive_name as prefix and get an appropriate
         * suffix.
         */

        path_component_1 = otf2_archive_get_file_name( archive->archive_name,
                                                       fileType );
        if ( !path_component_1 )
        {
            UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
            return NULL;
        }
        to_free = path_component_1;

        /*
         * UTILS_IO_JoinPath ignores "" path components
         */
        path_component_2 = "";
    }

    char* file_path = UTILS_IO_JoinPath( 3,
                                         archive->archive_path,
                                         path_component_1,
                                         path_component_2 );
    free( to_free );
    if ( !file_path )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT, "Allocation failed!" );
        return NULL;
    }

    return file_path;
}
