test('report arguments and file descriptors', bash, suite: 'fork', args: [files('report.sh')], env: test_env, depends: all_test_txt)
test('report arguments and file descriptors (no stdin)', bash, suite: 'fork', args: [files('report-no-stdin.sh')], env: test_env, depends: all_test_txt)
test('report arguments and file descriptors (extra fd)', bash, suite: 'fork', args: [files('report-extra-fd.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors', bash, suite: 'fork', args: [files('cron-report.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors (no stdin)', bash, suite: 'fork', args: [files('cron-report-no-stdin.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors (extra fd)', bash, suite: 'fork', args: [files('cron-report-extra-fd.sh')], env: test_env, depends: all_test_txt)

test('fork failure', bash, suite: 'fork', args: [files('fork-failure.sh')], env: test_env, depends: all_test_txt)
test('dup2 failure', bash, suite: 'fork', args: [files('dup2-failure.sh')], env: test_env, depends: all_test_txt)
test('socket close failure (child)', bash, suite: 'fork', args: [files('socket-close-failure-child.sh')], env: test_env, depends: all_test_txt)
test('socket close failure (parent)', bash, suite: 'fork', args: [files('socket-close-failure-parent.sh')], env: test_env, depends: all_test_txt)
test('cron fork failure', bash, suite: 'fork', args: [files('cron-fork-failure.sh')], env: test_env, depends: all_test_txt)
test('cron dup2 failure', bash, suite: 'fork', args: [files('cron-dup2-failure.sh')], env: test_env, depends: all_test_txt)
test('cron socket close failure (child)', bash, suite: 'fork', args: [files('cron-socket-close-failure-child.sh')], env: test_env, depends: all_test_txt)
test('cron socket close failure (parent)', bash, suite: 'fork', args: [files('cron-socket-close-failure-parent.sh')], env: test_env, depends: all_test_txt)

test('waitpid failure (no children)', bash, suite: 'fork', args: [files('waitpid-failure-no-children.sh')], env: test_env, depends: all_test_txt)
test('waitpid failure (no change)', bash, suite: 'fork', args: [files('waitpid-failure-no-change.sh')], env: test_env, depends: all_test_txt)
test('cron waitpid failure (no children)', bash, suite: 'fork', args: [files('cron-waitpid-failure-no-children.sh')], env: test_env, depends: all_test_txt)
test('cron waitpid failure (no change)', bash, suite: 'fork', args: [files('cron-waitpid-failure-no-change.sh')], env: test_env, depends: all_test_txt)

test('sigaction failure', bash, suite: 'fork', args: [files('sigaction-failure.sh')], env: test_env, depends: all_test_txt)
test('cron sigaction failure', bash, suite: 'fork', args: [files('cron-sigaction-failure.sh')], env: test_env, depends: all_test_txt)

# Test errors in Boost.Asio workaround at SignalHandler::add_non_interrupting_signal()
test('sigaction query failure', bash, suite: 'fork', args: [files('sigaction-query-failure.sh')], env: test_env, depends: all_test_txt)
test('sigaction set SA_RESTART failure', bash, suite: 'fork', args: [files('sigaction-set-restart-failure.sh')], env: test_env, depends: all_test_txt)
test('sigaction already has SA_RESTART set', bash, suite: 'fork', args: [files('sigaction-restart-already-set.sh')], env: test_env, depends: all_test_txt)
test('cron sigaction query failure', bash, suite: 'fork', args: [files('cron-sigaction-query-failure.sh')], env: test_env, depends: all_test_txt)
test('cron sigaction set SA_RESTART failure', bash, suite: 'fork', args: [files('cron-sigaction-set-restart-failure.sh')], env: test_env, depends: all_test_txt)
test('cron sigaction already has SA_RESTART set', bash, suite: 'fork', args: [files('cron-sigaction-restart-already-set.sh')], env: test_env, depends: all_test_txt)
