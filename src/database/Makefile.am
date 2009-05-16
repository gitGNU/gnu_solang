noinst_LTLIBRARIES =	libdatabase.la

libdatabase_la_SOURCES = \
	database.cpp \
	database.h \
	db-object.cpp \
	db-object.h \
	db-table.cpp \
	db-table-factory.cpp \
	db-table-factory.h \
	db-table-visitor.cpp \
        db-table-visitor.h \
	db-table.h \
	exifs-table.cpp \
	exifs-table.h \
	photos-table.cpp \
	photos-table.h \
	photo-tags-table.cpp \
	photo-tags-table.h \
	tags-table.cpp \
	tags-table.h \
	thumbnails-table.cpp \
	thumbnails-table.h

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
