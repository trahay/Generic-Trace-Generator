
## The SUBDIRS variable holds a list of subdirectories automake (recursivly)
## must build.
## first build in the top level, so that we can check for template updates
SUBDIRS = . $(subdirs) $(subdirs_custom)

## ACLOCAL_AMFLAGS contains options to pass to aclocal when aclocal.m4 is to be
## rebuilt by make. This line is also used by autoreconf to run aclocal with
## suitable options, or by autopoint and gettextize to locate the place where
## Gettext's macros should be installed. So even if you do not really care about
## the rebuild rules, you should define ACLOCAL_AMFLAGS.
ACLOCAL_AMFLAGS = -I vendor/common/build-config/m4

include vendor/common/build-config/Makefile.inc.am
include vendor/common/build-config/platforms.am

## Still, sometimes there are files that must be distributed, but which are not
## covered in the automatic rules. These files should be listed in the
## EXTRA_DIST variable. You can mention files from subdirectories in EXTRA_DIST.
##
## You can also mention a directory in EXTRA_DIST. In this case the entire
## directory will be recursively copied into the distribution. Please note that
## this will also copy everything in the directory, including CVS/RCS version
## control files. We recommend against using this feature.
EXTRA_DIST += $(srcdir)/src/config.h                              \
              $(srcdir)/src/config-custom.h                       \
              $(srcdir)/doc/doxygen-user/A1_installation.dox      \
              $(srcdir)/doc/doxygen-user/main_1.dox.in            \
              $(srcdir)/doc/doxygen-user/main_2.dox.in            \
              $(srcdir)/doc/doxygen-user/main_3.dox.in            \
              $(srcdir)/doc/doxygen-user/main_4.dox.in            \
              $(srcdir)/doc/doxygen-user/main_5.dox.in            \
              $(srcdir)/doc/doxygen-user/definitions.dox          \
              $(srcdir)/doc/doxygen-user/events.dox               \
              $(srcdir)/doc/doxygen-user/snaps.dox                \
              $(srcdir)/doc/doxygen-user/otf2_config_usage.dox.in \
              $(srcdir)/doc/doxygen-user/otf2_print_usage.dox.in  \
              $(srcdir)/doc/doxygen-user/otf2_marker_usage.dox.in \
              $(srcdir)/doc/doxygen-user/otf2_snapshots_usage.dox.in


if ! CROSS_BUILD
EXTRA_DIST += $(srcdir)/build-frontend/configure.ac \
              $(srcdir)/build-frontend/aclocal.m4   \
              $(srcdir)/build-frontend/Makefile.am  \
              $(srcdir)/build-frontend/Makefile.in  \
              $(srcdir)/build-frontend/configure    \
              $(srcdir)/src/config-frontend.h.in
endif

CLEANFILES += doc/doxygen-user/main.dox

CLEANFILES += \
    src/otf2_print_usage.h \
    src/otf2_config_usage.h \
    src/otf2_snapshots_usage.h \
    src/otf2_marker_usage.h

BUILT_SOURCES += \
    src/otf2_print_usage.h \
    src/otf2_config_usage.h \
    src/otf2_snapshots_usage.h \
    src/otf2_marker_usage.h

src/otf2_print_usage.h: $(srcdir)/doc/doxygen-user/otf2_print_usage.dox.in
	$(AM_V_GEN)$(AWK) '{print "\"" $$0 "\\n\""}' $< >$@

src/otf2_config_usage.h: $(srcdir)/doc/doxygen-user/otf2_config_usage.dox.in
	$(AM_V_GEN)$(AWK) '{print "\"" $$0 "\\n\""}' $< >$@

src/otf2_snapshots_usage.h: $(srcdir)/doc/doxygen-user/otf2_snapshots_usage.dox.in
	$(AM_V_GEN)$(AWK) '{print "\"" $$0 "\\n\""}' $< >$@

src/otf2_marker_usage.h: $(srcdir)/doc/doxygen-user/otf2_marker_usage.dox.in
	$(AM_V_GEN)$(AWK) '{print "\"" $$0 "\\n\""}' $< >$@

if SVN_CONTROLLED
doc/doxygen-user/main.dox: $(srcdir)/doc/doxygen-user/main_1.dox.in \
    $(srcdir)/doc/doxygen-user/otf2_config_usage.dox.in \
    $(srcdir)/doc/doxygen-user/main_2.dox.in \
    $(srcdir)/doc/doxygen-user/otf2_print_usage.dox.in \
    $(srcdir)/doc/doxygen-user/main_3.dox.in \
    $(srcdir)/doc/doxygen-user/otf2_snapshots_usage.dox.in \
    $(srcdir)/doc/doxygen-user/main_4.dox.in \
    $(srcdir)/doc/doxygen-user/otf2_marker_usage.dox.in \
    $(srcdir)/doc/doxygen-user/main_5.dox.in
	$(AM_V_GEN)$(MKDIR_P) `dirname $@` && \
	cat $^ >$@
else
doc/doxygen-user/main.dox:
	$(MKDIR_P) doc/doxygen-user
	touch doc/doxygen-user/main.dox
endif

install-data-local: doxygen-user-install

uninstall-hook: common-uninstall

dist-hook: prohibit-svn-files-in-dist \
           doc/doxygen-user/main.dox \
           doxygen-user-generate \
           dist-touch-template-outputs

clean-local: doxygen-user-clean

dist_bin_SCRIPTS=
pkgdata_DATA=
noinst_DATA=

DISTCLEANFILES += \
    include/otf2/OTF2_ErrorCodes.h \
    src/otf2_error_decls.gen.h

BUILT_SOURCES += \
    include/otf2/OTF2_ErrorCodes.h \
    src/otf2_error_decls.gen.h

include/otf2/OTF2_ErrorCodes.h: $(srcdir)/share/otf2/otf2.errors
src/otf2_error_decls.gen.h: $(srcdir)/share/otf2/otf2.errors

include share/Makefile.inc.am
include src/tools/otf2_template/Makefile.inc.am
include vendor/python/Makefile.inc.am

include templates/Makefile.inc.am

