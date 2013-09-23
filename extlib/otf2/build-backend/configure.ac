AC_PREREQ([2.68])
AC_INIT([OTF2], m4_esyscmd([../vendor/common/build-config/generate-package-version.sh ../build-config/VERSION]), [support@score-p.org], [otf2])

AC_SUBST([LIBRARY_INTERFACE_VERSION], m4_esyscmd([../vendor/common/build-config/generate-library-version.sh ../build-config/VERSION]))
AC_DEFINE_UNQUOTED([LIBRARY_INTERFACE_VERSION], ["$LIBRARY_INTERFACE_VERSION"], [Interface version number])

AC_CONFIG_AUX_DIR([../build-config])
AC_CONFIG_MACRO_DIR([../vendor/common/build-config/m4])

AC_CONFIG_HEADERS([../src/config-backend.h])

AC_SCOREP_SUMMARY_INIT([backend])

AC_DEFUN([AC_SCOREP_BACKEND], [])

## require $ac_scorep_cross_compiling to be set to 'yes' on 'no'
if test "x${ac_scorep_cross_compiling}" != "xyes" -a "x${ac_scorep_cross_compiling}" != "xno"; then
    AC_MSG_ERROR([invalid value '$ac_scorep_cross_compiling' for variable \$ac_scorep_cross_compiling.], [1])
fi
cross_compiling=${ac_scorep_cross_compiling}

ac_scorep_backend=yes
if test "x${ac_scorep_cross_compiling}" = "xyes"; then
    ac_scorep_frontend=no
else
    ac_scorep_frontend=yes
fi

AC_SCOREP_CHECK_COMPILER_VAR_SET([CC])
#AC_SCOREP_CHECK_COMPILER_VAR_SET([CXX])
#AC_SCOREP_CHECK_COMPILER_VAR_SET([F77])
#AC_SCOREP_CHECK_COMPILER_VAR_SET([FC])

AC_SCOREP_SVN_CONTROLLED

AM_INIT_AUTOMAKE([foreign color-tests 1.11.1 -Wall tar-pax nostdinc])
AM_SILENT_RULES([yes])

AC_SCOREP_COMMON_PACKAGE([otf2])

AM_CONDITIONAL([BUILD_TESTS], [test "x${ac_scorep_cross_compiling}" = "xno"])
AM_CONDITIONAL([CROSS_BUILD], [test "x${ac_scorep_cross_compiling}" = "xyes"])
AC_SCOREP_PLATFORM_SETTINGS   

AC_OTF2_COMMON_CHECKS

AC_SCOREP_BACKEND_TEST_RUNS

LT_PREREQ([2.4])
LT_INIT([disable-shared])
AC_SUBST([LIBTOOL_DEPS])

AC_SCOREP_BUILD_MODE
AC_SCOREP_PACKAGE_AND_LIBRARY_VERSION([../])

AC_SUBST([PASS_LINKER_FLAG_THROUGH_COMPILER], [$wl]) 

# On crosscompile machines, install backend stuff into $(bin|lid)dir/backend.
backend_suffix=""
AS_IF([test "x${ac_scorep_cross_compiling}" = "xyes"],
      [backend_suffix="/backend"])
AC_SUBST([backend_suffix])

dnl Predicate to use HARDCODE_LIBDIR_FLAG_* only if linker needs it. 
AS_IF([test "x${hardcode_into_libs}" = "xyes"], 
      [ac_scorep_hardcode_into_libs=1],
      [ac_scorep_hardcode_into_libs=0])
AC_SUBST([HARDCODE_INTO_LIBS], [${ac_scorep_hardcode_into_libs}])
dnl Flag to hardcode libdir into a binary during linking, C version.
AC_SUBST([HARDCODE_LIBDIR_FLAG_CC], [${hardcode_libdir_flag_spec}])
dnl Flag to hardcode libdir into a binary during linking, C++ version.
AC_SUBST([HARDCODE_LIBDIR_FLAG_CXX], [${hardcode_libdir_flag_spec_CXX}])
dnl The ${wl} part of the HARDCODE_LIBDIR_FLAG_* defines.
AC_SUBST([HARDCODE_LIBDIR_WL], [${wl}])
dnl The ${aix_libpath} part of the HARDCODE_LIBDIR_FLAG_* defines. Only available on AIX.
AC_SUBST([HARDCODE_LIBDIR_AIX_LIBPATH], [${aix_libpath}])

adl_RECURSIVE_EVAL([${libdir}], [LIB_DIR])
AC_SUBST([LIBDIR],[${LIB_DIR}])

AC_CONFIG_FILES([Makefile], [[
test "x${ac_scorep_cross_compiling}" = "xyes" && {
sed -e 's/_0 = @echo "  \(YACC\|GEN\|LEX\)/&  /; t; s/_0 = @echo "  /&BE/' \
Makefile >Makefile.be && mv -f Makefile.be Makefile || rm -f Makefile.be
}
]], [ac_scorep_cross_compiling='$ac_scorep_cross_compiling'])

AC_CONFIG_FILES([../src/otf2_config_tool_backend.h:../src/tools/otf2_config/otf2_config_tool_backend.h.in])

if test "x${ac_scorep_cross_compiling}" = "xno"; then
    AC_CONFIG_FILES([../src/otf2_config_tool_frontend.h:../src/tools/otf2_config/otf2_config_tool_frontend.h.in])
fi
AC_OUTPUT
