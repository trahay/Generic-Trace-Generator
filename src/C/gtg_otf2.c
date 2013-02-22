#define _GNU_SOURCE         /* See feature_test_macros(7) */

#define BUILD_OTF2
#ifdef BUILD_OTF2

#define _XOPEN_SOURCE 500   /* See feature_test_macros(7) */
#include <ftw.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <assert.h>
#include <otf2/otf2.h>
#include "gtg_otf2.h"

/* set in GTGBasic1_c.c
 * verbose !=0 means debugging mode
 */
extern int verbose;

/* Flags that should be used. */
static gtg_flag_t otf2_flags;

#define TIMER_RES 100000.

enum OTF2_Compression_enum otf2_compression = OTF2_COMPRESSION_NONE;


OTF2_GlobalDefWriter* global_def_writer = NULL;
OTF2_Archive*  archive = NULL;



uint32_t id_NIL;


/* each of these instances of structures corresponds to the NIL value. It contains:
 * - the head of the list
 * - the first identifier
 */
//OTF2_ContainerType_t ctType;
OTF2_Container_t container_list;
//OTF2_StateType_t stateTypes;
OTF2_EntityValue_t state_list;
//OTF2_VariableType_t variableTypes;
OTF2_VariableType_t var_list;
//OTF2_EventType_t eventTypes;
OTF2_Link_t link_list;



/*
 * Local methods for OTF only, will be put in an other file after to be cleaner than now...
 */
#define OTF2_getXXXPtrFromName(_type_, list_head)			\
  struct OTF2_##_type_ *OTF2_get##_type_##PtrFromName(const char *name) { \
    struct OTF2_##_type_ *ptr;						\
    if(name == NULL) {							\
      return NULL;							\
    }									\
    gtg_list_for_each_entry(ptr, &(list_head).token, token) {		\
      if(strcmp(name, ptr->name.str) == 0				\
	 || strcmp(name, ptr->alias.str) == 0) {			\
	return ptr;							\
      }									\
    }									\
    return NULL;							\
  }

#define OTF2_getXXXFromName(__type__)					\
  uint32_t OTF2_get##__type__##FromName(const char *name) {		\
    struct OTF2_##__type__ *res = OTF2_get##__type__##PtrFromName(name); \
    return (res? res->id : id_NIL);					\
  }

/* generate functions that permit to retrieve a pointer to an object*/
OTF2_getXXXPtrFromName(Container, container_list)
OTF2_getXXXPtrFromName(EntityValue, state_list)
OTF2_getXXXPtrFromName(VariableType, var_list)
OTF2_getXXXPtrFromName(Link, link_list)

// OTF2_getXXXPtrFromName(ContainerType, ctType)
// OTF2_getXXXPtrFromName(StateType, stateTypes)
// OTF2_getXXXPtrFromName(EventType, eventTypes)
// OTF2_getXXXPtrFromName(LinkType, linkTypes)

/* generate functions that permit to retrieve the id of objects */
OTF2_getXXXFromName(Container)
OTF2_getXXXFromName(EntityValue)
OTF2_getXXXFromName(VariableType)
OTF2_getXXXFromName(Link)
//OTF2_getXXXFromName(ContainerType)
//OTF2_getXXXFromName(StateType)
//OTF2_getXXXFromName(EventType)
//OTF2_getXXXFromName(LinkType)


/*
 * return a pointer to the Variable whose container is cont and whose variable type is type.
 * return NULL if not found
 */
OTF2_Variable_t *OTF2_getVariablePtrFromCont(OTF2_Container_t *p_cont, OTF2_VariableType_t *vtype) {
  struct OTF2_Variable *p_var;

  /* todo: use one list of variable per container */
  gtg_list_for_each_entry(p_var, &p_cont->variable_list.token, token) {
    if(vtype->id == p_var->p_var_type->id) {
      return p_var;
    }
  }
  return NULL;
}



