#ifdef BUILD_OTF

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <otf.h>

#include "GTG.h"
#include "GTGOTF.h"

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

/* each of these instances of structures corresponds to the NIL value. It contains:
 * - the head of the list
 * - the first identifier
 */
ContainerType_t ctType;
Container_t conts;
StateType_t stateTypes;
EntityValue_t states;
VariableType_t variableTypes;
Variable_t variables;
EventType_t eventTypes;
LinkType_t linkTypes;

/* Root name */
static char *filename = NULL;

#define CHECK_RESULT(__func__) {				\
    if(__func__ != 1) {						\
      fprintf(stderr, "Error while writing the OTF trace\n");	\
      abort();							\
    }								\
  }

#define TIMER_RES 100000.

struct otf_color otf_color_null;

const otf_color_t OTF_get_color(gtg_color_t color) {
  /* todo */
  return &otf_color_null;
}

/*
 * Local methods for OTF only, will be put in an other file after to be cleaner than now...
 */
#define getXXXPtrFromName(_type_, list_head)			\
  struct _type_ *get##_type_##PtrFromName(const char *name) {	\
    struct _type_ *ptr;						\
    if(name == NULL) {						\
      return NULL;						\
    }								\
    gtg_list_for_each_entry(ptr, &(list_head).token, token) {	\
      if(strcmp(name, ptr->name) == 0				\
	 || strcmp(name, ptr->alias) == 0) {			\
	return ptr;						\
      }								\
    }								\
    return NULL;						\
  }

#define getXXXFromName(__type__)				\
  int get##__type__##FromName(const char *name) {		\
    struct __type__ *res = get##__type__##PtrFromName(name);	\
    return (res? res->id : __type__##_NIL);			\
  }

/* generate functions that permit to retrieve a pointer to an object*/
getXXXPtrFromName(ContainerType, ctType)
getXXXPtrFromName(Container, conts)
getXXXPtrFromName(StateType, stateTypes)
getXXXPtrFromName(EntityValue, states)
getXXXPtrFromName(VariableType, variableTypes)
getXXXPtrFromName(EventType, eventTypes)
getXXXPtrFromName(LinkType, linkTypes)

/* generate functions that permit to retrieve the id of objects */
getXXXFromName(ContainerType)
getXXXFromName(Container)
getXXXFromName(StateType)
getXXXFromName(EntityValue)
getXXXFromName(VariableType)
getXXXFromName(EventType)
getXXXFromName(LinkType)

/*
 * return a pointer to the Variable whose container is cont and whose variable type is type.
 * return NULL if not found
 */
Variable_t *getVariablePtrFromCont(int cont, int type) {
  struct Variable *ptr;
  /* todo: use one list of variable per container */
  gtg_list_for_each_entry(ptr, &(variables).token, token) {
    if(type == ptr->type && cont == ptr->parent) {
      return ptr;
    }
  }
  return NULL;
}

/*
 * return the id of the Variable whose container is cont and whose variable type is type.
 * return NULL if not found
 */
int getVariableFromCont(int cont, int type) {
  Variable_t *res = getVariablePtrFromCont(cont, type);
  return (res? res->id : Variable_NIL);
}

/* Initialize all the OTF-specific variables */
static void __OTF_init() {
  /* initialize otf_color_null */
  asprintf(&otf_color_null.colorID, "NO COLOR");
  otf_color_null.red = 0;
  otf_color_null.green = 0;
  otf_color_null.blue = 0;

  /* initialize all the lists/stacks */
  init_ContainerType(ctType);
  init_Container(conts);
  init_StateType(stateTypes);
  init_EntityValue(states);
  init_VariableType(variableTypes);
  init_Variable(variables);
  init_EventType(eventTypes);
  init_LinkType(linkTypes);
}

/* Beginning of the implementation of the interface for OTF */
trace_return_t OTFInitTrace(const char* filenam, gtg_flag_t flags) {
  int ret = TRACE_ERR_OPEN;

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
			       const char* name) {
  uint32_t parent = 0;
  ContainerType_t *p_cont;

  if(contType != NULL && strcmp(contType, "0") != 0) {
    /* get its parent id */
    parent = getContainerTypeFromName(contType);
  }

  /* allocate the container type, initialise it and add it to the list of
   * container types.
   */
  alloc_init_struct(ContainerType_t, p_cont, &ctType.token, name, alias);

  if(verbose)
    printf("addCtType : name %s, alias %s, contType %s\n", name, alias, contType);

  return TRACE_SUCCESS;
}

