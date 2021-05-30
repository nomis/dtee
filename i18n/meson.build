i18n_package = meson.project_name()

import('i18n').gettext(
	i18n_package,
	args: [
		'--add-comments=i18n',
		'--add-location=file',
		'--boost',
		'--directory=' + meson.source_root(),
		'--from-code=UTF-8',
		'--keyword=N_',
		'--no-wrap',
		'--omit-header',
		'--sort-by-file',
	],
)

if host_machine.system() == 'linux' or host_machine.system() == 'gnu'
	gettext_deps = []
elif host_machine.system() == 'freebsd' or host_machine.system() == 'openbsd' or host_machine.system() == 'dragonfly'
	# Workaround for https://github.com/mesonbuild/meson/issues/4468
	gettext_deps = [cpp.find_library('intl', has_headers: ['libintl.h'], dirs: ['/usr/local/lib'], header_args: c_args)]
elif host_machine.system() == 'netbsd'
	# Workaround for https://github.com/mesonbuild/meson/issues/4468
	gettext_deps = [cpp.find_library('intl', has_headers: ['libintl.h'], dirs: ['/usr/pkg/lib'], header_args: c_args)]
else # darwin, cygwin
	gettext_deps = [cpp.find_library('intl', has_headers: ['libintl.h'])]
endif

test_languages = [
	['en', 'en_GB'],
	['en_US', 'en_US'],
	['fr', 'fr_FR'],
]

msgfmt = [find_program('msgfmt'), '@INPUT@', '-o', '@OUTPUT@']
all_test_i18n = {}
foreach language : test_languages
	all_test_i18n += {
		language[1]:
			custom_target('test-i18n-' + language[1],
				command: msgfmt,
				input: language[0] + '.po',
				output: 'test-' + language[1] + '.mo')
		}
endforeach
