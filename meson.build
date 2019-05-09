# https://mesonbuild.com/Subprojects.html
# https://mesonbuild.com/Wrap-dependency-system-manual.html

project(
    'harvest-almanac',
    'c',
    version: '1.0.0',
    license: [
        'GPL-3.0-only',
    ],
    default_options: [
        'c_std=gnu11', # Symptom of statically compiling with wraps
        'buildtype=debugoptimized',
        'warning_level=1',
        'libhandy:tests=false',
        'libhandy:vapi=false',
    ],
)

c_args = [
    '-Wall',
    '-Wextra',
    '--pedantic',
    # Required to pick up hal-resources.h in some IDEs
    '-I' + join_paths(meson.build_root(), 'data'),
]

cc = meson.get_compiler('c')

add_project_arguments(c_args, language: 'c')

i18n = import('i18n')
gnome = import('gnome')

libhandy_dep = dependency('libhandy-0.0', version: '>= 0.0.9', required: true, fallback: ['libhandy', 'libhandy_dep'])
libgio_dep = dependency('gio-2.0', version: '>= 2.50', required: true, fallback: ['glib', 'libgio_dep'])
libcurl_dep = dependency('libcurl', version: '>= 7.52', required: true)
json_glib_dep = dependency('json-glib-1.0', version: '>= 1.2', required: true, fallback: ['json-glib', 'json_glib_dep'])
libgtk_dep = dependency('gtk+-3.0', version: '>= 3.22', required: true, fallback: ['gtk', 'libgtk_dep'])

subdir('data')
subdir('lib')
subdir('src')
subdir('po')

meson.add_install_script('meson_post_install.sh')