/* Define a pre and post flush callback. If no memory is left in OTF2's internal memory buffer
   or the writer handle is closed a memory buffer flushing routine is triggered. The pre flush
   callback is triggered right before a buffer flush. It needs to return either OTF2_FLUSH to
   flush the recorded data to a file or OTF2_NO_FLUSH to supress flushing data to a file. The
   post flush callback is triggered right after a memory buffer flush. It has to return a current
   timestamp which is recorded to mark the time spend in a buffer flush. */

OTF2_FlushType pre_flush( void*         userData,
			  OTF2_FileType fileType,
			  uint64_t      locationId,
			  void*         callerData,
			  bool          final )
{
  return OTF2_FLUSH;
}

OTF2_TimeStamp post_flush( void*         userData,
			   OTF2_FileType fileType,
			   uint64_t      locationId )
{
  return gtg_otf2_get_time();
}

OTF2_FlushCallbacks flush_callbacks =
  {
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
  };



/* Initialize all the OTF-specific variables */
static void __OTF2_init() {
  init_id();

  /* initialize otf_color_null */
//  asprintf(&otf_color_null.colorID, "NO COLOR");
//  otf_color_null.red = 0;
//  otf_color_null.green = 0;
//  otf_color_null.blue = 0;

  /* initialize all the lists/stacks */
//  init_ContainerType(ctType);
  OTF2_init_Container(&container_list);
//  init_StateType(stateTypes);
  OTF2_init_EntityValue(&state_list);
  OTF2_init_VariableType(&var_list);
//  init_Variable(variables);
//  init_EventType(eventTypes);
  OTF2_init_Link(&link_list);
}

static int __rm_file(const char *fpath, const struct stat *sb,
	     int typeflag, struct FTW *ftwbuf) {

  if(S_ISDIR(sb->st_mode)) {
    if(rmdir(fpath))
      return 1;
  } else {
    if(unlink(fpath))
      return 1;
  }
  return 0;
}

/* Beginning of the implementation of the interface for OTF2 */
trace_return_t OTF2InitTrace(const char *filenam,
			     gtg_flag_t flags) {
  otf2_flags = flags;

  __OTF2_init();

  /* if the trace already exists, delete it */
  if(nftw(filenam, __rm_file, 10, FTW_DEPTH) != 0) {
    perror("nftw");
  }

  /* Create new archive handle. */
  archive = OTF2_Archive_Open( filenam,
			       filenam,
			       OTF2_FILEMODE_WRITE,
			       1024 * 1024,
			       4 * 1024 * 1024,
			       OTF2_SUBSTRATE_POSIX,
			       otf2_compression );

  /* Set master slave mode, description, and creator. */
  CHECK_STATUS(OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL ));

  CHECK_STATUS(OTF2_Archive_SetMasterSlaveMode( archive, OTF2_MASTER ));

  CHECK_STATUS(OTF2_Archive_SetDescription( archive, "OTF2 trace for testing GTG." ));

  CHECK_STATUS(OTF2_Archive_SetCreator( archive, "Generated with GTG" ));

  /* Get a local event writer and a local definition writer for location 0. Additionally a global definition writer is needed.  */
  global_def_writer = OTF2_Archive_GetGlobalDefWriter( archive );

  CHECK_STATUS(OTF2_GlobalDefWriter_WriteString( global_def_writer, 0, "Process" ));

  /* TODO: set the timer resolution */
  //OTF2_GlobalDefWriter_WriteClockProperties (OTF2_GlobalDefWriter *writerHandle, uint64_t timer_resolution, uint64_t global_offset, uint64_t trace_length)

  return TRACE_SUCCESS;
}

trace_return_t OTF2SetCompress(int val) {

  if(val) {
    fprintf(stderr, "otf2 compression mode ON\n");
    otf2_compression = OTF2_COMPRESSION_ZLIB;
  } else {
    otf2_compression = OTF2_COMPRESSION_NONE;
  }

  return TRACE_ERR_WRITE;
}

