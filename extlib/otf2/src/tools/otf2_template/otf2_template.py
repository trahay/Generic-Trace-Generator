#! @PYTHON@

import sys
import os
import re
import textwrap
import traceback

class Otf2Version(object):
    """Object to holds the OTF2 version information."""
    def __init__(self, major, minor, bugfix, suffix, version):
        self.major = major
        self.minor = minor
        self.bugfix = bugfix
        self.suffix = suffix
        self.version = version

    def __str__(self):
        return str(self.version)

prefix = '@prefix@'
exec_prefix = '@exec_prefix@'
datarootdir = '@datarootdir@'
package = '@PACKAGE@'
if prefix[1:-1] == 'prefix':
    try:
        abs_builddir = os.getcwd()
        mypath = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        abs_srcdir = os.path.dirname(mypath)
    except:
        print >>sys.stderr, "Can't determine build and source directory! Can only be called from a build dir."
        sys.exit(1)

    # add our own jinja2 module to the system module load path
    sys.path.insert(1, os.path.join(abs_srcdir, 'vendor', 'python', 'site-packages'))

    # add our otf2 module to the system module load path
    sys.path.insert(1, os.path.join(abs_srcdir, 'share'))

    # add our otf2 module from the build dir to the system module load path
    sys.path.insert(1, os.path.join(abs_builddir, 'share', 'otf2'))
    #print >>sys.stderr, "%s" % (':'.join([x for x in sys.path]))

else:
    # resolve ${exec_prefix} and ${prefix}
    if datarootdir[:14] == '${exec_prefix}':
        datarootdir = exec_prefix + datarootdir[14:]
    if datarootdir[:9] == '${prefix}':
        datarootdir = prefix + datarootdir[9:]

    # add our otf2 module to the system module load path
    sys.path.insert(1, os.path.join(datarootdir, package, 'python'))

import otf2
from jinja2 import Environment, FileSystemLoader
from jinja2.exceptions import TemplateError, TemplateNotFound, TemplateSyntaxError, UndefinedError

env = Environment(
    extensions=['jinja2.ext.with_'],
    loader=FileSystemLoader(['.', '/']),
    line_statement_prefix='@otf2',
    line_comment_prefix='//@',
    block_start_string='{@',
    block_end_string='@}',
    variable_start_string='@@',
    variable_end_string='@@',
    comment_start_string='/*@',
    comment_end_string='@*/',
    whitespace_re=r'[ \t]'
)

def jinja2filter(env):
    def make_filter(f):
        assert f.__name__[:7] == 'filter_', 'Function should start with filter_'
        env.filters[f.__name__[7:]] = f
        return f
    return make_filter

def jinja2test(env):
    def make_test(f):
        assert f.__name__[:5] == 'test_', 'Function should start with test_'
        env.tests[f.__name__[5:]] = f
        return f
    return make_test

@jinja2filter(env)
def filter_exclude(values, *excludes):
    s = set(excludes)
    return [v for v in values if v not in s]

@jinja2filter(env)
def filter_include(values, *includes):
    s = set(includes)
    return [v for v in values if v in s]

@jinja2test(env)
def test_snapshot_event(e):
    assert isinstance(e, otf2.EventRecord), 'Only event records allowed'
    return hasattr(e, 'snapshot_record') and e.snapshot_record

@jinja2filter(env)
def filter_snapshot_events(events):
    return [e for e in events if test_snapshot_event(e)]

@jinja2test(env)
def test_global_def(d):
    assert isinstance(d, otf2.DefRecord), 'Only definitions allowed.'
    return not isinstance(d, otf2.LocalOnlyDefRecord)

@jinja2filter(env)
def filter_global_defs(defs):
    return [d for d in defs if test_global_def(d)]

@jinja2filter(env)
def filter_not_global_defs(defs):
    return [d for d in defs if not test_global_def(d)]

