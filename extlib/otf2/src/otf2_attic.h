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
 *  @file       src/otf2_attic.h
 *
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @brief      Provides conversion functions for new and obsoleted record
 *              attributes.
 */


#ifndef OTF2_INTERNAL_ATTIC_H
#define OTF2_INTERNAL_ATTIC_H


OTF2_RegionType
otf2_attic_def_region_provide_region_type( OTF2_RegionRole regionRole,
                                           OTF2_Paradigm   paradigm,
                                           OTF2_RegionFlag regionFlags );

void
otf2_attic_def_region_convert_region_type( OTF2_RegionType  regionType,
                                           OTF2_RegionRole* regionRole,
                                           OTF2_Paradigm*   paradigm,
                                           OTF2_RegionFlag* regionFlags );


OTF2_GroupTypePre12
otf2_attic_def_group_provide_group_type_pre_1_2( OTF2_GroupType groupType,
                                                 OTF2_Paradigm  paradigm,
                                                 OTF2_GroupFlag groupFlags );

void
otf2_attic_def_group_convert_group_type_pre_1_2( OTF2_GroupTypePre12 groupTypePre12,
                                                 OTF2_GroupType*     groupType,
                                                 OTF2_Paradigm*      paradigm,
                                                 OTF2_GroupFlag*     groupFlags );


#endif /* OTF2_INTERNAL_ATTIC_H */
