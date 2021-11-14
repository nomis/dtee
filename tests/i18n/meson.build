foreach language, mo_file : all_test_i18n
	test(language + ' dtee --help', default_signal_handlers, suite: 'i18n', args: [bash, files('help-dtee.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
	test(language + ' cronty --help', default_signal_handlers, suite: 'i18n', args: [bash, files('help-cronty.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
	test(language + ' dtee --version', default_signal_handlers, suite: 'i18n', args: [bash, files('version.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
	test(language + ' cron command exit code 42', default_signal_handlers, suite: 'i18n', args: [bash, files('cron-command-exit-42.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
	test(language + ' cron command terminated by signal 15', default_signal_handlers, suite: 'i18n', args: [bash, files('cron-command-sigterm.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
endforeach
