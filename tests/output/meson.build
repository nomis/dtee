test('file not openable', default_signal_handlers, suite: 'output', args: [bash, files('file-open-failure.sh')], env: test_env, depends: all_test_txt)
test('file not writeable (stdout)', default_signal_handlers, suite: 'output', args: [bash, files('file-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('file not writeable (stderr)', default_signal_handlers, suite: 'output', args: [bash, files('file-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('file not writeable (combined)', default_signal_handlers, suite: 'output', args: [bash, files('file-write-failure-combined.sh')], env: test_env, depends: all_test_txt)

test('cron normal output file not openable', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-open-failure-normal.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (stdout)', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (stderr)', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (combined)', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-write-failure-combined.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not openable', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-open-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not writeable', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-write-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not readable', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-read-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not seekable', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-seek-failure-cron.sh')], env: test_env, depends: all_test_txt)

test('file writes fail and recover', default_signal_handlers, suite: 'output', args: [bash, files('file-write-failure-recovery.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file writes fail and recover (exit code 0)', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-write-failure-recovery-exit-0.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file writes fail and recover (exit code 42)', default_signal_handlers, suite: 'output', args: [bash, files('cron-file-write-failure-recovery-exit-42.sh')], env: test_env, depends: all_test_txt)

# It's difficult to test with real non-blocking output streams (that continue to
# be non-blocking) because the error message has to be output on the same file
# descriptor.
if has_builtin_apply
	test('stream non-blocking (stdout)', default_signal_handlers, suite: 'output', args: [bash, files('stream-non-blocking-stdout.sh')], env: test_env, depends: all_test_txt)
	test('stream non-blocking (stderr)', default_signal_handlers, suite: 'output', args: [bash, files('stream-non-blocking-stderr.sh')], env: test_env, depends: all_test_txt)
	test('stream fcntl F_GETFL failure (stdout)', default_signal_handlers, suite: 'output', args: [bash, files('stream-fcntl-get-stdout.sh')], env: test_env, depends: all_test_txt)
	test('stream fcntl F_GETFL failure (stderr)', default_signal_handlers, suite: 'output', args: [bash, files('stream-fcntl-get-stderr.sh')], env: test_env, depends: all_test_txt)
	test('stream fcntl F_SETFL failure (stdout)', default_signal_handlers, suite: 'output', args: [bash, files('stream-fcntl-set-stdout.sh')], env: test_env, depends: all_test_txt)
	test('stream fcntl F_SETFL failure (stderr)', default_signal_handlers, suite: 'output', args: [bash, files('stream-fcntl-set-stderr.sh')], env: test_env, depends: all_test_txt)
endif
