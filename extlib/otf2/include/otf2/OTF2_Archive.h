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


#ifndef OTF2_ARCHIVE_H
#define OTF2_ARCHIVE_H


/**
 *  @file       include/otf2/OTF2_Archive.h
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Writing interface for OTF2 archives.
 *
 *  @page usage_writing_page Usage in writing mode
 *
 *  @section usage_writing Usage in writing mode - a simple example
 *
 *  This is a short example of how to use the OTF2 writing interface.
 *
 *  First include the OTF2 header.
 *
 *  @code
 *  #include <otf2/otf2.h>
 *  @endcode
 *
 *  For this example an additional include statement is necessary.
 *
 *  @code
 *  #include <stdlib.h>
 *  @endcode
 *
 *  Furthermore this example uses a function delivering dummy timestamps.
 *  Real world applications will use a timer like gettimeofday.
 *
 *  @code
 *  OTF2_TimeStamp get_time( void )
 *  {
 *      static uint64_t sequence;
 *      return sequence++;
 *  }
 *  @endcode
 *
 *  Define a pre and post flush callback.
 *  If no memory is left in OTF2's internal memory buffer or the writer handle
 *  is closed a memory buffer flushing routine is triggered.
 *  The pre flush callback is triggered right before a buffer flush. It needs to
 *  return either OTF2_FLUSH to flush the recorded data to a file or
 *  OTF2_NO_FLUSH to supress flushing data to a file.
 *  The post flush callback is triggered right after a memory buffer flush. It
 *  has to return a current timestamp which is recorded to mark the time spend
 *  in a buffer flush.
 *
 *  @code
 *  OTF2_FlushType pre_flush( void*            userData,
 *                            OTF2_FileType    fileType,
 *                            OTF2_LocationRef location,
 *                            void*            callerData,
 *                            bool             final )
 *  {
 *      return OTF2_FLUSH;
 *  }
 *
 *  OTF2_TimeStamp post_flush( void*            userData,
 *                             OTF2_FileType    fileType,
 *                             OTF2_LocationRef location )
 *  {
 *      return get_time();
 *  }
 *
 *  OTF2_FlushCallbacks flush_callbacks =
 *  {
 *      .otf2_pre_flush  = pre_flush,
 *      .otf2_post_flush = post_flush
 *  };
 *  @endcode
 *
 *  @code
 *  int main( int argc, char** argv )
 *  {
 *  @endcode
 *
 *  Create new archive handle.
 *  @code
 *      OTF2_Archive* archive = OTF2_Archive_Open( "ArchivePath", "ArchiveName", OTF2_FILEMODE_WRITE, 1024 * 1024, 4 * 1024 * 1024, OTF2_SUBSTRATE_POSIX, OTF2_COMPRESSION_NONE );
 *  @endcode
 *
 *  Set the flush callbacks.
 *  @code
 *      OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
 *  @endcode
 *
 *  Define archive as master.
 *  @code
 *      OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
 *  @endcode
 *
 *  Get a local event writer and a local definition writer for location 0.
 *  Additionally a global definition writer is needed.
 *  @code
 *      OTF2_EvtWriter*       evt_writer        = OTF2_Archive_GetEvtWriter( archive, 0 );
 *      OTF2_DefWriter*       def_writer        = OTF2_Archive_GetDefWriter( archive, 0 );
 *      OTF2_GlobalDefWriter* global_def_writer = OTF2_Archive_GetGlobalDefWriter( archive );
 *  @endcode
 *
 *  Write an enter and a leave record for region 23 to the local event writer.
 *  @code
 *      OTF2_EvtWriter_Enter( evt_writer, NULL, get_time(), 23 );
 *      OTF2_EvtWriter_Leave( evt_writer, NULL, get_time(), 23 );
 *  @endcode
 *
 *  Write definitions for the strings as the first records to the global definition writer.
 *  @code
 *      OTF2_GlobalDefWriter_WriteString( global_def_writer, 0, "" );
 *      OTF2_GlobalDefWriter_WriteString( global_def_writer, 1, "Master Process" );
 *      OTF2_GlobalDefWriter_WriteString( global_def_writer, 2, "Main Thread" );
 *      OTF2_GlobalDefWriter_WriteString( global_def_writer, 3, "MyFunction" );
 *      OTF2_GlobalDefWriter_WriteString( global_def_writer, 4, "Alternative function name (e.g. mangled one)" );
 *      OTF2_GlobalDefWriter_WriteString( global_def_writer, 5, "Computes something" );
 *      OTF2_GlobalDefWriter_WriteString( global_def_writer, 6, "MyHost" );
 *      OTF2_GlobalDefWriter_WriteString( global_def_writer, 7, "node" );
 *  @endcode
 *
 *  Write definition for the code region which was just entered and left to the
 *  global definition writer.
 *  @code
 *      OTF2_GlobalDefWriter_WriteRegion( global_def_writer, 23, 3, 4, 5, OTF2_REGION_ROLE_FUNCTION, OTF2_PARADIGM_USER, OTF2_REGION_FLAG_NONE, 0, 0, 0 );
 *  @endcode
 *
 *  Write the system tree including a definition for the location group to the
 *  global definition writer.
 *  @code
 *      OTF2_GlobalDefWriter_WriteSystemTreeNode( global_def_writer, 0, 6, 7, OTF2_UNDEFINED_SYSTEM_TREE_NODE );
 *      OTF2_GlobalDefWriter_WriteLocationGroup( global_def_writer, 0, 1, OTF2_LOCATION_GROUP_TYPE_PROCESS, 0 );
 *  @endcode
 *
 *  Write a definition for the location to the global definition writer.
 *  @code
 *      OTF2_GlobalDefWriter_WriteLocation( global_def_writer, 0, 2, OTF2_LOCATION_TYPE_CPU_THREAD, 2, 0 );
 *  @endcode
 *
 *  At the end, close the archive and exit. All opened event and definition
 *  writers are closed automatically and the according files are created.
 *  @code
 *      OTF2_Archive_Close( archive );
 *
 *      return EXIT_SUCCESS;
 *  }
 *  @endcode
 *
 *  To compile your program use a command like:
 *  @code
 *  gcc `otf2-config --cflags` -c otf2_writer_example.c -o otf2_writer_example.o
 *  @endcode
 *
 *  Now you can link your program with:
 *  @code
 *  gcc otf2_writer_example.o `otf2-config --ldflags` `otf2-config --libs` -o otf2_writer_example
 *  @endcode
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Callbacks.h>
#include <otf2/OTF2_DefWriter.h>
#include <otf2/OTF2_DefReader.h>
#include <otf2/OTF2_EvtWriter.h>
#include <otf2/OTF2_EvtReader.h>
#include <otf2/OTF2_SnapWriter.h>
#include <otf2/OTF2_SnapReader.h>
#include <otf2/OTF2_GlobalDefWriter.h>
#include <otf2/OTF2_GlobalDefReader.h>
#include <otf2/OTF2_GlobalEvtReader.h>
#include <otf2/OTF2_GlobalSnapReader.h>
#include <otf2/OTF2_Thumbnail.h>
#include <otf2/OTF2_MarkerWriter.h>
#include <otf2/OTF2_MarkerReader.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Default size for OTF2's internal event chunk memory handling.
 *
 *  If you are not sure which chunk size is the best to use, use this default
 *  value.
 */
