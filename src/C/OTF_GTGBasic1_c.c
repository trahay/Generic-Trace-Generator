
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#include "OTF_Structs.h"
#include "OTF_GTGBasic1.h"
#include <otf.h>

/* set in GTGBasic1_c.c
 * verbose !=0 means debugging mode
 */
extern int verbose;

/* set to 'val' if the initialization should invoke set_compression
 * This may happen when setCompress is called before initTrace
 */
int use_compress = 0;

/* Flags that should be used. */
static gtg_flag_t otf_flags;

/* OTF data */
static OTF_FileManager* manager = NULL;
static OTF_Writer* writer = NULL;

/* ContainerTypes */
static int current_ctType = 0;
static ContainerType_t ctType[MAX_PROCESSTYPE];

/*! Containers */
static int current_ct = 1;
static Container_t conts[MAX_PROCESS];

/* stateType */
static int current_stateType = 1;
static StateType_t stateTypes[MAX_STATESTYPE];

/*! States */
static int current_state = 0;
static EntityValue_t states[MAX_STATES];


/*! Variables/Counters */
static int current_variableType = 1;
static VariableType_t variableTypes[MAX_VARIABLETYPE];
static int current_variable = 0;
static Variable_t variables[MAX_VARIABLE];

/*! Events/Markers */
static int current_eventType = 1;
static EventType_t eventTypes[MAX_EVENTTYPE];

/*! Link/Messages */
static int current_linkType = 1;
static LinkType_t linkTypes[MAX_LINKTYPE];

/* Root name */
static char *filename = NULL;

#define TIMER_RES 100000.

#define STACK_MAX_SIZE 10000
typedef struct StateStack {
    State_t values[STACK_MAX_SIZE];
    int current_id;
} StateStack_t;

static StateStack_t states_saved[MAX_PROCESS];
static State_t last_state[MAX_PROCESS];


struct otf_color otf_color_null;

const otf_color_t OTF_get_color(gtg_color_t color) {
    /* todo */
    return &otf_color_null;
}

/*
 * Local methods for OTF only, will be put in an other file after to be cleaner than now...
 */
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

int getVariableFromCont(int cont, int type) {
    int i = 1;
    for(; i < current_variable ; i ++) {
        if(type == variables[i].type && cont == variables[i].parent) {
            return i;
        }
    }
    return 0;
}

int getEventTypeFromName(const char *type) {
    int i = 1;
    if(type == NULL) {
        return 0;
    }
    for(; i < current_eventType ; i ++) {
        if(strcmp(type, eventTypes[i].name) == 0 || strcmp(type, eventTypes[i].alias) == 0) {
            return i;
        }
    }
    return 0;
}

int getLinkTypeFromName(const char *type) {
    int i = 1;
    if(type == NULL) {
        return 0;
    }
    for(; i < current_linkType ; i ++) {
        if(strcmp(type, linkTypes[i].name) == 0 || strcmp(type, linkTypes[i].alias) == 0) {
            return i;
        }
    }
    return 0;
}

/* Initialize all the OTF-specific variables */
static void __OTF_init() {
    /* initialize otf_color_null */
    asprintf(&otf_color_null.colorID, "NO COLOR");
    otf_color_null.red = 0;
    otf_color_null.green = 0;
    otf_color_null.blue = 0;
}

/* Beginning of the implementation of the interface for OTF */
trace_return_t OTFInitTrace(const char* filenam, gtg_flag_t flags) {
    int ret = TRACE_ERR_OPEN;
    int i;

    otf_flags = flags;
    /* first, let's initialize all the OTF-specific variables */
    __OTF_init();

    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);

    /* Open maximum of a MAX_PROCESS files for writing trace */
    manager = OTF_FileManager_open(MAX_PROCESS);

    if(manager == NULL) {
        fprintf (stderr, "Failed to open a manager for OTF. \n Leaving \n");
        return (trace_return_t) ret;
    }

    writer = OTF_Writer_open(filename, 0, manager);
    if(writer == NULL) {
        OTF_FileManager_close(manager);
        manager = NULL;
        fprintf (stderr, "Failed to open a writer for OTF. \n Leaving \n");
        return (trace_return_t) ret;
    }

    OTF_Writer_writeDefTimerResolution(writer, 0, TIMER_RES);

    if(use_compress)
        OTF_Writer_setCompression (writer, use_compress);

    OTFAddContType("0", NULL, "0");

    for(i = 0 ; i < MAX_PROCESS ; i ++) {
        states_saved[i].current_id = 0;
    }

    return TRACE_SUCCESS;
}

