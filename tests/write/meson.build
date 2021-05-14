xfail_general_writes = ['netbsd'].contains(host_machine.system())
xfail_stream = xfail_general_writes or ['freebsd', 'openbsd', 'dragonfly', 'darwin'].contains(host_machine.system())
xfail_stream_1mb = xfail_stream
xfail_stream_32mb = xfail_stream or ['cygwin'].contains(host_machine.system())
xfail_stream_256mb = xfail_stream or ['cygwin'].contains(host_machine.system())
xfail_writes_of_1_byte = xfail_general_writes or xfail_stream
xfail_small_writes = xfail_general_writes or ['dragonfly'].contains(host_machine.system())
xfail_large_writes = xfail_general_writes

test('write 0 bytes (stdout)', bash, suite: 'write', args: [files('write-0-bytes-stdout.sh')], env: test_env, depends: all_test_txt)
test('write 0 bytes (stderr)', bash, suite: 'write', args: [files('write-0-bytes-stderr.sh')], env: test_env, depends: all_test_txt)
test('writes of 1 byte (stdout)', bash, suite: 'write', args: [files('writes-of-1-byte-stdout.sh')], priority: 2000, env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('writes of 1 byte (stderr)', bash, suite: 'write', args: [files('writes-of-1-byte-stderr.sh')], priority: 2000, env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('writes of min(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, suite: 'write', args: [files('writes-of-min-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of min(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, suite: 'write', args: [files('writes-of-min-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of PIPE_BUF bytes (stdout)', bash, suite: 'write', args: [files('writes-of-PIPE_BUF-bytes-stdout.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of PIPE_BUF bytes (stderr)', bash, suite: 'write', args: [files('writes-of-PIPE_BUF-bytes-stderr.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of BUFSIZ bytes (stdout)', bash, suite: 'write', args: [files('writes-of-BUFSIZ-bytes-stdout.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of BUFSIZ bytes (stderr)', bash, suite: 'write', args: [files('writes-of-BUFSIZ-bytes-stderr.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of max(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, suite: 'write', args: [files('writes-of-max-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of max(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, suite: 'write', args: [files('writes-of-max-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron write 0 bytes (stdout)', bash, suite: 'write', args: [files('cron-write-0-bytes-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write 0 bytes (stderr)', bash, suite: 'write', args: [files('cron-write-0-bytes-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron writes of 1 byte (stdout)', bash, suite: 'write', args: [files('cron-writes-of-1-byte-stdout.sh')], priority: 2000, env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('cron writes of 1 byte (stderr)', bash, suite: 'write', args: [files('cron-writes-of-1-byte-stderr.sh')], priority: 2000, env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('cron writes of min(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, suite: 'write', args: [files('cron-writes-of-min-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of min(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, suite: 'write', args: [files('cron-writes-of-min-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of PIPE_BUF bytes (stdout)', bash, suite: 'write', args: [files('cron-writes-of-PIPE_BUF-bytes-stdout.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of PIPE_BUF bytes (stderr)', bash, suite: 'write', args: [files('cron-writes-of-PIPE_BUF-bytes-stderr.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of BUFSIZ bytes (stdout)', bash, suite: 'write', args: [files('cron-writes-of-BUFSIZ-bytes-stdout.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of BUFSIZ bytes (stderr)', bash, suite: 'write', args: [files('cron-writes-of-BUFSIZ-bytes-stderr.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of max(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, suite: 'write', args: [files('cron-writes-of-max-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of max(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, suite: 'write', args: [files('cron-writes-of-max-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)

if has_generic_sendfile
	test('sendfile (stdout)', bash, suite: 'write', args: [files('sendfile-stdout.sh')], priority: 500, env: test_env, depends: all_test_txt)
endif
test('stream 1MB (stdout)', bash, suite: 'write', args: [files('stream-1MB.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_1mb)
test('stream 32MB (stdout)', bash, suite: 'write', args: [files('stream-32MB.sh')], priority: 1000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_32mb)
test('stream 256MB (stdout)', bash, suite: 'write', args: [files('stream-256MB.sh')], priority: 2000, env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_256mb)

test('write truncated (stdout)', bash, suite: 'write', args: [files('write-truncate-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write truncated (stdout)', bash, suite: 'write', args: [files('cron-write-truncate-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write truncated (cron)', bash, suite: 'write', args: [files('cron-write-truncate-cron.sh')], env: test_env, depends: all_test_txt)

test('pipe write failure (stdout)', bash, suite: 'write', args: [files('pipe-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('pipe write failure (stderr)', bash, suite: 'write', args: [files('pipe-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('pipe write failure (file)', bash, suite: 'write', args: [files('pipe-write-failure-file.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stdout)', bash, suite: 'write', args: [files('cron-pipe-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stderr)', bash, suite: 'write', args: [files('cron-pipe-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stderr but no output)', bash, suite: 'write', args: [files('cron-pipe-write-failure-stderr2.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (file)', bash, suite: 'write', args: [files('cron-pipe-write-failure-file.sh')], env: test_env, depends: all_test_txt)

test('command writes to stdout/stderr after dtee exits', bash, suite: 'write', args: [files('command-write-after-ppid-exit.sh')], env: test_env, depends: all_test_txt)

if host_machine.system() != 'cygwin'
	# These don't work on Cygwin because the FIFO can't have multiple readers/writers
	test('interrupted by signal during write', bash, suite: 'write', args: [files('interrupt-write.sh')], env: test_env, depends: all_test_txt)
	test('interrupted by signal during write (no SA_RESTART)', bash, suite: 'write', args: [files('interrupt-write-no-restart.sh')], env: test_env, depends: all_test_txt)
	test('cron interrupt by signal during write', bash, suite: 'write', args: [files('cron-interrupt-write.sh')], env: test_env, depends: all_test_txt)
	test('cron interrupt by signal during write (no SA_RESTART)', bash, suite: 'write', args: [files('cron-interrupt-write-no-restart.sh')], env: test_env, depends: all_test_txt)
endif
