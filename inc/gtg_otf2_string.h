#ifndef GTG_OTF2_STRING_H
#define GTG_OTF2_STRING_H

#include <otf2/otf2.h>
#include "gtg_otf2_macros.h"
#include "gtg_otf2_id.h"

typedef struct otf2_string{
  char* str;
  uint32_t id;
} otf2_string_t;


extern OTF2_GlobalDefWriter* global_def_writer;
extern OTF2_Archive*  archive;

static inline
void init_otf2_string_nil(struct otf2_string *str) {
  str->str = NULL;
  str->id = id_NIL;
}

static inline
void init_otf2_string(struct otf2_string *str, const char* name) {
  asprintf(&str->str, "%s", name);
  str->id = get_new_string_id();

  CHECK_STATUS(OTF2_GlobalDefWriter_WriteString( global_def_writer,
						 str->id,
						 str->str));
}

static inline
struct otf2_string* new_string(const char* str) {
  struct otf2_string* ret = malloc(sizeof(struct otf2_string));
  init_otf2_string(ret, str);
  return ret;
}

#endif /* GTG_OTF2_STRING_H */
