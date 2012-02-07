AC_PREREQ(2.57)
AC_INIT(jushelf, 0.1)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE
#AC_PROG_INTLTOOL
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

AC_HEADER_STDC
# Checks for libraries.
pkg_modules="clutter-1.0 \
             xrandr \
             json-glib-1.0"

PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

JUSHELF_MODULE="-avoid-version \
-rpath \$(libdir)/jushelf \
-no-undefined "'-export-symbols-regex "^[[^_]].*"'
AC_SUBST(JUSHELF_MODULE)

AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug], [Enable debugging]),
              CFLAGS="$CFLAGS -g -D_DEBUG")

GETTEXT_PACKAGE=$PACKAGE
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])

ALL_LINGUAS=""
AC_SUBST(ALL_LINGUAS)
AM_GLIB_GNU_GETTEXT

AC_OUTPUT([
  Makefile
  include/Makefile
  include/jushelf/Makefile
  src/Makefile
  modules/Makefile
  modules/launch/Makefile
  data/Makefile
])
