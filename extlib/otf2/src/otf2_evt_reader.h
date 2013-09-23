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

#ifndef OTF2_INTERNAL_EVT_READER_H
#define OTF2_INTERNAL_EVT_READER_H


/** @internal
 *  @brief A clock synchronization interval. */
typedef struct OTF2_ClockInterval_struct OTF2_ClockInterval;


/** @internal
 *  @brief A local reader is defined by following struct. */
struct OTF2_EvtReader_struct
{
    /** Archive handle */
    OTF2_Archive* archive;

    /** Location ID of the related location */
    uint64_t     location_id;
    /** Buffer handle */
    OTF2_Buffer* buffer;

    /** The last read event from the buffer */
    OTF2_GenericEvent current_event;

    /** Chain to next event reader, used in OTF2_Archive */
    OTF2_EvtReader* next;

    /** Number of the current event in the whole stream */
    uint64_t  global_event_position;
    /** NUmber of the current event inside the current chunk */
    uint64_t  chunk_local_event_position;
    /** Byte postition of the current event */
    uint8_t*  current_position;
    /** Table of the byte position of each event in the current chunk */
    uint8_t** position_table;
    /** Table of the byte position of each timestamp record in the current chunk */
    uint8_t** timestamp_table;

    /** Cached attribute list */
    OTF2_AttributeList attribute_list;

    /** Defines if a local event reader is stand-alone or operated by a global
     * event reader. */
    bool operated;

    /** Apply the mapping tables for referenced definitions.
     *  Has no effect, when this EvtReader is operated by the GlobalEvtReader. */
    bool apply_mapping_tables;

    /** Apply the clock offset to the timestamps.
     *  Has no effect, when this EvtReader is operated by the GlobalEvtReader. */
    bool apply_clock_offsets;

    /** Structure of callback references */
    OTF2_EvtReaderCallbacks reader_callbacks;
    /** Userdata pointer which is passed to the callbacks */
    void*                   user_data;

    /** Array of mapping tables */
    const OTF2_IdMap* mapping_tables[ OTF2_MAPPING_MAX ];

    /** vector of clock intervals */
    OTF2_ClockInterval*  clock_intervals;
    OTF2_ClockInterval** tail_clock_intervals;
    OTF2_ClockInterval*  current_clock_interval;
    OTF2_ClockInterval*  pending_clock_interval;
};


OTF2_EvtReader*
otf2_evt_reader_new( OTF2_Archive* archive,
                     uint64_t      locationID );


OTF2_ErrorCode
otf2_evt_reader_delete( OTF2_EvtReader* readerHandle );


void
otf2_evt_reader_operated_by_global_reader( OTF2_EvtReader* reader );


/** @intrernal
 *  @brief Reads one record from the event reader.
 *
 *  @param reader   Reference to the reader object
 *  @param record   Generic handle for returning the record. This ia at
 *                  least needed for the global reader.
 *  @param type     Handle for the timestamp
 */
OTF2_ErrorCode
otf2_evt_reader_read( OTF2_EvtReader* reader );


/** @internal
 *  @brief Skips one event record.
 *
 *  @param reader           Reference to the reader object
 */
OTF2_ErrorCode
otf2_evt_reader_skip( OTF2_EvtReader* reader );


/** @brief This interface can be used from the local definition reader to set the
 *  mapping tables, which is important for mapped reading.
 *
 *  @param reader  Reader object which reads the events from its buffer.
 *  @param iDMap   ID map, which has to be inserted into the reader.
 *  @param mapType Says which ID has to be mapped.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_evt_reader_set_mapping_table( OTF2_EvtReader*   reader,
                                   OTF2_MappingType  mapType,
                                   const OTF2_IdMap* iDMap );


/** @brief Adds a clock offset to the reader.
 *
 *  @param reader Reader object which reads the events from its buffer.
 *  @param time   Time of the synchronization
 *  @param offset Offset to the global clock.
 *  @param stdDev Standard deviation, unused.
 *
 *  @return OTF2_ErrorCode with !=OTF2_SUCCESS if there was an error.
 */
OTF2_ErrorCode
otf2_evt_reader_add_clock_offset( OTF2_EvtReader* reader,
                                  uint64_t        time,
                                  int64_t         offset,
                                  double          stdDev );


#endif /* OTF2_INTERNAL_EVT_READER_H */
