## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). "\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(FONTCONFIG_CFLAGS) \
	$(GLU_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GLIBMM_CFLAGS) \
	$(GLM_CFLAGS) \
	$(EPOXY_CFLAGS)

libgenericglm_la_CFLAGS = \
	-std=c17 -Wall -pedantic

lib_LTLIBRARIES = libgenericglm.la

libgenericglm_la_CXXFLAGS = \
	-Wall -pedantic

libgenericglm_la_LDFLAGS = \
	@EXTRA_LDFLAGS@ \
	-version-info @version_info@

libgenericglm_la_SOURCES = \
	BaseDialog.cpp \
	FbShaderContext.cpp \
	Font.cpp \
	Geometry.cpp \
	GeometryContext.cpp \
	glarea-error.c \
	Glyph.cpp \
	Polygon.cpp \
	PositionDbl.cpp \
	MarkContext.cpp \
	Matrix.cpp \
	NaviContext.cpp \
	NaviGlArea.cpp \
	Scene.cpp \
	ShaderContext.cpp \
	SunDiscGeometry.cpp \
	SunDiscRenderer.cpp \
	Text.cpp \
	TextContext.cpp \
	TreeGeometry.cpp \
	TreeNode.cpp \
	TreeRenderer.cpp \
	FallShapeGeometry.cpp \
	FallShapeRenderer.cpp \
	LineShapeGeometry.cpp \
	LineShapeRenderer.cpp \
	TextTreeGeometry.cpp \
	Buffer.cpp \
	Diagram.cpp \
	StringUtils.cpp

libgenericglm_la_LIBADD =  \
	$(FONTCONFIG_LIBS) \
	$(GLU_LIBS) \
	$(GTKMM_LIBS) \
	$(GLIBMM_LIBS) \
	$(EPOXY_LIBS)


## using pkginclude_HEADERS will use include dir with packagename
##   (include_HEADERS  will use #install#/include)
pkginclude_HEADERS = \
	BaseDialog.hpp \
	GenericGlmCompat.hpp \
	FbShaderContext.hpp \
	Font.hpp \
	Geometry.hpp \
	GeometryContext.hpp \
	glarea-error.h \
	Glyph.hpp \
	Polygon.hpp \
	PositionDbl.hpp \
	MarkContext.hpp \
	Matrix.hpp \
	NaviContext.hpp \
	NaviGlArea.hpp \
	Scene.hpp \
	ShaderContext.hpp \
	SunDiscGeometry.hpp \
	SunDiscRenderer.hpp \
	Text.hpp \
	TextContext.hpp \
	TreeGeometry.hpp \
	TreeNode.hpp \
	TreeRenderer.hpp \
	FallShapeGeometry.hpp \
	FallShapeRenderer.hpp \
	LineShapeGeometry.hpp \
	LineShapeRenderer.hpp \
	TextTreeGeometry.hpp \
	Buffer.hpp \
	Diagram.hpp \
	StringUtils.hpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = genericglm-$(VERSION).pc

EXTRA_DIST = \
	genericglm-$(VERSION).pc.in
