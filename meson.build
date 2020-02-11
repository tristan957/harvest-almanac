project(
    'harvest-almanac',
    'c',
    version: '1.0.0',
    license: [
        'GPL-3.0',
    ],
    default_options: [
        'c_std=gnu18',
        'buildtype=debugoptimized',
    ],
)

cc = meson.get_compiler('c')

gnome = import('gnome')
i18n = import('i18n')

libgio_dep = dependency('gio-2.0', version: '>= 2.62.1', required: true, fallback: ['gio', 'libgio_dep'])
libgtk_dep = dependency('gtk+-3.0', version: '>= 3.24.12', required: true, fallback: ['gtk', 'libgtk_dep'])
libhandy_dep = dependency('libhandy-0.0', version: '>= 0.0.13', required: true, fallback: ['libhandy', 'libhandy_dep'])
libsoup_dep = dependency('libsoup-2.4', version: '>= 2.68.1', required: true, fallback: ['libsoup', 'libsoup_dep'])
libsecret_dep = dependency('libsecret-1', version: '>= 0.19.1', required: true, fallback: ['libsecret', 'libsecret_dep'])
harvest_glib_dep = dependency('harvest-glib-1.0', version: '>= 1.0.0', required: true, fallback: ['harvest-glib', 'harvest_glib_dep'])

subdir('data')
subdir('po')
subdir('src')

meson.add_install_script('meson_post_install.sh')
