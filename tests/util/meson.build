if host_machine.system() == 'linux' or host_machine.system() == 'gnu'
	test_c_args = c_args + ['-D_DEFAULT_SOURCE']
	dlfcn_args = test_c_args + ['-D_GNU_SOURCE', '-Wno-pedantic']
	dlfcn_deps = [c.find_library('dl')]
	wrap_gettext_deps = []
elif host_machine.system() == 'darwin'
	test_c_args = c_args
	dlfcn_args = test_c_args
	dlfcn_deps = []
	wrap_gettext_deps = gettext_deps
elif host_machine.system() == 'cygwin'
	test_c_args = c_args
	dlfcn_args = test_c_args + ['-Wno-pedantic']
	dlfcn_deps = []
	wrap_gettext_deps = gettext_deps
else # freebsd, openbsd, netbsd, dragonfly
	test_c_args = c_args
	dlfcn_args = test_c_args + ['-Wno-pedantic']
	dlfcn_deps = []
	wrap_gettext_deps = []
endif

if host_machine.system() == 'freebsd' or host_machine.system() == 'openbsd' or host_machine.system() == 'netbsd' or host_machine.system() == 'dragonfly'
	proc_deps = [c.find_library('kvm')]
else # linux, gnu, darwin
	proc_deps = []
endif

if host_machine.system() == 'linux' or host_machine.system() == 'gnu'
	has_generic_sendfile = true
else # freebsd, openbsd, netbsd, dragonfly, darwin, cygwin
	has_generic_sendfile = false
endif

testlib_allow_n_times = shared_library('test-allow-n-times', ['allow-n-times.c'], c_args: test_c_args, build_rpath: meson.current_build_dir())
testlib_is_dtee = shared_library('test-is-dtee', ['is-dtee.c', 'is-dtee-' + host_machine.system() + '.c'], c_args: dlfcn_args, dependencies: dlfcn_deps + proc_deps, build_rpath: meson.current_build_dir())
testlib_is_fd_unix_socket = shared_library('test-fd-unix-socket', ['is-fd-unix-socket.c'], c_args: test_c_args, build_rpath: meson.current_build_dir())

shared_library('test-bindtextdomain-override', ['bindtextdomain-override.c'] , c_args: dlfcn_args + ['-DGETTEXT_LOCALEDIR="' + meson.current_build_dir() + '/dtee@test/locale"'], dependencies: dlfcn_deps + wrap_gettext_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-dup2-failure', ['dup2-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-execvp-fd-check', ['execvp-fd-check.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-fake-getpid', ['fake-getpid.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-fake-getuid', ['fake-getuid.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-fake-strerror', ['fake-strerror.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-fake-strsignal', ['fake-strsignal.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-fork-failure', ['fork-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-fork-sigchld-check', ['fork-sigchld-check.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-file-write-failure', ['file-write-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-file-write-failure-recovery', ['file-write-failure-recovery.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-lseek-failure', ['lseek-failure.c'] , c_args: dlfcn_args + ['-D_LARGEFILE64_SOURCE'], dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-mkdtemp-consistent', ['mkdtemp-consistent.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-mkdtemp-failure', ['mkdtemp-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-mkostemp-consistent', ['mkostemp-consistent.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-mkostemp-failure', ['mkostemp-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-mkostemp-readonly', ['mkostemp-readonly.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-mkostemp-writeonly', ['mkostemp-writeonly.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-raise-failure', ['raise-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-security-recreate-output-socket', ['security-recreate-output-socket.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-sigaction-failure', ['sigaction-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-sigaction-no-restart', ['sigaction-no-restart.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-sigaction-query-failure', ['sigaction-query-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-sigaction-restart-already-set', ['sigaction-restart-already-set.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-sigaction-set-restart-failure', ['sigaction-set-restart-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-socket-add-extra-source', ['socket-add-extra-source.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-bind-failure', ['socket-bind-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-close-failure', ['socket-close-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-connect-failure', ['socket-connect-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-getsockopt-fake-rcvbuf', ['socket-getsockopt-fake-rcvbuf.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-recv-buffer-too-small', ['socket-recv-buffer-too-small.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-recv-failure', ['socket-recv-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-setsockopt-monitor', ['socket-setsockopt-monitor.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-shutdown-failure', ['socket-shutdown-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket], build_rpath: meson.current_build_dir())
shared_library('test-socket-unix-failure', ['socket-unix-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-waitpid-failure', ['waitpid-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-write-throw-exception', ['write-throw-exception.cc'] , cpp_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-write-truncate-cron', ['write-truncate-cron.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())
shared_library('test-write-truncate-stdout', ['write-truncate-stdout.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee], build_rpath: meson.current_build_dir())

executable('test-closed-pipe-reader', ['closed-pipe-reader.c'], c_args: test_c_args)
default_signal_handlers = executable('test-default-signal-handlers', ['default-signal-handlers.c'], c_args: test_c_args)
executable('test-closed-pipe-writer', ['closed-pipe-writer.c'], c_args: test_c_args)
executable('test-fifo-write-interrupt', ['fifo-write-interrupt.c'] , c_args: test_c_args)
executable('test-lorem-ipsum-late-exit', ['lorem-ipsum-late-exit.c'], c_args: test_c_args)
executable('test-lorem-ipsum-wait-exit', ['lorem-ipsum-wait-exit.c'], c_args: test_c_args)
executable('test-kill-pid', ['kill-pid.c'], c_args: test_c_args)
executable('test-kill-ppid', ['kill-ppid.c'], c_args: test_c_args, link_with: [testlib_is_fd_unix_socket])
executable('test-report', ['report.c'], c_args: test_c_args, link_with: [testlib_is_fd_unix_socket])
executable('test-security-recreate-input-socket', ['security-recreate-input-socket.c'], c_args: test_c_args)
if has_generic_sendfile
	executable('test-sendfile', ['sendfile.c'], c_args: test_c_args)
endif
generate_signal_txt = executable('test-signal-txt', ['signal-txt.c'], c_args: test_c_args)
executable('test-stream-read', ['stream-read.c'], c_args: test_c_args)
executable('test-stream-write', ['stream-write.c'], c_args: test_c_args)
generate_sysexits_txt = executable('test-sysexits-txt', ['sysexits-txt.c'], c_args: test_c_args)
executable('test-waitpid', ['waitpid.c'], c_args: test_c_args)
executable('test-write-0-bytes', ['write-0-bytes.c'], c_args: test_c_args)
executable('test-writes-of-n-bytes', ['writes-of-n-bytes.c'], c_args: test_c_args)

signal_txt = custom_target('signal.txt', output: 'signal.txt',
	command: [generate_signal_txt, '@OUTPUT@'])
sysexits_txt = custom_target('sysexits.txt', output: 'sysexits.txt',
	command: [generate_sysexits_txt, '@OUTPUT@'])
version_txt = vcs_tag(input: 'version.txt.in', output: 'version.txt')
all_test_txt = [signal_txt, sysexits_txt, version_txt]
