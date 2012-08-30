#ifndef GTG_OTF2_MACROS_H
#define GTG_OTF2_MACROS_H

/* Furthermore this example uses a function delivering dummy timestamps. Real world applications will use a timer like gettimeofday. */
static uint64_t gtg_otf2_get_time( void )
{
  static uint64_t sequence;
  return sequence++;
}

#define CHECK_STATUS(__func__) {				\
    if(__func__ != SCOREP_SUCCESS) {				\
      fprintf(stderr, "Error while writing the OTF trace\n");	\
      abort();							\
    }								\
  }


#define CHECK_PTR(ptr) {			\
  if(!(ptr)) {					\
      fprintf(stderr, "Error while writing the OTF trace\n");	\
      abort();							\
  }\
}


#endif /* GTG_OTF2_MACROS_H */
