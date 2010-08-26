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
int initTrace   (const char* filename){
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

int addProcType   (const char* alias, const char* contType, 
                   const char* name){
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

int addProcTypeNB (const char* alias, const char* contType, 
                   const char* name){
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

int addStateType   (const char* alias, const char* contType, 
                    const char* name){
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

int addStateTypeNB (const char* alias, const char* contType, 
                    const char* name){
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

int addEventType   (const char* alias, const char* contType, 
                    const char* name){
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

int addEventTypeNB (const char* alias, const char* contType, 
                    const char* name){
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

int addLinkType   (const char* alias   , const char* name,
                   const char* contType, const char* srcContType,
                   const char* destContType){
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

int addLinkTypeNB (const char* alias   , const char* name,
                   const char* contType, const char* srcContType,
                   const char* destContType){
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

int addVarType   (const char* alias   , const char* name,
                  const char* contType){
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

int addVarTypeNB (const char* alias   , const char* name,
                  const char* contType){
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

int addEntityValue   (const char* alias, const char* entType, 
                      const char* name , const char* color){
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

int addEntityValueNB (const char* alias, const char* entType, 
                      const char* name , const char* color){
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

int addContainer   (varPrec time, const char* alias    ,
                    const char*  type, const char* container,
                    const char*  name, const char* file){
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

int addContainerNB (varPrec time, const char* alias    ,
                    const char*  type, const char* container,
                    const char*  name, const char* file){
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

int destroyContainer     (varPrec time, const char*  name,
                          const char*  type){
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

int destroyContainerNB   (varPrec time, const char*  name,
                          const char*  type){
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

int setState   (varPrec time, const char* type,
                const char*  cont, const char* val){
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

int setStateNB (varPrec time, const char* type,
                const char*  cont, const char* val){
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

int pushState   (varPrec time, const char* type,
                 const char*  cont, const char* val){
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

int pushStateNB (varPrec time, const char* type,
                 const char*  cont, const char* val){
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

int popState   (varPrec time, const char* type,
                const char*  cont, const char* val){
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

int popStateNB (varPrec time, const char* type,
                const char*  cont, const char* val){
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

int addEvent   (varPrec time, const char* type,
                const char*  cont, const char* val){
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

int addEventNB (varPrec time, const char* type,
                const char*  cont, const char* val){
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

int startLink   (varPrec time, const char* type,
                 const char*  cont, const char* src,
                 const char*  dest, const char* val,
                 const char* key){
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

int startLinkNB (varPrec time, const char* type,
                 const char*  cont, const char* src,
                 const char*  dest, const char* val,
                 const char* key){
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

int endLink   (varPrec time, const char* type,
               const char*  cont, const char* src,
               const char*  dest, const char* val,
               const char* key){
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

int endLinkNB (varPrec time, const char* type,
               const char*  cont, const char* src,
               const char*  dest, const char* val,
               const char* key){
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

int setVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val){
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

int setVarNB (varPrec time, const char*  type,
              const char*  cont, varPrec val){
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

int addVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val){
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

int addVarNB (varPrec time, const char*  type,
              const char*  cont, varPrec val){
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

int subVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val){
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

int subVarNB (varPrec time, const char*  type,
              const char*  cont, varPrec val){
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


