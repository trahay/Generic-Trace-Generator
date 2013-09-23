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


/**
 *  @file       src/otf2_events.c
 *
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @brief      Holds conversion functions for deprecated event record.
 */


#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>

#include "otf2_attic_types.h"
#include "otf2_attic.h"

#include "otf2_events_int.h"

#include "otf2_attribute_list.h"


bool
otf2_event_thread_fork_convert_to_omp_fork( const OTF2_ThreadFork* threadForkRecord,
                                            OTF2_AttributeList*    attributeList,
                                            OTF2_OmpFork*          ompForkRecord )
{
    if ( threadForkRecord->model != OTF2_PARADIGM_OPENMP )
    {
        return false;
    }

    ompForkRecord->number_of_requested_threads = threadForkRecord->number_of_requested_threads;

    return true;
}


bool
otf2_event_thread_join_convert_to_omp_join( const OTF2_ThreadJoin* threadJoinRecord,
                                            OTF2_AttributeList*    attributeList,
                                            OTF2_OmpJoin*          ompJoinRecord )
{
    if ( threadJoinRecord->model != OTF2_PARADIGM_OPENMP )
    {
        return false;
    }

    return true;
}


bool
otf2_event_thread_acquire_lock_convert_to_omp_acquire_lock(
    const OTF2_ThreadAcquireLock* threadAcquireLockRecord,
    OTF2_AttributeList*           attributeList,
    OTF2_OmpAcquireLock*          ompAcquireLockRecord )
{
    if ( threadAcquireLockRecord->model != OTF2_PARADIGM_OPENMP )
    {
        return false;
    }

    ompAcquireLockRecord->lock_id           = threadAcquireLockRecord->lock_id;
    ompAcquireLockRecord->acquisition_order = threadAcquireLockRecord->acquisition_order;

    return true;
}


bool
otf2_event_thread_release_lock_convert_to_omp_release_lock(
    const OTF2_ThreadReleaseLock* threadReleaseLockRecord,
    OTF2_AttributeList*           attributeList,
    OTF2_OmpReleaseLock*          ompReleaseLockRecord )
{
    if ( threadReleaseLockRecord->model != OTF2_PARADIGM_OPENMP )
    {
        return false;
    }

    ompReleaseLockRecord->lock_id           = threadReleaseLockRecord->lock_id;
    ompReleaseLockRecord->acquisition_order = threadReleaseLockRecord->acquisition_order;

    return true;
}
