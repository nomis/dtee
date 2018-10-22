project('dtee',
	['c', 'cpp'],
	default_options: [
		'c_std=c11',
		'cpp_std=c++14',
		'warning_level=3',
		'werror=true',
		'b_lto=true',
	],
	license: 'GPL3+')

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

test('write 0 bytes (stdout)', find_program('tests/write-0-bytes-stdout.sh'), env: test_env)
test('write 0 bytes (stderr)', find_program('tests/write-0-bytes-stderr.sh'), env: test_env)
test('writes of PIPE_BUF bytes (stdout)', find_program('tests/writes-of-PIPE_BUF-bytes-stdout.sh'), env: test_env)
test('writes of PIPE_BUF bytes (stderr)', find_program('tests/writes-of-PIPE_BUF-bytes-stderr.sh'), env: test_env)
test('cron write 0 bytes (stdout)', find_program('tests/cron-write-0-bytes-stdout.sh'), env: test_env)
test('cron write 0 bytes (stderr)', find_program('tests/cron-write-0-bytes-stderr.sh'), env: test_env)
test('cron writes of PIPE_BUF bytes (stdout)', find_program('tests/cron-writes-of-PIPE_BUF-bytes-stdout.sh'), env: test_env)
test('cron writes of PIPE_BUF bytes (stderr)', find_program('tests/cron-writes-of-PIPE_BUF-bytes-stderr.sh'), env: test_env)

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
test('command terminated by signal 1', find_program('tests/command-sighup.sh'), env: test_env)
test('command terminated by signal 2', find_program('tests/command-sigint.sh'), env: test_env)
test('command terminated by signal 2 (ignore interrupts)', find_program('tests/command-sigint-ignore-sigint.sh'), env: test_env)
test('command terminated by signal 2 (self kill failure)', find_program('tests/command-sigint-self-kill-failure.sh'), env: test_env)
test('command terminated by signal 3', find_program('tests/command-sigquit.sh'), env: test_env)
test('command terminated by signal 9', find_program('tests/command-sigkill.sh'), env: test_env)
test('command terminated by signal 15', find_program('tests/command-sigterm.sh'), env: test_env)

test('cron command not found', find_program('tests/cron-command-not-found.sh'), env: test_env)
test('cron command not executable', find_program('tests/cron-command-not-executable.sh'), env: test_env)
test('cron command exit code 0 (stderr)', find_program('tests/cron-command-exit-code-0-stderr.sh'), env: test_env)
test('cron command exit code 42 (stderr)', find_program('tests/cron-command-exit-code-42-stderr.sh'), env: test_env)
test('cron command exit code 0 (no stderr)', find_program('tests/cron-command-exit-code-0-no-stderr.sh'), env: test_env)
test('cron command exit code 42 (no stderr)', find_program('tests/cron-command-exit-code-42-no-stderr.sh'), env: test_env)
test('cronty command exit code 0 (stderr)', find_program('tests/cronty-command-exit-code-0-stderr.sh'), env: test_env)
test('cronty command exit code 42 (stderr)', find_program('tests/cronty-command-exit-code-42-stderr.sh'), env: test_env)
test('cronty command exit code 0 (no stderr)', find_program('tests/cronty-command-exit-code-0-no-stderr.sh'), env: test_env)
test('cronty command exit code 42 (no stderr)', find_program('tests/cronty-command-exit-code-42-no-stderr.sh'), env: test_env)
test('cron command terminated by signal 1', find_program('tests/cron-command-sighup.sh'), env: test_env)
test('cron command terminated by signal 2', find_program('tests/cron-command-sigint.sh'), env: test_env)
test('cron command terminated by signal 2 (ignore interrupts)', find_program('tests/cron-command-sigint-ignore-sigint.sh'), env: test_env)
test('cron command terminated by signal 2 (self kill failure)', find_program('tests/cron-command-sigint-self-kill-failure.sh'), env: test_env)
test('cron command terminated by signal 3 (coredump)', find_program('tests/cron-command-sigquit-coredump.sh'), env: test_env)
test('cron command terminated by signal 3 (no coredump)', find_program('tests/cron-command-sigquit-no-coredump.sh'), env: test_env)
test('cron command terminated by signal 9', find_program('tests/cron-command-sigkill.sh'), env: test_env)
test('cron command terminated by signal 15', find_program('tests/cron-command-sigterm.sh'), env: test_env)

test('output file not openable', find_program('tests/file-open-failure.sh'), env: test_env)
test('output file not writeable (stdout)', find_program('tests/file-write-failure-stdout.sh'), env: test_env)
test('output file not writeable (stderr)', find_program('tests/file-write-failure-stderr.sh'), env: test_env)
test('output file not writeable (combined)', find_program('tests/file-write-failure-combined.sh'), env: test_env)

