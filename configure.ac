dnl Process this file with autoconf to produce a configure script.

AC_CONFIG_MACRO_DIR([m4])

m4_define([v_maj], [0])
m4_define([v_min], [3])
m4_define([v_mic], [2])
m4_define([project_version], [v_maj.v_min.v_mic])
m4_define([lt_cur], m4_eval(v_maj + v_min))
m4_define([lt_rev], [v_mic])
m4_define([lt_age], [v_min])

AC_INIT([genericglm],[project_version])

version_info="lt_cur:lt_rev:lt_age"
AC_SUBST([version_info])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AX_CHECK_ENABLE_DEBUG([info], [DEBUG], [RELEASE], [IS-RELEASE])
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
dnl we depend on c++20 in some places,
dnl with an older compiler, c++17 might work as well with some adaptions.
AX_CXX_COMPILE_STDCXX([20], [noext], [optional])

AC_DISABLE_STATIC

LT_INIT

AC_CANONICAL_HOST
AC_MSG_NOTICE([host_os $host_os])

depend_fontconfig="fontconfig"
AC_SUBST([depend_fontconfig])
PKG_CHECK_MODULES(FONTCONFIG, [$depend_fontconfig])

AC_ARG_WITH([gles], AS_HELP_STRING([--with-gles], [use openGL ES 3 (Raspi) otherweise fullgl]))

if test "x$with_gles" = "xyes" ; then
    PKG_CHECK_MODULES(EPOXY,[glesv2], [AC_DEFINE([USE_GLES], [1], [Using openGL ES 3])])
    depend_epoxy="glesv2"
else
    PKG_CHECK_MODULES(EPOXY,[epoxy])
    depend_epoxy="epoxy"
fi
AC_SUBST([depend_epoxy])

depend_gtkmm="gtkmm-3.0"
AC_SUBST([depend_gtkmm])
PKG_CHECK_MODULES(GTKMM,$depend_gtkmm)

depend_glibmm="glibmm-2.4 giomm-2.4"
AC_SUBST([depend_glibmm])
PKG_CHECK_MODULES([GLIBMM], [$depend_glibmm])

depend_glm="glm"
AC_SUBST([depend_glm])
PKG_CHECK_MODULES([GLM], [$depend_glm])

depend_genericimg="genericimg >= 0.3.1"
AC_SUBST([depend_genericimg])
PKG_CHECK_MODULES([GENERICIMG], [$depend_genericimg])

if test "$host_os" = "mingw32" ; then
    AC_SUBST(EXTRA_LDFLAGS, "-lglu32 -no-undefined")
    depend_glu=""
else
    PKG_CHECK_MODULES(GLU, [glu])
    depend_glu="glu"
fi
AC_SUBST([depend_glu])

AC_CONFIG_FILES([
Makefile
src/Makefile
test/Makefile
src/genericglm.pc
])
AC_OUTPUT
