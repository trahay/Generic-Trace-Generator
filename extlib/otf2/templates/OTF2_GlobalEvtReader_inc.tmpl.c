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


/**
 *  @file       src/OTF2_GlobalEvtReader_inc.c
 *  @source     templates/OTF2_GlobalEvtReader_inc.tmpl.c
 *
 *  @status     alpha
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This module reads the global events. This means that the
 *              programmer must first select a set of locations an register
 *              callbacks, and then gets a merged stream of records from
 *              these selected locations. Rudimentary filtering could be
 *              done by registering only callbacks for the needed record
 *              types. This is an external interface.
 */



@otf2 for event in events:

static inline OTF2_ErrorCode
otf2_global_evt_reader_trigger_@@event.lower@@( const OTF2_GlobalEvtReader* reader,
                                                OTF2_EvtReader*             evt_reader )
{
    OTF2_GenericEvent* event    = &evt_reader->current_event;
    OTF2_@@event.name@@* record = &event->record.@@event.lower@@;

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.@@event.lower@@ )
    {
        interrupt = reader->reader_callbacks.@@event.lower@@( evt_reader->location_id,
                                                              record->time,
                                                              reader->user_data,
                                                              &evt_reader->attribute_list @@event.callargs(case='lower', argprefix='record->')@@ );
    }
    @otf2 if event.alternative_event:
    else if ( reader->reader_callbacks.@@event.alternative_event.lower@@ )
    {
        OTF2_@@event.alternative_event.name@@ @@event.alternative_event.lower@@_record;
        bool conversion_successful
            = otf2_event_@@event.lower@@_convert_to_@@event.alternative_event.lower@@( record,
                                                                                       &evt_reader->attribute_list,
                                                                                       &@@event.alternative_event.lower@@_record );
        if ( conversion_successful )
        {
            interrupt = reader->reader_callbacks.@@event.alternative_event.lower@@( evt_reader->location_id,
                                                                                    record->time,
                                                                                    reader->user_data,
                                                                                    &evt_reader->attribute_list @@event.alternative_event.callargs(case='lower', argprefix=event.alternative_event.lower + '_record.')@@ );
        }
    }
    @otf2 endif

     /* Before the next event is read, the attribute list has to be cleaned up
      * i.e. remove all attributes from the list. */
    otf2_attribute_list_remove_all_attributes( &evt_reader->attribute_list );

    @otf2 for attr in event.all_array_attributes:
    free( record->@@attr.lower@@ );
    @otf2 endfor

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}

@otf2 endfor

static OTF2_ErrorCode
otf2_global_evt_reader_trigger_callback( const OTF2_GlobalEvtReader* reader,
                                         OTF2_EvtReader*             evt_reader )
{
    OTF2_GenericEvent* event = &evt_reader->current_event;

    switch ( event->type )
    {
        /* An attribute list should not be in the event queue alone. */
        case OTF2_ATTRIBUTE_LIST:
            return OTF2_ERROR_INTEGRITY_FAULT;

        @otf2 for event in events:
        case OTF2_EVENT_@@event.upper@@:
            return otf2_global_evt_reader_trigger_@@event.lower@@( reader,
                                                                   evt_reader );

        @otf2 endfor
        default:
            return otf2_global_evt_reader_trigger_unknown( reader,
                                                           evt_reader );
    }
}

@otf2 for event in events:

OTF2_ErrorCode
OTF2_GlobalEvtReaderCallbacks_Set@@event.name@@Callback(
    OTF2_GlobalEvtReaderCallbacks*              globalEvtReaderCallbacks,
    OTF2_GlobalEvtReaderCallback_@@event.name@@ @@event.lname@@Callback )
{
    if ( !globalEvtReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                             "Invalid globalEvtReaderCallback argument!" );
    }

    globalEvtReaderCallbacks->@@event.lower@@ = @@event.lname@@Callback;

    return OTF2_SUCCESS;
}

@otf2 endfor
