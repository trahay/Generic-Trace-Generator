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


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteClockProperties( OTF2_GlobalDefWriter* writerHandle,
                                           uint64_t              timerResolution,
                                           uint64_t              globalOffset,
                                           uint64_t              traceLength )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( uint64_t ) + 1; /* timerResolution */
    record_data_length += sizeof( uint64_t ) + 1; /* globalOffset */
    record_data_length += sizeof( uint64_t ) + 1; /* traceLength */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_CLOCK_PROPERTIES );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, timerResolution );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, globalOffset );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, traceLength );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteString( OTF2_GlobalDefWriter* writerHandle,
                                  OTF2_StringRef        self,
                                  const char*           string )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_StringRef ) + 1; /* self */
    if ( !string )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid string argument." );
    }
    record_data_length += strlen( string ) + 1; /* string */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_STRING );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteString( writerHandle->buffer, string );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteAttribute( OTF2_GlobalDefWriter* writerHandle,
                                     OTF2_AttributeRef     self,
                                     OTF2_StringRef        name,
                                     OTF2_Type             type )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_AttributeRef ) + 1; /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;    /* name */
    record_data_length += sizeof( OTF2_Type );             /* type */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_ATTRIBUTE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, type );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteSystemTreeNode( OTF2_GlobalDefWriter*  writerHandle,
                                          OTF2_SystemTreeNodeRef self,
                                          OTF2_StringRef         name,
                                          OTF2_StringRef         className,
                                          OTF2_SystemTreeNodeRef parent )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_SystemTreeNodeRef ) + 1; /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;         /* name */
    record_data_length += sizeof( OTF2_StringRef ) + 1;         /* className */
    record_data_length += sizeof( OTF2_SystemTreeNodeRef ) + 1; /* parent */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, className );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, parent );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteLocationGroup( OTF2_GlobalDefWriter*  writerHandle,
                                         OTF2_LocationGroupRef  self,
                                         OTF2_StringRef         name,
                                         OTF2_LocationGroupType locationGroupType,
                                         OTF2_SystemTreeNodeRef systemTreeParent )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_LocationGroupRef ) + 1;  /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;         /* name */
    record_data_length += sizeof( OTF2_LocationGroupType );     /* locationGroupType */
    record_data_length += sizeof( OTF2_SystemTreeNodeRef ) + 1; /* systemTreeParent */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_LOCATION_GROUP );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, locationGroupType );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, systemTreeParent );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteLocation( OTF2_GlobalDefWriter* writerHandle,
                                    OTF2_LocationRef      self,
                                    OTF2_StringRef        name,
                                    OTF2_LocationType     locationType,
                                    uint64_t              numberOfEvents,
                                    OTF2_LocationGroupRef locationGroup )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_LocationRef ) + 1;      /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;        /* name */
    record_data_length += sizeof( OTF2_LocationType );         /* locationType */
    record_data_length += sizeof( uint64_t ) + 1;              /* numberOfEvents */
    record_data_length += sizeof( OTF2_LocationGroupRef ) + 1; /* locationGroup */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_LOCATION );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint64( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, locationType );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, numberOfEvents );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, locationGroup );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;
    writerHandle->number_of_locations++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteRegion( OTF2_GlobalDefWriter* writerHandle,
                                  OTF2_RegionRef        self,
                                  OTF2_StringRef        name,
                                  OTF2_StringRef        canonicalName,
                                  OTF2_StringRef        description,
                                  OTF2_RegionRole       regionRole,
                                  OTF2_Paradigm         paradigm,
                                  OTF2_RegionFlag       regionFlags,
                                  OTF2_StringRef        sourceFile,
                                  uint32_t              beginLineNumber,
                                  uint32_t              endLineNumber )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    /* Generate pre 1.1 record fields */
    OTF2_RegionType regionType;
    otf2_global_def_region_provide_pre_1_1( &regionType,
                                            name,
                                            canonicalName,
                                            description,
                                            regionRole,
                                            paradigm,
                                            regionFlags,
                                            sourceFile,
                                            beginLineNumber,
                                            endLineNumber );

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

    record_data_length += sizeof( OTF2_RegionRef ) + 1;  /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;  /* name */
    record_data_length += sizeof( OTF2_StringRef ) + 1;  /* description */
    record_data_length += sizeof( OTF2_RegionType );     /* regionType */
    record_data_length += sizeof( OTF2_StringRef ) + 1;  /* sourceFile */
    record_data_length += sizeof( uint32_t ) + 1;        /* beginLineNumber */
    record_data_length += sizeof( uint32_t ) + 1;        /* endLineNumber */
    record_data_length += sizeof( OTF2_StringRef ) + 1;  /* canonicalName */
    record_data_length += sizeof( OTF2_RegionRole );     /* regionRole */
    record_data_length += sizeof( OTF2_Paradigm );       /* paradigm */
    record_data_length += sizeof( OTF2_RegionFlag ) + 1; /* regionFlags */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_REGION );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, description );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, regionType );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, sourceFile );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, beginLineNumber );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, endLineNumber );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, canonicalName );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, regionRole );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, paradigm );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, regionFlags );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteCallsite( OTF2_GlobalDefWriter* writerHandle,
                                    OTF2_CallsiteRef      self,
                                    OTF2_StringRef        sourceFile,
                                    uint32_t              lineNumber,
                                    OTF2_RegionRef        enteredRegion,
                                    OTF2_RegionRef        leftRegion )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_CallsiteRef ) + 1; /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;   /* sourceFile */
    record_data_length += sizeof( uint32_t ) + 1;         /* lineNumber */
    record_data_length += sizeof( OTF2_RegionRef ) + 1;   /* enteredRegion */
    record_data_length += sizeof( OTF2_RegionRef ) + 1;   /* leftRegion */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_CALLSITE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, sourceFile );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, lineNumber );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, enteredRegion );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, leftRegion );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteCallpath( OTF2_GlobalDefWriter* writerHandle,
                                    OTF2_CallpathRef      self,
                                    OTF2_CallpathRef      parent,
                                    OTF2_RegionRef        region )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_CallpathRef ) + 1; /* self */
    record_data_length += sizeof( OTF2_CallpathRef ) + 1; /* parent */
    record_data_length += sizeof( OTF2_RegionRef ) + 1;   /* region */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_CALLPATH );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, parent );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, region );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteGroup( OTF2_GlobalDefWriter* writerHandle,
                                 OTF2_GroupRef         self,
                                 OTF2_StringRef        name,
                                 OTF2_GroupType        groupType,
                                 OTF2_Paradigm         paradigm,
                                 OTF2_GroupFlag        groupFlags,
                                 uint32_t              numberOfMembers,
                                 const uint64_t*       members )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

    /* Generate pre 1.2 record fields */
    OTF2_GroupTypePre12 groupTypePre12;
    otf2_global_def_group_provide_pre_1_2( &groupTypePre12,
                                           name,
                                           groupType,
                                           paradigm,
                                           groupFlags,
                                           numberOfMembers,
                                           members );

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

    record_data_length += sizeof( OTF2_GroupRef ) + 1;                  /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;                 /* name */
    record_data_length += sizeof( OTF2_GroupTypePre12 );                /* groupTypePre12 */
    record_data_length += sizeof( uint32_t ) + 1;                       /* numberOfMembers */
    record_data_length += numberOfMembers * ( sizeof( uint64_t ) + 1 ); /* members */
    if ( numberOfMembers > 0 && !members )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid members array argument." );
    }
    record_data_length += sizeof( OTF2_GroupType );     /* groupType */
    record_data_length += sizeof( OTF2_Paradigm );      /* paradigm */
    record_data_length += sizeof( OTF2_GroupFlag ) + 1; /* groupFlags */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_GROUP );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, groupTypePre12 );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, numberOfMembers );
    for ( uint32_t numberOfMembers_i = 0;
          numberOfMembers_i < numberOfMembers;
          numberOfMembers_i++ )
    {
        OTF2_Buffer_WriteUint64( writerHandle->buffer,
                                 members[ numberOfMembers_i ] );
    }
    OTF2_Buffer_WriteUint8( writerHandle->buffer, groupType );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, paradigm );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, groupFlags );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteMetricMember( OTF2_GlobalDefWriter* writerHandle,
                                        OTF2_MetricMemberRef  self,
                                        OTF2_StringRef        name,
                                        OTF2_StringRef        description,
                                        OTF2_MetricType       metricType,
                                        OTF2_MetricMode       metricMode,
                                        OTF2_Type             valueType,
                                        OTF2_MetricBase       metricBase,
                                        int64_t               exponent,
                                        OTF2_StringRef        unit )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_MetricMemberRef ) + 1; /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;       /* name */
    record_data_length += sizeof( OTF2_StringRef ) + 1;       /* description */
    record_data_length += sizeof( OTF2_MetricType );          /* metricType */
    record_data_length += sizeof( OTF2_MetricMode );          /* metricMode */
    record_data_length += sizeof( OTF2_Type );                /* valueType */
    record_data_length += sizeof( OTF2_MetricBase );          /* metricBase */
    record_data_length += sizeof( int64_t ) + 1;              /* exponent */
    record_data_length += sizeof( OTF2_StringRef ) + 1;       /* unit */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_METRIC_MEMBER );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, description );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, metricType );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, metricMode );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, valueType );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, metricBase );
    OTF2_Buffer_WriteInt64( writerHandle->buffer, exponent );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, unit );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteMetricClass( OTF2_GlobalDefWriter*       writerHandle,
                                       OTF2_MetricRef              self,
                                       uint8_t                     numberOfMetrics,
                                       const OTF2_MetricMemberRef* metricMembers,
                                       OTF2_MetricOccurrence       metricOccurrence,
                                       OTF2_RecorderKind           recorderKind )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_MetricRef ) + 1;                             /* self */
    record_data_length += sizeof( uint8_t );                                        /* numberOfMetrics */
    record_data_length += numberOfMetrics * ( sizeof( OTF2_MetricMemberRef ) + 1 ); /* metricMembers */
    if ( numberOfMetrics > 0 && !metricMembers )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid metricMembers array argument." );
    }
    record_data_length += sizeof( OTF2_MetricOccurrence ); /* metricOccurrence */
    record_data_length += sizeof( OTF2_RecorderKind );     /* recorderKind */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_METRIC_CLASS );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, numberOfMetrics );
    for ( uint8_t numberOfMetrics_i = 0;
          numberOfMetrics_i < numberOfMetrics;
          numberOfMetrics_i++ )
    {
        OTF2_Buffer_WriteUint32( writerHandle->buffer,
                                 metricMembers[ numberOfMetrics_i ] );
    }
    OTF2_Buffer_WriteUint8( writerHandle->buffer, metricOccurrence );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, recorderKind );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteMetricInstance( OTF2_GlobalDefWriter* writerHandle,
                                          OTF2_MetricRef        self,
                                          OTF2_MetricRef        metricClass,
                                          OTF2_LocationRef      recorder,
                                          OTF2_MetricScope      metricScope,
                                          uint64_t              scope )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_MetricRef ) + 1;   /* self */
    record_data_length += sizeof( OTF2_MetricRef ) + 1;   /* metricClass */
    record_data_length += sizeof( OTF2_LocationRef ) + 1; /* recorder */
    record_data_length += sizeof( OTF2_MetricScope );     /* metricScope */
    record_data_length += sizeof( uint64_t ) + 1;         /* scope */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_METRIC_INSTANCE );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, metricClass );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, recorder );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, metricScope );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, scope );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteComm( OTF2_GlobalDefWriter* writerHandle,
                                OTF2_CommRef          self,
                                OTF2_StringRef        name,
                                OTF2_GroupRef         group,
                                OTF2_CommRef          parent )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_CommRef ) + 1;   /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1; /* name */
    record_data_length += sizeof( OTF2_GroupRef ) + 1;  /* group */
    record_data_length += sizeof( OTF2_CommRef ) + 1;   /* parent */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_COMM );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, group );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, parent );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteParameter( OTF2_GlobalDefWriter* writerHandle,
                                     OTF2_ParameterRef     self,
                                     OTF2_StringRef        name,
                                     OTF2_ParameterType    parameterType )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_ParameterRef ) + 1; /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1;    /* name */
    record_data_length += sizeof( OTF2_ParameterType );    /* parameterType */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_PARAMETER );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, parameterType );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteRmaWin( OTF2_GlobalDefWriter* writerHandle,
                                  OTF2_RmaWinRef        self,
                                  OTF2_StringRef        name,
                                  OTF2_CommRef          comm )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_RmaWinRef ) + 1; /* self */
    record_data_length += sizeof( OTF2_StringRef ) + 1; /* name */
    record_data_length += sizeof( OTF2_CommRef ) + 1;   /* comm */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_RMA_WIN );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, self );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, comm );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteMetricClassRecorder( OTF2_GlobalDefWriter* writerHandle,
                                               OTF2_MetricRef        metricClass,
                                               OTF2_LocationRef      recorder )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_MetricRef ) + 1;   /* metricClass */
    record_data_length += sizeof( OTF2_LocationRef ) + 1; /* recorder */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_METRIC_CLASS_RECORDER );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, metricClass );
    OTF2_Buffer_WriteUint64( writerHandle->buffer, recorder );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteSystemTreeNodeProperty( OTF2_GlobalDefWriter*  writerHandle,
                                                  OTF2_SystemTreeNodeRef systemTreeNode,
                                                  OTF2_StringRef         name,
                                                  OTF2_StringRef         value )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_SystemTreeNodeRef ) + 1; /* systemTreeNode */
    record_data_length += sizeof( OTF2_StringRef ) + 1;         /* name */
    record_data_length += sizeof( OTF2_StringRef ) + 1;         /* value */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE_PROPERTY );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, systemTreeNode );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, name );
    OTF2_Buffer_WriteUint32( writerHandle->buffer, value );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}


