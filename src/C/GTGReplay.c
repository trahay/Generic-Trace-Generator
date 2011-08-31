#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "GTGBasic1.h"
#include "GTGReplay.h"
#include "GTGMemory.h"

#define MAX_EVENTS (32*1024)
extern gtg_flag_t gtg_flags;

struct event_list_t {
  struct event_list_t *next;
  struct event_list_t *prev;
  enum event_type_t type;
  varPrec time;
  uint64_t data[6];
};

static struct event_list_t* __first_event = NULL;
static struct event_list_t* __last_event = NULL;
static int nb_events = 0;

gtg_memory_t memory;

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)


/* for debugging purpose, print the list of recorded events */
static void __gtg_print_events()
{
  struct event_list_t* cur_event = __first_event;
  while(cur_event) {
    printf("[%p]\t T=%f \t event type=%d\n", (void*)cur_event, cur_event->time, cur_event->type);
    cur_event = cur_event->next;
  }
}

/* run the first nb_events_to_write events
 * if nb_events_to_write == 1, it run all recorded events.
 */
void gtg_write_events(long nb_events_to_write)
{
  struct event_list_t* cur_event = __first_event;
  struct event_list_t* prev;
  struct event_list_t* next;
  void* ptr;
  volatile int n = 0;

  /* unset the outoforder flag so that functions can be called for real  */
  gtg_flags ^= GTG_FLAG_OUTOFORDER;

  while(cur_event &&
	((nb_events_to_write<0) ||(n < nb_events_to_write))) {

    switch(cur_event->type) {
    case event_addContainer:
      /* get the 5 parameters that were recorded and pass them to addContainer */
      addContainer(cur_event->time, (char*)cur_event->data[0],
		   (char*)cur_event->data[1],
		   (char*)cur_event->data[2],
		   (char*)cur_event->data[3],
		   (char*)cur_event->data[4]);
      break;
    case event_destroyContainer:
      destroyContainer(cur_event->time, (char*)cur_event->data[0],
		       (char*)cur_event->data[1]);
      break;
    case event_setState:
      setState(cur_event->time, (char*)cur_event->data[0],
	       (char*)cur_event->data[1],
	       (char*)cur_event->data[2]);
      break;
    case event_pushState:
      pushState(cur_event->time, (char*)cur_event->data[0],
		(char*)cur_event->data[1],
		(char*)cur_event->data[2]);
      break;
    case event_popState:
      popState(cur_event->time, (char*)cur_event->data[0],
	       (char*)cur_event->data[1]);
      break;
    case event_addEvent:
      addEvent(cur_event->time, (char*)cur_event->data[0],
	       (char*)cur_event->data[1],
	       (char*)cur_event->data[2]);
      break;
    case event_startLink:
      startLink(cur_event->time, (char*)cur_event->data[0],
		(char*)cur_event->data[1],
		(char*)cur_event->data[2],
		(char*)cur_event->data[3],
		(char*)cur_event->data[4],
		(char*)cur_event->data[5]);
      break;
    case event_endLink:
      endLink(cur_event->time, (char*)cur_event->data[0],
	      (char*)cur_event->data[1],
	      (char*)cur_event->data[2],
	      (char*)cur_event->data[3],
	      (char*)cur_event->data[4],
	      (char*)cur_event->data[5]);
      break;

    case event_setVar:
      setVar(cur_event->time, (char*)cur_event->data[0],
	     (char*)cur_event->data[1],
	     (double)cur_event->data[2]);
      break;
    case event_addVar:
      addVar(cur_event->time, (char*)cur_event->data[0],
	     (char*)cur_event->data[1],
	     (double)cur_event->data[2]);
      break;
    case event_subVar:
      subVar(cur_event->time, (char*)cur_event->data[0],
	     (char*)cur_event->data[1],
	     (double)cur_event->data[2]);
      break;
    default:
      fprintf(stderr, "Unknown event type: %d\n", cur_event->type);
    }

    /* remove the current item from the list */
    prev = cur_event->prev;
    next = cur_event->next;
    if(prev) prev->next = next;
    if(next) next->prev = prev;

    n++;
    nb_events--;
    /* delete the current event */
    ptr = cur_event;
    cur_event = next;

    gtg_block_free(memory, ptr);
  }
  __first_event = cur_event;

  /* set the outoforder flag so that the next calls to functions can be recorded */
  gtg_flags |= GTG_FLAG_OUTOFORDER;

}

/* Insert an event in the list of events.
 * At any time, this list is sorted.
 */
