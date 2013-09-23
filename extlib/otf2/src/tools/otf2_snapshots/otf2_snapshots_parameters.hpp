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

#ifndef OTF2_SNAPSHOTS_PARAMETERS_H
#define OTF2_SNAPSHOTS_PARAMETERS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief number of samples for function activity thumbnails */
const uint32_t OTF2_SNAPSHOTS_NUMBER_SAMPLES = 4096;


/** @brief Keeps all data defined by the command line parameters. */
typedef struct
{
    /** @brief Path (dirname) of the trace file */
    char*    archive_path;
    /** @brief Name (basename) of the trace file*/
    char*    archive_name;
    /** @brief Number of snapshots */
    uint64_t number_breaks;
    /** @brief Create break every 'tick_rate_for_breaks' ticks */
    uint64_t tick_rate_for_breaks;
    /** @brief Print progress report */
    bool     progress;
    /** @brief Verbose mode */
    bool     verbose;
} otf2_snapshots_param;

extern void
get_parameters( int                   argc,
                char**                argv,
                otf2_snapshots_param* param );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OTF2_SNAPSHOTS_PARAMETERS_H */
