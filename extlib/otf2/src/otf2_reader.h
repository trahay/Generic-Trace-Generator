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


#ifndef OTF2_INTERNAL_READER_H
#define OTF2_INTERNAL_READER_H


/**
 *  @file       src/otf2_reader.h
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 */


/* ___ Type definitions for the reader function pointers. ___________________ */


/** @internal
 *  @brief Delete an associated archive handle.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_DeleteArchive )( void* archive );


/** @internal
 *  @brief Set the sion callbacks for OTF2 traces.
 *
 *  @see OTF2_Archive_SetFileSionCallbacks.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_SetFileSionCallbacks )( void*                         archive,
                                   const OTF2_FileSionCallbacks* fileSionCallbacks,
                                   void*                         fileSionData );


/** @internal
 *  @brief Register event reader callbacks.
 *
 *  @param evtReader        Local event reader handle.
 *  @param callbacks        Callbacks for the event readers.
 *  @param userData         Addition user data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_RegisterEvtCallbacks )( OTF2_EvtReader*                evtReader,
                                   const OTF2_EvtReaderCallbacks* callbacks,
                                   void*                          userData );


/** @internal
 *  @brief Register global event reader callbacks.
 *
 *  @param evtReader        Global event reader handle.
 *  @param callbacks        Callbacks for the event readers.
 *  @param userData         Addition user data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_RegisterGlobalEvtCallbacks )( OTF2_GlobalEvtReader*                evtReader,
                                         const OTF2_GlobalEvtReaderCallbacks* callbacks,
                                         void*                                userData );


/** @internal
 *  @brief Register local definition reader callbacks.
 *
 *  @param defReader        Local definition reader handle.
 *  @param callbacks        Callbacks for the local definition readers.
 *  @param userData         Addition user data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_RegisterDefCallbacks )( OTF2_DefReader*                defReader,
                                   const OTF2_DefReaderCallbacks* callbacks,
                                   void*                          userData );


/** @internal
 *  @brief Register global definition reader callbacks.
 *
 *  @param defReader        Global definition reader handle.
 *  @param callbacks        Callbacks for the global definition readers.
 *  @param userData         Addition user data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_RegisterGlobalDefCallbacks )( OTF2_GlobalDefReader*                defReader,
                                         const OTF2_GlobalDefReaderCallbacks* callbacks,
                                         void*                                userData );


/** @internal
 *  @brief Register snapshot event reader callbacks.
 *
 *  @param snapReader       Local snap reader handle.
 *  @param callbacks        Callbacks for the snap readers.
 *  @param userData         Addition user data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_RegisterSnapCallbacks )( OTF2_SnapReader*                snapReader,
                                    const OTF2_SnapReaderCallbacks* callbacks,
                                    void*                           userData );


/** @internal
 *  @brief Register global snapshot event reader callbacks.
 *
 *  @param snapReader       Global snap reader handle.
 *  @param callbacks        Callbacks for the snap readers.
 *  @param userData         Addition user data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_RegisterGlobalSnapCallbacks )( OTF2_GlobalSnapReader*                snapReader,
                                          const OTF2_GlobalSnapReaderCallbacks* callbacks,
                                          void*                                 userData );


/** @internal
 *  @brief Register marker reader callbacks.
 *
 *  @param markerReader     Marker reader handle.
 *  @param callbacks        Callbacks for the marker reader.
 *  @param userData         Addition user data.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_RegisterMarkerCallbacks )( OTF2_MarkerReader*                markerReader,
                                      const OTF2_MarkerReaderCallbacks* callbacks,
                                      void*                             userData );


/** @internal
 *  @brief Read a given number of events via a local event reader.
 *
 *  @param reader           Local event reader handle.
 *  @param eventsToRead     Number events to be read.
 *  @param eventsRead       Return pointer to the number of events actually read.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_ReadLocalEvents )( OTF2_EvtReader* reader,
                              uint64_t        eventsToRead,
                              uint64_t*       eventsRead );


/** @internal
 *  @brief Read an event via a global event reader.
 *
 *  @param reader           Global event reader handle.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_ReadGlobalEvent )( OTF2_GlobalEvtReader* reader );


/** @internal
 *  @brief Read a given number of events via a global event reader.
 *
 *  @param reader           Global event reader handle.
 *  @param eventsToRead     Number events to be read.
 *  @param eventsRead       Return pointer to the number of events actually read.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_ReadGlobalEvents )( OTF2_GlobalEvtReader* reader,
                               uint64_t              eventsToRead,
                               uint64_t*             eventsRead );


/** @internal
 *  @brief Has the global event reader at least one more event to deliver.
 *
 *  @param reader           Global event reader handle.
 *  @param flag             In case of success, the flag will be set to 1
 *                          when there is at least more more event to read.
 *                          To 0 if not. Otherwise the value is undefined.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_HasGlobalEvent )( OTF2_GlobalEvtReader* reader,
                             int*                  flag );


/** @internal
 *  @brief Read a given number of definitions via a local definition reader.
 *
 *  @param reader            Local definition reader handle.
 *  @param definitionsToRead Number definitions to be read.
 *  @param definitionsRead   Return pointer to the number of definitions actually
 *                           read.
 *
 *  @return                  Returns OTF2_SUCCESS if successful, an error code
 *                           if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_ReadLocalDefinitions )( OTF2_DefReader* reader,
                                   uint64_t        definitionsToRead,
                                   uint64_t*       definitionsRead );


/** @internal
 *  @brief Read a given number of definitions via a global definition reader.
 *
 *  @param reader            Global definition reader handle.
 *  @param definitionsToRead Number definitions to be read.
 *  @param definitionsRead   Return pointer to the number of definitions actually
 *                           read.
 *
 *  @return                  Returns OTF2_SUCCESS if successful, an error code
 *                           if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_ReadGlobalDefinitions )( OTF2_GlobalDefReader* reader,
                                    uint64_t              definitionsToRead,
                                    uint64_t*             definitionsRead );


/** @internal
 *  @brief Read a given number of snapshot events via a local snap reader.
 *
 *  @param reader           Local snap reader handle.
 *  @param eventsToRead     Number events to be read.
 *  @param eventsRead       Return pointer to the number of events actually read.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_ReadLocalSnapshots )( OTF2_SnapReader* reader,
                                 uint64_t         eventsToRead,
                                 uint64_t*        eventsRead );


/** @internal
 *  @brief Read a given number of events via a global snap reader.
 *
 *  @param reader           Global snap reader handle.
 *  @param eventsToRead     Number events to be read.
 *  @param eventsRead       Return pointer to the number of events actually read.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_ReadGlobalSnapshots )( OTF2_GlobalSnapReader* reader,
                                  uint64_t               recordsToRead,
                                  uint64_t*              recordsRead );


/** @internal
 *  @brief Read a given number of markers via a marker reader.
 *
 *  @param reader            Global definition reader handle.
 *  @param markersToRead     Number markers to be read.
 *  @param markersRead       Return pointer to the number of markers actually
 *                           read.
 *
 *  @return                  Returns OTF2_SUCCESS if successful, an error code
 *                           if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_ReadMarkers )( OTF2_MarkerReader* reader,
                          uint64_t           markersToRead,
                          uint64_t*          markersRead );



/** @internal
 *  @brief Get a local event reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param locationID       Location ID for the requested local reader.
 *
 *  @return                 Returns a handle to the local event reader if
 *                          successful, NULL otherwise.
 */