OTF2_ErrorCode
OTF2_GlobalDefWriter_WriteSystemTreeNodeDomain( OTF2_GlobalDefWriter*  writerHandle,
                                                OTF2_SystemTreeNodeRef systemTreeNode,
                                                OTF2_SystemTreeDomain  systemTreeDomain )
{
    if ( !writerHandle )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "Invalid writerHandle argument." );
    }

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

    record_data_length += sizeof( OTF2_SystemTreeNodeRef ) + 1; /* systemTreeNode */
    record_data_length += sizeof( OTF2_SystemTreeDomain );      /* systemTreeDomain */
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

    OTF2_Buffer_WriteUint8( writerHandle->buffer, OTF2_GLOBAL_DEF_SYSTEM_TREE_NODE_DOMAIN );

    /* Write initial record length */
    OTF2_Buffer_WriteInitialRecordLength( writerHandle->buffer, record_data_length );

    OTF2_Buffer_WriteUint32( writerHandle->buffer, systemTreeNode );
    OTF2_Buffer_WriteUint8( writerHandle->buffer, systemTreeDomain );

    /* Write real record length */
    status = OTF2_Buffer_WriteFinalRecordLength( writerHandle->buffer, record_data_length );
    if ( OTF2_SUCCESS != status )
    {
        return status;
    }

    writerHandle->number_of_global_defs++;

    return OTF2_SUCCESS;
}