trace_return_t OTF2AddContType (const char *alias,
				const char *contType,
				const char *name) {

  return TRACE_SUCCESS;
}

trace_return_t OTF2AddStateType (const char *alias,
				 const char *contType,
				 const char *name) {

  return TRACE_SUCCESS;
}

trace_return_t OTF2AddEventType (const char *alias,
				 const char *contType,
				 const char *name) {

  return TRACE_SUCCESS;
}

trace_return_t OTF2AddLinkType (const char *alias,
				const char *name,
				const char *contType,
				const char *srcContType,
				const char *destContType) {

  return TRACE_SUCCESS;
}

trace_return_t OTF2AddVarType (const char *alias,
			       const char *contType,
			       const char *name) {

  OTF2_VariableType_t *var;
  alloc_struct(var, OTF2_VariableType_t, &var_list.token);

  /* initialize and declare (to lib OTF2) strings */
  init_otf2_string(&var->alias, alias);
  init_otf2_string(&var->name, name);
  var->id = get_new_var_id();

  OTF2_GlobalDefWriter_WriteParameter( 	global_def_writer,
					var->id,
					var->name.id,
					OTF2_PARAMETER_TYPE_UINT64);
  return TRACE_SUCCESS;
}

trace_return_t OTF2AddEntityValue (const char *alias,
				   const char *entType,
				   const char *name,
				   const otf_color_t color) {


  OTF2_EntityValue_t *state;
  alloc_struct(state, OTF2_EntityValue_t, &state_list.token);

  /* initialize and declare (to lib OTF2) strings */
  init_otf2_string(&state->alias, alias);
  init_otf2_string(&state->name, name);
  state->id = get_new_function_id();

  /* Write definition for the code region which was just entered and left to the global definition writer.  */
  OTF2_GlobalDefWriter_WriteRegion( global_def_writer,
				    state->id, /* The unique identifier for this Region definition.  */
				    state->alias.id, /* Name of the region (demangled name if available). References a String definition.  */
				    state->alias.id,	/* Alternative name of the region (e.g. mangled name). References a String definition. */
				    state->name.id,	/* A more detailed description of this region. References a String definition.  */
				    OTF2_REGION_ROLE_FUNCTION, /* Region role.  */
				    OTF2_PARADIGM_UNKNOWN,
				    OTF2_REGION_FLAG_NONE,
				    0,			       /* The source file where this region was declared. References a String definition.  */
				    0,			       /* Starting line number of this region in the source file.  */
				    0 /* Ending line number of this region in the source file. */ );


  return TRACE_SUCCESS;
}

trace_return_t OTF2DefineContainer (const char *alias,
				    const char *type,
				    const char *container,
				    const char *name,
				    const char *file) {


  OTF2_Container_t *p_parent = OTF2_getContainerPtrFromName(container);
  OTF2_Container_t *p_container;
  alloc_struct(p_container, OTF2_Container_t, &container_list.token);

  p_container->id = get_new_process_id();

  init_otf2_string(&p_container->name, name);
  init_otf2_string(&p_container->alias, alias);

  GTG_STACK_INIT(&p_container->state_stack.token);
  GTG_LIST_INIT(&p_container->variable_list.token);
  GTG_LIST_INIT(&p_container->children_list);
  p_container->nb_children = 0;
  p_container->parent = p_parent;



  CHECK_STATUS(OTF2_GlobalDefWriter_WriteSystemTreeNode(global_def_writer,
							p_container->id,
							p_container->name.id,
							p_container->name.id,
							p_container->parent?p_container->parent->id : OTF2_UNDEFINED_UINT32));

  CHECK_STATUS(OTF2_GlobalDefWriter_WriteLocationGroup(global_def_writer,
						       p_container->id,
						       p_container->name.id,
						       OTF2_LOCATION_GROUP_TYPE_PROCESS,
						       p_container->id));

  CHECK_STATUS(OTF2_GlobalDefWriter_WriteLocation(global_def_writer,
						  p_container->id,
						  p_container->name.id, /* name of the process */
						  OTF2_LOCATION_TYPE_CPU_THREAD,
						  0, /* nb of events before flush ? */
						  p_container->id));	  /* location group */


  p_container->evt_writer = OTF2_Archive_GetEvtWriter( archive, p_container->id);

  __attribute__((unused)) OTF2_DefWriter* def_writer = NULL;
  /* just to make the corresponding file exist */
  def_writer = OTF2_Archive_GetDefWriter( archive, p_container->id);

  return TRACE_SUCCESS;
}

