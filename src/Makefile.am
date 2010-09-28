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
	  FORTRAN/GTGBasic1_f.f90     \
          C/interfaceCFortranBasic1.c    \
	  C/paje_GTGBasic1_c.c

if USE_OTF
libgtg_la_CFLAGS += -DBUILD_OTF
libgtg_la_SOURCES += C/OTF_GTGBasic1_c.c
libgtg_la_LIBADD += -lotf
endif


#noinst_HEADERS =  ev_codes.h
#include_HEADERS = GTGBasic1.h  OTF_GTGBasic1.h  pajeColor.h  paje_GTGBasic1.h  types.h
