#! /usr/bin/env python

import sys
import os
import re
import textwrap

# we are called from the src directory directly, use path relative to
# our location, we sit in templates/
mypath = os.path.dirname(os.path.abspath(__file__))
abs_top_srcdir = os.path.dirname(mypath)
# add our own jinja2 module to the system module load path
sys.path.insert(1, os.path.join(abs_top_srcdir, 'vendor', 'python', 'site-packages'))
from jinja2 import Environment, FileSystemLoader

env = Environment(
    loader=FileSystemLoader(mypath),
    line_statement_prefix='#tmpl',
    line_comment_prefix='###',
    block_start_string='{#',
    block_end_string='#}',
    variable_start_string='##',
    variable_end_string='##',
    comment_start_string='/*#',
    comment_end_string='#*/',
    whitespace_re=r'[ \t]'
)

def jinja2filter(env):
    def make_filter(f):
        assert f.__name__[:7] == 'filter_', 'Function should start with filter_'
        env.filters[f.__name__[7:]] = f
        return f
    return make_filter

@jinja2filter(env)
def filter_prefix_path(values, prefix=' '):
    if len(values) == 0:
        return ''
    return prefix + prefix.join(values)

execfile(os.path.join(mypath, 'templates.py'))

tmpl = env.get_template(os.path.basename(sys.argv[1]))
out = open(sys.argv[2], 'w')

tmpl.stream({
    'common_template_deps': common_template_deps,
    'templates':            templates
}).dump(out)
out.close()
