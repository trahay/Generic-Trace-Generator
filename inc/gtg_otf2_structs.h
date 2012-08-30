#ifndef GTG_OTF2_STRUCTS_H
#define GTG_OTF2_STRUCTS_H

#include <stdint.h>
#include "GTGList.h"
#include "GTGStack.h"

struct OTF2_State;
struct OTF2_EntityValue;
struct OTF2_Container;

/*! StateTypes */
typedef struct StateType { /* Func group */
  struct otf2_string name;
  struct otf2_string alias;
  int                groupId;
  uint32_t           id;
  struct gtg_list    token;	/* stored in the stateTypes list */
} StateType_t;


/* ContainerTypes */
typedef struct ContainerType {
    char           *name;
    char           *alias;
    int             id;
    struct gtg_list token;	/* stored in the ctType list */
} ContainerType_t;




/*! States */
typedef struct OTF2_State {
  struct OTF2_EntityValue *value;
  struct OTF2_Container  *cont;
  gtg_stack token;	/* stored in the states list */
} OTF2_State_t;

/*! Containers */
typedef struct OTF2_Container {
  struct otf2_string alias;
  struct otf2_string name;
  OTF2_EvtWriter*    evt_writer;
  uint32_t           id;
  struct gtg_list token;	/* stored in the conts list */
  OTF2_State_t state_stack;
} OTF2_Container_t;





typedef struct gtg_otf2_process {
  struct otf2_string name;
} gtg_otf2_process_t;

/*! EntityValue, contains the name of the functions/states */
typedef struct OTF2_EntityValue {
  struct otf2_string alias;
  struct otf2_string name;
  uint32_t           id;
  struct gtg_list    token;	/* not used */
} OTF2_EntityValue_t;

/*! Events/Markers */
typedef struct EventType {
    char           *name;
    char           *alias;
    int             contType;
    int             id;
    struct gtg_list token;	/* stored in the eventTypes list */
} EventType_t;

/*! Links/Messages */
typedef struct LinkType {
    char           *name;
    char           *alias;
    int             contType;
    int             srcType;
    int             destType;
    int             id;
    struct gtg_list token;	/* stored in the linkTypes lisk */
} LinkType_t;

typedef struct Link {
    varPrec time;
    int src;
} Link_t;


/*! Variables/Counters */
typedef struct VariableType {
    char           *name;
    char           *alias;
    int             contType;
    int             id;
    struct gtg_list token;	/* stored in the variableTypes list */
} VariableType_t;

typedef struct Variable {
    int             parent;
    int             type;
    uint64_t        value;
    int             id;
    struct gtg_list token;	/* stored in the variables list */
} Variable_t;



#define alloc_struct(ptr, type, list_head)				\
  do {									\
    ptr = (type*) malloc(sizeof(type));					\
    GTG_LIST_INIT(&(ptr->token));					\
    ptr->id = (gtg_list_entry((list_head)->prev, type, token)->id) + 1;	\
    gtg_list_add_tail(&(ptr->token), list_head);			\
  } while(0)


#define free_struct(_type_, _list_head_)				\
  do{									\
    _type_ *ptr, *tmp;							\
    gtg_list_for_each_entry_safe(ptr, tmp, &(_list_head_).token, token) { \
      gtg_list_del(&(ptr->token));					\
      free(ptr);							\
    }									\
  }while(0)



static inline
void OTF2_init_Container(OTF2_Container_t *p_cont)
{
  init_otf2_string_nil(&p_cont->alias);
  init_otf2_string_nil(&p_cont->name);
  p_cont->evt_writer = NULL;
  p_cont->id = id_NIL;
  GTG_LIST_INIT(&p_cont->token);
  GTG_STACK_INIT(&p_cont->state_stack.token);
}


static inline
void OTF2_init_EntityValue(OTF2_EntityValue_t *p_ent)
{
  init_otf2_string_nil(&p_ent->alias);
  init_otf2_string_nil(&p_ent->name);
  p_ent->id = id_NIL;
  GTG_LIST_INIT(&p_ent->token);
}

#endif /* GTG_OTF2_STRUCTS_H */
