import sys
import os
import os.path
import re
import textwrap
import copy

from __version__ import __version__, __version_info__

__all__ = [
    'Type',
    'CompressedType',
    'EnumEntry',
    'Enum',
    'GeneralEnum',
    'DefEnum',
    'EventEnum',
    'Attribute',
    'ArrayAttribute',
    'DefRefAttribute',
    'SelfRefAttribute',
    'EnumAttribute',
    'Record',
    'DefRecord',
    'LocalOnlyDefRecord',
    'GlobalOnlyDefRecord',
    'EventRecord',
    'Version',
    '__version__',
    '__version_info__',
    'types',
    'enums',
    'defs',
    'events',
    'snaps',
    'attic_enums'
]

class streqobject(object):
    """Object which can be compared against a string."""
    def __eq__(self, other):
        if isinstance(other, basestring):
            return str(self) == other
        else:
            return id(self) == id(other)

class klist(list):
    """List where the entries can also be accessed by string."""
    def __init__(self, *args, **kwargs):
        list.__init__(self, *args, **kwargs)
        self._n2i = {}

    def __getitem__(self, key):
        if isinstance(key, basestring):
            return self[self.index(key)]
        else:
            return list.__getitem__(self, key)

types = klist()
enums = klist()
defs = klist()
events = klist()
snaps = klist()
attic_enums = klist()

_camel_case_name_re = re.compile(r'([A-Z]+[a-z0-9]*)*$')
_lower_camel_case_name_re = re.compile(r'[a-z]+([A-Z]+[a-z0-9]*)*$')

_mypath = os.path.dirname(os.path.abspath(__file__))

class Version(object):
    def __init__(self, major=1, minor=0):
        self.major = major
        self.minor = minor

    def number(self):
        return (self.major << 8) + self.minor

    def __str__(self):
        return '%s.%s' % (self.major, self.minor)

    def __eq__(self, other):
        return (self.major, self.minor) == (other.major, other.minor)

    def __ne__(self, other):
        return (self.major, self.minor) != (other.major, other.minor)

    def __lt__(self, other):
        return (self.major, self.minor) <  (other.major, other.minor)

    def __le__(self, other):
        return (self.major, self.minor) <= (other.major, other.minor)

    def __gt__(self, other):
        return (self.major, self.minor) >  (other.major, other.minor)

    def __ge__(self, other):
        return (self.major, self.minor) <= (other.major, other.minor)

    def __hash__(self):
        return hash(self.major) * hash(self.minor)

class Type(streqobject):
    def __init__(self, c_type, otf2_type, public_name='', **kwargs):
        self.c     = c_type
        self.otf2  = otf2_type
        self.lower = otf2_type.lower()

        for key in kwargs:
            setattr(self, key, kwargs[key])

        # provide us as the c_type in the global symbols
        self.name = self.c
        if public_name is not None:
            if public_name:
                self.name = public_name
            globals()[self.name] = self
        self.lname = self.name[0].lower() + self.name[1:]

        types.append(self)

    def size(self, attr):
        return 'sizeof( %s )' % (self.c,)

    def __str__(self):
        return str(self.c)

class _TypeAttributeFactory(object):
    def __init__(self, type, lname):
        self.type = type
        self.lname = lname

    def __call__(self, doc, name=None, **kwargs):
        if name is None:
            name = self.lname
        return _TypeAttribute(doc, self.type, name, **kwargs)

class CompressedType(Type):
    def __init__(self, c_type, otf2_type, public_name='', attribute=None):
        Type.__init__(self, c_type, otf2_type, public_name)
        self.errorcheck_on_read = 'Invalid compression size.'
        if attribute:
            attr_name = attribute + 'Attribute'
            globals()[attr_name] = _TypeAttributeFactory(self,
                    attribute[0].lower() + attribute[1:])
            __all__.append(attr_name)

    def size(self, attr):
        return '%s + 1' % (Type.size(self, attr),)

