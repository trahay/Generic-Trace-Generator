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
 *  @file       OTF2_Reader.c
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This module is an delegation layer to provide an uniform and
 *              convienent interface to read OTF2 archives as well as archives
 *              created in OTF oder EPILOG format.
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

#include "otf2_internal.h"
#include "otf2_reader.h"
#include "otf2_archive.h"

/* ___ Type definitions. ____________________________________________________ */



/** @internal
 *  @brief Defines the format of the provided archive. Please see
 *  OTF2_ArchiveType_enum for a detailed description.
 */
typedef enum OTF2_ArchiveType_enum OTF2_ArchiveType;

/** @internal
 *  @brief Defines the format of the provided archive.
 */
enum OTF2_ArchiveType_enum
{
    /** @internal No valid archive. */
    OTF2_ARCHIVE_UNDEFINED = 0,
    /** @internal OTF2 archive. */
    OTF2_ARCHIVE_OTF2      = 1,
    /** @internal OTF archive. */
    OTF2_ARCHIVE_OTF       = 2,
    /** @internal EPILOG archive. */
    OTF2_ARCHIVE_ELG       = 3
};


/* ___ Reader structure. ____________________________________________________ */



/** @brief Keeps all neccessary information for reading. */
struct OTF2_Reader_struct
{
    /** Own copy of the archive name. */
    char* archive_name;
    /** Own copy of the archive path. */
    char* archive_path;

    /** Own archive handle. */
    void*                           archive;
    /** Archive type: OTF2, OTF or ELG. */
    OTF2_ArchiveType                archive_type;

    const otf2_reader_archive_impl* impl;
};



/* ___ Prototypes for internal functions. ___________________________________ */



static inline OTF2_ErrorCode
otf2_reader_split_anchor_file_path( const char*       anchorFilePath,
                                    char**            archivePath,
                                    char**            archiveName,
                                    OTF2_ArchiveType* archiveType );



/* ___ Implementation. ______________________________________________________ */

OTF2_Reader*
OTF2_Reader_Open( const char* anchorFilePath )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* Validate arguments. */
    if ( NULL == anchorFilePath )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid file path!" );
        return NULL;
    }

    /* Allocate memory for reader handle. */
    OTF2_Reader* reader = ( OTF2_Reader* )calloc( 1, sizeof( OTF2_Reader ) );

    if ( reader == NULL )
    {
        UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                     "Could not create reader handle!" );
        return NULL;
    }

    /* Initialize handle data. */
    reader->archive_name = NULL;
    reader->archive_path = NULL;
    reader->archive      = NULL;
    reader->archive_type = OTF2_ARCHIVE_UNDEFINED;

    /* Get archive name and archive type. */
    status = otf2_reader_split_anchor_file_path( anchorFilePath,
                                                 &( reader->archive_path ),
                                                 &( reader->archive_name ),
                                                 &( reader->archive_type ) );

    if ( status != OTF2_SUCCESS )
    {
        goto otf2_reader_free;
    }


    /* Switch between OTF2, OTF, ELG.
     * This switch handles all initialization and function pointer registration
     * for each individual archive type. */
    switch ( reader->archive_type )
    {
        /* ___ OTF2 ___ */
        case OTF2_ARCHIVE_OTF2:
        {
            reader->archive = OTF2_Archive_Open( reader->archive_path,
                                                 reader->archive_name,
                                                 OTF2_FILEMODE_READ,
                                                 OTF2_UNDEFINED_UINT64,
                                                 OTF2_UNDEFINED_UINT64,
                                                 OTF2_SUBSTRATE_UNDEFINED,
                                                 OTF2_COMPRESSION_UNDEFINED );
            if ( reader->archive == NULL )
            {
                UTILS_ERROR( OTF2_ERROR_PROCESSED_WITH_FAULTS,
                             "Archive creation failed!" );
                goto otf2_reader_free;
            }

            reader->impl = otf2_archive_get_reader_impl();

            if ( status != OTF2_SUCCESS )
            {
                UTILS_ERROR( status, "Function pointer registration failed!" );
                goto otf2_reader_free;
            }

            break;
        }

        /* ___ OTF ___ */
        case OTF2_ARCHIVE_OTF:
        {
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "OTF is not supported yet!" );
            goto otf2_reader_free;
        }

        /* ___ ELG ___ */
        case OTF2_ARCHIVE_ELG:
        {
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "EPILOG is not supported yet!" );
            goto otf2_reader_free;
        }
        default:
        {
            UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                         "Unknown format!" );
            goto otf2_reader_free;
        }
    }

    return reader;

