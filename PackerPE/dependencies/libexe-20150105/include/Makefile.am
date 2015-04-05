include_HEADERS = \
        libexe.h

pkginclude_HEADERS = \
        libexe/codepage.h \
        libexe/definitions.h \
        libexe/error.h \
        libexe/extern.h \
        libexe/features.h \
        libexe/types.h

EXTRA_DIST = \
	libexe.h.in \
	libexe/definitions.h.in \
	libexe/features.h.in \
	libexe/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libexe.h
	/bin/rm -f libexe/definitions.h
	/bin/rm -f libexe/features.h
	/bin/rm -f libexe/types.h
	/bin/rm -f Makefile

