project('dtee',
	['c', 'cpp'],
	default_options: [
		'c_std=c11',
		'cpp_std=c++14',
		'warning_level=3',
		'werror=true',
	],
	version: '1.1.0',
	license: 'GPL3+',
	meson_version: '>=0.55.0',
)

c = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

if c.has_argument('-Wshadow')
	add_project_arguments('-Wshadow', language: 'c')
endif
if cpp.has_argument('-Wshadow')
	add_project_arguments('-Wshadow', language: 'cpp')
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
	c_args = ['-isystem/usr/local/include']
elif host_machine.system() == 'openbsd'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_BSD_SOURCE', '-isystem/usr/local/include']
elif host_machine.system() == 'netbsd'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_NETBSD_SOURCE']
elif host_machine.system() == 'gnu'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-DPIPE_BUF=4096', '-DPATH_MAX=4096']
elif host_machine.system() == 'darwin'
	# Workaround Apple clang bug where /usr/local/include is built-in but not considered a system directory by setting "-isystem/usr/local/include"
	# Workaround Meson bug where "-isystem/usr/local/include" is filtered out of the command line by adding an extra "/" (https://github.com/mesonbuild/meson/issues/8755)
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_DARWIN_C_SOURCE', '-isystem/usr/local/include/']
elif host_machine.system() == 'cygwin'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_GNU_SOURCE']
else # linux
	c_args = ['-D_POSIX_C_SOURCE=200809L']
endif

if host_machine.system() == 'netbsd'
	rpath = '/usr/pkg/lib'
else # linux, freebsd, openbsd, netbsd, dragonfly, gnu, darwin, cygwin
	rpath = ''
endif

subdir('src')
subdir('i18n')

boost = dependency('boost', modules: [
		'filesystem',
		'program_options',
		'system',
	], include_type: 'system')

dtee = executable('dtee',
	source_files,
	cpp_args: c_args + [
		'-DGETTEXT_PACKAGE="' + i18n_package + '"',
		'-DGETTEXT_LOCALEDIR="' + join_paths(get_option('prefix'), get_option('localedir')) + '"',
		# threads are not used, remove these if they are otherwise the asio signal handler will not work correctly
		'-DBOOST_ASIO_DISABLE_THREADS',
	] + (get_option('b_coverage') ? ['-DGCOV_ENABLED'] : []),
	dependencies: [boost] + gettext_deps,
	build_rpath: rpath,
	install_rpath: rpath,
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