class StringType(Type):
    def __init__(self):
        Type.__init__(self, 'const char*', 'String', 'string_t')
        self.errorcheck_on_argument = 'Invalid string argument.'
        self.errorcheck_on_read = 'Can\'t read string from buffer.'

    def size(self, attr):
        return 'strlen( %s ) + 1' % (attr.name,)

class IdMapType(Type):
    def __init__(self):
        Type.__init__(self, 'const OTF2_IdMap*', 'IdMap', 'OTF2_IdMap')
        self.errorcheck_on_argument = 'Invalid IdMap argument.'
        self.errorcheck_on_read = 'Can\'t read IdMap.'

    def size(self, attr):
        return 'otf2_id_map_get_size( %s )' % (attr.name,)

execfile(os.path.join(_mypath, 'otf2.types'))

class EnumAttributeFactory(object):
    def __init__(self, enum):
        self.enum = enum

    def __call__(self, doc, name=None, **kwargs):
        if name is None:
            name = self.enum.lname
        if self.enum.attic \
                and ('attic' not in kwargs or kwargs['attic'] is None):
            raise AttributeError('Enum is attic since version %d.%d, but used as record attribute not in attic.' % (self.enum.attic.major, self.enum.attic.minor))
        return EnumAttribute(doc, self.enum, name, **kwargs)

class EnumEntry(object):
    def __init__(self, doc, name, value=None, since=None, **kwargs):
        self.doc = textwrap.dedent(doc)
        if len(self.doc) and self.doc[-1] == '\n':
            self.doc = self.doc[:-1]
        self.name = name
        self.suffix = name
        self.value = value
        self.since = since

        for key in kwargs:
            setattr(self, key, kwargs[key])

        globals()[self.name] = self

    def prefixlen(self, length):
        self.suffix = self.name[length:]

    def doxygendoc(self, prefix=' *  ', width=70):
        pars = []
        for par in re.split('\\n{2,}', self.doc):
            if not par:
                continue
            pars.append(textwrap.fill(par,
                    width=width,
                    initial_indent='',
                    subsequent_indent=prefix))
        return ('\n' + prefix + '\n' + prefix).join(pars)

    def __str__(self):
        return str(self.name)