@jinja2test(env)
def test_local_def(d):
    assert isinstance(d, otf2.DefRecord), 'Only definitions allowed.'
    return not isinstance(d, otf2.GlobalOnlyDefRecord)

@jinja2filter(env)
def filter_local_defs(defs):
    return [d for d in defs if test_local_def(d)]

@jinja2filter(env)
def filter_not_local_defs(defs):
    return [d for d in defs if not test_local_def(d)]

@jinja2test(env)
def test_first_born_def(d):
    assert isinstance(d, otf2.DefRecord), 'Only definitions allowed.'
    return d.siblings[0] == d

@jinja2filter(env)
def filter_first_born_defs(defs):
    return [d for d in defs if test_first_born_def(d)]

@jinja2filter(env)
def filter_not_first_born_defs(defs):
    return [d for d in defs if not test_first_born_def(d)]

@jinja2test(env)
def test_shared_def(d):
    assert isinstance(d, otf2.DefRecord), 'Only definitions allowed.'
    return len(d.siblings) > 1

@jinja2filter(env)
def filter_shared_defs(defs):
    return [d for d in defs if test_shared_def(d)]

@jinja2filter(env)
def filter_not_shared_defs(defs):
    return [d for d in defs if not test_shared_def(d)]

@jinja2test(env)
def test_referable_def(d):
    assert isinstance(d, otf2.DefRecord), 'Only definitions allowed.'
    return d.self_attr is not None

@jinja2filter(env)
def filter_referable_defs(defs):
    return [d for d in defs if test_referable_def(d)]

@jinja2filter(env)
def filter_not_referable_defs(defs):
    return [d for d in defs if not test_referable_def(d)]

@jinja2test(env)
def test_supplement_def(d):
    assert isinstance(d, otf2.DefRecord), 'Only definitions allowed.'
    return isinstance(d, otf2.SuppDefRecord)

@jinja2filter(env)
def filter_supplement_defs(defs):
    return [d for d in defs if test_supplement_def(d)]

@jinja2filter(env)
def filter_not_supplement_defs(defs):
    return [d for d in defs if not test_supplement_def(d)]

@jinja2test(env)
def test_bitset_enum(e):
    assert isinstance(e, otf2.Enum), 'Only enums allowed.'
    return e.bitset_like

@jinja2test(env)
def test_general_enum(e):
    assert isinstance(e, otf2.Enum), 'Only enums allowed.'
    return isinstance(e, otf2.GeneralEnum)

@jinja2filter(env)
def filter_general_enums(enums):
    return [e for e in enums if test_general_enum(e)]

@jinja2test(env)
def test_def_enum(e):
    assert isinstance(e, otf2.Enum), 'Only enums allowed.'
    return isinstance(e, otf2.DefEnum)

@jinja2filter(env)
def filter_def_enums(enums):
    return [e for e in enums if test_def_enum(e)]

@jinja2test(env)
def test_event_enum(e):
    assert isinstance(e, otf2.Enum), 'Only enums allowed.'
    return isinstance(e, otf2.EventEnum)

@jinja2filter(env)
def filter_event_enums(enums):
    return [e for e in enums if test_event_enum(e)]

@jinja2test(env)
def test_terminator(e):
    assert isinstance(e, otf2.Enum), 'Only enums allowed.'
    return hasattr(e, 'terminator') and e.terminator is not None

@jinja2test(env)
def test_array_attr(a):
    assert isinstance(a, otf2.Attribute), 'Only attributes allowed.'
    return isinstance(a, otf2.ArrayAttribute)

@jinja2test(env)
def test_def_ref_attr(a):
    assert isinstance(a, otf2.Attribute), 'Only attributes allowed.'
    return isinstance(a, otf2.DefRefAttribute)

@jinja2test(env)
def test_enum_attr(a):
    assert isinstance(a, otf2.Attribute), 'Only attributes allowed.'
    return isinstance(a, otf2.EnumAttribute)

