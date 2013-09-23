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

/** @file       src/OTF2_GlobalSnapReader.c
 *
 *  @brief
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

#include "otf2_snaps.h"

#include "otf2_attribute_list.h"

#include "OTF2_File.h"
#include "OTF2_Buffer.h"

#include "otf2_snap_reader.h"

#include "otf2_global_snap_reader.h"


/* ___ Global snapshot reader structure. ____________________________________ */


static inline
OTF2_ErrorCode
otf2_global_snap_reader_trigger_unknown( const OTF2_GlobalSnapReader* reader,
                                         OTF2_SnapReader*             snap_reader );


/* This _must_ be done for the generated part of the OTF2 local reader */
#include "OTF2_GlobalSnapReader_inc.c"
/* This _must_ be done for the generated part of the OTF2 local reader */


/* ___ Prototypes for internal functions. ___________________________________ */


static inline bool
compare_snap_reader_lt( OTF2_SnapReader* snap_reader_i,
                        OTF2_SnapReader* snap_reader_j );


static void
percolate_down( OTF2_GlobalSnapReader* reader,
                uint64_t               node );


/* ___ Implementation. ______________________________________________________ */


/** @brief Create new global snapshot reader.
 *
 *  @return                 Returns a handle to the global snapshot reader if
 *                          successful, NULL otherwise.
 */
OTF2_GlobalSnapReader*
otf2_global_snap_reader_new( OTF2_Archive* archive )
{
    UTILS_ASSERT( archive );
    UTILS_ASSERT( archive->local_snap_readers );

    /* Create reader. */
    OTF2_GlobalSnapReader* reader = NULL;
    reader = ( OTF2_GlobalSnapReader* )calloc( 1, sizeof( OTF2_GlobalSnapReader )
                                               + ( archive->number_of_snap_readers * sizeof( OTF2_SnapReader* ) ) );
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not allocate memory for reader handle!" );
        return NULL;
    }

    /* Initialize reader. */
    reader->archive = archive;
    memset( &( reader->reader_callbacks ), '\0', sizeof( OTF2_GlobalSnapReaderCallbacks ) );
    reader->user_data = NULL;

    reader->number_of_snap_readers = 0;
    /* Read one record from each local reader and put it in the record queue. */
    for ( OTF2_SnapReader* snap_reader = archive->local_snap_readers;
          snap_reader;
          snap_reader = snap_reader->next )
    {
        /* Tell the local snapshot reader that he is operated by a global
         * snapshot reader and therefore not stand-alone. This is important
         * for the attribute lists. */
        otf2_snap_reader_operated_by_global_reader( snap_reader );

        OTF2_ErrorCode status = otf2_snap_reader_read( snap_reader );
        if ( status != OTF2_SUCCESS )
        {
            /* If there are no snapshot records in the local file, continue with
             * next local snapshot reader. */
            if ( status == OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
            {
                continue;
            }

            UTILS_ERROR( status, "Could not read snapshot record from location!" );
            otf2_global_snap_reader_delete( reader );
            return NULL;
        }

        reader->snap_queue[ reader->number_of_snap_readers++ ] = snap_reader;
    }

    /* build-up the heap */
    for ( uint64_t i = reader->number_of_snap_readers; i--; )
    {
        percolate_down( reader, i );
    }

    return reader;
}


/** @brief Triggers the callback for an unknown snapshot record.
 *
 *  @param reader      Reference to the reader object
 *  @param snap_reader Reference to the snapshot reader object
 *
 *  @return OTF2_ErrorCode, which is OTF2_SUCCESS if everything is fine.
 */
