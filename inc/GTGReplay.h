/**
 *  \file GTGReplay.h
 *  \version 0.1
 *  \brief
 *
 *  This file defines functions for postponing event-processing function calls
 */

#ifndef GTG_REPLAY_H
#define GTG_REPLAY_H

/**
 * \defgroup GTGReplay Functions for postponing event-processing function calls
 */
enum event_type_t {
  event_addContainer,
  event_destroyContainer,
  event_setState,
  event_pushState,
  event_popState,
  event_addEvent,
  event_startLink,
  event_endLink,
  event_setVar,
  event_addVar,
  event_subVar
};

/**
 * \ingroup GTGReplay
 * \fn void gtg_record(enum event_type_t type, varPrec time, ...)
 * \brief postpone the recording of an event
 * \param type The type of function to postpone
 * \param time The time at which the event happens
 */
void gtg_record(enum event_type_t type, varPrec time, ...);

/**
 * \ingroup GTGReplay
 * \fn void gtg_write_events(long nb_events_to_write)
 * \brief run the first nb_events_to_write events
 * \param nb_events_to_write The number of functions to process (-1 for all functions)
 */
void gtg_write_events(long nb_events_to_write);

#endif /* GTG_REPLAY_H */
