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
 *  @file       src/otf2_attic.c
 *
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @brief      Holds conversion functions for obsoleted record attributes.
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


OTF2_RegionType
otf2_attic_def_region_provide_region_type( OTF2_RegionRole regionRole,
                                           OTF2_Paradigm   paradigm,
                                           OTF2_RegionFlag regionFlags )
{
    switch ( paradigm )
    {
        case OTF2_PARADIGM_UNKNOWN:
            switch ( regionRole )
            {
                case OTF2_REGION_ROLE_FUNCTION:
                    return OTF2_REGION_TYPE_FUNCTION;

                default:
                    return OTF2_REGION_TYPE_UNKNOWN;
            }

        case OTF2_PARADIGM_USER:
            switch ( regionRole )
            {
                case OTF2_REGION_ROLE_FUNCTION:
                    if ( regionFlags & OTF2_REGION_FLAG_DYNAMIC )
                    {
                        return OTF2_REGION_TYPE_DYNAMIC_FUNCTION;
                    }
                    else
                    {
                        return OTF2_REGION_TYPE_USER_REGION;
                    }

                case OTF2_REGION_ROLE_FILE_IO:
                    return OTF2_REGION_TYPE_FILE;

                case OTF2_REGION_ROLE_LOOP:
                    if ( regionFlags & OTF2_REGION_FLAG_DYNAMIC )
                    {
                        /* DYNAMIC region */
                        if ( regionFlags & OTF2_REGION_FLAG_PHASE )
                        {
                            return OTF2_REGION_TYPE_DYNAMIC_LOOP_PHASE;
                        }
                        else
                        {
                            return OTF2_REGION_TYPE_DYNAMIC_LOOP;
                        }
                    }
                    else
                    {
                        /* no DYNAMIC region */
                        if ( regionFlags & OTF2_REGION_FLAG_PHASE )
                        {
                            /* This combination was not supported by OTF2 1.0.x */
                            return OTF2_REGION_TYPE_UNKNOWN;
                        }
                        else
                        {
                            return OTF2_REGION_TYPE_LOOP;
                        }
                    }

                case OTF2_REGION_ROLE_CODE:
                    if ( regionFlags & OTF2_REGION_FLAG_DYNAMIC )
                    {
                        /* DYNAMIC region */
                        if ( regionFlags & OTF2_REGION_FLAG_PHASE )
                        {
                            return OTF2_REGION_TYPE_DYNAMIC_PHASE;
                        }
                        else
                        {
                            return OTF2_REGION_TYPE_DYNAMIC;
                        }
                    }
                    else
                    {
                        /* no DYNAMIC region */
                        if ( regionFlags & OTF2_REGION_FLAG_PHASE )
                        {
                            return OTF2_REGION_TYPE_PHASE;
                        }
                        else
                        {
                            return OTF2_REGION_TYPE_FUNCTION;
                        }
                    }

                default:
                    return OTF2_REGION_TYPE_UNKNOWN;
            }

        case OTF2_PARADIGM_COMPILER:
            switch ( regionRole )
            {
                case OTF2_REGION_ROLE_FUNCTION:
                    return OTF2_REGION_TYPE_FUNCTION;

                default:
                    return OTF2_REGION_TYPE_UNKNOWN;
            }

        case OTF2_PARADIGM_OPENMP:
            switch ( regionRole )
            {
                case OTF2_REGION_ROLE_PARALLEL:
                    return OTF2_REGION_TYPE_OMP_PARALLEL;

                case OTF2_REGION_ROLE_LOOP:
                    return OTF2_REGION_TYPE_OMP_LOOP;

                case OTF2_REGION_ROLE_SECTIONS:
                    return OTF2_REGION_TYPE_OMP_SECTIONS;

                case OTF2_REGION_ROLE_SECTION:
                    return OTF2_REGION_TYPE_OMP_SECTION;

                case OTF2_REGION_ROLE_WORKSHARE:
                    return OTF2_REGION_TYPE_OMP_WORKSHARE;

                case OTF2_REGION_ROLE_SINGLE:
                    return OTF2_REGION_TYPE_OMP_SINGLE;

                case OTF2_REGION_ROLE_MASTER:
                    return OTF2_REGION_TYPE_OMP_MASTER;

                case OTF2_REGION_ROLE_CRITICAL:
                    return OTF2_REGION_TYPE_OMP_CRITICAL;

                case OTF2_REGION_ROLE_ATOMIC:
                    return OTF2_REGION_TYPE_OMP_ATOMIC;

                case OTF2_REGION_ROLE_BARRIER:
                    return OTF2_REGION_TYPE_OMP_BARRIER;

                case OTF2_REGION_ROLE_IMPLICIT_BARRIER:
                    return OTF2_REGION_TYPE_OMP_IBARRIER;

                case OTF2_REGION_ROLE_FLUSH:
                    return OTF2_REGION_TYPE_OMP_FLUSH;

                case OTF2_REGION_ROLE_CRITICAL_SBLOCK:
                    return OTF2_REGION_TYPE_OMP_CRITICAL_SBLOCK;

                case OTF2_REGION_ROLE_SINGLE_SBLOCK:
                    return OTF2_REGION_TYPE_OMP_SINGLE_SBLOCK;

                case OTF2_REGION_ROLE_WRAPPER:
                    return OTF2_REGION_TYPE_OMP_WRAPPER;

                case OTF2_REGION_ROLE_TASK:
                    return OTF2_REGION_TYPE_OMP_TASK;

                case OTF2_REGION_ROLE_TASK_WAIT:
                    return OTF2_REGION_TYPE_OMP_TASK_WAIT;

                case OTF2_REGION_ROLE_TASK_CREATE:
                    return OTF2_REGION_TYPE_OMP_TASK_CREATE;

                case OTF2_REGION_ROLE_ORDERED:
                    return OTF2_REGION_TYPE_OMP_ORDERED;

                case OTF2_REGION_ROLE_ORDERED_SBLOCK:
                    return OTF2_REGION_TYPE_OMP_ORDERED_SBLOCK;

                default:
                    return OTF2_REGION_TYPE_UNKNOWN;
            }

        case OTF2_PARADIGM_MPI:
            switch ( regionRole )
            {
                case OTF2_REGION_ROLE_BARRIER:
                    return OTF2_REGION_TYPE_MPI_COLL_BARRIER;

                case OTF2_REGION_ROLE_COLL_ONE2ALL:
                    return OTF2_REGION_TYPE_MPI_COLL_ONE2ALL;

                case OTF2_REGION_ROLE_COLL_ALL2ONE:
                    return OTF2_REGION_TYPE_MPI_COLL_ALL2ONE;

                case OTF2_REGION_ROLE_COLL_ALL2ALL:
                    return OTF2_REGION_TYPE_MPI_COLL_ALL2ALL;

                case OTF2_REGION_ROLE_COLL_OTHER:
                    return OTF2_REGION_TYPE_MPI_COLL_OTHER;

                default:
                    return OTF2_REGION_TYPE_UNKNOWN;
            }

        default:
            return OTF2_REGION_TYPE_UNKNOWN;
    }
}

