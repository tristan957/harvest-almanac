harvest_tests = {
	'user': {}
}

dependencies = [
	harvest_glib_dep,
	json_glib_dep,
	libgio_dep,
	libsoup_dep,
]

test_env = environment()
test_env.set('G_TEST_SRCDIR', meson.current_source_dir())
test_env.set('G_TEST_BUILDDIR', meson.current_build_dir())

c_args = [
	'-D__HARVEST_COMPILATION__'
]

foreach test_name, value : harvest_tests
	exe = executable(
		test_name,
		test_name + '.c',
		enums,
		dependencies: dependencies,
		include_directories: include_directories('..'),
		c_args: c_args,
	)

	test(test_name, exe, env: test_env)
endforeach
