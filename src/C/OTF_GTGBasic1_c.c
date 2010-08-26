#include <stdio.h>
#include <string.h>

#include "OTF_GTGBasic1.h"
#include <otf.h>

/* OTF data */
static OTF_FileManager* manager = NULL;
static OTF_Writer* writer = NULL;

/* OTF processes */
#define MAX_PROCESSTYPE 100

static int current_ctType = 0;

typedef struct ContainerType {
    char *name;
    char *alias;
} ContainerType_t;

/*! Global structure storing the ctType defined */
static ContainerType_t ctType[MAX_PROCESSTYPE];

/*! Containers */
typedef struct Container {
    char *name;
    char *alias;
    int ctType;
} Container_t;

static int current_ct = 1;

/*! Global structure storing the containers defined */
#define MAX_PROCESS 1000
static Container_t conts[MAX_PROCESS];


/* OTF states */
#define MAX_STATESTYPE 100

static int current_stateType = 1;

typedef struct StateType { /* Func group */
    char *name;
    char *alias;
    int   groupId;
} StateType_t;

/*! Global structure storing the stateTypes defined */
static StateType_t stateTypes[MAX_STATESTYPE];

typedef struct State {
    char *value;
    int   stateType;
} State_t;

#define MAX_STATES 1000
static int current_state = 0;

/*! EntityValue, contains the name of the functions/states */
typedef struct EntityValue {
    char *name;
    char *alias;
    int   groupId;
} EntityValue_t;

/*! Variables/Counters */
#define MAX_VARIABLETYPE 10
static int current_variableType = 1;
typedef struct VariableType {
    char *name;
    char *alias;
    int   contType;
} VariableType_t;
static VariableType_t variableTypes[MAX_VARIABLETYPE];

/*! Global structure storing the states defined */
static EntityValue_t states[MAX_STATES];



/* Root name */
static char *filename;

int getCtContFromName(const char *name) {
    int i = 1;
    if(name == NULL) {
        return 0;
    }
    for(; i < current_ctType ; i ++) {
        if(strcmp(name, ctType[i].name) == 0 || strcmp(name, ctType[i].alias) == 0) {
            return i;
        }
    }

    return 0;
}

int getCtFromName(const char *name) {
    int i = 1;
    if(name == NULL) {
        return 0;
    }
    for(; i < current_ct ; i ++) {
        if(strcmp(name, conts[i].name) == 0 || strcmp(name, conts[i].alias) == 0) {
            return i;
        }
    }

    return 0;
}

int getStateTypeFromName(const char *type) {
    int i = 1;
    if(type == NULL) {
        return 0;
    }
    for(; i < current_stateType ; i ++) {
        if(strcmp(type, stateTypes[i].name) == 0 || strcmp(type, stateTypes[i].alias) == 0) {
            return i;
        }
    }
    return 0;
}

int getStateFromName(const char *type) {
    int i = 1;
    if(type == NULL) {
        return 0;
    }
    for(; i < current_state ; i ++) {
        if(strcmp(type, states[i].name) == 0 || strcmp(type, states[i].alias) == 0) {
            return i;
        }
    }
    return 0;
}

int getVariableTypeFromName(const char *type) {
    int i = 1;
    if(type == NULL) {
        return 0;
    }
    for(; i < current_variableType ; i ++) {
        if(strcmp(type, variableTypes[i].name) == 0 || strcmp(type, variableTypes[i].alias) == 0) {
            return i;
        }
    }
    return 0;
}

int OTFInitTrace(const char* filenam) {
    int ret = OTF_ERR_OPEN;

    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);

    /* Open maximum of a MAX_PROCESS files for writing trace */
    manager = OTF_FileManager_open(MAX_PROCESS);

    if(manager == NULL) {
        fprintf (stderr, "Failed to open a manager for OTF. \n Leaving \n");
        return ret;
    }

    writer = OTF_Writer_open(filename, 0, manager);
    if(writer == NULL) {
        OTF_FileManager_close(manager);
        manager = NULL;
        fprintf (stderr, "Failed to open a writer for OTF. \n Leaving \n");
        return ret;
    }

    OTF_Writer_writeDefTimerResolution(writer, 0, 1000);

    OTFAddProcType("0", NULL, "0");
    return OTF_SUCCESS;
}

int OTFSetCompress(int val) {
    if(OTF_Writer_setCompression (writer, val))
        return OTF_SUCCESS;
    else
        return OTF_ERR_WRITE;
}

