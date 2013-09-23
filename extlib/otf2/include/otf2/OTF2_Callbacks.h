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


#ifndef OTF2_CALLBACKS_H
#define OTF2_CALLBACKS_H


/**
 *  @file       include/otf2/OTF2_Callbacks.h
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This header file provides all user callbacks.
 */


#include <stdio.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Definition for the pre flush callback.
 *
 *  This callback is triggered right before flushing the recorded data into
 *  file when running out of memory.
 *
 *  @param userData    Data passed to the call @ref OTF2_Archive_SetFlushCallbacks.
 *  @param fileType    The type of file for what this buffer holds data.
 *  @param location    The location id for what this buffer holds data.
 *                     This is only valid for files of type
 *                     @eref{OTF2_FILETYPE_LOCAL_DEFS} or @eref{OTF2_FILETYPE_EVENTS}.
 *                     For other files this is @eref{OTF2_UNDEFINED_LOCATION}.
 *                     A special case exists for files of type @eref{OTF2_FILETYPE_EVENTS}
 *                     in writing mode. The location ID may still be
 *                     @eref{OTF2_UNDEFINED_LOCATION}. In this case if the application
 *                     wants to write the data from the buffer into the file,
 *                     the application needs to provide a valid location ID
 *                     via a call to @ref OTF2_EvtWriter_SetLocationID() and
 *                     utilizing the @p callerData argument.
 *  @param callerData  Depending of the fileType, this can be an @ref OTF2_EvtWriter,
 *                     @ref OTF2_GlobalDefWriter, @ref OTF2_DefWriter.
 *  @param final       Indicates whether this is the final flush when closing
 *                     the writer objects.
 *
 *  @return            Returns @eref{OTF2_FLUSH} or @eref{OTF2_NO_FLUSH}.
 */
typedef OTF2_FlushType
( *OTF2_PreFlushCallback )( void*            userData,
                            OTF2_FileType    fileType,
                            OTF2_LocationRef location,
                            void*            callerData,
                            bool             final );


/** @brief Definition for the post flush callback.
 *
 *  This callback is triggered right after flushing the recorded data into
 *  file when running out of memory. The main function of this callback is to
 *  provide a timestamp for the end of flushing data into a file. So an
 *  according record can be written correctly.
 *
 *  @param userData Data passed to the call @ref OTF2_Archive_SetFlushCallbacks.
 *  @param fileType The file type for which the flush has happened.
 *  @param location The location ID of the writer for which the flush has happened
 *                  (for file types without an ID this is
 *                  @eref{OTF2_UNDEFINED_LOCATION}).
 *
 *  @return Returns a timestamp for the end of flushing data into a file.
 */
typedef OTF2_TimeStamp
( *OTF2_PostFlushCallback )( void*            userData,
                             OTF2_FileType    fileType,
                             OTF2_LocationRef location );


/** @brief Structure holding the flush callbacks.
 *
 *  To be used in a call to @ref OTF2_Archive_SetFlushCallbacks.
 *
 *  otf2_post_flush callback may be NULL to suppress writing a
 *  BufferFlush record.
 */
typedef struct OTF2_FlushCallbacks
{
    /** @brief Callback which is called prior a flush. */
    OTF2_PreFlushCallback  otf2_pre_flush;
    /** @brief Callback which is called after a flush. */
    OTF2_PostFlushCallback otf2_post_flush;
} OTF2_FlushCallbacks;


/** @brief Function pointer for allocating memory for chunks.
 *
 *  Please note: Do not use this feature if you do not really understand it.
 *  The OTF2 library is not able to do any kind of checks to validate if your
 *  memory management works properly. If you do not use it correctly OTF2's
 *  behaviour is undefined including dead locks and all that nasty stuff.
 *
 *  This function must return a pointer to a valid allocated memory location
 *  (just like malloc). This memory location must be of exact same size as the
 *  parameter 'chunkSize' provided with OTF2_Archive_Open().
 *
 *  @param userData      Data passed to the call @ref OTF2_Archive_SetMemoryCallbacks.
 *  @param fileType      The file type for which the chunk is requested.
 *  @param location      The location ID of the writer for which the flush has
 *                       happened (for file types without an ID this is
 *                       @eref{OTF2_UNDEFINED_LOCATION}).
 *  @param perBufferData A writeable pointer to store callee data. For the first
 *                       call this will be @p NULL.
 *  @param chunkSize     The size of the requested chunk.
 *
 *  @return              Returns a the allocated memory on success, @p NULL if
 *                       an error occurs.
 */
typedef void*
( *OTF2_MemoryAllocate )( void*            userData,
                          OTF2_FileType    fileType,
                          OTF2_LocationRef location,
                          void**           perBufferData,
                          uint64_t         chunkSize );


