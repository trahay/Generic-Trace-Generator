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
 *  @file       src/otf2_attic_types.h
 *  @source     templates/otf2_attic_types.tmpl.h
 *
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @brief      Holds obsolete types.
 */


#ifndef OTF2_INTERNAL_ATTIC_TYPES_H
#define OTF2_INTERNAL_ATTIC_TYPES_H


/** @brief Wrapper for enum @ref OTF2_RegionType_enum. */
typedef uint8_t OTF2_RegionType;

/**
 * @brief List of possible definitions of type Region.
 *
 * @since Version 1.0, removed in version 1.1.
 */
enum OTF2_RegionType_enum
{
    /** @brief A region of unknown type.
     */
    OTF2_REGION_TYPE_UNKNOWN             = 0,
    /** @brief
     */
    OTF2_REGION_TYPE_FUNCTION            = 1,
    /** @brief
     */
    OTF2_REGION_TYPE_LOOP                = 2,
    /** @brief
     */
    OTF2_REGION_TYPE_USER_REGION         = 3,
    /** @brief
     */
    OTF2_REGION_TYPE_FILE                = 4,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_PARALLEL        = 5,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_LOOP            = 6,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_SECTIONS        = 7,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_SECTION         = 8,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_WORKSHARE       = 9,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_SINGLE          = 10,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_MASTER          = 11,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_CRITICAL        = 12,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_ATOMIC          = 13,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_BARRIER         = 14,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_IBARRIER        = 15,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_FLUSH           = 16,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_CRITICAL_SBLOCK = 17,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_SINGLE_SBLOCK   = 18,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_WRAPPER         = 19,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_TASK            = 20,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_TASK_WAIT       = 21,
    /** @brief
     */
    OTF2_REGION_TYPE_MPI_COLL_BARRIER    = 22,
    /** @brief
     */
    OTF2_REGION_TYPE_MPI_COLL_ONE2ALL    = 23,
    /** @brief
     */
    OTF2_REGION_TYPE_MPI_COLL_ALL2ONE    = 24,
    /** @brief
     */
    OTF2_REGION_TYPE_MPI_COLL_ALL2ALL    = 25,
    /** @brief
     */
    OTF2_REGION_TYPE_MPI_COLL_OTHER      = 26,
    /** @brief
     */
    OTF2_REGION_TYPE_PHASE               = 27,
    /** @brief
     */
    OTF2_REGION_TYPE_DYNAMIC             = 28,
    /** @brief
     */
    OTF2_REGION_TYPE_DYNAMIC_PHASE       = 29,
    /** @brief
     */
    OTF2_REGION_TYPE_DYNAMIC_LOOP        = 30,
    /** @brief
     */
    OTF2_REGION_TYPE_DYNAMIC_FUNCTION    = 31,
    /** @brief
     */
    OTF2_REGION_TYPE_DYNAMIC_LOOP_PHASE  = 32,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_TASK_CREATE     = 33,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_ORDERED         = 34,
    /** @brief
     */
    OTF2_REGION_TYPE_OMP_ORDERED_SBLOCK  = 35
};


/** @brief Wrapper for enum @ref OTF2_GroupTypePre12_enum. */
typedef uint8_t OTF2_GroupTypePre12;

/**
 * @brief
 *
 * @since Version 1.0, removed in version 1.2.
 */
enum OTF2_GroupTypePre12_enum
{
    /** @brief Group of unknown type.
     */
    OTF2_GROUP_TYPE_PRE_1_2_UNKNOWN       = 0,
    /** @brief Group of locations.
     */
    OTF2_GROUP_TYPE_PRE_1_2_LOCATIONS     = 1,
    /** @brief Group of regions.
     */
    OTF2_GROUP_TYPE_PRE_1_2_REGIONS       = 2,
    /** @brief Group of metrics.
     */
    OTF2_GROUP_TYPE_PRE_1_2_METRIC        = 3,
    /** @brief MPI group.
     */
    OTF2_GROUP_TYPE_PRE_1_2_MPI_GROUP     = 4,
    /** @brief Special group type to efficiently handle MPI self-like communicators.
     */
    OTF2_GROUP_TYPE_PRE_1_2_MPI_COMM_SELF = 5,
    /** @brief List of location IDs, which are MPI ranks.
     *
     *  The size of this group should match the size of MPI_COMM_WORLD. Each
     *  entry in the list is a location ID, where the index of the
     *  entry is equal to the rank in MPI_COMM_WORLD. (Ie. rank i
     *  corresponds to location members[i])
     *
     *  Also, if this definition is present, the location group ids of
     *  locations with type OTF2_LOCATION_TYPE_CPU_THREAD should match
     *  The MPI rank.
     *
     *  This group needs to be defined, before any group of type
     *  @eref{OTF2_GROUP_TYPE_PRE_1_2_MPI_GROUP}.
     *
     *  Note: This does not makes sense in local definitions.
     */
    OTF2_GROUP_TYPE_PRE_1_2_MPI_LOCATIONS = 6
};


#endif /* OTF2_INTERNAL_ATTIC_TYPES_H */
