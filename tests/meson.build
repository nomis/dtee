test_env = [
	'LC_ALL=C',
	'LANG=C',
	'LANGUAGE=',
]

subdir('util')

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

subdir('basic')

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

subdir('options')

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

subdir('security')

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

subdir('i18n')
