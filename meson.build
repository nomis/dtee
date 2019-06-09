project('dtee',
	['c', 'cpp'],
	default_options: [
		'c_std=c11',
		'cpp_std=c++14',
		'warning_level=3',
		'werror=true',
	],
	version: '1.0.1',
	license: 'GPL3+',
	meson_version: '>=0.46.0',
)

subdir('i18n')

source_files = [
	'src/application.cc',
	'src/command_line.cc',
	'src/dispatch.cc',
	'src/cron.cc',
	'src/file_output.cc',
	'src/input.cc',
	'src/main.cc',
	'src/print_error.cc',
	'src/process.cc',
	'src/signal_handler.cc',
	'src/stream_output.cc',
	'src/temp_directory.cc',
	'src/temp_file.cc',
	'src/temp_filename_pattern.cc',
	'src/to_string.cc',
	'src/uninterruptible.cc',
	vcs_tag(input: 'src/version.h.in', output: 'version.h'),
]

c = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

if not (c.get_id() == 'gcc' and c.version().version_compare('<5'))
	if c.has_argument('-Wshadow')
		add_project_arguments('-Wshadow', language: 'c')
	endif
	if cpp.has_argument('-Wshadow')
		add_project_arguments('-Wshadow', language: 'cpp')
	endif
endif

if c.has_argument('-pipe')
	add_project_arguments('-pipe', language: 'c')
	add_project_link_arguments('-pipe', language: 'c')
endif
if cpp.has_argument('-pipe')
	add_project_arguments('-pipe', language: 'cpp')
	add_project_link_arguments('-pipe', language: 'cpp')
endif

if host_machine.system() == 'freebsd' or host_machine.system() == 'dragonfly'
	c_args = []
elif host_machine.system() == 'openbsd'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_BSD_SOURCE']
elif host_machine.system() == 'netbsd'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_NETBSD_SOURCE']
elif host_machine.system() == 'gnu'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-DPIPE_BUF=4096', '-DPATH_MAX=4096']
elif host_machine.system() == 'darwin'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_DARWIN_C_SOURCE']
elif host_machine.system() == 'cygwin'
	c_args = ['-D_POSIX_C_SOURCE=200809L', '-D_GNU_SOURCE']
else # linux
	c_args = ['-D_POSIX_C_SOURCE=200809L']
endif

if host_machine.system() == 'netbsd' # 😩
	link_args = ['-Wl,--wrap=sigaction', '-Wl,--wrap=__sigaction14']
elif host_machine.system() == 'darwin'
	link_args = ['-Wl,-alias,_sigaction,___real_sigaction']
else # linux, freebsd, openbsd, dragonfly, gnu, cygwin
	link_args = ['-Wl,--wrap=sigaction']
endif

if cpp.has_function('gettext')
	gettext_deps = []
else
	gettext_deps = [cpp.find_library('intl')]
endif

python = import('python').find_installation()

version_check = run_command(python, [files('docs/version_check.py'), files('docs/changelog.rst'), meson.project_version()])
if version_check.returncode() != 0
	error(version_check.stderr())
endif

dtee = executable('dtee',
	source_files,
	cpp_args: c_args + [
		'-DGETTEXT_PACKAGE="dtee"',
		'-DGETTEXT_LOCALEDIR="' + join_paths(get_option('prefix'), get_option('localedir')) + '"',
		# threads are not used, remove these if they are otherwise the asio signal handler will not work correctly
		'-DBOOST_ASIO_DISABLE_THREADS',
	] + (get_option('b_coverage') ? ['-DGCOV_ENABLED'] : []),
	link_args: link_args,
	dependencies: [
		dependency('boost', modules: [
			'filesystem',
			'program_options',
			'system',
		]),
	] + gettext_deps,
	install: true)

ln = find_program('ln')

custom_target('cronty',
	input: dtee,
	output: 'cronty',
	command: [ln, '-s', '-f', '@INPUT@', '@OUTPUT@'],
	install: true,
	install_dir: get_option('bindir'))

sphinx_build = find_program('sphinx-build', required: false)

