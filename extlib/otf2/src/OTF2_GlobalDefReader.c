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
 *  @file       src/OTF2_GlobalDefReader.c
 *  @status     alpha
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@tu-dresden.de>
 *
 *  @brief      This module reads the global definitions from the global
 *              definition file. Therefore, the programmer has to register
 *              a callback function for each type of record. Rudimentary
 *              filtering could be done by registering only callbacks for
 *              the needed record types. This is an external interface.
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

#include "otf2_global_def_reader.h"

/** @brief This local reader is defined by following struct. */
struct OTF2_GlobalDefReader_struct
{
    /** Archive handle */
    OTF2_Archive*                 archive;
    /** Buffer handle */
    OTF2_Buffer*                  buffer;
    /** Structure of callback references */
    OTF2_GlobalDefReaderCallbacks reader_callbacks;
    /** Userdata pointer which is passed to the callbacks */
    void*                         user_data;
};


static inline OTF2_ErrorCode
otf2_global_def_reader_read_unknown( OTF2_GlobalDefReader* reader );


static inline void
otf2_global_def_region_convert_pre_1_1( OTF2_GlobalDefRegion* record )
{
    /* If there is no additional name available use content of original name field. */
    record->canonical_name = record->name;

    otf2_attic_def_region_convert_region_type( record->region_type,
                                               &record->region_role,
                                               &record->paradigm,
                                               &record->region_flags );
}


static inline void
otf2_global_def_group_convert_pre_1_2( OTF2_GlobalDefGroup* record )
{
    otf2_attic_def_group_convert_group_type_pre_1_2( record->group_type_pre12,
                                                     &record->group_type,
                                                     &record->paradigm,
                                                     &record->group_flags );
}


static inline void
otf2_global_def_metric_class_convert_pre_1_2( OTF2_GlobalDefMetricClass* record )
{
    record->recorder_kind = OTF2_RECORDER_KIND_UNKNOWN;
}


/* This _must_ be done for the generated part of the OTF2 global definition reader */
#include "OTF2_GlobalDefReader_inc.c"
/* This _must_ be done for the generated part of the OTF2 global definition reader */


/** @brief Open a the reader object.
 *
 *  @param archivePath         Path of the OTF2 archive which has to be opened.
 *  @param archiveName         Name of the OTF2 archive inside the path.
 *  @param substrate           File type of the archive.
 *
 *  @return A valid OTF2_GlobalDefReader or NULL in case of an error.
 */

OTF2_GlobalDefReader*
otf2_global_def_reader_new( OTF2_Archive* archive )
{
    OTF2_ErrorCode ret;

    OTF2_GlobalDefReader* reader = NULL;
    reader = calloc( 1, sizeof( OTF2_GlobalDefReader ) );
    if ( !reader )
    {
        goto clean_mem_new;
    }

    memset( &( reader->reader_callbacks ), '\0', sizeof( OTF2_GlobalDefReaderCallbacks ) );

    uint64_t evt_chunk_size;
    uint64_t def_chunk_size;
    ret = otf2_archive_get_chunksize( archive,
                                      &evt_chunk_size,
                                      &def_chunk_size );
    if ( ret != OTF2_SUCCESS )
    {
        goto clean_mem_new;
    }

    /* Generate an internal buffer */
    reader->buffer = OTF2_Buffer_New( archive,
                                      reader,
                                      def_chunk_size,
                                      OTF2_BUFFER_READ,
                                      OTF2_BUFFER_CHUNKED,
                                      OTF2_FILETYPE_GLOBAL_DEFS,
                                      OTF2_UNDEFINED_LOCATION );

    reader->archive = archive;

    if ( !reader->buffer )
    {
        goto clean_mem_new;
    }

    return reader;

clean_mem_new:
    UTILS_ERROR_POSIX();
    free( reader );

    return NULL;
}


/** @brief Delete the reader object.
 *
 *  @param reader         Reader Object.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */

OTF2_ErrorCode
otf2_global_def_reader_delete( OTF2_GlobalDefReader* reader )
{
    if ( reader )
    {
        OTF2_ErrorCode ret
            = OTF2_Buffer_Delete( reader->buffer );
        if ( ret != OTF2_SUCCESS )
        {
            return UTILS_ERROR( ret, "Buffer deletion failed!" );
        }

        free( reader );
    }

    return OTF2_SUCCESS;
}


