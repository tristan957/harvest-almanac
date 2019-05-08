config_h = configuration_data()
if get_option('buildtype') == 'release'
    config_h.set('G_DISABLE_ASSERT', true)
    config_h.set('G_DISABLE_CHECKS', true)
    config_h.set('G_DISABLE_CAST_CHECKS', true)
elif get_option('buildtype').startswith('debug')
    config_h.set('G_ENABLE_DEBUG', true)
endif

harvest_h = configure_file(
    input: 'harvest.h.in',
    output: 'harvest.h',
    configuration: config_h,
)

sources = [
]

libharvest_public_headers = [
    harvest_h
]

install_headers(libharvest_public_headers, subdir: 'harvest')

pkgincludedir = join_paths(get_option('includedir'), 'harvest')

libharvest_version = '1.0.0'

c_args = [
    '-DHARVEST_COMPILATION',
]

libharvest = library(
    'harvest',
    sources,
    c_args: c_args,
    version: libharvest_version,
    dependencies: [
        libgio_dep,
        libcurl_dep,
        libjson_glib_dep,
    ],
    install: true,
)

libharvest_dep = declare_dependency(
    include_directories: include_directories('.'),
    link_with: libharvest,
    version: libharvest_version
)