#define OTF2_CHUNK_SIZE_EVENTS_DEFAULT ( 1024 * 1024 )


/** @brief Default size for OTF2's internal event chunk memory handling.
 *
 *  If you are not sure which chunk size is the best to use, use this default
 *  value.
 */
#define OTF2_CHUNK_SIZE_DEFINITIONS_DEFAULT ( 4 * 1024 * 1024 )


/** @brief Keeps all meta-data for an OTF2 archive.
 *
 *  An OTF2 archive handle keeps all runtime information about an OTF2 archive.
 *  It is the central handle to get and set information about the archive and to
 *  request event and definition writer handles. */
typedef struct OTF2_Archive_struct OTF2_Archive;


/** @brief Defines whether a location is master or slave.
 *
 *  The master of creates the directory layout and writes the anchor file.
 *  Therefore, only one archive handle can be the master, e.g. the MPI
 *  rank 0. All other archive handles must be defined as slaves.
 *
 *  Please see OTF2_MasterSlaveMode_enum for a description of available values.
 */
typedef uint8_t OTF2_MasterSlaveMode;


/** @brief Defines whether a location is master or slave.
 */
enum OTF2_MasterSlaveMode_enum
{
    /** @brief Location is slave. */
    OTF2_SLAVE  = 0,
    /** @brief Location is master. */
    OTF2_MASTER = 1
};


