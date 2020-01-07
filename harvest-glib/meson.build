harvest_glib_version = '1.0.0'

harvest_glib_config_h = configuration_data({
    'gettext_package': 'harvest-glib',
})

configure_file(
    input: 'config.h.in',
    output: 'config.h',
    configuration: harvest_glib_config_h,
)

harvest_glib_sources = [
    'harvest-api-client.c',
    'harvest-client.c',
    'harvest-common.c',
    'harvest-company.c',
    'harvest-creator.c',
    'harvest-estimate.c',
    'harvest-estimate-item-category.c',
    'harvest-estimate-line-item.c',
    'harvest-get-company-request.c',
    'harvest-invoice.c',
    'harvest-invoice-item-category.c',
    'harvest-invoice-line-item.c',
    'harvest-late-request.c',
    'harvest-links.c',
    'harvest-pageable.c',
    'harvest-paged.c',
    'harvest-project.c',
    'harvest-request.c',
    'harvest-response.c',
    'harvest-response-metadata.c',
    'harvest-task.c',
    'harvest-task-assignment.c',
    'harvest-time-entry.c',
    'harvest-user.c',
    'harvest-user-assignment.c',
    'harvest-users-me-request.c'
]

enums = gnome.mkenums_simple(
    'harvest-enum-types',
    sources: [
        'harvest-company.h',
        'harvest-estimate.h',
        'harvest-http.h',
        'harvest-invoice.h',
    ],
    install_header: true,
    header_prefix:
'''
#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif
''',
)

harvest_glib_public_headers = [
    'harvest.h',
    'harvest-api-client.h',
    'harvest-client.h',
    'harvest-common.h',
    'harvest-company.h',
    'harvest-creator.h',
    'harvest-estimate.h',
    'harvest-estimate-item-category.h',
    'harvest-estimate-line-item.h',
    'harvest-get-company-request.h',
    'harvest-http.h',
    'harvest-invoice.h',
    'harvest-invoice-item-category.h',
    'harvest-invoice-line-item.h',
    'harvest-late-request.h',
    'harvest-links.h',
    'harvest-pageable.h',
    'harvest-paged.h',
    'harvest-project.h',
    'harvest-request.h',
    'harvest-response.h',
    'harvest-response-metadata.h',
    'harvest-task.h',
    'harvest-task-assignment.h',
    'harvest-time-entry.h',
    'harvest-user.h',
    'harvest-user-assignment.h',
    'harvest-users-me-request.h',
]

install_headers(harvest_glib_public_headers, subdir: 'harvest-glib')

harvest_glib_c_args = [
    '-D__HARVEST_COMPILATION__',
]

if get_option('buildtype') == 'release'
    harvest_glib_c_args += [
        '-DG_DISABLE_ASSERT',
        '-DG_DISABLE_CHECKS',
        '-DG_DISABLE_CAST_CHECKS',
    ]
elif get_option('buildtype').startswith('debug')
    harvest_glib_c_args += [
        '-DG_ENABLE_DEBUG',
    ]
endif

harvest_glib_dependencies = [
    libgio_dep,
    json_glib_dep,
    libsoup_dep,
]

harvest_glib = library(
    'harvest-glib',
    harvest_glib_sources,
    enums,
    version: harvest_glib_version,
    c_args: harvest_glib_c_args,
    dependencies: harvest_glib_dependencies,
    install: true,
)

harvest_glib_dep = declare_dependency(
    include_directories: include_directories('..'),
    link_with: harvest_glib,
    version: harvest_glib_version,
)

if get_option('tests')
    subdir('tests')
endif

pkg.generate(
	harvest_glib,
	description: 'GLib-based library for interacting with the Harvest API.',
	filebase: 'harvest-glib-1.0',
	name: 'harvest-glib',
	version: harvest_glib_version,
    requires_private: [
        libgio_dep,
        json_glib_dep,
    ],
	url: 'https://git.sr.ht/~tristan957/harvest-almanac',
)
