i18n.merge_file(
    input: 'ht.sr.git.tristan957.harvest-almanac.desktop.in',
    output: 'ht.sr.git.tristan957.harvest-almanac.desktop',
    type: 'desktop',
    po_dir: '../po',
    install: true,
    install_dir: join_paths(get_option('datadir'), 'applications'),
)

i18n.merge_file(
    input: 'ht.sr.git.tristan957.harvest-almanac.appdata.xml.in',
    output: 'ht.sr.git.tristan957.harvest-almanac.appdata.xml',
    po_dir: '../po',
    install: true,
    install_dir: join_paths(get_option('datadir'), 'appdata'),
)

resources = gnome.compile_resources(
    'hal-resources',
    'harvest-almanac-resources.gresource.xml',
    source_dir: '../data',
    c_name: 'hal',
)

install_data('ht.sr.git.tristan957.harvest-almanac.gschema.xml',
    install_dir: join_paths(get_option('datadir'), 'glib-2.0/schemas')
)
