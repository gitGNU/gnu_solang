AC_INIT(Solang, 0.3, [solang-devel@nongnu.org], [solang])

AM_INIT_AUTOMAKE(AC_PACKAGE_TARNAME, AC_PACKAGE_VERSION)
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


AC_ARG_VAR(
    [DBUS_BINDING_TOOL],
    [Tool to generate C bindings from XML D-Bus interface definitions])
AC_PATH_PROG([DBUS_BINDING_TOOL], [dbus-binding-tool], [no])
if test x"$DBUS_BINDING_TOOL" = x"no"; then
    AC_MSG_ERROR([could not find dbus-binding-tool in \$PATH.])
fi

PKG_CHECK_MODULES(BABL, [babl])
PKG_CHECK_MODULES(DBUS, [dbus-glib-1])
PKG_CHECK_MODULES(FLICKCURL, [flickcurl libxml-2.0])
PKG_CHECK_MODULES(GDL, [gdl-1.0])
PKG_CHECK_MODULES(GEGL, [gegl])
PKG_CHECK_MODULES(GPHOTO2, [libgphoto2])
PKG_CHECK_MODULES(GTKIMAGEVIEW, [gtkimageview])
PKG_CHECK_MODULES(GTKMM, [gtkmm-2.4 >= 2.8])
PKG_CHECK_MODULES(SOUP, [libsoup-2.4])
PKG_CHECK_MODULES(TRACKER, [tracker-client-0.7])
PKG_CHECK_MODULES(WEBKIT, [webkit-1.0])


AC_OUTPUT([
Makefile
data/Makefile
data/solang.desktop.in
data/icons/Makefile
data/icons/16x16/Makefile
data/icons/22x22/Makefile
data/icons/24x24/Makefile
data/icons/32x32/Makefile
data/icons/48x48/Makefile
data/icons/96x96/Makefile
data/icons/128x128/Makefile
data/icons/scalable/Makefile
src/Makefile
src/application/Makefile
src/attribute/Makefile
src/common/Makefile
src/edit-engine/Makefile
src/editor/Makefile
src/exporter/Makefile
src/renderer/Makefile
po/Makefile.in
])