/** @brief Create a new archive.
 *
 *  Creates a new archive handle that keeps all meta data about the archive on
 *  runtime.
 *
 *  @param archivePath      Path to the archive i.e. the directory where the
 *                          anchor file is located.
 *  @param archiveName      Name of the archive. It is used to generate sub
 *                          pathes e.g. 'archiveName.otf2'.
 *  @param fileMode         Determines if in reading or writing mode.
 *                          Available values are @eref{OTF2_FILEMODE_WRITE} or
 *                          @eref{OTF2_FILEMODE_READ}.
 *  @param chunkSizeEvents  Requested size of OTF2's internal event chunks in
 *                          writing mode. Available values are from 256kB to
 *                          16MB. The event chunk size affects performance as
 *                          well as total memory usage.
 *                          A value satisfying both is about 1MB.
 *                          If you are not sure which chunk size is the best to
 *                          use, use @eref{OTF2_CHUNK_SIZE_EVENTS_DEFAULT}.
 *                          In reading mode this value is ignored because the
 *                          correct chunk size is extracted from the anchor
 *                          file.
 *  @param chunkSizeDefs    Requested size of OTF2's internal definition chunks
 *                          in writing mode. Available values are from 256kB to
 *                          16MB. The definition chunk size affects performance
 *                          as well as total memory usage. In addition, the
 *                          definition chunk size must be big enough to carry
 *                          the largest possible definition record. Therefore,
 *                          the definition chunk size must be at least 10 times
 *                          the number of locations.
 *                          A value satisfying these requirements is about 4MB.
 *                          If you are not sure which chunk size is the best to
 *                          use, use @eref{OTF2_CHUNK_SIZE_DEFINITIONS_DEFAULT}.
 *                          In reading mode this value is ignored because the
 *                          correct chunk size is extracted from the anchor
 *                          file.
 *  @param fileSubstrate    Determines which file substrate should be used in
 *                          writing mode.
 *                          Available values are @eref{OTF2_SUBSTRATE_POSIX} to use the
 *                          standard Posix interface, @eref{OTF2_SUBSTRATE_SION} to use
 *                          an installed SION library to store multiple logical
 *                          files into fewer or one physical file, and
 *                          @eref{OTF2_SUBSTRATE_NONE} to supress file writing at all.
 *                          In reading mode this value is ignored because the
 *                          correct file substrated is extracted from the anchor
 *                          file.
 *  @param compression      Determines if compression is used to reduce the size
 *                          of data in files.
 *                          Available values are @eref{OTF2_COMPRESSION_ZLIB}
 *                          to use an installed zlib and
 *                          @eref{OTF2_COMPRESSION_NONE} to disable compression.
 *                          In reading mode this value is ignored because the
 *                          correct file compression is extracted from the
 *                          anchor file.
 *
 *  @return                 Returns an archive handle if successful, NULL
 *                          otherwise.
 */
OTF2_Archive*
OTF2_Archive_Open( const char*              archivePath,
                   const char*              archiveName,
                   const OTF2_FileMode      fileMode,
                   const uint64_t           chunkSizeEvents,
                   const uint64_t           chunkSizeDefs,
                   const OTF2_FileSubstrate fileSubstrate,
                   const OTF2_Compression   compression );


