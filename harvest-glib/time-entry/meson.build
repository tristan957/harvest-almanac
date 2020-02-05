component_public_headers = [
	'harvest-time-entry.h',
]

request_public_headers = [
	'requests/harvest-late-request.h',
]

component_header_subdir = join_paths(harvest_glib_header_subdir, 'time-entry')
requests_header_subdir = join_paths(component_header_subdir, 'requests')

install_headers(component_public_headers, subdir: component_header_subdir)
install_headers(request_public_headers, subdir: requests_header_subdir)