test('cron normal output file not openable', find_program('tests/cron-file-open-failure-normal.sh'), env: test_env)
test('cron normal output file not writeable (stdout)', find_program('tests/cron-file-write-failure-stdout.sh'), env: test_env)
test('cron normal output file not writeable (stderr)', find_program('tests/cron-file-write-failure-stderr.sh'), env: test_env)
test('cron normal output file not writeable (combined)', find_program('tests/cron-file-write-failure-combined.sh'), env: test_env)
test('cron temporary output file not openable', find_program('tests/cron-file-open-failure-cron.sh'), env: test_env)
test('cron temporary output file not writeable', find_program('tests/cron-file-write-failure-cron.sh'), env: test_env)
test('cron temporary output file not readable', find_program('tests/cron-file-read-failure-cron.sh'), env: test_env)
test('cron temporary output file not seekable', find_program('tests/cron-file-seek-failure-cron.sh'), env: test_env)

test('output file writes fail and recover', find_program('tests/file-write-failure-recovery.sh'), env: test_env)
test('cron normal output file writes fail and recover (exit code 0)', find_program('tests/cron-file-write-failure-recovery-exit-0.sh'), env: test_env)
test('cron normal output file writes fail and recover (exit code 42)', find_program('tests/cron-file-write-failure-recovery-exit-42.sh'), env: test_env)

test('input temporary directory create failure', find_program('tests/input-temp-dir-failure.sh'), env: test_env)
test('input socket create failure', find_program('tests/input-socket-create-failure.sh'), env: test_env)
test('input socket create failure 2', find_program('tests/input-socket-create-failure-2.sh'), env: test_env)
test('input socket create failure 3', find_program('tests/input-socket-create-failure-3.sh'), env: test_env)
test('input socket create and output file open failure', find_program('tests/input-and-output-failure.sh'), env: test_env)
test('input socket bind failure', find_program('tests/input-socket-bind-failure.sh'), env: test_env)
test('input socket bind failure 2', find_program('tests/input-socket-bind-failure-2.sh'), env: test_env)
test('input socket bind failure 3', find_program('tests/input-socket-bind-failure-3.sh'), env: test_env)
test('input socket connect failure', find_program('tests/input-socket-connect-failure.sh'), env: test_env)
test('input socket connect failure 2', find_program('tests/input-socket-connect-failure-2.sh'), env: test_env)
test('input socket receive failure', find_program('tests/input-socket-recv-failure.sh'), env: test_env)
test('input socket shutdown failure', find_program('tests/input-socket-shutdown-failure.sh'), env: test_env)
test('input socket shutdown failure 2', find_program('tests/input-socket-shutdown-failure-2.sh'), env: test_env)
test('input socket shutdown failure 3', find_program('tests/input-socket-shutdown-failure-3.sh'), env: test_env)

test('cron input temporary directory create failure', find_program('tests/cron-input-temp-dir-failure.sh'), env: test_env)
test('cron input socket create failure', find_program('tests/cron-input-socket-create-failure.sh'), env: test_env)
test('cron input socket create and output file open failure', find_program('tests/cron-input-and-output-failure.sh'), env: test_env)
test('cron input socket bind failure', find_program('tests/cron-input-socket-bind-failure.sh'), env: test_env)
test('cron input socket connect failure', find_program('tests/cron-input-socket-connect-failure.sh'), env: test_env)
test('cron input socket receive failure', find_program('tests/cron-input-socket-recv-failure.sh'), env: test_env)
test('cron input socket shutdown failure', find_program('tests/cron-input-socket-shutdown-failure.sh'), env: test_env)

test('input socket rcvbuf large enough', find_program('tests/input-socket-rcvbuf-large-enough.sh'), env: test_env)
test('input socket rcvbuf minimum size', find_program('tests/input-socket-rcvbuf-minimum-size.sh'), env: test_env)
test('input socket rcvbuf too small', find_program('tests/input-socket-rcvbuf-too-small.sh'), env: test_env)

test('input socket message from unknown source', find_program('tests/input-socket-extra-source.sh'), env: test_env)
test('input socket receive buffer too small', find_program('tests/input-socket-recv-buffer-too-small.sh'), env: test_env)

test('report arguments and file descriptors', find_program('tests/report.sh'), env: test_env)
test('report arguments and file descriptors (no stdin)', find_program('tests/report-no-stdin.sh'), env: test_env)
test('report arguments and file descriptors (extra fd)', find_program('tests/report-extra-fd.sh'), env: test_env)
test('cron report arguments and file descriptors', find_program('tests/cron-report.sh'), env: test_env)
test('cron report arguments and file descriptors (no stdin)', find_program('tests/cron-report-no-stdin.sh'), env: test_env)
test('cron report arguments and file descriptors (extra fd)', find_program('tests/cron-report-extra-fd.sh'), env: test_env)

