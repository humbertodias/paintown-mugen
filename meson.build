# Warning: changing the default_options requires a new build directory and doing 'meson setup ...' again
project('paintown', ['c', 'cpp'], default_options: ['cpp_std=c++11'], version: '4.0')

build_tests = get_option('build_tests')
release = get_option('release')

add_global_arguments('-DUSE_SDL2=1', language: 'cpp')
add_global_arguments('-g3', language: ['cpp', 'c'])
add_global_arguments(['-Wextra', '-Wno-unused-variable', '-Wno-unused-function', '-Wno-unused-parameter'], language: ['cpp', 'c'])

link_args = []
is_static = true
sdl_image_dependency = []
sdl_image_library = []
sdl_graphics_dependency = []
sdl_graphics_library = []
yaml_cpp_dependency = []
yaml_cpp_library = []
other_dependencies = []

# Not supported in mingw
if not meson.is_cross_build()
  if release != true
    add_global_arguments(['-fsanitize=address'], language: ['cpp', 'c'])
    link_args = ['-fsanitize=address']
  else
    link_args = ['-pthread']
  endif
  # Avoid redefinition
  add_global_arguments('-DDATA_PATH="data"', language: 'cpp')
  
  is_static = false
  if not meson.has_external_property('steam_build')
    sdl_graphics_dependency = dependency('SDL2_gfx', required: true, static: is_static)
  endif
  sdl_image_dependency = dependency('SDL2_image', required: true, static: is_static)
  yaml_cpp_dependency = dependency('yaml-cpp', required: false, static: is_static)

  if yaml_cpp_dependency.found()
    add_global_arguments('-DHAVE_YAML_CPP', language: 'cpp')
  endif
else
  add_global_arguments('-DCROSS_BUILD', language: ['c', 'cpp'])
endif

includes = include_directories('src')
all_code = [
  'src/xmain.cpp',
 'src/main-menu.cpp',
 'src/factory/collector.cpp',
 'src/factory/font_render.cpp',
 'src/globals.cpp',
]

sdl_dependency = dependency('sdl2', required: true, static: is_static)
sdl_ttf_dependency = dependency('SDL2_ttf', required: true, version: '>=2.0.18', static: is_static)
sdl_mixer_dependency = dependency('SDL2_mixer', required: true, static: is_static)
#sdl_graphics_dependency = dependency('SDL2_gfx', required: true, static: is_static)
# freetype_dependency = dependency('freetype2', required: true)
# zlib static = true doesn't work here, have to put it in the built-in options
zlib_dependency = dependency('zlib', required: true)
vorbis_dependency = dependency('vorbis', required: false, static: is_static)
vorbis_file_dependency = dependency('vorbisfile', required: false, static: is_static)
ogg_dependency = dependency('ogg', required: false, static: is_static)
mpg123_dependency = dependency('libmpg123', required: false, static: is_static)

if vorbis_dependency.found() and vorbis_file_dependency.found() and ogg_dependency.found()
  add_global_arguments('-DHAVE_OGG=1', language: 'cpp')
endif

if mpg123_dependency.found()
  add_global_arguments('-DHAVE_MP3_MPG123=1', language: 'cpp')
endif

build_paintown_test = false
if meson.is_cross_build()
  build_sdl2_image = false
  if meson.has_external_property('dkp_build') and meson.get_external_property('dkp_build')
    # Need to add harfbuzz for DKP
    other_dependencies += [dependency('harfbuzz', required: true, static: is_static)]
    # Use included SDL_gfx
    sdl_graphics_dependency = dependency('SDL2_gfx', required: true, static: is_static)
    build_paintown_test = true
  endif

  if meson.has_external_property('dkp_build_type') and meson.get_external_property('dkp_build_type') == 'gamecube'
    build_sdl2_image = true
  else
    sdl_image_dependency = dependency('SDL2_image', required: true, static: is_static)
  endif

  fs = import('fs')
  if fs.is_dir('.tmp/yaml-cpp')
    add_global_arguments('-DHAVE_YAML_CPP', language: 'cpp')
    subdir('.tmp/yaml-cpp')
  endif
  if fs.is_dir('.tmp/SDL_gfx')
    subdir('.tmp/SDL_gfx')
  endif
  if fs.is_dir('.tmp/SDL_image')
    if build_sdl2_image
      subdir('.tmp/SDL_image')
    endif
  endif
endif

subdir('src/r-tech1')
subdir('src/mugen')
subdir('src/paintown-engine')
# subdir('src/openbor')
if meson.has_external_property('steam_build')
  subdir('.tmp/SDL_gfx')
endif

test_dependencies = other_dependencies + [yaml_cpp_dependency, sdl_dependency, sdl_image_dependency, sdl_ttf_dependency, sdl_mixer_dependency, sdl_graphics_dependency, zlib_dependency, vorbis_dependency, vorbis_file_dependency, ogg_dependency, mpg123_dependency]
#test_linkages = [yaml_cpp_library, sdl_image_library, sdl_graphics_library, rtech1_library, mugen_library, paintown_library, openbor_library]
test_linkages = [yaml_cpp_library, sdl_image_library, sdl_graphics_library, rtech1_library, paintown_library, mugen_library]
test_link_args = link_args

executable('paintown', all_code,
  include_directories: [includes, dumb_includes],
  dependencies: other_dependencies + [yaml_cpp_dependency, sdl_dependency, sdl_image_dependency, sdl_ttf_dependency, sdl_mixer_dependency, sdl_graphics_dependency, zlib_dependency, vorbis_dependency, vorbis_file_dependency, ogg_dependency, mpg123_dependency],
#  link_with: [yaml_cpp_library, sdl_image_library, sdl_graphics_library, rtech1_library, mugen_library, paintown_library, openbor_library],
  link_with: [yaml_cpp_library, sdl_image_library, sdl_graphics_library, rtech1_library, mugen_library, paintown_library],
  link_args: link_args,
  )

if build_tests == true
  # subdir('src/test/paintown')
  # subdir('src/test/graphics')
  # subdir('src/test/audio')
  subdir('src/test/mugen')
  # subdir('src/test/timing')
  # subdir('src/test/util')
  # subdir('src/test/thread')
  # subdir('src/test/token')
# elif build_paintown_test == true
#   subdir('src/test/devkitpro')
endif
