#include "GTGBasic1.h"
#include "OTF_GTGBasic1.h"
#include "paje_GTGBasic1.h"
#include "pajeColor.h"

static traceType_t traceType;

void setTraceType (traceType_t type){
    traceType = type;
}

traceType_t getTraceType (){
    return traceType;
}

trace_return_t initTrace   (const char* filename, int rank){
	/* first, let's initialize gtg */
	gtg_color_init();

    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
        return pajeSeqInitTrace (filename);
        break;
    case VITE :
        return pajeInitTrace (filename, rank);
        break;
#endif	/* BUILD_PAJE */
#ifdef BUILD_OTF
    case OTF :
        return OTFInitTrace (filename);
        break;
#endif	/* BUILD_OTF */
#ifdef BUILD_TAU
    case TAU :
#endif	/* BUILD_TAU */
    default :
        break;
    }
    return TRACE_ERR_NOT_IMPL;
}

trace_return_t setCompress (int val){
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

trace_return_t addProcType   (const char* alias, const char* contType, 
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

trace_return_t addProcTypeNB (const char* alias, const char* contType, 
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

trace_return_t addStateType   (const char* alias, const char* contType, 
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

trace_return_t addStateTypeNB (const char* alias, const char* contType, 
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

trace_return_t addEventType   (const char* alias, const char* contType, 
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

trace_return_t addEventTypeNB (const char* alias, const char* contType, 
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

trace_return_t addLinkType   (const char* alias   , const char* name,
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

trace_return_t addLinkTypeNB (const char* alias   , const char* name,
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

trace_return_t addVarType   (const char* alias   , const char* name,
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

trace_return_t addVarTypeNB (const char* alias   , const char* name,
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

trace_return_t addEntityValue   (const char* alias, const char* entType, 
                      const char* name , gtg_color_t p_color){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
	    return pajeAddEntityValue (alias, entType, name, Paje_get_color(p_color));
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddEntityValue (alias, entType, name, OTF_get_color(p_color));
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

trace_return_t addEntityValueNB (const char* alias, const char* entType, 
                      const char* name , gtg_color_t p_color){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddEntityValueNB (alias, entType, name, Paje_get_color(p_color));
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
	return OTFAddEntityValueNB (alias, entType, name, OTF_get_color(p_color));
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

trace_return_t addContainer   (varPrec time, const char* alias    ,
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

trace_return_t addContainerNB (varPrec time, const char* alias    ,
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

trace_return_t destroyContainer     (varPrec time, const char*  name,
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

trace_return_t destroyContainerNB   (varPrec time, const char*  name,
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

trace_return_t setState   (varPrec time, const char* type,
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

trace_return_t setStateNB (varPrec time, const char* type,
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

trace_return_t pushState   (varPrec time, const char* type,
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

trace_return_t pushStateNB (varPrec time, const char* type,
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

trace_return_t popState   (varPrec time, const char* type,
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

trace_return_t popStateNB (varPrec time, const char* type,
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

trace_return_t addEvent   (varPrec time, const char* type,
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

trace_return_t addEventNB (varPrec time, const char* type,
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

trace_return_t startLink   (varPrec time, const char* type,
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

trace_return_t startLinkNB (varPrec time, const char* type,
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

trace_return_t endLink   (varPrec time, const char* type,
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

trace_return_t endLinkNB (varPrec time, const char* type,
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

trace_return_t setVar   (varPrec time, const char*  type,
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

trace_return_t setVarNB (varPrec time, const char*  type,
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

trace_return_t addVar   (varPrec time, const char*  type,
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

trace_return_t addVarNB (varPrec time, const char*  type,
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

trace_return_t subVar   (varPrec time, const char*  type,
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

trace_return_t subVarNB (varPrec time, const char*  type,
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

trace_return_t endTrace (){
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


