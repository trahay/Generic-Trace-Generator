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
 *  @file       src/OTF2_GlobalDefReader_inc.c
 *  @source     templates/OTF2_GlobalDefReader_inc.tmpl.c
 *
 *  @status     alpha
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This module reads the global definitions from the global
 *              definition file. Therefore, the programmer has to register
 *              a callback function for each type of record. Rudimentary
 *              filtering could be done by registering only callbacks for
 *              the needed record types. This is an external interface.
 */

@otf2 for def in defs|global_defs:

static inline OTF2_ErrorCode
otf2_global_def_reader_read_@@def.lower@@( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDef@@def.name@@ static_record;

    OTF2_ErrorCode ret;
    uint64_t          record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    @otf2 if def.versions|length > 1:
    uint8_t* current_pos;
    @otf2 endif
    @otf2 for v in def.versions:
    @otf2  if v > def.since:
    /*
     * Read record attributes added in version @@v.major@@.@@v.minor@@, if there is
     * space left in the record
     */
    OTF2_Buffer_GetPosition( reader->buffer, &current_pos );
    if ( current_pos < record_end_pos )
    {
    @otf2  endif
    @otf2  for attr in def.record_attributes_by_version[v]:
    @otf2   if attr.type.errorcheck_on_read:
    ret = OTF2_Buffer_Read@@attr.type.otf2@@( reader->buffer, &static_record.@@attr.lower@@ );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read @@attr.name@@ attribute of @@def.name@@ record. @@attr.type.errorcheck_on_read@@" );
    }
    @otf2   else:
    OTF2_Buffer_Read@@attr.type.otf2@@( reader->buffer, &static_record.@@attr.lower@@ );
    @otf2   endif
    @otf2   if attr is array_attr:
    @otf2    for array_attr in attr.array_attributes:
    static_record.@@array_attr.lower@@ = malloc( static_record.@@attr.lower@@ * sizeof( @@array_attr.type@@ ) );
    if ( static_record.@@attr.lower@@ > 0 && !static_record.@@array_attr.lower@@ )
    {
        @otf2 for array_attr in attr.array_attributes[:loop.index - 1]:
        free( static_record.@@array_attr.lower@@ );
        @otf2 endfor
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Could not allocate memory for @@array_attr.name@@ array!" );
    }
    @otf2    endfor
    for ( @@attr.type@@ @@attr.lower@@_i = 0;
          @@attr.lower@@_i < static_record.@@attr.lower@@;
          @@attr.lower@@_i++ )
    {
        @otf2 for array_attr in attr.array_attributes:
        @otf2  if array_attr.type.errorcheck_on_read:
        ret = OTF2_Buffer_Read@@array_attr.type.otf2@@( reader->buffer,
                                                        static_record.@@array_attr.lower@@ + @@attr.lower@@_i );
        if ( OTF2_SUCCESS != ret )
        {
            @otf2 for array_attr in attr.array_attributes:
            free( static_record.@@array_attr.lower@@ );
            @otf2 endfor
            return UTILS_ERROR( ret, "Could not read @@array_attr.name@@ attribute of @@def.name@@ record. @@array_attr.type.errorcheck_on_read@@" );
        }
        @otf2  else:
        OTF2_Buffer_Read@@array_attr.type.otf2@@( reader->buffer,
                                                  static_record.@@array_attr.lower@@ + @@attr.lower@@_i );
        @otf2  endif
        @otf2 endfor
    }
    @otf2   endif
    @otf2  endfor
    @otf2  if v > def.since:
    }
    else
    {
        otf2_global_def_@@def.lower@@_convert_pre_@@v.major@@_@@v.minor@@( &static_record );
    }
    @otf2  endif

    @otf2 endfor
    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        @otf2 for attr in def.all_array_attributes:
        free( static_record.@@attr.lower@@ );
        @otf2 endfor
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.@@def.lower@@ )
    {
        interrupt = reader->reader_callbacks.@@def.lower@@( reader->user_data @@def.callargs(case='lower', argprefix='static_record.')@@ );
    }

    @otf2 for attr in def.all_array_attributes:
    free( static_record.@@attr.lower@@ );
    @otf2 endfor

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}

@otf2 endfor

OTF2_ErrorCode
otf2_global_def_reader_read( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );

    OTF2_ErrorCode ret;

    /* Read record type */
    uint8_t record_type;
    ret = OTF2_Buffer_Guarantee( reader->buffer, 1 );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record type." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &record_type );

    switch ( record_type )
    {
        case OTF2_BUFFER_END_OF_CHUNK:
            ret = OTF2_Buffer_ReadGetNextChunk( reader->buffer );
            if ( ret != OTF2_SUCCESS )
            {
                return UTILS_ERROR( ret, "Load of next chunk failed!" );
            }
            return otf2_global_def_reader_read( reader );

        case OTF2_BUFFER_END_OF_FILE:
            return OTF2_ERROR_INDEX_OUT_OF_BOUNDS;

        @otf2 for def in defs|global_defs:
        case OTF2_GLOBAL_DEF_@@def.upper@@:
            return otf2_global_def_reader_read_@@def.lower@@( reader );

        @otf2 endfor
        default:
            return otf2_global_def_reader_read_unknown( reader );
    }
}

@otf2 for def in defs|global_defs:

OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_Set@@def.name@@Callback(
    OTF2_GlobalDefReaderCallbacks*            globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_@@def.name@@ @@def.lname@@Callback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->@@def.lower@@ = @@def.lname@@Callback;

    return OTF2_SUCCESS;
}

@otf2 endfor
