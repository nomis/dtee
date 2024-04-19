test_env = [
	'LC_ALL=C',
	'LANG=C',
	'LANGUAGE=',
]

bash = find_program('bash').full_path()
xfail = get_option('test_expect_fail')

subdir('util')

subdir('basic')
subdir('command')
subdir('fork')
subdir('i18n')
subdir('input')
subdir('options')
subdir('output')
subdir('security')
subdir('stdio')
subdir('write')
