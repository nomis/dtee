if host_machine.system() != 'cygwin'
	# Assume Cygwin is insecure because the underlying Unix socket implementation is UDP
	test('recreate the input socket after the parent process exits', default_signal_handlers, suite: 'security', args: [bash, files('recreate-input-socket.sh')], env: test_env, depends: all_test_txt)
	test('recreate the output socket after the child process exits', default_signal_handlers, suite: 'security', args: [bash, files('recreate-output-socket.sh')], env: test_env, depends: all_test_txt)
endif
