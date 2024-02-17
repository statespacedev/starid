"""use sphinx options -a -E"""
from starid.util.get_project_root import get_project_root
import sys
sys.path.append(get_project_root() + '/starid/')
sys.path.append(get_project_root() + '/docs/')

project = "starid"
copyright = "2024, noah smith"
author = "noah smith"

extensions = ["sphinx.ext.autodoc", "sphinx_rtd_theme", "sphinx.ext.napoleon", "sphinx.ext.todo"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]
source_suffix = ".rst"
master_doc = "index"
html_theme = "sphinx_rtd_theme"
autodoc_member_order = "bysource"
# templates_path = ["_templates"]
# html_static_path = ["_static"]
