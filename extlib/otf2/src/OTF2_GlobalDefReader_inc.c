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


static inline OTF2_ErrorCode
otf2_global_def_reader_read_clock_properties( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefClockProperties static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.timer_resolution );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read timerResolution attribute of ClockProperties record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.global_offset );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read globalOffset attribute of ClockProperties record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.trace_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read traceLength attribute of ClockProperties record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.clock_properties )
    {
        interrupt = reader->reader_callbacks.clock_properties( reader->user_data,
                                                               static_record.timer_resolution,
                                                               static_record.global_offset,
                                                               static_record.trace_length );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_string( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefString static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of String record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadString( reader->buffer, &static_record.string );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read string attribute of String record. Can't read string from buffer." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.string )
    {
        interrupt = reader->reader_callbacks.string( reader->user_data,
                                                     static_record.self,
                                                     static_record.string );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_attribute( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefAttribute static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of Attribute record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of Attribute record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.type );

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.attribute )
    {
        interrupt = reader->reader_callbacks.attribute( reader->user_data,
                                                        static_record.self,
                                                        static_record.name,
                                                        static_record.type );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_system_tree_node( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefSystemTreeNode static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of SystemTreeNode record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of SystemTreeNode record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.class_name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read className attribute of SystemTreeNode record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.parent );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parent attribute of SystemTreeNode record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.system_tree_node )
    {
        interrupt = reader->reader_callbacks.system_tree_node( reader->user_data,
                                                               static_record.self,
                                                               static_record.name,
                                                               static_record.class_name,
                                                               static_record.parent );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_location_group( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefLocationGroup static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of LocationGroup record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of LocationGroup record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.location_group_type );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.system_tree_parent );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read systemTreeParent attribute of LocationGroup record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.location_group )
    {
        interrupt = reader->reader_callbacks.location_group( reader->user_data,
                                                             static_record.self,
                                                             static_record.name,
                                                             static_record.location_group_type,
                                                             static_record.system_tree_parent );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_location( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefLocation static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of Location record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of Location record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.location_type );
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.number_of_events );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfEvents attribute of Location record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.location_group );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read locationGroup attribute of Location record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.location )
    {
        interrupt = reader->reader_callbacks.location( reader->user_data,
                                                       static_record.self,
                                                       static_record.name,
                                                       static_record.location_type,
                                                       static_record.number_of_events,
                                                       static_record.location_group );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_region( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefRegion static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    uint8_t* current_pos;
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of Region record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of Region record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.description );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read description attribute of Region record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.region_type );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.source_file );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read sourceFile attribute of Region record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.begin_line_number );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read beginLineNumber attribute of Region record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.end_line_number );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read endLineNumber attribute of Region record. Invalid compression size." );
    }

    /*
     * Read record attributes added in version 1.1, if there is
     * space left in the record
     */
    OTF2_Buffer_GetPosition( reader->buffer, &current_pos );
    if ( current_pos < record_end_pos )
    {
        ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.canonical_name );
        if ( OTF2_SUCCESS != ret )
        {
            return UTILS_ERROR( ret, "Could not read canonicalName attribute of Region record. Invalid compression size." );
        }
        OTF2_Buffer_ReadUint8( reader->buffer, &static_record.region_role );
        OTF2_Buffer_ReadUint8( reader->buffer, &static_record.paradigm );
        ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.region_flags );
        if ( OTF2_SUCCESS != ret )
        {
            return UTILS_ERROR( ret, "Could not read regionFlags attribute of Region record. Invalid compression size." );
        }
    }
    else
    {
        otf2_global_def_region_convert_pre_1_1( &static_record );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.region )
    {
        interrupt = reader->reader_callbacks.region( reader->user_data,
                                                     static_record.self,
                                                     static_record.name,
                                                     static_record.canonical_name,
                                                     static_record.description,
                                                     static_record.region_role,
                                                     static_record.paradigm,
                                                     static_record.region_flags,
                                                     static_record.source_file,
                                                     static_record.begin_line_number,
                                                     static_record.end_line_number );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_callsite( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefCallsite static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of Callsite record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.source_file );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read sourceFile attribute of Callsite record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.line_number );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read lineNumber attribute of Callsite record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.entered_region );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read enteredRegion attribute of Callsite record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.left_region );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read leftRegion attribute of Callsite record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.callsite )
    {
        interrupt = reader->reader_callbacks.callsite( reader->user_data,
                                                       static_record.self,
                                                       static_record.source_file,
                                                       static_record.line_number,
                                                       static_record.entered_region,
                                                       static_record.left_region );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_callpath( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefCallpath static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of Callpath record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.parent );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parent attribute of Callpath record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.region );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read region attribute of Callpath record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.callpath )
    {
        interrupt = reader->reader_callbacks.callpath( reader->user_data,
                                                       static_record.self,
                                                       static_record.parent,
                                                       static_record.region );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_group( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefGroup static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    uint8_t* current_pos;
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of Group record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of Group record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.group_type_pre12 );
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.number_of_members );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read numberOfMembers attribute of Group record. Invalid compression size." );
    }
    static_record.members = malloc( static_record.number_of_members * sizeof( uint64_t ) );
    if ( static_record.number_of_members > 0 && !static_record.members )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Could not allocate memory for members array!" );
    }
    for ( uint32_t number_of_members_i = 0;
          number_of_members_i < static_record.number_of_members;
          number_of_members_i++ )
    {
        ret = OTF2_Buffer_ReadUint64( reader->buffer,
                                      static_record.members + number_of_members_i );
        if ( OTF2_SUCCESS != ret )
        {
            free( static_record.members );
            return UTILS_ERROR( ret, "Could not read members attribute of Group record. Invalid compression size." );
        }
    }

    /*
     * Read record attributes added in version 1.2, if there is
     * space left in the record
     */
    OTF2_Buffer_GetPosition( reader->buffer, &current_pos );
    if ( current_pos < record_end_pos )
    {
        OTF2_Buffer_ReadUint8( reader->buffer, &static_record.group_type );
        OTF2_Buffer_ReadUint8( reader->buffer, &static_record.paradigm );
        ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.group_flags );
        if ( OTF2_SUCCESS != ret )
        {
            return UTILS_ERROR( ret, "Could not read groupFlags attribute of Group record. Invalid compression size." );
        }
    }
    else
    {
        otf2_global_def_group_convert_pre_1_2( &static_record );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        free( static_record.members );
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.group )
    {
        interrupt = reader->reader_callbacks.group( reader->user_data,
                                                    static_record.self,
                                                    static_record.name,
                                                    static_record.group_type,
                                                    static_record.paradigm,
                                                    static_record.group_flags,
                                                    static_record.number_of_members,
                                                    static_record.members );
    }

    free( static_record.members );

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_metric_member( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefMetricMember static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of MetricMember record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of MetricMember record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.description );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read description attribute of MetricMember record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.metric_type );
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.metric_mode );
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.value_type );
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.metric_base );
    ret = OTF2_Buffer_ReadInt64( reader->buffer, &static_record.exponent );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read exponent attribute of MetricMember record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.unit );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read unit attribute of MetricMember record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.metric_member )
    {
        interrupt = reader->reader_callbacks.metric_member( reader->user_data,
                                                            static_record.self,
                                                            static_record.name,
                                                            static_record.description,
                                                            static_record.metric_type,
                                                            static_record.metric_mode,
                                                            static_record.value_type,
                                                            static_record.metric_base,
                                                            static_record.exponent,
                                                            static_record.unit );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_metric_class( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefMetricClass static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    uint8_t* current_pos;
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of MetricClass record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.number_of_metrics );
    static_record.metric_members = malloc( static_record.number_of_metrics * sizeof( OTF2_MetricMemberRef ) );
    if ( static_record.number_of_metrics > 0 && !static_record.metric_members )
    {
        return UTILS_ERROR( OTF2_ERROR_MEM_FAULT,
                            "Could not allocate memory for metricMembers array!" );
    }
    for ( uint8_t number_of_metrics_i = 0;
          number_of_metrics_i < static_record.number_of_metrics;
          number_of_metrics_i++ )
    {
        ret = OTF2_Buffer_ReadUint32( reader->buffer,
                                      static_record.metric_members + number_of_metrics_i );
        if ( OTF2_SUCCESS != ret )
        {
            free( static_record.metric_members );
            return UTILS_ERROR( ret, "Could not read metricMembers attribute of MetricClass record. Invalid compression size." );
        }
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.metric_occurrence );

    /*
     * Read record attributes added in version 1.2, if there is
     * space left in the record
     */
    OTF2_Buffer_GetPosition( reader->buffer, &current_pos );
    if ( current_pos < record_end_pos )
    {
        OTF2_Buffer_ReadUint8( reader->buffer, &static_record.recorder_kind );
    }
    else
    {
        otf2_global_def_metric_class_convert_pre_1_2( &static_record );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        free( static_record.metric_members );
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.metric_class )
    {
        interrupt = reader->reader_callbacks.metric_class( reader->user_data,
                                                           static_record.self,
                                                           static_record.number_of_metrics,
                                                           static_record.metric_members,
                                                           static_record.metric_occurrence,
                                                           static_record.recorder_kind );
    }

    free( static_record.metric_members );

    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_metric_instance( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefMetricInstance static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of MetricInstance record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.metric_class );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read metricClass attribute of MetricInstance record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.recorder );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read recorder attribute of MetricInstance record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.metric_scope );
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.scope );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read scope attribute of MetricInstance record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.metric_instance )
    {
        interrupt = reader->reader_callbacks.metric_instance( reader->user_data,
                                                              static_record.self,
                                                              static_record.metric_class,
                                                              static_record.recorder,
                                                              static_record.metric_scope,
                                                              static_record.scope );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_comm( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefComm static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of Comm record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of Comm record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.group );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read group attribute of Comm record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.parent );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read parent attribute of Comm record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.comm )
    {
        interrupt = reader->reader_callbacks.comm( reader->user_data,
                                                   static_record.self,
                                                   static_record.name,
                                                   static_record.group,
                                                   static_record.parent );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_parameter( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefParameter static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of Parameter record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of Parameter record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.parameter_type );

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.parameter )
    {
        interrupt = reader->reader_callbacks.parameter( reader->user_data,
                                                        static_record.self,
                                                        static_record.name,
                                                        static_record.parameter_type );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_rma_win( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefRmaWin static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.self );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read self attribute of RmaWin record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of RmaWin record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.comm );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read comm attribute of RmaWin record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.rma_win )
    {
        interrupt = reader->reader_callbacks.rma_win( reader->user_data,
                                                      static_record.self,
                                                      static_record.name,
                                                      static_record.comm );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_metric_class_recorder( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefMetricClassRecorder static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.metric_class );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read metricClass attribute of MetricClassRecorder record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint64( reader->buffer, &static_record.recorder );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read recorder attribute of MetricClassRecorder record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.metric_class_recorder )
    {
        interrupt = reader->reader_callbacks.metric_class_recorder( reader->user_data,
                                                                    static_record.metric_class,
                                                                    static_record.recorder );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_system_tree_node_property( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefSystemTreeNodeProperty static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.system_tree_node );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read systemTreeNode attribute of SystemTreeNodeProperty record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.name );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read name attribute of SystemTreeNodeProperty record. Invalid compression size." );
    }
    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.value );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read value attribute of SystemTreeNodeProperty record. Invalid compression size." );
    }

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.system_tree_node_property )
    {
        interrupt = reader->reader_callbacks.system_tree_node_property( reader->user_data,
                                                                        static_record.system_tree_node,
                                                                        static_record.name,
                                                                        static_record.value );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


static inline OTF2_ErrorCode
otf2_global_def_reader_read_system_tree_node_domain( OTF2_GlobalDefReader* reader )
{
    UTILS_ASSERT( reader );
    UTILS_ASSERT( reader->archive );

    OTF2_GlobalDefSystemTreeNodeDomain static_record;

    OTF2_ErrorCode ret;
    uint64_t       record_data_length;
    /* Get record length and test memory availability */
    ret = OTF2_Buffer_GuaranteeRecord( reader->buffer, &record_data_length );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }
    uint8_t* record_end_pos;
    OTF2_Buffer_GetPosition( reader->buffer, &record_end_pos );
    record_end_pos += record_data_length;

    ret = OTF2_Buffer_ReadUint32( reader->buffer, &static_record.system_tree_node );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read systemTreeNode attribute of SystemTreeNodeDomain record. Invalid compression size." );
    }
    OTF2_Buffer_ReadUint8( reader->buffer, &static_record.system_tree_domain );

    /*
     * Always jump to the announced end of the record. This way, we skip
     * future additions of attributes to this record.
     */
    ret = OTF2_Buffer_SetPosition( reader->buffer, record_end_pos );
    if ( OTF2_SUCCESS != ret )
    {
        return UTILS_ERROR( ret, "Could not read record of unknown type." );
    }

    OTF2_CallbackCode interrupt = OTF2_CALLBACK_SUCCESS;
    if ( reader->reader_callbacks.system_tree_node_domain )
    {
        interrupt = reader->reader_callbacks.system_tree_node_domain( reader->user_data,
                                                                      static_record.system_tree_node,
                                                                      static_record.system_tree_domain );
    }


    return OTF2_CALLBACK_SUCCESS == interrupt
           ? OTF2_SUCCESS
           : OTF2_ERROR_INTERRUPTED_BY_CALLBACK;
}


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

        case OTF2_GLOBAL_DEF_CLOCK_PROPERTIES:
            return otf2_global_def_reader_read_clock_properties( reader );

        case OTF2_GLOBAL_DEF_STRING:
            return otf2_global_def_reader_read_string( reader );

        case OTF2_GLOBAL_DEF_ATTRIBUTE:
            return otf2_global_def_reader_read_attribute( reader );

        case OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE:
            return otf2_global_def_reader_read_system_tree_node( reader );

        case OTF2_GLOBAL_DEF_LOCATION_GROUP:
            return otf2_global_def_reader_read_location_group( reader );

        case OTF2_GLOBAL_DEF_LOCATION:
            return otf2_global_def_reader_read_location( reader );

        case OTF2_GLOBAL_DEF_REGION:
            return otf2_global_def_reader_read_region( reader );

        case OTF2_GLOBAL_DEF_CALLSITE:
            return otf2_global_def_reader_read_callsite( reader );

        case OTF2_GLOBAL_DEF_CALLPATH:
            return otf2_global_def_reader_read_callpath( reader );

        case OTF2_GLOBAL_DEF_GROUP:
            return otf2_global_def_reader_read_group( reader );

        case OTF2_GLOBAL_DEF_METRIC_MEMBER:
            return otf2_global_def_reader_read_metric_member( reader );

        case OTF2_GLOBAL_DEF_METRIC_CLASS:
            return otf2_global_def_reader_read_metric_class( reader );

        case OTF2_GLOBAL_DEF_METRIC_INSTANCE:
            return otf2_global_def_reader_read_metric_instance( reader );

        case OTF2_GLOBAL_DEF_COMM:
            return otf2_global_def_reader_read_comm( reader );

        case OTF2_GLOBAL_DEF_PARAMETER:
            return otf2_global_def_reader_read_parameter( reader );

        case OTF2_GLOBAL_DEF_RMA_WIN:
            return otf2_global_def_reader_read_rma_win( reader );

        case OTF2_GLOBAL_DEF_METRIC_CLASS_RECORDER:
            return otf2_global_def_reader_read_metric_class_recorder( reader );

        case OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE_PROPERTY:
            return otf2_global_def_reader_read_system_tree_node_property( reader );

        case OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE_DOMAIN:
            return otf2_global_def_reader_read_system_tree_node_domain( reader );

        default:
            return otf2_global_def_reader_read_unknown( reader );
    }
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback(
    OTF2_GlobalDefReaderCallbacks*               globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_ClockProperties clockPropertiesCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->clock_properties = clockPropertiesCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetStringCallback(
    OTF2_GlobalDefReaderCallbacks*      globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_String stringCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->string = stringCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetAttributeCallback(
    OTF2_GlobalDefReaderCallbacks*         globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Attribute attributeCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->attribute = attributeCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback(
    OTF2_GlobalDefReaderCallbacks*              globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_SystemTreeNode systemTreeNodeCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->system_tree_node = systemTreeNodeCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback(
    OTF2_GlobalDefReaderCallbacks*             globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_LocationGroup locationGroupCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->location_group = locationGroupCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetLocationCallback(
    OTF2_GlobalDefReaderCallbacks*        globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Location locationCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->location = locationCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetRegionCallback(
    OTF2_GlobalDefReaderCallbacks*      globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Region regionCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->region = regionCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetCallsiteCallback(
    OTF2_GlobalDefReaderCallbacks*        globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Callsite callsiteCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->callsite = callsiteCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetCallpathCallback(
    OTF2_GlobalDefReaderCallbacks*        globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Callpath callpathCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->callpath = callpathCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetGroupCallback(
    OTF2_GlobalDefReaderCallbacks*     globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Group groupCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->group = groupCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetMetricMemberCallback(
    OTF2_GlobalDefReaderCallbacks*            globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_MetricMember metricMemberCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->metric_member = metricMemberCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetMetricClassCallback(
    OTF2_GlobalDefReaderCallbacks*           globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_MetricClass metricClassCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->metric_class = metricClassCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetMetricInstanceCallback(
    OTF2_GlobalDefReaderCallbacks*              globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_MetricInstance metricInstanceCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->metric_instance = metricInstanceCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetCommCallback(
    OTF2_GlobalDefReaderCallbacks*    globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Comm commCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->comm = commCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetParameterCallback(
    OTF2_GlobalDefReaderCallbacks*         globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_Parameter parameterCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->parameter = parameterCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetRmaWinCallback(
    OTF2_GlobalDefReaderCallbacks*      globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_RmaWin rmaWinCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->rma_win = rmaWinCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetMetricClassRecorderCallback(
    OTF2_GlobalDefReaderCallbacks*                   globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_MetricClassRecorder metricClassRecorderCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->metric_class_recorder = metricClassRecorderCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodePropertyCallback(
    OTF2_GlobalDefReaderCallbacks*                      globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_SystemTreeNodeProperty systemTreeNodePropertyCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->system_tree_node_property = systemTreeNodePropertyCallback;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeDomainCallback(
    OTF2_GlobalDefReaderCallbacks*                    globalDefReaderCallbacks,
    OTF2_GlobalDefReaderCallback_SystemTreeNodeDomain systemTreeNodeDomainCallback )
{
    if ( !globalDefReaderCallbacks )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid globalDefReaderCallbacks argument!" );
    }

    globalDefReaderCallbacks->system_tree_node_domain = systemTreeNodeDomainCallback;

    return OTF2_SUCCESS;
}
