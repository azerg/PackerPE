AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBEXE_DLL_IMPORT@

if HAVE_PYTHON
TESTS_PYEXE = \
	test_pyexe_set_ascii_codepage.sh \
	test_pyexe_open_close.sh
endif

TESTS = \
	test_set_ascii_codepage.sh \
	test_open_close.sh \
	test_exeinfo.sh \
	$(TESTS_PYEXE)

check_SCRIPTS = \
	pyexe_test_open_close.py \
	pyexe_test_set_ascii_codepage.py \
	test_open_close.sh \
	test_pyexe_open_close.sh \
	test_pyexe_set_ascii_codepage.sh \
	test_exeinfo.sh \
	test_runner.sh \
	test_set_ascii_codepage.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS =  \
	exe_test_open_close \
	exe_test_set_ascii_codepage

exe_test_open_close_SOURCES = \
	exe_test_libcerror.h \
	exe_test_libexe.h \
	exe_test_libcstring.h \
	exe_test_open_close.c

exe_test_open_close_LDADD = \
	../libexe/libexe.la \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

exe_test_set_ascii_codepage_SOURCES = \
	exe_test_libcerror.h \
	exe_test_libclocale.h \
	exe_test_libexe.h \
	exe_test_libcstring.h \
	exe_test_set_ascii_codepage.c

exe_test_set_ascii_codepage_LDADD = \
	../libexe/libexe.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

