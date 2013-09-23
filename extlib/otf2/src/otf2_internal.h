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


#ifndef OTF2_INTERNAL_H
#define OTF2_INTERNAL_H


/** @internal
 *  @brief Keeps all necessary information about the buffer.
 *  Please see OTF2_Buffer_struct for a detailed description.
 */
typedef struct OTF2_Buffer_struct OTF2_Buffer;



/** @internal
 *  @brief The currently supported trace format version
 */
#define OTF2_TRACE_FORMAT 2


/** @internal
 *  @brief Size of a timestamp record.
 */
#define OTF2_BUFFER_TIMESTAMP_SIZE ( size_t )9


#endif /* OTF2_INTERNAL_H */