@jinja2test(env)
def test_attic_attr(a):
    assert isinstance(a, otf2.Attribute), 'Only attributes allowed.'
    return hasattr(a, 'attic') and a.attic is not None

@jinja2test(env)
def test_singleton_event(e):
    assert isinstance(e, otf2.EventRecord), 'Only events allowed.'
    return hasattr(e, 'singleton') and e.singleton

@jinja2test(env)
def test_derived_snap(e):
    assert isinstance(e, otf2.SnapshotRecord), 'Only snapshots allowed.'
    return hasattr(e, 'origin')

tmpl_data = {
    'version': Otf2Version(
        otf2.__version_info__[0],
        otf2.__version_info__[1],
        otf2.__version_info__[2],
        otf2.__version_info__[3],
        otf2.__version__),
    'types': otf2.types,
    'enums': otf2.enums,
    'defs': otf2.defs,
    'events': otf2.events,
    'snaps': otf2.snaps,
    'attic': {
        'enums': otf2.attic_enums
    }
}
for e in otf2.types:
    tmpl_data[str(e)] = e
for e in otf2.enums:
    tmpl_data[str(e)] = e
for e in otf2.defs:
    tmpl_data[str(e)] = e
for e in otf2.events:
    tmpl_data[str(e)] = e
for e in otf2.snaps:
    tmpl_data[str(e)+'Snap'] = e

if '--help' in sys.argv \
        or '-h'  in sys.argv:
    print 'Usage: otf2-template TEMPLATE-FILE [OUTPUT-FILE]'
    print 'Template driver for the OTF2 library.'
    print
    print 'Report bugs to <%s>' % ('@PACKAGE_BUGREPORT@',)
    sys.exit(0)
elif '--version' in sys.argv \
        or '-V'  in sys.argv:
    print 'otf2-template: version %s' % (otf2.__version__,)
    sys.exit(0)

if len(sys.argv) not in (2, 3):
    print>>sys.stderr, 'otf2-template: unsupported number of arguments'
    print>>sys.stderr, 'Try \'otf2-template --help\' for more information.'
    sys.exit(1)

tmplfile = sys.argv[1]
tmplname = os.path.abspath(tmplfile)
try:
    tmpl = env.get_template(tmplname)
except TemplateNotFound, e:
    print>>sys.stderr, 'otf2-template: can\'t load template \'%s\': %s' % (tmplfile, e)
    sys.exit(1)
except TemplateSyntaxError, e:
    print>>sys.stderr, '%s:%d: error: %s' % (tmplfile, e.lineno, e.message)
    sys.exit(1)
except Exception, e:
    print>>sys.stderr, 'otf2-template: can\'t load template \'%s\': %s' % (tmplfile, e)
    sys.exit(1)

if len(sys.argv) == 3:
    try:
        out = open(sys.argv[2], 'w')
    except Exception, e:
        print>>sys.stderr, 'otf2-template: can\'t open output file \'%s\': %s' % (sys.argv[2], e,)
        sys.exit(1)
else:
    out = sys.stdout

try:
    tmpl.stream(tmpl_data).dump(out)
except TemplateSyntaxError, e:
    print>>sys.stderr, '%s:%d: %s' % (tmplfile, e.lineno, e.message)
    sys.exit(1)
except UndefinedError, e:
    for filename, lineno, func, text in traceback.extract_tb(sys.exc_info()[2]):
        if filename == tmplname:
            print>>sys.stderr, '%s: In %s:' % (tmplfile, func)
            print>>sys.stderr, '%s:%d: error: %s' % (tmplfile, lineno, e.message)
            sys.exit(1)
    print>>sys.stderr, '%s: %s' % (tmplfile, e.message)
    sys.exit(1)
except TemplateError, e:
    print>>sys.stderr, '%s: %s' % (tmplfile, e.message)
    sys.exit(1)
finally:
    if out != sys.stdout:
        out.close()