static inline
OTF2_ErrorCode
otf2_global_snap_reader_trigger_unknown( const OTF2_GlobalSnapReader* reader,
                                         OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap = &snap_reader->current_snap;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.unknown )
    {
        interrupt = reader->reader_callbacks.unknown( snap_reader->location_id,
                                                      snap->record.time,
                                                      reader->user_data,
                                                      &snap_reader->attribute_list );
    }

    /* Before the next record is read, the attribute list has to be cleaned up
     * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


/** @brief Delete a global snapshot reader.
 *
 *  Releases all allocated resources. Does nothing if NULL is provided.
 *
 *  @param readerHandle     Global snapshot reader handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
otf2_global_snap_reader_delete( OTF2_GlobalSnapReader* readerHandle )
{
    /* Validate argument. */
    if ( NULL == readerHandle )
    {
        return OTF2_SUCCESS;
    }

    while ( readerHandle->number_of_snap_readers-- )
    {
        OTF2_SnapReader* snap_reader = readerHandle->snap_queue[ readerHandle->number_of_snap_readers ];
        otf2_archive_close_snap_reader( readerHandle->archive, snap_reader );
    }

    free( readerHandle );

    return OTF2_SUCCESS;
}


OTF2_GlobalSnapReaderCallbacks*
OTF2_GlobalSnapReaderCallbacks_New( void )
{
    return calloc( 1, sizeof( OTF2_GlobalSnapReaderCallbacks ) );
}


void
OTF2_GlobalSnapReaderCallbacks_Delete( OTF2_GlobalSnapReaderCallbacks* GlobalSnapReaderCallbacks )
{
    free( GlobalSnapReaderCallbacks );
}


void
OTF2_GlobalSnapReaderCallbacks_Clear( OTF2_GlobalSnapReaderCallbacks* GlobalSnapReaderCallbacks )
{
    if ( GlobalSnapReaderCallbacks != NULL )
    {
        memset( GlobalSnapReaderCallbacks, 0, sizeof( OTF2_GlobalSnapReaderCallbacks ) );
    }
}


OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_SetUnknownCallback(
    OTF2_GlobalSnapReaderCallbacks*       GlobalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_Unknown unknownCallback )
{
    if ( !GlobalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid GlobalSnapReaderCallback argument!" );
    }

    GlobalSnapReaderCallbacks->unknown = unknownCallback;

    return OTF2_SUCCESS;
}


/** @brief Set user callbacks.
 *
 *  @param readerHandle     Global snapshot reader handle.
 *  @param callbacks        Structure keeping all defined callbacks.
 *  @param userData         Additional user data provided in the callbacks.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalSnapReader_SetCallbacks( OTF2_GlobalSnapReader*                readerHandle,
                                    const OTF2_GlobalSnapReaderCallbacks* callbacks,
                                    void*                                 userData )
{
    /* Validate arguments. */
    if ( NULL == readerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid global snapshot reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    /* Set internal callback struct and userdata */
    memcpy( &readerHandle->reader_callbacks,
            callbacks,
            sizeof( readerHandle->reader_callbacks ) );
    readerHandle->user_data = userData;

    return OTF2_SUCCESS;
}



/* ___ Read routines. _______________________________________________________ */



