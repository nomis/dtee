version_check = run_command(import('python').find_installation(), [files('version_check.py'), files('changelog.rst'), meson.project_version()], check: false)
if version_check.returncode() != 0
	error(version_check.stderr())
endif

sphinx_build = find_program('sphinx-build', required: get_option('strict_packaging'))
if sphinx_build.found()
	source_docs = [
		'architecture.rst',
		'changelog.rst',
		'conf.py',
		'dependencies.rst',
		'index.rst',
		'install.rst',
		'limitations.rst',
		'manual.rst',
		'packages.rst',
		'packages/debian.rst',
		'packages/fedora.rst',
		'packages/rhel.rst',
		'packages/ubuntu.rst',
	]

	dtee_man = custom_target('dtee.1',
		input: source_docs,
		output: 'dtee.1',
		command: [
			sphinx_build,
			'-n',
			'-W',
			'-b', 'man',
			'-D', 'version=' + meson.project_version(),
			'-D', 'release=' + meson.project_version(),
			'@CURRENT_SOURCE_DIR@',
			'@OUTDIR@',
		],
		install: true,
		install_dir: join_paths(get_option('mandir'), 'man1'))

	custom_target('cronty.1',
		input: dtee_man,
		output: 'cronty.1',
		command: [ln, '-s', '-f', '@PLAINNAME@', '@OUTPUT@'],
		install: true,
		install_dir: join_paths(get_option('mandir'), 'man1'))
endif