trace_return_t OTF2StartContainer (varPrec time,
				   const char *alias,
				   const char *type,
				   const char *container,
				   const char *name,
				   const char *file) {

  return TRACE_SUCCESS;
}

trace_return_t OTF2SeqAddContainer (varPrec time,
				    const char *alias,
				    const char *type,
				    const char *container,
				    const char *name) {

  return OTF2StartContainer(time*TIMER_RES, alias, type, container, name, NULL);
}

trace_return_t OTF2DestroyContainer (varPrec time,
				     const char *name,
				     const char *type) {

  return TRACE_SUCCESS;
}

trace_return_t OTF2SetState (varPrec time,
			     const char *type,
			     const char *cont,
			     const char *val) {

  OTF2_Container_t *p_cont = OTF2_getContainerPtrFromName(cont);

  /* We need to free the current State structure (if it exists). */
  OTF2_State_t* p_state;
  if(!gtg_stack_empty(&p_cont->state_stack.token)) {
    OTF2PopState(time, type, cont);
  }

  return OTF2PushState(time, type, cont, val);
}

trace_return_t OTF2PushState (varPrec time,
			      const char *type,
			      const char *cont,
			      const char *val) {


  OTF2_Container_t *p_cont = OTF2_getContainerPtrFromName(cont);
  OTF2_EntityValue_t *p_ent = OTF2_getEntityValuePtrFromName(val);

  OTF2_State_t* p_state = malloc(sizeof(OTF2_State_t));
  p_state->value = p_ent;
  p_state->cont = p_cont;

  /* Add the structure to the container state stack. */
  gtg_stack_push(&p_state->token, &p_cont->state_stack.token);

  CHECK_STATUS(OTF2_EvtWriter_Enter( p_cont->evt_writer, NULL, time*TIMER_RES, p_ent->id ));
  return TRACE_SUCCESS;
}

trace_return_t OTF2PopState (varPrec time,
			     const char *type,
			     const char *cont) {


  OTF2_Container_t *p_cont = OTF2_getContainerPtrFromName(cont);
  OTF2_State_t *p_state = gtg_list_entry(gtg_stack_top(&p_cont->state_stack.token), OTF2_State_t, token);
  gtg_stack_pop(&p_cont->state_stack.token);

  OTF2_EntityValue_t *p_ent = p_state->value;

  CHECK_STATUS(OTF2_EvtWriter_Leave( p_cont->evt_writer, NULL, time*TIMER_RES, p_ent->id ));

  free(p_state);
  return TRACE_SUCCESS;
}

trace_return_t OTF2AddEvent (varPrec time,
			     const char *type,
			     const char *cont,
			     const char *val){

  return TRACE_SUCCESS;
}