typedef OTF2_EvtReader*
( *OTF2_FP_GetEvtReader )( void*    archive,
                           uint64_t locationID );


/** @internal
 *  @brief Get a global event reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *
 *  @return                 Returns a handle to the global event reader if
 *                          successful, NULL otherwise.
 */
typedef OTF2_GlobalEvtReader*
( *OTF2_FP_GetGlobalEvtReader )( void* archive );


/** @internal
 *  @brief Get a local definition reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param locationID       Location ID for the requested local reader.
 *
 *  @return                 Returns a handle to the local definition reader if
 *                          successful, NULL otherwise.
 */
typedef OTF2_DefReader*
( *OTF2_FP_GetDefReader )( void*    archive,
                           uint64_t locationID );


/** @internal
 *  @brief Get a global definition reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *
 *  @return                 Returns a handle to the global definition reader if
 *                          successful, NULL otherwise.
 */
typedef OTF2_GlobalDefReader*
( *OTF2_FP_GetGlobalDefReader )( void* archive );


/** @internal
 *  @brief Get a local snapshot event reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param locationID       Location ID for the requested local reader.
 *
 *  @return                 Returns a handle to the local snapshot reader if
 *                          successful, NULL otherwise.
 */
typedef OTF2_SnapReader*
( *OTF2_FP_GetSnapReader )( void*    archive,
                            uint64_t locationID );

