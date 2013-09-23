## -*- mode: makefile -*-

## 
## This file is part of the Score-P software (http://www.score-p.org)
##
## Copyright (c) 2009-2013, 
##    RWTH Aachen, Germany
##    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
##    Technische Universitaet Dresden, Germany
##    University of Oregon, Eugene, USA
##    Forschungszentrum Juelich GmbH, Germany
##    German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
##    Technische Universitaet Muenchen, Germany
##
## See the COPYING file in the package base directory for details.
##

## file       build-backend/Makefile.am 
## maintainer Christian Roessel <c.roessel@fz-juelich.de>


## ACLOCAL_AMFLAGS contains options to pass to aclocal when aclocal.m4
## is to be rebuilt by make. This line is also used by autoreconf to
## run aclocal with suitable options, or by autopoint and gettextize
## to locate the place where Gettext's macros should be installed. So
## even if you do not really care about the rebuild rules, you should
## define ACLOCAL_AMFLAGS.
## For some reason this can't be moved to common.am.
ACLOCAL_AMFLAGS = -I ../vendor/common/build-config/m4

include ../build-includes/common.am

AM_CPPFLAGS += -DBACKEND_BUILD

bindir = @bindir@@backend_suffix@
libdir = @libdir@@backend_suffix@

if CROSS_BUILD

AM_CPPFLAGS += -DCROSS_BUILD
include ../build-includes/backend-only.am
include ../build-includes/front-and-backend.am

else

AM_CPPFLAGS += -DNOCROSS_BUILD
include ../build-includes/backend-only.am
include ../build-includes/front-and-backend.am
include ../build-includes/frontend-only.am

if BACKEND_TEST_RUNS
TESTS += $(SCOREP_TESTS)
endif

endif

if !BACKEND_TEST_RUNS
BUILT_SOURCES += scorep_tests

CLEANFILES += scorep_tests

scorep_tests: 
	$(AM_V_GEN)( \
	    echo "# Available tests:"; \
	    for i in $(SCOREP_TESTS); do echo "./$$i"; done; \
	) >$@
endif

## To be used by LIB_DIR_UTIL
BUILD_DIR=build-backend

install-data-local: 

uninstall-local:

if ! CROSS_BUILD
installcheck-local: installcheck-public-headers
endif
