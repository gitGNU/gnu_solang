bin_PROGRAMS = solang

solang_SOURCES = \
	application.cpp \
	application.h \
	deletion-queue.cpp \
	deletion-queue.h \
	engine.cpp \
	engine.h \
	main.cpp \
	main-window.cpp \
	main-window.h

solang_LDFLAGS = 

solang_LDADD = \
	$(top_builddir)/src/importer/libimporter.la \
	$(top_builddir)/src/renderer/librenderer.la \
	$(top_builddir)/src/storage/libstorage.la \
	$(top_builddir)/src/attribute/libattribute.la \
	$(top_builddir)/src/database/libdatabase.la \
	$(top_builddir)/src/common/libcommon.la \
	$(EXIV2_LIBS) \
	$(GPHOTO2_LIBS) \
	$(GTKIMAGEVIEW_LIBS) \
	$(GTKMM_LIBS) \
	$(CAIROMM_LIBS) \
	$(GDAMM_LIBS) \
	$(GIOMM_LIBS) \
	$(GLIBMM_LIBS) \
	$(GDL_LIBS) \
	$(IMAGEMAGICK_LIBS) \
	$(SIGC_LIBS)

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
	$(GPHOTO2_CFLAGS) \
	$(GTKIMAGEVIEW_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDAMM_CFLAGS) \
	$(GIOMM_CFLAGS) \
	$(GLIBMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(IMAGEMAGICK_CFLAGS) \
	$(SIGC_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
