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

#ifndef OTF2_INTERNAL_EVT_WRITER_H
#define OTF2_INTERNAL_EVT_WRITER_H


/** @brief Keeps all necessary information about the event writer. */
struct OTF2_EvtWriter_struct
{
    /** Archive handle */
    OTF2_Archive*   archive;
    /** Pointer to the memory buffer handle. */
    OTF2_Buffer*    buffer;
    /** ID of the location. */
    uint64_t        location_id;
    /** user data */
    void*           user_data;
    /** Chain to next event writer, used in OTF2_Archive */
    OTF2_EvtWriter* next;
};


OTF2_EvtWriter*
otf2_evt_writer_new( OTF2_Archive* archive,
                     uint64_t      locationID );


OTF2_ErrorCode
otf2_evt_writer_delete( OTF2_EvtWriter* writer );


#endif /* OTF2_INTERNAL_EVT_WRITER_H */