/* Release allocated memory on error. */
otf2_reader_free:
    if ( reader != NULL )
    {
        free( reader->archive_name );
        free( reader->archive_path );
        free( reader );
    }

    return NULL;
}


OTF2_ErrorCode
OTF2_Reader_Close( OTF2_Reader* reader )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    /* Validate arguments. */
    if ( NULL == reader )
    {
        return OTF2_SUCCESS;
    }

    /* Close archive. */
    status = reader->impl->delete_archive( reader->archive );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Archive deletion failed!" );
    }

    /* Release allocated memory. */
    free( reader->archive_name );
    free( reader->archive_path );
    free( reader );

    return OTF2_SUCCESS;
}



/* ___ Register callbacks ___________________________________________________ */


OTF2_ErrorCode
OTF2_Reader_SetFileSionCallbacks( OTF2_Reader*                  reader,
                                  const OTF2_FileSionCallbacks* fileSionCallbacks,
                                  void*                         fileSionData )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader handle!" );
    }

    if ( NULL == fileSionCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    return reader->impl->set_file_sion_callbacks( reader->archive,
                                                  fileSionCallbacks,
                                                  fileSionData );
}


OTF2_ErrorCode
OTF2_Reader_RegisterEvtCallbacks( OTF2_Reader*                   reader,
                                  OTF2_EvtReader*                evtReader,
                                  const OTF2_EvtReaderCallbacks* callbacks,
                                  void*                          userData )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    if ( NULL == evtReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid local event reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    return reader->impl->register_evt_callbacks( evtReader,
                                                 callbacks,
                                                 userData );
}


OTF2_ErrorCode
OTF2_Reader_RegisterGlobalEvtCallbacks( OTF2_Reader*                         reader,
                                        OTF2_GlobalEvtReader*                evtReader,
                                        const OTF2_GlobalEvtReaderCallbacks* callbacks,
                                        void*                                userData )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    if ( NULL == evtReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid global event reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    return reader->impl->register_global_evt_callbacks( evtReader,
                                                        callbacks,
                                                        userData );
}


OTF2_ErrorCode
OTF2_Reader_RegisterDefCallbacks( OTF2_Reader*                   reader,
                                  OTF2_DefReader*                defReader,
                                  const OTF2_DefReaderCallbacks* callbacks,
                                  void*                          userData )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    if ( NULL == defReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid local definition reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    return reader->impl->register_def_callbacks( defReader,
                                                 callbacks,
                                                 userData );
}


OTF2_ErrorCode
OTF2_Reader_RegisterGlobalDefCallbacks( OTF2_Reader*                         reader,
                                        OTF2_GlobalDefReader*                defReader,
                                        const OTF2_GlobalDefReaderCallbacks* callbacks,
                                        void*                                userData )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    if ( NULL == defReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid global definition reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    return reader->impl->register_global_def_callbacks( defReader,
                                                        callbacks,
                                                        userData );
}


OTF2_ErrorCode
OTF2_Reader_RegisterSnapCallbacks( OTF2_Reader*                    reader,
                                   OTF2_SnapReader*                snapReader,
                                   const OTF2_SnapReaderCallbacks* callbacks,
                                   void*                           userData )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    if ( NULL == snapReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid local event reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    return reader->impl->register_snap_callbacks( snapReader,
                                                  callbacks,
                                                  userData );
}


OTF2_ErrorCode
OTF2_Reader_RegisterGlobalSnapCallbacks( OTF2_Reader*                          reader,
                                         OTF2_GlobalSnapReader*                snapReader,
                                         const OTF2_GlobalSnapReaderCallbacks* callbacks,
                                         void*                                 userData )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    if ( NULL == snapReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid global event reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    return reader->impl->register_global_snap_callbacks( snapReader,
                                                         callbacks,
                                                         userData );
}


OTF2_ErrorCode
OTF2_Reader_RegisterMarkerCallbacks( OTF2_Reader*                      reader,
                                     OTF2_MarkerReader*                markerReader,
                                     const OTF2_MarkerReaderCallbacks* callbacks,
                                     void*                             userData )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    if ( NULL == markerReader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid marker reader handle!" );
    }

    if ( NULL == callbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid callback arguments!" );
    }

    return reader->impl->register_marker_callbacks( markerReader,
                                                    callbacks,
                                                    userData );
}


