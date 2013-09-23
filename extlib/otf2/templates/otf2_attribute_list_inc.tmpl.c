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
 *  @file       src/otf2_attribute_list_inc.c
 *  @source     templates/otf2_attribute_list_inc.tmpl.c
 *
 *  @status     alpha
 *
 *  @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 *  @brief      Convenient helper function to add and get attributes.
 */

@otf2 for entry in Type.entries if entry.type:

OTF2_ErrorCode
OTF2_AttributeList_Add@@entry.type.otf2@@( OTF2_AttributeList*  attributeList,
                                           OTF2_AttributeRef    attributeID,
                                           @@entry.type@@       @@entry.type.lower@@Value )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.@@entry.type.lower@@ = @@entry.type.lower@@Value;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            @@entry.name@@,
                                            attribute_value );
}

@otf2 endfor
@otf2 for entry in MappingType.entries:
@otf2 with def = entry.def

OTF2_ErrorCode
OTF2_AttributeList_Add@@def.ref_name@@Ref( OTF2_AttributeList*    attributeList,
                                           OTF2_AttributeRef      attributeID,
                                           @@def.self_attr.type@@ @@def.ref_lname@@Ref )
{
    OTF2_AttributeValue attribute_value;
    attribute_value.@@def.ref_lname@@Ref = @@def.ref_lname@@Ref;

    /* Let OTF2_AttributeList_AddAttribute do the error checking */
    return OTF2_AttributeList_AddAttribute( attributeList,
                                            attributeID,
                                            @@Type.prefix@@@@entry.suffix@@,
                                            attribute_value );
}

@otf2 endwith
@otf2 endfor
@otf2 for entry in Type.entries if entry.type:

OTF2_ErrorCode
OTF2_AttributeList_Get@@entry.type.otf2@@( const OTF2_AttributeList* attributeList,
                                           OTF2_AttributeRef         attributeID,
                                           @@entry.type@@*           @@entry.type.lower@@Value )
{
    if ( !@@entry.type.lower@@Value )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for @@entry.type.lower@@Value." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != @@entry.name@@ )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *@@entry.type.lower@@Value = attribute_value.@@entry.type.lower@@;

    return OTF2_SUCCESS;
}

@otf2 endfor
@otf2 for entry in MappingType.entries:
@otf2 with def = entry.def

OTF2_ErrorCode
OTF2_AttributeList_Get@@def.ref_name@@Ref( const OTF2_AttributeList*   attributeList,
                                           OTF2_AttributeRef           attributeID,
                                           @@def.self_attr.type@@*     @@def.ref_lname@@Ref )
{
    if ( !@@def.ref_lname@@Ref )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_ARGUMENT,
                            "NULL pointer for @@def.ref_lname@@Ref." );
    }

    OTF2_Type           type;
    OTF2_AttributeValue attribute_value;

    OTF2_ErrorCode status =
        OTF2_AttributeList_GetAttributeByID( attributeList,
                                             attributeID,
                                             &type,
                                             &attribute_value );

    if ( status != OTF2_SUCCESS )
    {
        return status;
    }

    if ( type != @@Type.prefix@@@@entry.suffix@@ )
    {
        return UTILS_ERROR( OTF2_ERROR_INVALID_DATA,
                            "Requested value does not match type." );
    }

    *@@def.ref_lname@@Ref = attribute_value.@@def.ref_lname@@Ref;

    return OTF2_SUCCESS;
}

@otf2 endwith
@otf2 endfor