void
otf2_attic_def_region_convert_region_type( OTF2_RegionType  regionType,
                                           OTF2_RegionRole* regionRole,
                                           OTF2_Paradigm*   paradigm,
                                           OTF2_RegionFlag* regionFlags )
{
    *regionRole  = OTF2_REGION_ROLE_UNKNOWN;
    *paradigm    = OTF2_PARADIGM_UNKNOWN;
    *regionFlags = OTF2_REGION_FLAG_NONE;

    switch ( regionType )
    {
        case OTF2_REGION_TYPE_FUNCTION:
            /* OTF2_REGION_TYPE_FUNCTION was used by several adapters
             * (e.g. compiler, MPI, and user adapter);
             * we lose some sematics here */
            *regionRole = OTF2_REGION_ROLE_FUNCTION;
            break;

        case OTF2_REGION_TYPE_LOOP:
            /* OTF2_REGION_TYPE_LOOP was only used by the user adapter */
            *regionRole = OTF2_REGION_ROLE_LOOP;
            *paradigm   = OTF2_PARADIGM_USER;
            break;

        case OTF2_REGION_TYPE_USER_REGION:
            /* OTF2_REGION_TYPE_USER_REGION was used by several adapters
             * (e.g. POMP and user adapter);
             * we lose some sematics here */
            *regionRole = OTF2_REGION_ROLE_FUNCTION;
            break;

        case OTF2_REGION_TYPE_FILE:
            *regionRole = OTF2_REGION_ROLE_FILE_IO;
            *paradigm   = OTF2_PARADIGM_USER;
            break;

        case OTF2_REGION_TYPE_OMP_PARALLEL:
            *regionRole = OTF2_REGION_ROLE_PARALLEL;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_LOOP:
            *regionRole = OTF2_REGION_ROLE_LOOP;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_SECTIONS:
            *regionRole = OTF2_REGION_ROLE_SECTIONS;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_SECTION:
            *regionRole = OTF2_REGION_ROLE_SECTION;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_WORKSHARE:
            *regionRole = OTF2_REGION_ROLE_WORKSHARE;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_SINGLE:
            *regionRole = OTF2_REGION_ROLE_SINGLE;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_MASTER:
            *regionRole = OTF2_REGION_ROLE_MASTER;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_CRITICAL:
            *regionRole = OTF2_REGION_ROLE_CRITICAL;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_ATOMIC:
            *regionRole = OTF2_REGION_ROLE_ATOMIC;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_BARRIER:
            *regionRole = OTF2_REGION_ROLE_BARRIER;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_IBARRIER:
            *regionRole = OTF2_REGION_ROLE_IMPLICIT_BARRIER;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_FLUSH:
            *regionRole = OTF2_REGION_ROLE_FLUSH;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_CRITICAL_SBLOCK:
            *regionRole = OTF2_REGION_ROLE_CRITICAL_SBLOCK;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_SINGLE_SBLOCK:
            *regionRole = OTF2_REGION_ROLE_SINGLE_SBLOCK;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_WRAPPER:
            *regionRole = OTF2_REGION_ROLE_WRAPPER;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_TASK:
            *regionRole = OTF2_REGION_ROLE_TASK;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_TASK_WAIT:
            *regionRole = OTF2_REGION_ROLE_TASK_WAIT;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_TASK_CREATE:
            *regionRole = OTF2_REGION_ROLE_TASK_CREATE;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_ORDERED:
            *regionRole = OTF2_REGION_ROLE_ORDERED;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_OMP_ORDERED_SBLOCK:
            *regionRole = OTF2_REGION_ROLE_ORDERED_SBLOCK;
            *paradigm   = OTF2_PARADIGM_OPENMP;
            break;

        case OTF2_REGION_TYPE_MPI_COLL_BARRIER:
            *regionRole = OTF2_REGION_ROLE_BARRIER;
            *paradigm   = OTF2_PARADIGM_MPI;
            break;

        case OTF2_REGION_TYPE_MPI_COLL_ONE2ALL:
            *regionRole = OTF2_REGION_ROLE_COLL_ONE2ALL;
            *paradigm   = OTF2_PARADIGM_MPI;
            break;

        case OTF2_REGION_TYPE_MPI_COLL_ALL2ONE:
            *regionRole = OTF2_REGION_ROLE_COLL_ALL2ONE;
            *paradigm   = OTF2_PARADIGM_MPI;
            break;

        case OTF2_REGION_TYPE_MPI_COLL_ALL2ALL:
            *regionRole = OTF2_REGION_ROLE_COLL_ALL2ALL;
            *paradigm   = OTF2_PARADIGM_MPI;
            break;

        case OTF2_REGION_TYPE_MPI_COLL_OTHER:
            *regionRole = OTF2_REGION_ROLE_COLL_OTHER;
            *paradigm   = OTF2_PARADIGM_MPI;
            break;

        case OTF2_REGION_TYPE_PHASE:
            /* OTF2_REGION_TYPE_PHASE was only used by user adapter */
            *regionRole = OTF2_REGION_ROLE_CODE;
            *paradigm   = OTF2_PARADIGM_USER;
            break;

        case OTF2_REGION_TYPE_DYNAMIC:
            /* OTF2_REGION_TYPE_DYNAMIC was only used by user adapter */
            *regionRole  = OTF2_REGION_ROLE_CODE;
            *paradigm    = OTF2_PARADIGM_USER;
            *regionFlags = OTF2_REGION_FLAG_DYNAMIC;
            break;

        case OTF2_REGION_TYPE_DYNAMIC_PHASE:
            /* OTF2_REGION_TYPE_DYNAMIC_PHASE was only used by user adapter */
            *regionRole  = OTF2_REGION_ROLE_CODE;
            *paradigm    = OTF2_PARADIGM_USER;
            *regionFlags = OTF2_REGION_FLAG_DYNAMIC | OTF2_REGION_FLAG_PHASE;
            break;

        case OTF2_REGION_TYPE_DYNAMIC_LOOP:
            /* OTF2_REGION_TYPE_DYNAMIC_LOOP was only used by user adapter */
            *regionRole  = OTF2_REGION_ROLE_LOOP;
            *paradigm    = OTF2_PARADIGM_USER;
            *regionFlags = OTF2_REGION_FLAG_DYNAMIC;
            break;

        case OTF2_REGION_TYPE_DYNAMIC_FUNCTION:
            /* OTF2_REGION_TYPE_DYNAMIC_FUNCTION was only used by user adapter */
            *regionRole  = OTF2_REGION_ROLE_FUNCTION;
            *paradigm    = OTF2_PARADIGM_USER;
            *regionFlags = OTF2_REGION_FLAG_DYNAMIC;
            break;

        case OTF2_REGION_TYPE_DYNAMIC_LOOP_PHASE:
            /* OTF2_REGION_TYPE_DYNAMIC_LOOP_PHASE was only used by user adapter */
            *regionRole  = OTF2_REGION_ROLE_LOOP;
            *paradigm    = OTF2_PARADIGM_USER;
            *regionFlags = OTF2_REGION_FLAG_DYNAMIC | OTF2_REGION_FLAG_PHASE;
            break;

        default:
            break;
    }
}