/* ___ Read routines. _______________________________________________________ */


OTF2_ErrorCode
OTF2_Reader_ReadLocalEvents( OTF2_Reader*    reader,
                             OTF2_EvtReader* evtReader,
                             uint64_t        eventsToRead,
                             uint64_t*       eventsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_local_events( evtReader, eventsToRead, eventsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadAllLocalEvents( OTF2_Reader*    reader,
                                OTF2_EvtReader* evtReader,
                                uint64_t*       eventsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    /* Just uses ReadLocalEvents() with UINT64_MAX. */
    return reader->impl->read_local_events( evtReader, OTF2_UNDEFINED_UINT64, eventsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadGlobalEvent( OTF2_Reader*          reader,
                             OTF2_GlobalEvtReader* evtReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_global_event( evtReader );
}


OTF2_ErrorCode
OTF2_Reader_HasGlobalEvent( OTF2_Reader*          reader,
                            OTF2_GlobalEvtReader* evtReader,
                            int*                  flag )
{
    /* Validate arguments. */
    if ( NULL == reader || NULL == flag )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer arguments!" );
    }

    return reader->impl->has_global_event( evtReader, flag );
}


OTF2_ErrorCode
OTF2_Reader_ReadGlobalEvents( OTF2_Reader*          reader,
                              OTF2_GlobalEvtReader* evtReader,
                              uint64_t              eventsToRead,
                              uint64_t*             eventsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_global_events( evtReader, eventsToRead, eventsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadAllGlobalEvents( OTF2_Reader*          reader,
                                 OTF2_GlobalEvtReader* evtReader,
                                 uint64_t*             eventsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_global_events( evtReader, OTF2_UNDEFINED_UINT64, eventsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadLocalEventsBackward( OTF2_Reader*    reader,
                                     OTF2_EvtReader* evtReader,
                                     uint64_t        eventsToRead,
                                     uint64_t*       eventsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_local_events_backward( evtReader, eventsToRead, eventsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadLocalDefinitions( OTF2_Reader*    reader,
                                  OTF2_DefReader* defReader,
                                  uint64_t        definitionsToRead,
                                  uint64_t*       definitionsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_local_definitions( defReader, definitionsToRead,
                                                 definitionsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadAllLocalDefinitions( OTF2_Reader*    reader,
                                     OTF2_DefReader* defReader,
                                     uint64_t*       definitionsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_local_definitions( defReader, OTF2_UNDEFINED_UINT64,
                                                 definitionsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadGlobalDefinitions( OTF2_Reader*          reader,
                                   OTF2_GlobalDefReader* defReader,
                                   uint64_t              definitionsToRead,
                                   uint64_t*             definitionsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_global_definitions( defReader, definitionsToRead,
                                                  definitionsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadAllGlobalDefinitions( OTF2_Reader*          reader,
                                      OTF2_GlobalDefReader* defReader,
                                      uint64_t*             definitionsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_global_definitions( defReader, OTF2_UNDEFINED_UINT64,
                                                  definitionsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadLocalSnapshots( OTF2_Reader*     reader,
                                OTF2_SnapReader* snapReader,
                                uint64_t         recordsToRead,
                                uint64_t*        recordsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_local_snapshots( snapReader, recordsToRead, recordsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadAllLocalSnapshots( OTF2_Reader*     reader,
                                   OTF2_SnapReader* snapReader,
                                   uint64_t*        recordsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    /* Just uses ReadLocalSnaps() with UINT64_MAX. */
    return reader->impl->read_local_snapshots( snapReader, OTF2_UNDEFINED_UINT64, recordsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadGlobalSnapshots( OTF2_Reader*           reader,
                                 OTF2_GlobalSnapReader* snapReader,
                                 uint64_t               recordsToRead,
                                 uint64_t*              recordsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_global_snapshots( snapReader, recordsToRead, recordsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadAllGlobalSnapshots( OTF2_Reader*           reader,
                                    OTF2_GlobalSnapReader* snapReader,
                                    uint64_t*              recordsRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_global_snapshots( snapReader, OTF2_UNDEFINED_UINT64, recordsRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadMarkers( OTF2_Reader*       reader,
                         OTF2_MarkerReader* markerReader,
                         uint64_t           markersToRead,
                         uint64_t*          markersRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_markers( markerReader, markersToRead,
                                       markersRead );
}


OTF2_ErrorCode
OTF2_Reader_ReadAllMarkers( OTF2_Reader*       reader,
                            OTF2_MarkerReader* markerReader,
                            uint64_t*          markersRead )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->read_markers( markerReader, OTF2_UNDEFINED_UINT64,
                                       markersRead );
}


/* ___ Get reader routines. _________________________________________________ */


OTF2_EvtReader*
OTF2_Reader_GetEvtReader( OTF2_Reader*     reader,
                          OTF2_LocationRef location )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_evt_reader( reader->archive, location );
}


OTF2_GlobalEvtReader*
OTF2_Reader_GetGlobalEvtReader( OTF2_Reader* reader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_global_evt_reader( reader->archive );
}


OTF2_DefReader*
OTF2_Reader_GetDefReader( OTF2_Reader*     reader,
                          OTF2_LocationRef location )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_def_reader( reader->archive, location );
}


OTF2_GlobalDefReader*
OTF2_Reader_GetGlobalDefReader( OTF2_Reader* reader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_global_def_reader( reader->archive );
}


OTF2_SnapReader*
OTF2_Reader_GetSnapReader( OTF2_Reader*     reader,
                           OTF2_LocationRef location
                           )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_snap_reader( reader->archive, location );
}


OTF2_GlobalSnapReader*
OTF2_Reader_GetGlobalSnapReader( OTF2_Reader* reader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_global_snap_reader( reader->archive );
}


/* Although its unusual to get a writer handle here, the marker writer is
 * typically used when also reading the trace archive. Therefore, it can be
 * requested from the reader handle. */
OTF2_MarkerWriter*
OTF2_Reader_GetMarkerWriter( OTF2_Reader* reader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_marker_writer( reader->archive );
}


OTF2_MarkerReader*
OTF2_Reader_GetMarkerReader( OTF2_Reader* reader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_marker_reader( reader->archive );
}


/* ___ Close reader routines. _______________________________________________ */


OTF2_ErrorCode
OTF2_Reader_CloseEvtReader( OTF2_Reader*    reader,
                            OTF2_EvtReader* evtReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->close_evt_reader( reader->archive, evtReader );
}


OTF2_ErrorCode
OTF2_Reader_CloseGlobalEvtReader( OTF2_Reader*          reader,
                                  OTF2_GlobalEvtReader* globalEvtReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader handle!" );
    }

    return reader->impl->close_global_evt_reader( reader->archive, globalEvtReader );
}


OTF2_ErrorCode
OTF2_Reader_CloseDefReader( OTF2_Reader*    reader,
                            OTF2_DefReader* defReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->close_def_reader( reader->archive, defReader );
}


OTF2_ErrorCode
OTF2_Reader_CloseGlobalDefReader( OTF2_Reader*          reader,
                                  OTF2_GlobalDefReader* globalDefReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader handle!" );
    }

    return reader->impl->close_global_def_reader( reader->archive, globalDefReader );
}


OTF2_ErrorCode
OTF2_Reader_CloseSnapReader( OTF2_Reader*     reader,
                             OTF2_SnapReader* snapReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->close_snap_reader( reader->archive, snapReader );
}


OTF2_ErrorCode
OTF2_Reader_CloseGlobalSnapReader( OTF2_Reader*           reader,
                                   OTF2_GlobalSnapReader* globalSnapReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader handle!" );
    }

    return reader->impl->close_global_snap_reader( reader->archive, globalSnapReader );
}


OTF2_ErrorCode
OTF2_Reader_CloseMarkerReader( OTF2_Reader*       reader,
                               OTF2_MarkerReader* markerReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader handle!" );
    }

    return reader->impl->close_marker_reader( reader->archive, markerReader );
}


OTF2_ErrorCode
OTF2_Reader_CloseMarkerWriter( OTF2_Reader*       reader,
                               OTF2_MarkerWriter* markerWriter )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid reader handle!" );
    }

    return reader->impl->close_marker_writer( reader->archive, markerWriter );
}


/* ___ Implementation of internal functions. ________________________________ */



/** @internal
 *  @brief Get archive name and type from a given anchor file name.
 *
 *  @param anchorFilePath   Name of the anchorfile e.g. 'anchorfile.otf2'.
 *  @param archivePath      Return pointer to the archive path. For the string
 *                          memory is alloceted and must be released.
 *  @param archiveName      Return pointer to the archive name. For the string
 *                          memory is alloceted and must be released.
 *  @param archiveType      Return pointer to the archive type.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
static inline OTF2_ErrorCode
otf2_reader_split_anchor_file_path( const char*       anchorFilePath,
                                    char**            archivePath,
                                    char**            archiveName,
                                    OTF2_ArchiveType* archiveType )
{
    int16_t last_slash = -1;
    int16_t last_dot   = -1;
    int16_t null_char  = 0;

    /* Get position of last slash, last dot and first null character. */
    while ( ( anchorFilePath[ null_char ] != '\0' ) &&
            ( null_char < INT16_MAX ) )
    {
        if ( anchorFilePath[ null_char ] == '/' )
        {
            last_slash = null_char;
        }
        if ( anchorFilePath[ null_char ] == '.' )
        {
            last_dot = null_char;
        }
        null_char++;
    }

    if ( null_char == INT16_MAX )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Could not find !" );
    }

    /* Check if there is at least one dot in the file path and this dot is not at
     * the first position. */
    if ( last_dot <= 0 )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Could not find file extension!" );
    }

    /* Match file extension string and set archive type. */
    const char* type = anchorFilePath + last_dot + 1;

    if ( ( strcmp( type, "otf2" ) == 0 ) || ( strcmp( type, "OTF2" ) == 0 ) )
    {
        *archiveType = OTF2_ARCHIVE_OTF2;
    }
    else if ( ( strcmp( type, "otf" ) == 0 ) || ( strcmp( type, "OTF" ) == 0 ) )
    {
        *archiveType = OTF2_ARCHIVE_OTF;
    }
    else if ( ( strcmp( type, "elg" ) == 0 ) || ( strcmp( type, "ELG" ) == 0 ) )
    {
        *archiveType = OTF2_ARCHIVE_ELG;
    }
    else
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This no valid file extension!" );
    }

    /* Get archive path from anchor file path.
     * If there was no slash found (-1) the archive path is '.',
     * if the last slash is at first position (0) the archive path is '/',
     * otherwise the archive path is everything before last slash. */
    char* archive_path = NULL;
    if ( last_slash > 0 )
    {
        archive_path = ( char* )malloc( ( last_slash + 1 ) * sizeof( char ) );
    }
    else
    {
        archive_path = ( char* )malloc( 2 * sizeof( char ) );
    }

    if ( NULL == archive_path )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Could not allocate memory for string!" );
    }

    if ( last_slash >= 1 )
    {
        memset( archive_path, '\0', ( last_slash + 1 ) * sizeof( char ) );
        memcpy( archive_path, anchorFilePath, last_slash * sizeof( char ) );
    }
    else if ( last_slash == 0 )
    {
        archive_path[ 0 ] = '/';
        archive_path[ 1 ] = '\0';
    }
    else if ( last_slash == -1 )
    {
        archive_path[ 0 ] = '.';
        archive_path[ 1 ] = '\0';
    }
    *archivePath = archive_path;


    /* Get archive name from anchor file path. */
    char* archive_name = ( char* )malloc( ( last_dot - last_slash ) * sizeof( char ) );
    if ( NULL == archive_name )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Could not allocate memory for string!" );
    }

    memset( archive_name, '\0', ( last_dot - last_slash ) * sizeof( char ) );
    memcpy( archive_name, anchorFilePath + last_slash + 1, last_dot - last_slash - 1  );
    *archiveName = archive_name;

    return OTF2_SUCCESS;
}



/* ___ Additional get functions. ____________________________________________ */


OTF2_ErrorCode
OTF2_Reader_GetVersion( OTF2_Reader* reader,
                        uint8_t*     major,
                        uint8_t*     minor,
                        uint8_t*     bugfix )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !major || !minor || !bugfix )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    return reader->impl->get_version( reader->archive, major, minor, bugfix );
}

OTF2_ErrorCode
OTF2_Reader_GetChunkSize( OTF2_Reader* reader,
                          uint64_t*    chunkSizeEvents,
                          uint64_t*    chunkSizeDefinitions )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !chunkSizeEvents || !chunkSizeDefinitions )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid arguments!" );
    }

    return reader->impl->get_chunk_size( reader->archive,
                                         chunkSizeEvents,
                                         chunkSizeDefinitions );
}

OTF2_ErrorCode
OTF2_Reader_GetFileSubstrate( OTF2_Reader*        reader,
                              OTF2_FileSubstrate* substrate )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !substrate )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid substrate argument!" );
    }

    return reader->impl->get_file_substrate( reader->archive, substrate );
}

OTF2_ErrorCode
OTF2_Reader_GetCompression( OTF2_Reader*      reader,
                            OTF2_Compression* compression )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !compression )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid compression argument!" );
    }

    return reader->impl->get_compression( reader->archive, compression );
}

