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
 * @file       test/misc/otf2_undefineds.c
 * @maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>
 *
 * @status alpha
 *
 */


#include <config.h>


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#include <CuTest.h>


#include <otf2/OTF2_GeneralDefinitions.h>


void
test_1( CuTest* tc )
{
    CuAssert( tc, "UINT8", OTF2_UNDEFINED_UINT8 == UINT8_MAX );
}


void
test_2( CuTest* tc )
{
    CuAssert( tc, "UINT16", OTF2_UNDEFINED_UINT16 == UINT16_MAX );
}


void
test_3( CuTest* tc )
{
    CuAssert( tc, "UINT32", OTF2_UNDEFINED_UINT32 == UINT32_MAX );
}


void
test_4( CuTest* tc )
{
    CuAssert( tc, "UINT64", OTF2_UNDEFINED_UINT64 == UINT64_MAX );
}


int
main()
{
    CuUseColors();
    CuString* output = CuStringNew();
    CuSuite*  suite  = CuSuiteNew( "OTF2_UNDEFINED C" );

    SUITE_ADD_TEST_NAME( suite, test_1, "uint8_t" );
    SUITE_ADD_TEST_NAME( suite, test_2, "uint16_t" );
    SUITE_ADD_TEST_NAME( suite, test_3, "uint32_t" );
    SUITE_ADD_TEST_NAME( suite, test_4, "uint64_t" );

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
