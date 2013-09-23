/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2011,
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
 * @file       test/OTF2_IdMap_test/OTF2_IdMap_test.c
 * @maintainer Christian R&ouml;ssel <c.roessel@fz-juelich.de>
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
    OTF2_IdMap* id_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, 10 );
    CuAssertPtrNotNull( tc, id_map );
    OTF2_IdMap_Free( id_map );
}


void
test_2( CuTest* tc )
{
    OTF2_IdMap* id_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, 100 );
    CuAssertPtrNotNull( tc, id_map );
    OTF2_IdMap_Free( id_map );
}


void
traverse( uint64_t localId,
          uint64_t globalId,
          void*    userData )
{
    ( ( uint64_t* )userData )[ globalId ] = localId;
}


void
test_3( CuTest* tc )
{
    OTF2_IdMap* id_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, 6 );
    CuAssertPtrNotNull( tc, id_map );

    OTF2_IdMap_AddIdPair( id_map, 42, 0 );
    OTF2_IdMap_AddIdPair( id_map, 13, 1 );
    OTF2_IdMap_AddIdPair( id_map,  0, 2 );

    uint64_t array[ 3 ];
    OTF2_IdMap_Traverse( id_map, traverse, array );

    CuAssertIntEquals( tc, 42, array[ 0 ] );
    CuAssertIntEquals( tc, 13, array[ 1 ] );
    CuAssertIntEquals( tc,  0, array[ 2 ] );

    OTF2_IdMap_Free( id_map );
}


void
test_4( CuTest* tc )
{
    uint64_t mappings[ 5 ] = { 0, 5, 2, 1, 4 };

    OTF2_IdMap* id_map = OTF2_IdMap_CreateFromUint64Array( 5, mappings, true );
    CuAssertPtrNotNull( tc, id_map );

    OTF2_IdMapMode mode;
    OTF2_ErrorCode status;
    status = OTF2_IdMap_GetMode( id_map, &mode );
    CuAssertIntEquals( tc, status, OTF2_SUCCESS );
    CuAssertIntEquals( tc, mode, OTF2_ID_MAP_SPARSE );

    for ( uint64_t i = 0; i < 5; i++ )
    {
        uint64_t globalId;
        status = OTF2_IdMap_GetGlobalId( id_map, i, &globalId );
        CuAssertIntEquals( tc, status, OTF2_SUCCESS );

        CuAssertIntEquals( tc, mappings[ i ],  globalId );
    }
    OTF2_IdMap_Free( id_map );
}


void
test_5( CuTest* tc )
{
    uint64_t mappings[ 5 ] = { 0, 5, 2, 1, 3 };

    OTF2_IdMap* id_map = OTF2_IdMap_CreateFromUint64Array( 5, mappings, true );
    CuAssertPtrNotNull( tc, id_map );

    OTF2_IdMapMode mode;
    OTF2_ErrorCode status;
    status = OTF2_IdMap_GetMode( id_map, &mode );
    CuAssertIntEquals( tc, status, OTF2_SUCCESS );
    CuAssertIntEquals( tc, mode, OTF2_ID_MAP_DENSE );

    for ( uint64_t i = 0; i < 5; i++ )
    {
        uint64_t globalId;
        status = OTF2_IdMap_GetGlobalId( id_map, i, &globalId );
        CuAssertIntEquals( tc, status, OTF2_SUCCESS );

        CuAssertIntEquals( tc, mappings[ i ],  globalId );
    }
    OTF2_IdMap_Free( id_map );
}


void
test_6( CuTest* tc )
{
    uint64_t    mappings[ 5 ] = { 0, 1, 2, 3, 4 };
    OTF2_IdMap* id_map;

    id_map = OTF2_IdMap_CreateFromUint64Array( 5, mappings, true );
    CuAssertPtrEquals( tc, id_map, NULL );

    id_map = OTF2_IdMap_CreateFromUint64Array( 0, mappings, true );
    CuAssertPtrEquals( tc, id_map, NULL );
}


void
test_7( CuTest* tc )
{
    OTF2_ErrorCode status;
    uint64_t       ids[ 5 ] = { 4, 0, 2, 1, 3 };

    OTF2_IdMap* id_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, 5 );
    CuAssertPtrNotNull( tc, id_map );

    for ( uint64_t i = 0; i < 5; i++ )
    {
        status = OTF2_IdMap_AddIdPair( id_map, ids[ i ], ids[ i ] );
        CuAssertIntEquals( tc, status, OTF2_SUCCESS );
    }

    for ( uint64_t i = 0; i < 5; i++ )
    {
        uint64_t globalId;
        status = OTF2_IdMap_GetGlobalId( id_map, i, &globalId );
        CuAssertIntEquals( tc, status, OTF2_SUCCESS );

        CuAssertIntEquals( tc, i,  globalId );
    }

    OTF2_IdMap_Free( id_map );
}


int
main()
{
    CuUseColors();
    CuString* output = CuStringNew();
    CuSuite*  suite  = CuSuiteNew( "ID map" );

    SUITE_ADD_TEST_NAME( suite, test_1, "create/destroy 10" );
    SUITE_ADD_TEST_NAME( suite, test_2, "create/destroy 100" );
    SUITE_ADD_TEST_NAME( suite, test_3, "add and traverse" );
    SUITE_ADD_TEST_NAME( suite, test_4, "optimize for size: SPARSE" );
    SUITE_ADD_TEST_NAME( suite, test_5, "optimize for size: DENSE" );
    SUITE_ADD_TEST_NAME( suite, test_6, "optimize for size: identity is NULL" );
    SUITE_ADD_TEST_NAME( suite, test_7, "binary insert" );

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