OTF2_ErrorCode
OTF2_Reader_GetNumberOfLocations( OTF2_Reader* reader,
                                  uint64_t*    numberOfLocations )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !numberOfLocations )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid numberOfLocations argument!" );
    }

    return reader->impl->get_number_of_locations( reader->archive, numberOfLocations );
}


OTF2_ErrorCode
OTF2_Reader_GetNumberOfGlobalDefinitions( OTF2_Reader* reader,
                                          uint64_t*    numberOfDefinitions )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !numberOfDefinitions )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid numberOfDefinitions argument!" );
    }

    return reader->impl->get_number_of_global_defs( reader->archive, numberOfDefinitions );
}

OTF2_ErrorCode
OTF2_Reader_GetMachineName( OTF2_Reader* reader,
                            char**       machineName )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !machineName )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid machineName argument!" );
    }

    return reader->impl->get_machine_name( reader->archive, machineName );
}

OTF2_ErrorCode
OTF2_Reader_GetCreator( OTF2_Reader* reader,
                        char**       creator )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !creator )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid creator argument!" );
    }

    return reader->impl->get_creator( reader->archive, creator );
}

OTF2_ErrorCode
OTF2_Reader_GetDescription( OTF2_Reader* reader,
                            char**       description )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !description )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid description argument!" );
    }

    return reader->impl->get_description( reader->archive, description );
}