/** @internal
 *  @brief Get a global snap reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *
 *  @return                 Returns a handle to the global snap reader if
 *                          successful, NULL otherwise.
 */
typedef OTF2_GlobalSnapReader*
( *OTF2_FP_GetGlobalSnapReader )( void* archive );


/** @internal
 *  @brief Get a marker reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *
 *  @return                 Returns a handle to the marker reader if
 *                          successful, NULL otherwise.
 */
typedef OTF2_MarkerReader*
( *OTF2_FP_GetMarkerReader )( void* archive );


/** @internal
 *  @brief Get a marker writer.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *
 *  @return                 Returns a handle to the marker writer if
 *                          successful, NULL otherwise.
 */
typedef OTF2_MarkerWriter*
( *OTF2_FP_GetMarkerWriter )( void* archive );


/** @internal
 *  @brief Get version information.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param major            Return pointer to major version number.
 *  @param minor            Return pointer to minor version number.
 *  @param bugfix           Return pointer to bugifx revision number.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetVersion )( void*    archive,
                         uint8_t* major,
                         uint8_t* minor,
                         uint8_t* bugfix );


/** @internal
 *  @brief Get chunk sizes.
 *
 *  @param archive              Archive handle according to the reader substrate.
 *  @param chunkSizeEvents      Return pointer to chunk size used to store events.
 *  @param chunkSizeDefinitions Return pointer to chunk size used to store definitions.
 *
 *  @return                     Returns OTF2_SUCCESS if successful, an error code
 *                              if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetChunkSize )( void*     archive,
                           uint64_t* chunkSizeEvents,
                           uint64_t* chunkSizeDefinitions );


/** @internal
 *  @brief Get file substrate.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param substrate        Return pointer to file substrate.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetFileSubstrate )( void*               archive,
                               OTF2_FileSubstrate* substrate );


/** @internal
 *  @brief Get compression mode.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param compression      Return pointer to compression mode.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetCompression )( void*             archive,
                             OTF2_Compression* compression );


/** @internal
 *  @brief Get number of locations.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param locations        Return pointer to the number of locations.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetNumLocations )( void*     archive,
                              uint64_t* locations );


/** @internal
 *  @brief Get number of global definitions.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param definitions      Return pointer to the number of global definitions.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetNumGlobalDefs )( void*     archive,
                               uint64_t* definitions );


/** @internal
 *  @brief Get machine name.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param machineName      Return pointer to machine name.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetMachineName )( void*  archive,
                             char** machineName );


/** @internal
 *  @brief Get creator name.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param creator          Return pointer to creator name.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetCreator )( void*  archive,
                         char** creator );


/** @internal
 *  @brief Get description.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param description      Return pointer to description.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetDescription )( void*  archive,
                             char** description );


/** @internal
 *  @brief Get the names of all trace file properties.
 *
 *  @param archive              Archive handle according to the reader substrate.
 *  @param numberOfProperties   Number of trace file properties.
 *  @param names                Pointer to a string where the property name will be stored, will be malloc'ed.
 *                              Just pass *names to free.
 *
 *  @return                     Returns OTF2_SUCCESS if successful, an error code
 *                              if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetPropertyNames )( void*     archive,
                               uint32_t* numberOfProperties,
                               char***   names );


/** @internal
 *  @brief Get the value of the named trace file property.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param name             Name of the property.
 *  @param value            Pointer to a string where the value will be stored, will be malloc'ed.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetProperty )( void*       archive,
                          const char* name,
                          char**      value );


/** @internal
 *  @brief Get the value trace identifier.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param id               Trace identifier.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetTraceId )( void*     archive,
                         uint64_t* id );


/** @internal
 *  @brief Close a local event reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param evtReader        Event reader.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseEvtReader )( void*           archive,
                             OTF2_EvtReader* evtReader );


/** @internal
 *  @brief Closes the global event reader.
 *
 *  This closes also all local event readers.
 *
 *  @param reader           Valid reader handle.
 *  @param globalEvtReader  The global event reader.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseGlobalEvtReader )( OTF2_Reader*          reader,
                                   OTF2_GlobalEvtReader* globalEvtReader );


/** @internal
 *  @brief Close a local definition reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param defReader        Definition reader.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseDefReader )( void*           archive,
                             OTF2_DefReader* defReader );


/** @internal
 *  @brief Closes the global definition reader.
 *
 *  @param reader           Valid reader handle.
 *  @param globalDefReader  The global definition reader.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseGlobalDefReader )( OTF2_Reader*          reader,
                                   OTF2_GlobalDefReader* globalDefReader );


/** @internal
 *  @brief Close a local snapshot reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param snapReader       Snapshot reader.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseSnapReader )( void*            archive,
                              OTF2_SnapReader* snapReader );


/** @internal
 *  @brief Closes the global snapshot reader.
 *
 *  This closes also all local snapshot readers.
 *
 *  @param reader           Valid reader handle.
 *  @param globalSnapReader The global snapshot reader.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseGlobalSnapReader )( OTF2_Reader*           reader,
                                    OTF2_GlobalSnapReader* globalSnapReader );


/** @internal
 *  @brief Get number of snapshots.
 *
 *  @param reader           Valid reader handle.
 *  @param number           return number.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetNumberOfSnapshots )( OTF2_Reader* reader,
                                   uint32_t*    number );


/** @internal
 *  @brief Get number of thumbnails.
 *
 *  @param reader           Valid reader handle.
 *  @param number           return number.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_GetNumberOfThumbnails )( OTF2_Reader* reader,
                                    uint32_t*    number );

/** @internal
 *  @brief Get a thumb reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param number           Thumb number.
 *
 *  @return                 Returns a handle to the thumbnail reader if
 *                          successful, NULL otherwise.
 */