/** @brief Close an opened archive.
 *
 *  Closes an opened archive and releases the associated resources. Closes also
 *  all opened writer and reader handles.
 *  Does nothing if NULL is passed.
 *
 *  @param archive          Archive handle.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_Close( OTF2_Archive* archive );


/** @brief Switch file mode of the archive.
 *
 *  Currently only a switch from @eref{OTF2_FILEMODE_READ} to
 *  @eref{OTF2_FILEMODE_WRITE} is permitted and in this case, the master/slave
 *  mode is reset and must be set again with @eref{OTF2_Archive_SetMasterSlaveMode}.
 *  Currrently it is also only permitted when operating on an OTF2 archive
 *  with the @eref{OTF2_SUBSTRATE_POSIX} file substrate.
 *
 *  @param archive          Archive handle.
 *  @param newFileMode      New @eref{OTF2_FileMode} to switch to.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Archive_SwitchFileMode( OTF2_Archive* archive,
                             OTF2_FileMode newFileMode );


/** @brief Set master slave mode.
 *
 *  Sets master slave mode for a location. If OTF2_MASTER is passed, the
 *  location creates the directory structure for the trace files to store.
 *  Therefore, exactly one location can be master, all other locations must be
 *  slaves.
 *
 *  Please note: This call is only allowed in writing mode.
 *
 *  @param archive          Archive handle.
 *  @param masterOrSlave    Master or slave. Available values are @eref{OTF2_MASTER}
 *                          and @eref{OTF2_SLAVE}.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_SetMasterSlaveMode( OTF2_Archive*        archive,
                                 OTF2_MasterSlaveMode masterOrSlave );


/** @brief Set machine name.
 *
 *  Sets the name for the machine the trace was recorded.
 *  This value is optional. It only needs to be set for an archive handle marked
 *  as 'master' or does not need to be set at all.
 *
 *  @param archive          Archive handle.
 *  @param machineName      Machine name.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_SetMachineName( OTF2_Archive* archive,
                             const char*   machineName );


/** @brief Set a description.
 *
 *  Sets a description for a trace archive.
 *  This value is optional. It only needs to be set for an archive handle marked
 *  as 'master' or does not need to be set at all.
 *
 *  @param archive          Archive handle.
 *  @param description      Description.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_SetDescription( OTF2_Archive* archive,
                             const char*   description );


/** @brief Set creator.
 *
 *  Sets information about the creator of the trace archive.
 *  This value is optional. It only needs to be set for an archive handle marked
 *  as 'master' or does not need to be set at all.
 *
 *  @param archive          Archive handle.
 *  @param creator          Creator information.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_SetCreator( OTF2_Archive* archive,
                         const char*   creator );


/** @brief Set the flush callbacks for the archive.
 *
 *  @param archive          Archive handle.
 *  @param flushCallbacks   Struct holding the flush callback functions.
 *  @param flushData        Data passed to the flush callbacks in the
 *                          @p userData argument.
 *
 *  @return OTF2_ErrorCode, or error code.
 */
OTF2_ErrorCode
OTF2_Archive_SetFlushCallbacks( OTF2_Archive*              archive,
                                const OTF2_FlushCallbacks* flushCallbacks,
                                void*                      flushData );


/** @brief Set the memory callbacks for the archive.
 *
 *  @param archive          Archive handle.
 *  @param memoryCallbacks  Struct holding the memory callback functions.
 *  @param memoryData       Data passed to the memory callbacks in the
 *                          @p userData argument.
 *
 *  @return OTF2_ErrorCode, or error code.
 */
OTF2_ErrorCode
OTF2_Archive_SetMemoryCallbacks( OTF2_Archive*               archive,
                                 const OTF2_MemoryCallbacks* memoryCallbacks,
                                 void*                       memoryData );


/** @brief Set the SION callbacks for the archive.
 *
 *  @param archive           Archive handle.
 *  @param fileSionCallbacks Struct holding the SION callback functions.
 *  @param fileSionData      Data passed to the SION callbacks in the
 *                           @p userData argument.
 *
 *  @return OTF2_ErrorCode, or error code.
 */
OTF2_ErrorCode
OTF2_Archive_SetFileSionCallbacks( OTF2_Archive*                 archive,
                                   const OTF2_FileSionCallbacks* fileSionCallbacks,
                                   void*                         fileSionData );


/** @brief Add or remove a trace file property to this archive.
 *
 *  Removing a trace file property is done by passing "" in the @p value parameter.
 *  The @p overwrite parameter is ignored than.
 *
 *  @note This call is only allowed when the archive was opened with mode
 *        @eref{OTF2_FILEMODE_WRITE}.
 *
 *  @param archive          Archive handle.
 *  @param name             Name of the trace file property (case insensitive, [A-Z0-9_]).
 *  @param value            Value of property.
 *  @param overwrite        If true a previous trace file property with the same
 *                          name @p name will be overwritten.
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_PROPERTY_NAME_INVALID,
 *             if property name does not conform to the naming scheme}
 *    @retcode{OTF2_ERROR_PROPERTY_NOT_FOUND,
 *             if property was not found\, but requested to remove}
 *    @retcode{OTF2_ERROR_PROPERTY_EXISTS,
 *             if property exists but overwrite was not set}
 *  @retend
 */
OTF2_ErrorCode
OTF2_Archive_SetProperty( OTF2_Archive* archive,
                          const char*   name,
                          const char*   value,
                          bool          overwrite );


