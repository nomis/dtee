project('dtee',
	['c', 'cpp'],
	default_options: [
		'c_std=c11',
		'cpp_std=c++14',
		'warning_level=3',
		'werror=true',
	],
	version: '1.0.1',
	license: 'GPL3+',
	meson_version: '>=0.53.0',
)

source_files = [
	'src/application.cc',
	'src/command_line.cc',
	'src/dispatch.cc',
	'src/cron.cc',
	'src/file_output.cc',
	'src/input.cc',
	'src/main.cc',
	'src/print_error.cc',
	'src/process.cc',
	'src/signal_block.cc',
	'src/signal_handler.cc',
	'src/stream_output.cc',
	'src/temp_directory.cc',
	'src/temp_file.cc',
	'src/temp_filename_pattern.cc',
	vcs_tag(input: 'src/version.h.in', output: 'version.h'),
]

c = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

subdir('i18n')

if not (c.get_id() == 'gcc' and c.version().version_compare('<5'))
	if c.has_argument('-Wshadow')
		add_project_arguments('-Wshadow', language: 'c')
	endif
	if cpp.has_argument('-Wshadow')
		add_project_arguments('-Wshadow', language: 'cpp')
	endif
endif

if c.has_argument('-pipe')
	add_project_arguments('-pipe', language: 'c')
	add_project_link_arguments('-pipe', language: 'c')
endif
if cpp.has_argument('-pipe')
	add_project_arguments('-pipe', language: 'cpp')
	add_project_link_arguments('-pipe', language: 'cpp')
endif

if host_machine.system() == 'freebsd' or host_machine.system() == 'dragonfly'
	c_args = []
elif host_machine.system() == 'openbsd'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_BSD_SOURCE']
elif host_machine.system() == 'netbsd'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_NETBSD_SOURCE']
elif host_machine.system() == 'gnu'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-DPIPE_BUF=4096', '-DPATH_MAX=4096']
elif host_machine.system() == 'darwin'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_DARWIN_C_SOURCE']
elif host_machine.system() == 'cygwin'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_GNU_SOURCE']
else # linux
	c_args = ['-D_POSIX_C_SOURCE=200809L']
endif

dtee = executable('dtee',
	source_files,
	cpp_args: c_args + [
		'-DGETTEXT_PACKAGE="' + i18n_package + '"',
		'-DGETTEXT_LOCALEDIR="' + join_paths(get_option('prefix'), get_option('localedir')) + '"',
		# threads are not used, remove these if they are otherwise the asio signal handler will not work correctly
		'-DBOOST_ASIO_DISABLE_THREADS',
	] + (get_option('b_coverage') ? ['-DGCOV_ENABLED'] : []),
	dependencies: [
		dependency('boost', modules: [
			'filesystem',
			'program_options',
			'system',
		]),
	] + gettext_deps,
	install: true)

ln = find_program('ln')

custom_target('cronty',
	input: dtee,
	output: 'cronty',
	command: [ln, '-s', '-f', '@INPUT@', '@OUTPUT@'],
	install: true,
	install_dir: get_option('bindir'))

subdir('docs')
subdir('tests')

cppcheck = find_program('cppcheck', required: false)
if cppcheck.found()
	run_target('cppcheck',
		command: [
			cppcheck,
			'--project=' + join_paths(meson.current_build_dir(), 'compile_commands.json'),
			'--enable=all',
			'--inline-suppr',
		] + source_files)
endif