OTF2_ErrorCode
OTF2_Reader_GetPropertyNames( OTF2_Reader* reader,
                              uint32_t*    numberOfProperties,
                              char***      names )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !numberOfProperties )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid numberOfProperties argument!" );
    }
    if ( !names )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid names argument!" );
    }

    return reader->impl->get_property_names( reader->archive, numberOfProperties, names );
}

OTF2_ErrorCode
OTF2_Reader_GetProperty( OTF2_Reader* reader,
                         const char*  name,
                         char**       value )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !name )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid name argument!" );
    }
    if ( !value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid value argument!" );
    }

    return reader->impl->get_property( reader->archive, name, value );
}

OTF2_ErrorCode
OTF2_Reader_GetBoolProperty( OTF2_Reader* reader,
                             const char*  name,
                             bool*        value )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !name )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid name argument!" );
    }
    if ( !value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid value argument!" );
    }

    char*          value_as_string = NULL;
    OTF2_ErrorCode status          = reader->impl->get_property( reader->archive,
                                                                 name,
                                                                 &value_as_string );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    /* convert value to lowercase */
    for ( char* ptr = value_as_string; *ptr; ptr++ )
    {
        *ptr = tolower( *ptr );
    }


    if ( 0 == strcmp( value_as_string, "true" ) )
    {
        *value = true;
    }
    else if ( 0 == strcmp( value_as_string, "false" ) )
    {
        *value = false;
    }
    else
    {
        status = UTILS_ERROR( OTF2_ERROR_PROPERTY_VALUE_INVALID,
                              "Value is neither 'true' nor 'false'!" );
    }

    free( value_as_string );
    return status;
}

