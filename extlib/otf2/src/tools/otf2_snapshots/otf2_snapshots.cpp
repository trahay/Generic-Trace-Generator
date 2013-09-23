/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2013,
 * Technische Universitaet Dresden, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 */

/**
 *  @file       otf2_snapshots.cpp
 *
 *  @brief      This tool reads all event files and generates snapshot files,
 *              which will be used to load the trace partially.
 *
 *  Usage: otf2-snapshots [OPTION]... ANCHORFILE \n
 *  Append snapshots to existing otf2 traces at given
 *  'break' timestamps.
 *
 *  Options:
 *    -h, --help            Print this help information.
 *    -n, --number BREAKS   Number of breaks (distributed regularly);
 *                          if -p and -t are not set, the default for -n is 10
 *                          breaks.
 *    -p TICK_RATE          Create break every 'TICK_RATE' ticks;
 *                          (if both, -n and -p are specified the one producing
 *                          more breaks wins)
 *        --verbose         Verbose mode, print break timestamps, i.e. snapshot \n" );
 *                          informations to stdout.\n" );
 *    -V, --version         Print version information.
 *
 *  @param argc             Programs argument counter.
 *  @param argv             Programs argument values.
 *
 *  @return                 Returns EXIT_SUCCESS if successful, EXIT_FAILURE
 *                          if an error occures.
 */

#include <config.h>

#define __STDC_FORMAT_MACROS    // for PRIu8 in inttypes.h
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <UTILS_Error.h>

#include <otf2/otf2.h>

#include "otf2_snapshots_parameters.hpp"
#include "otf2_snapshots.hpp"
#include "otf2_snapshots_callbacks.hpp"


using namespace std;


const uint32_t OTF2_SNAPSHOTS_DEFAULT_NUMBER_BREAKS = 10;


/** @brief Initialize all member of the otf2_snapshots_data struct.
 *
 *  @param user_data Self-defined user data.
 */
static void
initialize_otf2_snapshots_data( otf2_snapshots_data* user_data )
{
    user_data->trace_length           = 0;
    user_data->global_offset          = 0;
    user_data->timer_resolution       = OTF2_UNDEFINED_UINT64;
    user_data->number_breaks          = OTF2_UNDEFINED_UINT64;
    user_data->tick_rate_for_breaks   = OTF2_UNDEFINED_UINT64;
    user_data->tick_barrier_for_break = OTF2_UNDEFINED_UINT64;
    user_data->number_events          = 0;
    user_data->param                  = NULL;
    user_data->sampling_counter       = 0;
}

/** @brief Deallocate everthing within the otf2_snapshots_data struct.
 *
 *  @param user_data Self-defined user data.
 */
static void
clean_up( otf2_snapshots_data** user_data )
{
    otf2_snapshots_data* data = *user_data;

    /* Delete locations event stack */
    for ( vector<uint64_t>::iterator it_loc = data->locations.begin();
          it_loc != data->locations.end();
          it_loc++ )
    {
        for ( list<otf2_snapshots_event*>::iterator it_evt = data->locations_event_stack[ *it_loc ].begin();
              it_evt != data->locations_event_stack[ *it_loc ].end();
              it_evt++ )
        {
            delete *it_evt;
        }
    }

    /* Delete metric members */
    for ( map<OTF2_MetricRef, OTF2_MetricMemberRef*>:: iterator it = data->metric_members.begin();
          it != data->metric_members.end();
          it++ )
    {
        delete[] it->second;
    }

    /* Delete parameters struct. */
    delete[] data->param->archive_path;

    delete data->param;

    /* delete user data */
    delete data;
}

static OTF2_FlushType
pre_flush( void*         userData,
           OTF2_FileType fileType,
           uint64_t      locationId,
           void*         callerData,
           bool          final )
{
    return OTF2_FLUSH;
}

static OTF2_FlushCallbacks flush_callbacks =
{
    pre_flush,
    0
};


