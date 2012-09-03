#ifndef GTG_OTF2_ID_H
#define GTG_OTF2_ID_H

enum id_type {
  string,
  process,
  function,
  variable,
  mpi_req,
  last_id
};

extern uint32_t id_NIL;

uint32_t seq_array[last_id];

static uint32_t get_new_id(enum id_type type) {
  return seq_array[type]++;
}

#define get_new_string_id() get_new_id(string)

#define get_new_process_id() get_new_id(process)

#define get_new_function_id() get_new_id(function)

#define get_new_var_id() get_new_id(variable)

#define get_new_mpi_req_id() get_new_id(mpi_req)


static inline
void  init_id() {
  int i;
  for(i=0; i<last_id; i++)
    seq_array[i] = 0;

  id_NIL = -1;
}

#endif	/* GTG_OTF2_ID_H */
