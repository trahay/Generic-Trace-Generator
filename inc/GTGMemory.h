/**
 *  \file GTGMemory.h
 *  \version 0.1
 *  \brief
 *
 *  This file defines a fast allocator for fixed-size blocks
 */

#ifndef GTG_MEMORY_H
#define GTG_MEMORY_H

#include <stdlib.h>


/**
 * \defgroup GTGMemory Memory management
 */

struct gtg_memory {
  void *first_mem;
  void *current_mem;
  size_t block_len;
  long mem_len;
  void *first_free;
  long first_new;
  long nb_allocated;
};
typedef struct gtg_memory *gtg_memory_t;

/**
 * \ingroup GTGMemory
 * \fn void gtg_block_memory_init(gtg_memory_t *memory, size_t block_size, long initial_block_number)
 * \brief Initialize the allocator
 * \param memory A memory describer
 * \param block_size The block size to be allocated when malloc is called
 * \param initial_block_number The number of blocks to allocate initialy
 */
void gtg_block_memory_init(gtg_memory_t *memory, size_t block_size, long initial_block_number);

/**
 * \ingroup GTGMemory
 * \fn void* gtg_block_malloc(gtg_memory_t memory);
 * \brief Allocate a block of data
 * \param memory The memory describer
 * \return A pointer to a block or NULL if allocation failed
 */
void* gtg_block_malloc(gtg_memory_t memory);

/**
 * \ingroup GTGMemory
 * \fn void gtg_block_free(gtg_memory_t memory, void *ptr);
 * \brief Free a block of data
 * \param memory The memory describer
 * \param ptr The block of data to free
 */

void gtg_block_free(gtg_memory_t memory, void *ptr);

#endif