class Enum(streqobject):
    def __init__(self, doc, name, *entries, **kwargs):
        self.since = kwargs.pop('since', Version())
        self.attic = kwargs.pop('attic', None)
        self.bitset_like = kwargs.pop('bitset_like', False)
        base_type = kwargs.pop('type', None)
        public_name = kwargs.pop('public_name', '')
        terminator = kwargs.pop('terminator', None)
        includes = kwargs.pop('includes', [])

        if _camel_case_name_re.match(name) is None:
            raise ValueError('Invalid enum name %s.' % (name,))

        # check for duplicate record name
        if name in enums:
            raise ValueError('A record/enum of name %s already exists.' % (self.name,))

        self.doc = textwrap.dedent(doc)
        if len(self.doc) and self.doc[-1] == '\n':
            self.doc = self.doc[:-1]
        self.name = name
        self.entries = []
        self.prefix = None
        if not base_type:
            base_type = uint8_t
        assert isinstance(base_type, Type), 'Invalid value type of \'type\' argument: %r' % (base_type,)
        c_orig = base_type.c
        self.type = copy.copy(base_type)
        self.type.c = 'OTF2_' + self.name
        setattr(self.type, 'c_orig', c_orig)
        assert isinstance(self.since, Version), 'Invalid value type for @since: %r' % (self.since,)
        self.latest_since = self.since

        for key in kwargs:
            setattr(self, key, kwargs[key])

        # convert camel-case to underscore
        res = re.subn(r'([A-Z]+)', r'_\1', self.name)
        # remove first _
        res = re.subn(r'^_', '', res[0])

        self.upper = res[0].upper()
        self.lower = res[0].lower()
        self.lname = self.name[0].lower() + self.name[1:]

        ids = set((-1,))
        for entry in entries:
            assert isinstance(entry, EnumEntry), 'Not of type EnumEntry'
            if entry.value is None:
                entry.value = max(ids) + 1
            assert entry.value not in ids, 'Duplicate enum value %s' % (entry.value,)
            if entry.since is None:
                entry.since = self.since
            if self.latest_since > entry.since:
                raise AttributeError('Enum entries must be added in monotonical increasing @since order: %s > %s' % (self.latest_since, entry.since))
            if self.latest_since < entry.since:
                self.latest_since = entry.since
            if self.since < entry.since:
                entry.doc += '\n@since Version %s.' % (entry.since,)
            ids.add(entry.value)
            self.entries.append(entry)

        self.prefix = os.path.commonprefix([x.name for x in self.entries])
        # prefix should be empty, or last charachter is an '_'
        if self.prefix != '' and self.prefix[-1] != '_':
            last = self.prefix.rfind('_')
            if last == -1:
                self.prefix = ''
            else:
                self.prefix = self.prefix[0:last+1]

        # includes get the common prefix
        for enum_name in includes:
            assert enum_name in enums, 'Unknown enum in include statement: %s' % (enum_name,)
            enum = enums[enum_name]
            for entry in enum.entries:
                new_entry = EnumEntry(
                    """""",
                    self.prefix + entry.suffix,
                    max(ids) + 1)
                ids.add(new_entry.value)
                self.entries.append(new_entry)

        # set the suffix name for all entries
        for entry in self.entries:
            entry.prefixlen(len(self.prefix))

        if terminator is not None and terminator is not True:
            assert isinstance(terminator, EnumEntry), 'Terminator is not of type EnumEntry'
        self.terminator = terminator

        if self.terminator is True:
            assert len(ids) > 1, 'Auto terminator without other entries.'
            self.terminator = EnumEntry(
                'Max entry.',
                self.prefix + 'MAX',
                max(ids) + 1)

        if self.terminator is not None:
            self.terminator.prefixlen(len(self.prefix))

        attr_name = name + 'Attribute'
        globals()[attr_name] = EnumAttributeFactory(self)
        __all__.append(attr_name)

        if not self.attic:
            enums.append(self)
        else:
            attic_enums.append(self)

    def need_comma(self, entry):
        if self.terminator is not None or entry is not self.entries[-1]:
            return ','
        else:
            return ''

    def doxygendoc(self, prefix=' *  ', width=70):
        pars = []
        for par in re.split('\\n{2,}', self.doc):
            if not par:
                continue
            pars.append(textwrap.fill(par,
                    width=width,
                    initial_indent='',
                    subsequent_indent=prefix))
        return ('\n' + prefix + '\n' + prefix).join(pars)

    def __str__(self):
        return self.name

class GeneralEnum(Enum):
    def __init__(self, doc, name, *entries, **kwargs):
        Enum.__init__(self, doc, name, *entries, **kwargs)

class DefEnum(Enum):
    def __init__(self, doc, name, *entries, **kwargs):
        Enum.__init__(self, doc, name, *entries, **kwargs)

class EventEnum(Enum):
    def __init__(self, doc, name, *entries, **kwargs):
        Enum.__init__(self, doc, name, *entries, **kwargs)

execfile(os.path.join(_mypath, 'otf2.enums'))

class Attribute(object):
    """Represents an attribute for any record type."""

    def __init__(self, doc, type, name, **kwargs):
        self.attic = kwargs.pop('attic', None);
        self.since = kwargs.pop('since', None);
        self.doc = textwrap.dedent(doc)
        if len(self.doc) and self.doc[-1] == '\n':
            self.doc = self.doc[:-1]
        self.type = type
        self.set_name(name)

    def size(self):
        return self.type.size(self)

    def set_name(self, name):
        if _lower_camel_case_name_re.match(name) is None:
            raise ValueError('Invalid attribute name %s.' % (name,))

        self.name = name
        res = re.subn(r'([A-Z]+)', r'_\1', self.name)
        self.upper = res[0].upper()
        self.lower = res[0].lower()

    def doxygendoc(self, prefix=' *  ', width=70):
        pars = []
        for par in re.split('\\n{2,}', self.doc):
            if not par:
                continue
            pars.append(textwrap.fill(par,
                    width=width,
                    initial_indent='',
                    subsequent_indent=prefix))
        return ('\n' + prefix + '\n' + prefix).join(pars)

    def __str__(self):
        return str(self.name)

    def __repr__(self):
        return '<%s: %s>' % (self.__class__.__name__, self.name)

