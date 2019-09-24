harvest_glib_sources = [
    'harvest-api-client.c',
    'harvest-client.c',
    'harvest-common.c',
    'harvest-creator.c',
    'harvest-estimate.c',
    'harvest-estimate-item-category.c',
    'harvest-estimate-line-item.c',
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
]

harvest_glib_public_headers = [
    'harvest.h',
    'harvest-api-client.c',
    'harvest-client.c',
    'harvest-common.c',
    'harvest-creator.c',
    'harvest-error.h',
    'harvest-estimate.c',
    'harvest-estimate-item-category.c',
    'harvest-estimate-line-item.c',
    'harvest-http.h',
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
]

install_headers(harvest_glib_public_headers, subdir: 'harvest-glib')

harvest_glib_version = '1.0.0'

harvest_glib = library(
    'harvest-glib',
    harvest_glib_sources,
    version: harvest_glib_version,
    c_args: [
        '-D__HARVEST_COMPILATION__',
    ],
    dependencies: [
        libgio_dep,
        json_glib_dep,
        libsoup_dep,
    ],
    install: true,
)

harvest_glib_dep = declare_dependency(
    include_directories: include_directories('.'),
    link_with: harvest_glib,
    version: harvest_glib_version
)

pkg.generate(
	harvest_glib,
	description: 'GLib-based library for interacting with the Harvest API',
	filebase: 'harvest-glib-1.0.0',
	name: 'harvest-glib',
	version: harvest_glib_version,
    requires: [
        libsoup_dep,
    ],
    requires_private: [
        libgio_dep,
        json_glib_dep,
        libsecret_dep,
    ],
	url: 'https://git.sr.ht/~tristan957/harvest-almanac',
)
