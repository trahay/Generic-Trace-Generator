#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "GTG.h"
#include "GTGPaje.h"
#include "GTGBasic.h"
#include "GTGReplay.h"
#include "GTGOTF_Basic.h"
#include "GTGPaje_Basic.h"

/* verbose !=0 means debugging mode */
int verbose = 0;
static traceType_t traceType;

/* Flags that should be used */
gtg_flag_t gtg_flags;


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

    if( (flags & GTG_FLAG_USE_MPI) && (flags & GTG_FLAG_OUTOFORDER)) {
      fprintf(stderr, "Warning: you cannot record events out of order when using MPI\n");
      flags ^= GTG_FLAG_OUTOFORDER;
    }
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
#ifndef BUILD_OTF
(void) val;
#endif
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

#ifdef BUILD_OTF
  /* for OTF traces, adding a container consists in two steps:
   * 1 - defining the container (this appears in the trace headers)
   * 2 - starting the container (this appears in the body)
   * We need to define the container right now so that it can be refered to
   * before its start (for example a StartLink event may use this container
   * before its creation)
   */
  if (traceType == OTF){
        return OTFDefineContainer (alias, type, container, name, file);
  }
#endif

  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    /* if the application records events out of order, don't execute this function right now. */
    gtg_record(event_addContainer, time, alias, type, container, name, file);
    return TRACE_SUCCESS;
  }

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
        return OTFStartContainer (time, alias, type, container, name, file);
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
  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_destroyContainer, time, name, type);
    return TRACE_SUCCESS;
  }

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

  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_setState, time, type, cont, val);
    return TRACE_SUCCESS;
  }

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
  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_pushState, time, type, cont, val);
    return TRACE_SUCCESS;
  }

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
  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_popState, time, type, cont);
    return TRACE_SUCCESS;
  }

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
  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_addEvent, time, type, cont, val);
    return TRACE_SUCCESS;
  }

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

  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_startLink, time, type, cont, src, dest, val, key);
    return TRACE_SUCCESS;
  }


#ifndef BUILD_OTF
(void) dest;
#endif

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
  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_endLink, time, type, cont, src, dest, val, key);
    return TRACE_SUCCESS;
  }

#ifndef BUILD_OTF
(void) src;
#endif

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
  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_setVar, time, type, cont, val);
    return TRACE_SUCCESS;
  }

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
  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_addVar, time, type, cont, val);
    return TRACE_SUCCESS;
  }
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
  if(gtg_flags & GTG_FLAG_OUTOFORDER) {
    gtg_record(event_subVar, time, type, cont, val);
    return TRACE_SUCCESS;
  }
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

trace_return_t AddComment   (const char*  comment){
    switch (traceType){
#ifdef BUILD_PAJE
    case PAJE :
    case VITE :
        return pajeAddComment (comment);
        break;
#endif
#ifdef BUILD_OTF
    case OTF :
        return OTFAddComment (comment);
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

    if(gtg_flags & GTG_FLAG_OUTOFORDER) {
      gtg_write_events(-1);
    }

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

    return ret;
}

