AC_INIT(Solang, 0.1, [solang-devel@nongnu.org], [solang])

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

SOLANG_LOCALE_DIR=[${datadir}/locale]
AC_SUBST(SOLANG_LOCALEDIR)


AM_PROG_LIBTOOL


PKG_CHECK_MODULES(CAIROMM, [cairomm-1.0])
PKG_CHECK_MODULES(EXIV2, [exiv2 <= 0.17.1])
PKG_CHECK_MODULES(GDAMM, [libgdamm-3.0 <= 3.0.1])
PKG_CHECK_MODULES(GDA_SQLITE, [libgda-sqlite-3.0])
PKG_CHECK_MODULES(GDL, [gdl-1.0])
PKG_CHECK_MODULES(GIOMM, [giomm-2.4])
PKG_CHECK_MODULES(GLIBMM, [glibmm-2.4])
PKG_CHECK_MODULES(GPHOTO2, [libgphoto2])
PKG_CHECK_MODULES(GTKIMAGEVIEW, [gtkimageview])
PKG_CHECK_MODULES(GTKMM, [gtkmm-2.4 >= 2.8])

PKG_CHECK_MODULES(IMAGEMAGICK, [GraphicsMagick++],,
    [PKG_CHECK_MODULES(IMAGEMAGICK, [ImageMagick++])])

PKG_CHECK_MODULES(SIGC, [sigc++-2.0])


AC_OUTPUT([
Makefile
data/Makefile
src/Makefile
src/application/Makefile
src/attribute/Makefile
src/common/Makefile
src/database/Makefile
src/exporter/Makefile
src/importer/Makefile
src/renderer/Makefile
src/storage/Makefile
po/Makefile.in
])
