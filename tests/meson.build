test_env = [
	'LC_ALL=C',
	'LANG=C',
	'LANGUAGE=',
]

bash = find_program('bash')
xfail = get_option('test_expect_fail')

subdir('util')
subdir('basic')
subdir('write')
subdir('options')
subdir('command')

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

# Test errors in Boost.Asio workaround at SignalHandler::add_non_interrupting_signal()
test('sigaction query failure', bash, args: [files('sigaction-query-failure.sh')], env: test_env, depends: all_test_txt)
test('sigaction set SA_RESTART failure', bash, args: [files('sigaction-set-restart-failure.sh')], env: test_env, depends: all_test_txt)

subdir('i18n')
