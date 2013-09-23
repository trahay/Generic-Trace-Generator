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
 * @file       utils_io_test.c
 * @maintainer Christian R&ouml;ssel <c.roessel@fz-juelich.de>
 *
 * @status alpha
 *
 */

#include <config.h>
#include <UTILS_IO.h>

#include <CuTest.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* *INDENT-OFF* */
/* *INDENT-ON*  */


void
io_test_1( CuTest* tc )
{
    char path[] = "";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "", path );
}

void
io_test_2( CuTest* tc )
{
    char path[] = "a";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "a", path );
}

void
io_test_3( CuTest* tc )
{
    char path[] = "abc";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "abc", path );
}

void
io_test_4( CuTest* tc )
{
    char path[] = "//";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/", path );
}

void
io_test_5( CuTest* tc )
{
    char path[] = "foo/../bar";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./bar", path );
}

void
io_test_6( CuTest* tc )
{
    char path[] = "/foo/../bar";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/bar", path );
}

void
io_test_7( CuTest* tc )
{
    char path[] = "foo/bar/../../foobar";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./foobar", path );
}

void
io_test_8( CuTest* tc )
{
    char path[] = "bar/../../foobar";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "../foobar", path );
}

void
io_test_9( CuTest* tc )
{
    char path[] = "foo/bar/../baz/../../foobar";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./foobar", path );
}

void
io_test_10( CuTest* tc )
{
    char path[] = "//foo/..///bar///////////baz";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/bar/baz", path );
}

void
io_test_11( CuTest* tc )
{
    char path[] = "../../foo/../bar/baz/foobar";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "../../bar/baz/foobar", path );
}

void
io_test_12( CuTest* tc )
{
    char path[] = "./../foo/../bar";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "../bar", path );
}

void
io_test_13( CuTest* tc )
{
    char path[] = "./foo/bar/../baz";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./foo/baz", path );
}

void
io_test_14( CuTest* tc )
{
    char path[] = "./foo/bar/./././.././../baz";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./baz", path );
}

void
io_test_15( CuTest* tc )
{
    char path[] = ".";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, ".", path );
}

void
io_test_16( CuTest* tc )
{
    char path[] = "./";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./", path );
}

void
io_test_17( CuTest* tc )
{
    char path[] = "./.";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "", path );
}

void
io_test_18( CuTest* tc )
{
    char path[] = "./..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "++failed++", path );
}

void
io_test_19( CuTest* tc )
{
    char path[] = "../.";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "++failed++", path );
}

void
io_test_20( CuTest* tc )
{
    char path[] = "./.././/";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "++failed++", path );
}

void
io_test_21( CuTest* tc )
{
    char path[] = "dir/..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "", path );
}

void
io_test_22( CuTest* tc )
{
    char path[] = "dir/sub/../..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "", path );
}

void
io_test_23( CuTest* tc )
{
    char path[] = "dir/sub/../../..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "..", path );
}

void
io_test_24( CuTest* tc )
{
    char path[] = "dir";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "dir", path );
}

void
io_test_25( CuTest* tc )
{
    char path[] = "dir//";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "dir/", path );
}

void
io_test_26( CuTest* tc )
{
    char path[] = "./dir";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./dir", path );
}

void
io_test_27( CuTest* tc )
{
    char path[] = "dir/.";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "dir/.", path );
}

void
io_test_28( CuTest* tc )
{
    char path[] = "dir///./";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "dir/", path );
}

void
io_test_29( CuTest* tc )
{
    char path[] = "dir//sub/..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./dir", path );
}

void
io_test_30( CuTest* tc )
{
    char path[] = "dir/sub/../";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "dir/", path );
}

void
io_test_31( CuTest* tc )
{
    char path[] = "dir/sub/../.";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "dir/.", path );
}

void
io_test_32( CuTest* tc )
{
    char path[] = "dir/s1/../s2/";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "dir/s2/", path );
}

void
io_test_33( CuTest* tc )
{
    char path[] = "d1/s1///s2/..//../s3/";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "d1/s3/", path );
}

