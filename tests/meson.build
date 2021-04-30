test_env = [
	'LC_ALL=C',
	'LANG=C',
	'LANGUAGE=',
]

if host_machine.system() == 'linux' or host_machine.system() == 'gnu'
	test_c_args = c_args + ['-D_DEFAULT_SOURCE']
	dlfcn_args = test_c_args + ['-D_GNU_SOURCE', '-Wno-pedantic']
	dlfcn_deps = [c.find_library('dl')]
elif host_machine.system() == 'darwin'
	test_c_args = c_args
	dlfcn_args = test_c_args
	dlfcn_deps = []
else # freebsd, openbsd, netbsd, dragonfly, cygwin
	test_c_args = c_args
	dlfcn_args = test_c_args + ['-Wno-pedantic']
	dlfcn_deps = []
endif

if host_machine.system() == 'freebsd' or host_machine.system() == 'openbsd' or host_machine.system() == 'netbsd' or host_machine.system() == 'dragonfly'
	proc_deps = [c.find_library('kvm')]
else # linux, gnu, darwin
	proc_deps = []
endif

if host_machine.system() == 'linux' or host_machine.system() == 'gnu'
	has_generic_sendfile = true
else # freebsd, openbsd, netbsd, dragonfly, darwin, cygwin
	has_generic_sendfile = false
endif

testlib_allow_n_times = shared_library('test-allow-n-times', ['allow-n-times.c'], c_args: test_c_args)
testlib_is_dtee = shared_library('test-is-dtee', ['is-dtee.c', 'is-dtee-' + host_machine.system() + '.c'], c_args: dlfcn_args, dependencies: dlfcn_deps + proc_deps)
testlib_is_fd_unix_socket = shared_library('test-fd-unix-socket', ['is-fd-unix-socket.c'], c_args: test_c_args)