/** @brief Add or remove a boolean trace file property to this archive.
 *
 *  @note This call is only allowed when the archive was opened with mode
 *        @eref{OTF2_FILEMODE_WRITE}.
 *
 *  @param archive          Archive handle.
 *  @param name             Name of the trace file property (case insensitive, [A-Z0-9_]).
 *  @param value            Boolean value of property (e.g. true or false).
 *  @param overwrite        If true a previous trace file property with the same
 *                          name @p name will be overwritten.
 *
 *  @retbegin
 *    @retcode{OTF2_SUCCESS, if successful}
 *    @retcode{OTF2_ERROR_PROPERTY_NAME_INVALID,
 *             if property name does not conform to the naming scheme}
 *    @retcode{OTF2_ERROR_PROPERTY_NOT_FOUND,
 *             if property was not found\, but requested to remove}
 *    @retcode{OTF2_ERROR_PROPERTY_EXISTS,
 *             if property exists but overwrite was not set}
 *  @retend
 */
OTF2_ErrorCode
OTF2_Archive_SetBoolProperty( OTF2_Archive* archive,
                              const char*   name,
                              bool          value,
                              bool          overwrite );


/** @brief Get the names of all trace file properties.
 *
 *  @param archive                 Archive handle.
 *  @param[out] numberOfProperties Returned number of trace file properties.
 *  @param[out] names              Returned list of property names. Allocated
 *                                 with @a malloc. To release memory, just pass
 *                                 @p *names to @a free.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetPropertyNames( OTF2_Archive* archive,
                               uint32_t*     numberOfProperties,
                               char***       names );


/** @brief Get the value of the named trace file property.
 *
 *  @param archive          Archive handle.
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
OTF2_Archive_GetProperty( OTF2_Archive* archive,
                          const char*   name,
                          char**        value );


/** @brief Get the identifier of the trace file.
 *
 *  @note This call is only allowed when the archive was opened with mode
 *        OTF2_FILEMODE_READ.
 *
 *  @param archive          Archive handle.
 *  @param[out] id          Trace identifier.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetTraceId( OTF2_Archive* archive,
                         uint64_t*     id );


/** @brief Get master slave mode.
 *
 *  @param archive            Archive handle.
 *  @param[out] masterOrSlave Return pointer to the master slave mode.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetMasterSlaveMode( OTF2_Archive*         archive,
                                 OTF2_MasterSlaveMode* masterOrSlave );


/** @brief Get the number of locations.
 *
 *  @param archive                Archive handle.
 *  @param[out] numberOfLocations Return pointer to the number of locations.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetNumberOfLocations( OTF2_Archive* archive,
                                   uint64_t*     numberOfLocations );


/** @brief Get the number of global definitions.
 *
 *  @param archive                  Archive handle.
 *  @param[out] numberOfDefinitions Return pointer to the number of global definitions.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetNumberOfGlobalDefinitions( OTF2_Archive* archive,
                                           uint64_t*     numberOfDefinitions );


/** @brief Get machine name.
 *
 *  @param archive          Archive handle.
 *  @param[out] machineName Returned machine name. Allocated with @a malloc.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetMachineName( OTF2_Archive* archive,
                             char**        machineName );


/** @brief Get description.
 *
 *  @param archive          Archive handle.
 *  @param[out] description Returned description. Allocated with @a malloc.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetDescription( OTF2_Archive* archive,
                             char**        description );


/** @brief Get creator information.
 *
 *  @param archive          Archive handle.
 *  @param[out] creator     Returned creator. Allocated with @a malloc.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetCreator( OTF2_Archive* archive,
                         char**        creator );


/** @brief Get format version.
 *
 *  @param archive          Archive handle
 *  @param[out] major       Major version number
 *  @param[out] minor       Minor version number
 *  @param[out] bugfix      Bugfix revision
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetVersion( OTF2_Archive* archive,
                         uint8_t*      major,
                         uint8_t*      minor,
                         uint8_t*      bugfix );


/** @brief Get the chunksize.
 *
 *  @param archive              Archive handle.
 *  @param[out] chunkSizeEvents Chunk size for event files.
 *  @param[out] chunkSizeDefs   Chunk size for definition files.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetChunkSize( OTF2_Archive* archive,
                           uint64_t*     chunkSizeEvents,
                           uint64_t*     chunkSizeDefs );


/** @brief Get the file substrate (posix, sion, none)
 *
 *  @param archive          Archive handle.
 *  @param[out] substrate   Returned file substrate.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetFileSubstrate( OTF2_Archive*       archive,
                               OTF2_FileSubstrate* substrate );


/** @brief Get compression mode (none or zlib)
 *
 *  @param archive          Archive handle.
 *  @param[out] compression Returned compression mode.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_GetCompression( OTF2_Archive*     archive,
                             OTF2_Compression* compression );


/** @brief Get a local event writer.
 *
 *  @param archive          Archive handle.
 *  @param location         Location ID of the requested writer handle.
 *
 *  @return                 Returns a local event writer handle if successful,
 *                          NULL if an error occurs.
 */
