import os, sys
sys.path.insert(0, os.path.abspath('../..'))
project = 'Starid'
copyright = '2021, Noah Smith'
author = 'Noah Smith'
extensions = ['sphinx_rtd_theme', 'sphinx.ext.autodoc', 'sphinx.ext.coverage', 'sphinx.ext.napoleon']
templates_path = ['_templates']
exclude_patterns = []
html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
# release = '1.3.22'
