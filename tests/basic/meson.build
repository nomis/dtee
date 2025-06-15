test('dtee (no arguments)', default_signal_handlers, suite: 'basic', args: [bash, files('noargs-dtee.sh')], env: test_env, depends: all_test_deps)
test('cronty (no arguments)', default_signal_handlers, suite: 'basic', args: [bash, files('noargs-cronty.sh')], env: test_env, depends: all_test_deps)
test('dtee --help', default_signal_handlers, suite: 'basic', args: [bash, files('help-dtee.sh')], env: test_env, depends: all_test_deps)
test('cronty --help', default_signal_handlers, suite: 'basic', args: [bash, files('help-cronty.sh')], env: test_env, depends: all_test_deps)
test('dtee --version', default_signal_handlers, suite: 'basic', args: [bash, files('version.sh')], env: test_env, depends: all_test_deps)

test('passthrough stdout', default_signal_handlers, suite: 'basic', args: [bash, files('passthrough-stdout.sh')], env: test_env, depends: all_test_deps)
test('passthrough stderr', default_signal_handlers, suite: 'basic', args: [bash, files('passthrough-stderr.sh')], env: test_env, depends: all_test_deps)
test('passthrough stdout and stderr 1', default_signal_handlers, suite: 'basic', args: [bash, files('passthrough-stdout-stderr1.sh')], env: test_env, depends: all_test_deps)
test('passthrough stdout and stderr 2', default_signal_handlers, suite: 'basic', args: [bash, files('passthrough-stdout-stderr2.sh')], env: test_env, depends: all_test_deps)
test('interleaved characters', default_signal_handlers, suite: 'basic', args: [bash, files('interleaved-characters.sh')], env: test_env, depends: all_test_deps)

test('file stdout overwrite (new file)', default_signal_handlers, suite: 'basic', args: [bash, files('file-stdout-overwrite-new.sh')], env: test_env, depends: all_test_deps)
test('file stdout overwrite (existing)', default_signal_handlers, suite: 'basic', args: [bash, files('file-stdout-overwrite-existing.sh')], env: test_env, depends: all_test_deps)
test('file stdout append (new file)', default_signal_handlers, suite: 'basic', args: [bash, files('file-stdout-append-new.sh')], env: test_env, depends: all_test_deps)
test('file stdout append (existing)', default_signal_handlers, suite: 'basic', args: [bash, files('file-stdout-append-existing.sh')], env: test_env, depends: all_test_deps)

test('file stderr overwrite (new file)', default_signal_handlers, suite: 'basic', args: [bash, files('file-stderr-overwrite-new.sh')], env: test_env, depends: all_test_deps)
test('file stderr overwrite (existing)', default_signal_handlers, suite: 'basic', args: [bash, files('file-stderr-overwrite-existing.sh')], env: test_env, depends: all_test_deps)
test('file stderr append (new file)', default_signal_handlers, suite: 'basic', args: [bash, files('file-stderr-append-new.sh')], env: test_env, depends: all_test_deps)
test('file stderr append (existing)', default_signal_handlers, suite: 'basic', args: [bash, files('file-stderr-append-existing.sh')], env: test_env, depends: all_test_deps)

test('file combined overwrite (new file)', default_signal_handlers, suite: 'basic', args: [bash, files('file-combined-overwrite-new.sh')], env: test_env, depends: all_test_deps)
test('file combined overwrite (existing)', default_signal_handlers, suite: 'basic', args: [bash, files('file-combined-overwrite-existing.sh')], env: test_env, depends: all_test_deps)
test('file combined append (new file)', default_signal_handlers, suite: 'basic', args: [bash, files('file-combined-append-new.sh')], env: test_env, depends: all_test_deps)
test('file combined append (existing)', default_signal_handlers, suite: 'basic', args: [bash, files('file-combined-append-existing.sh')], env: test_env, depends: all_test_deps)

test('throw exception', default_signal_handlers, suite: 'basic', args: [bash, files('throw-exception.sh')], env: test_env, depends: all_test_deps)
test('cron throw exception', default_signal_handlers, suite: 'basic', args: [bash, files('cron-throw-exception.sh')], env: test_env, depends: all_test_deps)