typedef OTF2_ThumbReader*
( *OTF2_FP_GetThumbReader )( OTF2_Archive* archive,
                             uint32_t      number );

/** @internal
 *  @brief Close a thumb reader.
 *
 *  @param archive          Archive handle according to the reader substrate.
 *  @param thumbReader      Thumb reader.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseThumbReader )( void*             archive,
                               OTF2_ThumbReader* thumbReader );


/** @internal
 *  @brief Closes the marker reader.
 *
 *  @param reader           Valid reader handle.
 *  @param markerReader     The marker reader.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseMarkerReader )( OTF2_Reader*       reader,
                                OTF2_MarkerReader* markerReader );


/** @internal
 *  @brief Closes the marker writer.
 *
 *  @param reader           Valid reader handle.
 *  @param markerReader     The marker writer.
 *
 *  @return                 Returns OTF2_SUCCESS if successful, an error code
 *                          if an error occurs.
 */
typedef OTF2_ErrorCode
( *OTF2_FP_CloseMarkerWriter )( OTF2_Reader*       reader,
                                OTF2_MarkerWriter* markerWriter );


typedef struct otf2_reader_archive_impl_struct
{
    /** @name Function pointer to the actual reader operations according to the
     *  reader substrate.
     *  @{ */
    OTF2_FP_DeleteArchive               delete_archive;
    OTF2_FP_SetFileSionCallbacks        set_file_sion_callbacks;
    OTF2_FP_RegisterEvtCallbacks        register_evt_callbacks;
    OTF2_FP_RegisterGlobalEvtCallbacks  register_global_evt_callbacks;
    OTF2_FP_RegisterDefCallbacks        register_def_callbacks;
    OTF2_FP_RegisterGlobalDefCallbacks  register_global_def_callbacks;
    OTF2_FP_RegisterSnapCallbacks       register_snap_callbacks;
    OTF2_FP_RegisterGlobalSnapCallbacks register_global_snap_callbacks;
    OTF2_FP_RegisterMarkerCallbacks     register_marker_callbacks;
    OTF2_FP_ReadLocalEvents             read_local_events;
    OTF2_FP_ReadGlobalEvent             read_global_event;
    OTF2_FP_HasGlobalEvent              has_global_event;
    OTF2_FP_ReadGlobalEvents            read_global_events;
    OTF2_FP_ReadLocalEvents             read_local_events_backward;
    OTF2_FP_ReadLocalDefinitions        read_local_definitions;
    OTF2_FP_ReadGlobalDefinitions       read_global_definitions;
    OTF2_FP_ReadLocalSnapshots          read_local_snapshots;
    OTF2_FP_ReadGlobalSnapshots         read_global_snapshots;
    OTF2_FP_ReadMarkers                 read_markers;
    OTF2_FP_GetEvtReader                get_evt_reader;
    OTF2_FP_GetDefReader                get_def_reader;
    OTF2_FP_GetGlobalEvtReader          get_global_evt_reader;
    OTF2_FP_GetGlobalDefReader          get_global_def_reader;
    OTF2_FP_GetSnapReader               get_snap_reader;
    OTF2_FP_GetGlobalSnapReader         get_global_snap_reader;
    OTF2_FP_GetThumbReader              get_thumb_reader;
    OTF2_FP_GetMarkerReader             get_marker_reader;
    OTF2_FP_GetMarkerWriter             get_marker_writer;
    OTF2_FP_GetVersion                  get_version;
    OTF2_FP_GetChunkSize                get_chunk_size;
    OTF2_FP_GetFileSubstrate            get_file_substrate;
    OTF2_FP_GetCompression              get_compression;
    OTF2_FP_GetNumLocations             get_number_of_locations;
    OTF2_FP_GetNumGlobalDefs            get_number_of_global_defs;
    OTF2_FP_GetMachineName              get_machine_name;
    OTF2_FP_GetCreator                  get_creator;
    OTF2_FP_GetDescription              get_description;
    OTF2_FP_GetPropertyNames            get_property_names;
    OTF2_FP_GetProperty                 get_property;
    OTF2_FP_GetTraceId                  get_trace_id;
    OTF2_FP_GetNumberOfSnapshots        get_number_of_snapshots;
    OTF2_FP_GetNumberOfThumbnails       get_number_of_thumbnails;
    OTF2_FP_CloseEvtReader              close_evt_reader;
    OTF2_FP_CloseGlobalEvtReader        close_global_evt_reader;
    OTF2_FP_CloseDefReader              close_def_reader;
    OTF2_FP_CloseGlobalDefReader        close_global_def_reader;
    OTF2_FP_CloseSnapReader             close_snap_reader;
    OTF2_FP_CloseGlobalSnapReader       close_global_snap_reader;
    OTF2_FP_CloseThumbReader            close_thumb_reader;
    OTF2_FP_CloseMarkerReader           close_marker_reader;
    OTF2_FP_CloseMarkerWriter           close_marker_writer;
    /** @} */
} otf2_reader_archive_impl;


#endif /* OTF2_INTERNAL_READER_H */
