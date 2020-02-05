component_public_headers = [
	'harvest-invoice-item-category.h',
	'harvest-invoice-line-item.h',
	'harvest-invoice.h',
]

component_header_subdir = join_paths(harvest_glib_header_subdir, 'invoice')

install_headers(component_public_headers, subdir: component_header_subdir)

invoice_enums = gnome.mkenums_simple(
    'harvest-invoice-enum-types',
    sources: [
        'harvest-invoice.h',
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
