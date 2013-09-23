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


#ifndef OTF2_READER_H
#define OTF2_READER_H


/**
 *  @file       include/otf2/OTF2_Reader.h
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Reading interface for OTF2 archives.
 *
 *  @page usage_reading_page Usage in reading mode
 *
 *  @section usage_reading Usage in reading mode - a simple example
 *
 *  This is a short example of how to use the OTF2 reading interface. It shows
 *  hows to define and register callbacks and how to use the reader interface to
 *  read all events of a given OTF2 archive.
 *
 *  First include the OTF2 header.
 *
 *  @code
 *  #include <otf2/otf2.h>
 *  @endcode
 *
 *  For this example two additional include statements are necessary.
 *
 *  @code
 *  #include <stdlib.h>
 *  #include <string.h>
 *  #include <stdint.h>
 *  #include <inttypes.h>
 *  @endcode
 *
 *  Define an event callback for entering and leaving a region.
 *  @code
 *  OTF2_CallbackCode
 *  Enter_print( OTF2_LocationRef    location,
 *               OTF2_TimeStamp      time,
 *               void*               userData,
 *               OTF2_AttributeList* attributes,
 *               OTF2_RegionRef      region )
 *  {
 *      printf( "Entering region %u at location: %" PRIu64 " at time %" PRIu64 ".\n",
 *              region, location, time );
 *
 *      return OTF2_SUCCESS;
 *  }
 *
 *  OTF2_CallbackCode
 *  Leave_print( OTF2_LocationRef    location,
 *               OTF2_TimeStamp      time,
 *               void*               userData,
 *               OTF2_AttributeList* attributes,
 *               OTF2_RegionRef      region )
 *  {
 *      printf( "Leaving region %u at location: %" PRIu64 " at time %" PRIu64 ".\n",
 *              region, location, time );
 *
 *      return OTF2_SUCCESS;
 *  }
 *  @endcode
 *
 *  Define a definition callback that opens a new local event reader for each
 *  found location definition. The global event reader will use only events
 *  from opened local event readers. Therefore, if only a subset of locations
 *  should be read from, only for those locations a local event reader has to be
 *  opened.
 *  In addition, open a local definition reader, if there are local definitions
 *  present in the trace archive. Local definitions contain location specific
 *  definitions.
 *  Please note: Local definitions must be read in order to use automated
 *  identifierer translation. Otherwise, all delivered identifiers are invalid.
 *  @code
 *  OTF2_CallbackCode
 *  GlobDefLocation_Register( void*                 userData,
 *                            OTF2_LocationRef      location,
 *                            OTF2_StringRef        name,
 *                            OTF2_LocationType     locationType,
 *                            uint64_t              numberOfEvents,
 *                            OTF2_LocationGroupRef locationGroup )
 *  {
 *      OTF2_Reader* reader = ( OTF2_Reader* )userData;
 *      OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( reader, location );
 *
 *      OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( reader, location );
 *      uint64_t definitions_read = 0;
 *      OTF2_Reader_ReadAllLocalDefinitions( reader, def_reader, &definitions_read );
 *  }
 *  @endcode
 *
 *  @code
 *  int main( int argc, char** argv )
 *  {
 *  @endcode
 *
 *  Create a new reader handle. The path to the OTF2 anchor file must be
 *  provided as argument.
 *  @code
 *      OTF2_Reader* reader = OTF2_Reader_Open( "ArchivePath/ArchiveName.otf2" );
 *  @endcode
 *
 *  Get a global definition reader with the above reader handle as argument.
 *  @code
 *      OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
 *  @endcode
 *
 *  Register the above defined global definition callbacks. All other definition
 *  callbacks will be deactivated.
 *  @code
 *      OTF2_GlobalDefReaderCallbacks* global_def_callbacks = OTF2_GlobalDefReaderCallbacks_New();
 *      OTF2_GlobalDefReaderCallbacks_SetLocationCallback( global_def_callbacks, &GlobDefLocation_Register );
 *      OTF2_Reader_RegisterGlobalDefCallbacks( reader, global_def_reader, global_def_callbacks, reader );
 *      OTF2_GlobalDefReaderCallbacks_Delete( global_def_callbacks );
 *  @endcode
 *
 *  Read all global definitions. Everytime a location definition is read, the
 *  previosly registered callback is triggered.
 *  In @c definitions_read the number of read definitions is returned.
 *  @code
 *      uint64_t definitions_read = 0;
 *      OTF2_Reader_ReadAllGlobalDefinitions( reader, global_def_reader, &definitions_read );
 *  @endcode
 *
 *  Open a new global event reader. This global reader automatically contains
 *  all previously opened local event readers.
 *  @code
 *      OTF2_GlobalEvtReader* global_evt_reader = OTF2_Reader_GetGlobalEvtReader( reader );
 *  @endcode
 *
 *  Register the above defined global event callbacks. All other global event
 *  callbacks will be deactivated.
 *  @code
 *      OTF2_GlobalEvtReaderCallbacks* event_callbacks = OTF2_GlobalEvtReaderCallbacks_New();
 *      OTF2_GlobalEvtReaderCallbacks_SetEnterCallback( event_callbacks, &Enter_print );
 *      OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback( event_callbacks, &Leave_print );
 *
 *      OTF2_Reader_RegisterGlobalEvtCallbacks( reader, global_evt_reader, event_callbacks, NULL );
 *      OTF2_GlobalEvtReaderCallbacks_Delete( event_callbacks );
 *  @endcode
 *
 *  Read all events in the OTF2 archive. The events are automatically ordered by
 *  the time they occured in the trace. Everytime an enter or leave event is
 *  read, the previously registered callbacks are triggered.
 *  In @c events_read the number of read events is returned.
 *  @code
 *      uint64_t events_read = 0;
 *      OTF2_Reader_ReadAllGlobalEvents( reader, global_evt_reader, &events_read );
 *  @endcode
 *
 *  At the end, close the reader and exit. All opened event and definition
 *  readers are closed automatically.
 *  @code
 *      OTF2_Reader_Close( reader );
 *
 *      return EXIT_SUCCESS;
 *  }
 *  @endcode
 *
 *  To compile your program use a command like:
 *  @code
 *  gcc `otf2-config --cflags` -c otf2_reader_example.c -o otf2_reader_example.o
 *  @endcode
 *
 *  Now you can link your program with:
 *  @code
 *  gcc otf2_reader_example.o `otf2-config --ldflags` `otf2-config --libs` -o otf2_reader_example
 *  @endcode
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Archive.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Keeps all neccessary information for the reader. */
typedef struct OTF2_Reader_struct OTF2_Reader;


