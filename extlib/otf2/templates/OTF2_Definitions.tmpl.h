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


#ifndef OTF2_DEFINITIONS_H
#define OTF2_DEFINITIONS_H


/**
 *  @file       include/otf2/OTF2_Definitions.h
 *  @source     templates/OTF2_Definitions.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      Data types used in the definition records.
 */


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_GeneralDefinitions.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

@otf2 for enum in enums|def_enums:

/** @brief Wrapper for enum @ref @@enum.type@@_enum. */
typedef @@enum.type.c_orig@@ @@enum.type@@;

/**
 * @brief @@enum.doxygendoc(prefix=' * ')@@
 *
 * @since Version @@enum.since@@
 */
enum @@enum.type@@_enum
{
    @otf2 for entry in enum.entries:
    /** @brief @@entry.doxygendoc(prefix='     *  ')@@
     */
    @@entry.name@@ = @@entry.value@@@@enum.need_comma(entry)@@
    @otf2 endfor
    @otf2 if enum has terminator:

    /** @brief @@enum.terminator.doxygendoc(prefix='     *  ')@@
     */
    @@enum.terminator.name@@ = @@enum.terminator.value@@
    @otf2 endif
};

@otf2 endfor

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* OTF2_DEFINITIONS_H */
