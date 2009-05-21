noinst_LTLIBRARIES =	libattribute.la

libattribute_la_SOURCES = \
	basic-exif-view-model-column-record.cpp \
	basic-exif-view-model-column-record.h \
	basic-exif-view.cpp \
	basic-exif-view.h \
	exif-data-key.cpp \
	exif-data-key.h \
	exif-data.cpp \
	exif-data.h \
	modification-date.cpp \
	modification-date.h \
	photo-tag.cpp \
	photo-tag.h \
	property-manager.cpp \
	property-manager.h \
	search-basket.cpp \
	search-basket.h \
	search-basket-column-record.cpp \
	search-basket-column-record.h \
	search-criterion-repo.cpp \
	search-criterion-repo.h \
	search-criterion-source.cpp \
	search-criterion-source.h \
	tag.cpp \
	tag.h \
	tag-manager.cpp \
	tag-manager.h \
	tag-new-dialog.cpp \
	tag-new-dialog.h \
	tag-view.h \
	tag-view.cpp \
	tag-view-model-column-record.h \
	tag-view-model-column-record.cpp \
	thumbnail.cpp \
	thumbnail.h	

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
	$(LIBXML_CFLAGS) \
	$(SIGC_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
