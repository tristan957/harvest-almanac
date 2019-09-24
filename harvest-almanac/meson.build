harvest_almanac_config_h = configuration_data({
    'PACKAGE_NAME': '"@0@"'.format('Harvest Almanac'),
    'PACKAGE_VERSION': '"@0@"'.format(meson.project_version()),
    'GETTEXT_PACKAGE': '"@0@"'.format(meson.project_name()),
    'LOCALEDIR': '"@0@"'.format(join_paths(get_option('prefix'), get_option('localedir'))),
    'LIBDIR': '"@0@"'.format(join_paths(get_option('prefix'), get_option('libdir'))),
    'PACKAGE_LICENSE': 'GTK_LICENSE_GPL_3_0_ONLY',
    'PACKAGE_WEBSITE': '"@0@"'.format('https://git.sr.ht/~tristan957/harvest-almanac'),
    'PACKAGE_WEBSITE_LABEL': '"@0@"'.format('Repository'),
})

configure_file(
    output: 'config.h',
    configuration: harvest_almanac_config_h,
)

harvest_almanac_sources = [
    'main.c',
    'hal-application.c',
    'hal-preferences-window.c',
    'hal-profile.c',
    'hal-time-entry.c',
    'hal-time-tracker.c',
    'hal-window.c',
]

harvest_almanac_c_args = [
    '-DHANDY_USE_UNSTABLE_API',
]

if get_option('buildtype') == 'release'
    harvest_almanac_c_args += [
        '-DG_DISABLE_ASSERT',
        '-DG_DISABLE_CHECKS',
        '-DG_DISABLE_CAST_CHECKS',
    ]
elif get_option('buildtype').startswith('debug')
    harvest_almanac_c_args += [
        '-DG_ENABLE_DEBUG',
    ]
endif

harvest_almanac_dependencies = [
    libgtk_dep,
    libhandy_dep,
    libsecret_dep,
    libsoup_dep,
    harvest_glib_dep,
]

executable(
    'harvest-almanac',
    harvest_almanac_sources + harvest_almanac_resources,
    c_args: harvest_almanac_c_args,
    dependencies: harvest_almanac_dependencies,
    gui_app: true,
    install: true,
)
