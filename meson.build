# https://mesonbuild.com/Subprojects.html
# https://mesonbuild.com/Wrap-dependency-system-manual.html

project(
    'harvest-almanac',
    ['c'],
    version: '1.0.0',
    license: [
        'GPL-3.0-only',
    ],
    default_options: [
        'c_std=c11',
    ],
)

c_args = [
    '-Wall',
    '-Wextra',
    '--pedantic',
]

cc = meson.get_compiler('c')

add_project_arguments(c_args, language: 'c')

i18n = import('i18n')
gnome = import('gnome')

libgio_dep = dependency('gio-2.0', version: '>= 2.50', required: true)
libcurl_dep = dependency('libcurl', version: '>= 7.52', required: true)
libjson_glib_dep = dependency('json-glib-1.0', version: '>= 1.2', required: true)
libgtk_dep = dependency('gtk+-3.0', version: '>= 3.22', required: true)

subdir('data')
subdir('lib')
subdir('src')
subdir('po')

meson.add_install_script('meson_post_install.sh')