shared_library('test-bindtextdomain-override', ['bindtextdomain-override.c'] , c_args: dlfcn_args + ['-DGETTEXT_LOCALEDIR="' + meson.current_build_dir() + '/dtee@test/locale"'], dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-dup2-failure', ['dup2-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-execvp-fd-check', ['execvp-fd-check.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-getpid', ['fake-getpid.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-getuid', ['fake-getuid.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-strerror', ['fake-strerror.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-strsignal', ['fake-strsignal.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fork-failure', ['fork-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fork-sigchld-check', ['fork-sigchld-check.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-file-write-failure', ['file-write-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-file-write-failure-recovery', ['file-write-failure-recovery.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-kill-failure', ['kill-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-lseek-failure', ['lseek-failure.c'] , c_args: dlfcn_args + ['-D_LARGEFILE64_SOURCE'], dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkdtemp-consistent', ['mkdtemp-consistent.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkdtemp-failure', ['mkdtemp-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-consistent', ['mkostemp-consistent.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-failure', ['mkostemp-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-readonly', ['mkostemp-readonly.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-writeonly', ['mkostemp-writeonly.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-security-recreate-output-socket', ['security-recreate-output-socket.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-sigaction-no-restart', ['sigaction-no-restart.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-sigaction-query-failure', ['sigaction-query-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-sigaction-set-restart-failure', ['sigaction-set-restart-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-socket-add-extra-source', ['socket-add-extra-source.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-bind-failure', ['socket-bind-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-close-failure', ['socket-close-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-connect-failure', ['socket-connect-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-getsockopt-fake-rcvbuf', ['socket-getsockopt-fake-rcvbuf.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-recv-buffer-too-small', ['socket-recv-buffer-too-small.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-recv-failure', ['socket-recv-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-setsockopt-monitor', ['socket-setsockopt-monitor.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-shutdown-failure', ['socket-shutdown-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-unix-failure', ['socket-unix-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee])
shared_library('test-waitpid-failure', ['waitpid-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-write-truncate-cron', ['write-truncate-cron.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-write-truncate-stdout', ['write-truncate-stdout.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])

executable('test-closed-pipe-reader', ['closed-pipe-reader.c'], c_args: test_c_args)
executable('test-closed-pipe-writer', ['closed-pipe-writer.c'], c_args: test_c_args)
executable('test-fifo-write-interrupt', ['fifo-write-interrupt.c'] , c_args: test_c_args)
executable('test-lorem-ipsum-late-exit', ['lorem-ipsum-late-exit.c'], c_args: test_c_args)
executable('test-lorem-ipsum-wait-exit', ['lorem-ipsum-wait-exit.c'], c_args: test_c_args)
executable('test-kill-pid', ['kill-pid.c'], c_args: test_c_args)
executable('test-kill-ppid', ['kill-ppid.c'], c_args: test_c_args, link_with: [testlib_is_fd_unix_socket])
executable('test-report', ['report.c'], c_args: test_c_args, link_with: [testlib_is_fd_unix_socket])
executable('test-security-recreate-input-socket', ['security-recreate-input-socket.c'], c_args: test_c_args)
if has_generic_sendfile
	executable('test-sendfile', ['sendfile.c'], c_args: test_c_args)
endif
generate_signal_txt = executable('test-signal-txt', ['signal-txt.c'], c_args: test_c_args)
executable('test-stream-read', ['stream-read.c'], c_args: test_c_args)
executable('test-stream-write', ['stream-write.c'], c_args: test_c_args)
generate_sysexits_txt = executable('test-sysexits-txt', ['sysexits-txt.c'], c_args: test_c_args)
executable('test-waitpid', ['waitpid.c'], c_args: test_c_args)
executable('test-write-0-bytes', ['write-0-bytes.c'], c_args: test_c_args)
executable('test-writes-of-n-bytes', ['writes-of-n-bytes.c'], c_args: test_c_args)

signal_txt = custom_target('signal.txt',
	command: [generate_signal_txt],
	output: 'signal.txt')
sysexits_txt = custom_target('sysexits.txt',
	command: [generate_sysexits_txt],
	output: 'sysexits.txt')
version_txt = vcs_tag(input: 'version.txt.in', output: 'version.txt')
all_test_txt = [signal_txt, sysexits_txt, version_txt]

bash = find_program('bash')

xfail = get_option('test_expect_fail')
xfail_general_writes = ['netbsd'].contains(host_machine.system())
xfail_stream = xfail_general_writes or ['freebsd', 'openbsd', 'dragonfly', 'darwin'].contains(host_machine.system())
xfail_stream_1mb = xfail_stream
xfail_stream_32mb = xfail_stream or ['cygwin'].contains(host_machine.system())
xfail_stream_256mb = xfail_stream or ['cygwin'].contains(host_machine.system())
xfail_writes_of_1_byte = xfail_general_writes or xfail_stream
xfail_small_writes = xfail_general_writes
xfail_large_writes = xfail_general_writes

test('dtee (no arguments)', bash, args: [files('noargs-dtee.sh')], env: test_env, depends: all_test_txt)
test('cronty (no arguments)', bash, args: [files('noargs-cronty.sh')], env: test_env, depends: all_test_txt)
test('dtee --help', bash, args: [files('help-dtee.sh')], env: test_env, depends: all_test_txt)
test('cronty --help', bash, args: [files('help-cronty.sh')], env: test_env, depends: all_test_txt)
test('dtee --version', bash, args: [files('version.sh')], env: test_env, depends: all_test_txt)

test('passthrough stdout', bash, args: [files('passthrough-stdout.sh')], env: test_env, depends: all_test_txt)
test('passthrough stderr', bash, args: [files('passthrough-stderr.sh')], env: test_env, depends: all_test_txt)
test('passthrough stdout and stderr 1', bash, args: [files('passthrough-stdout-stderr1.sh')], env: test_env, depends: all_test_txt)
test('passthrough stdout and stderr 2', bash, args: [files('passthrough-stdout-stderr2.sh')], env: test_env, depends: all_test_txt)
test('interleaved characters', bash, args: [files('interleaved-characters.sh')], env: test_env, depends: all_test_txt)

test('file stdout overwrite (new file)', bash, args: [files('file-stdout-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file stdout overwrite (existing)', bash, args: [files('file-stdout-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file stdout append (new file)', bash, args: [files('file-stdout-append-new.sh')], env: test_env, depends: all_test_txt)
test('file stdout append (existing)', bash, args: [files('file-stdout-append-existing.sh')], env: test_env, depends: all_test_txt)

test('file stderr overwrite (new file)', bash, args: [files('file-stderr-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file stderr overwrite (existing)', bash, args: [files('file-stderr-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file stderr append (new file)', bash, args: [files('file-stderr-append-new.sh')], env: test_env, depends: all_test_txt)
test('file stderr append (existing)', bash, args: [files('file-stderr-append-existing.sh')], env: test_env, depends: all_test_txt)

test('file combined overwrite (new file)', bash, args: [files('file-combined-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file combined overwrite (existing)', bash, args: [files('file-combined-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file combined append (new file)', bash, args: [files('file-combined-append-new.sh')], env: test_env, depends: all_test_txt)
test('file combined append (existing)', bash, args: [files('file-combined-append-existing.sh')], env: test_env, depends: all_test_txt)

test('write 0 bytes (stdout)', bash, args: [files('write-0-bytes-stdout.sh')], env: test_env, depends: all_test_txt)
test('write 0 bytes (stderr)', bash, args: [files('write-0-bytes-stderr.sh')], env: test_env, depends: all_test_txt)
test('writes of 1 byte (stdout)', bash, args: [files('writes-of-1-byte-stdout.sh')], env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('writes of 1 byte (stderr)', bash, args: [files('writes-of-1-byte-stderr.sh')], env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('writes of min(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, args: [files('writes-of-min-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of min(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, args: [files('writes-of-min-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of PIPE_BUF bytes (stdout)', bash, args: [files('writes-of-PIPE_BUF-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of PIPE_BUF bytes (stderr)', bash, args: [files('writes-of-PIPE_BUF-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of BUFSIZ bytes (stdout)', bash, args: [files('writes-of-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of BUFSIZ bytes (stderr)', bash, args: [files('writes-of-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of max(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, args: [files('writes-of-max-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of max(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, args: [files('writes-of-max-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron write 0 bytes (stdout)', bash, args: [files('cron-write-0-bytes-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write 0 bytes (stderr)', bash, args: [files('cron-write-0-bytes-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron writes of 1 byte (stdout)', bash, args: [files('cron-writes-of-1-byte-stdout.sh')], env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('cron writes of 1 byte (stderr)', bash, args: [files('cron-writes-of-1-byte-stderr.sh')], env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('cron writes of min(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, args: [files('cron-writes-of-min-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of min(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, args: [files('cron-writes-of-min-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of PIPE_BUF bytes (stdout)', bash, args: [files('cron-writes-of-PIPE_BUF-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of PIPE_BUF bytes (stderr)', bash, args: [files('cron-writes-of-PIPE_BUF-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of BUFSIZ bytes (stdout)', bash, args: [files('cron-writes-of-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of BUFSIZ bytes (stderr)', bash, args: [files('cron-writes-of-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of max(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, args: [files('cron-writes-of-max-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of max(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, args: [files('cron-writes-of-max-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)

if has_generic_sendfile
	test('sendfile (stdout)', bash, args: [files('sendfile-stdout.sh')], env: test_env, depends: all_test_txt)
endif
test('stream 1MB (stdout)', bash, args: [files('stream-1MB.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_1mb)
test('stream 32MB (stdout)', bash, args: [files('stream-32MB.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_32mb)
test('stream 256MB (stdout)', bash, args: [files('stream-256MB.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_256mb)

test('debug options (none)', bash, args: [files('options-none.sh')], env: test_env, depends: all_test_txt)
test('debug options (cron)', bash, args: [files('options-cron.sh')], env: test_env, depends: all_test_txt)
test('debug options (invalid short options)', bash, args: [files('options-invalid-short.sh')], env: test_env, depends: all_test_txt)
test('debug options (invalid short options with arguments)', bash, args: [files('options-invalid-short-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (invalid long options)', bash, args: [files('options-invalid-long.sh')], env: test_env, depends: all_test_txt)
test('debug options (invalid long options with arguments)', bash, args: [files('options-invalid-long-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (1 file)', bash, args: [files('options-1-file.sh')], env: test_env, depends: all_test_txt)
test('debug options (2 files)', bash, args: [files('options-2-files.sh')], env: test_env, depends: all_test_txt)
test('debug options (3 files)', bash, args: [files('options-3-files.sh')], env: test_env, depends: all_test_txt)
test('debug options (command with no arguments)', bash, args: [files('options-cmd-noargs.sh')], env: test_env, depends: all_test_txt)
test('debug options (command with 1 argument)', bash, args: [files('options-cmd-1-arg.sh')], env: test_env, depends: all_test_txt)
test('debug options (command with 2 arguments)', bash, args: [files('options-cmd-2-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (command with 3 arguments)', bash, args: [files('options-cmd-3-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (command stops following options)', bash, args: [files('options-cmd-opts.sh')], env: test_env, depends: all_test_txt)
test('debug options (obscure boost syntax)', bash, args: [files('options-cmd-obscure-boost-syntax.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command with no arguments)', bash, args: [files('options-cmd-empty-noargs.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command with 1 argument)', bash, args: [files('options-cmd-empty-1-arg.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command with 2 arguments)', bash, args: [files('options-cmd-empty-2-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command with 3 arguments)', bash, args: [files('options-cmd-empty-3-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty filename and empty command)', bash, args: [files('options-mixed-empty1.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty filename and empty command with empty argument)', bash, args: [files('options-mixed-empty2.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command stops following options)', bash, args: [files('options-mixed-empty3.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command stops following options even when empty)', bash, args: [files('options-mixed-empty4.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command stops following options)', bash, args: [files('options-mixed-empty5.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command stops following options)', bash, args: [files('options-mixed-empty6.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options)', bash, args: [files('options-mixed-split1.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options even with immediate opts)', bash, args: [files('options-mixed-split2.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options at the start)', bash, args: [files('options-cmd-split1.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options at the start even with empty command)', bash, args: [files('options-cmd-split2.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options at the start even with empty command and empty argument)', bash, args: [files('options-cmd-split3.sh')], env: test_env, depends: all_test_txt)

test('command not found', bash, args: [files('command-not-found.sh')], env: test_env, depends: all_test_txt)
test('command not executable', bash, args: [files('command-not-executable.sh')], env: test_env, depends: all_test_txt)
test('command exit code 42', bash, args: [files('command-exit-code.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 1', bash, args: [files('command-sighup.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 2', bash, args: [files('command-sigint.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 2 (ignore interrupts)', bash, args: [files('command-sigint-ignore-sigint.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 2 (self kill failure)', bash, args: [files('command-sigint-self-kill-failure.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 3', bash, args: [files('command-sigquit.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 9', bash, args: [files('command-sigkill.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 15', bash, args: [files('command-sigterm.sh')], env: test_env, depends: all_test_txt)

test('cron command not found', bash, args: [files('cron-command-not-found.sh')], env: test_env, depends: all_test_txt)
test('cron command not executable', bash, args: [files('cron-command-not-executable.sh')], env: test_env, depends: all_test_txt)
test('cron command exit code 0 (stderr)', bash, args: [files('cron-command-exit-code-0-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron command exit code 42 (stderr)', bash, args: [files('cron-command-exit-code-42-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron command exit code 0 (no stderr)', bash, args: [files('cron-command-exit-code-0-no-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron command exit code 42 (no stderr)', bash, args: [files('cron-command-exit-code-42-no-stderr.sh')], env: test_env, depends: all_test_txt)
test('cronty command exit code 0 (stderr)', bash, args: [files('cronty-command-exit-code-0-stderr.sh')], env: test_env, depends: all_test_txt)
test('cronty command exit code 42 (stderr)', bash, args: [files('cronty-command-exit-code-42-stderr.sh')], env: test_env, depends: all_test_txt)
test('cronty command exit code 0 (no stderr)', bash, args: [files('cronty-command-exit-code-0-no-stderr.sh')], env: test_env, depends: all_test_txt)
test('cronty command exit code 42 (no stderr)', bash, args: [files('cronty-command-exit-code-42-no-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 1', bash, args: [files('cron-command-sighup.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 2', bash, args: [files('cron-command-sigint.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 2 (ignore interrupts)', bash, args: [files('cron-command-sigint-ignore-sigint.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 2 (self kill failure)', bash, args: [files('cron-command-sigint-self-kill-failure.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 3 (coredump)', bash, args: [files('cron-command-sigquit-coredump.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 3 (no coredump)', bash, args: [files('cron-command-sigquit-no-coredump.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 9', bash, args: [files('cron-command-sigkill.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 15', bash, args: [files('cron-command-sigterm.sh')], env: test_env, depends: all_test_txt)

test('output file not openable', bash, args: [files('file-open-failure.sh')], env: test_env, depends: all_test_txt)
test('output file not writeable (stdout)', bash, args: [files('file-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('output file not writeable (stderr)', bash, args: [files('file-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('output file not writeable (combined)', bash, args: [files('file-write-failure-combined.sh')], env: test_env, depends: all_test_txt)

test('cron normal output file not openable', bash, args: [files('cron-file-open-failure-normal.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (stdout)', bash, args: [files('cron-file-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (stderr)', bash, args: [files('cron-file-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (combined)', bash, args: [files('cron-file-write-failure-combined.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not openable', bash, args: [files('cron-file-open-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not writeable', bash, args: [files('cron-file-write-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not readable', bash, args: [files('cron-file-read-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not seekable', bash, args: [files('cron-file-seek-failure-cron.sh')], env: test_env, depends: all_test_txt)

test('output file writes fail and recover', bash, args: [files('file-write-failure-recovery.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file writes fail and recover (exit code 0)', bash, args: [files('cron-file-write-failure-recovery-exit-0.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file writes fail and recover (exit code 42)', bash, args: [files('cron-file-write-failure-recovery-exit-42.sh')], env: test_env, depends: all_test_txt)

test('input temporary directory create failure', bash, args: [files('input-temp-dir-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket create failure (input)', bash, args: [files('input-socket-create-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket create failure (stdout)', bash, args: [files('input-socket-create-failure-2.sh')], env: test_env, depends: all_test_txt)
test('input socket create failure (stderr)', bash, args: [files('input-socket-create-failure-3.sh')], env: test_env, depends: all_test_txt)
test('input socket create and output file open failure', bash, args: [files('input-and-output-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket bind failure (input)', bash, args: [files('input-socket-bind-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket bind failure (stdout)', bash, args: [files('input-socket-bind-failure-2.sh')], env: test_env, depends: all_test_txt)
test('input socket bind failure (stderr)', bash, args: [files('input-socket-bind-failure-3.sh')], env: test_env, depends: all_test_txt)
test('input socket connect failure (stdout)', bash, args: [files('input-socket-connect-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket connect failure (stderr)', bash, args: [files('input-socket-connect-failure-2.sh')], env: test_env, depends: all_test_txt)
test('input socket receive failure', bash, args: [files('input-socket-recv-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket shutdown failure (stdout)', bash, args: [files('input-socket-shutdown-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket shutdown failure (stderr)', bash, args: [files('input-socket-shutdown-failure-2.sh')], env: test_env, depends: all_test_txt)

test('cron input temporary directory create failure', bash, args: [files('cron-input-temp-dir-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket create failure', bash, args: [files('cron-input-socket-create-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket create and output file open failure', bash, args: [files('cron-input-and-output-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket bind failure', bash, args: [files('cron-input-socket-bind-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket connect failure', bash, args: [files('cron-input-socket-connect-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket receive failure', bash, args: [files('cron-input-socket-recv-failure.sh')], env: test_env, depends: all_test_txt)

test('input socket rcvbuf large enough', bash, args: [files('input-socket-rcvbuf-large-enough.sh')], env: test_env, depends: all_test_txt)
test('input socket rcvbuf minimum size', bash, args: [files('input-socket-rcvbuf-minimum-size.sh')], env: test_env, depends: all_test_txt)
test('input socket rcvbuf too small', bash, args: [files('input-socket-rcvbuf-too-small.sh')], env: test_env, depends: all_test_txt)

test('input socket message from unknown source', bash, args: [files('input-socket-extra-source.sh')], env: test_env, depends: all_test_txt)
test('input socket receive buffer too small', bash, args: [files('input-socket-recv-buffer-too-small.sh')], env: test_env, depends: all_test_txt)

test('report arguments and file descriptors', bash, args: [files('report.sh')], env: test_env, depends: all_test_txt)
test('report arguments and file descriptors (no stdin)', bash, args: [files('report-no-stdin.sh')], env: test_env, depends: all_test_txt)
test('report arguments and file descriptors (extra fd)', bash, args: [files('report-extra-fd.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors', bash, args: [files('cron-report.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors (no stdin)', bash, args: [files('cron-report-no-stdin.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors (extra fd)', bash, args: [files('cron-report-extra-fd.sh')], env: test_env, depends: all_test_txt)

test('dtee killed by signal 1', bash, args: [files('command-kill-ppid-sighup.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 2', bash, args: [files('command-kill-ppid-sigint.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 2 (ignore interrupts)', bash, args: [files('command-kill-ppid-sigint-ignore-sigint.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 3', bash, args: [files('command-kill-ppid-sigquit.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 9', bash, args: [files('command-kill-ppid-sigkill.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 15', bash, args: [files('command-kill-ppid-sigterm.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 1', bash, args: [files('cron-command-kill-ppid-sighup.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 1 (self kill failure)', bash, args: [files('cron-command-kill-ppid-sighup-self-kill-failure.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 2', bash, args: [files('cron-command-kill-ppid-sigint.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 2 (ignore interrupts)', bash, args: [files('cron-command-kill-ppid-sigint-ignore-sigint.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 2 (self kill failure)', bash, args: [files('cron-command-kill-ppid-sigint-self-kill-failure.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 3', bash, args: [files('cron-command-kill-ppid-sigquit.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 9', bash, args: [files('cron-command-kill-ppid-sigkill.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 15', bash, args: [files('cron-command-kill-ppid-sigterm.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 15 (self kill failure)', bash, args: [files('cron-command-kill-ppid-sigterm-self-kill-failure.sh')], env: test_env, depends: all_test_txt)

test('fork failure', bash, args: [files('fork-failure.sh')], env: test_env, depends: all_test_txt)
test('dup2 failure', bash, args: [files('dup2-failure.sh')], env: test_env, depends: all_test_txt)
test('socket close failure (child)', bash, args: [files('socket-close-failure-child.sh')], env: test_env, depends: all_test_txt)
test('socket close failure (parent)', bash, args: [files('socket-close-failure-parent.sh')], env: test_env, depends: all_test_txt)
test('cron fork failure', bash, args: [files('cron-fork-failure.sh')], env: test_env, depends: all_test_txt)
test('cron dup2 failure', bash, args: [files('cron-dup2-failure.sh')], env: test_env, depends: all_test_txt)
test('cron socket close failure (child)', bash, args: [files('cron-socket-close-failure-child.sh')], env: test_env, depends: all_test_txt)
test('cron socket close failure (parent)', bash, args: [files('cron-socket-close-failure-parent.sh')], env: test_env, depends: all_test_txt)

test('waitpid failure (no children)', bash, args: [files('waitpid-failure-no-children.sh')], env: test_env, depends: all_test_txt)
test('waitpid failure (no change)', bash, args: [files('waitpid-failure-no-change.sh')], env: test_env, depends: all_test_txt)
test('cron waitpid failure (no children)', bash, args: [files('cron-waitpid-failure-no-children.sh')], env: test_env, depends: all_test_txt)
test('cron waitpid failure (no change)', bash, args: [files('cron-waitpid-failure-no-change.sh')], env: test_env, depends: all_test_txt)

if host_machine.system() != 'cygwin'
	# Assume Cygwin is insecure because the underlying Unix socket implementation is UDP
	test('security recreate the input socket after the parent process exits', bash, args: [files('security-recreate-input-socket.sh')], env: test_env, depends: all_test_txt)
	test('security recreate the output socket after the child process exits', bash, args: [files('security-recreate-output-socket.sh')], env: test_env, depends: all_test_txt)
endif

test('pipe write failure (stdout)', bash, args: [files('pipe-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('pipe write failure (stderr)', bash, args: [files('pipe-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('pipe write failure (file)', bash, args: [files('pipe-write-failure-file.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stdout)', bash, args: [files('cron-pipe-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stderr)', bash, args: [files('cron-pipe-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stderr but no output)', bash, args: [files('cron-pipe-write-failure-stderr2.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (file)', bash, args: [files('cron-pipe-write-failure-file.sh')], env: test_env, depends: all_test_txt)

test('command writes to stdout/stderr after dtee exits', bash, args: [files('command-write-after-ppid-exit.sh')], env: test_env, depends: all_test_txt)

if host_machine.system() != 'cygwin'
	# These don't work on Cygwin because the FIFO can't have multiple readers/writers
	test('interrupted by signal during write', bash, args: [files('interrupt-write.sh')], env: test_env, depends: all_test_txt)
	test('interrupted by signal during write (no SA_RESTART)', bash, args: [files('interrupt-write-no-restart.sh')], env: test_env, depends: all_test_txt)
	test('cron interrupt by signal during write', bash, args: [files('cron-interrupt-write.sh')], env: test_env, depends: all_test_txt)
	test('cron interrupt by signal during write (no SA_RESTART)', bash, args: [files('cron-interrupt-write-no-restart.sh')], env: test_env, depends: all_test_txt)
endif

test('write truncated (stdout)', bash, args: [files('write-truncate-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write truncated (stdout)', bash, args: [files('cron-write-truncate-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write truncated (cron)', bash, args: [files('cron-write-truncate-cron.sh')], env: test_env, depends: all_test_txt)

# Test errors in Boost.Asio workaround at SignalHandler::add_non_interrupting_signal()
test('sigaction query failure', bash, args: [files('sigaction-query-failure.sh')], env: test_env, depends: all_test_txt)
test('sigaction set SA_RESTART failure', bash, args: [files('sigaction-set-restart-failure.sh')], env: test_env, depends: all_test_txt)

foreach language, mo_file : all_test_i18n
	test('i18n ' + language + ' dtee --help', bash, args: [files('i18n-help-dtee.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
	test('i18n ' + language + ' cronty --help', bash, args: [files('i18n-help-cronty.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
	test('i18n ' + language + ' dtee --version', bash, args: [files('i18n-version.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
endforeach
