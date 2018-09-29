project('dtee',
	'cpp',
	default_options: [
		'cpp_std=c++14',
		'warning_level=3',
		'werror=true',
		'b_lto=true',
		'b_coverage=true',
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
			'program_options',
			'system',
		]),
	],
	install: true)

meson.add_install_script('sh',
	'-c',
	'ln -s dtee ${DESTDIR}/${MESON_INSTALL_PREFIX}/cronty')

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

test_env = [
	'LC_ALL=C',
	'LANG=C',
]

test('dtee (no arguments)', find_program('tests/noargs-dtee.sh'), env: test_env)
test('cronty (no arguments)', find_program('tests/noargs-cronty.sh'), env: test_env)
test('dtee --help', find_program('tests/help-dtee.sh'), env: test_env)
test('cronty --help', find_program('tests/help-cronty.sh'), env: test_env)
test('dtee --version', find_program('tests/version.sh'), env: test_env)

test('passthrough stdout', find_program('tests/passthrough-stdout.sh'), env: test_env)
test('passthrough stderr', find_program('tests/passthrough-stderr.sh'), env: test_env)
test('passthrough stdout and stderr 1', find_program('tests/passthrough-stdout-stderr1.sh'), env: test_env)
test('passthrough stdout and stderr 2', find_program('tests/passthrough-stdout-stderr2.sh'), env: test_env)
test('interleaved characters', find_program('tests/interleaved-characters.sh'), env: test_env)

test('file stdout overwrite (new file)', find_program('tests/file-stdout-overwrite-new.sh'), env: test_env)
test('file stdout overwrite (existing)', find_program('tests/file-stdout-overwrite-existing.sh'), env: test_env)
test('file stdout append (new file)', find_program('tests/file-stdout-append-new.sh'), env: test_env)
test('file stdout append (existing)', find_program('tests/file-stdout-append-existing.sh'), env: test_env)

test('file stderr overwrite (new file)', find_program('tests/file-stderr-overwrite-new.sh'), env: test_env)
test('file stderr overwrite (existing)', find_program('tests/file-stderr-overwrite-existing.sh'), env: test_env)
test('file stderr append (new file)', find_program('tests/file-stderr-append-new.sh'), env: test_env)
test('file stderr append (existing)', find_program('tests/file-stderr-append-existing.sh'), env: test_env)

test('file combined overwrite (new file)', find_program('tests/file-combined-overwrite-new.sh'), env: test_env)
test('file combined overwrite (existing)', find_program('tests/file-combined-overwrite-existing.sh'), env: test_env)
test('file combined append (new file)', find_program('tests/file-combined-append-new.sh'), env: test_env)
test('file combined append (existing)', find_program('tests/file-combined-append-existing.sh'), env: test_env)

test('debug options (none)', find_program('tests/options-none.sh'), env: test_env)
test('debug options (cron)', find_program('tests/options-cron.sh'), env: test_env)
test('debug options (invalid short options)', find_program('tests/options-invalid-short.sh'), env: test_env)
test('debug options (invalid short options with arguments)', find_program('tests/options-invalid-short-args.sh'), env: test_env)
test('debug options (invalid long options)', find_program('tests/options-invalid-long.sh'), env: test_env)
test('debug options (invalid long options with arguments)', find_program('tests/options-invalid-long-args.sh'), env: test_env)
test('debug options (1 file)', find_program('tests/options-1-file.sh'), env: test_env)
test('debug options (2 files)', find_program('tests/options-2-files.sh'), env: test_env)
test('debug options (3 files)', find_program('tests/options-3-files.sh'), env: test_env)
test('debug options (command with no arguments)', find_program('tests/options-cmd-noargs.sh'), env: test_env)
test('debug options (command with 1 argument)', find_program('tests/options-cmd-1-arg.sh'), env: test_env)
test('debug options (command with 2 arguments)', find_program('tests/options-cmd-2-args.sh'), env: test_env)
test('debug options (command with 3 arguments)', find_program('tests/options-cmd-3-args.sh'), env: test_env)
test('debug options (command stops following options)', find_program('tests/options-cmd-opts.sh'), env: test_env)
test('debug options (obscure boost syntax)', find_program('tests/options-cmd-obscure-boost-syntax.sh'), env: test_env)
test('debug options (empty command with no arguments)', find_program('tests/options-cmd-empty-noargs.sh'), env: test_env)
test('debug options (empty command with 1 argument)', find_program('tests/options-cmd-empty-1-arg.sh'), env: test_env)
test('debug options (empty command with 2 arguments)', find_program('tests/options-cmd-empty-2-args.sh'), env: test_env)
test('debug options (empty command with 3 arguments)', find_program('tests/options-cmd-empty-3-args.sh'), env: test_env)
test('debug options (empty filename and empty command)', find_program('tests/options-mixed-empty1.sh'), env: test_env)
test('debug options (empty filename and empty command with empty argument)', find_program('tests/options-mixed-empty2.sh'), env: test_env)
test('debug options (empty command stops following options)', find_program('tests/options-mixed-empty3.sh'), env: test_env)
test('debug options (empty command stops following options even when empty)', find_program('tests/options-mixed-empty4.sh'), env: test_env)
test('debug options (empty command stops following options)', find_program('tests/options-mixed-empty5.sh'), env: test_env)
test('debug options (empty command stops following options)', find_program('tests/options-mixed-empty6.sh'), env: test_env)
test('debug options (end of options signifier stops following options)', find_program('tests/options-mixed-split1.sh'), env: test_env)
test('debug options (end of options signifier stops following options even with immediate opts)', find_program('tests/options-mixed-split2.sh'), env: test_env)
test('debug options (end of options signifier stops following options at the start)', find_program('tests/options-cmd-split1.sh'), env: test_env)
test('debug options (end of options signifier stops following options at the start even with empty command)', find_program('tests/options-cmd-split2.sh'), env: test_env)
test('debug options (end of options signifier stops following options at the start even with empty command and empty argument)', find_program('tests/options-cmd-split3.sh'), env: test_env)

test('command not found', find_program('tests/command-not-found.sh'), env: test_env)
test('command not executable', find_program('tests/command-not-executable.sh'), env: test_env)
test('command exit code 42', find_program('tests/command-exit-code.sh'), env: test_env)
test('command terminated by signal 2', find_program('tests/command-sigint.sh'), env: test_env)
test('command terminated by signal 9', find_program('tests/command-sigkill.sh'), env: test_env)
test('command terminated by signal 15', find_program('tests/command-sigterm.sh'), env: test_env)

test('cron command not found', find_program('tests/cron-command-not-found.sh'), env: test_env)
test('cron command not executable', find_program('tests/cron-command-not-executable.sh'), env: test_env)
test('cron command exit code 0 (stderr)', find_program('tests/cron-command-exit-code-0-stderr.sh'), env: test_env)
test('cron command exit code 42 (stderr)', find_program('tests/cron-command-exit-code-42-stderr.sh'), env: test_env)
test('cron command exit code 0 (no stderr)', find_program('tests/cron-command-exit-code-0-no-stderr.sh'), env: test_env)
test('cron command exit code 42 (no stderr)', find_program('tests/cron-command-exit-code-42-no-stderr.sh'), env: test_env)
test('cron terminated by signal 2', find_program('tests/cron-command-sigint.sh'), env: test_env)
test('cron terminated by signal 9', find_program('tests/cron-command-sigkill.sh'), env: test_env)
test('cron terminated by signal 15', find_program('tests/cron-command-sigterm.sh'), env: test_env)