trace_return_t OTF2StartLink (varPrec time,
			      const char *type,
			      const char *src,
			      const char *dest,
			      const char *val,
			      const char *key){

  OTF2_Container_t *p_dest = OTF2_getContainerPtrFromName(dest);
  OTF2_Container_t *p_src = OTF2_getContainerPtrFromName(src);

  OTF2_Link_t *p_link = NULL;
  alloc_struct(p_link, OTF2_Link_t, &link_list.token);

  init_otf2_string(&p_link->alias, key);
  init_otf2_string(&p_link->name, val);

  p_link->p_src = p_src;
  p_link->p_dest = p_dest;
  p_link->id = get_new_mpi_req_id();

  CHECK_STATUS(OTF2_EvtWriter_MpiSend( p_src->evt_writer,
				       NULL,
				       time*TIMER_RES,
				       p_dest->id,
				       0, /* communicator */
				       p_link->id, /* tag */
				       0)); /* msg length */

  return TRACE_SUCCESS;
}

trace_return_t OTF2EndLink (varPrec time,
			    const char *type,
			    const char *src,
			    const char *dest,
			    const char *val,
			    const char *key){


  OTF2_Link_t *p_link = OTF2_getLinkPtrFromName(key);

  CHECK_STATUS(OTF2_EvtWriter_MpiRecv(p_link->p_dest->evt_writer,
				      NULL,
				      time*TIMER_RES,
				      p_link->p_src->id,
				      0, /* communicator */
				      p_link->id, /* tag */
				      0));	  /* length */
  gtg_list_del(&p_link->token);
  return TRACE_SUCCESS;
}


static OTF2_Variable_t* __create_new_var(OTF2_Container_t *p_cont,
					 OTF2_VariableType_t *p_type) {

 OTF2_Variable_t* p_var = malloc(sizeof(OTF2_Variable_t));
 p_var->p_cont = p_cont;
 p_var->p_var_type = p_type;
 p_var->value = 0;
 gtg_list_add(&p_var->token, &p_cont->variable_list.token);
 return p_var;
}

trace_return_t OTF2SetVar (varPrec time,
			   const char *type,
			   const char *cont,
			   varPrec val) {

  OTF2_Container_t *p_cont = OTF2_getContainerPtrFromName(cont);
  OTF2_VariableType_t *p_type = OTF2_getVariableTypePtrFromName(type);
  OTF2_Variable_t *p_var = OTF2_getVariablePtrFromCont(p_cont, p_type);

  if(!p_var) {
    /* first time this variable is set for this container */
    p_var = __create_new_var(p_cont, p_type);
  }

  p_var->value = (uint64_t) val;

  OTF2_EvtWriter_ParameterUnsignedInt(p_cont->evt_writer,
				      NULL,
				      time*TIMER_RES,
				      p_type->id,
				      p_var->value);
  return TRACE_SUCCESS;
}

trace_return_t OTF2AddVar (varPrec time,
			   const char *type,
			   const char *cont,
			   varPrec val) {

  OTF2_Container_t *p_cont = OTF2_getContainerPtrFromName(cont);
  OTF2_VariableType_t *p_type = OTF2_getVariableTypePtrFromName(type);
  OTF2_Variable_t *p_var = OTF2_getVariablePtrFromCont(p_cont, p_type);

  if(!p_var) {
    /* first time this variable is set for this container */
    p_var = __create_new_var(p_cont, p_type);
  }

  p_var->value += (uint64_t) val;

  OTF2_EvtWriter_ParameterUnsignedInt(p_cont->evt_writer,
				      NULL,
				      time*TIMER_RES,
				      p_type->id,
				      p_var->value);
  return TRACE_SUCCESS;
}

trace_return_t OTF2SubVar (varPrec time,
			   const char *type,
			   const char *cont,
			   varPrec val) {

  return OTF2AddVar(time, type, cont, - (val) );
}

trace_return_t OTF2AddComment (const char *comment){
  /* Not implemented yet */
  return TRACE_ERR_WRITE;
}


trace_return_t OTF2EndTrace () {

  /* At the end, close the archive and exit. All opened event and
     definition writers are closed automatically and the according
     files are created.  */

  OTF2_Archive_Close( archive );

  return TRACE_SUCCESS;
}

#endif /* BUILD_OTF2 */
