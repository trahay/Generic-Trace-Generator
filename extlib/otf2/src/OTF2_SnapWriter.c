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
 * @file        src/OTF2_SnapWriter.c
 * @status      beta
 *
 * @brief       This layer provides write routines to store event records of a
 *              single location to an OTF2 memory buffer.
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

#include "otf2_snaps.h"

#include "otf2_attribute_list.h"

#include "OTF2_File.h"
#include "OTF2_Buffer.h"

#include "otf2_snap_writer.h"

static inline
OTF2_ErrorCode
otf2_snap_writer_write_attribute_list( const OTF2_SnapWriter* writerHandle,
                                       OTF2_AttributeList*    attributeList );


/* This _must_ be done for the generated part of the OTF2 local writer */
#include "OTF2_SnapWriter_inc.c"
/* This _must_ be done for the generated part of the OTF2 local writer */


/** @brief Create a new local event writer.
 *
 *  @param location         The Location ID is needed to name the trace file.
 *  @param archivePath      Path to the archive i.e. the directory where the
 *                          anchor file is located.
 *  @param archiveName      Name of the archive. It is used to generate sub
 *                          pathes e.g. 'archiveName.otf2'.
 *  @param substrate        Defines which substrate is used to write data into
 *                          a file.
 *                          Available values are: 'OTF2_SUBSTRATE_UNDEFINED',
 *                          'OTF2_SUBSTRATE_POSIX', 'OTF2_SUBSTRATE_ZIP',
 *                          'OTF2_SUBSTRATE_SION' and 'OTF2_SUBSTRATE_NONE'.
 *  @param chunksize        Size of the chunks which are used internally.
 *  @param preFlush         Preflush callback, which is called internally
 *                          if the buffer is full. Look into OTF2_Callbacks.h
 *                          for the function pointer definition.
 *  @param postFlush        Posflush callback, which is called internally
 *                          if the buffer is full. Look into OTF2_Callbacks.h
 *                          for the function pointer definition.
 *  @param allocate         Allocate function for a user's own memory management.
 *                          You also have to pass an according free function.
 *                          To use OTF2's internal memory management just pass
 *                          NULL.
 *  @param free             Free function for a user's own memory management.
 *                          You also have to pass an according allocate function.
 *                          To use OTF2's internal memory management just pass
 *                          NULL.
 *  @param allocatorData    User data for a user's own memory management.
 *                          You also have to pass an according allocate and free
 *                          function.
 *                          To use OTF2's internal memory management just pass
 *                          NULL.
 *
 *  @return                 Returns a handle to the event writer if successful,
 *                          NULL otherwise.
 */

OTF2_SnapWriter*
otf2_snap_writer_new
(
    OTF2_Archive*    archive,
    OTF2_LocationRef location
)
{
    UTILS_ASSERT( archive );

    OTF2_SnapWriter* writer = NULL;
    OTF2_ErrorCode   status;

    /* Allocate new writer */
    writer = ( OTF2_SnapWriter* )calloc( 1, sizeof( OTF2_SnapWriter ) );
    if ( !writer )
    {
        goto clean_mem_new;
    }

    uint64_t evt_chunk_size;
    uint64_t def_chunk_size;
    status = otf2_archive_get_chunksize( archive,
                                         &evt_chunk_size,
                                         &def_chunk_size );
    if ( status != OTF2_SUCCESS )
    {
        goto clean_mem_new;
    }

    /* Allocate the buffer module */
    writer->buffer = OTF2_Buffer_New( archive,
                                      writer,
                                      evt_chunk_size,
                                      OTF2_BUFFER_WRITE,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_SNAPSHOTS,
                                      location );


    if ( !writer->buffer )
    {
        goto clean_mem_new;
    }

    /* Initialize the variables */
    writer->archive     = archive;
    writer->location_id = location;

    return writer;

clean_mem_new:

    free( writer );

    return NULL;
}



/** @brief
 *  Releases the resources associated with the given local event writer handle.
 *  Does nothing if a NULL pointer is provided.
 *
 *  @param writer   Writer object which has to be deleted.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */

OTF2_ErrorCode
otf2_snap_writer_delete( OTF2_SnapWriter* writer )
{
    if ( !writer )
    {
        return OTF2_SUCCESS;
    }

    if ( writer->location_id == OTF2_UNDEFINED_LOCATION )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "No valid valid location ID in deletion!\n" );
    }

    OTF2_ErrorCode ret;
    OTF2_Buffer_WriteUint8( writer->buffer, OTF2_BUFFER_END_OF_FILE );

    ret = OTF2_Buffer_Delete( writer->buffer );
    if ( ret != OTF2_SUCCESS )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Buffer deletion failed!" );
    }

    free( writer );

    return OTF2_SUCCESS;
}

OTF2_ErrorCode
OTF2_SnapWriter_GetLocationID( const OTF2_SnapWriter* writer,
                               OTF2_LocationRef*      location )
{
    if ( !writer )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "Writer Object is not valid!" );
    }

    *location = writer->location_id;
    return OTF2_SUCCESS;
}
