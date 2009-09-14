AC_INIT(Solang, 0.3, [solang-devel@nongnu.org], [solang])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC


IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=solang
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [GETTEXT package name])
AM_GLIB_GNU_GETTEXT


AM_PROG_LIBTOOL


PKG_CHECK_MODULES(BABL, [babl])
PKG_CHECK_MODULES(EXIV2, [exiv2 >= 0.18])
PKG_CHECK_MODULES(FLICKCURL, [flickcurl])
PKG_CHECK_MODULES(GDAMM, [libgdamm-3.0 <= 3.0.1])
PKG_CHECK_MODULES(GDA_SQLITE, [libgda-sqlite-3.0])
PKG_CHECK_MODULES(GDL, [gdl-1.0])
PKG_CHECK_MODULES(GEGL, [gegl])
PKG_CHECK_MODULES(GPHOTO2, [libgphoto2])
PKG_CHECK_MODULES(GTKIMAGEVIEW, [gtkimageview])
PKG_CHECK_MODULES(GTKMM, [gtkmm-2.4 >= 2.8])
PKG_CHECK_MODULES(SOUP, [libsoup-2.4])
PKG_CHECK_MODULES(WEBKIT, [webkit-1.0])


AC_OUTPUT([
Makefile
data/Makefile
src/Makefile
src/application/Makefile
src/attribute/Makefile
src/common/Makefile
src/database/Makefile
src/edit-engine/Makefile
src/editor/Makefile
src/exporter/Makefile
src/importer/Makefile
src/renderer/Makefile
src/storage/Makefile
po/Makefile.in
])
