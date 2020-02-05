project(
    'harvest-almanac',
    'c',
    license: [
        'GPL-3.0-only',
        'LGPL-3.0-only',
    ],
    default_options: [
        'c_std=gnu18', # Enable GCC extensions
        'buildtype=debugoptimized',
    ],
)

cc = meson.get_compiler('c')

gnome = import('gnome')
i18n = import('i18n')
pkg = import('pkgconfig')

libgio_dep = dependency('gio-2.0', version: '>= 2.62.1', required: true, fallback: ['gio', 'libgio_dep'])
json_glib_dep = dependency('json-glib-1.0', version: '>= 1.4.4', required: true, fallback: ['json-glib', 'json_glib_dep'])
libgtk_dep = dependency('gtk+-3.0', version: '>= 3.24.12', required: true, fallback: ['gtk', 'libgtk_dep'])
libhandy_dep = dependency('libhandy-0.0', version: '>= 0.0.13', required: true, fallback: ['libhandy', 'libhandy_dep'])
libsoup_dep = dependency('libsoup-2.4', version: '>= 2.68.1', required: true, fallback: ['libsoup', 'libsoup_dep'])
libsecret_dep = dependency('libsecret-1', version: '>= 0.19.1', required: true, fallback: ['libsecret', 'libsecret_dep'])

if get_option('use-system-harvest-glib')
    harvest_glib_dep = dependency('harvest-glib-1.0', version: '>= 1.0.0', required: true)
else
    subdir('harvest-glib')
endif

if not get_option('lib-only')
    subdir('data')
    subdir('harvest-almanac')
endif

subdir('po')

meson.add_install_script('meson_post_install.sh')