void
io_test_34( CuTest* tc )
{
    char path[] = "d1/s1//../s2/../../d2";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "./d2", path );
}

void
io_test_35( CuTest* tc )
{
    char path[] = "d1/.../d2";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "d1/.../d2", path );
}

void
io_test_36( CuTest* tc )
{
    char path[] = "d1/..././../d2";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "d1/d2", path );
}

void
io_test_37( CuTest* tc )
{
    char path[] = "/";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/", path );
}

void
io_test_38( CuTest* tc )
{
    char path[] = "//";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/", path );
}

void
io_test_39( CuTest* tc )
{
    char path[] = "///";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/", path );
}

void
io_test_40( CuTest* tc )
{
    char path[] = "/.";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/.", path );
}

void
io_test_41( CuTest* tc )
{
    char path[] = "/./";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/", path );
}

void
io_test_42( CuTest* tc )
{
    char path[] = "/./..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "++failed++", path );
}

void
io_test_43( CuTest* tc )
{
    char path[] = "/../.";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "++failed++", path );
}

void
io_test_44( CuTest* tc )
{
    char path[] = "/./.././/";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "++failed++", path );
}

void
io_test_45( CuTest* tc )
{
    char path[] = "/dir/..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/", path );
}

void
io_test_46( CuTest* tc )
{
    char path[] = "/dir/sub/../..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/", path );
}

void
io_test_47( CuTest* tc )
{
    char path[] = "/dir/sub/../../..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "++failed++", path );
}

void
io_test_48( CuTest* tc )
{
    char path[] = "/dir";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir", path );
}

void
io_test_49( CuTest* tc )
{
    char path[] = "/dir//";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir/", path );
}

void
io_test_50( CuTest* tc )
{
    char path[] = "/./dir";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir", path );
}

void
io_test_51( CuTest* tc )
{
    char path[] = "/dir/.";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir/.", path );
}

void
io_test_52( CuTest* tc )
{
    char path[] = "/dir///./";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir/", path );
}

void
io_test_53( CuTest* tc )
{
    char path[] = "/dir//sub/..";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir", path );
}

void
io_test_54( CuTest* tc )
{
    char path[] = "/dir/sub/../";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir/", path );
}

void
io_test_55( CuTest* tc )
{
    char path[] = "//dir/sub/../.";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir/.", path );
}

void
io_test_56( CuTest* tc )
{
    char path[] = "/dir/s1/../s2/";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/dir/s2/", path );
}

void
io_test_57( CuTest* tc )
{
    char path[] = "/d1/s1///s2/..//../s3/";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/d1/s3/", path );
}

void
io_test_58( CuTest* tc )
{
    char path[] = "/d1/s1//../s2/../../d2";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/d2", path );
}

void
io_test_59( CuTest* tc )
{
    char path[] = "/d1/.../d2";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/d1/.../d2", path );
}

void
io_test_60( CuTest* tc )
{
    char path[] = "/d1/..././../d2";
    UTILS_IO_SimplifyPath( path );
    CuAssertStrEquals( tc, "/d1/d2", path );
}


void
io_test_61( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 0 );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "", joined_path );
    free( joined_path );
}


void
io_test_62( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 1, "" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "", joined_path );
    free( joined_path );
}


void
io_test_63( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 2, "", "" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "", joined_path );
    free( joined_path );
}


void
io_test_64( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 1, NULL );
    CuAssertPtrEquals( tc, NULL, joined_path );
}


void
io_test_65( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 2, "/", "/" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "/", joined_path );
    free( joined_path );
}


void
io_test_66( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 3, "a", "b", "c" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "a/b/c", joined_path );
    free( joined_path );
}


void
io_test_67( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 3, "a", "b/c", "d" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "a/b/c/d", joined_path );
    free( joined_path );
}


void
io_test_68( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 3, "a", "/b/c", "d" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "/b/c/d", joined_path );
    free( joined_path );
}


void
io_test_69( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 3, "a", "b/c", "/d" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "/d", joined_path );
    free( joined_path );
}


void
io_test_70( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 3, "a", "/b/c", "/d" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "/d", joined_path );
    free( joined_path );
}


