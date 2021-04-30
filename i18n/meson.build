i18n = import('i18n')
i18n.gettext(meson.project_name(),
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

test_languages = [
	['en', 'en_GB'],
	['en_US', 'en_US'],
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
