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

#ifndef OTF2_INTERNAL_DEF_WRITER_H
#define OTF2_INTERNAL_DEF_WRITER_H


/** @brief Keeps all necessary information about the local definition writer. */
struct OTF2_DefWriter_struct
{
    /** Archive handle */
    OTF2_Archive*   archive;
    /** Buffer handle */
    OTF2_Buffer*    buffer;
    /** Location ID of the related definition stream */
    uint64_t        location_id;
    /** Chain to next def writer, used in OTF2_Archive */
    OTF2_DefWriter* next;
};


OTF2_DefWriter*
otf2_def_writer_new( OTF2_Archive* archive,
                     uint64_t      locationID );

OTF2_ErrorCode
otf2_def_writer_delete( OTF2_DefWriter* writerHandle );


#endif /* OTF2_INTERNAL_DEF_WRITER_H */
