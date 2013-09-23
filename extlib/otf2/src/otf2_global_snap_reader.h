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

#ifndef OTF2_INTERNAL_GLOBAL_SNAP_READER_H
#define OTF2_INTERNAL_GLOBAL_SNAP_READER_H


/** @brief Keeps all neccessary information for the global snapshot reader. */
struct OTF2_GlobalSnapReader_struct
{
    /** Archive handle */
    OTF2_Archive*                  archive;
    /** @brief Number of snapshot readers still in the queue */
    uint64_t                       number_of_snap_readers;
    /** @brief User callbacks. */
    OTF2_GlobalSnapReaderCallbacks reader_callbacks;
    /** @brief Additional user data. */
    void*                          user_data;
    /** @brief Heap keeping all snapshot readers sorted by timestamp. */
    OTF2_SnapReader*               snap_queue[];
};

OTF2_GlobalSnapReader*
otf2_global_snap_reader_new( OTF2_Archive* archive );

OTF2_ErrorCode
otf2_global_snap_reader_delete( OTF2_GlobalSnapReader* readerHandle );


#endif /* OTF2_INTERNAL_GLOBAL_SNAP_READER_H */