if sphinx_build.found()
	source_docs = [
		'docs/architecture.rst',
		'docs/changelog.rst',
		'docs/conf.py',
		'docs/dependencies.rst',
		'docs/index.rst',
		'docs/install.rst',
		'docs/limitations.rst',
		'docs/manual.rst',
		'docs/packages.rst',
		'docs/packages/debian.rst',
		'docs/packages/fedora.rst',
		'docs/packages/rhel.rst',
		'docs/packages/ubuntu.rst',
	]

	dtee_man = custom_target('dtee.1',
		input: source_docs,
		output: 'dtee.1',
		command: [
			sphinx_build,
			'-n',
			'-W',
			'-b', 'man',
			'-D', 'version=' + meson.project_version(),
			'-D', 'release=' + meson.project_version(),
			'@CURRENT_SOURCE_DIR@/docs',
			'@BUILD_ROOT@',
		],
		install: true,
		install_dir: join_paths(get_option('mandir'), 'man1'))

	custom_target('cronty.1',
		input: dtee_man,
		output: 'cronty.1',
		command: [ln, '-s', '-f', '@INPUT@', '@OUTPUT@'],
		install: true,
		install_dir: join_paths(get_option('mandir'), 'man1'))
endif

run_target('cppcheck',
	command: [
		'cppcheck',
		'--project=' + join_paths(meson.current_build_dir(), 'compile_commands.json'),
		'--enable=all',
		'--inline-suppr',
	] + source_files)

test_env = [
	'LC_ALL=C',
	'LANG=C',
]

if host_machine.system() == 'linux' or host_machine.system() == 'gnu'
	test_c_args = c_args + ['-D_DEFAULT_SOURCE']
	dlfcn_args = test_c_args + ['-D_GNU_SOURCE', '-Wno-pedantic']
	dlfcn_deps = [c.find_library('dl')]
elif host_machine.system() == 'darwin'
	test_c_args = c_args
	dlfcn_args = test_c_args
	dlfcn_deps = []
else # freebsd, openbsd, netbsd, dragonfly, cygwin
	test_c_args = c_args
	dlfcn_args = test_c_args + ['-Wno-pedantic']
	dlfcn_deps = []
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

testlib_allow_n_times = shared_library('test-allow-n-times', ['tests/allow-n-times.c'], c_args: test_c_args)
testlib_is_dtee = shared_library('test-is-dtee', ['tests/is-dtee.c', 'tests/is-dtee-' + host_machine.system() + '.c'], c_args: dlfcn_args, dependencies: dlfcn_deps + proc_deps)
testlib_is_fd_unix_socket = shared_library('test-fd-unix-socket', ['tests/is-fd-unix-socket.c'], c_args: test_c_args)

