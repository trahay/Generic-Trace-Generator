AC_PREREQ([2.68])
AC_INIT([OTF2], m4_esyscmd([vendor/common/build-config/generate-package-version.sh build-config/VERSION]), [support@score-p.org], [otf2])

AC_SCOREP_SUMMARY_INIT

AC_SCOREP_REVISION

AC_SCOREP_TOPLEVEL_ARGS

## Packages that do manual configuration or use the install program might need
## to tell configure where to find some other shell scripts by calling
## AC_CONFIG_AUX_DIR, though the default places it looks are correct for most
## cases.
## Need to be called early on configure.ac because other macros search tools
## in this directory.
AC_CONFIG_AUX_DIR([build-config])

## Packages that use aclocal should declare where local macros can be found
## using AC_CONFIG_MACRO_DIR.
AC_CONFIG_MACRO_DIR([vendor/common/build-config/m4])

## Set the default installation prefix to /opt/otf2 instead of /usr/local
AC_PREFIX_DEFAULT([/opt/otf2])

## Call not before AC_CONFIG_MACRO_DIR
AM_INIT_AUTOMAKE([foreign color-tests 1.11.1 -Wall tar-pax nostdinc])
AM_SILENT_RULES([yes])

AC_SCOREP_COMMON_PACKAGE([otf2], [subst-only])

# don't detect and load defaults in nested configures (e.g. otf2)
if test ! -n "$ac_scorep_platform_data_provided" -o "x${ac_scorep_platform_data_provided}" = "xno"; then
    AC_SCOREP_DETECT_PLATFORMS
    AC_SCOREP_WITH_COMPILER_SUITE
    # determine arguments for subdir configures
    args="`$AWK -f $srcdir/vendor/common/build-config/process_arguments.awk \
           $ac_scorep_compilers_frontend                                    \
           $ac_scorep_compilers_backend                                     \
           user_provided_configure_args`"
else
    AC_MSG_NOTICE([platform data provided by toplevel configure.])
    AC_SCOREP_SUMMARY([Platform], [$ac_scorep_platform (provided)])
    # determine arguments for subdir configures
    args="`$AWK -f $srcdir/vendor/common/build-config/process_arguments.awk \
           user_provided_configure_args`"
fi
   
AC_SCOREP_SVN_CONTROLLED

AC_SCOREP_UNCRUSTIFY

AC_SCOREP_DOXYGEN
AC_SCOREP_DOXYGEN_CONFIG_FILES

AC_SCOREP_DEBUG_OPTION

## Autoconf supports changing the names of programs when installing them. In
## order to use these transformations, configure.ac must call the macro
## AC_ARG_PROGRAM.
# implicitly called by ?
#AC_ARG_PROGRAM

# used for EXTRA_DIST in Makefile.am
AM_CONDITIONAL([CROSS_BUILD], [test "x${ac_scorep_cross_compiling}" = "xyes"])


# handle external package's subdir configures
#AC_CONFIG_SUBDIR_CUSTOM([vendor/utility], [${args} ac_scorep_platform=${ac_scorep_platform} ac_scorep_platform_data_provided=yes ac_scorep_cross_compiling=${ac_scorep_cross_compiling} ac_scorep_doxygen_distdir=${ac_scorep_doxygen_distdir}/vendor/utility])

# handle own package's subdir configures
AC_CONFIG_SUBDIR_CUSTOM([build-backend], [${args} ac_scorep_platform=${ac_scorep_platform} ac_scorep_cross_compiling=${ac_scorep_cross_compiling}])

if test "x${ac_scorep_cross_compiling}" = "xyes"; then
    AC_CONFIG_SUBDIR_CUSTOM([build-frontend], [${args} ac_scorep_platform=${ac_scorep_platform}])
fi

AM_PATH_PYTHON([2.5],, [:])
AM_CONDITIONAL([HAVE_PYTHON], [test "$PYTHON" != :])

AC_CONFIG_FILES([
    Makefile
    src/config-common.h:vendor/common/build-config/config-common.h
    share/otf2/__version__.py
])
AC_CONFIG_FILES(
    [src/tools/otf2_template/otf2-template:src/tools/otf2_template/otf2_template.py],
    [chmod +x src/tools/otf2_template/otf2-template])

# generate the OTF2 error codes header
AC_CONFIG_FILES(
    [include/otf2/OTF2_ErrorCodes.h:vendor/common/src/utils/exception/ErrorCodes.tmpl.h],
    [$srcdir/vendor/common/src/utils/exception/finalize_error_codes.sh OTF2 $srcdir/share/otf2/otf2.errors include/otf2/OTF2_ErrorCodes.h src/otf2_error_decls.gen.h])

AC_OUTPUT

AS_IF([test "x$ac_scorep_platform_data_provided" != "xyes"], [
    AC_SCOREP_SUMMARY_COLLECT
])
