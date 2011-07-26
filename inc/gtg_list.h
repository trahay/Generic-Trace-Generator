#ifndef GTG_LIST_H
#define GTG_LIST_H

struct gtg_list {
  struct gtg_list *prev;
  struct gtg_list *next;
};

typedef struct gtg_list* gtg_list_t;

/**
 * \fn GTG_LIST_INIT(ptr)
 * \brief initialize a list.
 * \param ptr pointer to the list (gtg_list_t).
 */
#define GTG_LIST_INIT(ptr)			\
  do {						\
    (ptr)->prev = (ptr);			\
    (ptr)->next = (ptr);			\
  } while(0)

/**
 * \fn GTG_LIST(name)
 * \brief declare and initialize a list.
 * \param name Name of the variable
 */
#define GTG_LIST(name) \
  struct gtg_list name; \
  GTG_LIST_INIT(&name)


/**
 * \fn gtg_list_entry(ptr, type, member)
 * \brief get the structure corresponding to a list entry
 * \param ptr pointer to the list entry (gtg_list_t)
 * \param type the type of the struct this is embedded in.
 * \param member the name of the struct gtg_list member within the struct.
 */
#define gtg_list_entry(ptr, type, member) \
  ((type *)((char *)(ptr) - (char *)(&((type *)0)->member)))


/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __gtg_list_add(gtg_list_t lnew,
				  gtg_list_t prev,
				  gtg_list_t next)
{
  next->prev = lnew;
  lnew->next = next;
  lnew->prev = prev;
  prev->next = lnew;
}

/**
 * \fn void gtg_list_add(gtg_list_t lnew, gtg_list_t head)
 * \brief Insert a new entry after the specified head.
 * \param lnew new entry to be added
 * \param head list head to add it after
 */
static inline void gtg_list_add(gtg_list_t lnew, gtg_list_t head)
{
  __gtg_list_add(lnew, head, head->next);
}

/**
 * \fn void gtg_list_add_tail(gtg_list_t lnew, gtg_list_t head)
 * \brief Insert a new entry before the specified head (ie. at the tail of the list).
 * \param lnew new entry to be added
 * \param head list head to add it after
 */
static inline void gtg_list_add_tail(gtg_list_t lnew, gtg_list_t head)
{
  __gtg_list_add(lnew, head->prev, head);
}

/**
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __gtg_list_del(gtg_list_t prev, gtg_list_t next)
{
        next->prev = prev;
        prev->next = next;
}

/**
 * \fn void gtg_list_del(gtg_list_t entry)
 * \brief delete an entry from its list and reinitialize it.
 * \param entry the element to delete from the list.
 */
static inline void gtg_list_del(gtg_list_t entry)
{
  __gtg_list_del(entry->prev, entry->next);
  GTG_LIST_INIT(entry);
}


/**
 * \fn gtg_list_for_each(gtg_list_t pos, gtg_list_t head)
 * \brief iterate over a list
 * \param pos  the gtg_list_t to use as a loop counter.
 * \param head the head for the list.
 */
#define gtg_list_for_each(pos, head) \
  for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * \fn gtg_list_for_each_reverse(gtg_list_t pos, gtg_list_t head)
 * \brief iterate over a list backwards
 * \param pos  the gtg_list_t to use as a loop counter.
 * \param head the head for the list.
 */
#define gtg_list_for_each_reverse(pos, head)			\
  for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * \fn gtg_list_for_each_safe(gtg_list_t pos, gtg_list_t n, gtg_list_t head)
 * \brief iterate over a list safe against removal of list entry
 * \param pos  the gtg_list_t to use as a loop counter.
 * \param n    another gtg_list_t to use as temporary storage
 * \param head the head for the list.
 */
#define gtg_list_for_each_safe(pos, n, head)			\
  for (pos = (head)->next, n = pos->next; pos != (head);	\
       pos = n, n = pos->next)



/**
 * gtg_list_for_each_entry(pos, head, member)
 * \brief iterate over list of given type
 * \param pos    the type * to use as a loop counter.
 * \param head   the head for the list.
 * \param member the name of the struct gtg_list member within the struct.
 */
#define gtg_list_for_each_entry(pos, head, member)			\
  for (pos = gtg_list_entry((head)->next, typeof(*pos), member);	\
       &pos->member != (head);						\
       pos = gtg_list_entry(pos->member.next, typeof(*pos), member))

/**
 * gtg_list_for_each_entry_safe(pos, n, head, member)
 * \brief iterate over list of given type safe against removal of list entry
 * \param pos    the type * to use as a loop counter.
 * \param n      another type * to use as temporary storage
 * \param head   the head for the list.
 * \param member the name of the struct gtg_list member within the struct.
 */
#define gtg_list_for_each_entry_safe(pos, n, head, member)		\
  for (pos = gtg_list_entry((head)->next, typeof(*pos), member),	\
	 n = gtg_list_entry(pos->member.next, typeof(*pos), member);	\
       &pos->member != (head);						\
       pos = n, n = gtg_list_entry(n->member.next, typeof(*n), member))


static inline int gtg_list_size(gtg_list_t l)
{
  int res = 0;
  gtg_list_t ptr = NULL;
  gtg_list_for_each(ptr, l)
    res++;

  return res;
}

#endif	/* GTG_LIST_H */
