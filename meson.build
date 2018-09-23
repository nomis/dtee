project('dtee',
	'cpp',
	default_options: [
		'cpp_std=c++14',
		'warning_level=3',
		'werror=true',
		'b_lto=true',
	],
	license: 'GPL3+')

source_files = [
	'src/application.cc',
	'src/application_command_line.cc',
	'src/copy.cc',
	'src/cron.cc',
	'src/file_output.cc',
	'src/input.cc',
	'src/stream_output.cc',
	'src/temp_directory.cc',
	'src/temp_file.cc',
	'src/main.cc',
]

test_args = [
	'-Wshadow',
	'-g3',
]
cc = meson.get_compiler('cpp')
foreach arg: test_args
	if cc.has_argument(arg)
		add_project_arguments(arg, language: 'cpp')
	endif
endforeach

executable('dtee',
	source_files,
	cpp_args: [
		'-D_POSIX_C_SOURCE=200809L',

		# threads are not used, remove these if they are otherwise the asio signal handler will not work correctly
		'-DBOOST_DISABLE_THREADS',
		'-DBOOST_ASIO_DISABLE_THREADS',
	],
	dependencies: [
		dependency('boost', modules: [
			'asio',
			'program_options',
			'system',
		]),
	],
	install: true)

meson.add_install_script('sh',
	'-c',
	'ln -s dtee ${DESTDIR}/${MESON_INSTALL_PREFIX}/cronic')

run_target('cppcheck',
	command: [
		'cppcheck',
		'--project=' + join_paths(meson.current_build_dir(), 'compile_commands.json'),
		'--enable=all',
		'--inline-suppr',
	] + source_files)

run_target('clang-check',
	command: [
		'clang-check',
		'-p', meson.current_build_dir(),
		'-analyze',
		'-fixit',
	] + source_files)
