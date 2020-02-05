component_public_headers = [
	'harvest-company.h',
]

request_public_headers = [
	'requests/harvest-get-company-request.h',
]

component_header_subdir = join_paths(harvest_glib_header_subdir, 'company')
requests_header_subdir = join_paths(component_header_subdir, 'requests')

company_enums = gnome.mkenums_simple(
    'harvest-company-enum-types',
    sources: [
        'harvest-company.h',
    ],
    install_header: true,
    install_dir: join_paths(get_option('prefix'), get_option('includedir'), component_header_subdir),
    header_prefix:
'''
#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif
''',
)

install_headers(component_public_headers, subdir: component_header_subdir)
install_headers(request_public_headers, subdir: requests_header_subdir)
