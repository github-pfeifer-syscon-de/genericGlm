
# make this config persistent, to share infos at runtime -> makes in header reference possible
configure_file ( output : meson.project_name() + '_config.h'
               , configuration : conf
               , install : true
               , install_dir : 'include/' + meson.project_name())
project_headers = [
	  'BaseDialog.hpp'
	, 'GenericGlmCompat.hpp'
	, 'FbShaderContext.hpp'
	, 'Displayable.hpp'
	, 'GeometryContext.hpp'
        , 'glarea-error.h'
	, 'PositionDbl.hpp'
	, 'MarkContext.hpp'
	, 'Matrix.hpp'
	, 'NaviContext.hpp'
	, 'NaviGlArea.hpp'
	, 'Scene.hpp'
	, 'ShaderContext.hpp'
	, 'TextContext.hpp'
	, 'Buffer.hpp'
	, 'active_ptr.hpp'
	, 'Geom2.hpp'
	, 'Diagram2.hpp'
	, 'Font2.hpp'
	, 'Glyph2.hpp'
	, 'Polygon2.hpp'
	, 'Text2.hpp'
	, 'TreeRenderer2.hpp'
	, 'LineShapeRenderer2.hpp'
	, 'LineShapeGeometry2.hpp'
	, 'SunDiscRenderer2.hpp'
	, 'SunDiscGeometry2.hpp'
	, 'FallShapeRenderer2.hpp'
	, 'FallShapeGeometry2.hpp'
	, 'TreeGeometry2.hpp'
	, 'TextTreeGeometry2.hpp'
	, 'TreeNode2.hpp'
	, 'NamedTreeNode2.hpp' ]


# Make this library usable from the system's
# package manager.
install_headers(project_headers, subdir : meson.project_name())
