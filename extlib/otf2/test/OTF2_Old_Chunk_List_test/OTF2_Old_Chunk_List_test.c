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

/**
 *
 * @file       test/OTF2_old_chunk_list_test/OTF2_old_chunk_list_test.c
 *
 * @author     Andre Groetzsch <andre.groetzsch@tu-dresden.de>
 *
 * @brief      testing the functionality of the old_chunk_list
 *
 */


#include <config.h>
#include <otf2/otf2.h>
#include <stdlib.h>
#include <string.h>



#define OTF2_REWIND_TEST_NUMBER_OF_CHUNKS 2

static bool allocate_chunk_called = false;

struct otf2_chunk_pool
{
    uint8_t chunks[ OTF2_REWIND_TEST_NUMBER_OF_CHUNKS ]
    [ OTF2_CHUNK_SIZE_MIN ];
    size_t  next_free;
};


static void*
allocate_chunk( void*         userData,
                OTF2_FileType fileType,
                uint64_t      locationId, // buffer->locationID
                void**        perBufferData,
                uint64_t      chunkSize )
{
    allocate_chunk_called = true;

    if ( !*perBufferData )
    {
        *perBufferData = calloc( 1, sizeof( struct otf2_chunk_pool ) );
    }
    struct otf2_chunk_pool* chunk_pool = *perBufferData;

    if ( chunk_pool->next_free == OTF2_REWIND_TEST_NUMBER_OF_CHUNKS )
    {
        printf( "no more chunks available!\n" );
        return NULL;
    }

    //printf("allocate_chunk: chunk pool %p, location %lu, switching to chunk %lu.\n",
    //		*perBufferData, locationId, chunk_pool->next_free + 1);

    return chunk_pool->chunks[ chunk_pool->next_free++ ];
}

static void
free_all_chunks( void*         userData,
                 OTF2_FileType fileType,
                 uint64_t      locationId,
                 void**        perBufferData,
                 bool          final )
{
    struct otf2_chunk_pool* chunk_pool = *perBufferData;

    chunk_pool->next_free = 0;

    if ( final )
    {
        free( chunk_pool );
        *perBufferData = NULL;
    }
}

static OTF2_MemoryCallbacks memory_callbacks =
{
    .otf2_allocate = allocate_chunk,
    .otf2_free_all = free_all_chunks
};

uint64_t
get_time( void )          // dummy timestamps
{
    static uint64_t sequence;
    return sequence++;
}


OTF2_FlushType
pre_flush( void*         userData,
           OTF2_FileType fileType,
           uint64_t      locationId,
           void*         callerData,
           bool          final )
{
    return OTF2_FLUSH;
}

OTF2_TimeStamp
post_flush( void*         userData,
            OTF2_FileType fileType,
            uint64_t      locationId )
{
    return get_time();
}

OTF2_FlushCallbacks flush_callbacks =
{
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
};


int
main( int argc, char** argv )
{
    const char* archivePath = "OTF2_OLD_CHUNK_LIST_TEST_PATH";
    const char* archiveName = "OTF2_OLD_CHUNK_LIST_TEST_ARCHIVE";

    OTF2_Archive* archive = OTF2_Archive_Open( archivePath, archiveName, OTF2_FILEMODE_WRITE,
                                               OTF2_CHUNK_SIZE_MIN, // chunkSizeEvents
                                               OTF2_CHUNK_SIZE_MIN, // chunkSizeDefs (assumed to be equal to chunkSizeEvents here!)
                                               OTF2_SUBSTRATE_POSIX,
                                               OTF2_COMPRESSION_NONE );

    OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
    OTF2_Archive_SetMemoryCallbacks( archive, &memory_callbacks, NULL );
    OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );

    OTF2_EvtWriter* evt_writer = OTF2_Archive_GetEvtWriter( archive, 0 );
    // -> new buffer for local_def_writer
    // -> allocate_chunk is called
    OTF2_DefWriter* def_writer = OTF2_Archive_GetDefWriter( archive, 0 );
    // -> new buffer for def_writer
    // -> allocate_chunk is called
    OTF2_GlobalDefWriter* global_def_writer = OTF2_Archive_GetGlobalDefWriter( archive );
    // -> new buffer for global_def_writer: location ID is OTF2_UNDEFINED_LOCATION
    // -> allocate_chunk is called

    OTF2_EvtWriter_StoreRewindPoint( evt_writer, 0 );

    /* writing events until a new chunk is used */
    allocate_chunk_called = false;
    int i = 0;
    do
    {
        OTF2_EvtWriter_Enter( evt_writer, NULL, get_time(), 1 );
        OTF2_EvtWriter_Leave( evt_writer, NULL, get_time(), 1 );
        i++;     // counting written events
    }
    while ( !allocate_chunk_called );


    OTF2_EvtWriter_Rewind( evt_writer, 0 );
    // (this will not cause enter/leave rewind events! )
    // -> now there should be an old chunk


    /* rewrite the same events as before to force the need of a second chunk again */
    OTF2_EvtWriter_StoreRewindPoint( evt_writer, 0 );
    allocate_chunk_called = false;
    int j;
    for ( j = 0; j < i; j++ )
    {
        OTF2_EvtWriter_Enter( evt_writer, NULL, get_time(), 1 );
        OTF2_EvtWriter_Leave( evt_writer, NULL, get_time(), 1 );
        j++;
    }

    /* evaluation */
    if ( allocate_chunk_called )
    {
        exit( EXIT_FAILURE );
    }


    OTF2_Archive_Close( archive );
    // -> new buffer for anchor file
    // -> allocate_chunk is called


    /* delete archive */
    char rm_command[ 128 ];
    strcpy( rm_command, "rm -rf " );
    strcat( rm_command, archivePath );
    int ret = system( rm_command );

    return EXIT_SUCCESS;
}
