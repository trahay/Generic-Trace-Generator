#include "pajeColor.h"
#include "GTGBasic1.h"
#include "OTF_GTGBasic1.h"
#include "paje_GTGBasic1.h"

/* verbose !=0 means debugging mode */
int verbose = 0;
static traceType_t traceType;
/* Bufferized or not bufferized */
static int mode;
/* Flags that should be used */
static gtg_flag_t gtg_flags;

char* getName (int procNbr){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeGetName (procNbr);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }
    return "0";
}

void setTraceType (traceType_t type){
    char* res;
    traceType = type;
    res = getenv("GTG_VERBOSE");
    if(res)
        verbose = 1;
}

traceType_t getTraceType (){
    return traceType;
}

int bufferedModeActivated() {
    return !(gtg_flags & GTG_FLAG_NOTBUF);
}

trace_return_t initTrace   (const char* filename, int rank, gtg_flag_t flags){
    /* first, let's initialize gtg */
    gtg_color_init();

    gtg_flags = flags;

    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
      return pajeInitTrace (filename, rank, flags, FMT_PAJE);
        break;
    case VITE :
      return pajeInitTrace (filename, rank, flags, FMT_VITE);
        break;
#endif	/* BUILD_PAJE */
#ifdef BUILD_OTF
    case OTF :
      return OTFInitTrace (filename, flags);
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
      return pajeSetCompress(val);
      break;
    case TAU :
    default :
        return TRACE_SUCCESS;
    }
    return TRACE_SUCCESS;
}

trace_return_t addContType   (const char* alias, const char* contType, 
                              const char* name){
        switch (traceType){
#ifdef BUILD_PAJE
        case PAJE :
        case VITE :
            return pajeAddContType (alias, contType, name);
            break;
#endif
#ifdef BUILD_OTF
        case OTF :
            return OTFAddContType (alias, contType, name);
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

trace_return_t popState   (varPrec time, const char* type,
                const char*  cont){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajePopState (time, type, cont);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFPopState (time, type, cont);
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
        return OTFStartLink (time, type, src, dest, val, key);
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
        return OTFEndLink (time, type, src, dest, val, key);
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

trace_return_t endTrace (){
    int ret = TRACE_ERR_CLOSE;

    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
        ret = pajeEndTrace ();
        break;
    case VITE :
        ret = viteEndTrace ();
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        ret = OTFEndTrace ();
        break;
#endif
#ifdef BUILD_TAU
    case TAU :
#endif
    default :
        break;
    }

    gtg_color_exit();

    return TRACE_SUCCESS;
}


