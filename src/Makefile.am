# Copyright © CNRS, INRIA, Université Bordeaux 1
# See COPYING in top-level directory.

if HAVE_LIBZ
bin_PROGRAMS = gtg_compress gtg_decompress
endif

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
	  C/paje_GTGBasic1_c.c


if FORTRAN
libgtg_la_SOURCES += C/interfaceCFortranBasic1.c    \
		  FORTRAN/GTGBasic1_f.f90
endif

if USE_MPI
libgtg_la_CFLAGS += -DUSE_MPI
libgtg_la_LIBADD += -lmpi
endif

if HAVE_LIBZ
libgtg_la_CFLAGS += -DHAVE_LIBZ
libgtg_la_SOURCES += C/gtg_compress_core.c

endif

if USE_OTF
libgtg_la_CFLAGS += -DBUILD_OTF
libgtg_la_SOURCES += C/OTF_GTGBasic1_c.c
libgtg_la_LIBADD += -lotf
endif

if HAVE_LIBZ
gtg_compress_SOURCES = C/gtg_compress.c \
	               C/gtg_compress.h

gtg_compress_LDADD = -lz libgtg.la
gtg_compress_CPPFLAGS = -I$(srcdir)/../inc/

gtg_decompress_SOURCES = C/gtg_decompress.c \
	               C/gtg_compress.h

gtg_decompress_LDADD = -lz libgtg.la
gtg_decompress_CPPFLAGS = -I$(srcdir)/../inc/
endif

#noinst_HEADERS =  ev_codes.h
#include_HEADERS = GTGBasic1.h  OTF_GTGBasic1.h  pajeColor.h  paje_GTGBasic1.h  types.h
