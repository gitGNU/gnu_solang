noinst_LTLIBRARIES =	libimporter.la

libimporter_la_SOURCES = \
	directory-source.cpp \
	directory-source.h \
	flickr-source.cpp \
	flickr-source.h \
	importer.cpp \
	importer-dialog.cpp \
	importer-dialog.h \
	importer.h \
	photo-source.cpp \
	photo-source.h

AM_CPPFLAGS = \
       	-DPACKAGE_LOCALE_DIR=\"${SOLANG_LOCALE_DIR}\" \
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
	$(CAIROMM_CFLAGS) \
	$(EXIV2_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDAMM_CFLAGS) \
	$(GIOMM_CFLAGS) \
	$(GLIBMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(IMAGEMAGICK_CFLAGS) \
	$(SIGC_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