trace_return_t OTFAddStateType (const char* alias, const char* contType,
				const char* name) {

  /* allocate the state type, initialise it and add it to the list of
   * state types.
   */
  StateType_t *p_state;
  alloc_init_struct(StateType_t, p_state, &stateTypes.token, name, alias);

  if(verbose)
    printf("addStateType : id %d, alias %s, name %s, contType %s\n", p_state->id, alias, name, contType);

  CHECK_RESULT(OTF_Writer_writeDefFunctionGroup(writer, 0, p_state->id, name));

  return TRACE_SUCCESS;
}

trace_return_t OTFAddEventType (const char* alias, const char* contType,
				const char* name) {

  /* allocate the event type, initialise it and add it to the list of
   * event types.
   */
  EventType_t *p_event;
  alloc_init_struct(EventType_t, p_event, &eventTypes.token, name, alias);

  p_event->contType = getContainerTypeFromName(contType);

  if(verbose)
    printf("addEventType : id %d, alias %s, name %s, contType %s\n", p_event->id, alias, name, contType);

  CHECK_RESULT(OTF_Writer_writeDefMarker(writer, 0, p_event->id, name, OTF_MARKER_TYPE_UNKNOWN));

  return TRACE_SUCCESS;
}

trace_return_t OTFAddLinkType (const char* alias   , const char* name,
			       const char* contType, const char* srcContType,
			       const char* destContType) {
  /* allocate the link type, initialise it and add it to the list of
   * link types.
   */
  LinkType_t *p_link;
  alloc_init_struct(LinkType_t, p_link, &linkTypes.token, name, alias);

  p_link->contType = getContainerTypeFromName(contType);
  p_link->srcType  = getContainerTypeFromName(srcContType);
  p_link->destType = getContainerTypeFromName(destContType);

  if(verbose)
    printf("addLinkType : id %d, alias %s, name %s, contType %s, src %s, dest %s\n", p_link->id, alias, name, contType, srcContType, destContType);

  return TRACE_SUCCESS;
}

trace_return_t OTFAddVarType (const char* alias   , const char* contType,
			      const char* name) {
  /* allocate the Variable type, initialise it and add it to the list of
   * variable types.
   */
  VariableType_t *p_variable;
  alloc_init_struct(VariableType_t, p_variable, &variableTypes.token, name, alias);

  CHECK_RESULT(OTF_Writer_writeDefCounterGroup (writer, 0, p_variable->id, name));

  if(verbose)
    printf("addVarType : id %d, alias %s, name %s, contType %s\n", p_variable->id, alias, name, contType);

  return TRACE_SUCCESS;
}

trace_return_t OTFAddEntityValue (const char* alias, const char* entType,
				  const char* name , const otf_color_t color) {
  int type;
  EntityValue_t *p_ent;
  type = getStateTypeFromName(entType);

  alloc_init_struct(EntityValue_t, p_ent, &states.token, name, alias);

  if(verbose)
    printf("addEntityValue : id %d, alias %s, name %s, type %d\n", p_ent->id, alias, name, type);

  CHECK_RESULT(OTF_Writer_writeDefFunction(writer, 0, p_ent->id, name, type, 0));
  return TRACE_SUCCESS;
}

trace_return_t OTFDefineContainer (const char* alias,
				   const char*  type, const char* container,
				   const char*  name, const char* file) {
  int parent;
  Container_t *p_cont;
  parent = getContainerFromName(container);
  /*int ctType = getContainerTypeFromName(type);*/

  /* allocate the Container, initialize it and add it to the list of
   * containers.
   */
  alloc_init_struct(Container_t, p_cont, &conts.token, name, alias);
  /* Initialize the state stack. */
  init_State(p_cont->state_stack);
  if(verbose)
    printf("addCont : parent %d, id %d, name %s, alias %s, type %s, parent %s\n", parent, p_cont->id, name, alias, type, container);

  CHECK_RESULT(OTF_Writer_writeDefProcess(writer, 0, p_cont->id, name, parent));

  return TRACE_SUCCESS;
}

trace_return_t OTFStartContainer (varPrec time, const char* alias,
				const char*  type, const char* container,
				const char*  name, const char* file) {
  uint32_t new_container = getContainerFromName(alias);
  if(!new_container) {
    /* container not yet defined */
    OTFDefineContainer(alias, type, container, name, file);
  }
  OTF_Writer_writeBeginProcess (writer, time*TIMER_RES, new_container);

  return TRACE_SUCCESS;
}

trace_return_t OTFSeqAddContainer (varPrec time, const char* alias,
				   const char*  type, const char* container,
				   const char*  name) {
  return OTFStartContainer(time, alias, type, container, name, NULL);
}

