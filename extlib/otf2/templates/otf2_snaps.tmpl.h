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


#ifndef OTF2_INTERNAL_SNAPS_H
#define OTF2_INTERNAL_SNAPS_H


/**
 *  @file       src/otf2_snaps.h
 *  @source     templates/otf2_snaps.tmpl.h
 *
 *  @brief      This file list all snap records supported in OTF2.
 */


typedef uint8_t OTF2_SnapType;


enum OTF2_SnapType_enum
{
    /* First five entries are reserved for OTF2 internals */

    /* Snapshots */
    @otf2 for snap in snaps:
    OTF2_SNAP_@@snap.upper@@ = @@snap.id@@, /** snap record identifier for the @@snap.snap_name@@ record. */
    @otf2 endfor

    OTF2_SNAP_MAX_ID
};

@otf2 for snap in snaps:

/** @brief @@snap.name@@ snapshot record.
 */
typedef struct OTF2_@@snap.snap_name@@_struct
{
    /** Timestamp, which is exactly here in every record */
    OTF2_TimeStamp time;
    @@snap.structargs(attributes=snap.record_attributes, case='lower')@@
} OTF2_@@snap.snap_name@@;

@otf2 endfor

typedef struct OTF2_GenericSnap_struct OTF2_GenericSnap;


struct OTF2_GenericSnap_struct
{
    OTF2_SnapType type;
    union {
        OTF2_TimeStamp time;   /**< Timestamp, which is exactly here in every record */
        @otf2 for snap in snaps:
        OTF2_@@snap.snap_name@@ @@snap.lower@@;
        @otf2 endfor
    } record;
};


struct OTF2_SnapReaderCallbacks_struct
{
    OTF2_SnapReaderCallback_Unknown unknown;
    @otf2 for snap in snaps:
    OTF2_SnapReaderCallback_@@snap.name@@ @@snap.lower@@;
    @otf2 endfor
};


struct OTF2_GlobalSnapReaderCallbacks_struct
{
    OTF2_GlobalSnapReaderCallback_Unknown unknown;
    @otf2 for snap in snaps:
    OTF2_GlobalSnapReaderCallback_@@snap.name@@ @@snap.lower@@;
    @otf2 endfor
};

#endif /* OTF2_INTERNAL_SNAPS_H */
