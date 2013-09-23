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

/**
 * @file       src/OTF2_GlobalDefWriter.c
 * @status     alpha
 *
 * @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 * @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *             Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 * @brief
 * This layer always writes globally defined OTF2 definition records and
 * is used to write either the global definitions in addition to local
 * definitions or write all definitions as globally valid in combination
 * with OTF2_GlobalEventWriter. Global definitions are stored in one global
 * definition file, which makes it nearly impossible to write them in a
 * distributed manner. It is therefore only allowed to get such a writer
 * from an OTF2_ArchiveHandler  which is marked as OTF2_MASTER.
 *
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

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

#include "otf2_attic_types.h"
#include "otf2_attic.h"

#include "otf2_global_definitions.h"

#include "otf2_events_int.h"

#include "OTF2_File.h"
#include "OTF2_Buffer.h"

#include "otf2_global_def_writer.h"

/** @brief Keeps all necessary information a global definition writer. */
struct OTF2_GlobalDefWriter_struct
{
    /** Archive handle */
    OTF2_Archive* archive;
    /** Reference to a buffer where the global definition are stored. */
    OTF2_Buffer*  buffer;
    /** Counter for the number of global definitions. */
    uint64_t      number_of_global_defs;
    /** Counter for the number of locations. */
    uint64_t      number_of_locations;
};


static inline void
otf2_global_def_region_provide_pre_1_1( OTF2_RegionType* regionType,
                                        OTF2_StringRef   name,
                                        OTF2_StringRef   canonicalName,
                                        OTF2_StringRef   description,
                                        OTF2_RegionRole  regionRole,
                                        OTF2_Paradigm    paradigm,
                                        OTF2_RegionFlag  regionFlags,
                                        OTF2_StringRef   sourceFile,
                                        uint32_t         beginLineNumber,
                                        uint32_t         endLineNumber )
{
    *regionType = otf2_attic_def_region_provide_region_type( regionRole,
                                                             paradigm,
                                                             regionFlags );
}


static inline void
otf2_global_def_group_provide_pre_1_2( OTF2_GroupTypePre12* groupTypePre12,
                                       OTF2_StringRef       name,
                                       OTF2_GroupType       groupType,
                                       OTF2_Paradigm        paradigm,
                                       OTF2_GroupFlag       groupFlags,
                                       uint32_t             numberOfMembers,
                                       const uint64_t*      members )
{
    *groupTypePre12
        = otf2_attic_def_group_provide_group_type_pre_1_2( groupType,
                                                           paradigm,
                                                           groupFlags );
}


/* This _must_ be done for the generated part of the OTF2 local writer. */
#include "OTF2_GlobalDefWriter_inc.c"
/* This _must_ be done for the generated part of the OTF2 local writer. */


/** @brief
 *  Creates a new global definition writer instance.
 *
 *  @param archivePath  Path to the archive on disk (path to a folder).
 *  @param archiveName  Name of the archive, to make more different
 *                      archives in one directory possible.
 *  @param substrate    The substrate which should be used to write
 *                      the archive to disk.
 *  @param preFlush     Preflush callback, which is called internally
 *                      if the buffer is full.
 *  @param postFlush    Posflush callback, which is called internally
 *                      if the buffer is full.
 *
 *  @return Returns a handle to the global definition writer if successful,
 *          NULL otherwise.
 */
OTF2_GlobalDefWriter*
otf2_global_def_writer_new( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );

    OTF2_GlobalDefWriter* writer = ( OTF2_GlobalDefWriter* )calloc( 1, sizeof( OTF2_GlobalDefWriter ) );
    if ( !writer )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for writer handle!" );
        return NULL;
    }

    writer->archive               = archive;
    writer->number_of_global_defs = 0;
    writer->number_of_locations   = 0;

    uint64_t       evt_chunk_size;
    uint64_t       def_chunk_size;
    OTF2_ErrorCode ret = otf2_archive_get_chunksize( archive,
                                                     &evt_chunk_size,
                                                     &def_chunk_size );
    if ( ret != OTF2_SUCCESS )
    {
        free( writer );
        return NULL;
    }

    writer->buffer = OTF2_Buffer_New( archive,
                                      writer,
                                      def_chunk_size,
                                      OTF2_BUFFER_WRITE,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_GLOBAL_DEFS,
                                      OTF2_UNDEFINED_LOCATION );

    if ( !writer->buffer )
    {
        UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                     "Creation of buffer handle failed!" );
        free( writer );
        return NULL;
    }

    return writer;
}


/** @brief
 *  Releases the resources associated with the given global definition writer
 *  handle. Does nothing if a NULL pointer is provided.
 *
 *  @param writerHandle  Writer object which has to be deleted.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_global_def_writer_delete( OTF2_GlobalDefWriter* writerHandle )
{
    if ( !writerHandle )
    {
        return OTF2_SUCCESS;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_BUFFER_END_OF_FILE );

    OTF2_ErrorCode ret = OTF2_Buffer_Delete( writerHandle->buffer );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Buffer deletion failed!" );
    }

    free( writerHandle );

    return OTF2_SUCCESS;
}


/** @brief
 *  Releases the resources associated with the given global definition writer
 *  handle. Does nothing if a NULL pointer is provided.
 *
 *  @param writerHandle  Writer object which has to be deleted.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_GetNumberOfDefinitions( OTF2_GlobalDefWriter* writerHandle,
                                             uint64_t*             numberOfDefinitions )
{
    if ( NULL == writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid global definition writer handle!" );
    }

    *numberOfDefinitions = writerHandle->number_of_global_defs;

    return OTF2_SUCCESS;
}


/** @brief
 *  Returns the number of global location definitions written so far.
 *
 *  @param writerHandle      Writer object which has to be deleted.
 *  @param numberOfLocations Pointer where to store the number of locations.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
OTF2_GlobalDefWriter_GetNumberOfLocations( OTF2_GlobalDefWriter* writerHandle,
                                           uint64_t*             numberOfLocations )
{
    if ( NULL == writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid global definition writer handle!" );
    }

    *numberOfLocations = writerHandle->number_of_locations;

    return OTF2_SUCCESS;
}
