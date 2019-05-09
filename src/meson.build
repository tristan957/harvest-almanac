config_h = configuration_data({
    'PACKAGE_NAME': '"@0@"'.format('Harvest Almanac'),
    'PACKAGE_DOMAIN': '1',
    'PACKAGE_VERSION': '"@0@"'.format(meson.project_version()),
    'GETTEXT_PACKAGE': '"@0@"'.format(meson.project_name()),
    'G_LOG_USE_STRUCTURED': true,
    'LOCALEDIR': '"@0@"'.format(join_paths(get_option('prefix'), get_option('localedir'))),
    'LIBDIR': '"@0@"'.format(join_paths(get_option('prefix'), get_option('libdir'))),
    'PACKAGE_LICENSE': 'GTK_LICENSE_GPL_3_0_ONLY',
    'PACKAGE_WEBSITE': '"@0@"'.format('https://git.sr.ht/~tristan957/tllt-cp'),
    'PACKAGE_WEBSITE_LABEL': '"@0@"'.format('Repository'),
})
if get_option('buildtype') == 'release'
    config_h.set('G_DISABLE_ASSERT', true)
    config_h.set('G_DISABLE_CHECKS', true)
    config_h.set('G_DISABLE_CAST_CHECKS', true)
elif get_option('buildtype').startswith('debug')
    config_h.set('G_ENABLE_DEBUG', true)
endif

configure_file(
    output: 'hal-config.h',
    configuration: config_h,
)

sources = [
    'main.c',
    'hal-application.c',
    'hal-window.c',
]

c_args = [
    '-D HANDY_USE_UNSTABLE_API',
]

executable(
    'harvest-almanac',
    sources + resources,
    c_args: c_args,
    dependencies: [
        libgtk_dep,
        libhandy_dep,
        libharvest_glib_dep,
	],
    gui_app: true,
    install: true,
)