trace_return_t OTFDestroyContainer (varPrec time, const char*  name,
				    const char*  type) {
  uint32_t process = getContainerFromName(name);
  CHECK_RESULT(OTF_Writer_writeEndProcess (writer, time*TIMER_RES, process));
  return TRACE_SUCCESS;
}

trace_return_t OTFSetState (varPrec time, const char* type,
			    const char*  cont, const char* val) {
  Container_t *p_parent = getContainerPtrFromName(cont);
  int stateType = getStateTypeFromName(type);
  int state     = getEntityValueFromName(val);

  /* We need to free the current State structure (if it exists). */
  State_t* p_state;
  if(!gtg_stack_empty(&p_parent->state_stack.token)) {
    p_state = gtg_list_entry(gtg_stack_top(&p_parent->state_stack.token), State_t, token);
    gtg_stack_pop(&p_parent->state_stack.token);
    free(p_state);
  }

  /* Allocate a new State structure and fill it */
  alloc_State(p_state, state, p_parent->id, stateType);

  /* Add the structure to the container state stack. */
  gtg_stack_push(&p_state->token, &p_parent->state_stack.token);
  if(verbose)
    printf("SetState : parent %d, stateType %d, val %d\n", p_parent->id, stateType, state);

  CHECK_RESULT(OTF_Writer_writeEnter (writer, time*TIMER_RES, state, p_parent->id, 0));

  return TRACE_SUCCESS;
}

trace_return_t OTFPushState (varPrec time, const char* type,
			     const char*  cont, const char* val) {
  Container_t *p_parent = getContainerPtrFromName(cont);
  int stateType = getStateTypeFromName(type);
  int value     = getEntityValueFromName(val);

  State_t* p_state;

  /* Allocate a new State structure and fill it */
  alloc_State(p_state, value, p_parent->id, stateType);

  /* Add the structure to the container state stack. */
  gtg_stack_push(&p_state->token, &p_parent->state_stack.token);

  if(verbose)
    printf("PushState : parent %d, stateType %d, val %d\n", p_parent->id, stateType, value);

  CHECK_RESULT(OTF_Writer_writeEnter (writer, time*TIMER_RES, value, p_parent->id, 0));
  return TRACE_SUCCESS;
}

trace_return_t OTFPopState (varPrec time, const char* type,
			    const char*  cont) {
  /* Pop and set */
  Container_t *p_parent     = getContainerPtrFromName(cont);

  State_t *p_state;
  /* Free the current State structure (if it exists). */
  if(!gtg_stack_empty(&p_parent->state_stack.token)) {
    /* Get the top of the stack. */
    p_state = gtg_list_entry(gtg_stack_top(&p_parent->state_stack.token), State_t, token);
    gtg_stack_pop(&p_parent->state_stack.token);
  } else {
    /* The stack is empty, don't do anything and print a warning */
    fprintf(stderr, "Warning: PopState called, but the stack is empty!\n");
    return TRACE_ERR_WRITE;
  }

  if(verbose)
    printf("PopState : parent %d, stateType %d, val %d\n", p_state->cont, p_state->stateType, p_state->value);

  CHECK_RESULT(OTF_Writer_writeLeave (writer, time*TIMER_RES, p_state->value, p_state->cont, 0));
  free(p_state);

  return TRACE_SUCCESS;
}

trace_return_t OTFAddEvent (varPrec time    , const char* type,
			    const char *cont, const char* val){
  uint32_t process = getContainerFromName(cont);
  uint32_t eventType = getEventTypeFromName(type);
  CHECK_RESULT(OTF_Writer_writeMarker (writer, time*TIMER_RES, process, eventType, val));

  if(verbose)
    printf("AddEvent : parent %d, eventType %d, val %s\n", process, eventType, val);

  return TRACE_SUCCESS;
}

trace_return_t OTFStartLink (varPrec time, const char* type,
			     const char*   src, const char* dest,
			     const char*   val , const char* key){
  uint32_t source = getContainerFromName(src);
  uint32_t destination = getContainerFromName(dest);
  uint32_t linkType = getLinkTypeFromName(type);

  CHECK_RESULT(OTF_Writer_writeSendMsg(writer, time*TIMER_RES, source, destination, 0, linkType, 0, 0));
  if(verbose)
    printf("StartLink : time %f, src %d, dest %d, linkType %d, val %s, key %s\n", time*TIMER_RES, source, destination, linkType, val, key);

  return TRACE_SUCCESS;
}

