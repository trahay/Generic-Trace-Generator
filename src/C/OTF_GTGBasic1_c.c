#include <stdio.h>
#include <string.h>

#include "OTF_GTGBasic1.h"

/* Root name */
static char*  filename;

int OTFInitTrace   (const char* filenam){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddProcType   (const char* alias, const char* contType, 
                       const char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddProcTypeNB (const char* alias, const char* contType, 
                       const char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddStateType   (const char* alias, const char* contType, 
                        const char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddStateTypeNB (const char* alias, const char* contType, 
                        const char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEventType   (const char* alias, const char* contType, 
                        const char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEventTypeNB (const char* alias, const char* contType, 
                        const char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddLinkType   (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddLinkTypeNB (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddVarType   (const char* alias   , const char* name,
                      const char* contType){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddVarTypeNB (const char* alias   , const char* name,
                      const char* contType){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEntityValue   (const char* alias, const char* entType, 
                          const char* name , const char* color){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEntityValueNB (const char* alias, const char* entType, 
                          const char* name , const char* color){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddContainer   (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name, const char* file){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddContainerNB (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name, const char* file){
  return TRACE_ERR_NOT_IMPL;
}


int OTFSeqAddContainer   (varPrec time, const char* alias    ,
                           const char*  type, const char* container,
                           const char*  name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSeqAddContainerNB (varPrec time, const char* alias    ,
                           const char*  type, const char* container,
                           const char*  name){
  return TRACE_ERR_NOT_IMPL;
}


int OTFDestroyContainer     (varPrec time, const char*  name,
                              const char*  type){
  return TRACE_ERR_NOT_IMPL;
}

int OTFDestroyContainerNB   (varPrec time, const char*  name,
                              const char*  type){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetState   (varPrec time, const char* type,
                    const char*  cont, const char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFPushState   (varPrec time, const char* type,
                     const char*  cont, const char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFPushStateNB (varPrec time, const char* type,
                     const char*  cont, const char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFPopState   (varPrec time, const char* type,
                    const char*  cont, const char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFPopStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEvent   (varPrec time, const char* type,
                    const char*  cont, const char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEventNB (varPrec time, const char* type,
                    const char*  cont, const char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFStartLink   (varPrec time, const char* type,
                     const char*   cont, const char* src,
                     const char*   val , const char* key){
  return TRACE_ERR_NOT_IMPL;
}

int OTFStartLinkNB (varPrec time, const char* type,
                     const char*   cont, const char* src,
                     const char*   val , const char* key){
  return TRACE_ERR_NOT_IMPL;
}

int OTFEndLink   (varPrec time, const char* type,
                   const char*   cont, const char* dest,
                   const char*   val , const char* key){
  return TRACE_ERR_NOT_IMPL;
}

int OTFEndLinkNB (varPrec time, const char* type,
                   const char*   cont, const char* dest,
                   const char*   val , const char* key){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSubVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSubVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFEndTrace (){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetCompress (int i){
  return TRACE_ERR_NOT_IMPL;
}
