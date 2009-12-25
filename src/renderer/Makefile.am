noinst_LTLIBRARIES =	librenderer.la

librenderer_la_SOURCES = \
	browser-renderer.cpp \
	browser-renderer.h \
	console-renderer.cpp \
	console-renderer.h \
	editor-renderer.cpp \
	editor-renderer.h \
	enlarged-renderer.cpp \
	enlarged-renderer.h \
	browser-model-column-record.cpp \
	browser-model-column-record.h \
	cell-renderer-thumbnail.cpp \
	cell-renderer-thumbnail.h \
	pagination-bar.cpp \
	pagination-bar.h \
	slideshow-renderer.cpp \
	slideshow-renderer.h \
	slideshow-window.cpp \
	slideshow-window.h \
	thumbnail-view.h \
	thumbnail-view.cpp

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
	-I$(top_srcdir)/src/edit-engine \
	-I$(top_srcdir)/src/importer \
	-I$(top_srcdir)/src/renderer \
	-I$(top_srcdir)/src/storage \
	$(SOLANG_CFLAGS) \
	$(DBUS_CFLAGS) \
	$(GTKIMAGEVIEW_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(BABL_CFLAGS) \
	$(GEGL_CFLAGS) \
	$(TRACKER_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