int
main( int    argc,
      char** argv )
{
    OTF2_ErrorCode status = OTF2_SUCCESS;

    otf2_snapshots_param* param = new otf2_snapshots_param;
    UTILS_ASSERT( param );

    /* Process the user defined options for this run. */
    get_parameters( argc, argv, param );

    otf2_snapshots_data* user_data = new otf2_snapshots_data;
    UTILS_ASSERT( user_data );

    initialize_otf2_snapshots_data( user_data );
    user_data->param = param;

    if ( param->verbose )
    {
        printf( "\n=== OTF2 Snapshots ===\n" );
    }

    /* Get an archive handle. */
    OTF2_Archive* archive = OTF2_Archive_Open( param->archive_path,
                                               param->archive_name,
                                               OTF2_FILEMODE_READ,
                                               OTF2_UNDEFINED_UINT64,
                                               OTF2_UNDEFINED_UINT64,
                                               OTF2_SUBSTRATE_UNDEFINED,
                                               OTF2_COMPRESSION_UNDEFINED );
    UTILS_ASSERT( archive );

    user_data->archive = archive;


    /* ___ Read Global Definitions _____________________________________*/

    /* set definition callbacks. */

    OTF2_GlobalDefReaderCallbacks* global_def_callbacks = otf2_snapshots_get_global_def_callbacks();
    UTILS_ASSERT( global_def_callbacks );

    /* read all definitions. */
    OTF2_GlobalDefReader* glob_def_reader = OTF2_Archive_GetGlobalDefReader( archive );
    UTILS_ASSERT( glob_def_reader );

    OTF2_GlobalDefReader_SetCallbacks( glob_def_reader, global_def_callbacks, user_data );

    OTF2_GlobalDefReaderCallbacks_Delete( global_def_callbacks );

    uint64_t definitions_read = 0;
    status = OTF2_GlobalDefReader_ReadDefinitions( glob_def_reader, OTF2_UNDEFINED_UINT64, &definitions_read );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Unable to read global definitions!" );
    }


    /* ___ Compute number of snapshots to write, tick_rate and tick barrier for breaks. _____*/

    uint64_t number_breaks        = OTF2_SNAPSHOTS_DEFAULT_NUMBER_BREAKS + 1;  /* +1 for better calculation in the next steps, will be changed back afterwards. */
    uint64_t tick_rate_for_breaks = OTF2_UNDEFINED_UINT64;

    /* If -n is set uses this number of break points instead of the default. */
    if ( param->number_breaks != OTF2_UNDEFINED_UINT64 )
    {
        number_breaks = param->number_breaks + 1; /* +1, see explanation above. */

        /* If -n and -p is set uses the one from which you get the largest number of breaks. */
        if ( param->tick_rate_for_breaks != OTF2_UNDEFINED_UINT64 &&
             ( uint64_t )ceil( user_data->trace_length / ( double )param->tick_rate_for_breaks ) > number_breaks )
        {
            number_breaks        = ( uint64_t )floor( user_data->trace_length / ( double )param->tick_rate_for_breaks );
            tick_rate_for_breaks = param->tick_rate_for_breaks;
        }
        else
        {
            if ( 1 != number_breaks ) /* if the user requests 0 breaks. */
            {
                tick_rate_for_breaks = ( uint64_t )floor( user_data->trace_length / ( double )number_breaks );
            }
        }
    }
    else if ( param->tick_rate_for_breaks != OTF2_UNDEFINED_UINT64 )
    {
        number_breaks        = ( uint64_t )floor( user_data->trace_length / ( double )param->tick_rate_for_breaks );
        tick_rate_for_breaks = param->tick_rate_for_breaks;
    }
    else
    {
        tick_rate_for_breaks = ( uint64_t )floor( user_data->trace_length / ( double )number_breaks );
    }
    number_breaks--; /* -1, changed back, see explanation above. */

    user_data->number_breaks          = 0;
    user_data->tick_rate_for_breaks   = ( tick_rate_for_breaks > 0 ) ? tick_rate_for_breaks : 1;
    user_data->tick_barrier_for_break = user_data->global_offset + user_data->tick_rate_for_breaks;


    /* ___ Prepare function activity sampling ________________________*/

    /* calculate tick rate and barrier for function activity sampling */

    user_data->tick_rate_for_samples   = user_data->trace_length / ( OTF2_SNAPSHOTS_NUMBER_SAMPLES + 1 );
    user_data->tick_barrier_for_sample = user_data->global_offset + user_data->tick_rate_for_samples;
    if ( user_data->trace_length < OTF2_SNAPSHOTS_NUMBER_SAMPLES )
    {
        user_data->tick_rate_for_samples   = 1;
        user_data->tick_barrier_for_sample = user_data->global_offset + 1;
    }
    /* allocate memory for region activity sampling */
    user_data->samples.resize( user_data->region_defs.size(), 0 );


    /* ___ Read Event Records ________________________________________*/

    OTF2_GlobalEvtReaderCallbacks* global_event_callbacks = otf2_snapshots_get_event_callbacks();
    UTILS_ASSERT( global_event_callbacks );

    /* Get global event reader. */
    OTF2_GlobalEvtReader* glob_evt_reader = OTF2_Archive_GetGlobalEvtReader( archive );
    UTILS_ASSERT( glob_evt_reader );

    status = OTF2_GlobalEvtReader_SetCallbacks( glob_evt_reader, global_event_callbacks, user_data );

    OTF2_GlobalEvtReaderCallbacks_Delete( global_event_callbacks );

    /* switch archive to write mode to be able to get snap and thumb writers */
    OTF2_Archive_SwitchFileMode( archive, OTF2_FILEMODE_WRITE );
    OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER );
    OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );

    /* get region activity thumbnail writer */
    user_data->thumb_writer = OTF2_Archive_GetThumbWriter( archive,
                                                           "region activity",
                                                           "",
                                                           OTF2_THUMBNAIL_TYPE_REGION,
                                                           OTF2_SNAPSHOTS_NUMBER_SAMPLES,
                                                           user_data->region_defs.size(),
                                                           &*user_data->region_defs.begin() );
    UTILS_ASSERT( user_data->thumb_writer );


    uint64_t progress_dots = 50;
    if ( param->progress )
    {
        printf( "=== Progress ===\n" );
        printf( "===10===20===30===40===50===60===70===80===90==100\n" );
    }

    /* Calulate the number of events to read in #progress_dots phases, so that
     * we get the progress dots. */
    uint64_t events_to_read = ( uint64_t )floor( user_data->number_events / ( double )( progress_dots + 1 ) );

    /* Read a defined number of events with the global event reader and
     * progress information if necessary. */
    uint64_t events_read = 0;
    for ( size_t i = 0; i < progress_dots; ++i )
    {
        status = OTF2_GlobalEvtReader_ReadEvents( glob_evt_reader, events_to_read, &events_read );
        if ( status != OTF2_SUCCESS )
        {
            UTILS_ERROR( status, "Unable to read with global event reader!" );
        }

        if ( param->progress && i < progress_dots - 1 )
        {
            printf( "." );
        }
        else if ( param->progress && i == progress_dots - 1 )
        {
            printf( ".\n" );
        }
    }

    /* Read the remaining events to write remaining samples. */
    events_to_read = user_data->number_events - events_to_read * ( progress_dots );

    status = OTF2_GlobalEvtReader_ReadEvents( glob_evt_reader, events_to_read, &events_read );
    if ( status != OTF2_SUCCESS )
    {
        UTILS_ERROR( status, "Unable to read with global event reader!" );
    }

    /* write still missing zero-activity samples */

    user_data->samples.assign( user_data->samples.size(), 0 );
    while ( user_data->sampling_counter < OTF2_SNAPSHOTS_NUMBER_SAMPLES )
    {
        OTF2_ThumbWriter_WriteSample( user_data->thumb_writer,
                                      user_data->locations.size(),
                                      user_data->samples.size(),
                                      &*user_data->samples.begin() );

        user_data->sampling_counter++;
    }

    /* set number of snapshots in archive */
    OTF2_Archive_SetNumberOfSnapshots( archive, user_data->number_breaks );

    OTF2_Archive_Close( archive );

    clean_up( &user_data );

    return EXIT_SUCCESS;
}