OTF2_EvtWriter*
OTF2_Archive_GetEvtWriter( OTF2_Archive*    archive,
                           OTF2_LocationRef location );


/** @brief Get a local definition writer.
 *
 *  @param archive          Archive handle.
 *  @param location         Location ID of the requested writer handle.
 *
 *  @return                 Returns a local definition writer handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_DefWriter*
OTF2_Archive_GetDefWriter( OTF2_Archive*    archive,
                           OTF2_LocationRef location );


/** @brief Get a global definition writer.
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns a global definition writer handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_GlobalDefWriter*
OTF2_Archive_GetGlobalDefWriter( OTF2_Archive* archive );


/** @brief Get a local snap writer.
 *
 *  @param archive          Archive handle.
 *  @param location         Location ID of the requested writer handle.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a local event writer handle if successful,
 *                          NULL if an error occurs.
 */
OTF2_SnapWriter*
OTF2_Archive_GetSnapWriter( OTF2_Archive*    archive,
                            OTF2_LocationRef location );


/** @brief Get a thumb writer.
 *
 *  @param archive          Archive handle.
 *
 *  @param name             Name of thumb.
 *  @param description      Description of thumb.
 *  @param type             Type of thumb.
 *  @param numberOfSamples  Number of samples.
 *  @param numberOfMetrics  Number of metrics.
 *  @param refsToDefs       @a numberOfMetrics references to defintion matching
 *                          the thumbnail type.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a thumb writer handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_ThumbWriter*
OTF2_Archive_GetThumbWriter( OTF2_Archive*      archive,
                             const char*        name,
                             const char*        description,
                             OTF2_ThumbnailType type,
                             uint32_t           numberOfSamples,
                             uint32_t           numberOfMetrics,
                             const uint64_t*    refsToDefs );


/** @brief Get a marker writer.
 *
 *  @param archive          Archive handle.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a marker writer handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_MarkerWriter*
OTF2_Archive_GetMarkerWriter( OTF2_Archive* archive );


/** @brief Get a local event reader.
 *
 *  @param archive          Archive handle.
 *  @param location         Location ID of the requested reader handle.
 *
 *  @return                 Returns a local event reader handle if successful,
 *                          NULL if an error occurs.
 */
OTF2_EvtReader*
OTF2_Archive_GetEvtReader( OTF2_Archive*    archive,
                           OTF2_LocationRef location );


/** @brief Get a global event reader.
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns a global event reader handle if successful,
 *                          NULL if an error occurs.
 */
OTF2_GlobalEvtReader*
OTF2_Archive_GetGlobalEvtReader( OTF2_Archive* archive );


/** @brief Get a local definition reader.
 *
 *  @param archive          Archive handle.
 *  @param location         Location ID of the requested reader handle.
 *
 *  @return                 Returns a local definition reader handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_DefReader*
OTF2_Archive_GetDefReader( OTF2_Archive*    archive,
                           OTF2_LocationRef location );


/** @brief Get a global definition reader.
 *
 *  @param archive          Archive handle.
 *
 *  @return                 Returns a global definition reader handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_GlobalDefReader*
OTF2_Archive_GetGlobalDefReader( OTF2_Archive* archive );


/** @brief Get a local snap reader.
 *
 *  @param archive          Archive handle.
 *  @param location         Location ID of the requested snap handle.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a local snap handle if successful,
 *                          NULL if an error occurs.
 */
OTF2_SnapReader*
OTF2_Archive_GetSnapReader( OTF2_Archive*    archive,
                            OTF2_LocationRef location );


/** @brief Get a global snap reader.
 *
 *  @param archive          Archive handle.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a global snap reader handle if successful,
 *                          NULL if an error occurs.
 */
