#include <stdio.h>
#include <string.h>
#include "GTGMemory.h"


#define CTRL_ALLOC(ptr)						\
  {								\
    if(!ptr) {							\
      fprintf(stderr, "Cannot allocate memory. Aborting\n");	\
      abort();							\
    }								\
  }
/* Initialize the allocator */
void gtg_block_memory_init(gtg_memory_t *memory, size_t block_len, long initial_block_number) {
  gtg_memory_t temp_mem = NULL;
  /* allocate the memory descriptor */
  temp_mem = (gtg_memory_t) malloc(sizeof(struct gtg_memory));
  CTRL_ALLOC(temp_mem);

  if (block_len < sizeof(void *)) {
    block_len = sizeof(void *);
  }

  /* allocate n * block_len */
  temp_mem->first_mem = malloc((initial_block_number * block_len) + sizeof(void *));
  CTRL_ALLOC(temp_mem->first_mem);
  memset(temp_mem->first_mem, 0, (initial_block_number * block_len) + sizeof(void *));

  temp_mem->current_mem = temp_mem->first_mem;

  *(void **) ((char *) temp_mem->current_mem +
	      (initial_block_number * block_len)) = NULL;

  temp_mem->block_len = block_len;
  temp_mem->mem_len = initial_block_number;
  temp_mem->first_free = NULL;
  temp_mem->first_new = 0;
  temp_mem->nb_allocated = 0;

  *memory = temp_mem;
}

/* Allocate a block of data */
void* gtg_block_malloc(gtg_memory_t memory) {
  void *ptr = NULL;

  if (memory->first_free != NULL) {
    /* There is a remaining block in memory */
    ptr = memory->first_free;
    memory->first_free = *(void **) ptr;
  } else {
    if (memory->first_new >= memory->mem_len) {
      /* The allocated memory is exhausted. Allocate a larger space. */
      const size_t mem_size = memory->mem_len * memory->block_len;
      void *new_mem = malloc(mem_size + sizeof(void *));
      memset(new_mem, 0, mem_size + sizeof(void *));

      *(void **) ((char *) new_mem + mem_size) = NULL;
      *(void **) ((char *) memory->current_mem + mem_size) = new_mem;
      memory->current_mem = new_mem;
      memory->first_new = 0;
    }

    ptr = (char *) memory->current_mem + memory->block_len * memory->first_new;
    memory->first_new++;
  }
  memory->nb_allocated++;

  return ptr;
}

/* Free a block of data */
void gtg_block_free(gtg_memory_t memory, void *ptr) {
  *(void **) ptr = memory->first_free;
  memory->first_free = ptr;
  memory->nb_allocated--;
}
