if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

pyexec_LTLIBRARIES = pyexe.la

pyexe_la_SOURCES = \
	pyexe.c pyexe.h \
	pyexe_codepage.c pyexe_codepage.h \
	pyexe_error.c pyexe_error.h \
	pyexe_datetime.c pyexe_datetime.h \
	pyexe_file.c pyexe_file.h \
	pyexe_file_object_io_handle.c pyexe_file_object_io_handle.h \
	pyexe_integer.c pyexe_integer.h \
	pyexe_libbfio.h \
	pyexe_libcerror.h \
	pyexe_libclocale.h \
	pyexe_libcstring.h \
	pyexe_libexe.h \
	pyexe_section.c pyexe_section.h \
	pyexe_sections.c pyexe_sections.h \
	pyexe_python.h \
	pyexe_unused.h

pyexe_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libexe/libexe.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyexe_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyexe_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

EXTRA_DIST = \
	setup.py \
	setup.py.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f setup.py
	/bin/rm -f Makefile

