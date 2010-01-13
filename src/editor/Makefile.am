noinst_LTLIBRARIES =	libeditor.la

libeditor_la_SOURCES = \
	buffer-maker.cpp \
	buffer-maker.h \
	buffer-pixbuf-converter.cpp \
	buffer-pixbuf-converter.h \
	editable-photo.cpp \
	editable-photo.h \
	editor.cpp \
	editor.h \
	flip-horiz-operation.cpp \
	flip-horiz-operation.h \
	flip-operation.cpp \
	flip-operation.h \
	flip-vert-operation.cpp \
	flip-vert-operation.h \
	rotate-clock-operation.cpp \
	rotate-clock-operation.h \
	rotate-counter-operation.cpp \
	rotate-counter-operation.h \
	rotate-operation.cpp \
	rotate-operation.h

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""${datadir}/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_builddir) \
	-I$(top_srcdir)/src/application \
	-I$(top_srcdir)/src/attribute \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/editor \
	-I$(top_srcdir)/src/exporter \
	-I$(top_srcdir)/src/renderer \
	$(SOLANG_CFLAGS) \
	$(DBUS_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(GEGLMM_CFLAGS) \
	$(TRACKER_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
