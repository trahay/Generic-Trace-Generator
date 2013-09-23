/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2013,
 * Technische Universitaet Dresden, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 */


#ifndef OTF2_MARKER_H
#define OTF2_MARKER_H


/**
 *  @file       include/otf2/OTF2_Marker.h
 *
 *  @brief      This file provides types and enums for markers.
 */


#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Definitions.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Type used to indicate a reference to a @ref Marker definition */
typedef uint32_t OTF2_MarkerRef;
/** @brief The invalid value for a reference to a @ref Marker definition */
#define OTF2_UNDEFINED_MARKER ( ( OTF2_MarkerRef )OTF2_UNDEFINED_UINT32 )


/** @brief Wrapper for enum @ref OTF2_MarkerSeverity_enum. */
typedef uint8_t OTF2_MarkerSeverity;

/** A list of possible severities of user markers. */
enum OTF2_MarkerSeverity_enum
{
    /** The marker does not have a severity. */
    OTF2_SEVERITY_NONE,
    /** The marker has a low severity. */
    OTF2_SEVERITY_LOW,
    /** The marker has a medium severity. */
    OTF2_SEVERITY_MEDIUM,
    /** The marker has a high severity. */
    OTF2_SEVERITY_HIGH
};


/** @brief Wrapper for enum @ref OTF2_MarkerScope_enum. */
typedef uint8_t OTF2_MarkerScope;

/** A user marker does have a scope of it validity. */
enum OTF2_MarkerScope_enum
{
    /** The user marker has a global scope (could also be NONE). */
    OTF2_MARKER_SCOPE_GLOBAL,
    /** The user marker has a scope of a location. */
    OTF2_MARKER_SCOPE_LOCATION,
    /** The user marker has a scope of a location group. */
    OTF2_MARKER_SCOPE_LOCATION_GROUP,
    /** The user marker has a scope of a system tree. */
    OTF2_MARKER_SCOPE_SYSTEM_TREE_NODE,
    /** The user marker has a scope of a group. */
    OTF2_MARKER_SCOPE_GROUP,
    /** The user marker has a scope of a communicator. */
    OTF2_MARKER_SCOPE_COMM
};


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_MARKER_H */
