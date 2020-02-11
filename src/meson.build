harvest_almanac_config_h = configuration_data({
    'package_name': 'Harvest Almanac',
    'package_version': meson.project_version(),
    'package_license': 'GTK_LICENSE_GPL_3_0',
    'package_website': 'https://git.sr.ht/~tristan957/harvest-almanac',
    'package_website_label': 'Repository',
    'gettext_package': 'meson.project_name()',
    'localedir': join_paths(get_option('prefix'), get_option('localedir')),
    'libdir': join_paths(get_option('prefix'), get_option('libdir')),
})

configure_file(
    input: 'config.h.in',
    output: 'config.h',
    configuration: harvest_almanac_config_h,
)

harvest_almanac_sources = [
    'main.c',
    'hal-application.c',
    'hal-context.c',
    'hal-preferences-window.c',
    'hal-profile.c',
    'hal-time-entry.c',
    'hal-time-tracker.c',
    'hal-window.c',
]

harvest_almanac_c_args = [
    '-DHANDY_USE_UNSTABLE_API',
    '-DG_LOG_DOMAIN="harvest-almanac"',
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
    include_directories: include_directories(join_paths('..', 'include')),
    gui_app: true,
    install: true,
)
