#include "GTGBasic1.h"
#include "OTF_GTGBasic1.h"
#include "paje_GTGBasic1.h"

static int traceType;

void setTraceType (int type){
    traceType = type;
}
int getTraceType (){
    return traceType;
}
int initTrace   (char* filename){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
        return pajeSeqInitTrace (filename);
        break;
    case VITE :
        return pajeInitTrace (filename);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFInitTrace (filename);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int setCompress (int val){
    switch (traceType){
#ifdef BUILD_OTF
    case OTF :
        return OTFSetCompress (val);
            break;
#endif
    case PAJE :
    case TAU :
    default :
        return TRACE_SUCCESS;
    }
    return TRACE_SUCCESS;
}

int addProcType   (char* alias, char* contType, 
                   char* name){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddProcType (alias, contType, name);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddProcType (alias, contType, name);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addProcTypeNB (char* alias, char* contType, 
                   char* name){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddProcTypeNB (alias, contType, name);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddProcTypeNB (alias, contType, name);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addStateType   (char* alias, char* contType, 
                    char* name){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddStateType (alias, contType, name);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddStateType (alias, contType, name);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addStateTypeNB (char* alias, char* contType, 
                    char* name){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddStateTypeNB (alias, contType, name);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddStateTypeNB (alias, contType, name);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addEventType   (char* alias, char* contType, 
                    char* name){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddEventType (alias, contType, name);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddEventType (alias, contType, name);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addEventTypeNB (char* alias, char* contType, 
                    char* name){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddEventTypeNB (alias, contType, name);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddEventTypeNB (alias, contType, name);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addLinkType   (char* alias   , char* name,
                   char* contType, char* srcContType,
                   char* destContType){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddLinkType (alias, name, contType, srcContType, destContType);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddLinkType (alias, name, contType, srcContType, destContType);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addLinkTypeNB (char* alias   , char* name,
                   char* contType, char* srcContType,
                   char* destContType){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddLinkTypeNB (alias, name, contType, srcContType, destContType);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddLinkTypeNB (alias, name, contType, srcContType, destContType);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addVarType   (char* alias   , char* name,
                  char* contType){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddVarType (alias, contType, name);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddVarType (alias, contType, name);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addVarTypeNB (char* alias   , char* name,
                  char* contType){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddVarTypeNB (alias, contType, name);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddVarTypeNB (alias, contType, name);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addEntityValue   (char* alias, char* entType, 
                      char* name , char* color){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddEntityValue (alias, entType, name, color);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddEntityValue (alias, entType, name, color);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addEntityValueNB (char* alias, char* entType, 
                      char* name , char* color){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddEntityValueNB (alias, entType, name, color);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddEntityValueNB (alias, entType, name, color);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addContainer   (varPrec time, char* alias    ,
                    char*  type, char* container,
                    char*  name, char* file){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
        return pajeSeqAddContainer (time, alias, type, container, name);
        break;
    case VITE :
        return pajeAddContainer (time, alias, type, container, name, file);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddContainer (time, alias, type, container, name, file);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addContainerNB (varPrec time, char* alias    ,
                    char*  type, char* container,
                    char*  name, char* file){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
        return pajeSeqAddContainerNB (time, alias, type, container, name);
        break;
    case VITE :
        return pajeAddContainerNB (time, alias, type, container, name, file);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddContainerNB (time, alias, type, container, name, file);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int destroyContainer     (varPrec time, char*  name,
                          char*  type){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeDestroyContainer (time, name, type);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFDestroyContainer (time, name, type);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int destroyContainerNB   (varPrec time, char*  name,
                          char*  type){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeDestroyContainerNB (time, name, type);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFDestroyContainerNB (time, name, type);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int setState   (varPrec time, char* type,
                char*  cont, char* val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeSetState (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFSetState (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int setStateNB (varPrec time, char* type,
                char*  cont, char* val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeSetStateNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFSetStateNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int pushState   (varPrec time, char* type,
                 char*  cont, char* val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajePushState (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFPushState (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int pushStateNB (varPrec time, char* type,
                 char*  cont, char* val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajePushStateNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFPushStateNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int popState   (varPrec time, char* type,
                const char*  cont, char* val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajePopState (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFPopState (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int popStateNB (varPrec time, char* type,
                char*  cont, char* val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajePopStateNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFPopStateNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addEvent   (varPrec time, char* type,
                char*  cont, char* val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddEvent (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddEvent (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addEventNB (varPrec time, char* type,
                char*  cont, char* val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddEventNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddEventNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int startLink   (varPrec time, char* type,
                 const char*  cont, const char* src,
                 char*  dest, char* val,
                 char* key){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeStartLink (time, type, cont, src, val, key);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFStartLink (time, type, cont, src, val, key);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int startLinkNB (varPrec time, char* type,
                 char*  cont, char* src,
                 char*  dest, char* val,
                 char* key){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeStartLinkNB (time, type, cont, src, val, key);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFStartLinkNB (time, type, cont, src, val, key);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int endLink   (varPrec time, char* type,
               const char*  cont, const char* src,
               const char*  dest, char* val,
               char* key){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeEndLink (time, type, cont, dest, val, key);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFEndLink (time, type, cont, dest, val, key);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int endLinkNB (varPrec time, char* type,
               char*  cont, char* src,
               char*  dest, char* val,
               char* key){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeEndLinkNB (time, type, cont, dest, val, key);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFEndLinkNB (time, type, cont, dest, val, key);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int setVar   (varPrec time, char*  type,
              char*  cont, varPrec val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeSetVar (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFSetVar (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int setVarNB (varPrec time, char*  type,
              char*  cont, varPrec val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeSetVarNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFSetVarNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addVar   (varPrec time, char*  type,
              char*  cont, varPrec val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddVar (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddVar (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int addVarNB (varPrec time, char*  type,
              char*  cont, varPrec val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddVarNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddVarNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int subVar   (varPrec time, char*  type,
              char*  cont, varPrec val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeSubVar (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFSubVar (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int subVarNB (varPrec time, char*  type,
              char*  cont, varPrec val){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeSubVarNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFSubVarNB (time, type, cont, val);
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}

int endTrace (){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeEndTrace ();
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFEndTrace ();
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return TRACE_SUCCESS;
}


