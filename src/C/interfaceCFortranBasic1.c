#include <stdlib.h>
#include <string.h>
#include "GTG.h"

void
setTraceType_i (int *type)
{
  setTraceType ((traceType_t) * type);
}

int
getTraceType_i ()
{
  return getTraceType ();
}

int bufferedModeActivated_i()
{
  return bufferedModeActivated_i();
}

void
initTrace_i (char *f, int *len, int *rank, gtg_flag_t* flags, int *err)
{
  char *filename = (char *) malloc (sizeof (char) * (*len + 1));
  memcpy (filename, f, *len);
  filename[*len] = '\0';
  *err = initTrace (filename, *rank, *flags);
}

void
addContType_i (char *alias, int *s1,
	       char *contType, int *s2, char *name, int *s3, int *err)
{
  char *a = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *n = (char *) malloc (sizeof (char) * (*s3 + 1));
  memcpy (a, alias, *s1);
  a[*s1] = '\0';
  memcpy (c, contType, *s2);
  c[*s2] = '\0';
  memcpy (n, name, *s3);
  n[*s3] = '\0';
  *err = addContType (a, c, n);
}



void
addStateType_i (char *alias, int *s1,
		char *contType, int *s2, char *name, int *s3, int *err)
{
  char *a = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *n = (char *) malloc (sizeof (char) * (*s3 + 1));
  memcpy (a, alias, *s1);
  a[*s1] = '\0';
  memcpy (c, contType, *s2);
  c[*s2] = '\0';
  memcpy (n, name, *s3);
  n[*s3] = '\0';
  *err = addStateType (a, c, n);
}

void
addEventType_i (char *alias, int *s1,
		char *contType, int *s2, char *name, int *s3, int *err)
{
  char *a = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *n = (char *) malloc (sizeof (char) * (*s3 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s2 + 1));
  memcpy (a, alias, *s1);
  a[*s1] = '\0';
  memcpy (c, contType, *s2);
  c[*s2] = '\0';
  memcpy (n, name, *s3);
  n[*s3] = '\0';
  *err = addEventType (a, c, n);
}

void
addLinkType_i (char *alias, int *s1,
	       char *name, int *s3,
	       char *contType, int *s2,
	       char *src, int *s4, char *dest, int *s5, int *err)
{
  char *a = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *d = (char *) malloc (sizeof (char) * (*s5 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *n = (char *) malloc (sizeof (char) * (*s3 + 1));
  char *s = (char *) malloc (sizeof (char) * (*s4 + 1));
  memcpy (a, alias, *s1);
  a[*s1] = '\0';
  memcpy (c, contType, *s2);
  c[*s2] = '\0';
  memcpy (n, name, *s3);
  n[*s3] = '\0';
  memcpy (s, src, *s4);
  s[*s4] = '\0';
  memcpy (d, dest, *s5);
  d[*s5] = '\0';
  *err = addLinkType (a, n, c, s, d);
}

void
addVarType_i (char *alias, int *s1,
	      char *name, int *s3, char *contType, int *s2, int *err)
{
  char *a = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *n = (char *) malloc (sizeof (char) * (*s3 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s2 + 1));
  memcpy (a, alias, *s1);
  a[*s1] = '\0';
  memcpy (c, contType, *s2);
  c[*s2] = '\0';
  memcpy (n, name, *s3);
  n[*s3] = '\0';
  *err = addVarType (a, c, n);
}

void
addEntityValue_i (char *alias, int *s1,
		  char *entType, int *s2,
		  char *name, int *s3, gtg_color_t color, int *s4, int *err)
{

  char *a = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *n = (char *) malloc (sizeof (char) * (*s3 + 1));
  memcpy (a, alias, *s1);
  a[*s1] = '\0';
  memcpy (c, entType, *s2);
  c[*s2] = '\0';
  memcpy (n, name, *s3);
  n[*s3] = '\0';
  *err = addEntityValue (a, c, n, color);
}

void
addContainer_i (varPrec * time,
		char *alias, int *s1,
		char *type, int *s2,
		char *container, int *s3,
		char *name, int *s4, char *filename, int *s5, int *err)
{
  char *a = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s3 + 1));
  char *n = (char *) malloc (sizeof (char) * (*s4 + 1));
  char *f = (char *) malloc (sizeof (char) * (*s5 + 1));
  memcpy (a, alias, *s1);
  a[*s1] = '\0';
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, container, *s3);
  c[*s3] = '\0';
  memcpy (n, name, *s4);
  n[*s4] = '\0';
  memcpy (f, filename, *s5);
  f[*s5] = '\0';
  *err = addContainer (*time, a, t, c, n, f);
}