void
io_test_71( CuTest* tc )
{
    char* joined_path = UTILS_IO_JoinPath( 2, "a", "" );
    CuAssertPtrNotNull( tc, joined_path );
    CuAssertStrEquals( tc, "a", joined_path );
    free( joined_path );
}


int
main()
{
    CuUseColors();
    CuString* output = CuStringNew();
    CuSuite*  suite  = CuSuiteNew( "IO tools" );

    SUITE_ADD_TEST_NAME( suite, io_test_1, "simplify path \"\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_2, "simplify path \"a\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_3, "simplify path \"abc\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_4, "simplify path \"//\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_5, "simplify path \"foo/../bar\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_6, "simplify path \"/foo/../bar\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_7, "simplify path \"foo/bar/../../foobar\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_8, "simplify path \"bar/../../foobar\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_9, "simplify path \"foo/bar/../baz/../../foobar\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_10, "simplify path \"//foo/..///bar///////////baz\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_11, "simplify path \"../../foo/../bar/baz/foobar\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_12, "simplify path \"./../foo/../bar\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_13, "simplify path \"./foo/bar/../baz\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_14, "simplify path \"./foo/bar/./././.././../baz\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_15, "simplify path \".\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_16, "simplify path \"./\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_17, "simplify path \"./.\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_18, "simplify path \"./..\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_19, "simplify path \"../.\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_20, "simplify path \"./.././/\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_21, "simplify path \"dir/..\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_22, "simplify path \"dir/sub/../..\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_23, "simplify path \"dir/sub/../../..\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_24, "simplify path \"dir\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_25, "simplify path \"dir//\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_26, "simplify path \"./dir\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_27, "simplify path \"dir/.\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_28, "simplify path \"dir///./\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_29, "simplify path \"dir//sub/..\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_30, "simplify path \"dir/sub/../\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_31, "simplify path \"dir/sub/../.\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_32, "simplify path \"dir/s1/../s2/\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_33, "simplify path \"d1/s1///s2/..//../s3/\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_34, "simplify path \"d1/s1//../s2/../../d2\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_35, "simplify path \"d1/.../d2\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_36, "simplify path \"d1/..././../d2\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_37, "simplify path \"/\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_38, "simplify path \"//\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_39, "simplify path \"///\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_40, "simplify path \"/.\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_41, "simplify path \"/./\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_42, "simplify path \"/./..\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_43, "simplify path \"/../.\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_44, "simplify path \"/./.././/\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_45, "simplify path \"/dir/..\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_46, "simplify path \"/dir/sub/../..\"" );
    //SUITE_ADD_TEST_NAME( suite, io_test_47, "simplify path \"/dir/sub/../../..\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_48, "simplify path \"/dir\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_49, "simplify path \"/dir//\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_50, "simplify path \"/./dir\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_51, "simplify path \"/dir/.\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_52, "simplify path \"/dir///./\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_53, "simplify path \"/dir//sub/..\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_54, "simplify path \"/dir/sub/../\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_55, "simplify path \"//dir/sub/../.\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_56, "simplify path \"/dir/s1/../s2/\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_57, "simplify path \"/d1/s1///s2/..//../s3/\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_58, "simplify path \"/d1/s1//../s2/../../d2\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_59, "simplify path \"/d1/.../d2\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_60, "simplify path \"/d1/..././../d2\"" );

    SUITE_ADD_TEST_NAME( suite, io_test_61, "join path" );
    SUITE_ADD_TEST_NAME( suite, io_test_62, "join path \"\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_63, "join path \"\", \"\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_64, "join path NULL" );
    SUITE_ADD_TEST_NAME( suite, io_test_65, "join path \"/\", \"/\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_66, "join path \"a\", \"b\", \"c\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_67, "join path \"a\", \"b/c\", \"d\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_68, "join path \"a\", \"/b/c\", \"d\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_69, "join path \"a\", \"b/c\", \"/d\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_70, "join path \"a\", \"/b/c\", \"/d\"" );
    SUITE_ADD_TEST_NAME( suite, io_test_71, "join path \"a\", \"\"" );


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