class ArrayAttribute(Attribute):
    """Represents an array attribute which has multiple child attributes for
    which this attribute is the count attribute."""

    def __init__(self, doc, type, name, *arrayAttributes, **kwargs):
        Attribute.__init__(self, doc, type, name, **kwargs)
        assert len(arrayAttributes) >= 1, 'Need at least one value attribute member'
        self.array_attributes = []
        for array_attr in arrayAttributes:
            assert not isinstance(array_attr, ArrayAttribute), 'Recursive array attributes are not support.'
            self.array_attributes.append(array_attr)

    def __repr__(self):
        return '<%s: %s[%s]>' % (
            self.__class__.__name__,
            self.name,
            ','.join([repr(x) for x in self.array_attributes]))

class _TypeAttribute(Attribute):
    def __init__(self, doc, type, name, **kwargs):
        Attribute.__init__(self, doc, type, name, **kwargs)

class DefRefAttribute(Attribute):
    """Represents an attribute for event or definition records, that reference
     a definition."""

    def __init__(self, doc, type, name, def_ref, **kwargs):
        self.mapping_type = kwargs.pop('mapping_type', None)
        Attribute.__init__(self, doc, type, name, **kwargs)
        self.def_ref = def_ref

    def __repr__(self):
        return '<%sAttribute: %s>' % (self.def_ref.name, self.name)

class SelfRefAttribute(object):
    """Represents an attribute for definition records, that references itself."""

    def __init__(self, doc, name=None, since=None):
        self.doc = doc
        self.name = name
        self.since = since

class TimeStampAttribute(Attribute):
    """\
    Represents an timestamp attribute for event records only.
    The clock offsets will be applied to timestamp attributes.
    """
    def __init__(self, doc, name, **kwargs):
        Attribute.__init__(self, doc, OTF2_TimeStamp, name, **kwargs)
        self.apply_clock_offset = True

class DefRefAttributeFactory(object):
    def __init__(self, type, def_ref, mapping_type=None):
        self.type = type
        self.def_ref = def_ref
        self.mapping_type = mapping_type

    def __call__(self, doc, name=None, **kwargs):
        if name is None:
            name = self.def_ref.lname
        return DefRefAttribute(doc, self.type, name,
                self.def_ref,
                mapping_type=self.mapping_type,
                **kwargs)

class EnumAttribute(Attribute):
    def __init__(self, doc, enum, name, **kwargs):
        self.enum = enum
        Attribute.__init__(self, doc, self.enum.type, name, **kwargs)

