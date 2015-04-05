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
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@

lib_LTLIBRARIES = libexe.la

libexe_la_SOURCES = \
	exe_file_header.h \
	exe_section_table.h \
	libexe.c \
	libexe_codepage.h \
	libexe_data_directory_descriptor.h \
	libexe_debug.c libexe_debug.h \
	libexe_debug_data.c libexe_debug_data.h \
	libexe_definitions.h \
	libexe_error.c libexe_error.h \
	libexe_export_table.c libexe_export_table.h \
	libexe_extern.h \
	libexe_file.c libexe_file.h \
	libexe_import_table.c libexe_import_table.h \
	libexe_io_handle.c libexe_io_handle.h \
	libexe_libbfio.h \
	libexe_libcdata.h \
	libexe_libcerror.h \
	libexe_libclocale.h \
	libexe_libcnotify.h \
	libexe_libcstring.h \
	libexe_libfcache.h \
	libexe_libfdata.h \
	libexe_libfdatetime.h \
	libexe_libuna.h \
	libexe_notify.c libexe_notify.h \
	libexe_section.c libexe_section.h \
	libexe_section_descriptor.c libexe_section_descriptor.h \
	libexe_section_io_handle.c libexe_section_io_handle.h \
	libexe_support.c libexe_support.h \
	libexe_types.h \
	libexe_unused.h

libexe_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@PTHREAD_LIBADD@

libexe_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libexe_definitions.h.in \
	libexe.rc \
	libexe.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libexe_definitions.h
	/bin/rm -f libexe.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libexe ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libexe_la_SOURCES)

