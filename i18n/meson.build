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