class Record(streqobject):
    def __init__(self, doc, id, name, since=Version(), global_name=None, **kwargs):
        if _camel_case_name_re.match(name) is None:
            raise ValueError('Invalid record name %s.' % (name,))

        self.id = id
        self.name = name
        assert isinstance(since, Version), 'Invalid value type for @since: %r' % (since,)
        self.since = since
        self.deprecated = kwargs.pop('deprecated', None)
        assert self.deprecated is None or isinstance(self.deprecated, Version), 'Invalid value type for @deprecated: %r' % (self.deprecated,)
        self.versions = [self.since]
        self.max_attr_len = 0

        for key in kwargs:
            setattr(self, key, kwargs[key])

        # convert camel-case to underscore
        res = re.subn(r'([A-Z]+)', r'_\1', name)
        # remove first _
        res = re.subn(r'^_', '', res[0])

        self.upper = res[0].upper()
        self.lower = res[0].lower()
        self.lname = name[0].lower() + name[1:]
        self.doc = textwrap.dedent(doc)
        if len(self.doc) and self.doc[-1] == '\n':
            self.doc = self.doc[:-1]
        self.attribute_by_name = {}
        self.attributes = []
        self.all_attributes = []
        self.all_array_attributes = []
        self.record_attributes = []
        self.record_attributes_by_version = {}
        self.attic_attributes_by_version = {}

        if global_name is None:
            global_name = self.name
        globals()[global_name + 'Record'] = self

    def _addAttribute(self, attr):
        assert isinstance(attr, Attribute), 'Only accept attributes of type Attribute'
        if attr.since is not None and not isinstance(attr.since, Version):
            raise AttributeError('Invalid value type for @since: %r' % (attr.since,))
        if attr.attic is not None and not isinstance(attr.attic, Version):
            raise AttributeError('Invalid value type for @attic: %r' % (attr.attic,))
        if attr.since is None:
            attr.since = self.since
        if self.since < attr.since:
            attr.doc += '\nSince version %s.' % (attr.since,)
            if attr.since not in self.versions:
                self.versions.append(attr.since)
                self.versions.sort()
        if attr.name in self.attribute_by_name:
            raise ValueError('An attribute of this name already exist: %s (%s).' % (attr.name, ','.join(self.attribute_by_name)))
        self.attribute_by_name[attr.name] = attr
        self.record_attributes.append(attr)
        self.record_attributes.sort(key=lambda x: x.since)
        if attr.since not in self.record_attributes_by_version:
            self.record_attributes_by_version[attr.since] = []
        self.record_attributes_by_version[attr.since].append(attr)
        if not attr.attic:
            self.attributes.append(attr)
            self.all_attributes.append(attr)
        else:
            if attr.attic not in self.attic_attributes_by_version:
                self.attic_attributes_by_version[attr.attic] = []
            self.attic_attributes_by_version[attr.attic].append(attr)
        if len(attr.name) > self.max_attr_len:
            self.max_attr_len = len(attr.name)
        if isinstance(attr, ArrayAttribute):
            for array_attr in attr.array_attributes:
                if array_attr.since is not None:
                    raise AttributeError('Array attribute members should not have a @since added')
                array_attr.since = attr.since
                if self.since < attr.since:
                    attr.doc += '\nSince version %s.' % (attr.since,)
                if array_attr.name[-1:] != 's':
                    array_attr.set_name(array_attr.name + 's')
                if array_attr.name in self.attribute_by_name:
                    raise ValueError('An attribute of this name already exist: %s.' % (array_attr.name,))
                self.attribute_by_name[array_attr.name] = array_attr
                self.all_attributes.append(array_attr)
                self.all_array_attributes.append(array_attr)
                if len(array_attr.name) > self.max_attr_len:
                    self.max_attr_len = len(array_attr.name)

    def doxygendoc(self, prefix=' *  ', width=70):
        pars = []
        for par in re.split('\\n\\n', self.doc):
            if not par:
                continue
            pars.append(textwrap.fill(par,
                    width=width,
                    initial_indent='',
                    subsequent_indent=prefix))
        return ('\n' + prefix + '\n' + prefix).join(pars)

    def funcargs(self, case='name', prefix='', leading_comma=True):
        """\
        Generate an function prototype argument list.

        Empty for records without any attributes. Otherwise the attributes are
        separated by `,\n%(prefix)`, including a first comma if `leading_comma`
        is true.
        """

        if len(self.attributes) == 0:
            return ''

        args = []
        for attr in self.attributes:
            args.append('%s %s' % (attr.type, getattr(attr, case)))
            if isinstance(attr, ArrayAttribute):
                for array_attr in attr.array_attributes:
                    args.append('const %s* %s' % (array_attr.type, getattr(array_attr, case)))
        sep = ',\n%s' % (prefix,)
        if leading_comma:
            return sep + sep.join(args)
        else:
            return sep.join(args)

    def callargs(self, attributes=None, case='name', argprefix='', prefix='', leading_comma=True):
        """\
        Generates a function call argument list.
        
        Empty for records with no attributes. Otherwise the attributes are
        separated by `,\n%(prefix)`, including a first comma if `leading_comma`
        is true.
        """

        attrs = self.attributes
        if attributes is not None:
            attrs = attributes

        if len(attrs) == 0:
            return ''

        args = []
        for attr in attrs:
            args.append('%s%s' % (argprefix, getattr(attr, case)))
            if isinstance(attr, ArrayAttribute):
                for array_attr in attr.array_attributes:
                    args.append('%s%s' % (argprefix, getattr(array_attr, case)))
        sep = ',\n%s' % (prefix,)
        if leading_comma:
            return sep + sep.join(args)
        else:
            return sep.join(args)

    def printargs(self, case='name', argprefix='', prefix=''):
        """\
        print argument list without linebreaks.
        
        Empty for records with no attributes. Otherwise the attributes are
        separated by `%(prefix)`.
        """

        if len(self.attributes) == 0:
            return ''

        args = []
        for attr in self.all_attributes:
            args.append('%s%s' % (argprefix, getattr(attr, case)))
        sep = '%s ' % (prefix,)
        return sep + sep.join(args)


    def structargs(self, attributes=None, case='name', prefix='    '):
        """Generates the list of members of a struct for the attributes.
        (include Doxygen-style documentation)."""

        attrs = self.attributes
        if attributes is not None:
            attrs = attributes

        args = []
        for attr in attributes:
            args.append('%s %s;' % (attr.type, getattr(attr, case)))
            if isinstance(attr, ArrayAttribute):
                for array_attr in attr.array_attributes:
                    args.append('%s* %s;' % (array_attr.type, getattr(array_attr, case)))
        return ('\n%s' % (prefix,)).join(args)

    def doxygenargs(self, case='name', prefix=' *  '):
        """Generates doxygen-style @param list for the attributes."""
        args = []
        for attr in self.all_attributes:
            args.append(
                '\n'.join(
                    textwrap.wrap(
                        attr.doc,
                        width=77,
                        initial_indent='@param %-*s ' % (self.max_attr_len, getattr(attr, case)),
                        subsequent_indent='%s%s' % (prefix, ' ' * (8 + self.max_attr_len)))))
        return ('\n%s' % (prefix,)).join(args)

    def __str__(self):
        return str(self.name)

    def __repr__(self):
        return '<%s: %s(%s)>' % (
            self.__class__.__name__,
            self.name,
            ','.join([repr(x) for x in self.attributes]))