/** @brief Create a new reader handle.
 *
 *  Creates a new reader handle, opens an according archive handle, and calls a
 *  routine to register all neccessary function pointers.
 *
 *  @param anchorFilePath   Path to the anchor file e.g. 'trace.otf2'. This
 *                          can be a relative as well as an absolute path.
 *
 *  @return                 Returns a handle to the reader if successful,
 *                          NULL otherwise.
 */
OTF2_Reader*
OTF2_Reader_Open( const char* anchorFilePath );


/** @brief Close a reader handle.
 *
 *  Closes a reader handle and releases all associated handles. Does nothing if
 *  NULL is provided.
 *
 *  @param reader           Reader handle.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_Close( OTF2_Reader* reader );


/** @brief Register SION callbacks to the reader.
 *
 *  It suffice to provide a function for @a OTF2_FileSionGetRank. The neccessary
 *  information for the rank mapping can be extracted from the global group
 *  definition of type @a OTF2_GROUP_TYPE_MPI_LOCATIONS or by the @a
 *  locationGroupId attribute of the Location definitions.
 *
 *  @param reader            Reader handle.
 *  @param fileSionCallbacks Struct holding the callbacks.
 *  @param fileSionData      Pointer passed to the callbacks by the caller.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_SetFileSionCallbacks( OTF2_Reader*                  reader,
                                  const OTF2_FileSionCallbacks* fileSionCallbacks,
                                  void*                         fileSionData );


/** @brief Register event reader callbacks.
 *
 *  @param reader           OTF2_Reader handle.
 *  @param evtReader        Local event reader handle.
 *  @param callbacks        Callbacks for the event readers.
 *  @param userData         Addition user data.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_RegisterEvtCallbacks( OTF2_Reader*                   reader,
                                  OTF2_EvtReader*                evtReader,
                                  const OTF2_EvtReaderCallbacks* callbacks,
                                  void*                          userData );


/** @brief Register global event reader callbacks.
 *
 *  @param reader           OTF2_Reader handle.
 *  @param evtReader        Global event reader handle.
 *  @param callbacks        Callbacks for the global event reader.
 *  @param userData         Addition user data.
 *
 *  @return Returns @eref{OTF2_SUCCESS} if successful, an error code if an error
 *          occurs.
 */
