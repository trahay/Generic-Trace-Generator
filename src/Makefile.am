# Copyright © CNRS, INRIA, Université Bordeaux 1
# See COPYING in top-level directory.

# todo:
# bin_PROGRAMS = cpaje cpaje2 cvite fpaje

# todo:
# cpaje_SOURCES = paje_example.c
# cpaje_LDADD = libgtg.la @LIBS@


lib_LTLIBRARIES = libgtg.la

libgtg_la_CPPFLAGS = -I$(srcdir)/../inc/
libgtg_la_CFLAGS = -Wall -pedantic -DBUILD_PAJE
libgtg_la_LIBADD =
libgtg_la_LDFLAGS = --no-undefined

libgtg_la_SOURCES = \
	  C/GTGBasic1_c.c     \
	  C/GTGColor.c     \
	  C/paje_header.c	\
	  C/paje_GTGBasic1_c.c

if FORTRAN
libgtg_la_SOURCES += C/interfaceCFortranBasic1.c    \
		  FORTRAN/GTGBasic1_f.f90
endif

if USE_MPI
libgtg_la_CFLAGS += -DUSE_MPI
libgtg_la_LIBADD += -lmpi
endif

if USE_OTF

if DONT_USE_INTERNAL_OTF
OTFINCLUDE =
OTFLIBADD = -lotf
OTFDEPENDENCIES =
else
OTFINCLUDE = -I$(top_srcdir)/extlib/otf/otflib -I$(top_builddir)/extlib/otf/otflib
OTFLIBADD = $(top_builddir)/extlib/otf/otflib/libotf.la
OTFDEPENDENCIES = $(OTFLIBADD)
endif

libgtg_la_CFLAGS += -DBUILD_OTF $(OTFINCLUDE)
libgtg_la_SOURCES += C/OTF_GTGBasic1_c.c
libgtg_la_LIBADD += $(OTFLIBADD)
libgtg_la_DEPENDENCIES = $(OTFDEPENDENCIES)
endif


#noinst_HEADERS =  ev_codes.h
#include_HEADERS = GTGBasic.h  GTGOTF_Basic.h  GTGPaje_Color.h GTGPaje_Basic.h GTGtypes.h