int OTFAddProcType (const char* alias, const char* contType, 
                    const char* name){
    uint32_t parent = 0;
    
    if(contType != NULL && strcmp(contType, "0") != 0) {
        /* get its parent id */
        parent = getCtContFromName(contType);
    }

    ctType[current_ctType].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(ctType[current_ctType].name, name);
    
    ctType[current_ctType].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(ctType[current_ctType].alias, alias);
    
    printf("addCtType : name %s, alias %s, contType %s\n", name, alias, contType);
    current_ctType ++;

    return OTF_SUCCESS;
}


int OTFAddProcTypeNB (const char* alias, const char* contType, 
                      const char* name){
    return OTFAddProcType(alias, contType, name);
}

int OTFAddStateType (const char* alias, const char* contType, 
                     const char* name){
    stateTypes[current_stateType].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(stateTypes[current_stateType].name, name);
    
    stateTypes[current_stateType].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(stateTypes[current_stateType].alias, alias);
    
    printf("addStateType : id %d, alias %s, name %s, contType %s\n", current_stateType, alias, name, contType);

    OTF_Writer_writeDefFunctionGroup(writer, getCtContFromName(contType), current_stateType, name);

    current_stateType ++;

    return OTF_SUCCESS;
}

int OTFAddStateTypeNB (const char* alias, const char* contType, 
                       const char* name){
    return OTFAddStateType(alias, contType, name);
}

int OTFAddEventType (const char* alias, const char* contType, 
                     const char* name){
    return OTF_SUCCESS;
}

int OTFAddEventTypeNB (const char* alias, const char* contType, 
                       const char* name){
    return OTF_SUCCESS;
}

int OTFAddLinkType (const char* alias   , const char* name,
                    const char* contType, const char* srcContType,
                    const char* destContType){
    return OTF_SUCCESS;
}

int OTFAddLinkTypeNB (const char* alias   , const char* name,
                      const char* contType, const char* srcContType,
                      const char* destContType){
    return OTF_SUCCESS;
}

int OTFAddVarType (const char* alias   , const char* name,
                   const char* contType){
    variableTypes[current_variableType].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(variableTypes[current_variableType].name, name);
    
    variableTypes[current_variableType].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(variableTypes[current_variableType].alias, alias);
    
    printf("addVarType : id %d, alias %s, name %s, contType %s\n", current_variableType, alias, name, contType);

    current_variableType ++;

    return OTF_SUCCESS;
}

int OTFAddVarTypeNB (const char* alias   , const char* name,
                     const char* contType){
    return OTFAddVarType(alias, name, contType);
}

int OTFAddEntityValue (const char* alias, const char* entType, 
                       const char* name , const char* color){
    int type = getStateTypeFromName(entType);
    states[current_state].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(states[current_state].name, name);
    
    states[current_state].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(states[current_state].alias, alias);
    printf("addEntityValue : id %d, alias %s, name %s, type %d\n", current_state, alias, name, type);
    OTF_Writer_writeDefFunction(writer, 0, current_state, name, type, 0);
    current_state ++;
    return OTF_SUCCESS;
}

int OTFAddEntityValueNB (const char* alias, const char* entType, 
                         const char* name , const char* color){
    return OTFAddEntityValue(alias, entType, name, color);
}

int OTFAddContainer (varPrec time, const char* alias    ,
                     const char*  type, const char* container,
                     const char*  name, const char* file){
    /*int ctType = getCtContFromName(type);*/

    int parent = getCtFromName(container);
    printf("addCont : parent %d, id %d, name %s, alias %s, type %s, parent %s\n", parent, current_ct, name, alias, type, container);
    conts[current_ct].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(conts[current_ct].name, name);
    
    conts[current_ct].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(conts[current_ct].alias, alias);
    
    OTF_Writer_writeDefProcess(writer, 1, current_ct, name, parent);
    OTF_Writer_writeBeginProcess (writer, time,	current_ct);

    current_ct ++;

    return OTF_SUCCESS;
}

int OTFAddContainerNB (varPrec time, const char* alias    ,
                       const char*  type, const char* container,
                       const char*  name, const char* file){
    return OTFAddContainer(time, alias, type, container, name, file);
}


int OTFSeqAddContainer (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name){
    return OTFAddContainer(time, alias, type, container, name, NULL);
}

int OTFSeqAddContainerNB (varPrec time, const char* alias    ,
                          const char*  type, const char* container,
                          const char*  name){
    return OTFAddContainer(time, alias, type, container, name, NULL);
}


int OTFDestroyContainer (varPrec time, const char*  name,
                         const char*  type){
    uint32_t process = getCtFromName(name);
    OTF_Writer_writeEndProcess (writer, time, process);
    return OTF_SUCCESS;
}


