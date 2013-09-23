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
 *  @file       src/OTF2_DefReader_inc.c
 *  @source     templates/OTF2_DefReader_inc.tmpl.c
 *
 *  @status     beta
 *
 *  @maintainer Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 *  @authors    Dominic Eschweiler <d.eschweiler@fz-juelich.de>,
 *              Michael Wagner <michael.wagner@zih.tu-dresden.de>
 *
 *  @brief      This is the local definition reader, which reads location
 *              dependend definitions, and can also be used to get the mapping
 *              information from the local definition file. Local definitions
 *              are always assigned to a location.
 */

@otf2 for def in defs|local_defs:

/** @brief Reads a @@def.name@@ record and triggers the callback.
 *
 *  @param reader   Reference to the reader object
 *
 *  @return OTF2_SUCCESS
 */
static inline OTF2_ErrorCode
otf2_def_reader_read_@@def.lower@@( OTF2_DefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_LocalDef@@def.name@@ static_record;

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
        otf2_local_def_@@def.lower@@_convert_pre_@@v.major@@_@@v.minor@@( &static_record );
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
        @otf2 if def.name == 'MappingTable':
        /*@ Special case the MappingTable, to free the map on failure. @*/
        OTF2_IdMap_Free( ( OTF2_IdMap* )static_record.id_map );
        @otf2 endif
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    @otf2 if def.evt_reader_def_callback:
    if ( NULL == reader->evt_reader )
    {
        ret = otf2_archive_get_evt_reader( reader->archive,
                                           reader->location_id,
                                           &reader->evt_reader );
        if ( OTF2_SUCCESS != ret )
        {
            @otf2 for attr in def.all_array_attributes:
            free( static_record.@@attr.lower@@ );
            @otf2 endfor
            @otf2 if def.name == 'MappingTable':
            /*@ Special case the MappingTable, to free the map on failure. @*/
            OTF2_IdMap_Free( ( OTF2_IdMap* )static_record.id_map );
            @otf2 endif
            return UTILS_ERROR( ret, "Can't get local event reader" );
        }
    }
    ret = otf2_evt_reader_@@def.evt_reader_def_callback@@( reader->evt_reader @@def.callargs(case='lower', argprefix='static_record.')@@ );

    @otf2 endif
    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.@@def.lower@@ )
    {
        interrupt = reader->reader_callbacks.@@def.lower@@( reader->user_data @@def.callargs(case='lower', argprefix='static_record.')@@ );
    }

    @otf2 for attr in def.all_array_attributes:
    free( static_record.@@attr.lower@@ );
    @otf2 endfor

    @otf2 if def.evt_reader_def_callback:
    if ( OTF2_SUCCESS != ret )
    {
        @otf2 if def.name == 'MappingTable':
        /*@ Special case the MappingTable, to free the map on failure. @*/
        OTF2_IdMap_Free( ( OTF2_IdMap* )static_record.id_map );
        @otf2 endif
        return UTILS_ERROR( ret, "Can't add @@def.name@@ defintion to associated event reader." );
    }

    @otf2 endif
    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}

@otf2 endfor

/** @brief Reads one definition record and triggers the callback.
 *
 *  @param reader   Reference to the reader object
 *
 *  @return OTF2_ErrorCode, which is OTF2_SUCCESS if everything is fine.
 */
static OTF2_ErrorCode
otf2_def_reader_read( OTF2_DefReader* reader )
{
    UTILS_ASSERT( reader );

    /* Read record type */
    uint8_t           record_type;
    OTF2_ErrorCode ret;
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
            if ( OTF2_SUCCESS != ret )
            {
                return UTILS_ERROR( ret, "Load of next chunk failed!" );
            }
            return otf2_def_reader_read( reader );

        case OTF2_BUFFER_END_OF_FILE:
            return OTF2_ERROR_INDEX_OUT_OF_BOUNDS;

        @otf2 for def in defs|local_defs:
        case OTF2_LOCAL_DEF_@@def.upper@@:
            return otf2_def_reader_read_@@def.lower@@( reader );

        @otf2 endfor
        default:
            return otf2_def_reader_read_unknown( reader );
    }
}

@otf2 for def in defs|local_defs:

OTF2_ErrorCode
OTF2_DefReaderCallbacks_Set@@def.name@@Callback(
    OTF2_DefReaderCallbacks*            defReaderCallbacks,
    OTF2_DefReaderCallback_@@def.name@@ @@def.lname@@Callback )
{
    if ( !defReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid defReaderCallbacks argument!" );
    }

    defReaderCallbacks->@@def.lower@@ = @@def.lname@@Callback;

    return OTF2_SUCCESS;
}

@otf2 endfor
