
#include <string.h>

#include <string>
#include <vector>
#include <map>

#include <otf2/otf2.h>

//@ class, which holds all definitions
class Otf2Definitions
{
public:
    Otf2Definitions( OTF2_Reader* reader );
    ~Otf2Definitions();
    //@ There are definitions, which don't have an Id. for example the TimeRange
    //@ collect them all into an vector
    @otf2 for def in defs|global_defs|not_referable_defs|not_supplement_defs:
    struct @@def.name@@
    {
        @otf2 for attr in def.attributes:
        @otf2  if attr is array_attr:
        @otf2   for array_attr in attr.array_attributes:
        std::vector< @@array_attr.type@@ > @@array_attr.name@@;
        @otf2   endfor
        @otf2  else:
        @@attr.type@@ @@attr.name@@;
        @otf2  endif
        @otf2 endfor
    };
    typedef std::vector< @@def.name@@* > @@def.name@@T;
    typedef @@def.name@@T::iterator @@def.name@@IT;
    typedef @@def.name@@T::value_type @@def.name@@V;
    @otf2  if def is not supplement_def:
    @@def.name@@T m_@@def.name@@s;
    @otf2  endif

    @otf2 endfor
    //@ supplement definitions to super definitions.
    @otf2 for def in defs|global_defs|supplement_defs:
    struct @@def.name@@
    {
        @otf2 for attr in def.attributes[1:]:
        @otf2  if attr is array_attr:
        @otf2   for array_attr in attr.array_attributes:
        std::vector< @@array_attr.type@@ > @@array_attr.name@@;
        @otf2   endfor
        @otf2  else:
        @@attr.type@@ @@attr.name@@;
        @otf2  endif
        @otf2 endfor
    };
    typedef std::vector< @@def.name@@* > @@def.name@@T;
    typedef @@def.name@@T::iterator @@def.name@@IT;
    typedef @@def.name@@T::value_type @@def.name@@V;

    @otf2 endfor
    //@ special case the String def
    @otf2 with def = String
    typedef std::map< @@def.self_attr.type@@, std::string > StringT;
    typedef StringT::iterator StringIT;
    typedef StringT::value_type StringV;
    StringT m_Strings;
    @otf2 endwith

    @otf2 for def in defs|global_defs|shared_defs|first_born_defs|referable_defs|exclude(String):
    enum @@def.ref_name@@Type
    {
        @otf2 for sibling in def.siblings:
        @@sibling.upper@@,
        @otf2 endfor
    };
    struct @@def.ref_name@@
    {
        @@def.ref_name@@Type @@def.ref_lname@@Type;
    };
    typedef std::map< @@def.self_attr.type@@, @@def.ref_name@@* > @@def.ref_name@@T;
    typedef @@def.ref_name@@T::iterator   @@def.ref_name@@IT;
    typedef @@def.ref_name@@T::value_type @@def.ref_name@@V;
    @@def.ref_name@@T m_@@def.ref_name@@s;

    @otf2 endfor
    //@ normal definitions have an id, as the first attribute
    //@ special case the String def
    @otf2 for def in defs|global_defs|referable_defs|exclude(String):
    struct @@def.name@@
    @otf2 if def is shared_def:
        : @@def.ref_name@@
    @otf2 endif
    {
        @otf2 for attr in def.attributes[1:]:
        @otf2 if attr is array_attr:
        @otf2   for array_attr in attr.array_attributes:
        std::vector< @@array_attr.type@@ > @@array_attr.name@@;
        @otf2   endfor
        @otf2  else:
        @@attr.type@@ @@attr.name@@;
        @otf2  endif
        @otf2 endfor
        @otf2 for supp_def in def.supplements:
        @@supp_def.name@@T @@supp_def.lname@@s;
        @otf2 endfor
    };
    @otf2 if def is not shared_def:
    typedef std::map< @@def.self_attr.type@@, @@def.name@@* > @@def.name@@T;
    typedef @@def.name@@T::iterator @@def.name@@IT;
    typedef @@def.name@@T::value_type @@def.name@@V;
    @@def.name@@T m_@@def.name@@s;
    @otf2 endif

    @otf2 endfor
    @otf2 for def in defs|global_defs:
    OTF2_CallbackCode
    HandleDef@@def.name@@( @@def.funcargs(leading_comma=False)@@ );

    @otf2 endfor
private:
    OTF2_Reader* m_Reader;
};

//@ callbacks for defs without id
@otf2 for def in defs|global_defs|not_referable_defs|not_supplement_defs:
OTF2_CallbackCode
Otf2Definitions::HandleDef@@def.name@@( @@def.funcargs(leading_comma=False)@@ )
{
    @@def.name@@& new_@@def.lower@@ = *( new @@def.name@@ );

    @otf2 for attr in def.attributes:
    @otf2  if attr is array_attr:
    @otf2   for array_attr in attr.array_attributes:
    std::vector< @@array_attr.type@@ > @@array_attr.name@@_vector( @@array_attr.name@@, @@array_attr.name@@ + @@attr.name@@ );
    new_@@def.lower@@.@@array_attr.name@@ = @@array_attr.name@@_vector;
    @otf2   endfor
    @otf2  else:
    new_@@def.lower@@.@@attr.name@@ = @@attr.name@@;
    @otf2  endif
    @otf2 endfor

    m_@@def.name@@s.push_back( &new_@@def.lower@@ );

    return OTF2_CALLBACK_SUCCESS;
}