trace_return_t OTFSetCompress(int val) {
    if(writer) {
        if(OTF_Writer_setCompression (writer, val))
            return TRACE_SUCCESS;
    } else {
        /* postpone the invocation of setCompression */
        use_compress=val;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t OTFAddContType (const char* alias, const char* contType, 
                               const char* name){
    uint32_t parent = 0;
    if(current_ctType >= MAX_PROCESSTYPE) {
        fprintf(stderr, "Too many Container types (%d)!\n", MAX_PROCESSTYPE);
	return TRACE_ERR_WRITE;
    }

    if(contType != NULL && strcmp(contType, "0") != 0) {
        /* get its parent id */
        parent = getCtContFromName(contType);
    }

    ctType[current_ctType].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(ctType[current_ctType].name, name);
    
    ctType[current_ctType].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(ctType[current_ctType].alias, alias);

    if(verbose)
        printf("addCtType : name %s, alias %s, contType %s\n", name, alias, contType);
    current_ctType ++;

    return TRACE_SUCCESS;
}

trace_return_t OTFAddStateType (const char* alias, const char* contType, 
                                const char* name){
    if(current_stateType >= MAX_STATESTYPE) {
        fprintf(stderr, "Too many State types (%d)!\n", MAX_STATESTYPE);
	return TRACE_ERR_WRITE;
    }

    stateTypes[current_stateType].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(stateTypes[current_stateType].name, name);
    
    stateTypes[current_stateType].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(stateTypes[current_stateType].alias, alias);

    if(verbose)
        printf("addStateType : id %d, alias %s, name %s, contType %s\n", current_stateType, alias, name, contType);

    OTF_Writer_writeDefFunctionGroup(writer, 0, current_stateType, name);

    current_stateType ++;

    return TRACE_SUCCESS;
}

trace_return_t OTFAddEventType (const char* alias, const char* contType, 
                                const char* name){
    if(current_eventType >= MAX_EVENTTYPE) {
        fprintf(stderr, "Too many Event types (%d)!\n", MAX_EVENTTYPE);
        return TRACE_ERR_WRITE;
    }
    eventTypes[current_eventType].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(eventTypes[current_eventType].name, name);

    eventTypes[current_eventType].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(eventTypes[current_eventType].alias, alias);

    eventTypes[current_eventType].contType = getCtContFromName(contType);

    if(verbose)
        printf("addEventType : id %d, alias %s, name %s, contType %s\n", current_eventType, alias, name, contType);

    OTF_Writer_writeDefMarker(writer, 0, current_eventType, name, OTF_MARKER_TYPE_UNKNOWN);
    current_eventType ++;

    return TRACE_SUCCESS;
}

trace_return_t OTFAddLinkType (const char* alias   , const char* name,
                               const char* contType, const char* srcContType,
                               const char* destContType){
    linkTypes[current_linkType].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(linkTypes[current_linkType].name, name);
    
    linkTypes[current_linkType].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(linkTypes[current_linkType].alias, alias);
    
    linkTypes[current_linkType].contType = getCtContFromName(contType);
    linkTypes[current_linkType].srcType = getCtContFromName(srcContType);
    linkTypes[current_linkType].destType = getCtContFromName(destContType);

    if(verbose)
        printf("addLinkType : id %d, alias %s, name %s, contType %s, src %s, dest %s\n", current_linkType, alias, name, contType, srcContType, destContType);
    
    current_linkType ++;
    return TRACE_SUCCESS;
}

trace_return_t OTFAddVarType (const char* alias   , const char* contType,
                              const char* name){
    if(current_variableType >= MAX_VARIABLETYPE) {
        fprintf(stderr, "Too many Variable types (%d)!\n", MAX_VARIABLETYPE);
	return TRACE_ERR_WRITE;
    }

    variableTypes[current_variableType].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(variableTypes[current_variableType].name, name);

    variableTypes[current_variableType].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(variableTypes[current_variableType].alias, alias);

    OTF_Writer_writeDefCounterGroup (writer, 0, current_variableType, name);

    if(verbose)
        printf("addVarType : id %d, alias %s, name %s, contType %s\n", current_variableType, alias, name, contType);

    current_variableType ++;

    return TRACE_SUCCESS;
}

trace_return_t OTFAddEntityValue (const char* alias, const char* entType, 
				  const char* name , const otf_color_t color){
    int type = -1;
    if(current_state >= MAX_STATES) {
        fprintf(stderr, "Too many Entity values (%d)!\n", MAX_STATES);
	return TRACE_ERR_WRITE;
    }

    type = getStateTypeFromName(entType);
    states[current_state].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(states[current_state].name, name);

    states[current_state].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(states[current_state].alias, alias);

    if(verbose)
        printf("addEntityValue : id %d, alias %s, name %s, type %d\n", current_state, alias, name, type);

    OTF_Writer_writeDefFunction(writer, 0, current_state, name, type, 0);
    current_state ++;
    return TRACE_SUCCESS;
}

trace_return_t OTFAddContainer (varPrec time, const char* alias,
                                const char*  type, const char* container,
                                const char*  name, const char* file){
    int parent;
    if(current_ct >= MAX_PROCESS) {
        fprintf(stderr, "Too many Containers (%d)!\n", MAX_PROCESS);
	return TRACE_ERR_WRITE;
    }

    /*int ctType = getCtContFromName(type);*/
    parent = getCtFromName(container);

    if(verbose)
        printf("addCont : parent %d, id %d, name %s, alias %s, type %s, parent %s\n", parent, current_ct, name, alias, type, container);

    conts[current_ct].name = (char *)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(conts[current_ct].name, name);

    conts[current_ct].alias = (char *)malloc(sizeof(char)*(strlen(alias)+1));
    strcpy(conts[current_ct].alias, alias);

    OTF_Writer_writeDefProcess(writer, 0, current_ct, name, parent);
    OTF_Writer_writeBeginProcess (writer, time*TIMER_RES, current_ct);

    current_ct ++;

    return TRACE_SUCCESS;
}

trace_return_t OTFSeqAddContainer (varPrec time, const char* alias,
                                   const char*  type, const char* container,
                                   const char*  name){
    return OTFAddContainer(time, alias, type, container, name, NULL);
}

trace_return_t OTFDestroyContainer (varPrec time, const char*  name,
                                    const char*  type){
    uint32_t process = getCtFromName(name);
    OTF_Writer_writeEndProcess (writer, time*TIMER_RES, process);
    return TRACE_SUCCESS;
}

trace_return_t OTFSetState (varPrec time, const char* type,
                            const char*  cont, const char* val){
    int parent    = getCtFromName(cont);
    int stateType = getStateTypeFromName(type);
    int state     = getStateFromName(val);

    last_state[parent].value     = state;
    last_state[parent].cont      = parent;
    last_state[parent].stateType = stateType;

    if(verbose)
        printf("SetState : parent %d, stateType %d, val %d\n", parent, stateType, state);

    OTF_Writer_writeEnter (writer, time*TIMER_RES, state, parent, 0);

    return TRACE_SUCCESS;
}

trace_return_t OTFPushState (varPrec time, const char* type,
                             const char*  cont, const char* val){
    int parent    = getCtFromName(cont);
    int stateType = getStateTypeFromName(type);
    int state     = getStateFromName(val);

    int current_id = states_saved[parent].current_id;
    /* Push previous and set the new */
    states_saved[parent].values[current_id].value = last_state[parent].value;
    states_saved[parent].values[current_id].cont = last_state[parent].cont;
    states_saved[last_state[parent].cont].values[current_id].stateType = last_state[parent].stateType;

    states_saved[parent].current_id ++;

    if(verbose)
        printf("PushState : parent %d, stateType %d, val %d\n", parent, stateType, state);

    OTF_Writer_writeEnter (writer, time*TIMER_RES, state, parent, 0);
    return TRACE_SUCCESS;
}

trace_return_t OTFPopState (varPrec time, const char* type,
                            const char*  cont){
    /* Pop and set */
    int parent     = getCtFromName(cont);
    int current_id = states_saved[last_state[parent].cont].current_id - 1;
    State_t st;
    st.value = states_saved[last_state[parent].cont].values[current_id].value;
    st.cont = states_saved[last_state[parent].cont].values[current_id].cont;
    st.stateType = states_saved[last_state[parent].cont].values[current_id].stateType;

    states_saved[last_state[parent].cont].current_id --;

    if(verbose)
        printf("PopState : parent %d, stateType %d, val %d\n", st.cont, st.stateType, st.value);

    OTF_Writer_writeLeave (writer, time*TIMER_RES, st.value, st.cont, 0);

    return TRACE_SUCCESS;
}

trace_return_t OTFAddEvent (varPrec time    , const char* type,
                            const char *cont, const char* val){
    uint32_t process = getCtFromName(cont);
    uint32_t eventType = getEventTypeFromName(type);
    OTF_Writer_writeMarker (writer, time*TIMER_RES, process, eventType, val);

    if(verbose)
        printf("AddEvent : parent %d, eventType %d, val %s\n", process, eventType, val);

    return TRACE_SUCCESS;
}

trace_return_t OTFStartLink (varPrec time, const char* type,
                             const char*   src, const char* dest,
                             const char*   val , const char* key){
      uint32_t source = getCtFromName(src);
      uint32_t destination = getCtFromName(dest);
      uint32_t linkType = getLinkTypeFromName(type);

      OTF_Writer_writeSendMsg(writer, time*TIMER_RES, source, destination, 0, linkType, 0, 0);
      if(verbose)
      printf("StartLink : time %f, src %d, dest %d, linkType %d, val %s, key %s\n", time*TIMER_RES, source, destination, linkType, val, key);

    return TRACE_SUCCESS;
}

trace_return_t OTFEndLink (varPrec time, const char* type,
                           const char*   src, const char* dest,
                           const char*   val, const char* key){
      uint32_t src_cont = getCtFromName(src);
      uint32_t dest_cont = getCtFromName(dest);
      uint32_t linkType = getLinkTypeFromName(type);
      
      OTF_Writer_writeRecvMsg(writer, time*TIMER_RES, dest_cont, src_cont, 0, linkType, 0, 0);
      if(verbose)
      printf("EndLink : time %f, src %d, dest %d, linkType %d, val %s, key %s\n", time*TIMER_RES, src_cont, dest_cont, linkType, val, key);

    return TRACE_SUCCESS;
}

trace_return_t OTFSetVar (varPrec time, const char*  type,
                          const char*  cont, varPrec val){
    int parent  = getCtFromName(cont);
    int varType = getVariableTypeFromName(type);
    
    int counter = getVariableFromCont(parent, varType);
    if(counter == 0) { /* New one */
        current_variable ++;
        variables[current_variable].parent = parent;
        variables[current_variable].type = varType;
        counter = current_variable;
        OTF_Writer_writeDefCounter(writer, 0, counter, type, 0, varType, NULL);
    }

    variables[current_variable].value = val;

    if(variables[current_variable].value < 0) {
        fprintf(stderr, "A counter value can not be negative!\n");
	return TRACE_ERR_WRITE;
    }

    OTF_Writer_writeCounter (writer, time*TIMER_RES, parent, varType, variables[current_variable].value);

    if(verbose)
        printf("setVar : %s %s %f\n", type, cont, val);
    return TRACE_SUCCESS;
}

trace_return_t OTFAddVar (varPrec time, const char*  type,
                          const char*  cont, varPrec val){
    int parent  = getCtFromName(cont);
    int varType = getVariableTypeFromName(type);
    
    int counter = getVariableFromCont(parent, varType);
    if(counter == 0) { /* New one */
        current_variable ++;
        variables[current_variable].parent = parent;
        variables[current_variable].type = varType;
        counter = current_variable;
        variables[current_variable].value = 0;
        OTF_Writer_writeDefCounter(writer, 0, counter, type, 0, varType, NULL);
    }

    variables[current_variable].value += val;

    if(variables[current_variable].value < 0) {
        fprintf(stderr, "A counter value can not be negative!\n");
	return TRACE_ERR_WRITE;
    }

    OTF_Writer_writeCounter (writer, time*TIMER_RES, parent, varType, variables[current_variable].value);

    if(verbose)
        printf("addVar : %s %s %f\n", type, cont, val);
    return TRACE_SUCCESS;
}

trace_return_t OTFSubVar (varPrec time, const char*  type,
                          const char*  cont, varPrec val){
    int parent  = getCtFromName(cont);
    int varType = getVariableTypeFromName(type);
    
    int counter = getVariableFromCont(parent, varType);
    if(counter == 0) { /* New one */
        current_variable ++;
        variables[current_variable].parent = parent;
        variables[current_variable].type = varType;
        counter = current_variable;
        variables[current_variable].value = 0;
        OTF_Writer_writeDefCounter(writer, 0, counter, type, 0, varType, NULL);
    }

    variables[current_variable].value -= val;

    if(variables[current_variable].value < 0) {
        fprintf(stderr, "A counter value can not be negative!\n");
	return TRACE_ERR_WRITE;
    }

    OTF_Writer_writeCounter (writer, time*TIMER_RES, parent, varType, variables[current_variable].value);

    if(verbose)
        printf("subVar : %s %s %f\n", type, cont, val);
    return TRACE_SUCCESS;
}

trace_return_t OTFEndTrace (){
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

    for(i = 1 ; i < current_eventType ; i ++) {
        free(eventTypes[i].name);
        free(eventTypes[i].alias);
    }

    for(i = 1 ; i < current_ct ; i ++) {
        free(conts[i].name);
        free(conts[i].alias);
    }

    for(i = 1 ; i < current_linkType ; i ++) {
        free(linkTypes[i].name);
        free(linkTypes[i].alias);
    }

    for(i = 0 ; i < current_state ; i ++) {
        free(states[i].name);
        free(states[i].alias);
    }

    if(!OTF_Writer_close(writer)) {
        fprintf (stderr, "unable to close the file writer");
        return TRACE_ERR_CLOSE;
    }
    writer = NULL;

    OTF_FileManager_close(manager);
    manager = NULL;

    if (filename)
        free (filename);
    filename = NULL;

    free(otf_color_null.colorID);

    return TRACE_SUCCESS;
}
