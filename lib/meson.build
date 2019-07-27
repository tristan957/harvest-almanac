sources = [
    'harvest-api-client.c',
    'client/objects/harvest-client.c',
    'estimate/objects/harvest-estimate.c',
    'estimate/objects/harvest-estimate-item-category.c',
    'estimate/objects/harvest-estimate-line-item.c',
    'invoice/objects/harvest-invoice.c',
    'invoice/objects/harvest-invoice-item-category.c',
    'invoice/objects/harvest-invoice-line-item.c',
    'project/objects/harvest-project.c',
    'shared/harvest-common.c',
    'shared/harvest-pageable.c',
    'shared/objects/harvest-creator.c',
    'shared/requests/harvest-request.c',
    'shared/responses/harvest-links.c',
    'shared/responses/harvest-paged.c',
    'shared/responses/harvest-response.c',
    'task/objects/harvest-task.c',
    'task/objects/harvest-task-assignment.c',
    'timesheet/objects/harvest-time-entry.c',
    'timesheet/requests/harvest-late-request.c',
    'user/objects/harvest-user.c',
    'user/objects/harvest-user-assignment.c',
]

libharvest_glib_public_headers = [
    'harvest-api-client.h',
    'harvest-error.h',
    'client/objects/harvest-client.h',
    'estimate/objects/harvest-estimate.h',
    'estimate/objects/harvest-estimate-item-category.h',
    'estimate/objects/harvest-estimate-line-item.h',
    'invoice/objects/harvest-invoice.h',
    'invoice/objects/harvest-invoice-item-category.h',
    'invoice/objects/harvest-invoice-line-item.h',
    'project/objects/harvest-project.h',
    'shared/harvest-http.h',
    'shared/harvest-pageable.c',
    'shared/objects/harvest-creator.h',
    'shared/requests/harvest-request.h',
    'shared/responses/harvest-links.h',
    'shared/responses/harvest-paged.h',
    'shared/responses/harvest-response.h',
    'task/objects/harvest-task.h',
    'task/objects/harvest-task-assignment.h',
    'timesheet/objects/harvest-time-entry.h',
    'timesheet/requests/harvest-late-request.h',
    'user/objects/harvest-user.h',
    'user/objects/harvest-user-assignment.h',
]

install_headers(libharvest_glib_public_headers, subdir: 'harvest-glib')

pkgincludedir = join_paths(get_option('includedir'), 'harvest-glib')

libharvest_glib_version = '1.0.0'

libharvest_glib = library(
    'harvest-glib',
    sources,
    version: libharvest_glib_version,
    dependencies: [
        libgio_dep,
        libcurl_dep,
        json_glib_dep,
    ],
    install: true,
)

libharvest_glib_dep = declare_dependency(
    include_directories: include_directories('.'),
    link_with: libharvest_glib,
    version: libharvest_glib_version
)

pkg = import('pkgconfig')
pkg.generate(
	libharvest_glib,
	description: 'GLib-based library for interacting with the Harvest API',
	filebase: 'libharvest-glib-1.0.0',
	name: 'libharvest-glib',
	version: libharvest_glib_version,
	url: 'https://git.sr.ht/~tristan957/harvest-almanac',
)
