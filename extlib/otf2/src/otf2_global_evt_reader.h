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

#ifndef OTF2_INTERNAL_GLOBAL_EVT_READER_H
#define OTF2_INTERNAL_GLOBAL_EVT_READER_H


/** @brief Keeps all neccessary information for the global event reader. */
struct OTF2_GlobalEvtReader_struct
{
    /** Archive handle */
    OTF2_Archive*                 archive;
    /** @brief Number of event readers still in the queue */
    uint64_t                      number_of_evt_readers;
    /** @brief User callbacks. */
    OTF2_GlobalEvtReaderCallbacks reader_callbacks;
    /** @brief Additional user data. */
    void*                         user_data;
    /** @brief Heap keeping all event readers sorted by timestamp. */
    OTF2_EvtReader*               event_queue[];
};

OTF2_GlobalEvtReader*
otf2_global_evt_reader_new( OTF2_Archive* archive );

OTF2_ErrorCode
otf2_global_evt_reader_delete( OTF2_GlobalEvtReader* readerHandle );


#endif /* OTF2_INTERNAL_GLOBAL_EVT_READER_H */
