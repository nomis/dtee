test('report arguments and file descriptors', default_signal_handlers, suite: 'fork', args: [bash, files('report.sh')], env: test_env, depends: all_test_deps)
test('report arguments and file descriptors (no stdin)', default_signal_handlers, suite: 'fork', args: [bash, files('report-no-stdin.sh')], env: test_env, depends: all_test_deps)
test('report arguments and file descriptors (extra fd)', default_signal_handlers, suite: 'fork', args: [bash, files('report-extra-fd.sh')], env: test_env, depends: all_test_deps)
test('cron report arguments and file descriptors', default_signal_handlers, suite: 'fork', args: [bash, files('cron-report.sh')], env: test_env, depends: all_test_deps)
test('cron report arguments and file descriptors (no stdin)', default_signal_handlers, suite: 'fork', args: [bash, files('cron-report-no-stdin.sh')], env: test_env, depends: all_test_deps)
test('cron report arguments and file descriptors (extra fd)', default_signal_handlers, suite: 'fork', args: [bash, files('cron-report-extra-fd.sh')], env: test_env, depends: all_test_deps)

test('fork failure', default_signal_handlers, suite: 'fork', args: [bash, files('fork-failure.sh')], env: test_env, depends: all_test_deps)
test('dup2 failure', default_signal_handlers, suite: 'fork', args: [bash, files('dup2-failure.sh')], env: test_env, depends: all_test_deps)
test('socket close failure (child)', default_signal_handlers, suite: 'fork', args: [bash, files('socket-close-failure-child.sh')], env: test_env, depends: all_test_deps)
test('socket close failure (parent)', default_signal_handlers, suite: 'fork', args: [bash, files('socket-close-failure-parent.sh')], env: test_env, depends: all_test_deps)
test('cron fork failure', default_signal_handlers, suite: 'fork', args: [bash, files('cron-fork-failure.sh')], env: test_env, depends: all_test_deps)
test('cron dup2 failure', default_signal_handlers, suite: 'fork', args: [bash, files('cron-dup2-failure.sh')], env: test_env, depends: all_test_deps)
test('cron socket close failure (child)', default_signal_handlers, suite: 'fork', args: [bash, files('cron-socket-close-failure-child.sh')], env: test_env, depends: all_test_deps)
test('cron socket close failure (parent)', default_signal_handlers, suite: 'fork', args: [bash, files('cron-socket-close-failure-parent.sh')], env: test_env, depends: all_test_deps)

test('waitpid failure (no children)', default_signal_handlers, suite: 'fork', args: [bash, files('waitpid-failure-no-children.sh')], env: test_env, depends: all_test_deps)
test('waitpid failure (no change)', default_signal_handlers, suite: 'fork', args: [bash, files('waitpid-failure-no-change.sh')], env: test_env, depends: all_test_deps)
test('cron waitpid failure (no children)', default_signal_handlers, suite: 'fork', args: [bash, files('cron-waitpid-failure-no-children.sh')], env: test_env, depends: all_test_deps)
test('cron waitpid failure (no change)', default_signal_handlers, suite: 'fork', args: [bash, files('cron-waitpid-failure-no-change.sh')], env: test_env, depends: all_test_deps)

test('sigaction failure', default_signal_handlers, suite: 'fork', args: [bash, files('sigaction-failure.sh')], env: test_env, depends: all_test_deps)
test('cron sigaction failure', default_signal_handlers, suite: 'fork', args: [bash, files('cron-sigaction-failure.sh')], env: test_env, depends: all_test_deps)

if not has_boost_asio_1_27_0
	# Test errors in Boost.Asio workaround at SignalHandler::add()
	test('sigaction query failure', default_signal_handlers, suite: 'fork', args: [bash, files('sigaction-query-failure.sh')], env: test_env, depends: all_test_deps)
	test('sigaction set SA_RESTART failure', default_signal_handlers, suite: 'fork', args: [bash, files('sigaction-set-restart-failure.sh')], env: test_env, depends: all_test_deps)
	test('sigaction already has SA_RESTART set', default_signal_handlers, suite: 'fork', args: [bash, files('sigaction-restart-already-set.sh')], env: test_env, depends: all_test_deps)
	test('cron sigaction query failure', default_signal_handlers, suite: 'fork', args: [bash, files('cron-sigaction-query-failure.sh')], env: test_env, depends: all_test_deps)
	test('cron sigaction set SA_RESTART failure', default_signal_handlers, suite: 'fork', args: [bash, files('cron-sigaction-set-restart-failure.sh')], env: test_env, depends: all_test_deps)
	test('cron sigaction already has SA_RESTART set', default_signal_handlers, suite: 'fork', args: [bash, files('cron-sigaction-restart-already-set.sh')], env: test_env, depends: all_test_deps)
endif
