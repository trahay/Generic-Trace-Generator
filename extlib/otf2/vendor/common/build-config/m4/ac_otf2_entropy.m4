## -*- mode: autoconf -*-

##
## This file is part of the Score-P software (http://www.score-p.org)
##
## Copyright (c) 2009-2012,
##    RWTH Aachen University, Germany
##    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
##    Technische Universitaet Dresden, Germany
##    University of Oregon, Eugene, USA
##    Forschungszentrum Juelich GmbH, Germany
##    German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
##    Technische Universitaet Muenchen, Germany
##
## See the COPYING file in the package base directory for details.
##


## check fo some usefull entropy sources for the trace id.

AC_DEFUN([AC_OTF2_ENTROPY_SOURCES], [

AC_LANG_PUSH([C])
AC_SCOREP_TIMER_CHECK([clock_gettime])
AC_SCOREP_TIMER_CHECK([gettimeofday])

ac_otf2_entropy_sources=""
ac_otf2_entropy_sources_sep=""

otf2_timer_lib=""
AS_IF([test "x${ac_scorep_timer_clock_gettime_available}" = "xyes"], [
        ac_otf2_timer_lib=${ac_scorep_timer_librt}
        ac_otf2_entropy_sources="${ac_otf2_entropy_sources}${ac_otf2_entropy_sources_sep}clock_gettime"
        ac_otf2_entropy_sources_sep=" "
      ], [test "x${ac_scorep_timer_gettimeofday_available}" = "xyes"], [
        ac_otf2_entropy_sources="${ac_otf2_entropy_sources}${ac_otf2_entropy_sources_sep}gettimeofday"
        ac_otf2_entropy_sources_sep=" "
      ])
AC_SUBST([TIMER_LIB], ["$ac_otf2_timer_lib"])

AH_TEMPLATE([HAVE_UNISTD_H],
            [Define to 1 if you have the <unistd.h> header file.])
AH_TEMPLATE([HAVE_SYS_TYPES_H],
            [Define to 1 if you have the <sys/types.h> header file.])
AH_TEMPLATE([HAVE_GETPID],
            [Define to 1 if the getpid() function is available.])
AH_TEMPLATE([HAVE_SYS_SYSINFO_H],
            [Define to 1 if you have the <sys/sysinfo.h> header file.])
AH_TEMPLATE([HAVE_SYSINFO],
            [Define to 1 if the sysinfo() function is available.])

AC_MSG_CHECKING([for getpid])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <unistd.h>
#include <sys/types.h>
                                ]], [[
pid_t me = getpid();
                                ]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_UNISTD_H])
AC_DEFINE([HAVE_SYS_TYPES_H])
AC_DEFINE([HAVE_GETPID])
ac_otf2_entropy_sources="${ac_otf2_entropy_sources}${ac_otf2_entropy_sources_sep}getpid"
ac_otf2_entropy_sources_sep=" "
                                ], [
AC_MSG_RESULT([no])
                                ])

AC_MSG_CHECKING([for sysinfo])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/sysinfo.h>
                                ]], [[
struct sysinfo info;
sysinfo( &info );
                                ]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_SYS_SYSINFO_H])
AC_DEFINE([HAVE_SYSINFO])
ac_otf2_entropy_sources="${ac_otf2_entropy_sources}${ac_otf2_entropy_sources_sep}sysinfo"
ac_otf2_entropy_sources_sep=" "
                                ], [
AC_MSG_RESULT([no])
                                ])

AC_LANG_POP([C])

AS_IF([test -z "${ac_otf2_entropy_sources}"], [ac_otf2_entropy_sources=none])
AC_SCOREP_SUMMARY([Entropy sources], [${ac_otf2_entropy_sources}])
])
