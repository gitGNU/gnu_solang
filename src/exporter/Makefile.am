noinst_LTLIBRARIES =	libexporter.la

libexporter_la_SOURCES = \
	photo-destination.h \
	photo-destination.cpp \
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
	$(CAIROMM_CFLAGS) \
	$(EXIV2_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDAMM_CFLAGS) \
	$(GIOMM_CFLAGS) \
	$(GLIBMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(SIGC_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
