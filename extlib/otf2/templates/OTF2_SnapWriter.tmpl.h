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


#ifndef OTF2_SNAP_WRITER_H
#define OTF2_SNAP_WRITER_H


/**
 *  @file       include/otf2/OTF2_SnapWriter.h
 *  @source     templates/OTF2_SnapWriter.tmpl.h
 *
 *  @brief      This lowest user-visible layer provides write routines to
 *              write snapshot records for a single location.
 */



#include <stdint.h>


#include <otf2/OTF2_ErrorCodes.h>


#include <otf2/OTF2_Events.h>
#include <otf2/OTF2_AttributeList.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @brief Keeps all necessary information about the snap writer.
 *  See OTF2_SnapWriter_struct for detailed information.
 * 
 *  @since Version 1.2
 *  */
typedef struct OTF2_SnapWriter_struct OTF2_SnapWriter;


/** @brief
 *  Function to get the location ID of a snap writer object.
 *
 *  @param writer       Snap writer object of interest
 *  @param locationID   Pointer to a variable where the ID is returned in
 * 
 *  @since Version 1.2
 *
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_GetLocationID( const OTF2_SnapWriter* writer,
                               OTF2_LocationRef*      locationID );


@otf2 for snap in snaps:

/** @brief Records an @@snap.name@@ snapshot record.
 *
 *  @@snap.doxygendoc()@@
 *
 *  @param writer        Writer object.
 *  @param attributeList Generic attributes for the record.
 *  @param snapTime      Snapshot time.
 *
 *  @@snap.doxygenargs()@@
 *
 *  @since Version @@snap.since@@
 *
@otf2 if snap.deprecated:
 *
 *  @deprecated In version @@snap.deprecated@@
@otf2 endif
 *  @return @eref{OTF2_SUCCESS} if successful, an error code if an error occurs.
 */
OTF2_ErrorCode
OTF2_SnapWriter_@@snap.name@@( OTF2_SnapWriter*    writer,
                               OTF2_AttributeList* attributeList,
                               OTF2_TimeStamp      snapTime @@snap.funcargs()@@ );

@otf2 endfor


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !OTF2_SNAP_WRITER_H */
