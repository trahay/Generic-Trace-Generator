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
 *  @file       src/OTF2_GlobalSnapReader_inc.c
 *  @source     templates/OTF2_GlobalSnapReader_inc.tmpl.c
 *
 *  @brief      This module reads the global snapshot records.
 */



@otf2 for snap in snaps:

static inline OTF2_ErrorCode
otf2_global_snap_reader_trigger_@@snap.lower@@( const OTF2_GlobalSnapReader* reader,
                                                OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap          = &snap_reader->current_snap;
    OTF2_@@snap.snap_name@@* record = &snap->record.@@snap.lower@@;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.@@snap.lower@@ )
    {
        interrupt = reader->reader_callbacks.@@snap.lower@@( snap_reader->location_id,
                                                              record->time,
                                                              reader->user_data,
                                                              &snap_reader->attribute_list @@snap.callargs(case='lower', argprefix='record->')@@ );
    }

     /* Before the next record is read, the attribute list has to be cleaned up
      * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &snap_reader->attribute_list );

    @otf2 for attr in snap.all_array_attributes:
    free( record->@@attr.lower@@ );
    @otf2 endfor

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}

@otf2 endfor

static OTF2_ErrorCode
otf2_global_snap_reader_trigger_callback( const OTF2_GlobalSnapReader* reader,
                                          OTF2_SnapReader*             snap_reader )
{
    OTF2_GenericSnap* snap = &snap_reader->current_snap;

    switch ( snap->type )
    {
        /* An attribute list should not be in the record queue alone. */
        case OTF2_ATTRIBUTE_LIST:
            return OTF2_ERROR_INTEGRITY_FAULT;

        @otf2 for snap in snaps:
        case OTF2_SNAP_@@snap.upper@@:
            return otf2_global_snap_reader_trigger_@@snap.lower@@( reader,
                                                                   snap_reader );

        @otf2 endfor
        default:
            return otf2_global_snap_reader_trigger_unknown( reader,
                                                            snap_reader );
    }
}

@otf2 for snap in snaps:

OTF2_ErrorCode
OTF2_GlobalSnapReaderCallbacks_Set@@snap.name@@Callback(
    OTF2_GlobalSnapReaderCallbacks*              globalSnapReaderCallbacks,
    OTF2_GlobalSnapReaderCallback_@@snap.name@@ @@snap.lname@@Callback )
{
    if ( !globalSnapReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                             "Invalid globalSnapReaderCallback argument!" );
    }

    globalSnapReaderCallbacks->@@snap.lower@@ = @@snap.lname@@Callback;

    return OTF2_SUCCESS;
}

@otf2 endfor
