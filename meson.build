project('dtee',
	['c', 'cpp'],
	default_options: [
		'c_std=c11',
		'cpp_std=c++14',
		'warning_level=3',
		'werror=true',
		'b_lto=true',
	],
	version: '0.0.0',
	license: 'GPL3+',
	meson_version: '>=0.41.0',
)

source_files = [
	'src/application.cc',
	'src/command_line.cc',
	'src/copy.cc',
	'src/cron.cc',
	'src/file_output.cc',
	'src/input.cc',
	'src/main.cc',
	'src/process.cc',
	'src/stream_output.cc',
	'src/temp_directory.cc',
	'src/temp_file.cc',
	'src/temp_filename_pattern.cc',
	'src/to_string.cc',
	vcs_tag(input: 'src/version.h.in', output: 'version.h'),
]

compile_args = [
	'-Wshadow',
	'-g3',
]
c = meson.get_compiler('c')
cc = meson.get_compiler('cpp')
foreach arg: compile_args
	if c.has_argument(arg)
		add_project_arguments(arg, language: 'c')
	endif
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
	] + (get_option('b_coverage') ? ['-DGCOV_ENABLED'] : []),
	dependencies: [
		dependency('boost', modules: [
			'filesystem',
			'program_options',
			'system',
		]),
	],
	install: true)

custom_target('cronty',
	command: [find_program('ln'), '-s', '-f', 'dtee', 'cronty'],
	output: 'cronty',
	install: true,
	install_dir: get_option('bindir'))

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

test_c_args = [
	'-D_POSIX_C_SOURCE=200809L',
]

dlfcn_args = test_c_args + [
	'-D_GNU_SOURCE',
	'-Wno-pedantic',
]
libdl = meson.get_compiler('c').find_library('dl')
dlfcn_deps = [
	libdl,
]

testlib_allow_n_times = shared_library('test-allow-n-times', ['tests/allow-n-times.c'], c_args: test_c_args)
testlib_is_dtee = shared_library('test-is-dtee', ['tests/is-dtee.c'], c_args: test_c_args)
testlib_is_fd_unix_socket = shared_library('test-fd-unix-socket', ['tests/is-fd-unix-socket.c'], c_args: test_c_args)