static void __gtg_insert(struct event_list_t* new_event) {

  struct event_list_t* cur_event = NULL;
  struct event_list_t* next_event = NULL;
  struct event_list_t* prev_event = NULL;

  /* The list of event is not initialized yet */
  if(!__last_event || !__first_event) {
    __last_event = new_event;
    __first_event = new_event;
    new_event->next = NULL;
    new_event->prev = NULL;
    return ;
  }

  /* This is the 'normal' case: the event is the last one */
  if(likely(new_event->time >= __last_event->time))
  {
    /* the event is the last one */
    __last_event->next = new_event;
    new_event->prev = __last_event;
    new_event->next = NULL;
    __last_event = new_event;
    return ;
  }

  if(new_event->time <= __first_event->time) {
    /* the event is the first one */
    __first_event->prev = new_event;
    new_event->next = __first_event;
    new_event->prev = NULL;
    __first_event = new_event;
    return ;
  }

  /* the last event happened after new_event.
   * let's browse the list and find a place to insert new_event.
   */
  cur_event = __first_event;
  next_event = __first_event->next;
  if(unlikely(!next_event)) {
    /* only one event in the list. We know that it happened after new_event.
     * let's insert new_event in the first position
     */

    /* this should never happen */
    new_event->next = __first_event;
    __first_event->prev = new_event;
    __first_event = new_event;
    return ;
  }

  /* browse the list starting from the end (since it is likely that new_event
   * happened not so long before last_event.
   */
  cur_event = __last_event;
  prev_event = cur_event->prev;

  while(prev_event->time > new_event->time) {
    cur_event = prev_event;
    prev_event = prev_event->prev;
  }

  /* insert new_event between cur_event and next_event */
  new_event->next = cur_event;
  new_event->prev = prev_event;

  cur_event->prev = new_event;
  prev_event->next = new_event;
}

/* parameters are concatenated and separated with the \3 byte */
#define FORMAT_SIX_STRING "%s\3%s\3%s\3%s\3%s\3%s"
#define FORMAT_FIVE_STRING "%s\3%s\3%s\3%s\3%s"
#define FORMAT_THREE_STRING "%s\3%s\3%s"
#define FORMAT_TWO_STRING_ONE_DOUBLE "%s\3%s\3%e"
#define FORMAT_TWO_STRING "%s\3%s"

/* copy nb_string string parameters and nb_double double parameters into the data arary of evt
 * warning: actually, the parameters are not copied (in the case of string)
 */
static void __copy_args(struct event_list_t* evt, int nb_string, int nb_double, va_list args)
{
  int i;
  for(i=0; i<nb_string; i++) {
    const char*x = va_arg(args, char*);
    evt->data[i] = (uint64_t) x;
  }

  for(i=nb_string; i<nb_string+nb_double; i++) {
    double x = va_arg(args, double);
    evt->data[i] = (uint64_t)x;
  }
}

/* postpone the recording of an event */
void gtg_record(enum event_type_t type, varPrec time, ...) {
  struct event_list_t* new_event;
  va_list arguments;

  static int initialized=0;
  if(!initialized) {
    /* initialize the block allocator */
    gtg_block_memory_init(&memory, sizeof(struct event_list_t), MAX_EVENTS);
    initialized = 1;
  }

  /* If the buffer is full, flush half of the recorded events.
   * There may be out of order events, but it is quite unlikely.
   */
  if(nb_events > MAX_EVENTS)
    gtg_write_events(MAX_EVENTS/2);

  /* allocate the new event */
  new_event = gtg_block_malloc(memory);

  new_event->next = NULL;
  new_event->type = type;
  new_event->time = time;

  /* Depending on the type of event, copy its parameters */
  switch(type) {
  case event_startLink:
  case event_endLink:
    /* 6 parameters */
    va_start (arguments, FORMAT_SIX_STRING);
    __copy_args(new_event, 6, 0, arguments);
    break;

  case event_addContainer:
    /* 5 parameters */
    va_start (arguments, FORMAT_FIVE_STRING);
    __copy_args(new_event, 5, 0, arguments);
    break;

  case event_setState:
  case event_pushState:
  case event_addEvent:
    /* 3 parameters */
    va_start (arguments, FORMAT_THREE_STRING);
    __copy_args(new_event, 3, 0, arguments);
    break;
  case event_setVar:
  case event_addVar:
  case event_subVar:
    /* 2 string parameters + 1 double */
    va_start (arguments, FORMAT_TWO_STRING_ONE_DOUBLE);
    __copy_args(new_event, 2, 1, arguments);
    break;

  case event_destroyContainer:
  case event_popState:
    /* 2 parameters */
    va_start (arguments, FORMAT_TWO_STRING);
    __copy_args(new_event, 2, 0, arguments);
    break;
  default:
    fprintf(stderr, "Unknown event type: %d\n", type);
  }

  va_end (arguments);
  /* Insert the new event in the list */
  nb_events++;
  __gtg_insert(new_event);
}
