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
 *  @file       src/OTF2_DefWriter_inc.c
 *  @source     templates/OTF2_DefWriter_inc.tmpl.c
 *
 *  @status     beta
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This is the lowest user-visible layer which provides write
 *              operations for the event data of a single location (i.e.,
 *              data from a single thread). Due file writing is engaged by
 *              the memory buffer module, the related callbacks for
 *              buffer-flushing must be set for the whole archive handle.
 */

@otf2 for def in defs|local_defs:

OTF2_ErrorCode
OTF2_DefWriter_Write@@def.name@@( OTF2_DefWriter* writerHandle @@def.funcargs()@@ )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    OTF2_ErrorCode ret;

    @otf2 for v in def.versions[1:] if def.attic_attributes_by_version[v]|length:
    /* Generate pre @@v.major@@.@@v.minor@@ record fields */
    @otf2  for attic_attr in def.attic_attributes_by_version[v]:
    @@attic_attr.type@@ @@attic_attr.name@@;
    @otf2  endfor
    otf2_local_def_@@def.lower@@_provide_pre_@@v.major@@_@@v.minor@@( @@def.callargs(attributes=def.attic_attributes_by_version[v], argprefix='&', leading_comma=False)@@ @@def.callargs(attributes=def.attributes[1:])@@ );

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

    ret = OTF2_Buffer_WriteMemoryRequest( writerHandle->buffer, record_length );
    if ( OTF2_SUCCESS != ret )
    {
        return ret;
    }

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_LOCAL_DEF_@@def.upper@@ );

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
    return OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
}

@otf2 endfor
