# Copyright © CNRS, INRIA, Université Bordeaux 1
# See COPYING in top-level directory.

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([GTG], [0.2], [gtg-devel@lists.gforge.inria.fr])
AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CC
AC_PROG_F77
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Correct install paths
AS_IF([test x"$prefix" = x -o x"$prefix" = "xNONE"], [prefix="/usr/local"])
AS_IF([test x"$exec_prefix" = x -o x"$exec_prefix" = "xNONE"], [exec_prefix="\${prefix}"])
AS_IF([test "$docdir" = "\${datarootdir}/doc/\${PACKAGE_TARNAME}"],
[docdir="\${datarootdir}/doc"])


# Checks for header files.
AC_CHECK_HEADERS([stdio.h string.h stdlib.h])

AC_CHECK_FUNCS([getline], have_getline=yes)
if [ test  "x$have_getline" = xyes ]; then
   CFLAGS="${CFLAGS} -DHAVE_GETLINE"
fi


use_libz=yes
AC_ARG_WITH(libz,
            [AS_HELP_STRING([--with-libz[=<bool>]], [Enable libZ])],
            [
                if test x$withval != xno; then
                  use_libz=yes
                else
                  use_libz=no
                fi
            ])


if [ test $use_libz = yes ]; then
  AC_CHECK_LIB(z, deflate, have_libz=yes)
fi
AM_CONDITIONAL([HAVE_LIBZ], [ test "x$have_libz" = xyes])

# crappy hack: of the macros AC_PROG_{CC,CXX,F77,FC}, only the
# first expanded one will check for object file extension etc,
# and fail if that doesn't work.

### No fortran by default.
# use --with-fortran to build fortran interface
use_fortran=yes
AC_ARG_WITH(fortran,
            [AS_HELP_STRING([--with-fortran[=<bool>]], [Enable fortran])],
            [
                if test x$withval != xno; then
                  use_fortran=yes
                else
                  use_fortran=no
                fi
            ])

if [ test $use_fortran = yes ]; then
   if test x$F77 = x; then
      AC_MSG_ERROR(A Fortran compiler is required)
   fi
fi

AM_CONDITIONAL([FORTRAN], [test "x$use_fortran" = xyes])

# MPI stuff
use_mpi=yes
AC_ARG_WITH(mpi, [AS_HELP_STRING([--with-mpi[=<dir>]], [Use a specific MPI installation])],
        [
                if test x$withval != xno; then
                        if test x$withval = xyes; then
                                AC_MSG_RESULT(yes)
                                use_mpi_from_system=yes
                        else
                                # use specified path
                                use_mpi_from_system=no
                                mpidir=$withval
                                AC_MSG_RESULT(yes using $mpidir)
                                AC_SUBST(MPIDIR, $mpidir)
                        fi
                else
			# either --without-mpi or --with-mpi=no is set
                        use_mpi=no
                fi
        ])

if test $use_mpi = yes ; then
   AC_SUBST(USE_MPIDIR_FROM_USER,$use_mpi_from_system)
   if test x$use_mpi_from_system = xno; then
        CPPFLAGS="${CPPFLAGS} -I$mpidir/include "
        LDFLAGS="${LDFLAGS} -L$mpidir/lib "
   fi

   # MPI header
   AC_CHECK_HEADER([mpi.h],[enable_mpi=yes],)
else
  enable_mpi=no
fi

AM_CONDITIONAL([USE_MPI], [test "x$enable_mpi" = xyes])




# OTF stuff
use_internal_otf=yes
otf_asked=no
use_otf=yes
AC_ARG_WITH(otf, [AS_HELP_STRING([--with-otf[=<dir>]], [Use a specific OTF installation])],
        [
                if test x$withval != xno; then
		   	otf_asked=yes
                        if test x$withval = xyes; then
                                AC_MSG_RESULT(yes)
				use_internal_otf=yes
                        else
                                # use specified path
				use_internal_otf=no
                                otfdir=$withval
                                AC_MSG_RESULT(yes using $otfdir)
                                AC_SUBST(OTFDIR, $otfdir)
				OTF_INC=$otfdir/include
				OTF_LIB=$otfdir/lib
                        fi
                else
			# either --without-otf or --with-otf=no is set
			use_internal_otf=no
                        use_otf=no
                fi
        ])