OTF2_GroupTypePre12
otf2_attic_def_group_provide_group_type_pre_1_2( OTF2_GroupType groupType,
                                                 OTF2_Paradigm  paradigm,
                                                 OTF2_GroupFlag groupFlags )
{
    if ( paradigm == OTF2_PARADIGM_UNKNOWN
         && groupFlags == OTF2_GROUP_FLAG_NONE )
    {
        switch ( groupType )
        {
            case OTF2_GROUP_TYPE_LOCATIONS:
                return OTF2_GROUP_TYPE_PRE_1_2_LOCATIONS;

            case OTF2_GROUP_TYPE_REGIONS:
                return OTF2_GROUP_TYPE_PRE_1_2_REGIONS;

            case OTF2_GROUP_TYPE_METRIC:
                return OTF2_GROUP_TYPE_PRE_1_2_METRIC;
        }
    }

    if ( paradigm == OTF2_PARADIGM_MPI
         && groupFlags == OTF2_GROUP_FLAG_NONE )
    {
        switch ( groupType )
        {
            case OTF2_GROUP_TYPE_COMM_LOCATIONS:
                return OTF2_GROUP_TYPE_PRE_1_2_MPI_LOCATIONS;
            case OTF2_GROUP_TYPE_COMM_GROUP:
                return OTF2_GROUP_TYPE_PRE_1_2_MPI_GROUP;
            case OTF2_GROUP_TYPE_COMM_SELF:
                return OTF2_GROUP_TYPE_PRE_1_2_MPI_COMM_SELF;
        }
    }

    return OTF2_GROUP_TYPE_PRE_1_2_UNKNOWN;
}

