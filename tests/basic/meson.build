test('dtee (no arguments)', bash, suite: 'basic', args: [files('noargs-dtee.sh')], env: test_env, depends: all_test_txt)
test('cronty (no arguments)', bash, suite: 'basic', args: [files('noargs-cronty.sh')], env: test_env, depends: all_test_txt)
test('dtee --help', bash, suite: 'basic', args: [files('help-dtee.sh')], env: test_env, depends: all_test_txt)
test('cronty --help', bash, suite: 'basic', args: [files('help-cronty.sh')], env: test_env, depends: all_test_txt)
test('dtee --version', bash, suite: 'basic', args: [files('version.sh')], env: test_env, depends: all_test_txt)

test('passthrough stdout', bash, suite: 'basic', args: [files('passthrough-stdout.sh')], env: test_env, depends: all_test_txt)
test('passthrough stderr', bash, suite: 'basic', args: [files('passthrough-stderr.sh')], env: test_env, depends: all_test_txt)
test('passthrough stdout and stderr 1', bash, suite: 'basic', args: [files('passthrough-stdout-stderr1.sh')], env: test_env, depends: all_test_txt)
test('passthrough stdout and stderr 2', bash, suite: 'basic', args: [files('passthrough-stdout-stderr2.sh')], env: test_env, depends: all_test_txt)
test('interleaved characters', bash, suite: 'basic', args: [files('interleaved-characters.sh')], env: test_env, depends: all_test_txt)

test('file stdout overwrite (new file)', bash, suite: 'basic', args: [files('file-stdout-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file stdout overwrite (existing)', bash, suite: 'basic', args: [files('file-stdout-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file stdout append (new file)', bash, suite: 'basic', args: [files('file-stdout-append-new.sh')], env: test_env, depends: all_test_txt)
test('file stdout append (existing)', bash, suite: 'basic', args: [files('file-stdout-append-existing.sh')], env: test_env, depends: all_test_txt)

test('file stderr overwrite (new file)', bash, suite: 'basic', args: [files('file-stderr-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file stderr overwrite (existing)', bash, suite: 'basic', args: [files('file-stderr-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file stderr append (new file)', bash, suite: 'basic', args: [files('file-stderr-append-new.sh')], env: test_env, depends: all_test_txt)
test('file stderr append (existing)', bash, suite: 'basic', args: [files('file-stderr-append-existing.sh')], env: test_env, depends: all_test_txt)

test('file combined overwrite (new file)', bash, suite: 'basic', args: [files('file-combined-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file combined overwrite (existing)', bash, suite: 'basic', args: [files('file-combined-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file combined append (new file)', bash, suite: 'basic', args: [files('file-combined-append-new.sh')], env: test_env, depends: all_test_txt)
test('file combined append (existing)', bash, suite: 'basic', args: [files('file-combined-append-existing.sh')], env: test_env, depends: all_test_txt)

test('throw exception', bash, suite: 'basic', args: [files('throw-exception.sh')], env: test_env, depends: all_test_txt)
test('cron throw exception', bash, suite: 'basic', args: [files('cron-throw-exception.sh')], env: test_env, depends: all_test_txt)
