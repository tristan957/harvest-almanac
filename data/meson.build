i18n.merge_file(
    input: 'io.partin.tristan.HarvestAlmanac.desktop.in',
    output: 'io.partin.tristan.HarvestAlmanac.desktop',
    type: 'desktop',
    po_dir: '../po',
    install: true,
    install_dir: join_paths(get_option('datadir'), 'applications'),
)

i18n.merge_file(
    input: 'io.partin.tristan.HarvestAlmanac.appdata.xml.in',
    output: 'io.partin.tristan.HarvestAlmanac.appdata.xml',
    po_dir: '../po',
    install: true,
    install_dir: join_paths(get_option('datadir'), 'appdata'),
)

resources = gnome.compile_resources(
    'hal-resources',
    'hal-resources.gresource.xml',
    source_dir: '../data',
    c_name: 'hal',
)

install_data('io.partin.tristan.HarvestAlmanac.gschema.xml',
    install_dir: join_paths(get_option('datadir'), 'glib-2.0/schemas')
)
