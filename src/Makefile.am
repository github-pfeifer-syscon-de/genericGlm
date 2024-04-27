## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). "\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(FONTCONFIG_CFLAGS) \
	$(GLU_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(GLIBMM_CFLAGS) \
	$(GENERICIMG_CFLAGS) \
	$(GLM_CFLAGS) \
	$(EPOXY_CFLAGS)

libgenericglm_la_CFLAGS = \
	-Wall \
	-Wextra \
	-Wconversion \
	-pedantic

lib_LTLIBRARIES = libgenericglm.la

# excluded unused as this gets annoying
# not using -Wconversion as i did not make use of casts ...at the time it was feasible
libgenericglm_la_CXXFLAGS = \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-pedantic

libgenericglm_la_LDFLAGS = \
	@EXTRA_LDFLAGS@ \
	-version-info @version_info@

libgenericglm_la_SOURCES = \
	BaseDialog.cpp \
	FbShaderContext.cpp \
	Displayable.cpp \
	GeometryContext.cpp \
	glarea-error.c \
	PositionDbl.cpp \
	MarkContext.cpp \
	Matrix.cpp \
	NaviContext.cpp \
	NaviGlArea.cpp \
	Scene.cpp \
	ShaderContext.cpp \
	TextContext.cpp \
	Buffer.cpp \
	active_ptr.cpp \
	Geom2.cpp \
	Diagram2.cpp \
	Font2.cpp \
	Glyph2.cpp \
	Polygon2.cpp \
	Text2.cpp \
	TreeRenderer2.cpp \
	LineShapeRenderer2.cpp \
	LineShapeGeometry2.cpp \
	SunDiscRenderer2.cpp \
	SunDiscGeometry2.cpp \
	FallShapeRenderer2.cpp \
	FallShapeGeometry2.cpp \
	TreeGeometry2.cpp \
	TextTreeGeometry2.cpp \
	TreeNode2.cpp \
	NamedTreeNode2.cpp

libgenericglm_la_LIBADD =  \
	$(FONTCONFIG_LIBS) \
	$(GLU_LIBS) \
	$(GTKMM_LIBS) \
	$(GLIBMM_LIBS) \
	$(GENERICIMG_LIBS) \
	$(EPOXY_LIBS)


## using pkginclude_HEADERS will use include dir with packagename
##   (include_HEADERS  will use #install#/include)
pkginclude_HEADERS = \
	BaseDialog.hpp \
	GenericGlmCompat.hpp \
	FbShaderContext.hpp \
	Displayable.hpp \
	GeometryContext.hpp \
	glarea-error.h \
	PositionDbl.hpp \
	MarkContext.hpp \
	Matrix.hpp \
	NaviContext.hpp \
	NaviGlArea.hpp \
	Scene.hpp \
	ShaderContext.hpp \
	TextContext.hpp \
	Buffer.hpp \
	active_ptr.hpp \
	Geom2.hpp \
	Diagram2.hpp \
	Font2.hpp \
	Glyph2.hpp \
	Polygon2.hpp \
	Text2.hpp \
	TreeRenderer2.hpp \
	LineShapeRenderer2.hpp \
	LineShapeGeometry2.hpp \
	SunDiscRenderer2.hpp \
	SunDiscGeometry2.hpp \
	FallShapeRenderer2.hpp \
	FallShapeGeometry2.hpp \
	TreeGeometry2.hpp \
	TextTreeGeometry2.hpp \
	TreeNode2.hpp \
	NamedTreeNode2.hpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = genericglm.pc

EXTRA_DIST = \
	genericglm.pc.in
