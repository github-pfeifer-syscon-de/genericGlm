project('genericglm', 'c', 'cpp'
    , default_options: ['c_std=c11'
                       , 'cpp_std=c++20'
                       , 'buildtype=debugoptimized'
                       , 'warning_level=2']
    , license : 'GPL-3.0'
    , meson_version: '>= 1.9.0'
    , version: '0.3.2')
project_description = 'Common openGL functions for psc projects'

genericimg_deps = dependency('genericimg', version : '>= 0.3.1')
glibmm2_deps    = dependency('glibmm-2.4 giomm-2.4')
libpng_deps     = dependency('libpng')
fontconfig_deps = dependency('fontconfig freetype2')
thread_deps     = dependency('threads')

if get_option('gles')
    opengl_deps = dependency('glesv2')
else
    opengl_deps = dependency('epoxy')
endif

cc = meson.get_compiler('c')
if target_machine.system() == 'windows'
  #glu_deps = cc.find_library('glu32', dirs: 'c:\windows\system32', required: true)
    glu_deps = cc.find_library('glu32', required: true)
else
    glu_deps = dependency('glu')
endif

cc.has_header('glm/glm.hpp', required: true)

pkgdatadir = join_paths(get_option('prefix'), get_option('datadir'), meson.project_name())
add_project_arguments('-DPACKAGE_DATA_DIR="' + pkgdatadir + '"', language: 'cpp')
public_headers = include_directories('include')

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())     # Surround the version in quotes to make it a C string
conf.set('USE_GLES', get_option('gles'))
configure_file ( output : meson.project_name() + '_config.h'
               , configuration : conf)

subdir('include')
subdir('src')
pkg_deps = [
      genericimg_deps
    , fontconfig_deps
    , thread_deps
    , opengl_deps
    ]
deps = [
      pkg_deps
    , glu_deps
    ]
project_target = both_libraries(meson.project_name()
    , sources
    , install : true
    , dependencies: deps
    , gnu_symbol_visibility : 'default'         # visibility was hidden, but selected visible for compatibility
    , include_directories : public_headers
    , version : meson.project_version())
subdir('test')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
    name : meson.project_name()
  , filebase : meson.project_name()
  , description : project_description
  , subdirs : meson.project_name()
  , libraries : project_target
  , requires : pkg_deps
)

# Make this library usable as a Meson subproject.
project_deps = declare_dependency(
  include_directories: public_headers,
  link_with : project_target
)
set_variable(meson.project_name() + '_dep', project_deps )
