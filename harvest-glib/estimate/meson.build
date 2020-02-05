component_public_headers = [
	'harvest-estimate.h',
	'harvest-estimate-item-category.h',
	'harvest-estimate-line-item.h',
]

component_header_subdir = join_paths(harvest_glib_header_subdir, 'estimate')

install_headers(component_public_headers, subdir: component_header_subdir)

estimate_enums = gnome.mkenums_simple(
    'harvest-estimate-enum-types',
    sources: [
        'harvest-estimate.h',
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
