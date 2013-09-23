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
 * @file       test/OTF2_AttributeList_test/OTF2_AttributeList_test.c
 * @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 * @status alpha
 *
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>

#include <otf2/otf2.h>

#include <UTILS_Error.h>
#include <UTILS_Debug.h>

#include <CuTest.h>

// CuTest's assert functions:
// void CuAssert(CuTest* tc, char* message, int condition);
// void CuAssertTrue(CuTest* tc, int condition);
// void CuAssertStrEquals(CuTest* tc, char* expected, char* actual);
// void CuAssertIntEquals(CuTest* tc, int expected, int actual);
// void CuAssertPtrEquals(CuTest* tc, void* expected, void* actual);
// void CuAssertPtrNotNull(CuTest* tc, void* pointer);


void
test_1( CuTest* tc )
{
    OTF2_AttributeList* attr_list = OTF2_AttributeList_New();
    CuAssertPtrNotNull( tc, attr_list );
    OTF2_AttributeList_Delete( attr_list );
}


void
test_2( CuTest* tc )
{
    OTF2_AttributeList* attr_list = OTF2_AttributeList_New();
    CuAssertPtrNotNull( tc, attr_list );

    OTF2_AttributeValue attr;
    attr.uint32 = 1;
    OTF2_ErrorCode ret = OTF2_AttributeList_AddAttribute( attr_list,
                                                          42,
                                                          OTF2_TYPE_UINT32,
                                                          attr );
    CuAssert( tc, "successful add", ret == OTF2_SUCCESS );

    bool res = OTF2_AttributeList_TestAttributeByID( attr_list, 42 );
    CuAssert( tc, "successful test id", res );

    uint32_t number_of_elements = OTF2_AttributeList_GetNumberOfElements( attr_list );
    CuAssert( tc, "number of entries", number_of_elements == 1 );

    OTF2_Type type;
    ret = OTF2_AttributeList_GetAttributeByID( attr_list,
                                               42,
                                               &type,
                                               &attr );
    CuAssert( tc, "successful get by id", ret == OTF2_SUCCESS );
    CuAssert( tc, "got correct type", type == OTF2_TYPE_UINT32 );
    CuAssert( tc, "got correct value", attr.uint32 == 1 );

    uint32_t id;
    ret = OTF2_AttributeList_GetAttributeByIndex( attr_list,
                                                  0,
                                                  &id,
                                                  &type,
                                                  &attr );
    CuAssert( tc, "successful get by index", ret == OTF2_SUCCESS );
    CuAssert( tc, "got correct id", id == 42 );
    CuAssert( tc, "got correct type", type == OTF2_TYPE_UINT32 );
    CuAssert( tc, "got correct value", attr.uint32 == 1 );

    ret = OTF2_AttributeList_PopAttribute( attr_list,
                                           &id,
                                           &type,
                                           &attr );
    CuAssert( tc, "successful pop", ret == OTF2_SUCCESS );
    CuAssert( tc, "got correct id", id == 42 );
    CuAssert( tc, "got correct type", type == OTF2_TYPE_UINT32 );
    CuAssert( tc, "got correct value", attr.uint32 == 1 );

    number_of_elements = OTF2_AttributeList_GetNumberOfElements( attr_list );
    CuAssert( tc, "number of entries", number_of_elements == 0 );

    OTF2_AttributeList_Delete( attr_list );
}


void
test_3( CuTest* tc )
{
    OTF2_AttributeList* attr_list = OTF2_AttributeList_New();
    CuAssertPtrNotNull( tc, attr_list );

    uint32_t ids_to_add[] = { 4, 8, 15, 16, 23, 42 };
    /* from the end, from the front, from the middle, as index into ids_to_add */
    uint32_t ids_to_remove[] = { 5, 0, 2, 1, 4, 3 };

    OTF2_ErrorCode      ret;
    bool                res;
    uint32_t            number_of_elements;
    OTF2_AttributeValue attr;
    for ( int i = 0; i < 6; i++ )
    {
        attr.uint32 = i;
        ret         = OTF2_AttributeList_AddAttribute( attr_list,
                                                       ids_to_add[ i ],
                                                       OTF2_TYPE_UINT32,
                                                       attr );
        CuAssert( tc, "successful add", ret == OTF2_SUCCESS );

        res = OTF2_AttributeList_TestAttributeByID( attr_list, ids_to_add[ i ] );
        CuAssert( tc, "successful test id", res );

        number_of_elements = OTF2_AttributeList_GetNumberOfElements( attr_list );
        CuAssert( tc, "number of entries", number_of_elements == ( i + 1 ) );
    }

    OTF2_Type type;
    for ( int i = 0; i < 6; i++ )
    {
        ret = OTF2_AttributeList_GetAttributeByID( attr_list,
                                                   ids_to_add[ ids_to_remove[ i ] ],
                                                   &type,
                                                   &attr );
        CuAssert( tc, "successful get by id", ret == OTF2_SUCCESS );
        CuAssert( tc, "got correct type", type == OTF2_TYPE_UINT32 );
        CuAssert( tc, "got correct value", attr.uint32 == ids_to_remove[ i ] );

        ret = OTF2_AttributeList_RemoveAttribute( attr_list,
                                                  ids_to_add[ ids_to_remove[ i ] ] );
        CuAssert( tc, "successful remove", ret == OTF2_SUCCESS );

        res = OTF2_AttributeList_TestAttributeByID( attr_list,
                                                    ids_to_add[ ids_to_remove[ i ] ] );
        CuAssert( tc, "unsuccessful test id", !res );

        number_of_elements = OTF2_AttributeList_GetNumberOfElements( attr_list );
        CuAssert( tc, "number of entries", number_of_elements == ( 6 - i - 1 ) );
    }

    OTF2_AttributeList_Delete( attr_list );
}


int
main()
{
    CuUseColors();
    CuString* output = CuStringNew();
    CuSuite*  suite  = CuSuiteNew( "AttributeList" );

    SUITE_ADD_TEST_NAME( suite, test_1, "new/delete" );
    SUITE_ADD_TEST_NAME( suite, test_2, "add/get/pop" );
    SUITE_ADD_TEST_NAME( suite, test_3, "random remove" );

    CuSuiteRun( suite );
    CuSuiteSummary( suite, output );

    int failCount = suite->failCount;
    if ( failCount )
    {
        printf( "%s", output->buffer );
    }

    CuSuiteFree( suite );
    CuStringFree( output );

    return failCount ? EXIT_FAILURE : EXIT_SUCCESS;
}