OTF2_ErrorCode
OTF2_Reader_RegisterGlobalEvtCallbacks( OTF2_Reader*                         reader,
                                        OTF2_GlobalEvtReader*                evtReader,
                                        const OTF2_GlobalEvtReaderCallbacks* callbacks,
                                        void*                                userData );


/** @brief Register local definition reader callbacks.
 *
 *  @param reader           OTF2_Reader handle.
 *  @param defReader        Local definition reader handle.
 *  @param callbacks        Callbacks for the local definition readers.
 *  @param userData         Addition user data.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_RegisterDefCallbacks( OTF2_Reader*                   reader,
                                  OTF2_DefReader*                defReader,
                                  const OTF2_DefReaderCallbacks* callbacks,
                                  void*                          userData );


/** @brief Register global definition reader callbacks.
 *
 *  @param reader           OTF2_Reader handle.
 *  @param defReader        Global definition reader handle.
 *  @param callbacks        Callbacks for the global definition readers.
 *  @param userData         Addition user data.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_RegisterGlobalDefCallbacks( OTF2_Reader*                         reader,
                                        OTF2_GlobalDefReader*                defReader,
                                        const OTF2_GlobalDefReaderCallbacks* callbacks,
                                        void*                                userData );


/** @brief Register snapshot event reader callbacks.
 *
 *  @param reader           OTF2_Reader handle.
 *  @param snapReader       Local snap reader handle.
 *  @param callbacks        Callbacks for the event readers.
 *  @param userData         Addition user data.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_RegisterSnapCallbacks( OTF2_Reader*                    reader,
                                   OTF2_SnapReader*                snapReader,
                                   const OTF2_SnapReaderCallbacks* callbacks,
                                   void*                           userData );


/** @brief Register global event reader callbacks.
 *
 *  @param reader           OTF2_Reader handle.
 *  @param evtReader        Global event reader handle.
 *  @param callbacks        Callbacks for the global event reader.
 *  @param userData         Addition user data.
 *
 *  @since Version 1.2
 *
 *  @return Returns @eref{OTF2_SUCCESS} if successful, an error code if an error
 *          occurs.
 */
OTF2_ErrorCode
OTF2_Reader_RegisterGlobalSnapCallbacks( OTF2_Reader*                          reader,
                                         OTF2_GlobalSnapReader*                evtReader,
                                         const OTF2_GlobalSnapReaderCallbacks* callbacks,
                                         void*                                 userData );