void
destroyContainer_i (varPrec * time,
		    char *name, int *s1, char *type, int *s2, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *n = (char *) malloc (sizeof (char) * (*s1 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (n, name, *s1);
  n[*s1] = '\0';
  *err = destroyContainer (*time, n, t);
}

void
setState_i (varPrec * time,
	    char *type, int *s2,
	    char *cont, int *s1, char *val, int *s3, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *v = (char *) malloc (sizeof (char) * (*s3 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  memcpy (v, val, *s3);
  v[*s3] = '\0';
  *err = setState (*time, t, c, v);
}

void
pushState_i (varPrec * time,
	     char *type, int *s2,
	     char *cont, int *s1, char *val, int *s3, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *v = (char *) malloc (sizeof (char) * (*s3 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  memcpy (v, val, *s3);
  v[*s3] = '\0';
  *err = pushState (*time, t, c, v);
}

void
popState_i (varPrec * time,
	    char *type, int *s2,
	    char *cont, int *s1, int *s3, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  *err = popState (*time, t, c);
}

void
addEvent_i (varPrec * time,
	    char *type, int *s2,
	    char *cont, int *s1, char *val, int *s3, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *v = (char *) malloc (sizeof (char) * (*s3 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  memcpy (v, val, *s3);
  v[*s3] = '\0';
  *err = addEvent (*time, t, c, v);
}

void
startLink_i (varPrec * time,
	     char *type, int *s2,
	     char *cont, int *s1,
	     char *src, int *s3,
	     char *dest, int *s4,
	     char *val, int *s5, char *key, int *s6, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *k = (char *) malloc (sizeof (char) * (*s6 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *v = (char *) malloc (sizeof (char) * (*s5 + 1));
  char *s = (char *) malloc (sizeof (char) * (*s3 + 1));
  char *d = (char *) malloc (sizeof (char) * (*s4 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  memcpy (v, val, *s5);
  v[*s5] = '\0';
  memcpy (s, src, *s3);
  s[*s3] = '\0';
  memcpy (d, dest, *s4);
  d[*s4] = '\0';
  memcpy (k, key, *s6);
  k[*s6] = '\0';
  *err = startLink (*time, t, c, s, d, v, k);
}

void
endLink_i (varPrec * time,
	   char *type, int *s2,
	   char *cont, int *s1,
	   char *src, int *s3,
	   char *dest, int *s4,
	   char *val, int *s5, char *key, int *s6, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *k = (char *) malloc (sizeof (char) * (*s6 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  char *v = (char *) malloc (sizeof (char) * (*s5 + 1));
  char *s = (char *) malloc (sizeof (char) * (*s3 + 1));
  char *d = (char *) malloc (sizeof (char) * (*s4 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  memcpy (v, val, *s5);
  v[*s5] = '\0';
  memcpy (s, src, *s3);
  s[*s3] = '\0';
  memcpy (d, dest, *s4);
  d[*s4] = '\0';
  memcpy (k, key, *s6);
  k[*s6] = '\0';
  *err = endLink (*time, t, c, s, d, v, k);
}

void
setVar_i (varPrec * time,
	  char *type, int *s2, char *cont, int *s1, varPrec * val, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  *err = setVar (*time, t, c, *val);
}

void
addVar_i (varPrec * time,
	  char *type, int *s2, char *cont, int *s1, varPrec * val, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  *err = addVar (*time, t, c, *val);
}

void
subVar_i (varPrec * time,
	  char *type, int *s2, char *cont, int *s1, varPrec * val, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  char *c = (char *) malloc (sizeof (char) * (*s1 + 1));
  memcpy (t, type, *s2);
  t[*s2] = '\0';
  memcpy (c, cont, *s1);
  c[*s1] = '\0';
  *err = subVar (*time, t, c, *val);
}

void
addComm_i (char *comm, int *s2, int *err)
{
  char *t = (char *) malloc (sizeof (char) * (*s2 + 1));
  memcpy (t, comm, *s2);
  t[*s2] = '\0';
  *err = AddComment (t);
}


void
endTrace_i (int *err)
{
  *err = endTrace ();
}


/* manual aliasing of function, in order to avoid using GCC specific __attribute__ keyword */
void
settracetype_i_ (int *type)
{
    setTraceType_i (type);
}

int
gettracetype_i_ ()
{
  return getTraceType_i ();
}

int bufferedmodeactivated_i_()
{
  return bufferedModeActivated_i();
}

void
inittrace_i_ (char *f, int *len, int*rank, gtg_flag_t* flags, int *err)
{
  initTrace_i (f, len, rank, flags, err);
}

void
addconttype_i_ (char *alias, int *s1,
		char *contType, int *s2, char *name, int *s3, int *err)
{
  addContType_i (alias, s1, contType, s2, name, s3, err);
}

void
addstatetype_i_ (char *alias, int *s1,
		 char *contType, int *s2, char *name, int *s3, int *err)
{
  addStateType_i (alias, s1, contType, s2, name, s3, err);
}

void
addeventtype_i_ (char *alias, int *s1,
		 char *contType, int *s2, char *name, int *s3, int *err)
{
  addEventType_i (alias, s1, contType, s2, name, s3, err);
}

void
addlinktype_i_ (char *alias, int *s1,
		char *name, int *s3,
		char *contType, int *s2,
		char *src, int *s4, char *dest, int *s5, int *err)
{
  addLinkType_i (alias, s1, name, s3, contType, s2, src, s4, dest, s5, err);
}

void
addvartype_i_ (char *alias, int *s1,
	       char *name, int *s3, char *contType, int *s2, int *err)
{
  addVarType_i (alias, s1, name, s3, contType, s2, err);
}

void
addentityvalue_i_ (char *alias, int *s1,
		   char *entType, int *s2,
		   char *name, int *s3, gtg_color_t color, int *s4, int *err)
{
  addEntityValue_i (alias, s1, entType, s2, name, s3, color, s4, err);
}

void
addcontainer_i_ (varPrec * time,
		 char *alias, int *s1,
		 char *type, int *s2,
		 char *container, int *s3,
		 char *name, int *s4, char *filename, int *s5, int *err)
{
  addContainer_i (time,
		  alias, s1,
		  type, s2, container, s3, name, s4, filename, s5, err);
}

void
destroycontainer_i_ (varPrec * time,
		     char *name, int *s1, char *type, int *s2, int *err)
{
  destroyContainer_i (time, name, s1, type, s2, err);
}

void
setstate_i_ (varPrec * time,
	     char *type, int *s2,
	     char *cont, int *s1, char *val, int *s3, int *err)
{
  setState_i (time, type, s2, cont, s1, val, s3, err);
}

void
pushstate_i_ (varPrec * time,
	      char *type, int *s2,
	      char *cont, int *s1, char *val, int *s3, int *err)
{
  pushState_i (time, type, s2, cont, s1, val, s3, err);
}

void
popstate_i_ (varPrec * time,
	     char *type, int *s2,
	     char *cont, int *s1, int *s3, int *err)
{
  popState_i (time, type, s2, cont, s1, s3, err);
}

void
addevent_i_ (varPrec * time,
	     char *type, int *s2,
	     char *cont, int *s1, char *val, int *s3, int *err)
{
  addEvent_i (time, type, s2, cont, s1, val, s3, err);
}

void
startlink_i_ (varPrec * time,
	      char *type, int *s2,
	      char *cont, int *s1,
	      char *src, int *s3,
	      char *dest, int *s4,
	      char *val, int *s5, char *key, int *s6, int *err)
{
  startLink_i (time,
	       type, s2, cont, s1, src, s3, dest, s4, val, s5, key, s6, err);
}

void
endlink_i_ (varPrec * time,
	    char *type, int *s2,
	    char *cont, int *s1,
	    char *src, int *s3,
	    char *dest, int *s4,
	    char *val, int *s5, char *key, int *s6, int *err)
{
  endLink_i (time,
	     type, s2, cont, s1, src, s3, dest, s4, val, s5, key, s6, err);
}

void
setvar_i_ (varPrec * time,
	   char *type, int *s2, char *cont, int *s1, varPrec * val, int *err)
{
  setVar_i (time, type, s2, cont, s1, val, err);
}

void
addvar_i_ (varPrec * time,
	   char *type, int *s2, char *cont, int *s1, varPrec * val, int *err)
{
  addVar_i (time, type, s2, cont, s1, val, err);
}

void
subvar_i_ (varPrec * time,
	   char *type, int *s2, char *cont, int *s1, varPrec * val, int *err)
{
  subVar_i (time, type, s2, cont, s1, val, err);
}

void
addcomm_i_ (char *comm, int *s2, int *err)
{
  addComm_i (comm, s2, err);
}

void
endtrace_i_ (int *err)
{
  endTrace_i (err);
}