int OTFDestroyContainerNB (varPrec time, const char*  name,
                           const char*  type){
    return OTFDestroyContainer(time, name, type);
}

int OTFSetState (varPrec time, const char* type,
                 const char*  cont, const char* val){
    int parent    = getCtFromName(cont);
    int stateType = getStateTypeFromName(type);
    int state     = getStateFromName(val);

    printf("SetState : parent %d, stateType %d, val %d\n", parent, stateType, state);
    
    
    OTF_Writer_writeEnter (writer, time, state, parent, 0);

    return OTF_SUCCESS;
}

int OTFSetStateNB (varPrec time, const char* type,
                   const char*  cont, const char* val){
    return OTFSetState(time, type, cont, val);
}

int OTFPushState (varPrec time, const char* type,
                  const char*  cont, const char* val){
    return OTF_SUCCESS;
}

int OTFPushStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val){
    return OTF_SUCCESS;
}

int OTFPopState (varPrec time, const char* type,
                 const char*  cont, const char* val){
    return OTF_SUCCESS;
}

int OTFPopStateNB (varPrec time, const char* type,
                   const char*  cont, const char* val){
    return OTF_SUCCESS;
}

int OTFAddEvent (varPrec time, const char* type,
                 const char*  cont, const char* val){
    return OTF_SUCCESS;
}

int OTFAddEventNB (varPrec time, const char* type,
                   const char*  cont, const char* val){
    return OTF_SUCCESS;
}

int OTFStartLink (varPrec time, const char* type,
                  const char*   cont, const char* src,
                  const char*   val , const char* key){
    return OTF_SUCCESS;
}

int OTFStartLinkNB (varPrec time, const char* type,
                    const char*   cont, const char* src,
                    const char*   val , const char* key){
    return OTF_SUCCESS;
}

int OTFEndLink (varPrec time, const char* type,
                const char*   cont, const char* dest,
                const char*   val , const char* key){
    return OTF_SUCCESS;
}

int OTFEndLinkNB (varPrec time, const char* type,
                  const char*   cont, const char* dest,
                  const char*   val , const char* key){
    return OTF_SUCCESS;
}

int OTFSetVar (varPrec time, const char*  type,
               const char*  cont, varPrec val){
    int parent  = getCtFromName(cont);
    int varType = getVariableTypeFromName(type);
    
    //OTF_Writer_writeDefCounter(writer, 0, 0, type, 0, 0, NULL);
    //OTF_Writer_writeCounter (writer, time, parent, varType, val);
    
    printf("setVar : %s %s %f\n", type, cont, val);
    return OTF_SUCCESS;
}

int OTFSetVarNB (varPrec time, const char*  type,
                 const char*  cont, varPrec val){
    return OTFSetVar(time, type, cont, val);
}

int OTFAddVar (varPrec time, const char*  type,
               const char*  cont, varPrec val){
    printf("addVar : %s %s %f\n", type, cont, val);
    return OTF_SUCCESS;
}

int OTFAddVarNB (varPrec time, const char*  type,
                 const char*  cont, varPrec val){
    return OTFAddVar(time, type, cont, val);
}

int OTFSubVar (varPrec time, const char*  type,
               const char*  cont, varPrec val){
    printf("subVar : %s %s %f\n", type, cont, val);
    return OTF_SUCCESS;
}

int OTFSubVarNB (varPrec time, const char*  type,
                 const char*  cont, varPrec val){
    return OTFSubVar(time, type, cont, val);
}

int OTFEndTrace (){
    int i = 0;
    for(; i < current_ctType ; i ++) {
        free(ctType[i].name);
        free(ctType[i].alias);
    }

    for(i = 0 ; i < current_stateType ; i ++) {
        free(stateTypes[i].name);
        free(stateTypes[i].alias);
    }

    for(i = 0 ; i < current_variableType ; i ++) {
        free(variableTypes[i].name);
        free(variableTypes[i].alias);
    }

    for(i = 1 ; i < current_ct ; i ++) {
        free(conts[i].name);
        free(conts[i].alias);
    }

    for(i = 0 ; i < current_state ; i ++) {
        free(states[i].name);
        free(states[i].alias);
    }

    if(!OTF_Writer_close(writer)) {
        fprintf (stderr, "unable to close the file writer");
        return OTF_ERR_CLOSE;
    }
    writer = NULL;

    OTF_FileManager_close(manager);
    manager = NULL;

    if (filename)
        free (filename);
    filename = NULL;

    return OTF_SUCCESS;
}
