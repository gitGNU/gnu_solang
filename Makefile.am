## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = data src po

INTLTOOL_FILES = \
        intltool-extract.in \
        intltool-merge.in \
        intltool-update.in

solangdocdir = ${prefix}/doc/solang
solangdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = \
        $(INTLTOOL_FILES) \
        $(solangdoc_DATA)

DISTCLEANFILES = \
        intltool-extract \
        intltool-merge \
        intltool-update \
        po/.intltool-merge-cache

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