@otf2 endfor
@otf2 for def in defs|global_defs|supplement_defs:
OTF2_CallbackCode
Otf2Definitions::HandleDef@@def.name@@( @@def.funcargs(leading_comma=False)@@ )
{
    @@def.parent_def.name@@IT parent_it = m_@@def.parent_def.name@@s.find( @@def.attributes[0].name@@ );
    @@def.name@@& new_@@def.lower@@ = *( new @@def.name@@ );

    @otf2 for attr in def.attributes[1:]:
    @otf2  if attr is array_attr:
    @otf2   for array_attr in attr.array_attributes:
    std::vector< @@array_attr.type@@ > @@array_attr.name@@_vector( @@array_attr.name@@, @@array_attr.name@@ + @@attr.name@@ );
    new_@@def.lower@@.@@array_attr.name@@ = @@array_attr.name@@_vector;
    @otf2   endfor
    @otf2  else:
    new_@@def.lower@@.@@attr.name@@ = @@attr.name@@;
    @otf2  endif
    @otf2 endfor

    parent_it->second->@@def.lname@@s.push_back( &new_@@def.lower@@ );

    return OTF2_CALLBACK_SUCCESS;
}

@otf2 endfor
@otf2 with def = String

OTF2_CallbackCode
Otf2Definitions::HandleDefString( @@def.self_attr.type@@ stringID,
                                  const char*            string )
{
    m_Strings[ stringID ] = std::string( string );

    return OTF2_CALLBACK_SUCCESS;
}

@otf2 endwith
//@ callbacks for defs with id
@otf2 for def in defs|global_defs|referable_defs|exclude(String):
OTF2_CallbackCode
Otf2Definitions::HandleDef@@def.name@@( @@def.funcargs(leading_comma=False)@@ )
{
    @@def.name@@& new_@@def.lower@@ = *( new @@def.name@@ );

    @otf2 if def is shared_def:
    new_@@def.lower@@.@@def.ref_lname@@Type = @@def.upper@@;

    @otf2 endif
    @otf2 for attr in def.attributes[1:]:
    @otf2  if attr is array_attr:
    @otf2   for array_attr in attr.array_attributes:
    std::vector< @@array_attr.type@@ > @@array_attr.name@@_vector( @@array_attr.name@@, @@array_attr.name@@ + @@attr.name@@ );
    new_@@def.lower@@.@@array_attr.name@@ = @@array_attr.name@@_vector;
    @otf2   endfor
    @otf2  else:
    new_@@def.lower@@.@@attr.name@@ = @@attr.name@@;
    @otf2  endif
    @otf2 endfor

    @otf2 if def is shared_def:
    m_@@def.ref_name@@s[ @@def.self_attr.name@@ ] = &new_@@def.lower@@;
    @otf2 else:
    m_@@def.name@@s[ @@def.self_attr.name@@ ] = &new_@@def.lower@@;
    @otf2 endif

    return OTF2_CALLBACK_SUCCESS;
}

@otf2 endfor

extern "C" {

@otf2 for def in defs|global_defs:

static OTF2_CallbackCode
@@def.lower@@_forward_cb( void* userData @@def.funcargs()@@ )
{
    Otf2Definitions* p_this = ( Otf2Definitions* )userData;

    return p_this->HandleDef@@def.name@@( @@def.callargs(leading_comma=False)@@ );
}

@otf2 endfor

} // extern "C"

Otf2Definitions::Otf2Definitions( OTF2_Reader* reader ) :
    m_Reader( reader )
{
    OTF2_GlobalDefReader* def_reader = OTF2_Reader_GetGlobalDefReader( reader );

    OTF2_GlobalDefReaderCallbacks* def_callbacks = OTF2_GlobalDefReaderCallbacks_New();

    @otf2 for def in defs|global_defs:
    OTF2_GlobalDefReaderCallbacks_Set@@def.name@@Callback( def_callbacks, @@def.lower@@_forward_cb );
    @otf2 endfor

    OTF2_Reader_RegisterGlobalDefCallbacks( reader, def_reader, def_callbacks, this );

    uint64_t definitions_read = 0;
    OTF2_Reader_ReadAllGlobalDefinitions( reader, def_reader, &definitions_read );

    OTF2_GlobalDefReaderCallbacks_Delete( def_callbacks );

    OTF2_Reader_CloseGlobalDefReader( reader,
                                      def_reader );
}

Otf2Definitions::~Otf2Definitions()
{
    @otf2 for def in defs|global_defs|not_referable_defs|not_supplement_defs:
    for ( @@def.name@@IT it = m_@@def.name@@s.begin();
          it != m_@@def.name@@s.end();
          it++ )
    {
        delete *it;
    }

    @otf2 endfor
    //@ normal definitions have an id, as the first attribute
    @otf2 for def in defs|global_defs|referable_defs|not_shared_defs|exclude(String):
    for ( @@def.name@@IT it = m_@@def.name@@s.begin();
          it != m_@@def.name@@s.end();
          it++ )
    {
        @otf2 for supp_def in def.supplements:
        for ( @@supp_def.name@@IT sit = it->second->@@supp_def.lname@@s.begin();
              sit != it->second->@@supp_def.lname@@s.end();
              sit++ )
        {
            delete *sit;
        }
        @otf2 endfor
        delete it->second;
    }

    @otf2 endfor
    //@ shared definitions have an id, as the first attribute
    @otf2 for def in defs|global_defs|shared_defs|first_born_defs|referable_defs|exclude(String):
    for ( @@def.ref_name@@IT it = m_@@def.ref_name@@s.begin();
          it != m_@@def.ref_name@@s.end();
          it++ )
    {
        @@def.ref_name@@* super = it->second;
        switch ( super->@@def.ref_lname@@Type )
        {
            @otf2 for sibling in def.siblings:
            case @@sibling.upper@@:
                delete reinterpret_cast<@@sibling.name@@*>( super );
                break;

            @otf2 endfor
        }
    }

    @otf2 endfor
}
