noinst_LTLIBRARIES =	libimporter.la

libimporter_la_SOURCES = \
	camera-import-widget.cpp \
	camera-import-widget.h \
	camera-source.cpp \
	camera-source.h \
	directory-source.cpp \
	directory-source.h \
	flickr-auth-dialog.cpp \
	flickr-auth-dialog.h \
	flickr-chooser-button.cpp \
	flickr-chooser-button.h \
	flickr-chooser-dialog.cpp \
	flickr-chooser-dialog.h \
	flickr-enums.h \
	flickr-photo-uri-builder.cpp \
	flickr-photo-uri-builder.h \
	flickr-source.cpp \
	flickr-source.h \
	gp-context.cpp \
	gp-context.h \
	importer.cpp \
	importer-dialog.cpp \
	importer-dialog.h \
	importer.h \
	photo-source.cpp \
	photo-source.h \
	photo-source-enums.h

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""${datadir}/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_builddir) \
	-I$(top_srcdir)/src/application \
	-I$(top_srcdir)/src/attribute \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/database \
	-I$(top_srcdir)/src/editor \
	-I$(top_srcdir)/src/importer \
	-I$(top_srcdir)/src/renderer \
	-I$(top_srcdir)/src/storage \
	$(SOLANG_CFLAGS) \
	$(EXIV2_CFLAGS) \
	$(FLICKCURL_CFLAGS) \
	$(GPHOTO2_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDAMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(SOUP_CFLAGS) \
	$(WEBKIT_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
