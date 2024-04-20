# -*- coding: utf-8 -*-

import os

travis_ci = os.environ.get("TRAVIS") == 'true'
rtd = os.environ.get("READTHEDOCS") == 'True'

needs_sphinx = '4.1'
extensions = []
source_suffix = ['.rst']

project = u'dtee'
copyright = u'2018-2024, Simon Arlott'
author = u'Simon Arlott'

master_doc = 'index'
manual_doc = 'manual'

version = u''
release = u''

language = 'en'
exclude_patterns = ['build', 'Thumbs.db', '.DS_Store']
pygments_style = 'sphinx'
todo_include_todos = False

if rtd:
	html_theme = 'sphinx_rtd_theme'

man_pages = [
	(manual_doc, 'dtee', u'Run a program with standard output and standard error copied to files', [author], 1)
]

man_show_urls = True

texinfo_documents = [
	(manual_doc, 'dtee', u'dtee - Run a program with standard output and standard error copied to files',
		author, 'dtee', 'Run a program with standard output and standard error copied to files',
		'Miscellaneous'),
]

texinfo_show_urls = 'inline'

linkcheck_timeout = 60
linkcheck_ignore = [r'https://github.com/.+/.+/(compare|commits)/.+']
linkcheck_allowed_redirects = {
	'https://dtee.readthedocs.io/': 'https://dtee.readthedocs.io/en/latest/',
	'https://www.sphinx-doc.org/': 'https://www.sphinx-doc.org/en/master/'
}