/** @brief Register marker reader callbacks.
 *
 *  @param reader           OTF2_Reader handle.
 *  @param markerReader     Marker reader handle.
 *  @param callbacks        Callbacks for the marker reader.
 *  @param userData         Addition user data.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_RegisterMarkerCallbacks( OTF2_Reader*                      reader,
                                     OTF2_MarkerReader*                markerReader,
                                     const OTF2_MarkerReaderCallbacks* callbacks,
                                     void*                             userData );


/** @brief Read a given number of events via a local event reader.
 *
 *  @param reader           Reader handle.
 *  @param evtReader        Local event reader handle.
 *  @param eventsToRead     Number events to be read.
 *  @param eventsRead       Return pointer to the number of events actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadLocalEvents( OTF2_Reader*    reader,
                             OTF2_EvtReader* evtReader,
                             uint64_t        eventsToRead,
                             uint64_t*       eventsRead );


/** @brief Read all events via a local event reader.
 *
 *  @param reader           Reader handle.
 *  @param evtReader        Local event reader handle.
 *  @param[out] eventsRead  Return pointer to the number of events actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadAllLocalEvents( OTF2_Reader*    reader,
                                OTF2_EvtReader* evtReader,
                                uint64_t*       eventsRead );


/** @brief Read a given number of events via a local event reader backwards.
 *
 *  @param reader           Reader handle.
 *  @param evtReader        Local event reader handle.
 *  @param eventsToRead     Number events to be read.
 *  @param[out] eventsRead  Return pointer to the number of events actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadLocalEventsBackward( OTF2_Reader*    reader,
                                     OTF2_EvtReader* evtReader,
                                     uint64_t        eventsToRead,
                                     uint64_t*       eventsRead );


/** @brief Read an event via a global event reader.
 *
 *  @param reader           Reader handle.
 *  @param evtReader        Global event reader handle.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadGlobalEvent( OTF2_Reader*          reader,
                             OTF2_GlobalEvtReader* evtReader );


/** @brief Has the global event reader at least one more event to deliver.
 *
 *  @param reader           Global event reader handle.
 *  @param evtReader        Global event reader handle.
 *  @param[out] flag        In case of success, the flag will be set to 1
 *                          when there is at least more more event to read.
 *                          To 0 if not. Otherwise the value is undefined.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_HasGlobalEvent( OTF2_Reader*          reader,
                            OTF2_GlobalEvtReader* evtReader,
                            int*                  flag );


/** @brief Read a given number of events via a global event reader.
 *
 *  @param reader           Reader handle.
 *  @param evtReader        Global event reader handle.
 *  @param eventsToRead     Number events to be read.
 *  @param[out] eventsRead  Return pointer to the number of events actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadGlobalEvents( OTF2_Reader*          reader,
                              OTF2_GlobalEvtReader* evtReader,
                              uint64_t              eventsToRead,
                              uint64_t*             eventsRead );


/** @brief Read all events via a global event reader.
 *
 *  @param reader           Reader handle.
 *  @param evtReader        Global event reader handle.
 *  @param[out] eventsRead  Return pointer to the number of events actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadAllGlobalEvents( OTF2_Reader*          reader,
                                 OTF2_GlobalEvtReader* evtReader,
                                 uint64_t*             eventsRead );


/** @brief Read a given number of definitions via a local definition reader.
 *
 *  @param reader               Reader handle.
 *  @param defReader            Local definition reader handle.
 *  @param definitionsToRead    Number definitions to be read.
 *  @param[out] definitionsRead Return pointer to the number of definitions
 *                              actually read.
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INTERRUPTED_BY_CALLBACK,
 *             if an user supplied callback returned OTF2_CALLBACK_INTERRUPT}
 *    @retcode{OTF2_ERROR_DUPLICATE_MAPPING_TABLE,
 *             if an duplicate mapping table definition was read}
 *    @retelse{the error code}
 *  @retend
 */
OTF2_ErrorCode
OTF2_Reader_ReadLocalDefinitions( OTF2_Reader*    reader,
                                  OTF2_DefReader* defReader,
                                  uint64_t        definitionsToRead,
                                  uint64_t*       definitionsRead );


/** @brief Read all definitions via a local definition reader.
 *
 *  @param reader               Reader handle.
 *  @param defReader            Local definition reader handle.
 *  @param[out] definitionsRead Return pointer to the number of definitions
 *                              actually read.
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_INTERRUPTED_BY_CALLBACK,
 *             if an user supplied callback returned OTF2_CALLBACK_INTERRUPT}
 *    @retcode{OTF2_ERROR_DUPLICATE_MAPPING_TABLE,
 *             if an duplicate mapping table definition was read}
 *    @retelse{the error code}
 *  @retend
 */
OTF2_ErrorCode
OTF2_Reader_ReadAllLocalDefinitions( OTF2_Reader*    reader,
                                     OTF2_DefReader* defReader,
                                     uint64_t*       definitionsRead );


