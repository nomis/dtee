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
subdir('output')
subdir('input')

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
