noinst_LTLIBRARIES =	libeditor.la

libeditor_la_SOURCES = \
	brightness-widget.cpp \
	brightness-widget.h \
	contrast-widget.cpp \
	contrast-widget.h \
	desaturate.cpp \
	desaturate.h \
	desaturate-widget.cpp \
	desaturate-widget.h \
	edit-action.cpp \
	edit-action.h \
	edit-action-history.cpp \
	edit-action-history.h \
	edit-action-widget.cpp \
	edit-action-widget.h \
	editable-photo.cpp \
	editable-photo.h \
	editor-toolbar.cpp \
	editor-toolbar.h \
	editor.cpp \
	editor.h \
	flip.cpp \
	flip.h \
	flip-widget.cpp \
	flip-widget.h \
	gegl-operation.cpp \
	gegl-operation.h \
	rotate-widget.cpp \
	rotate-widget.h \
	rotate.cpp \
	rotate.h \
	save-photos-window.cpp \
	save-photos-window.h \
	scale.cpp \
	scale.h \
	scale-widget.cpp \
	scale-widget.h \
	stock-button.cpp \
	stock-button.h

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""${datadir}/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_builddir) \
	-I$(top_srcdir)/src/application \
	-I$(top_srcdir)/src/attribute \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/database \
	-I$(top_srcdir)/src/edit-engine \
	-I$(top_srcdir)/src/editor \
	-I$(top_srcdir)/src/importer \
	-I$(top_srcdir)/src/renderer \
	-I$(top_srcdir)/src/storage \
	$(SOLANG_CFLAGS) \
	$(EXIV2_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDAMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(GEGL_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
