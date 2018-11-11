# -*- coding: utf-8 -*-

needs_sphinx = '1.3'
extensions = []
source_suffix = ['.rst']

project = u'dtee'
copyright = u'2018, Simon Arlott'
author = u'Simon Arlott'

master_doc = 'index'
manual_doc = 'manual'

version = u''
release = u''

language = None
exclude_patterns = ['build', 'Thumbs.db', '.DS_Store']
pygments_style = 'sphinx'
todo_include_todos = False

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

html_static_path = ['_static']

linkcheck_timeout = 60