trace_return_t OTFEndLink (varPrec time, const char* type,
                           const char*   src, const char* dest,
                           const char*   val, const char* key){
  uint32_t src_cont = getContainerFromName(src);
  uint32_t dest_cont = getContainerFromName(dest);
  uint32_t linkType = getLinkTypeFromName(type);

  CHECK_RESULT(OTF_Writer_writeRecvMsg(writer, time*TIMER_RES, dest_cont, src_cont, 0, linkType, 0, 0));
  if(verbose)
    printf("EndLink : time %f, src %d, dest %d, linkType %d, val %s, key %s\n", time*TIMER_RES, src_cont, dest_cont, linkType, val, key);

    return TRACE_SUCCESS;
}

trace_return_t OTFSetVar (varPrec time, const char*  type,
			  const char*  cont, varPrec val) {

  int parent  = getContainerFromName(cont);
  int varType = getVariableTypeFromName(type);

  /* Check wether the variable already exists */
  Variable_t * p_counter = getVariablePtrFromCont(parent, varType);

  if(!p_counter) {
    /* the variable doesn't exist yet. Allocate and initialize it  */
    alloc_Variable(p_counter, (gtg_list_entry(variables.token.prev, Variable_t, token)->id) + 1,
		   parent, varType, 0);
    gtg_list_add_tail(&(p_counter->token), &(variables.token));
    CHECK_RESULT(OTF_Writer_writeDefCounter(writer, 0, p_counter->id, type, 0, varType, NULL));
  }

  /* Update the value of the variable */
  p_counter->value = val;

  if( p_counter->value < 0) {
    fprintf(stderr, "A counter value can not be negative!\n");
    return TRACE_ERR_WRITE;
  }

  CHECK_RESULT(OTF_Writer_writeCounter (writer, time*TIMER_RES, parent, varType, p_counter->value));

  if(verbose)
    printf("setVar : %s %s %f\n", type, cont, val);

  return TRACE_SUCCESS;
}

trace_return_t OTFAddVar (varPrec time, const char*  type,
			  const char*  cont, varPrec val) {

  int parent  = getContainerFromName(cont);
  int varType = getVariableTypeFromName(type);

  /* Check wether the variable already exists */
  Variable_t * p_counter = getVariablePtrFromCont(parent, varType);

  if(!p_counter) {
    /* the variable doesn't exist yet. Allocate and initialize it  */
    alloc_Variable(p_counter, (gtg_list_entry(variables.token.prev, Variable_t, token)->id) + 1,
		   parent, varType, 0 /* Let's assume that the initial value is 0 */);

    gtg_list_add_tail(&(p_counter->token), &(variables.token));

    CHECK_RESULT(OTF_Writer_writeDefCounter(writer, 0, p_counter->id, type, 0, varType, NULL));
  }
  /* update the variable value */
  p_counter->value += val;

  if(p_counter->value < 0) {
    fprintf(stderr, "A counter value can not be negative!\n");
    return TRACE_ERR_WRITE;
  }

  CHECK_RESULT(OTF_Writer_writeCounter (writer, time*TIMER_RES, parent, varType, p_counter->value));

  if(verbose)
    printf("addVar : %s %s %f\n", type, cont, val);
  return TRACE_SUCCESS;
}

trace_return_t OTFSubVar (varPrec time, const char*  type,
			  const char*  cont, varPrec val) {
  return OTFAddVar(time, type, cont, - (val) );
}

trace_return_t OTFAddComment   (const char*  comment){
  /* Not implemented yet */
    return TRACE_ERR_WRITE;
}


trace_return_t OTFEndTrace () {
  Container_t *ptr, *tmp;
  State_t *ptr2, *tmp2;

  /* free all the allocated structures */
  free_struct(ContainerType_t, ctType);
  free_struct(StateType_t, stateTypes);
  free_struct(VariableType_t, variableTypes);
  free_struct(EventType_t, eventTypes);
  free_struct(LinkType_t, linkTypes);
  free_struct(EntityValue_t, states);

  /* special case for the list of containers since each one contains a state stack */
  gtg_list_for_each_entry_safe(ptr, tmp, &(conts).token, token) {
    gtg_list_del(&(ptr->token));

    /* Free the State stack */
    gtg_list_for_each_entry_safe(ptr2, tmp2, &(ptr->state_stack).token, token) {
      gtg_list_del(&(ptr2->token));
      free(ptr2);
    }

    free(ptr->name);
    free(ptr->alias);
    free(ptr);
  }

  /* Close the output trace */
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

#endif /* BUILD_OTF */
