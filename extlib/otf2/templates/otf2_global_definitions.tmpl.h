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


#ifndef OTF2_INTERNAL_GLOBAL_DEFINITIONS_H
#define OTF2_INTERNAL_GLOBAL_DEFINITIONS_H


/**
 *  @file       src/otf2_global_definitions.h
 *  @source     templates/otf2_global_definitions.tmpl.h
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 */


/** @brief Wrapper for the OTF2_GlobalDefinitionType_enum
 */
typedef uint8_t OTF2_GlobalDefinitionType;


/** @brief An enum which holds all values that are possible global definition types.
 */
enum OTF2_GlobalDefinitionType_enum
{
    /* First five entries are reserved for OTF2 internals */

    /* OTF2 Records */
    @otf2 for def in defs|global_defs:
    OTF2_GLOBAL_DEF_@@def.upper@@ = @@def.id@@, /**< */
    @otf2 endfor

    OTF2_GLOBAL_DEF_MAX_ID
};

@otf2 for def in defs|global_defs:

/** @brief @@def.name@@ definition.
 */
typedef struct OTF2_GlobalDef@@def.name@@_struct
{
    @@def.structargs(attributes=def.record_attributes, case='lower')@@
} OTF2_GlobalDef@@def.name@@;

@otf2 endfor

struct OTF2_GlobalDefReaderCallbacks_struct
{
    OTF2_GlobalDefReaderCallback_Unknown unknown;
    @otf2 for def in defs|global_defs:
    OTF2_GlobalDefReaderCallback_@@def.name@@ @@def.lower@@;
    @otf2 endfor
};


#endif /* OTF2_INTERNAL_GLOBAL_DEFINITIONS_H */