OTF2_GlobalSnapReader*
OTF2_Archive_GetGlobalSnapReader( OTF2_Archive* archive );


/** @brief Get a thumb reader.
 *
 *  @param archive          Archive handle.
 *  @param number           Thumbnail number.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a global definition writer handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_ThumbReader*
OTF2_Archive_GetThumbReader( OTF2_Archive* archive,
                             uint32_t      number );


/** @brief Get a marker reader.
 *
 *  @param archive          Archive handle.
 *
 *  @since Version 1.2
 *
 *  @return                 Returns a marker reader handle if
 *                          successful, NULL if an error occurs.
 */
OTF2_MarkerReader*
OTF2_Archive_GetMarkerReader( OTF2_Archive* archive );


/** @brief Close an opened local event writer.
 *
 *  @param archive          Archive handle.
 *  @param writer           Writer handle to be closed.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseEvtWriter( OTF2_Archive*   archive,
                             OTF2_EvtWriter* writer );


/** @brief Close an opened local definition writer.
 *
 *  @param archive          Archive handle.
 *  @param writer           Writer handle to be closed.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseDefWriter( OTF2_Archive*   archive,
                             OTF2_DefWriter* writer );

/** @brief Close an opened marker writer.
 *
 *  @param archive          Archive handle.
 *  @param writer           Writer handle to be closed.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseMarkerWriter( OTF2_Archive*      archive,
                                OTF2_MarkerWriter* writer );


/** @brief Close an opened local snap writer.
 *
 *  @param archive          Archive handle.
 *  @param writer           Writer handle to be closed.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseSnapWriter( OTF2_Archive*    archive,
                              OTF2_SnapWriter* writer );


/** @brief Close an opened local event reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseEvtReader( OTF2_Archive*   archive,
                             OTF2_EvtReader* reader );


/** @brief Close an opened thumbnail reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseThumbReader( OTF2_Archive*     archive,
                               OTF2_ThumbReader* reader );

/** @brief Closes the global event reader.
 *
 *  This closes also all local event readers.
 *
 *  @param archive          Archive handle.
 *  @param globalEvtReader  The global event reader.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseGlobalEvtReader( OTF2_Archive*         archive,
                                   OTF2_GlobalEvtReader* globalEvtReader );


/** @brief Close an opened local definition reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseDefReader( OTF2_Archive*   archive,
                             OTF2_DefReader* reader );


/** @brief Closes the global definition reader.
 *
 *  @param archive          Archive handle.
 *  @param globalDefReader  The global definition reader.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseGlobalDefReader( OTF2_Archive*         archive,
                                   OTF2_GlobalDefReader* globalDefReader );


/** @brief Close an opened local snap reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 *  @since Version 1.2
 */
OTF2_ErrorCode
OTF2_Archive_CloseSnapReader( OTF2_Archive*    archive,
                              OTF2_SnapReader* reader );


/** @brief Close the opened global snapshot reader.
 *
 *  @param archive          Archive handle.
 *  @param reader           Reader handle to be closed.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 */
OTF2_ErrorCode
OTF2_Archive_CloseGlobalSnapReader( OTF2_Archive*          archive,
                                    OTF2_GlobalSnapReader* globalSnapReader );

/** @brief Closes the marker reader.
 *
 *  @param archive          Archive handle.
 *  @param markerReader     The marker reader.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_Archive_CloseMarkerReader( OTF2_Archive*      archive,
                                OTF2_MarkerReader* markerReader );


/** @brief Get the number of thumbnails.
 *
 *  @param archive          Archive handle.
 *  @param number           Thumb number.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 */
OTF2_ErrorCode
OTF2_Archive_GetNumberOfThumbnails( OTF2_Archive* archive,
                                    uint32_t*     number );

/** @brief Set the number of snapshots.
 *
 *  @param archive          Archive handle.
 *  @param number           Snapshot number.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 */
OTF2_ErrorCode
OTF2_Archive_SetNumberOfSnapshots( OTF2_Archive* archive,
                                   uint32_t      number );

/** @brief Get the number of snapshots.
 *
 *  @param archive          Archive handle.
 *  @param number           Snapshot number.
 *
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 *
 */
OTF2_ErrorCode
OTF2_Archive_GetNumberOfSnapshots( OTF2_Archive* archive,
                                   uint32_t*     number );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_ARCHIVE_H */