OTF2_GlobalDefReaderCallbacks*
OTF2_GlobalDefReaderCallbacks_New( void )
{
    return calloc( 1, sizeof( OTF2_GlobalDefReaderCallbacks ) );
}


void
OTF2_GlobalDefReaderCallbacks_Delete( OTF2_GlobalDefReaderCallbacks* globalDefReaderCallbacks )
{
    free( globalDefReaderCallbacks );
}

void
OTF2_GlobalDefReaderCallbacks_Clear( OTF2_GlobalDefReaderCallbacks* globalDefReaderCallbacks )
{
    if ( globalDefReaderCallbacks != NULL )
    {
        memset( globalDefReaderCallbacks, 0, sizeof( OTF2_GlobalDefReaderCallbacks ) );
    }
}


/** @brief Before any global definition record could be read, the user
 *         must register a callback function for each record type. This
 *         could be done by the following function.
 *
 *  @param reader     Reader Object.
 *  @param callbacks  Callback pointer struct.
 *  @param userData   Generic pointer to handle users data inside callbacks.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */

OTF2_ErrorCode
OTF2_GlobalDefReader_SetCallbacks( OTF2_GlobalDefReader*                reader,
                                   const OTF2_GlobalDefReaderCallbacks* callbacks,
                                   void*                                userData )
{
    /* Check if this is a valid reader object */
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INTEGRITY_FAULT,
                            "No valid reader object!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    /* Set internal callback struct and userdata */
    memcpy( &reader->reader_callbacks,
            callbacks,
            sizeof( reader->reader_callbacks ) );
    reader->user_data = userData;

    return OTF2_SUCCESS;
}


/** @brief After callback registration, the definitions could be
 *         read with the following function. The user of this
 *         function tells the system how many definitions he is
 *         able to handle (recordsToRead) and the function returns
 *         how many definitions where in the stream (recordsRead).
 *         It should usually be the case that both values are the
 *         same. If this is not the case, then there where less
 *         records than requested in the stream.
 *
 *  @param reader         Reader Object.
 *  @param recordsToRead  How many records have to be read next.
 *  @param recordsRead    How many records where read?
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */

OTF2_ErrorCode
OTF2_GlobalDefReader_ReadDefinitions( OTF2_GlobalDefReader* reader,
                                      uint64_t              recordsToRead,
                                      uint64_t*             recordsRead )
{
    /* Check if this is a valid reader object */
    if ( !reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "No valid reader object!" );
    }

    OTF2_ErrorCode ret = OTF2_SUCCESS;
    uint64_t       read;
    for ( read = 0; read < recordsToRead; read++ )
    {
        ret = otf2_global_def_reader_read( reader );
        if ( ret != OTF2_SUCCESS )
        {
            if ( ret == OTF2_ERROR_INTERRUPTED_BY_CALLBACK )
            {
                /* We still delivered a record, count it */
                read++;
            }
            /* This is the end of file, which is not an error. */
            else if ( ret == OTF2_ERROR_INDEX_OUT_OF_BOUNDS )
            {
                ret = OTF2_SUCCESS;
            }
            break;
        }
    }

    *recordsRead = read;
    return ret;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetUnknownCallback( OTF2_GlobalDefReaderCallbacks*       globalDefReaderCallbacks,
                                                  OTF2_GlobalDefReaderCallback_Unknown unknownCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->unknown = unknownCallback;

    return OTF2_SUCCESS;
}


/** @brief Reads a definition record of unknown type and triggers the callback.
 *
 *  @param reader   Reference to the reader object
 *
 *  @return OTF2_SUCCESS
 */
static inline OTF2_ErrorCode
otf2_global_def_reader_read_unknown( OTF2_GlobalDefReader* reader )
{
    OTF2_ErrorCode ret;

    /* Get record length and test memory availability */
    uint64_t record_length;
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read definition record of unknown type." );
    }

    /* Skip record */
    ret = OTF2_Buffer_Skip( reader->buffer, record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not skip definition record of unknown type." );
    }

    /* Trigger callback */
    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.unknown )
    {
        interrupt = reader->reader_callbacks.unknown( reader->user_data );
    }

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}
