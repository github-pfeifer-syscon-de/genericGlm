dnl Process this file with autoconf to produce a configure script.

AC_CONFIG_MACRO_DIR([m4])

AC_INIT([genericglm],[0.22])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AX_CHECK_ENABLE_DEBUG([info], [DEBUG], [RELEASE], [IS-RELEASE])
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11(noext, mandatory)

AC_DISABLE_STATIC

LT_INIT

AC_CANONICAL_HOST
AC_MSG_NOTICE([host_os $host_os])

PKG_CHECK_MODULES(FONTCONFIG, [fontconfig])

AC_ARG_WITH([gles], AS_HELP_STRING([--with-gles], [use openGL ES 3 (Raspi) otherweise fullgl]))

if test "x$with_gles" = "xyes" ; then
    PKG_CHECK_MODULES(EPOXY,[glesv2], [AC_DEFINE([USE_GLES], [1], [Using openGL ES 3])])
else
    PKG_CHECK_MODULES(EPOXY,[epoxy])
fi

PKG_CHECK_MODULES(GTKMM,gtkmm-3.0)

PKG_CHECK_MODULES([GLIBMM], [glibmm-2.4 giomm-2.4])

PKG_CHECK_MODULES([GLM], [glm])

if test "$host_os" = "mingw32" ; then
    AC_SUBST(EXTRA_LDFLAGS, "-lglu32 -no-undefined")
else
    PKG_CHECK_MODULES(GLU, [glu])
fi

AC_CONFIG_FILES([
Makefile
src/genericglm-0.22.pc
src/Makefile
])
AC_OUTPUT
