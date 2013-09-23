## -*- mode: autoconf -*-

## 
## This file is part of the Score-P software (http://www.score-p.org)
##
## Copyright (c) 2009-2011, 
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


# needs to be called after LT_INIT, otherwise enable_(static|shared) are not set
AC_DEFUN([AC_SCOREP_BUILD_MODE], [

AM_CONDITIONAL([BUILD_SHARED_LIBRARIES], [test "x${enable_shared}" = "xyes"])
AM_CONDITIONAL([BUILD_STATIC_LIBRARIES], [test "x${enable_static}" = "xyes"])

if test "x${enable_shared}" = "xyes"; then
    AC_DEFINE([SCOREP_SHARED_BUILD], [], [Defined if we are building shared libraries. See also SCOREP_STATIC_BUILD])
fi

if test "x${enable_static}" = "xyes"; then
    AC_DEFINE([SCOREP_STATIC_BUILD], [], [Defined if we are building static libraries. See also SCOREP_SHARED_BUILD])
fi

])
