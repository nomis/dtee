if host_machine.system() != 'cygwin'
	# Can't override libintl_bindtextdomain on Cygwin
	foreach language, mo_file : all_test_i18n
		test(language + ' dtee --help', bash, suite: 'i18n', args: [files('help-dtee.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
		test(language + ' cronty --help', bash, suite: 'i18n', args: [files('help-cronty.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
		test(language + ' dtee --version', bash, suite: 'i18n', args: [files('version.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
		test(language + ' cron command exit code 42', bash, suite: 'i18n', args: [files('cron-command-exit-42.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
		test(language + ' cron command terminated by signal 15', bash, suite: 'i18n', args: [files('cron-command-sigterm.sh'), language], env: test_env, depends: all_test_txt + [mo_file])
	endforeach
endif
