noinst_LTLIBRARIES =	libexporter.la

libexporter_la_SOURCES = \
	exporter.cpp \
	exporter.h \
	exporter-dialog.cpp \
	exporter-dialog.h \
	photo-destination.h \
	photo-destination.cpp \
	photo-destination-enums.h \
	brasero-destination.cpp \
	brasero-destination.h \
	directory-destination.h \
	directory-destination.cpp

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
	$(BRASERO_CFLAGS) \
	$(EXIV2_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDAMM_CFLAGS) \
	$(GDL_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