class DefRecord(Record):
    """\
    Declare a definition record.
    
    Attributes:
        self_attr: The attribute for the unique ID of this definition.
                   Maybe None if this definition does not have a unique ID.
    
        siblings: List of definitions, which share the same ID space.
                  If len(siblings) == 1 this definition is a singleton.
    
        supplements: List of definitions, which are a supplement to this one.
    """
    def __init__(self, doc, id, name, self_type, *attributes, **kwargs):
        mapping_type = kwargs.pop('mapping_type', None)
        self.ref_name = kwargs.pop('ref_name', True)

        # all children defs, which are supplementary defs to this one
        self.supplements = []

        # check for duplicate record name
        if name in defs:
            raise ValueError('A definition named %s already exists.' % (name,))

        Record.__init__(self, doc, id, name, **kwargs)

        # if we have a type, than add an implicit identifier attribute
        self.self_attr = None
        # list of defs which share the same ID space
        # if self.siblings[0] == self holds, self is the first born
        self.siblings = [self]
        if self_type:

            if isinstance(self_type, Record):
                assert isinstance(self_type, DefRecord), 'Only definition records allowed.'
                first_born = self_type
                # the referenced def sibling needs to be the first born
                assert first_born.siblings[0] == first_born, \
                    'The referenced definition sibling needs to be the first born.'
                first_born.siblings.append(self)
                self.siblings = first_born.siblings
                self.self_attr = first_born.self_attr
                self.ref_name = first_born.ref_name
                self.ref_upper = first_born.ref_upper
                self.ref_lower = first_born.ref_lower
                self.ref_lname = first_born.ref_lname
                self.self_attr = first_born.self_attr
                self_type = first_born.self_attr.type
            elif isinstance(self_type, CompressedType):
                self_type = copy.copy(self_type)

                # determine our name to reference this definition
                if self.ref_name is True:
                    self.ref_name  = self.name
                if self.ref_name is not None:
                    if _camel_case_name_re.match(self.ref_name) is None:
                        raise ValueError('Invalid record def ref name %s.' % (self.ref_name,))
                    self.ref_lname = self.ref_name[0].lower() + self.ref_name[1:]

                    # convert camel-case to underscore
                    res = re.subn(r'([A-Z]+)', r'_\1', self.ref_name)
                    # remove first _
                    res = re.subn(r'^_', '', res[0])

                    self.ref_upper = res[0].upper()
                    self.ref_lower = res[0].lower()

                    setattr(self_type, 'c_orig', self_type.c)
                    self_type.c = 'OTF2_' + self.ref_name + 'Ref'

                    if mapping_type:
                        setattr(mapping_type, 'def', self)

                    self.self_attr = DefRefAttribute(
                        'The unique identifier for this @ref %s definition.' % (self.name,),
                        self_type,
                        'self',
                        self,
                        mapping_type=mapping_type)

                    # provide a <name>Attribute class, so that other defs and events can
                    # reference us
                    attr_name = self.ref_name + 'Attribute'
                    globals()[attr_name] = DefRefAttributeFactory(self_type, self, mapping_type)
                    __all__.append(attr_name)
            else:
                raise TypeError('Unexpected type of parameter: %s' % (type(self_type),))

            # prepend to all other attributes
            self._addAttribute(self.self_attr)

        else:
            assert not mapping_type, 'Definition record with mapping_type, but no identifier attribute.'

        def add_def_ref_doc(attr):
            if isinstance(attr, DefRefAttribute):
                attr.doc += '\nReferences a @ref %s definition.' % (attr.def_ref.name,)

        # add all attributes, but catch SelfRefAttribute and resolve them
        # to our self
        for attr in attributes:
            assert not isinstance(attr, TimeStampAttribute), 'TimeStamp attribute used in definition record.'
            if isinstance(attr, SelfRefAttribute):
                assert self.self_attr is not None, 'Self referencing attribute without id.'
                if attr.name is None:
                    attr.name = 'parent'
                attr = DefRefAttribute(attr.doc, self_type, attr.name, self, since=attr.since)
            elif isinstance(attr, ArrayAttribute):
                new_array_attributes = []
                for array_attr in attr.array_attributes:
                    assert not isinstance(array_attr, TimeStampAttribute), 'TimeStamp attribute used in definition record.'
                    if isinstance(array_attr, SelfRefAttribute):
                        assert self.self_attr is not None, 'Self referencing attribute without id.'
                        if array_attr.name is None:
                            attr.name = 'parent'
                        array_attr = DefRefAttribute(array_attr.doc, self_type, array_attr.name, self, since=attr.since)
                    add_def_ref_doc(array_attr)
                    new_array_attributes.append(array_attr)
                attr.array_attributes = new_array_attributes
            add_def_ref_doc(attr)
            self._addAttribute(attr)

        defs.append(self)