/** @brief Function pointer to release all allocated chunks.
 *
 *  Please note: Do not use this feature if you do not really understand it.
 *  The OTF2 library is not able to do any kind of checks to validate if your
 *  memory management works properly. If you do not use it correctly OTF2's
 *  behaviour is undefined including dead locks and all that nasty stuff.
 *
 *  This function must free all those memory locations that were allocated for a
 *  buffer handle with the according allocate function. Please note: This is
 *  different from a posix free(). You must free _all_ memory locations for that
 *  were allocated for exactly this buffer handle.
 *
 *  @param userData      Data passed to the call @ref OTF2_Archive_SetMemoryCallbacks.
 *  @param fileType      The file type for which free is requested.
 *  @param location      The location ID of the writer for which the flush has
 *                       happened (for file types without an ID this is
 *                       @eref{OTF2_UNDEFINED_LOCATION}).
 *  @param perBufferData A writeable pointer to store callee data. For the first
 *                       call this will be @p NULL.
 *  @param final         Indicates whether this is the final free when closing
 *                       the writer objects. @p perBufferData should be handled
 *                       than.
 */
typedef void
( *OTF2_MemoryFreeAll )( void*            userData,
                         OTF2_FileType    fileType,
                         OTF2_LocationRef location,
                         void**           perBufferData,
                         bool             final );


/** @brief Structure holding the memory callbacks.
 *
 *  To be used in a call to @ref OTF2_Archive_SetMemoryCallbacks.
 */
typedef struct OTF2_MemoryCallbacks
{
    /** @brief Callback which is called to allocate a new chunck. */
    OTF2_MemoryAllocate otf2_allocate;
    /** @brief Callback which is called to release all previous allocated chunks. */
    OTF2_MemoryFreeAll  otf2_free_all;
} OTF2_MemoryCallbacks;


/** @brief Callbacks to wrap sion_paropen_mpi() for the OTF2 SION substrate.
 *         Every paramater that can be given by OTF2 is named equally like the
 *         the according paramater of sion_paropen_mpi(). Therfore, these given
 *         parameters MUST be given to SION.
 *
 *  @param userData          Data passed to the call @ref OTF2_Archive_SetFileSionCallbacks.
 *  @param fileType          The file type for which the file open is called.
 *  @param location          The location ID of the writer for which the flush
 *                           has happened (for file types without an ID this is
 *                           @eref{OTF2_UNDEFINED_LOCATION}).
 *  @param fname             Name of file, should equal on all tasks.
 *  @param fileMode          Like the type parameter of fopen.
 *  @param[in,out] chunkSize Requested space for this task.
 *  @param[in,out] fsblkSize Blocksize of filesystem, must be equal on all processors.
 *  @param[out] filePtr      Filepointer for this task.
 *
 *  @return sion file handle integer (0, ...) -1 if error occured
 */
typedef int
( *OTF2_FileSionOpen )( void*            userData,
                        OTF2_FileType    fileType,
                        OTF2_LocationRef location,
                        const char*      fname,
                        const char*      fileMode,
                        long long int*   chunkSize,
                        int*             fsblkSize,
                        FILE**           filePtr );


/** @brief Callbacks to wrap sion_parclose_mpi() for the OTF2 SION substrate.
 *
 *  @param userData  Data passed to the call @ref OTF2_Archive_SetFileSionCallbacks.
 *  @param fileType  The file type for which the file close is called.
 *  @param location  The location ID of the writer for which the flush has
 *                   happened (for file types without an ID this is
 *                   @eref{OTF2_UNDEFINED_LOCATION}).
 *  @param sid       Sion file handle.
 *
 *  @return Return value of sion_parclose_mpi()
 */
typedef int
( *OTF2_FileSionClose )( void*            userData,
                         OTF2_FileType    fileType,
                         OTF2_LocationRef location,
                         int              sid );


/** @brief Provides location->rank translation, when using the SION substrate.
 *
 * In case no OTF2_FileSionOpen and no OTF2_FileSionClose callback is given,
 * the SION substrate still needs information what rank the current location
 * has.
 *
 *  @param userData  Data passed to the call @ref OTF2_Archive_SetFileSionCallbacks.
 *  @param fileType  The file type for which the file close is called.
 *  @param location  The location ID of the writer for which the flush has
 *                   happened (for file types without an ID this is
 *                   @eref{OTF2_UNDEFINED_LOCATION}).
 *  @param[out] rank The associated MPI rank for the @p location.
 *
 *  @return @eref{OTF2_SUCCESS}, or error code.
 */
typedef OTF2_ErrorCode
( *OTF2_FileSionGetRank )( void*            userData,
                           OTF2_FileType    fileType,
                           OTF2_LocationRef location,
                           int32_t*         rank );


/** @brief Structure holding the SION callbacks.
 *
 *  To be used in a call to @ref OTF2_Archive_SetFileSionCallbacks.
 */
typedef struct OTF2_FileSionCallbacks
{
    /** @brief Callback which is called to open a SION file. */
    OTF2_FileSionOpen    otf2_file_sion_open;
    /** @brief Callback which is called to close a SION file. */
    OTF2_FileSionClose   otf2_file_sion_close;
    /** @brief Callback which is called to get the MPI rank in read mode. */
    OTF2_FileSionGetRank otf2_file_sion_get_rank;
} OTF2_FileSionCallbacks;


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_CALLBACKS_H */