/** @brief Read a given number of definitions via a global definition reader.
 *
 *  @param reader               Reader handle.
 *  @param defReader            Global definition reader handle.
 *  @param definitionsToRead    Number definitions to be read.
 *  @param[out] definitionsRead Return pointer to the number of definitions
 *                              actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadGlobalDefinitions( OTF2_Reader*          reader,
                                   OTF2_GlobalDefReader* defReader,
                                   uint64_t              definitionsToRead,
                                   uint64_t*             definitionsRead );


/** @brief Read all definitions via a global definition reader.
 *
 *  @param reader               Reader handle.
 *  @param defReader            Global definition reader handle.
 *  @param[out] definitionsRead Return pointer to the number of definitions
 *                              actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadAllGlobalDefinitions( OTF2_Reader*          reader,
                                      OTF2_GlobalDefReader* defReader,
                                      uint64_t*             definitionsRead );


/** @brief Read a given number of records via a local snapshot reader.
 *
 *  @param reader           Reader handle.
 *  @param snapReader       Local snapshot reader handle.
 *  @param recordsToRead    Number records to be read.
 *  @param recordsRead      Return pointer to the number of records actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Reader_ReadLocalSnapshots( OTF2_Reader*     reader,
                                OTF2_SnapReader* snapReader,
                                uint64_t         recordsToRead,
                                uint64_t*        recordsRead );


/** @brief Read all records via a local snapshot reader.
 *
 *  @param reader            Reader handle.
 *  @param snapReader        Local snapshot reader handle.
 *  @param[out] recordsRead  Return pointer to the number of records
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Reader_ReadAllLocalSnapshots( OTF2_Reader*     reader,
                                   OTF2_SnapReader* snapReader,
                                   uint64_t*        recordsRead );


/** @brief Read a given number of records via a global snapshot reader.
 *
 *  @param reader           Reader handle.
 *  @param snapReader       Global snapshot reader handle.
 *  @param recordsToRead    Number records to be read.
 *  @param[out] recordsRead Return pointer to the number of records
 *                          actually read.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Reader_ReadGlobalSnapshots( OTF2_Reader*           reader,
                                 OTF2_GlobalSnapReader* snapReader,
                                 uint64_t               recordsToRead,
                                 uint64_t*              recordsRead );


/** @brief Read all records via a global snapshot reader.
 *
 *  @param reader           Reader handle.
 *  @param snapReader       Global snapshot reader handle.
 *  @param[out] recordsRead Return pointer to the number of records
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Reader_ReadAllGlobalSnapshots( OTF2_Reader*           reader,
                                    OTF2_GlobalSnapReader* snapReader,
                                    uint64_t*              recordsRead );


/** @brief Read a given number of markers via a marker reader.
 *
 *  @param reader               Reader handle.
 *  @param markerReader         Marker reader handle.
 *  @param markersToRead        Number markers to be read.
 *  @param[out] markersRead     Return pointer to the number of markers
 *                              actually read.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadMarkers( OTF2_Reader*       reader,
                         OTF2_MarkerReader* markerReader,
                         uint64_t           markersToRead,
                         uint64_t*          markersRead );


/** @brief Read all markers via a marker reader.
 *
 *  @param reader               Reader handle.
 *  @param markerReader         Marker reader handle.
 *  @param[out] markersRead     Return pointer to the number of markers
 *                              actually read.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_ReadAllMarkers( OTF2_Reader*       reader,
                            OTF2_MarkerReader* markerReader,
                            uint64_t*          markersRead );


/** @brief Get a local event reader.
 *
 *  @param reader           Valid reader handle.
 *  @param location         Location ID for the requested local reader.
 *
 *  @return                 Returns a handle to the local event reader if
 *                          successful, NULL otherwise.
 */
OTF2_EvtReader*
OTF2_Reader_GetEvtReader( OTF2_Reader*     reader,
                          OTF2_LocationRef location );


/** @brief Get a global event reader.
 *
 *  @param reader           Valid reader handle.
 *
 *  @return                 Returns a handle to the global event reader if
 *                          successful, NULL otherwise.
 */
OTF2_GlobalEvtReader*
OTF2_Reader_GetGlobalEvtReader( OTF2_Reader* reader );


/** @brief Get a local definition reader.
 *
 *  @param reader           Valid reader handle.
 *  @param location         Location ID for the requested local reader.
 *
 *  @return                 Returns a handle to the local definition reader if
 *                          successful, NULL otherwise.
 */
OTF2_DefReader*
OTF2_Reader_GetDefReader( OTF2_Reader*     reader,
                          OTF2_LocationRef location );


/** @brief Get a global definition reader.
 *
 *  @param reader           Valid reader handle.
 *
 *  @return                 Returns a handle to the global definition reader if
 *                          successful, NULL otherwise.
 */
OTF2_GlobalDefReader*
OTF2_Reader_GetGlobalDefReader( OTF2_Reader* reader );


