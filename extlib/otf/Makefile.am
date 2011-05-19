ACLOCAL_AMFLAGS = -I config/m4

if AMHAVESWIGPYTHON
OTFLIB_PY = otflib_py
else
OTFLIB_PY =
endif

SUBDIRS = \
	otflib \
	$(OTFLIB_PY) \
	tools \
	docu

EXTRA_DIST= \
	otf_vc08.sln \
	config/otf_get_version.sh \
	tests/hello/Makefile \
	tests/hello/hello_otf.c \
	tests/otf_python/groups.py \
	tests/otf_python/test_read.py \
	tests/otf_python/test_read_qt.py \
	tests/otf_python/test_write.py \
	tests/otf_python/python3/groups.py \
	tests/otf_python/python3/test_read.py \
	tests/otf_python/python3/test_read_qt.py \
	tests/otf_python/python3/test_write.py \
	tests/otf_read_from_buffer/hello_world.0.def--missing \
	tests/otf_read_from_buffer/hello_world.0.marker \
	tests/otf_read_from_buffer/hello_world.1.events \
	tests/otf_read_from_buffer/hello_world.2.events \
	tests/otf_read_from_buffer/hello_world.otf \
	tests/otf_read_from_buffer/Makefile \
	tests/otf_read_from_buffer/otf_read_from_buffer.c \
	VERSION