class LocalOnlyDefRecord(DefRecord):
    def __init__(self, doc, id, name, *attributes, **kwargs):
        self_type = kwargs.pop('type', None)
        self.evt_reader_def_callback = kwargs.pop('evt_reader_def_callback', None)
        DefRecord.__init__(self, doc, id, name, self_type, *attributes, **kwargs)

class GlobalOnlyDefRecord(DefRecord):
    def __init__(self, doc, id, name, *attributes, **kwargs):
        self_type = kwargs.pop('type', None)
        DefRecord.__init__(self, doc, id, name, self_type, *attributes, **kwargs)

class SuppDefRecord(DefRecord):
    def __init__(self, doc, id, name, parent_def, *attributes, **kwargs):
        assert isinstance(parent_def, DefRecord), 'Parent must be a definition: %r' % (parent_def,)
        self.parent_def = parent_def
        assert self.parent_def.self_attr, 'Parent must be a referable definition: %r' % (self.parent_def,)
        new_attributes = [
                DefRefAttribute(
                        'Parent @ref %s definition to which this one is a supplementary definition.' % (self.parent_def.name,),
                        self.parent_def.self_attr.type,
                        self.parent_def.lname,
                        self.parent_def)] + list(attributes)
        DefRecord.__init__(self, doc, id, name, None, *new_attributes, **kwargs)
        self.parent_def.supplements.append(self)