shared_library('test-dup2-failure', ['tests/dup2-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-execvp-fd-check', ['tests/execvp-fd-check.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-getpid', ['tests/fake-getpid.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-getuid', ['tests/fake-getuid.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fork-failure', ['tests/fork-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-file-write-failure-recovery', ['tests/file-write-failure-recovery.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-kill-failure', ['tests/kill-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-lseek-failure', ['tests/lseek-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-lseek64-failure', ['tests/lseek64-failure.c'] , c_args: dlfcn_args + ['-D_LARGEFILE64_SOURCE'], dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkdtemp-consistent', ['tests/mkdtemp-consistent.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkdtemp-failure', ['tests/mkdtemp-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-consistent', ['tests/mkostemp-consistent.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-failure', ['tests/mkostemp-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-readonly', ['tests/mkostemp-readonly.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-writeonly', ['tests/mkostemp-writeonly.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-security-recreate-output-socket', ['tests/security-recreate-output-socket.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-add-extra-source', ['tests/socket-add-extra-source.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-bind-failure', ['tests/socket-bind-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-close-failure', ['tests/socket-close-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-connect-failure', ['tests/socket-connect-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-getsockopt-fake-rcvbuf', ['tests/socket-getsockopt-fake-rcvbuf.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-recv-buffer-too-small', ['tests/socket-recv-buffer-too-small.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-recv-failure', ['tests/socket-recv-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-setsockopt-monitor', ['tests/socket-setsockopt-monitor.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-shutdown-failure', ['tests/socket-shutdown-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-unix-failure', ['tests/socket-unix-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee])
shared_library('test-waitpid-failure', ['tests/waitpid-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])

executable('test-closed-pipe-reader', ['tests/closed-pipe-reader.c'], c_args: test_c_args)
executable('test-closed-pipe-writer', ['tests/closed-pipe-writer.c'], c_args: test_c_args)
executable('test-lorem-ipsum-late-exit', ['tests/lorem-ipsum-late-exit.c'], c_args: test_c_args)
executable('test-lorem-ipsum-wait-exit', ['tests/lorem-ipsum-wait-exit.c'], c_args: test_c_args)
executable('test-kill-pid', ['tests/kill-pid.c'], c_args: test_c_args)
executable('test-kill-ppid', ['tests/kill-ppid.c'], c_args: test_c_args, link_with: [testlib_is_fd_unix_socket])
executable('test-report', ['tests/report.c'], c_args: test_c_args, link_with: [testlib_is_fd_unix_socket])
executable('test-security-recreate-input-socket', ['tests/security-recreate-input-socket.c'], c_args: test_c_args)
executable('test-waitpid', ['tests/waitpid.c'], c_args: test_c_args)
executable('test-write-0-bytes', ['tests/write-0-bytes.c'], c_args: test_c_args)
executable('test-writes-of-n-bytes', ['tests/writes-of-n-bytes.c'], c_args: test_c_args)

bash = find_program('bash')

test('dtee (no arguments)', bash, args: [files('tests/noargs-dtee.sh')], env: test_env)
test('cronty (no arguments)', bash, args: [files('tests/noargs-cronty.sh')], env: test_env)
test('dtee --help', bash, args: [files('tests/help-dtee.sh')], env: test_env)
test('cronty --help', bash, args: [files('tests/help-cronty.sh')], env: test_env)
test('dtee --version', bash, args: [files('tests/version.sh')], env: test_env)

test('passthrough stdout', bash, args: [files('tests/passthrough-stdout.sh')], env: test_env)
test('passthrough stderr', bash, args: [files('tests/passthrough-stderr.sh')], env: test_env)
test('passthrough stdout and stderr 1', bash, args: [files('tests/passthrough-stdout-stderr1.sh')], env: test_env)
test('passthrough stdout and stderr 2', bash, args: [files('tests/passthrough-stdout-stderr2.sh')], env: test_env)
test('interleaved characters', bash, args: [files('tests/interleaved-characters.sh')], env: test_env)

test('file stdout overwrite (new file)', bash, args: [files('tests/file-stdout-overwrite-new.sh')], env: test_env)
test('file stdout overwrite (existing)', bash, args: [files('tests/file-stdout-overwrite-existing.sh')], env: test_env)
test('file stdout append (new file)', bash, args: [files('tests/file-stdout-append-new.sh')], env: test_env)
test('file stdout append (existing)', bash, args: [files('tests/file-stdout-append-existing.sh')], env: test_env)

test('file stderr overwrite (new file)', bash, args: [files('tests/file-stderr-overwrite-new.sh')], env: test_env)
test('file stderr overwrite (existing)', bash, args: [files('tests/file-stderr-overwrite-existing.sh')], env: test_env)
test('file stderr append (new file)', bash, args: [files('tests/file-stderr-append-new.sh')], env: test_env)
test('file stderr append (existing)', bash, args: [files('tests/file-stderr-append-existing.sh')], env: test_env)

test('file combined overwrite (new file)', bash, args: [files('tests/file-combined-overwrite-new.sh')], env: test_env)
test('file combined overwrite (existing)', bash, args: [files('tests/file-combined-overwrite-existing.sh')], env: test_env)
test('file combined append (new file)', bash, args: [files('tests/file-combined-append-new.sh')], env: test_env)
test('file combined append (existing)', bash, args: [files('tests/file-combined-append-existing.sh')], env: test_env)

test('write 0 bytes (stdout)', bash, args: [files('tests/write-0-bytes-stdout.sh')], env: test_env)
test('write 0 bytes (stderr)', bash, args: [files('tests/write-0-bytes-stderr.sh')], env: test_env)
test('writes of PIPE_BUF bytes (stdout)', bash, args: [files('tests/writes-of-PIPE_BUF-bytes-stdout.sh')], env: test_env)
test('writes of PIPE_BUF bytes (stderr)', bash, args: [files('tests/writes-of-PIPE_BUF-bytes-stderr.sh')], env: test_env)
test('cron write 0 bytes (stdout)', bash, args: [files('tests/cron-write-0-bytes-stdout.sh')], env: test_env)
test('cron write 0 bytes (stderr)', bash, args: [files('tests/cron-write-0-bytes-stderr.sh')], env: test_env)
test('cron writes of PIPE_BUF bytes (stdout)', bash, args: [files('tests/cron-writes-of-PIPE_BUF-bytes-stdout.sh')], env: test_env)
test('cron writes of PIPE_BUF bytes (stderr)', bash, args: [files('tests/cron-writes-of-PIPE_BUF-bytes-stderr.sh')], env: test_env)

test('debug options (none)', bash, args: [files('tests/options-none.sh')], env: test_env)
test('debug options (cron)', bash, args: [files('tests/options-cron.sh')], env: test_env)
test('debug options (invalid short options)', bash, args: [files('tests/options-invalid-short.sh')], env: test_env)
test('debug options (invalid short options with arguments)', bash, args: [files('tests/options-invalid-short-args.sh')], env: test_env)
test('debug options (invalid long options)', bash, args: [files('tests/options-invalid-long.sh')], env: test_env)
test('debug options (invalid long options with arguments)', bash, args: [files('tests/options-invalid-long-args.sh')], env: test_env)
test('debug options (1 file)', bash, args: [files('tests/options-1-file.sh')], env: test_env)
test('debug options (2 files)', bash, args: [files('tests/options-2-files.sh')], env: test_env)
test('debug options (3 files)', bash, args: [files('tests/options-3-files.sh')], env: test_env)
test('debug options (command with no arguments)', bash, args: [files('tests/options-cmd-noargs.sh')], env: test_env)
test('debug options (command with 1 argument)', bash, args: [files('tests/options-cmd-1-arg.sh')], env: test_env)
test('debug options (command with 2 arguments)', bash, args: [files('tests/options-cmd-2-args.sh')], env: test_env)
test('debug options (command with 3 arguments)', bash, args: [files('tests/options-cmd-3-args.sh')], env: test_env)
test('debug options (command stops following options)', bash, args: [files('tests/options-cmd-opts.sh')], env: test_env)
test('debug options (obscure boost syntax)', bash, args: [files('tests/options-cmd-obscure-boost-syntax.sh')], env: test_env)
test('debug options (empty command with no arguments)', bash, args: [files('tests/options-cmd-empty-noargs.sh')], env: test_env)
test('debug options (empty command with 1 argument)', bash, args: [files('tests/options-cmd-empty-1-arg.sh')], env: test_env)
test('debug options (empty command with 2 arguments)', bash, args: [files('tests/options-cmd-empty-2-args.sh')], env: test_env)
test('debug options (empty command with 3 arguments)', bash, args: [files('tests/options-cmd-empty-3-args.sh')], env: test_env)
test('debug options (empty filename and empty command)', bash, args: [files('tests/options-mixed-empty1.sh')], env: test_env)
test('debug options (empty filename and empty command with empty argument)', bash, args: [files('tests/options-mixed-empty2.sh')], env: test_env)
test('debug options (empty command stops following options)', bash, args: [files('tests/options-mixed-empty3.sh')], env: test_env)
test('debug options (empty command stops following options even when empty)', bash, args: [files('tests/options-mixed-empty4.sh')], env: test_env)
test('debug options (empty command stops following options)', bash, args: [files('tests/options-mixed-empty5.sh')], env: test_env)
test('debug options (empty command stops following options)', bash, args: [files('tests/options-mixed-empty6.sh')], env: test_env)
test('debug options (end of options signifier stops following options)', bash, args: [files('tests/options-mixed-split1.sh')], env: test_env)
test('debug options (end of options signifier stops following options even with immediate opts)', bash, args: [files('tests/options-mixed-split2.sh')], env: test_env)
test('debug options (end of options signifier stops following options at the start)', bash, args: [files('tests/options-cmd-split1.sh')], env: test_env)
test('debug options (end of options signifier stops following options at the start even with empty command)', bash, args: [files('tests/options-cmd-split2.sh')], env: test_env)
test('debug options (end of options signifier stops following options at the start even with empty command and empty argument)', bash, args: [files('tests/options-cmd-split3.sh')], env: test_env)

test('command not found', bash, args: [files('tests/command-not-found.sh')], env: test_env)
test('command not executable', bash, args: [files('tests/command-not-executable.sh')], env: test_env)
test('command exit code 42', bash, args: [files('tests/command-exit-code.sh')], env: test_env)
test('command terminated by signal 1', bash, args: [files('tests/command-sighup.sh')], env: test_env)
test('command terminated by signal 2', bash, args: [files('tests/command-sigint.sh')], env: test_env)
test('command terminated by signal 2 (ignore interrupts)', bash, args: [files('tests/command-sigint-ignore-sigint.sh')], env: test_env)
test('command terminated by signal 2 (self kill failure)', bash, args: [files('tests/command-sigint-self-kill-failure.sh')], env: test_env)
test('command terminated by signal 3', bash, args: [files('tests/command-sigquit.sh')], env: test_env)
test('command terminated by signal 9', bash, args: [files('tests/command-sigkill.sh')], env: test_env)
test('command terminated by signal 15', bash, args: [files('tests/command-sigterm.sh')], env: test_env)

test('cron command not found', bash, args: [files('tests/cron-command-not-found.sh')], env: test_env)
test('cron command not executable', bash, args: [files('tests/cron-command-not-executable.sh')], env: test_env)
test('cron command exit code 0 (stderr)', bash, args: [files('tests/cron-command-exit-code-0-stderr.sh')], env: test_env)
test('cron command exit code 42 (stderr)', bash, args: [files('tests/cron-command-exit-code-42-stderr.sh')], env: test_env)
test('cron command exit code 0 (no stderr)', bash, args: [files('tests/cron-command-exit-code-0-no-stderr.sh')], env: test_env)
test('cron command exit code 42 (no stderr)', bash, args: [files('tests/cron-command-exit-code-42-no-stderr.sh')], env: test_env)
test('cronty command exit code 0 (stderr)', bash, args: [files('tests/cronty-command-exit-code-0-stderr.sh')], env: test_env)
test('cronty command exit code 42 (stderr)', bash, args: [files('tests/cronty-command-exit-code-42-stderr.sh')], env: test_env)
test('cronty command exit code 0 (no stderr)', bash, args: [files('tests/cronty-command-exit-code-0-no-stderr.sh')], env: test_env)
test('cronty command exit code 42 (no stderr)', bash, args: [files('tests/cronty-command-exit-code-42-no-stderr.sh')], env: test_env)
test('cron command terminated by signal 1', bash, args: [files('tests/cron-command-sighup.sh')], env: test_env)
test('cron command terminated by signal 2', bash, args: [files('tests/cron-command-sigint.sh')], env: test_env)
test('cron command terminated by signal 2 (ignore interrupts)', bash, args: [files('tests/cron-command-sigint-ignore-sigint.sh')], env: test_env)
test('cron command terminated by signal 2 (self kill failure)', bash, args: [files('tests/cron-command-sigint-self-kill-failure.sh')], env: test_env)
test('cron command terminated by signal 3 (coredump)', bash, args: [files('tests/cron-command-sigquit-coredump.sh')], env: test_env)
test('cron command terminated by signal 3 (no coredump)', bash, args: [files('tests/cron-command-sigquit-no-coredump.sh')], env: test_env)
test('cron command terminated by signal 9', bash, args: [files('tests/cron-command-sigkill.sh')], env: test_env)
test('cron command terminated by signal 15', bash, args: [files('tests/cron-command-sigterm.sh')], env: test_env)

test('output file not openable', bash, args: [files('tests/file-open-failure.sh')], env: test_env)
test('output file not writeable (stdout)', bash, args: [files('tests/file-write-failure-stdout.sh')], env: test_env)
test('output file not writeable (stderr)', bash, args: [files('tests/file-write-failure-stderr.sh')], env: test_env)
test('output file not writeable (combined)', bash, args: [files('tests/file-write-failure-combined.sh')], env: test_env)

test('cron normal output file not openable', bash, args: [files('tests/cron-file-open-failure-normal.sh')], env: test_env)
test('cron normal output file not writeable (stdout)', bash, args: [files('tests/cron-file-write-failure-stdout.sh')], env: test_env)
test('cron normal output file not writeable (stderr)', bash, args: [files('tests/cron-file-write-failure-stderr.sh')], env: test_env)
test('cron normal output file not writeable (combined)', bash, args: [files('tests/cron-file-write-failure-combined.sh')], env: test_env)
test('cron temporary output file not openable', bash, args: [files('tests/cron-file-open-failure-cron.sh')], env: test_env)
test('cron temporary output file not writeable', bash, args: [files('tests/cron-file-write-failure-cron.sh')], env: test_env)
test('cron temporary output file not readable', bash, args: [files('tests/cron-file-read-failure-cron.sh')], env: test_env)
test('cron temporary output file not seekable', bash, args: [files('tests/cron-file-seek-failure-cron.sh')], env: test_env)

test('output file writes fail and recover', bash, args: [files('tests/file-write-failure-recovery.sh')], env: test_env)
test('cron normal output file writes fail and recover (exit code 0)', bash, args: [files('tests/cron-file-write-failure-recovery-exit-0.sh')], env: test_env)
test('cron normal output file writes fail and recover (exit code 42)', bash, args: [files('tests/cron-file-write-failure-recovery-exit-42.sh')], env: test_env)

test('input temporary directory create failure', bash, args: [files('tests/input-temp-dir-failure.sh')], env: test_env)
test('input socket create failure', bash, args: [files('tests/input-socket-create-failure.sh')], env: test_env)
test('input socket create failure 2', bash, args: [files('tests/input-socket-create-failure-2.sh')], env: test_env)
test('input socket create failure 3', bash, args: [files('tests/input-socket-create-failure-3.sh')], env: test_env)
test('input socket create and output file open failure', bash, args: [files('tests/input-and-output-failure.sh')], env: test_env)
test('input socket bind failure', bash, args: [files('tests/input-socket-bind-failure.sh')], env: test_env)
test('input socket bind failure 2', bash, args: [files('tests/input-socket-bind-failure-2.sh')], env: test_env)
test('input socket bind failure 3', bash, args: [files('tests/input-socket-bind-failure-3.sh')], env: test_env)
test('input socket connect failure', bash, args: [files('tests/input-socket-connect-failure.sh')], env: test_env)
test('input socket connect failure 2', bash, args: [files('tests/input-socket-connect-failure-2.sh')], env: test_env)
test('input socket receive failure', bash, args: [files('tests/input-socket-recv-failure.sh')], env: test_env)
test('input socket shutdown failure', bash, args: [files('tests/input-socket-shutdown-failure.sh')], env: test_env)
test('input socket shutdown failure 2', bash, args: [files('tests/input-socket-shutdown-failure-2.sh')], env: test_env)
test('input socket shutdown failure 3', bash, args: [files('tests/input-socket-shutdown-failure-3.sh')], env: test_env)

test('cron input temporary directory create failure', bash, args: [files('tests/cron-input-temp-dir-failure.sh')], env: test_env)
test('cron input socket create failure', bash, args: [files('tests/cron-input-socket-create-failure.sh')], env: test_env)
test('cron input socket create and output file open failure', bash, args: [files('tests/cron-input-and-output-failure.sh')], env: test_env)
test('cron input socket bind failure', bash, args: [files('tests/cron-input-socket-bind-failure.sh')], env: test_env)
test('cron input socket connect failure', bash, args: [files('tests/cron-input-socket-connect-failure.sh')], env: test_env)
test('cron input socket receive failure', bash, args: [files('tests/cron-input-socket-recv-failure.sh')], env: test_env)
test('cron input socket shutdown failure', bash, args: [files('tests/cron-input-socket-shutdown-failure.sh')], env: test_env)

test('input socket rcvbuf large enough', bash, args: [files('tests/input-socket-rcvbuf-large-enough.sh')], env: test_env)
test('input socket rcvbuf minimum size', bash, args: [files('tests/input-socket-rcvbuf-minimum-size.sh')], env: test_env)
test('input socket rcvbuf too small', bash, args: [files('tests/input-socket-rcvbuf-too-small.sh')], env: test_env)

test('input socket message from unknown source', bash, args: [files('tests/input-socket-extra-source.sh')], env: test_env)
test('input socket receive buffer too small', bash, args: [files('tests/input-socket-recv-buffer-too-small.sh')], env: test_env)

test('report arguments and file descriptors', bash, args: [files('tests/report.sh')], env: test_env)
test('report arguments and file descriptors (no stdin)', bash, args: [files('tests/report-no-stdin.sh')], env: test_env)
test('report arguments and file descriptors (extra fd)', bash, args: [files('tests/report-extra-fd.sh')], env: test_env)
test('cron report arguments and file descriptors', bash, args: [files('tests/cron-report.sh')], env: test_env)
test('cron report arguments and file descriptors (no stdin)', bash, args: [files('tests/cron-report-no-stdin.sh')], env: test_env)
test('cron report arguments and file descriptors (extra fd)', bash, args: [files('tests/cron-report-extra-fd.sh')], env: test_env)

test('dtee killed by signal 1', bash, args: [files('tests/command-kill-ppid-sighup.sh')], env: test_env)
test('dtee killed by signal 2', bash, args: [files('tests/command-kill-ppid-sigint.sh')], env: test_env)
test('dtee killed by signal 2 (ignore interrupts)', bash, args: [files('tests/command-kill-ppid-sigint-ignore-sigint.sh')], env: test_env)
test('dtee killed by signal 3', bash, args: [files('tests/command-kill-ppid-sigquit.sh')], env: test_env)
test('dtee killed by signal 9', bash, args: [files('tests/command-kill-ppid-sigkill.sh')], env: test_env)
test('dtee killed by signal 15', bash, args: [files('tests/command-kill-ppid-sigterm.sh')], env: test_env)
test('cron dtee killed by signal 1', bash, args: [files('tests/cron-command-kill-ppid-sighup.sh')], env: test_env)
test('cron dtee killed by signal 1 (self kill failure)', bash, args: [files('tests/cron-command-kill-ppid-sighup-self-kill-failure.sh')], env: test_env)
test('cron dtee killed by signal 2', bash, args: [files('tests/cron-command-kill-ppid-sigint.sh')], env: test_env)
test('cron dtee killed by signal 2 (ignore interrupts)', bash, args: [files('tests/cron-command-kill-ppid-sigint-ignore-sigint.sh')], env: test_env)
test('cron dtee killed by signal 2 (self kill failure)', bash, args: [files('tests/cron-command-kill-ppid-sigint-self-kill-failure.sh')], env: test_env)
test('cron dtee killed by signal 3', bash, args: [files('tests/cron-command-kill-ppid-sigquit.sh')], env: test_env)
test('cron dtee killed by signal 9', bash, args: [files('tests/cron-command-kill-ppid-sigkill.sh')], env: test_env)
test('cron dtee killed by signal 15', bash, args: [files('tests/cron-command-kill-ppid-sigterm.sh')], env: test_env)
test('cron dtee killed by signal 15 (self kill failure)', bash, args: [files('tests/cron-command-kill-ppid-sigterm-self-kill-failure.sh')], env: test_env)

test('fork failure', bash, args: [files('tests/fork-failure.sh')], env: test_env)
test('dup2 failure', bash, args: [files('tests/dup2-failure.sh')], env: test_env)
test('socket close failure (child)', bash, args: [files('tests/socket-close-failure-child.sh')], env: test_env)
test('socket close failure (parent)', bash, args: [files('tests/socket-close-failure-parent.sh')], env: test_env)
test('cron fork failure', bash, args: [files('tests/cron-fork-failure.sh')], env: test_env)
test('cron dup2 failure', bash, args: [files('tests/cron-dup2-failure.sh')], env: test_env)
test('cron socket close failure (child)', bash, args: [files('tests/cron-socket-close-failure-child.sh')], env: test_env)
test('cron socket close failure (parent)', bash, args: [files('tests/cron-socket-close-failure-parent.sh')], env: test_env)

test('waitpid failure (no children)', bash, args: [files('tests/waitpid-failure-no-children.sh')], env: test_env)
test('waitpid failure (no change)', bash, args: [files('tests/waitpid-failure-no-change.sh')], env: test_env)
test('cron waitpid failure (no children)', bash, args: [files('tests/cron-waitpid-failure-no-children.sh')], env: test_env)
test('cron waitpid failure (no change)', bash, args: [files('tests/cron-waitpid-failure-no-change.sh')], env: test_env)

test('security recreate the input socket after the parent process exits', bash, args: [files('tests/security-recreate-input-socket.sh')], env: test_env)
test('security recreate the output socket after the child process exits', bash, args: [files('tests/security-recreate-output-socket.sh')], env: test_env)

test('pipe write failure (stdout)', bash, args: [files('tests/pipe-write-failure-stdout.sh')], env: test_env)
test('pipe write failure (stderr)', bash, args: [files('tests/pipe-write-failure-stderr.sh')], env: test_env)
test('pipe write failure (file)', bash, args: [files('tests/pipe-write-failure-file.sh')], env: test_env)
test('cron pipe write failure (stdout)', bash, args: [files('tests/cron-pipe-write-failure-stdout.sh')], env: test_env)
test('cron pipe write failure (stderr)', bash, args: [files('tests/cron-pipe-write-failure-stderr.sh')], env: test_env)
test('cron pipe write failure (stderr but no output)', bash, args: [files('tests/cron-pipe-write-failure-stderr2.sh')], env: test_env)
test('cron pipe write failure (file)', bash, args: [files('tests/cron-pipe-write-failure-file.sh')], env: test_env)

test('command writes to stdout/stderr after dtee exits', bash, args: [files('tests/command-write-after-ppid-exit.sh')], env: test_env)
