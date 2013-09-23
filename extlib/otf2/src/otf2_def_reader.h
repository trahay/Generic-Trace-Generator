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

#ifndef OTF2_INTERNAL_DEF_READER_H
#define OTF2_INTERNAL_DEF_READER_H


/** @brief Keeps all necessary information about the reader. */
struct OTF2_DefReader_struct
{
    /** The internal archive is later used to get the event writer where
     *  the mapping tables are inserted
     */
    OTF2_Archive* archive;

    /** Location of the reader. */
    uint64_t location_id;

    /** Buffer handle, on which the read is performed. */
    OTF2_Buffer* buffer;

    /** Chain to next definition reader, used in OTF2_Archive */
    OTF2_DefReader* next;

    /** Callbacks, which are triggered on read.*/
    OTF2_DefReaderCallbacks reader_callbacks;

    /** Generic pointer to the userdata, to handle them inside a callback. */
    void* user_data;

    /** This is the evt reader where the mapping tables are inserted */
    OTF2_EvtReader* evt_reader;
};


OTF2_DefReader*
otf2_def_reader_new( OTF2_Archive* archive,
                     uint64_t      locationID );


OTF2_ErrorCode
otf2_def_reader_delete( OTF2_DefReader* reader );


#endif /* OTF2_INTERNAL_DEF_READER_H */