AC_ARG_WITH(otf-include, [AS_HELP_STRING([--with-otf-include[=<dir>]], [Use a specific OTF header directory])],
        [
                if test x$withval != xno; then
		   	otf_asked=yes
                        if test x$withval = xyes; then
                                AC_MSG_RESULT(yes)
				use_internal_otf=yes
                        else
                                # use specified path
				use_internal_otf=no
                                otf_inc_dir=$withval
                                AC_MSG_RESULT(yes using $otf_inc_dir)
				OTF_INC=$otf_inc_dir
                                AC_SUBST(OTF_INC, $otf_inc_dir)
                        fi
  		fi
        ])

AC_ARG_WITH(otf-lib, [AS_HELP_STRING([--with-otf-lib[=<dir>]], [Use a specific libotf.so directory])],
        [
                if test x$withval != xno; then
		   	otf_asked=yes
                        if test x$withval = xyes; then
                                AC_MSG_RESULT(yes)
				use_internal_otf=yes
                        else
                                # use specified path
				use_internal_otf=no
                                otf_lib_dir=$withval
                                AC_MSG_RESULT(yes using $otf_lib_dir)
				OTF_LIB=$otf_lib_dir
                                AC_SUBST(OTF_LIB, $otf_lib_dir)
                        fi
  		fi
        ])

