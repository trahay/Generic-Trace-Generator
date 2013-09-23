/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2011,
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

/** @internal
 *
 *  @status     alpha
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Writes a defined data set to a file, reads it back and compares.
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>

#include <otf2_internal.h>
#include <otf2_reader.h>
#include <otf2_archive.h>

#include <OTF2_File.h>

/* ___ Global variables. ____________________________________________________ */



/** @brief Defines if debug is turned on (1) or off (0). */
static bool otf2_DEBUG;



/* ___ Prototypes for static functions. _____________________________________ */



static OTF2_FlushType
pre_flush
(
    void*         userData,
    OTF2_FileType fileType,
    uint64_t      locationId,
    void*         callerData,
    bool          final
);

static OTF2_TimeStamp
post_flush
(
    void*         userData,
    OTF2_FileType fileType,
    uint64_t      locationId
);

static OTF2_FlushCallbacks flush_callbacks =
{
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
};

static uint64_t
get_time
(
    void
);

static inline void
check_pointer
(
    void* pointer,
    char* description,
    ...
);

static inline void
check_status
(
    OTF2_ErrorCode status,
    char*          description,
    ...
);

/* ___ main _________________________________________________________________ */



int
main( void )
{
    OTF2_ErrorCode status = OTF2_ERROR_INVALID;

    otf2_DEBUG = !!getenv( "OTF2_DEBUG_TESTS" );

    if ( otf2_DEBUG )
    {
        printf( "\n=== OTF2_FILE_TEST [DEBUG MODE] ===\n\n" );
    }
    else
    {
        printf( "\n=== OTF2_FILE_TEST ===\n\n" );
    }

    OTF2_Archive* archive = OTF2_Archive_Open( ".",
                                               "file-test",
                                               OTF2_FILEMODE_WRITE,
                                               1 * 1024 * 1024,
                                               4 * 1024 * 1024,
                                               OTF2_SUBSTRATE_POSIX,
                                               OTF2_COMPRESSION_NONE );
    check_pointer( archive, "Create writer archive handle" );

    status = OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
    check_status( status, "Set flush callbacks." );
    status = OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    check_status( status, "Set archive to master mode." );

    status = otf2_archive_set_number_of_locations( archive, 0 );
    check_status( status, "Set number of locations." );
    status = otf2_archive_set_number_of_global_defs( archive, 0 );
    check_status( status, "Set number of global definitions." );

    /* Open a new file. */
    OTF2_File* file_handle = OTF2_File_Open( archive,
                                             OTF2_FILEMODE_WRITE,
                                             OTF2_FILETYPE_GLOBAL_DEFS,
                                             OTF2_UNDEFINED_LOCATION );
    check_pointer( file_handle, "Open a new file." );


    /* Create a buffer and write it to a file. */
    uint8_t* buffer = ( uint8_t* )malloc( UINT8_MAX * sizeof( uint8_t ) );
    for ( uint8_t i = 0; i < UINT8_MAX; ++i )
    {
        buffer[ i ] = i;
    }

    status = OTF2_File_Write( file_handle, buffer, UINT8_MAX );
    check_status( status, "Write to file." );


    /* Close the file. */
    status = OTF2_File_Close( file_handle );
    check_status( status, "Close the file." );

    status = OTF2_Archive_Close( archive );
    check_status( status, "Close archive." );

    archive = OTF2_Archive_Open( ".",
                                 "file-test",
                                 OTF2_FILEMODE_READ,
                                 OTF2_UNDEFINED_UINT64,
                                 OTF2_UNDEFINED_UINT64,
                                 OTF2_SUBSTRATE_UNDEFINED,
                                 OTF2_COMPRESSION_UNDEFINED );
    check_pointer( archive, "Create reader archive handle" );

    /* Open file again. */
    file_handle = OTF2_File_Open( archive,
                                  OTF2_FILEMODE_READ,
                                  OTF2_FILETYPE_GLOBAL_DEFS,
                                  OTF2_UNDEFINED_LOCATION );
    check_pointer( file_handle, "Open file again." );

    /* Create a empty buffer and read to it from a file. */
    memset( buffer, '\0', UINT8_MAX );

    status = OTF2_File_Read( file_handle, buffer, UINT8_MAX );
    check_status( status, "Read from file." );

    /* Close file. */
    status = OTF2_File_Close( file_handle );
    check_status( status, "Close file." );

    /* Close archive */
    status = OTF2_Archive_Close( archive );
    check_status( status, "Close archive." );

    /* Create a buffer and write it to a file. */
    for ( uint8_t i = 0; i < UINT8_MAX; ++i )
    {
        if ( buffer[ i ] != i )
        {
            check_status( OTF2_ERROR_INTEGRITY_FAULT,
                          "Wrong value at %u: %u", i, buffer[ i ] );
        }
    }

    /* This is just to add a message to the debug output. */
    check_status( OTF2_SUCCESS, "Programm finished successfully." );

    return EXIT_SUCCESS;
}



/* ___ Implementation of static functions ___________________________________ */



/** @brief Pre flush callback.
 *
 *  @param evtWriter        Ignored.
 *  @param evtReader        Ignored.
 *
 *  @return                 Returns always OTF2_NO_FLUSH.
 */
static OTF2_FlushType
pre_flush
(
    void*         userData,
    OTF2_FileType fileType,
    uint64_t      locationId,
    void*         callerData,
    bool          final
)
{
    return OTF2_FLUSH;
}


static OTF2_TimeStamp
post_flush
(
    void*         userData,
    OTF2_FileType fileType,
    uint64_t      locationId
)
{
    return get_time();
}


/** @brief Get generic timestamp.
 *
 *  @return                 Returns consecutive timestamps 0,1,2 ...
 */
static inline uint64_t
get_time
(
    void
)
{
    static uint64_t sequence;
    return sequence++;
}


/** @brief Check if pointer is NULL.
 *
 *  Checks if a pointer is NULL. If so it prints an error with the passed
 *  description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param pointer          Pointer to be checked.
 *  @param description      Description for error/debug message.
 *  @param ...              Variable argument list like e.g. in printf.
 */
static inline void
check_pointer
(
    void* pointer,
    char* description,
    ...
)
{
    va_list va;
    va_start( va, description );

    if ( pointer == NULL )
    {
        printf( "==ERROR== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
        exit( EXIT_FAILURE );
    }

    if ( otf2_DEBUG )
    {
        printf( "==DEBUG== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
    }

    va_end( va );
}


/** @brief Check if status is not OTF2_SUCCESS.
 *
 *  Checks if status is not OTF2_SUCCESS. If so it prints an error with the
 *  passed description and exits the program.
 *  If in debug mode, it prints a debug message with the passed description.
 *  It is possible to passed a variable argument list like e.g. in printf.
 *
 *  @param pointer          Status to be checked.
 *  @param description      Description for error/debug message.
 *  @param ...              Variable argument list like e.g. in printf.
 */
static inline void
check_status
(
    OTF2_ErrorCode status,
    char*          description,
    ...
)
{
    va_list va;
    va_start( va, description );

    if ( status != OTF2_SUCCESS )
    {
        printf( "==ERROR== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
        exit( EXIT_FAILURE );
    }

    if ( otf2_DEBUG )
    {
        printf( "==DEBUG== " );
        vfprintf( stdout, description, va );
        printf( "\n" );
    }

    va_end( va );
}
