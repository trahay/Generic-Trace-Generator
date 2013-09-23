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

/**
 *  @internal
 *
 *  @file       src/OTF2_AnchorFile.h
 *
 *  @maintainer Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer enables writing and parsing to and from anchor files.
 */

#ifndef OTF2_ANCHOR_FILE_H
#define OTF2_ANCHOR_FILE_H


OTF2_ErrorCode
OTF2_AnchorFile_Load( OTF2_Archive* archive );

OTF2_ErrorCode
OTF2_AnchorFile_Save( OTF2_Archive* archive );


#endif /* !OTF2_ANCHOR_FILE_H */
