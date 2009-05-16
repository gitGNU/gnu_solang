noinst_LTLIBRARIES =	librenderer.la

librenderer_la_SOURCES = \
	browser-renderer.cpp \
	browser-renderer.h \
	console-renderer.cpp \
	console-renderer.h \
	renderer.cpp \
	renderer.h \
	browser-model-column-record.cpp \
	browser-model-column-record.h \
	cell-renderer-info.cpp \
	cell-renderer-info.h \
	cell-renderer-thumbnail.cpp \
	cell-renderer-thumbnail.h \
	thumbnail-view.h \
	thumbnail-view.cpp

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
