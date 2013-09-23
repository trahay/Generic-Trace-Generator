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

#ifndef OTF2_INTERNAL_GLOBAL_DEF_WRITER_H
#define OTF2_INTERNAL_GLOBAL_DEF_WRITER_H


OTF2_GlobalDefWriter*
otf2_global_def_writer_new( OTF2_Archive* archive );

OTF2_ErrorCode
otf2_global_def_writer_delete( OTF2_GlobalDefWriter* writerHandle );


#endif /* OTF2_INTERNAL_GLOBAL_DEF_WRITER_H */
