noinst_LTLIBRARIES =	libeditengine.la

libeditengine_la_SOURCES = \
	brightness-operation.cpp \
	brightness-operation.h \
	contrast-operation.cpp \
	contrast-operation.h \
	buffer.cpp \
	buffer.h \
	edit-types.h \
	edit-engine.h \
	edit-engine.cpp \
	edit-utils.h \
	filter.cpp \
	filter.h \
	load-file.cpp \
	load-file.h \
	operation.cpp \
	operation.h

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""${datadir}/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_builddir) \
	-I$(top_srcdir)/src/application \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/edit-engine \
	-I$(top_srcdir)/src/renderer \
	$(SOLANG_CFLAGS) \
	$(EXIV2_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GDAMM_CFLAGS) \
	$(GDL_CFLAGS) \
	$(GEGL_CFLAGS)

AM_CXXFLAGS = \
	 -Wall