execfile(os.path.join(_mypath, 'otf2.defs'))

class EventRecord(Record):
    def __init__(self, doc, id, name, *attributes, **kwargs):
        # check for duplicate record name
        if name in events:
            raise ValueError('A event named %s already exists.' % (name,))

        Record.__init__(self, doc, id, name, **kwargs)

        # do not permit event records after 1.1 to be singleton
        # they can't be handled when skipping records and there payload
        # is an UNDEFINED value (ie. 0xff) which conflicts
        # with the 0xff record length byte indicating a record
        # length >= 256
        self.singleton = len(attributes) == 1 and self.since <= Version(1, 1)
        for attr in attributes:
            if isinstance(attr, DefRefAttribute):
                if not attr.mapping_type:
                    raise ValueError('Definition reference attribute does not have a mapping type. Needed when used in event records.')
                if len(attr.def_ref.siblings) > 1:
                    defs = ', or a @ref '.join([x.name for x in attr.def_ref.siblings])
                else:
                    defs = attr.def_ref.name
                attr.doc += '\nReferences a @ref %s definition and will be mapped to the global definition if a mapping table of type @ref %s is available.' % (defs, attr.mapping_type)
            if isinstance(attr, ArrayAttribute):
                # having an array attribute destroys the singleton property
                self.singleton = False
            if not isinstance(attr.type, CompressedType):
                # having an attribute which will not be compressed destroys the
                # singleton property
                self.singleton = False
            self._addAttribute(attr)

        events.append(self)

execfile(os.path.join(_mypath, 'otf2.events'))

class SnapshotRecord(Record):
    def __init__(self, doc, id, name_or_origin, *attributes, **kwargs):
        since = kwargs.pop('since', Version(1,2))

        if isinstance(name_or_origin, EventRecord):
            assert len(attributes) == 0, 'No attributes allowed when cloning an event record.'
            derived_event = True
            name = name_or_origin.name
            self.snap_name = name_or_origin.name + 'Snap'
            attributes = copy.deepcopy(name_or_origin.attributes)
            setattr(name_or_origin, 'snapshot_record', self)
            self.origin = name_or_origin
            for attr in attributes:
                if attr.since < since:
                    attr.since = since
                if isinstance(attr, ArrayAttribute):
                    for array_attr in attr.array_attributes:
                        array_attr.since = None
        else:
            assert isinstance(name_or_origin, basestring), 'Expecting the name of the record.'
            derived_event = False
            name = name_or_origin
            self.snap_name = name_or_origin
            attributes = list(attributes)

        # check for duplicate record name
        if name in snaps:
            print>>sys.stderr, '%r' % snaps
            raise ValueError('A snapshot event named %s already exists.' % (name,))

        Record.__init__(self, doc, id, name,
                since=since, global_name=self.snap_name, **kwargs)

        if derived_event:
            attributes.insert(0, TimeStampAttribute(
                    """The original time this event happended.""",
                    'origEventTime'))

        for attr in attributes:
            self._addAttribute(attr)

        snaps.append(self)

execfile(os.path.join(_mypath, 'otf2.snaps'))
