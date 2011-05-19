#include <stdio.h>
#include <assert.h>
#include "otf.h"


typedef struct {
    uint64_t count;
} HandlerArgument;



int handleDefProcess (void *userData, uint32_t stream, uint32_t process, const char *name, uint32_t parent) {

    fprintf( stdout, "      DefProcess %u '%s'\n", process, name );
    return OTF_RETURN_OK;
}

int handleDefFunction (void *userData, uint32_t stream, uint32_t func, const char *name, uint32_t funcGroup, uint32_t source) {

    fprintf( stdout, "      DefFunction %u '%s'\n", func, name );
    return OTF_RETURN_OK;
}


int main( int argc, char** argv ) {


    OTF_FileManager* manager;
    OTF_Reader* reader;
    OTF_HandlerArray* handlers;


    uint64_t ret;
    HandlerArgument ha;
    ha.count = 0;

    manager= OTF_FileManager_open( 100 );
    assert( manager );

    handlers = OTF_HandlerArray_open();
    assert( handlers );

    OTF_HandlerArray_setHandler( handlers, (OTF_FunctionPointer*) handleDefProcess, OTF_DEFPROCESS_RECORD );
    OTF_HandlerArray_setFirstHandlerArg( handlers, &ha, OTF_DEFPROCESS_RECORD );

    OTF_HandlerArray_setHandler( handlers, (OTF_FunctionPointer*) handleDefFunction, OTF_DEFFUNCTION_RECORD );
    OTF_HandlerArray_setFirstHandlerArg( handlers, &ha, OTF_DEFFUNCTION_RECORD );


    reader = OTF_Reader_open( "hello_world.otf", manager );
    assert( reader );


const char * txt= 
"DV1.7.0\"alpha\"\n"
"DTRf4240\n"
"DP1NM\"Process 1\"\n"
"DP2NM\"Process 2\"\n"
"DFG10NM\"standard functions\"\n"
"DFG11NM\"user functions\"\n"
"DF1G10NM\"main\"\n"
"DF2G11NM\"foo\"\n"
"DF3G11NM\"bar\"\n"
"DCG3fNM\"all my counters\"\n"
"DCNT5G3fNM\"my counter\"P0U\"my unit\"\n";


    OTF_RBuffer* newdefbuffer= OTF_RBuffer_open_with_external_buffer( strlen(txt), txt, 0 );

    OTF_RStream* defstream= OTF_Reader_getStream( reader, 0 );
    OTF_RBuffer* old= OTF_RStream_setDefBuffer( defstream, newdefbuffer );
    assert( NULL == old );

    ret= OTF_Reader_readDefinitions( reader, handlers );

    fprintf( stdout, " read %llu definition records\n", ret );

    OTF_Reader_close( reader );
    OTF_HandlerArray_close( handlers );
    OTF_FileManager_close( manager );

    return 0;
}















