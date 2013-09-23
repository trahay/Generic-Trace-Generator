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

AC_DEFUN([AC_SCOREP_COMMON_PACKAGE], [
AC_REQUIRE([AM_INIT_AUTOMAKE])

m4_define([$1_UP], [m4_toupper($1)])
m4_define([$1_DOWN], [m4_tolower($1)])

m4_ifval([$2], [], [
    _AC_SRCDIRS([.])
    AC_DEFINE_UNQUOTED([PACKAGE_SRCDIR],   ["$ac_abs_srcdir/.."], [Source dir])
    AC_DEFINE_UNQUOTED([PACKAGE_BUILDDIR], ["$ac_abs_builddir"],  [Build dir])

    AC_DEFINE_UNQUOTED(
        [PACKAGE_SYM],
        $1_DOWN,
        [The package name usable as a symbol.])

    AC_DEFINE_UNQUOTED(
        [PACKAGE_SYM_CAPS],
        $1_UP,
        [The package name usable as a symbol in all caps.])
])

AC_SUBST([PACKAGE_SYM],      $1_DOWN)
AC_SUBST([PACKAGE_SYM_CAPS], $1_UP)

AH_TEMPLATE([HAVE_]$1_UP[_DEBUG],
            [Define to 1 to enable internal debug messages (like NDEBUG).])
AH_TEMPLATE([HAVE_]$1_UP[_NO_ASSERT],
            [Define to 1 to disable assertions (like NDEBUG).])
])

# AC_SCOREP_DEFINE_HAVE(VARIABLE, VALUE[, DESCRIPTION])
# ------------------------------------------------------
# Like AC_DEFINE, but prepends the HAVE_ prefix and also defines the
# HAVE_BACKEND_ variant, if in cross mode.
#
AC_DEFUN([AC_SCOREP_DEFINE_HAVE], [
AC_DEFINE(HAVE_[]$1,         [$2], [$3])
AC_DEFINE(HAVE_BACKEND_[]$1, [$2], [$3])
])

# AC_SCOREP_COND_HAVE(VARIABLE, CONDITION[, DESCRIPTION[, COND_TRUE[, COND_FALSE]]])
# --------------------------------------------------
# Convenience macro to define a AM_CONDITIONAL and always a
# AC_SCOREP_DEFINE_HAVE at once. VARIABLE will be prefixed with HAVE_
#
AC_DEFUN([AC_SCOREP_COND_HAVE], [
AM_CONDITIONAL(HAVE_[]$1, [$2])
AM_COND_IF(HAVE_[]$1,
           [AC_SCOREP_DEFINE_HAVE([$1], [1], [$3])
            $4],
           [AC_SCOREP_DEFINE_HAVE([$1], [0], [$3])
            $5])
])
