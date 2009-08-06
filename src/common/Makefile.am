noinst_LTLIBRARIES =	libcommon.la

libcommon_la_SOURCES = \
	content-type-repo.cpp \
	content-type-repo.h \
	error.cpp \
	error.h \
	flickr-context.cpp \
	flickr-context.h \
	id-base.h \
	id-repo.cpp \
	id-repo.h \
	i-photo-destination.cpp \
	i-photo-destination.h \
	i-photo-source.cpp \
	i-photo-source.h \
	i-plugin.cpp \
	i-plugin.h \
	i-renderer.cpp \
	i-renderer.h \
	i-renderer-selector.cpp \
	i-renderer-selector.h \
	i-storage.cpp \
	i-storage.h \
	non-copyable.cpp \
	non-copyable.h \
	photo.cpp \
	photo.h \
	photo-search-criteria.cpp \
	photo-search-criteria.h \
	plugin.cpp \
	plugin.h \
	progress-dialog.cpp \
	progress-dialog.h \
	progress-observer.cpp \
	progress-observer.h \
	renderer-selector.h \
	spinner.cpp \
	spinner.h \
	spinner-dialog.cpp \
	spinner-dialog.h \
	spinner-tool-item.cpp \
	spinner-tool-item.h \
	thumbbuf-maker.cpp \
	thumbbuf-maker.h \
	types.h

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""${datadir}/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_builddir) \
	-I$(top_srcdir)/src/application \
	-I$(top_srcdir)/src/attribute \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/database \
	-I$(top_srcdir)/src/importer \
	-I$(top_srcdir)/src/renderer \
	-I$(top_srcdir)/src/storage \
	$(SOLANG_CFLAGS) \
	$(EXIV2_CFLAGS) \
	$(FLICKCURL_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDAMM_CFLAGS) \
	$(GIOMM_CFLAGS) \
	$(GLIBMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(SIGC_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