/** @brief Deliver the next snapshot record and read the next one.
 *
 *  @param readerHandle     Global snapshot reader handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static OTF2_ErrorCode
otf2_global_snap_reader_read_snap( OTF2_GlobalSnapReader* readerHandle )
{
    UTILS_ASSERT( readerHandle );

    /* If the heap is empty, all records where already read. */
    if ( 0 == readerHandle->number_of_snap_readers )
    {
        return OTF2_SUCCESS;
    }

    /* Get snapshot record with the least timestamp. */
    OTF2_SnapReader* snap_reader = readerHandle->snap_queue[ 0 ];

    /* Trigger according callback. */
    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    OTF2_ErrorCode    status    = otf2_global_snap_reader_trigger_callback( readerHandle,
                                                                            snap_reader );
    if ( OTF2_ERROR_INTERRUPTED_BY_CALLBACK == status )
    {
        interrupt = !OTF2_CALLBACK_SUCCESS;
        status    = OTF2_SUCCESS;
    }
    if ( status != OTF2_SUCCESS )
    {
        return UTILS_ERROR( status, "Triggering sapshot callback failed!" );
    }

    /*
     * Read next snapshot record from the according local reader and push it to
     * queue. This can't return OTF2_ERROR_INTERRUPTED_BY_CALLBACK, because no
     * callback will be triggered.
     */
    status = otf2_snap_reader_read( snap_reader );
    if ( status != OTF2_SUCCESS )
    {
        /* This error code means that all snapshot records from the according
         * buffer are already read. So this is no error. */
        if ( status != OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
        {
            /* This error overrules an callback interruption. */
            return UTILS_ERROR( status, "Could not read snapshot from location!" );
        }

        otf2_archive_close_snap_reader( readerHandle->archive, snap_reader );

        /* remove snapshot reader from queue */
        readerHandle->number_of_snap_readers--;

        if ( 0 == readerHandle->number_of_snap_readers )
        {
            /*
             * Queue is empty.
             * But still pass an !OTF2_CALLBACK_SUCCESS to the caller.
             */
            goto out;
        }

        /* move snapshot reader from the last leaf to the top */
        readerHandle->snap_queue[ 0 ] = readerHandle->snap_queue[ readerHandle->number_of_snap_readers ];
    }

    /* update queue with new snapshot record in top snapshot reader */
    percolate_down( readerHandle, 0 );

out:
    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


/** @brief Read multiple snapshot records.
 *
 *  @param readerHandle   Global snapshot reader handle.
 *  @param recordsToRead  Number of record to read.
 *  @param recordsRead    Number of records read.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
OTF2_ErrorCode
OTF2_GlobalSnapReader_ReadSnapshots( OTF2_GlobalSnapReader* readerHandle,
                                     const uint64_t         recordsToRead,
                                     uint64_t*              recordsRead )
{
    /* Validate arguments. */
    if ( NULL == readerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid global snapshot reader handle!" );
    }

    OTF2_ErrorCode status = OTF2_SUCCESS;
    uint64_t       read   = 0;
    for ( read = 0; read < recordsToRead; read++ )
    {
        /* Check, if we have a new snapshot record */
        if ( 0 == readerHandle->number_of_snap_readers )
        {
            break;
        }

        status = otf2_global_snap_reader_read_snap( readerHandle );
        if ( status != OTF2_SUCCESS )
        {
            if ( status == OTF2_ERROR_INTERRUPTED_BY_CALLBACK )
            {
                /* We still delivered a record, count it */
                read++;
            }
            break;
        }
    }

    *recordsRead = read;
    return status;
}


/* ___ Implementation of internal functions. ________________________________ */



/** @brief Compare two snapshot records by timestamp.
 *
 *  @param snap_reader_i First generic snapshot record to be compared.
 *  @param snap_reader_j Second generic snapshot record to be compared.
 *
 *  @return             True, if snap_reader_i has a smaller timestamp than
 *                      snap_reader_j.
 */
bool
compare_snap_reader_lt( OTF2_SnapReader* snap_reader_i,
                        OTF2_SnapReader* snap_reader_j )
{
    if ( snap_reader_i->current_snap.record.time < snap_reader_j->current_snap.record.time )
    {
        return true;
    }
    else if ( snap_reader_i->current_snap.record.time == snap_reader_j->current_snap.record.time )
    {
        /* If timestamps equal, compare the location IDs. This guaranties that
         * snapshot records with same timestamps on one location are delivered
         * right after each other without records of other locations with same
         * timestamp between them.
         * Note, there are at most one record for each location in the queue,
         * thus the locations are never equal. */
        if ( snap_reader_i->location_id < snap_reader_j->location_id )
        {
            return true;
        }
    }

    return false;
}

void
percolate_down( OTF2_GlobalSnapReader* reader,
                uint64_t               node )
{
    OTF2_SnapReader** snap_queue = reader->snap_queue;
    uint64_t          n          = reader->number_of_snap_readers;

    while ( node < n )
    {
        uint64_t smallest = node;

        /* check left child */
        uint64_t child = node * 2 + 1;
        if ( child < n
             && compare_snap_reader_lt( snap_queue[ child ],
                                        snap_queue[ smallest ] ) )
        {
            smallest = child;
        }

        /* check right child */
        child++;
        if ( child < n
             && compare_snap_reader_lt( snap_queue[ child ],
                                        snap_queue[ smallest ] ) )
        {
            smallest = child;
        }

        if ( smallest == node )
        {
            return;
        }

        /* swap down, and continue with child */
        OTF2_SnapReader* tmp = snap_queue[ smallest ];
        snap_queue[ smallest ] = snap_queue[ node ];
        snap_queue[ node ]     = tmp;
        node                   = smallest;
    }
}
