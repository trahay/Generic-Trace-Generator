/**
 *  \file GTGOTF_Structs.h
 *  \version 0.1
 *  \brief
 *  OTF_Structs gives the global types and functions needed to have the OTF implementation.
 *
 *  \authors
 *    Developers are : \n
 *        Francois Rue      - francois.rue@labri.fr \n
 *        Francois Trahay   - francois.trahay@labri.fr \n
 *        Johnny   Jazeix   - jazeix@enseirb-matmeca.fr \n
 *        Kevin    Coulomb  - kevin.coulomb@gmail.com \n
 *        Mathieu  Faverge  - faverge@labri.fr \n
 *        Olivier  Lagrasse - lagrasse@enseirb-matmeca.fr \n
 *
 */
#ifndef _GTG_OTF_STRUCTS_H_
#define _GTG_OTF_STRUCTS_H_

#include <stdint.h>
#include "GTGList.h"
#include "GTGStack.h"

/* todo: remove this */
#define MAX_PROCESS 64

/*! StateTypes */
typedef struct StateType { /* Func group */
    char           *name;
    char           *alias;
    int             groupId;
    int             id;
    struct gtg_list token;	/* stored in the stateTypes list */
} StateType_t;

/*! States */
typedef struct State {
    int             value;
    int             cont;
    int             stateType;
    gtg_stack token;	/* stored in the states list */
} State_t;

/* ContainerTypes */
typedef struct ContainerType {
    char           *name;
    char           *alias;
    int             id;
    struct gtg_list token;	/* stored in the ctType list */
} ContainerType_t;

/*! Containers */
typedef struct Container {
    char            *name;
    char            *alias;
    int             ctType;
    int             id;
    struct gtg_list token;	/* stored in the conts list */
    State_t state_stack;
} Container_t;

/*! EntityValue, contains the name of the functions/states */
typedef struct EntityValue {
    char           *name;
    char           *alias;
    int             groupId;
    int             id;
    struct gtg_list token;	/* not used */
} EntityValue_t;

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

struct otf_color {
    char *colorID;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

typedef struct otf_color* otf_color_t;

#define ContainerType_NIL 0
#define Container_NIL     0
#define StateType_NIL     0
#define State_NIL         0
#define EntityValue_NIL   0
#define EventType_NIL     0
#define LinkType_NIL      0
#define VariableType_NIL  0
#define Variable_NIL      0

#define init_ContainerType(var)\
  do {				  \
    (var).name  = NULL;		  \
    (var).alias = NULL;		  \
    (var).id    = ContainerType_NIL;		  \
    GTG_LIST_INIT(&(var).token);  \
  }while(0)

#define init_Container(var)			\
  do {						\
    (var).name   = NULL;			\
    (var).alias  = NULL;			\
    (var).ctType = ContainerType_NIL;		\
    (var).id     = Container_NIL;		\
    GTG_LIST_INIT(&(var).token);		\
    GTG_STACK_INIT(&(var).state_stack.token);	\
  }while(0)

#define init_StateType(var)		  \
  do {					  \
    (var).name    = NULL;		  \
    (var).alias   = NULL;		  \
    (var).groupId = 0;			  \
    (var).id      = StateType_NIL;	  \
    GTG_LIST_INIT(&(var).token);	  \
  }while(0)

#define init_EntityValue(var)			\
  do {						\
    (var).name    = NULL;			\
    (var).alias   = NULL;			\
    (var).groupId = 0;				\
    (var).id      = EntityValue_NIL;		\
    GTG_LIST_INIT(&(var).token);		\
  }while(0)

#define init_EventType(var)			\
  do {						\
    (var).name     = NULL;			\
    (var).alias   = NULL;			\
    (var).contType = ContainerType_NIL;		\
    (var).id       = EventType_NIL;		\
    GTG_LIST_INIT(&(var).token);		\
  }while(0)

#define init_LinkType(var)			\
  do {						\
    (var).name     = NULL;			\
    (var).alias   = NULL;			\
    (var).contType = ContainerType_NIL;		\
    (var).srcType  = ContainerType_NIL;		\
    (var).destType = ContainerType_NIL;		\
    (var).id       = LinkType_NIL;		\
    GTG_LIST_INIT(&(var).token);		\
  }while(0)

#define init_VariableType(var)			\
  do {						\
    (var).name     = NULL;			\
    (var).alias    = NULL;			\
    (var).contType = ContainerType_NIL;		\
    (var).id       = VariableType_NIL;		\
    GTG_LIST_INIT(&(var).token);		\
  }while(0)

#define init_Variable(var)			\
  do {						\
    (var).parent = Container_NIL;		\
    (var).parent = VariableType_NIL;		\
    (var).value  = 0;				\
    (var).id     = Variable_NIL;		\
    GTG_LIST_INIT(&(var).token);		\
  }while(0)

#define init_State(var)				\
  do {						\
    (var).value   = EntityValue_NIL;		\
    (var).cont    = Container_NIL;		\
    (var).stateType = StateType_NIL;		\
    GTG_STACK_INIT(&(var).token);		\
  }while(0)


#define alloc_struct(ptr, type, list_head)				\
  do {									\
    ptr = (type*) malloc(sizeof(type));				\
    GTG_LIST_INIT(&(ptr->token));					\
    ptr->id = (gtg_list_entry((list_head)->prev, type, token)->id) + 1;	\
    gtg_list_add_tail(&(ptr->token), list_head);			\
  } while(0)

#define alloc_init_struct(type, ptr, list_head, _name_, _alias_)	\
  do {									\
  alloc_struct(ptr, type, list_head);					\
  (ptr)->name = (char *)malloc(sizeof(char)*(strlen(_name_)+1));	\
  strcpy((ptr)->name, _name_);						\
  (ptr)->alias = (char *)malloc(sizeof(char)*(strlen(_alias_)+1));	\
  strcpy((ptr)->alias, _alias_);					\
  }while(0)

#define alloc_Variable(_ptr_, _id_, _parent_, _type_, _value_)	\
  do {								\
    (_ptr_) = (Variable_t*) malloc(sizeof(Variable_t));		\
    init_Variable(*(_ptr_));					\
    (_ptr_)->id = _id_;						\
    (_ptr_)->parent = _parent_;					\
    (_ptr_)->type = _type_;					\
    (_ptr_)->value = _value_;					\
  }while(0)

#define alloc_State(_ptr_, _value_, _cont_, _stateType_)	\
  do {								\
    _ptr_ = (State_t*) malloc(sizeof(State_t));			\
    init_State(*(_ptr_));					\
    (_ptr_)->value = _value_;					\
    (_ptr_)->cont = _cont_;					\
    (_ptr_)->stateType = _stateType_;				\
  }while(0)

#define free_struct(_type_, _list_head_)\
  do{\
    _type_ *ptr, *tmp;					\
    gtg_list_for_each_entry_safe(ptr, tmp, &(_list_head_).token, token) {	\
    gtg_list_del(&(ptr->token));\
    free(ptr->name);\
    free(ptr->alias);\
    free(ptr);\
  }\
  }while(0)

#endif /* _GTG_OTF_STRUCTS_H_ */
