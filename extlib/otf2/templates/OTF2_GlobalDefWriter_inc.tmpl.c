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
 *  @file       src/OTF2_GlobalDefWriter_inc.c
 *  @source     templates/OTF2_GlobalDefWriter_inc.tmpl.c
 *
 *  @status     alpha
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This layer always writes globally defined OTF2 definition
 *              records and is used to write either the global definitions
 *              in addition to local definitions or write all definitions as
 *              globally valid in combination with OTF2_GlobalEventWriter.
 *              Global definitions are stored in one global definition file,
 *              which makes it nearly impossible to write them in a distributed
 *              manner. It is therefore only allowed to get such a writer from
 *              an OTF2_ArchiveHandler  which is marked as OTF2_MASTER.
 */

@otf2 for def in defs|global_defs:

OTF2_ErrorCode
OTF2_GlobalDefWriter_Write@@def.name@@( OTF2_GlobalDefWriter* writerHandle @@def.funcargs()@@ )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    @otf2 for v in def.versions[1:] if def.attic_attributes_by_version[v]|length:
    /* Generate pre @@v.major@@.@@v.minor@@ record fields */
    @otf2  for attic_attr in def.attic_attributes_by_version[v]:
    @@attic_attr.type@@ @@attic_attr.name@@;
    @otf2  endfor
    otf2_global_def_@@def.lower@@_provide_pre_@@v.major@@_@@v.minor@@( @@def.callargs(attributes=def.attic_attributes_by_version[v], argprefix='&', leading_comma=False)@@ @@def.callargs(attributes=def.attributes[1:])@@ );

    @otf2 endfor
    /*
     * Maximum memory usage of pure record data (excluding type ID
     * and record length information).
     */
    uint64_t record_data_length = 0;
    /*
     * Maximum record length is the record id byte, plus the individual sizes of the
     * attributes.
     */
    uint64_t record_length = 1;

    @otf2 for attr in def.record_attributes:
    @otf2  if attr.type.errorcheck_on_argument:
    if ( !@@attr.name@@ )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "@@attr.type.errorcheck_on_argument@@" );
    }
    @otf2  endif
    record_data_length += @@attr.size()@@; /* @@attr.name@@ */
    @otf2  if attr is array_attr:
    @otf2   for array_attr in attr.array_attributes:
    record_data_length += @@attr.name@@ * ( @@array_attr.size()@@ ); /* @@array_attr.name@@ */
    if ( @@attr.name@@ > 0 && !@@array_attr.name@@ )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid @@array_attr.name@@ array argument." );
    }
    @otf2   endfor
    @otf2  endif
    @otf2 endfor
    /* Finalize the upper bound of the record. */
    record_length += record_data_length;

    /*
     * Additional byte to store real record length. Due to on-line compression
     * real record length can differ from maximum record length.
     */
    record_length += 1;
    if ( record_data_length >= UINT8_MAX )
    {
        record_length += 8;
    }

    OTF2_ErrorCode status =
        OTF2_Buffer_WriteMemoryRequest( writerHandle->buffer, record_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_@@def.upper@@ );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    @otf2 for attr in def.record_attributes:
    OTF2_Buffer_Write@@attr.type.otf2@@( writerHandle->buffer, @@attr.name@@ );
    @otf2  if attr is array_attr:
    for ( @@attr.type@@ @@attr.name@@_i = 0;
          @@attr.name@@_i < @@attr.name@@;
          @@attr.name@@_i++ )
    {
        @otf2 for array_attr in attr.array_attributes:
        OTF2_Buffer_Write@@array_attr.type.otf2@@( writerHandle->buffer,
                                                   @@array_attr.name@@[ @@attr.name@@_i ] );
        @otf2 endfor
    }
    @otf2  endif
    @otf2 endfor

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;
    @otf2 if def == Location
    writerHandle->number_of_locations++;
    @otf2 endif

    return OTF2_SUCCESS;
}

@otf2 endfor