test('dtee killed by signal 1', find_program('tests/command-kill-ppid-sighup.sh'), env: test_env)
test('dtee killed by signal 2', find_program('tests/command-kill-ppid-sigint.sh'), env: test_env)
test('dtee killed by signal 2 (ignore interrupts)', find_program('tests/command-kill-ppid-sigint-ignore-sigint.sh'), env: test_env)
test('dtee killed by signal 3', find_program('tests/command-kill-ppid-sigquit.sh'), env: test_env)
test('dtee killed by signal 9', find_program('tests/command-kill-ppid-sigkill.sh'), env: test_env)
test('dtee killed by signal 15', find_program('tests/command-kill-ppid-sigterm.sh'), env: test_env)
test('cron dtee killed by signal 1', find_program('tests/cron-command-kill-ppid-sighup.sh'), env: test_env)
test('cron dtee killed by signal 1 (self kill failure)', find_program('tests/cron-command-kill-ppid-sighup-self-kill-failure.sh'), env: test_env)
test('cron dtee killed by signal 2', find_program('tests/cron-command-kill-ppid-sigint.sh'), env: test_env)
test('cron dtee killed by signal 2 (ignore interrupts)', find_program('tests/cron-command-kill-ppid-sigint-ignore-sigint.sh'), env: test_env)
test('cron dtee killed by signal 2 (self kill failure)', find_program('tests/cron-command-kill-ppid-sigint-self-kill-failure.sh'), env: test_env)
test('cron dtee killed by signal 3', find_program('tests/cron-command-kill-ppid-sigquit.sh'), env: test_env)
test('cron dtee killed by signal 9', find_program('tests/cron-command-kill-ppid-sigkill.sh'), env: test_env)
test('cron dtee killed by signal 15', find_program('tests/cron-command-kill-ppid-sigterm.sh'), env: test_env)
test('cron dtee killed by signal 15 (self kill failure)', find_program('tests/cron-command-kill-ppid-sigterm-self-kill-failure.sh'), env: test_env)

test('fork failure', find_program('tests/fork-failure.sh'), env: test_env)
test('dup2 failure', find_program('tests/dup2-failure.sh'), env: test_env)
test('socket close failure (child)', find_program('tests/socket-close-failure-child.sh'), env: test_env)
test('socket close failure (parent)', find_program('tests/socket-close-failure-parent.sh'), env: test_env)
test('cron fork failure', find_program('tests/cron-fork-failure.sh'), env: test_env)
test('cron dup2 failure', find_program('tests/cron-dup2-failure.sh'), env: test_env)
test('cron socket close failure (child)', find_program('tests/cron-socket-close-failure-child.sh'), env: test_env)
test('cron socket close failure (parent)', find_program('tests/cron-socket-close-failure-parent.sh'), env: test_env)

test('waitpid failure (no children)', find_program('tests/waitpid-failure-no-children.sh'), env: test_env)
test('waitpid failure (no change)', find_program('tests/waitpid-failure-no-change.sh'), env: test_env)
test('cron waitpid failure (no children)', find_program('tests/cron-waitpid-failure-no-children.sh'), env: test_env)
test('cron waitpid failure (no change)', find_program('tests/cron-waitpid-failure-no-change.sh'), env: test_env)

test('security recreate the input socket after the parent process exits', find_program('tests/security-recreate-input-socket.sh'), env: test_env)
test('security recreate the output socket after the child process exits', find_program('tests/security-recreate-output-socket.sh'), env: test_env)

test('pipe write failure (stdout)', find_program('tests/pipe-write-failure-stdout.sh'), env: test_env)
test('pipe write failure (stderr)', find_program('tests/pipe-write-failure-stderr.sh'), env: test_env)
test('pipe write failure (file)', find_program('tests/pipe-write-failure-file.sh'), env: test_env)
test('cron pipe write failure (stdout)', find_program('tests/cron-pipe-write-failure-stdout.sh'), env: test_env)
test('cron pipe write failure (stderr)', find_program('tests/cron-pipe-write-failure-stderr.sh'), env: test_env)
test('cron pipe write failure (stderr but no output)', find_program('tests/cron-pipe-write-failure-stderr2.sh'), env: test_env)
test('cron pipe write failure (file)', find_program('tests/cron-pipe-write-failure-file.sh'), env: test_env)

test('command writes to stdout/stderr after dtee exits', find_program('tests/command-write-after-ppid-exit.sh'), env: test_env)
