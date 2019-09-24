project(
    'harvest-almanac',
    'c',
    version: '1.0.0',
    license: [
        'GPL-3.0-only',
    ],
    default_options: [
        'c_std=gnu11', # Enable GCC extensions
        'buildtype=debugoptimized',
        'warning_level=1',
    ],
)

c_args = [
    '-Wall',
    '-Wextra',
    # Required to pick up hal-resources.h in some IDEs
    '-I' + join_paths(meson.build_root(), 'data'),
]

cc = meson.get_compiler('c')

add_project_arguments(c_args, language: 'c')

gnome = import('gnome')
i18n = import('i18n')
pkg = import('pkgconfig')

libgio_dep = dependency('gio-2.0', version: '>= 2.60.4', required: true, fallback: ['gio', 'libgio_dep'])
json_glib_dep = dependency('json-glib-1.0', version: '>= 1.4.4', required: true, fallback: ['json-glib', 'json_glib_dep'])
libgtk_dep = dependency('gtk+-3.0', version: '>= 3.24.8', required: true, fallback: ['gtk', 'libgtk_dep'])
libhandy_dep = dependency('libhandy-0.0', version: '>= 0.0.11', required: true, fallback: ['libhandy', 'libhandy_dep'])
libsoup_dep = dependency('libsoup-2.4', version: '>= 2.66.1', required: true, fallback: ['libsoup', 'libsoup_dep'])
libsecret_dep = dependency('libsecret-1', version: '>= 0.18.8', required: true, fallback: ['libsecret', 'libsecret_dep'])

subdir('data')
subdir('harvest-glib')
subdir('harvest-almanac')
subdir('po')

meson.add_install_script('meson_post_install.sh')