/** @brief Get a local snapshot reader.
 *
 *  @param reader           Valid reader handle.
 *  @param location         Location ID for the requested local reader.
 *
 *  @return                 Returns a handle to the local event reader if
 *                          successful, NULL otherwise.
 *  @since Version 1.2
 */
OTF2_SnapReader*
OTF2_Reader_GetSnapReader( OTF2_Reader*     reader,
                           OTF2_LocationRef location );


/** @brief Get a global snap reader.
 *
 *  @param reader           Valid reader handle.
 *
 *  @return                 Returns a handle to the global snap reader if
 *                          successful, NULL otherwise.
 *  @since Version 1.2
 */
OTF2_GlobalSnapReader*
OTF2_Reader_GetGlobalSnapReader( OTF2_Reader* reader );


/** @brief Get a thumb reader.
 *
 *  @param reader           Reader handle.
 *  @param number           Thumbnail number.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a global definition writer handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_ThumbReader*
OTF2_Reader_GetThumbReader( OTF2_Reader* reader,
                            uint32_t     number );


/** @brief Get a marker reader.
 *
 *  @param reader           Valid reader handle.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a handle to the marker reader if
 *                          successful, NULL otherwise.
 */
OTF2_MarkerReader*
OTF2_Reader_GetMarkerReader( OTF2_Reader* reader );


/** @brief Get a marker writer.
 *
 *  @param reader           Valid reader handle.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a handle to the marker writer if
 *                          successful, NULL otherwise.
 */
OTF2_MarkerWriter*
OTF2_Reader_GetMarkerWriter( OTF2_Reader* reader );


