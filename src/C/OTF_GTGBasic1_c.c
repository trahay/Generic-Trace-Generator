#include <stdio.h>
#include <string.h>

#include "OTF_GTGBasic1.h"

/* Root name */
static char*  filename;

int OTFInitTrace   (char* filenam){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddProcType   (char* alias, char* contType, 
                       char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddProcTypeNB (char* alias, char* contType, 
                       char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddStateType   (char* alias, char* contType, 
                        char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddStateTypeNB (char* alias, char* contType, 
                        char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEventType   (char* alias, char* contType, 
                        char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEventTypeNB (char* alias, char* contType, 
                        char* name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddLinkType   (char* alias   , char* name,
                       char* contType, char* srcContType,
                       char* destContType){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddLinkTypeNB (char* alias   , char* name,
                       char* contType, char* srcContType,
                       char* destContType){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddVarType   (char* alias   , char* name,
                      char* contType){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddVarTypeNB (char* alias   , char* name,
                      char* contType){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEntityValue   (char* alias, char* entType, 
                          char* name , char* color){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEntityValueNB (char* alias, char* entType, 
                          char* name , char* color){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddContainer   (varPrec time, char* alias    ,
                        char*  type, char* container,
                        char*  name, char* file){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddContainerNB (varPrec time, char* alias    ,
                        char*  type, char* container,
                        char*  name, char* file){
  return TRACE_ERR_NOT_IMPL;
}


int OTFSeqAddContainer   (varPrec time, char* alias    ,
                           char*  type, char* container,
                           char*  name){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSeqAddContainerNB (varPrec time, char* alias    ,
                           char*  type, char* container,
                           char*  name){
  return TRACE_ERR_NOT_IMPL;
}


int OTFDestroyContainer     (varPrec time, char*  name,
                              char*  type){
  return TRACE_ERR_NOT_IMPL;
}

int OTFDestroyContainerNB   (varPrec time, char*  name,
                              char*  type){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetState   (varPrec time, char* type,
                    char*  cont, char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetStateNB (varPrec time, char* type,
                    char*  cont, char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFPushState   (varPrec time, char* type,
                     char*  cont, char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFPushStateNB (varPrec time, char* type,
                     char*  cont, char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFPopState   (varPrec time, char* type,
                    char*  cont, char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFPopStateNB (varPrec time, char* type,
                    char*  cont, char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEvent   (varPrec time, char* type,
                    char*  cont, char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddEventNB (varPrec time, char* type,
                    char*  cont, char* val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFStartLink   (varPrec time, char* type,
                     char*   cont, char* src,
                     char*   val , char* key){
  return TRACE_ERR_NOT_IMPL;
}

int OTFStartLinkNB (varPrec time, char* type,
                     char*   cont, char* src,
                     char*   val , char* key){
  return TRACE_ERR_NOT_IMPL;
}

int OTFEndLink   (varPrec time, char* type,
                   char*   cont, char* dest,
                   char*   val , char* key){
  return TRACE_ERR_NOT_IMPL;
}

int OTFEndLinkNB (varPrec time, char* type,
                   char*   cont, char* dest,
                   char*   val , char* key){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFAddVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSubVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSubVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
  return TRACE_ERR_NOT_IMPL;
}

int OTFEndTrace (){
  return TRACE_ERR_NOT_IMPL;
}

int OTFSetCompress (int i){
  return TRACE_ERR_NOT_IMPL;
}