void
otf2_attic_def_group_convert_group_type_pre_1_2( OTF2_GroupTypePre12 groupTypePre12,
                                                 OTF2_GroupType*     groupType,
                                                 OTF2_Paradigm*      paradigm,
                                                 OTF2_GroupFlag*     groupFlags )
{
    *groupType  = OTF2_GROUP_TYPE_UNKNOWN;
    *paradigm   = OTF2_PARADIGM_UNKNOWN;
    *groupFlags = OTF2_GROUP_FLAG_NONE;

    switch ( groupTypePre12 )
    {
        case OTF2_GROUP_TYPE_PRE_1_2_LOCATIONS:
            *groupType = OTF2_GROUP_TYPE_LOCATIONS;
            break;

        case OTF2_GROUP_TYPE_PRE_1_2_REGIONS:
            *groupType = OTF2_GROUP_TYPE_REGIONS;
            break;

        case OTF2_GROUP_TYPE_PRE_1_2_METRIC:
            *groupType = OTF2_GROUP_TYPE_REGIONS;
            break;

        case OTF2_GROUP_TYPE_PRE_1_2_MPI_GROUP:
            *groupType = OTF2_GROUP_TYPE_COMM_GROUP;
            *paradigm  = OTF2_PARADIGM_MPI;
            break;

        case OTF2_GROUP_TYPE_PRE_1_2_MPI_COMM_SELF:
            *groupType = OTF2_GROUP_TYPE_COMM_SELF;
            *paradigm  = OTF2_PARADIGM_MPI;
            break;

        case OTF2_GROUP_TYPE_PRE_1_2_MPI_LOCATIONS:
            *groupType = OTF2_GROUP_TYPE_COMM_LOCATIONS;
            *paradigm  = OTF2_PARADIGM_MPI;
            break;

        case OTF2_GROUP_TYPE_PRE_1_2_UNKNOWN:
        default:
            break;
    }
}