shared_library('test-dup2-failure', ['tests/dup2-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-execvp-fd-check', ['tests/execvp-fd-check.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-getpid', ['tests/fake-getpid.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-getuid', ['tests/fake-getuid.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-strerror', ['tests/fake-strerror.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fake-strsignal', ['tests/fake-strsignal.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-fork-failure', ['tests/fork-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-file-write-failure', ['tests/file-write-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-file-write-failure-recovery', ['tests/file-write-failure-recovery.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-kill-failure', ['tests/kill-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-lseek-failure', ['tests/lseek-failure.c'] , c_args: dlfcn_args + ['-D_LARGEFILE64_SOURCE'], dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkdtemp-consistent', ['tests/mkdtemp-consistent.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkdtemp-failure', ['tests/mkdtemp-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-consistent', ['tests/mkostemp-consistent.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-failure', ['tests/mkostemp-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-readonly', ['tests/mkostemp-readonly.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-mkostemp-writeonly', ['tests/mkostemp-writeonly.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-security-recreate-output-socket', ['tests/security-recreate-output-socket.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-sigaction-no-restart', ['tests/sigaction-no-restart.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-add-extra-source', ['tests/socket-add-extra-source.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-bind-failure', ['tests/socket-bind-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-close-failure', ['tests/socket-close-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-connect-failure', ['tests/socket-connect-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-getsockopt-fake-rcvbuf', ['tests/socket-getsockopt-fake-rcvbuf.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-recv-buffer-too-small', ['tests/socket-recv-buffer-too-small.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-recv-failure', ['tests/socket-recv-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-setsockopt-monitor', ['tests/socket-setsockopt-monitor.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-shutdown-failure', ['tests/socket-shutdown-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee, testlib_is_fd_unix_socket])
shared_library('test-socket-unix-failure', ['tests/socket-unix-failure.c'], c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_allow_n_times, testlib_is_dtee])
shared_library('test-waitpid-failure', ['tests/waitpid-failure.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-write-truncate-cron', ['tests/write-truncate-cron.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])
shared_library('test-write-truncate-stdout', ['tests/write-truncate-stdout.c'] , c_args: dlfcn_args, dependencies: dlfcn_deps, link_with: [testlib_is_dtee])

executable('test-closed-pipe-reader', ['tests/closed-pipe-reader.c'], c_args: test_c_args)
executable('test-closed-pipe-writer', ['tests/closed-pipe-writer.c'], c_args: test_c_args)
executable('test-fifo-write-interrupt', ['tests/fifo-write-interrupt.c'] , c_args: test_c_args)
executable('test-lorem-ipsum-late-exit', ['tests/lorem-ipsum-late-exit.c'], c_args: test_c_args)
executable('test-lorem-ipsum-wait-exit', ['tests/lorem-ipsum-wait-exit.c'], c_args: test_c_args)
executable('test-kill-pid', ['tests/kill-pid.c'], c_args: test_c_args)
executable('test-kill-ppid', ['tests/kill-ppid.c'], c_args: test_c_args, link_with: [testlib_is_fd_unix_socket])
executable('test-report', ['tests/report.c'], c_args: test_c_args, link_with: [testlib_is_fd_unix_socket])
executable('test-security-recreate-input-socket', ['tests/security-recreate-input-socket.c'], c_args: test_c_args)
if has_generic_sendfile
	executable('test-sendfile', ['tests/sendfile.c'], c_args: test_c_args)
endif
generate_signal_txt = executable('test-signal-txt', ['tests/signal-txt.c'], c_args: test_c_args)
executable('test-stream-read', ['tests/stream-read.c'], c_args: test_c_args)
executable('test-stream-write', ['tests/stream-write.c'], c_args: test_c_args)
generate_sysexits_txt = executable('test-sysexits-txt', ['tests/sysexits-txt.c'], c_args: test_c_args)
executable('test-waitpid', ['tests/waitpid.c'], c_args: test_c_args)
executable('test-write-0-bytes', ['tests/write-0-bytes.c'], c_args: test_c_args)
executable('test-writes-of-n-bytes', ['tests/writes-of-n-bytes.c'], c_args: test_c_args)

signal_txt = custom_target('signal.txt',
	command: [generate_signal_txt],
	output: ['signal.txt'])
sysexits_txt = custom_target('sysexits.txt',
	command: [generate_sysexits_txt],
	output: 'sysexits.txt')
all_test_txt = [signal_txt, sysexits_txt]

bash = find_program('bash')

xfail = get_option('test_expect_fail')
xfail_general_writes = ['netbsd'].contains(host_machine.system())
xfail_stream = xfail_general_writes or ['freebsd', 'openbsd', 'darwin'].contains(host_machine.system())
xfail_stream_1mb = xfail_stream
xfail_stream_32mb = xfail_stream or ['cygwin'].contains(host_machine.system())
xfail_stream_256mb = xfail_stream or ['cygwin'].contains(host_machine.system())
xfail_writes_of_1_byte = xfail_general_writes or xfail_stream
xfail_small_writes = xfail_general_writes or ['dragonfly'].contains(host_machine.system())
xfail_large_writes = xfail_general_writes

test('dtee (no arguments)', bash, args: [files('tests/noargs-dtee.sh')], env: test_env, depends: all_test_txt)
test('cronty (no arguments)', bash, args: [files('tests/noargs-cronty.sh')], env: test_env, depends: all_test_txt)
test('dtee --help', bash, args: [files('tests/help-dtee.sh')], env: test_env, depends: all_test_txt)
test('cronty --help', bash, args: [files('tests/help-cronty.sh')], env: test_env, depends: all_test_txt)
test('dtee --version', bash, args: [files('tests/version.sh')], env: test_env, depends: all_test_txt)

test('passthrough stdout', bash, args: [files('tests/passthrough-stdout.sh')], env: test_env, depends: all_test_txt)
test('passthrough stderr', bash, args: [files('tests/passthrough-stderr.sh')], env: test_env, depends: all_test_txt)
test('passthrough stdout and stderr 1', bash, args: [files('tests/passthrough-stdout-stderr1.sh')], env: test_env, depends: all_test_txt)
test('passthrough stdout and stderr 2', bash, args: [files('tests/passthrough-stdout-stderr2.sh')], env: test_env, depends: all_test_txt)
test('interleaved characters', bash, args: [files('tests/interleaved-characters.sh')], env: test_env, depends: all_test_txt)

test('file stdout overwrite (new file)', bash, args: [files('tests/file-stdout-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file stdout overwrite (existing)', bash, args: [files('tests/file-stdout-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file stdout append (new file)', bash, args: [files('tests/file-stdout-append-new.sh')], env: test_env, depends: all_test_txt)
test('file stdout append (existing)', bash, args: [files('tests/file-stdout-append-existing.sh')], env: test_env, depends: all_test_txt)

test('file stderr overwrite (new file)', bash, args: [files('tests/file-stderr-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file stderr overwrite (existing)', bash, args: [files('tests/file-stderr-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file stderr append (new file)', bash, args: [files('tests/file-stderr-append-new.sh')], env: test_env, depends: all_test_txt)
test('file stderr append (existing)', bash, args: [files('tests/file-stderr-append-existing.sh')], env: test_env, depends: all_test_txt)

test('file combined overwrite (new file)', bash, args: [files('tests/file-combined-overwrite-new.sh')], env: test_env, depends: all_test_txt)
test('file combined overwrite (existing)', bash, args: [files('tests/file-combined-overwrite-existing.sh')], env: test_env, depends: all_test_txt)
test('file combined append (new file)', bash, args: [files('tests/file-combined-append-new.sh')], env: test_env, depends: all_test_txt)
test('file combined append (existing)', bash, args: [files('tests/file-combined-append-existing.sh')], env: test_env, depends: all_test_txt)

test('write 0 bytes (stdout)', bash, args: [files('tests/write-0-bytes-stdout.sh')], env: test_env, depends: all_test_txt)
test('write 0 bytes (stderr)', bash, args: [files('tests/write-0-bytes-stderr.sh')], env: test_env, depends: all_test_txt)
test('writes of 1 byte (stdout)', bash, args: [files('tests/writes-of-1-byte-stdout.sh')], env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('writes of 1 byte (stderr)', bash, args: [files('tests/writes-of-1-byte-stderr.sh')], env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('writes of min(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, args: [files('tests/writes-of-min-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of min(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, args: [files('tests/writes-of-min-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of PIPE_BUF bytes (stdout)', bash, args: [files('tests/writes-of-PIPE_BUF-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of PIPE_BUF bytes (stderr)', bash, args: [files('tests/writes-of-PIPE_BUF-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('writes of BUFSIZ bytes (stdout)', bash, args: [files('tests/writes-of-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of BUFSIZ bytes (stderr)', bash, args: [files('tests/writes-of-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of max(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, args: [files('tests/writes-of-max-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('writes of max(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, args: [files('tests/writes-of-max-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron write 0 bytes (stdout)', bash, args: [files('tests/cron-write-0-bytes-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write 0 bytes (stderr)', bash, args: [files('tests/cron-write-0-bytes-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron writes of 1 byte (stdout)', bash, args: [files('tests/cron-writes-of-1-byte-stdout.sh')], env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('cron writes of 1 byte (stderr)', bash, args: [files('tests/cron-writes-of-1-byte-stderr.sh')], env: test_env, depends: all_test_txt, timeout: 180, should_fail: xfail and xfail_writes_of_1_byte)
test('cron writes of min(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, args: [files('tests/cron-writes-of-min-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of min(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, args: [files('tests/cron-writes-of-min-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of PIPE_BUF bytes (stdout)', bash, args: [files('tests/cron-writes-of-PIPE_BUF-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of PIPE_BUF bytes (stderr)', bash, args: [files('tests/cron-writes-of-PIPE_BUF-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_small_writes)
test('cron writes of BUFSIZ bytes (stdout)', bash, args: [files('tests/cron-writes-of-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of BUFSIZ bytes (stderr)', bash, args: [files('tests/cron-writes-of-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of max(PIPE_BUF, BUFSIZ) bytes (stdout)', bash, args: [files('tests/cron-writes-of-max-PIPE_BUF-BUFSIZ-bytes-stdout.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)
test('cron writes of max(PIPE_BUF, BUFSIZ) bytes (stderr)', bash, args: [files('tests/cron-writes-of-max-PIPE_BUF-BUFSIZ-bytes-stderr.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_large_writes)

if has_generic_sendfile
	test('sendfile (stdout)', bash, args: [files('tests/sendfile-stdout.sh')], env: test_env, depends: all_test_txt)
endif
test('stream 1MB (stdout)', bash, args: [files('tests/stream-1MB.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_1mb)
test('stream 32MB (stdout)', bash, args: [files('tests/stream-32MB.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_32mb)
test('stream 256MB (stdout)', bash, args: [files('tests/stream-256MB.sh')], env: test_env, depends: all_test_txt, should_fail: xfail and xfail_stream_256mb)

test('debug options (none)', bash, args: [files('tests/options-none.sh')], env: test_env, depends: all_test_txt)
test('debug options (cron)', bash, args: [files('tests/options-cron.sh')], env: test_env, depends: all_test_txt)
test('debug options (invalid short options)', bash, args: [files('tests/options-invalid-short.sh')], env: test_env, depends: all_test_txt)
test('debug options (invalid short options with arguments)', bash, args: [files('tests/options-invalid-short-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (invalid long options)', bash, args: [files('tests/options-invalid-long.sh')], env: test_env, depends: all_test_txt)
test('debug options (invalid long options with arguments)', bash, args: [files('tests/options-invalid-long-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (1 file)', bash, args: [files('tests/options-1-file.sh')], env: test_env, depends: all_test_txt)
test('debug options (2 files)', bash, args: [files('tests/options-2-files.sh')], env: test_env, depends: all_test_txt)
test('debug options (3 files)', bash, args: [files('tests/options-3-files.sh')], env: test_env, depends: all_test_txt)
test('debug options (command with no arguments)', bash, args: [files('tests/options-cmd-noargs.sh')], env: test_env, depends: all_test_txt)
test('debug options (command with 1 argument)', bash, args: [files('tests/options-cmd-1-arg.sh')], env: test_env, depends: all_test_txt)
test('debug options (command with 2 arguments)', bash, args: [files('tests/options-cmd-2-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (command with 3 arguments)', bash, args: [files('tests/options-cmd-3-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (command stops following options)', bash, args: [files('tests/options-cmd-opts.sh')], env: test_env, depends: all_test_txt)
test('debug options (obscure boost syntax)', bash, args: [files('tests/options-cmd-obscure-boost-syntax.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command with no arguments)', bash, args: [files('tests/options-cmd-empty-noargs.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command with 1 argument)', bash, args: [files('tests/options-cmd-empty-1-arg.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command with 2 arguments)', bash, args: [files('tests/options-cmd-empty-2-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command with 3 arguments)', bash, args: [files('tests/options-cmd-empty-3-args.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty filename and empty command)', bash, args: [files('tests/options-mixed-empty1.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty filename and empty command with empty argument)', bash, args: [files('tests/options-mixed-empty2.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command stops following options)', bash, args: [files('tests/options-mixed-empty3.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command stops following options even when empty)', bash, args: [files('tests/options-mixed-empty4.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command stops following options)', bash, args: [files('tests/options-mixed-empty5.sh')], env: test_env, depends: all_test_txt)
test('debug options (empty command stops following options)', bash, args: [files('tests/options-mixed-empty6.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options)', bash, args: [files('tests/options-mixed-split1.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options even with immediate opts)', bash, args: [files('tests/options-mixed-split2.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options at the start)', bash, args: [files('tests/options-cmd-split1.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options at the start even with empty command)', bash, args: [files('tests/options-cmd-split2.sh')], env: test_env, depends: all_test_txt)
test('debug options (end of options signifier stops following options at the start even with empty command and empty argument)', bash, args: [files('tests/options-cmd-split3.sh')], env: test_env, depends: all_test_txt)

test('command not found', bash, args: [files('tests/command-not-found.sh')], env: test_env, depends: all_test_txt)
test('command not executable', bash, args: [files('tests/command-not-executable.sh')], env: test_env, depends: all_test_txt)
test('command exit code 42', bash, args: [files('tests/command-exit-code.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 1', bash, args: [files('tests/command-sighup.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 2', bash, args: [files('tests/command-sigint.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 2 (ignore interrupts)', bash, args: [files('tests/command-sigint-ignore-sigint.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 2 (self kill failure)', bash, args: [files('tests/command-sigint-self-kill-failure.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 3', bash, args: [files('tests/command-sigquit.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 9', bash, args: [files('tests/command-sigkill.sh')], env: test_env, depends: all_test_txt)
test('command terminated by signal 15', bash, args: [files('tests/command-sigterm.sh')], env: test_env, depends: all_test_txt)

test('cron command not found', bash, args: [files('tests/cron-command-not-found.sh')], env: test_env, depends: all_test_txt)
test('cron command not executable', bash, args: [files('tests/cron-command-not-executable.sh')], env: test_env, depends: all_test_txt)
test('cron command exit code 0 (stderr)', bash, args: [files('tests/cron-command-exit-code-0-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron command exit code 42 (stderr)', bash, args: [files('tests/cron-command-exit-code-42-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron command exit code 0 (no stderr)', bash, args: [files('tests/cron-command-exit-code-0-no-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron command exit code 42 (no stderr)', bash, args: [files('tests/cron-command-exit-code-42-no-stderr.sh')], env: test_env, depends: all_test_txt)
test('cronty command exit code 0 (stderr)', bash, args: [files('tests/cronty-command-exit-code-0-stderr.sh')], env: test_env, depends: all_test_txt)
test('cronty command exit code 42 (stderr)', bash, args: [files('tests/cronty-command-exit-code-42-stderr.sh')], env: test_env, depends: all_test_txt)
test('cronty command exit code 0 (no stderr)', bash, args: [files('tests/cronty-command-exit-code-0-no-stderr.sh')], env: test_env, depends: all_test_txt)
test('cronty command exit code 42 (no stderr)', bash, args: [files('tests/cronty-command-exit-code-42-no-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 1', bash, args: [files('tests/cron-command-sighup.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 2', bash, args: [files('tests/cron-command-sigint.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 2 (ignore interrupts)', bash, args: [files('tests/cron-command-sigint-ignore-sigint.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 2 (self kill failure)', bash, args: [files('tests/cron-command-sigint-self-kill-failure.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 3 (coredump)', bash, args: [files('tests/cron-command-sigquit-coredump.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 3 (no coredump)', bash, args: [files('tests/cron-command-sigquit-no-coredump.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 9', bash, args: [files('tests/cron-command-sigkill.sh')], env: test_env, depends: all_test_txt)
test('cron command terminated by signal 15', bash, args: [files('tests/cron-command-sigterm.sh')], env: test_env, depends: all_test_txt)

test('output file not openable', bash, args: [files('tests/file-open-failure.sh')], env: test_env, depends: all_test_txt)
test('output file not writeable (stdout)', bash, args: [files('tests/file-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('output file not writeable (stderr)', bash, args: [files('tests/file-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('output file not writeable (combined)', bash, args: [files('tests/file-write-failure-combined.sh')], env: test_env, depends: all_test_txt)

test('cron normal output file not openable', bash, args: [files('tests/cron-file-open-failure-normal.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (stdout)', bash, args: [files('tests/cron-file-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (stderr)', bash, args: [files('tests/cron-file-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file not writeable (combined)', bash, args: [files('tests/cron-file-write-failure-combined.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not openable', bash, args: [files('tests/cron-file-open-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not writeable', bash, args: [files('tests/cron-file-write-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not readable', bash, args: [files('tests/cron-file-read-failure-cron.sh')], env: test_env, depends: all_test_txt)
test('cron temporary output file not seekable', bash, args: [files('tests/cron-file-seek-failure-cron.sh')], env: test_env, depends: all_test_txt)

test('output file writes fail and recover', bash, args: [files('tests/file-write-failure-recovery.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file writes fail and recover (exit code 0)', bash, args: [files('tests/cron-file-write-failure-recovery-exit-0.sh')], env: test_env, depends: all_test_txt)
test('cron normal output file writes fail and recover (exit code 42)', bash, args: [files('tests/cron-file-write-failure-recovery-exit-42.sh')], env: test_env, depends: all_test_txt)

test('input temporary directory create failure', bash, args: [files('tests/input-temp-dir-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket create failure (input)', bash, args: [files('tests/input-socket-create-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket create failure (stdout)', bash, args: [files('tests/input-socket-create-failure-2.sh')], env: test_env, depends: all_test_txt)
test('input socket create failure (stderr)', bash, args: [files('tests/input-socket-create-failure-3.sh')], env: test_env, depends: all_test_txt)
test('input socket create and output file open failure', bash, args: [files('tests/input-and-output-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket bind failure (input)', bash, args: [files('tests/input-socket-bind-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket bind failure (stdout)', bash, args: [files('tests/input-socket-bind-failure-2.sh')], env: test_env, depends: all_test_txt)
test('input socket bind failure (stderr)', bash, args: [files('tests/input-socket-bind-failure-3.sh')], env: test_env, depends: all_test_txt)
test('input socket connect failure (stdout)', bash, args: [files('tests/input-socket-connect-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket connect failure (stderr)', bash, args: [files('tests/input-socket-connect-failure-2.sh')], env: test_env, depends: all_test_txt)
test('input socket receive failure', bash, args: [files('tests/input-socket-recv-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket shutdown failure (stdout)', bash, args: [files('tests/input-socket-shutdown-failure.sh')], env: test_env, depends: all_test_txt)
test('input socket shutdown failure (stderr)', bash, args: [files('tests/input-socket-shutdown-failure-2.sh')], env: test_env, depends: all_test_txt)

test('cron input temporary directory create failure', bash, args: [files('tests/cron-input-temp-dir-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket create failure', bash, args: [files('tests/cron-input-socket-create-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket create and output file open failure', bash, args: [files('tests/cron-input-and-output-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket bind failure', bash, args: [files('tests/cron-input-socket-bind-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket connect failure', bash, args: [files('tests/cron-input-socket-connect-failure.sh')], env: test_env, depends: all_test_txt)
test('cron input socket receive failure', bash, args: [files('tests/cron-input-socket-recv-failure.sh')], env: test_env, depends: all_test_txt)

test('input socket rcvbuf large enough', bash, args: [files('tests/input-socket-rcvbuf-large-enough.sh')], env: test_env, depends: all_test_txt)
test('input socket rcvbuf minimum size', bash, args: [files('tests/input-socket-rcvbuf-minimum-size.sh')], env: test_env, depends: all_test_txt)
test('input socket rcvbuf too small', bash, args: [files('tests/input-socket-rcvbuf-too-small.sh')], env: test_env, depends: all_test_txt)

test('input socket message from unknown source', bash, args: [files('tests/input-socket-extra-source.sh')], env: test_env, depends: all_test_txt)
test('input socket receive buffer too small', bash, args: [files('tests/input-socket-recv-buffer-too-small.sh')], env: test_env, depends: all_test_txt)

test('report arguments and file descriptors', bash, args: [files('tests/report.sh')], env: test_env, depends: all_test_txt)
test('report arguments and file descriptors (no stdin)', bash, args: [files('tests/report-no-stdin.sh')], env: test_env, depends: all_test_txt)
test('report arguments and file descriptors (extra fd)', bash, args: [files('tests/report-extra-fd.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors', bash, args: [files('tests/cron-report.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors (no stdin)', bash, args: [files('tests/cron-report-no-stdin.sh')], env: test_env, depends: all_test_txt)
test('cron report arguments and file descriptors (extra fd)', bash, args: [files('tests/cron-report-extra-fd.sh')], env: test_env, depends: all_test_txt)

test('dtee killed by signal 1', bash, args: [files('tests/command-kill-ppid-sighup.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 2', bash, args: [files('tests/command-kill-ppid-sigint.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 2 (ignore interrupts)', bash, args: [files('tests/command-kill-ppid-sigint-ignore-sigint.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 3', bash, args: [files('tests/command-kill-ppid-sigquit.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 9', bash, args: [files('tests/command-kill-ppid-sigkill.sh')], env: test_env, depends: all_test_txt)
test('dtee killed by signal 15', bash, args: [files('tests/command-kill-ppid-sigterm.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 1', bash, args: [files('tests/cron-command-kill-ppid-sighup.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 1 (self kill failure)', bash, args: [files('tests/cron-command-kill-ppid-sighup-self-kill-failure.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 2', bash, args: [files('tests/cron-command-kill-ppid-sigint.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 2 (ignore interrupts)', bash, args: [files('tests/cron-command-kill-ppid-sigint-ignore-sigint.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 2 (self kill failure)', bash, args: [files('tests/cron-command-kill-ppid-sigint-self-kill-failure.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 3', bash, args: [files('tests/cron-command-kill-ppid-sigquit.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 9', bash, args: [files('tests/cron-command-kill-ppid-sigkill.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 15', bash, args: [files('tests/cron-command-kill-ppid-sigterm.sh')], env: test_env, depends: all_test_txt)
test('cron dtee killed by signal 15 (self kill failure)', bash, args: [files('tests/cron-command-kill-ppid-sigterm-self-kill-failure.sh')], env: test_env, depends: all_test_txt)

test('fork failure', bash, args: [files('tests/fork-failure.sh')], env: test_env, depends: all_test_txt)
test('dup2 failure', bash, args: [files('tests/dup2-failure.sh')], env: test_env, depends: all_test_txt)
test('socket close failure (child)', bash, args: [files('tests/socket-close-failure-child.sh')], env: test_env, depends: all_test_txt)
test('socket close failure (parent)', bash, args: [files('tests/socket-close-failure-parent.sh')], env: test_env, depends: all_test_txt)
test('cron fork failure', bash, args: [files('tests/cron-fork-failure.sh')], env: test_env, depends: all_test_txt)
test('cron dup2 failure', bash, args: [files('tests/cron-dup2-failure.sh')], env: test_env, depends: all_test_txt)
test('cron socket close failure (child)', bash, args: [files('tests/cron-socket-close-failure-child.sh')], env: test_env, depends: all_test_txt)
test('cron socket close failure (parent)', bash, args: [files('tests/cron-socket-close-failure-parent.sh')], env: test_env, depends: all_test_txt)

test('waitpid failure (no children)', bash, args: [files('tests/waitpid-failure-no-children.sh')], env: test_env, depends: all_test_txt)
test('waitpid failure (no change)', bash, args: [files('tests/waitpid-failure-no-change.sh')], env: test_env, depends: all_test_txt)
test('cron waitpid failure (no children)', bash, args: [files('tests/cron-waitpid-failure-no-children.sh')], env: test_env, depends: all_test_txt)
test('cron waitpid failure (no change)', bash, args: [files('tests/cron-waitpid-failure-no-change.sh')], env: test_env, depends: all_test_txt)

if host_machine.system() != 'cygwin'
	# Assume Cygwin is insecure because the underlying Unix socket implementation is UDP
	test('security recreate the input socket after the parent process exits', bash, args: [files('tests/security-recreate-input-socket.sh')], env: test_env, depends: all_test_txt)
	test('security recreate the output socket after the child process exits', bash, args: [files('tests/security-recreate-output-socket.sh')], env: test_env, depends: all_test_txt)
endif

test('pipe write failure (stdout)', bash, args: [files('tests/pipe-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('pipe write failure (stderr)', bash, args: [files('tests/pipe-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('pipe write failure (file)', bash, args: [files('tests/pipe-write-failure-file.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stdout)', bash, args: [files('tests/cron-pipe-write-failure-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stderr)', bash, args: [files('tests/cron-pipe-write-failure-stderr.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (stderr but no output)', bash, args: [files('tests/cron-pipe-write-failure-stderr2.sh')], env: test_env, depends: all_test_txt)
test('cron pipe write failure (file)', bash, args: [files('tests/cron-pipe-write-failure-file.sh')], env: test_env, depends: all_test_txt)

test('command writes to stdout/stderr after dtee exits', bash, args: [files('tests/command-write-after-ppid-exit.sh')], env: test_env, depends: all_test_txt)

if host_machine.system() != 'cygwin'
	# These don't work on Cygwin because the FIFO can't have multiple readers/writers
	test('interrupted by signal during write', bash, args: [files('tests/interrupt-write.sh')], env: test_env, depends: all_test_txt)
	test('interrupted by signal during write (no SA_RESTART)', bash, args: [files('tests/interrupt-write-no-restart.sh')], env: test_env, depends: all_test_txt)
	test('cron interrupt by signal during write', bash, args: [files('tests/cron-interrupt-write.sh')], env: test_env, depends: all_test_txt)
	test('cron interrupt by signal during write (no SA_RESTART)', bash, args: [files('tests/cron-interrupt-write-no-restart.sh')], env: test_env, depends: all_test_txt)
endif

test('write truncated (stdout)', bash, args: [files('tests/write-truncate-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write truncated (stdout)', bash, args: [files('tests/cron-write-truncate-stdout.sh')], env: test_env, depends: all_test_txt)
test('cron write truncated (cron)', bash, args: [files('tests/cron-write-truncate-cron.sh')], env: test_env, depends: all_test_txt)