/** @brief Close a local event reader.
 *
 *  @param reader           Valid reader handle.
 *  @param evtReader        Event reader to be closed.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_CloseEvtReader( OTF2_Reader*    reader,
                            OTF2_EvtReader* evtReader );


/** @brief Closes the global event reader.
 *
 *  This closes also all local event readers.
 *
 *  @param reader           Valid reader handle.
 *  @param globalEvtReader  The global event reader.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_CloseGlobalEvtReader( OTF2_Reader*          reader,
                                  OTF2_GlobalEvtReader* globalEvtReader );


/** @brief Close a local definition reader.
 *
 *  @param reader           Valid reader handle.
 *  @param defReader        Definition reader to be closed.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_CloseDefReader( OTF2_Reader*    reader,
                            OTF2_DefReader* defReader );


/** @brief Closes the global definition reader.
 *
 *  @param reader           Valid reader handle.
 *  @param globalDefReader  The global definition reader.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_CloseGlobalDefReader( OTF2_Reader*          reader,
                                  OTF2_GlobalDefReader* globalDefReader );


/** @brief Close a local snapshot reader.
 *
 *  @param reader           Valid reader handle.
 *  @param snapReader       snapshot reader to be closed.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Reader_CloseSnapReader( OTF2_Reader*     reader,
                             OTF2_SnapReader* snapReader );


/** @brief Closes the global snapshot reader.
 *
 *  @param reader           Valid reader handle.
 *  @param globalSnapReader The global snapshot reader.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Reader_CloseGlobalSnapReader( OTF2_Reader*           reader,
                                   OTF2_GlobalSnapReader* globalSnapReader );


/** @brief Close an opened thumbnail reader.
 *
 *  @param reader           Reader handle.
 *  @param thumbReader      Thumbn reader handle to be closed.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_CloseThumbReader( OTF2_Reader*      reader,
                              OTF2_ThumbReader* thumbReader );


/** @brief Closes the marker reader.
 *
 *  @param reader           Valid reader handle.
 *  @param markerReader     The marker reader.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_CloseMarkerReader( OTF2_Reader*       reader,
                               OTF2_MarkerReader* markerReader );


/** @brief Closes the marker writer.
 *
 *  @param reader           Valid reader handle.
 *  @param markerWriter     The marker writer.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_CloseMarkerWriter( OTF2_Reader*       reader,
                               OTF2_MarkerWriter* markerWriter );


/** @brief Get OTF2 version.
 *
 *  @param reader           Valid reader handle.
 *  @param[out] major       Major version.
 *  @param[out] minor       Minor version.
 *  @param[out] bugfix      Bugfix revision.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetVersion( OTF2_Reader* reader,
                        uint8_t*     major,
                        uint8_t*     minor,
                        uint8_t*     bugfix );


/** @brief Get event and definition chunk sizes.
 *
 *  @param reader                    Reader handle.
 *  @param[out] chunkSizeEvents      Returned size of event chunks
 *  @param[out] chunkSizeDefinitions Returned size of definition chunks.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetChunkSize( OTF2_Reader* reader,
                          uint64_t*    chunkSizeEvents,
                          uint64_t*    chunkSizeDefinitions );


/** @brief Get file substrate information.
 *
 *  @param reader           Reader handle.
 *  @param[out] substrate   Returned file substrate.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetFileSubstrate( OTF2_Reader*        reader,
                              OTF2_FileSubstrate* substrate );


/** @brief Get copression mode.
 *
 *  @param reader           Reader handle.
 *  @param[out] compression Returned compression mode.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetCompression( OTF2_Reader*      reader,
                            OTF2_Compression* compression );


/** @brief Get number of locations.
 *
 *  @param reader                 Reader handle.
 *  @param[out] numberOfLocations Returned number of locations.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetNumberOfLocations( OTF2_Reader* reader,
                                  uint64_t*    numberOfLocations );


/** @brief Get number of global definitions.
 *
 *  @param reader                   Reader handle.
 *  @param[out] numberOfDefinitions Returned number of global definitions.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetNumberOfGlobalDefinitions( OTF2_Reader* reader,
                                          uint64_t*    numberOfDefinitions );


/** @brief Get machine name.
 *
 *  @param reader           Reader handle.
 *  @param[out] machineName Returned machine name. Allocated with @a malloc.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetMachineName( OTF2_Reader* reader,
                            char**       machineName );


/** @brief Get creator name.
 *
 *  @param reader           Reader handle.
 *  @param[out] creator     Returned creator. Allocated with @a malloc.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetCreator( OTF2_Reader* reader,
                        char**       creator );


/** @brief Get description.
 *
 *  @param reader           Reader handle.
 *  @param[out] description Returned description. Allocated with @a malloc.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetDescription( OTF2_Reader* reader,
                            char**       description );


/** @brief Get the names of all trace file properties.
 *
 *  @param reader                  Reader handle.
 *  @param[out] numberOfProperties Returned number of trace file properties.
 *  @param[out] names              Returned list of property names. Allocated
 *                                 with @a malloc. To release memory, just pass
 *                                 @p *names to @a free.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetPropertyNames( OTF2_Reader* reader,
                              uint32_t*    numberOfProperties,
                              char***      names );


/** @brief Get the value of the named trace file property.
 *
 *  @param reader           Reader handle.
 *  @param name             Name of the property.
 *  @param[out] value       Returned value of the property. Allocated with
 *                          @a malloc.
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_PROPERTY_NOT_FOUND,
 *             if the named property was not found}
 *  @retend
 */
OTF2_ErrorCode
OTF2_Reader_GetProperty( OTF2_Reader* reader,
                         const char*  name,
                         char**       value );


/** @brief Get the value of the named trace file property as boolean.
 *
 *  @param reader           Reader handle.
 *  @param name             Name of the property.
 *  @param[out] value       Returned boolean value of the property.
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_PROPERTY_NOT_FOUND,
 *             if the named property was not found}
 *    @retcode{OTF2_ERROR_PROPERTY_VALUE_INVALID,
 *             if the value could not be interpreted as an boolean value}
 *  @retend
 */
OTF2_ErrorCode
OTF2_Reader_GetBoolProperty( OTF2_Reader* reader,
                             const char*  name,
                             bool*        value );


/** @brief Get the identifier of the trace file.
 *
 *  @param reader           Reader handle.
 *  @param[out] id          Trace identifier.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Reader_GetTraceId( OTF2_Reader* reader,
                        uint64_t*    id );


/** @brief Get number of snapshots.
 *
 *  @param reader                   Reader handle.
 *  @param[out] number              Returned number of snapshots.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Reader_GetNumberOfSnapshots( OTF2_Reader* reader,
                                  uint32_t*    number );


/** @brief Get number of thumbs.
 *
 *  @param reader                   Reader handle.
 *  @param[out] number              Returned number of thumbs.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Reader_GetNumberOfThumbnails( OTF2_Reader* reader,
                                   uint32_t*    number );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_READER_H */
