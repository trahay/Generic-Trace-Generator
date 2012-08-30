#ifndef GTG_OTF2_H
#define GTG_OTF2_H

#include "GTG.h"
#include "gtg_otf2_id.h"
#include "gtg_otf2_string.h"
#include "gtg_otf2_structs.h"

struct otf2_color {
    char *colorID;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

typedef struct otf_color* otf_color_t;


extern struct gtg_otf2_process *process_array;
extern unsigned nb_process;



trace_return_t OTF2InitTrace(const char *filenam,
			     gtg_flag_t flags);


trace_return_t OTF2SetCompress(int val);

trace_return_t OTF2AddContType (const char *alias,
				const char *contType,
				const char *name);

trace_return_t OTF2AddStateType (const char *alias,
				 const char *contType,
				 const char *name);

trace_return_t OTF2AddEventType (const char *alias,
				 const char *contType,
				 const char *name);

trace_return_t OTF2AddLinkType (const char *alias,
				const char *name,
				const char *contType,
				const char *srcContType,
				const char *destContType);

trace_return_t OTF2AddVarType (const char *alias,
			       const char *contType,
			       const char *name);

trace_return_t OTF2AddEntityValue (const char *alias,
				   const char *entType,
				   const char *name,
				   const otf_color_t color);

trace_return_t OTF2DefineContainer (const char *alias,
				    const char *type,
				    const char *container,
				    const char *name,
				    const char *file);

trace_return_t OTF2StartContainer (varPrec time,
				   const char *alias,
				   const char *type,
				   const char *container,
				   const char *name,
				   const char *file);

static inline
trace_return_t OTF2AddContainer (varPrec time,
				 const char *alias,
				 const char *type,
				 const char *container,
				 const char *name,
				 const char *file)
{
  OTF2DefineContainer(alias, type, container, name, file);
  return OTF2StartContainer(time, alias, type, container, name, file);
}

trace_return_t OTF2SeqAddContainer (varPrec time,
				    const char *alias,
				    const char *type,
				    const char *container,
				    const char *name);

trace_return_t OTF2DestroyContainer (varPrec time,
				     const char *name,
				     const char *type);

trace_return_t OTF2SetState (varPrec time,
			     const char *type,
			     const char *cont,
			     const char *val);

trace_return_t OTF2PushState (varPrec time,
			      const char *type,
			      const char *cont,
			      const char *val);

trace_return_t OTF2PopState (varPrec time,
			     const char *type,
			     const char *cont);

trace_return_t OTF2AddEvent (varPrec time,
			     const char *type,
			     const char *cont,
			     const char *val);

trace_return_t OTF2StartLink (varPrec time,
			      const char *type,
			      const char *src,
			      const char *dest,
			      const char *val,
			      const char *key);

trace_return_t OTF2EndLink (varPrec time,
			    const char *type,
			    const char *src,
			    const char *dest,
			    const char *val,
			    const char *key);

trace_return_t OTF2SetVar (varPrec time,
			   const char *type,
			   const char *cont,
			   varPrec val);

trace_return_t OTF2AddVar (varPrec time,
			   const char *type,
			   const char *cont,
			   varPrec val);

trace_return_t OTF2SubVar (varPrec time,
			   const char *type,
			   const char *cont,
			   varPrec val);

trace_return_t OTF2AddComment (const char *comment);

trace_return_t OTF2EndTrace ();

#endif /* GTG_OTF2_H */
