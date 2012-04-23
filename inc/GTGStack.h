#ifndef GTG_STACK_H
#define GTG_STACK_H

#include "GTGList.h"

typedef struct gtg_list gtg_stack;
typedef gtg_stack* gtg_stack_t;

#define GTG_STACK_INIT(ptr) GTG_LIST_INIT(ptr)

#define GTG_STACK(ptr) GTG_LIST(ptr)

#define gtg_stack_entry(ptr, type, member) gtg_list_entry(ptr, type, member)

static inline void gtg_stack_push(gtg_stack_t lnew, gtg_stack_t p_stack)
{
  gtg_list_add_tail(lnew, p_stack);
}

static inline void gtg_stack_pop(gtg_stack_t p_stack)
{
  gtg_list_del(p_stack->prev);
}

static inline gtg_stack_t gtg_stack_top(gtg_stack_t p_stack)
{
  return p_stack->prev;
}

static inline int gtg_stack_empty(gtg_stack_t p_stack)
{
  return p_stack->prev == p_stack;
}

#endif	/* GTG_STACK_H */
