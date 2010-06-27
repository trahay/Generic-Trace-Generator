#include "../../inc/OTF_GTGBasic1.h"
#include <stdio.h>
#include <string.h>

/* Root name */
static char*  filename;
/* Rank MPI */
static int    rank;
/* Size of MPI communiecator */
static int    size;
/* MPI communicator */
static MPI_Comm comm;

int OTFInitTrace   (char* filenam){

}

int OTFAddProcType   (char* alias, char* contType, 
                       char* name){
}

int OTFAddProcTypeNB (char* alias, char* contType, 
                       char* name){
}

int OTFAddStateType   (char* alias, char* contType, 
                        char* name){
}

int OTFAddStateTypeNB (char* alias, char* contType, 
                        char* name){
}

int OTFAddEventType   (char* alias, char* contType, 
                        char* name){
}

int OTFAddEventTypeNB (char* alias, char* contType, 
                        char* name){
}

int OTFAddLinkType   (char* alias   , char* name,
                       char* contType, char* srcContType,
                       char* destContType){
}

int OTFAddLinkTypeNB (char* alias   , char* name,
                       char* contType, char* srcContType,
                       char* destContType){
}

int OTFAddVarType   (char* alias   , char* name,
                      char* contType){
}

int OTFAddVarTypeNB (char* alias   , char* name,
                      char* contType){
}

int OTFAddEntityValue   (char* alias, char* entType, 
                          char* name , char* color){
}

int OTFAddEntityValueNB (char* alias, char* entType, 
                          char* name , char* color){
}

int OTFAddContainer   (varPrec time, char* alias    ,
                        char*  type, char* container,
                        char*  name, char* file){
}

int OTFAddContainerNB (varPrec time, char* alias    ,
                        char*  type, char* container,
                        char*  name, char* file){
}


int OTFSeqAddContainer   (varPrec time, char* alias    ,
                           char*  type, char* container,
                           char*  name){
}

int OTFSeqAddContainerNB (varPrec time, char* alias    ,
                           char*  type, char* container,
                           char*  name){
}


int OTFDestroyContainer     (varPrec time, char*  name,
                              char*  type){
}

int OTFDestroyContainerNB   (varPrec time, char*  name,
                              char*  type){
}

int OTFSetState   (varPrec time, char* type,
                    char*  cont, char* val){
}

int OTFSetStateNB (varPrec time, char* type,
                    char*  cont, char* val){
}

int OTFPushState   (varPrec time, char* type,
                     char*  cont, char* val){
}

int OTFPushStateNB (varPrec time, char* type,
                     char*  cont, char* val){
}

int OTFPopState   (varPrec time, char* type,
                    char*  cont, char* val){
}

int OTFPopStateNB (varPrec time, char* type,
                    char*  cont, char* val){
}

int OTFAddEvent   (varPrec time, char* type,
                    char*  cont, char* val){
}

int OTFAddEventNB (varPrec time, char* type,
                    char*  cont, char* val){
}

int OTFStartLink   (varPrec time, char* type,
                     char*   cont, char* src,
                     char*   val , char* key){
}

int OTFStartLinkNB (varPrec time, char* type,
                     char*   cont, char* src,
                     char*   val , char* key){
}

int OTFEndLink   (varPrec time, char* type,
                   char*   cont, char* dest,
                   char*   val , char* key){
}

int OTFEndLinkNB (varPrec time, char* type,
                   char*   cont, char* dest,
                   char*   val , char* key){
}

int OTFSetVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
}

int OTFSetVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
}

int OTFAddVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
}

int OTFAddVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
}

int OTFSubVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
}

int OTFSubVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
}

int OTFSetComm (MPI_Comm c){
    comm = c;
    MPI_Comm_rank (comm, &rank);
    MPI_Comm_size (comm, &size);
    return 0;
}

int OTFEndTrace (){
}

int OTFSetCompress (int i){
}
