# Copyright © CNRS, INRIA, Université Bordeaux 1
# See COPYING in top-level directory.

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([GTG], [0.1], [gtg-devel@lists.gforge.inria.fr])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_FC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for header files.
AC_CHECK_HEADERS([stdio.h string.h stdlib.h])
AC_CHECK_HEADERS([mpi.h])

AC_OUTPUT([Makefile inc/Makefile src/Makefile])