if test $use_otf = yes ; then
   enable_otf=yes
   if test x$use_internal_otf = xno; then
      CPPFLAGS="${CPPFLAGS} -I$OTF_INC/"
      LDFLAGS="${LDFLAGS} -L$OTF_LIB"

   # Check for OTF header
      AC_CHECK_HEADER([otf.h],[enable_otf=yes], AC_MSG_WARN([Cannot find libOTF.]))
      if test $enable_otf = no && test $otf_asked = yes ; then
         AC_MSG_ERROR(Cannot find libOTF.)
      fi

   else

  dnl configure otf from extlib
    otf_parent_dir=`pwd`
    otf_dir="extlib/otf"
    AC_MSG_NOTICE([configuring in otf_dir ($otf_parent_dir/$otf_dir)])
    AS_IF([test "$srcdir" != "."],
                [
                        test -d "$otf_dir" ||
                        mkdir -p "$otf_dir" ||
                        AC_MSG_ERROR([cannot create $otf_dir])
                ])

                cd $otf_dir

                case $srcdir in
                        .)
                                otf_srcdir="$srcdir"
                                ;;
                        /*)
                                otf_srcdir="$srcdir/$otf_dir"
                                ;;
                        *)
                                otf_srcdir="../../$srcdir/$otf_dir"
                                ;;
                esac

                otf_conf_cmd="$otf_srcdir/configure"
   otf_conf_args="$otf_conf_args --prefix=\"$prefix\" --exec-prefix=\"$exec_prefix\" --bindir=\"$bindir\" --libdir=\"$libdir\" --includedir=\"$includedir\" --docdir=\"$docdir/otf\" $OTFFLAGS --cache-file=\"/dev/null\" --srcdir=\"$otf_srcdir\" --enable-shared=yes"
                AC_MSG_NOTICE([running $SHELL $otf_conf_cmd $otf_conf_args])
                eval "$SHELL '$otf_conf_cmd' $otf_conf_args"
                AS_IF([test $? != "0"], [AC_MSG_ERROR([$otf_conf_cmd failed for $otf_dir])])

                cd $otf_parent_dir
                AS_IF([test x"$OTFLIB" = x], [OTF_LDFLAGS="-lotf"])

#		OTF_CPPFLAGS="$OTF_CPPFLAGS -I${prefix}/include/otf"
#		CPPFLAGS="${CPPFLAGS} -I${prefix}/include/otf"
   fi
else
  enable_otf=no
fi

AM_CONDITIONAL([USE_OTF], [test "x$enable_otf" = xyes])

AC_MSG_WARN([use otf = $use_internal_otf])

AM_CONDITIONAL(DONT_USE_INTERNAL_OTF, test x"$use_internal_otf" != "xyes")


# OTF2 stuff
use_internal_otf2=yes
otf2_asked=no
use_otf2=yes
otf2_cflags="-I${srcdir}/extlib/otf2/include -I${top_srcdir}/extlib/otf2/include"
otf2_libs=""
AC_ARG_WITH(otf2, [AS_HELP_STRING([--with-otf2[=<dir>]], [Use a specific OTF2 installation])],
        [
                if test x$withval != xno; then
		   	otf2_asked=yes
                        if test x$withval = xyes; then
                                AC_MSG_RESULT(yes)
				use_internal_otf2=yes
                        else
                                # use specified path
				use_internal_otf2=no
                                otf2dir=$withval
                                AC_MSG_RESULT(yes using $otf2dir)
                                AC_SUBST(OTFDIR, $otf2dir)
				otf2_cflags="-I${otf2dir}/include"
				otf2_libs="-L$otf2dir/lib"
                        fi
                else
			# either --without-otf or --with-otf=no is set
			use_internal_otf2=no
                        use_otf2=no
                fi
        ])

AC_ARG_WITH(otf2-include, [AS_HELP_STRING([--with-otf2-include[=<dir>]], [Use a specific OTF2 header directory])],
        [
                if test x$withval != xno; then
		   	otf2_asked=yes
                        if test x$withval = xyes; then
                                AC_MSG_RESULT(yes)
				use_internal_otf2=yes
                        else
                                # use specified path
				use_internal_otf2=no
                                otf2_inc_dir=$withval
                                AC_MSG_RESULT(yes using $otf2_inc_dir)
				otf2_cflags="-I$otf2_inc_dir"
                        fi
  		fi
        ])

AC_ARG_WITH(otf2-lib, [AS_HELP_STRING([--with-otf2-lib[=<dir>]], [Use a specific libotf2.a directory])],
        [
                if test x$withval != xno; then
		   	otf2_asked=yes
                        if test x$withval = xyes; then
                                AC_MSG_RESULT(yes)
				use_internal_otf2=yes
                        else
                                # use specified path
				use_internal_otf2=no
                                otf2_lib_dir=$withval
                                AC_MSG_RESULT(yes using $otf2_lib_dir)
				otf2_libs="-L$otf2_lib_dir"
                        fi
  		fi
        ])

if test $use_otf2 = yes ; then
   enable_otf2=yes
   if test x$use_internal_otf2 = xno; then
      CPPFLAGS="${CPPFLAGS} ${otf2_cflags}"
      LDFLAGS="${LDFLAGS} ${otf2_libs}"

   # Check for OTF header
      AC_CHECK_HEADER([otf2.h],[enable_otf2=yes], AC_MSG_WARN([Cannot find libOTF2.]))
      if test $enable_otf2 = no && test $otf2_asked = yes ; then
         AC_MSG_ERROR(Cannot find libOTF2.)
      fi

   else

  dnl configure otf from extlib
    otf2_parent_dir=`pwd`
    otf2_dir="extlib/otf2"
    AC_MSG_NOTICE([configuring in otf2_dir ($otf2_parent_dir/$otf2_dir)])
    AS_IF([test "$srcdir" != "."],
                [
                        test -d "$otf2_dir" ||
                        mkdir -p "$otf2_dir" ||
                        AC_MSG_ERROR([cannot create $otf2_dir])
                ])

                cd $otf2_dir

                case $srcdir in
                        .)
                                otf2_srcdir="$srcdir"
                                ;;
                        /*)
                                otf2_srcdir="$srcdir/$otf2_dir"
                                ;;
                        *)
                                otf2_srcdir="../../$srcdir/$otf2_dir"
                                ;;
                esac

                otf2_conf_cmd="$otf2_srcdir/configure"

                otf2_conf_args="--enable-shared=yes --prefix=\"$prefix\" --exec-prefix=\"${exec_prefix}\" --includedir=\"${includedir}\" $OTF2FLAGS --cache-file=\"/dev/null\" --srcdir=\"${otf2_srcdir}\" --enable-shared=yes"
                AC_MSG_NOTICE([running "$SHELL $otf2_conf_cmd $otf2_conf_args"])
                eval "$SHELL '$otf2_conf_cmd' $otf2_conf_args"
                AS_IF([test $? != "0"], [AC_MSG_ERROR([$otf2_conf_cmd failed for $otf2_dir])])

                cd $otf2_parent_dir
                AS_IF([test x"$OTF2LIB" = x], [OTF2_LDFLAGS="-lotf2"])

#		OTF2_CPPFLAGS="$OTF2_CPPFLAGS -I${prefix}/include/otf2"
#		CPPFLAGS="${CPPFLAGS} -I${prefix}/include/otf2"
   fi
else
  enable_otf2=no
fi

AM_CONDITIONAL([USE_OTF2], [test "x$enable_otf2" = xyes])

AC_MSG_WARN([use otf2 = $use_internal_otf2])

AM_CONDITIONAL(DONT_USE_INTERNAL_OTF2, test x"$use_internal_otf2" != "xyes")

# # OTF2 stuff
# use_otf2=no
# AC_ARG_WITH(otf2, [AS_HELP_STRING([--with-otf2[=<dir>]], [Use a specific OTF2 installation])],
#         [
#                 if test x$withval != xno; then
# 		   	use_otf2=yes
#                         if test x$withval = xyes; then
#                                 AC_MSG_RESULT(yes)
#                         else
#                                 # use specified path
#                                 otf2dir=$withval
#                                 AC_MSG_RESULT(yes using $otf2dir)
#                                 AC_SUBST(OTF2DIR, $otf2dir)
# 
# #				AC_CHECK_PROG (OTF2_CONFIG, otf2-config, yes, no, [$otf2dir/bin])
# 				otf2_cflags=`$otf2dir/bin/otf2-config --cflags`
# 				otf2_libs="-L$otf2dir/lib -Wl,-rpath,$otf2dir/lib -lotf2"
#                         fi
#                 fi
#         ])
# 
# 
# AM_CONDITIONAL([USE_OTF2], [test "x$use_otf2" = xyes])
# 
# AC_MSG_WARN([use otf2 = $use_otf2])

AC_SUBST(OTF2_LIBS, $otf2_libs)
AC_SUBST(OTF2_CFLAGS, $otf2_cflags)





# Checks for programs
AC_CHECK_PROG(HAVE_DOXYGEN, [doxygen], yes, no)

if test x$HAVE_DOXYGEN = xno ; then
   AC_MSG_WARN([Cannot find doxygen])
fi

AC_ARG_VAR([PDFLATEX], [Location of the pdflatex program (required for building the hwloc doxygen documentation)])
AC_PATH_TOOL([PDFLATEX], [pdflatex], no)

AM_CONDITIONAL([USE_PDFLATEX], [test "x$PDFLATEX" != xno])
AM_CONDITIONAL([USE_DOXYGEN], [test "x$HAVE_DOXYGEN" = xyes])

#AC_CONFIG_SUBDIRS([extlib/otf extlib/otf2])

AC_CONFIG_FILES([Makefile \
		gtg.pc   \
           	extlib/Makefile \
           	inc/Makefile \
	   	src/Makefile \
	   	doc/Makefile \
	   	doc/Doxyfile])

#AC_CONFIG_SUBDIRS([extlib/otf extlib/otf2])

AC_OUTPUT