OTF2_ErrorCode
OTF2_Reader_GetTraceId( OTF2_Reader* reader,
                        uint64_t*    id )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !id )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid id argument!" );
    }

    return reader->impl->get_trace_id( reader->archive, id );
}

OTF2_ErrorCode
OTF2_Reader_GetNumberOfSnapshots( OTF2_Reader* reader,
                                  uint32_t*    number )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !number )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid numberOfSnapshots argument!" );
    }

    return reader->impl->get_number_of_snapshots( reader->archive, number );
}


OTF2_ErrorCode
OTF2_Reader_GetNumberOfThumbnails( OTF2_Reader* reader,
                                   uint32_t*    number )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }
    if ( !number )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid numberOfThums argument!" );
    }

    return reader->impl->get_number_of_thumbnails( reader->archive, number );
}


OTF2_ThumbReader*
OTF2_Reader_GetThumbReader( OTF2_Reader* reader,
                            uint32_t     number )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                     "This is no valid reader handle!" );

        return NULL;
    }

    return reader->impl->get_thumb_reader( reader->archive, number );
}

OTF2_ErrorCode
OTF2_Reader_CloseThumbReader( OTF2_Reader*      reader,
                              OTF2_ThumbReader* thumbReader )
{
    /* Validate arguments. */
    if ( NULL == reader )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "This is no valid reader handle!" );
    }

    return reader->impl->close_thumb_reader( reader->archive, thumbReader );
}
