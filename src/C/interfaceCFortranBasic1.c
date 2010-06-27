#include "../../inc/GTGBasic1.h"

void setTraceType_i (int* type){
    setTraceType (*type);
}

int getTraceType_i (){
    return getTraceType ();
}

void initTrace_i (char* f, int* len, int* err){
    char* filename = (char *)malloc (sizeof (char)*(*len+1));
    memcpy (filename, f, *len);
    filename[*len]='\0';
    *err = initTrace (filename);
}

void setComm_i (MPI_Comm* c, int* err){
    *err = setComm (*c);
}

void addProcType_i (char* alias   , int* s1, 
                    char* contType, int* s2,
                    char* name    , int* s3, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    *err = addProcType (a, c, n);
}


void addProcTypeNB_i (char* alias   , int* s1, 
                      char* contType, int* s2,
                      char* name    , int* s3, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    *err = addProcTypeNB (a, c, n);
}




void addStateType_i (char* alias   , int* s1, 
                     char* contType, int* s2,
                     char* name    , int* s3, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    *err = addStateType (a, c, n);
}


void addStateTypeNB_i (char* alias   , int* s1, 
                       char* contType, int* s2,
                       char* name    , int* s3, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    *err = addStateTypeNB (a, c, n);
}


void addEventType_i (char* alias   , int* s1, 
                     char* contType, int* s2,
                     char* name    , int* s3, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    *err = addEventType (a, c, n);
}


void addEventTypeNB_i (char* alias   , int* s1, 
                       char* contType, int* s2,
                       char* name    , int* s3, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    *err = addEventTypeNB (a, c, n);
}


void addLinkType_i (char* alias   , int* s1, 
                    char* name    , int* s3,
                    char* contType, int* s2,
                    char* src     , int* s4,
                    char* dest    , int* s5, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* d = (char *)malloc (sizeof (char)*(*s5+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    char* s = (char *)malloc (sizeof (char)*(*s4+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    memcpy (s, src, *s4);
    s[*s4]='\0';
    memcpy (d, dest, *s5);
    d[*s5]='\0';
    *err = addLinkType (a, n, c, s, d);
}


void addLinkTypeNB_i (char* alias   , int* s1, 
                      char* name    , int* s3,
                      char* contType, int* s2,
                      char* src     , int* s4,
                      char* dest    , int* s5, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* d = (char *)malloc (sizeof (char)*(*s5+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    char* s = (char *)malloc (sizeof (char)*(*s4+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    memcpy (s, src, *s4);
    s[*s4]='\0';
    memcpy (d, dest, *s5);
    d[*s5]='\0';
    *err = addLinkTypeNB (a, n, c, s, d);
}

void addVarType_i (char* alias   , int* s1, 
                   char* name    , int* s3,
                   char* contType, int* s2, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    *err = addVarType (a, c, n);
}

void addVarTypeNB_i (char* alias   , int* s1, 
                     char* name    , int* s3,
                     char* contType, int* s2, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* n = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s2+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, contType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    *err = addVarTypeNB (a, c, n);
}


void addEntityValue_i (char* alias  , int* s1, 
                       char* entType, int* s2,
                       char* name   , int* s3,
                       char* color  , int* s4, int* err){

    char* a  = (char *)malloc (sizeof (char)*(*s1+1));
    char* co = (char *)malloc (sizeof (char)*(*s4+1));
    char* c  = (char *)malloc (sizeof (char)*(*s2+1));
    char* n  = (char *)malloc (sizeof (char)*(*s3+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, entType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    memcpy (co, color, *s4);
    co[*s4]='\0';
    *err = addEntityValue (a, c, n, co);
}

void addEntityValueNB_i (char* alias  , int* s1, 
                         char* entType, int* s2,
                         char* name   , int* s3,
                         char* color  , int* s4, int* err){

    char* a  = (char *)malloc (sizeof (char)*(*s1+1));
    char* co = (char *)malloc (sizeof (char)*(*s4+1));
    char* c  = (char *)malloc (sizeof (char)*(*s2+1));
    char* n  = (char *)malloc (sizeof (char)*(*s3+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (c, entType, *s2);
    c[*s2]='\0';
    memcpy (n, name, *s3);
    n[*s3]='\0';
    memcpy (co, color, *s4);
    co[*s4]='\0';
    *err = addEntityValueNB (a, c, n, co);
}

void addContainer_i (varPrec* time     ,
                     char  * alias    , int* s1,
                     char  * type     , int* s2,
                     char  * container, int* s3,
                     char  * name     , int* s4,
                     char  * filename , int* s5, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s3+1));
    char* n = (char *)malloc (sizeof (char)*(*s4+1));
    char* f = (char *)malloc (sizeof (char)*(*s5+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, container, *s3);
    c[*s3]='\0';
    memcpy (n, name, *s4);
    n[*s4]='\0';
    memcpy (f, filename, *s5);
    f[*s5]='\0';
    *err = addContainer (*time, a, t, c, n, f);
}

void addContainerNB_i (varPrec* time, 
                       char  * alias    , int* s1,
                       char  * type     , int* s2,
                       char  * container, int* s3,
                       char  * name     , int* s4,
                       char  * filename , int* s5, int* err){
    char* a = (char *)malloc (sizeof (char)*(*s1+1));
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s3+1));
    char* n = (char *)malloc (sizeof (char)*(*s4+1));
    char* f = (char *)malloc (sizeof (char)*(*s5+1));
    memcpy (a, alias, *s1);
    a[*s1]='\0';
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, container, *s3);
    c[*s3]='\0';
    memcpy (n, name, *s4);
    n[*s4]='\0';
    memcpy (f, filename, *s5);
    f[*s5]='\0';
    *err = addContainerNB (*time, a, t, c, n, f);
}

void destroyContainer_i (varPrec* time,
                         char * name , int* s1,
                         char * type , int* s2, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* n = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (n, name, *s1);
    n[*s1]='\0';
    *err = destroyContainer (*time, n, t);
}

void destroyContainerNB_i (varPrec* time,
                           char * name , int* s1,
                           char * type , int* s2, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* n = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (n, name, *s1);
    n[*s1]='\0';
    *err = destroyContainerNB (*time, n, t);
}

void setState_i (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 char  * val , int* s3, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* v = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s3);
    v[*s3]='\0';
    *err = setState (*time, t, c, v);
}

void setStateNB_i (varPrec* time,
                   char  * type, int* s2,
                   char  * cont, int* s1,
                   char  * val , int* s3, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* v = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s3);
    v[*s3]='\0';
    *err = setStateNB (*time, t, c, v);
}

void pushState_i (varPrec* time,
                  char  * type, int* s2,
                  char  * cont, int* s1,
                  char  * val , int* s3, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    char* v = (char *)malloc (sizeof (char)*(*s3+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s3);
    v[*s3]='\0';
    *err = pushState (*time, t, c, v);
}

void pushStateNB_i (varPrec* time,
                    char  * type, int* s2,
                    char  * cont, int* s1,
                    char  * val , int* s3, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* v = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s3);
    v[*s3]='\0';
    *err = pushStateNB (*time, t, c, v);
}

void popState_i (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 char  * val , int* s3, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* v = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s3);
    v[*s3]='\0';
    *err = popState (*time, t, c, v);
}

void popStateNB_i (varPrec* time,
                   char  * type, int* s2,
                   char  * cont, int* s1,
                   char  * val , int* s3, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* v = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s3);
    v[*s3]='\0';
    *err = popStateNB (*time, t, c, v);
}

void addEvent_i (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 char  * val , int* s3, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* v = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s3);
    v[*s3]='\0';
    *err = addEvent (*time, t, c, v);
}

void addEventNB_i (varPrec* time,
                   char  * type, int* s2,
                   char  * cont, int* s1,
                   char  * val , int* s3, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* v = (char *)malloc (sizeof (char)*(*s3+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s3);
    v[*s3]='\0';
    *err = addEventNB (*time, t, c, v);
}

void startLink_i (varPrec* time,
                  char  * type, int* s2,
                  char  * cont, int* s1,
                  char  * src , int* s3,
                  char  * dest, int* s4,
                  char  * val , int* s5,
                  char  * key , int* s6, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* k = (char *)malloc (sizeof (char)*(*s6+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    char* v = (char *)malloc (sizeof (char)*(*s5+1));
    char* s = (char *)malloc (sizeof (char)*(*s3+1));
    char* d = (char *)malloc (sizeof (char)*(*s4+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s5);
    v[*s5]='\0';
    memcpy (s, src, *s3);
    s[*s3]='\0';
    memcpy (d, dest, *s4);
    d[*s4]='\0';
    memcpy (k, key, *s6);
    k[*s6]='\0';
    *err = startLink (*time, t, c, s, d, v, k);
}

void startLinkNB_i (varPrec* time,
                    char  * type, int* s2,
                    char  * cont, int* s1,
                    char  * src , int* s3,
                    char  * dest, int* s4,
                    char  * val , int* s5,
                    char  * key , int* s6, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* k = (char *)malloc (sizeof (char)*(*s6+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    char* v = (char *)malloc (sizeof (char)*(*s5+1));
    char* s = (char *)malloc (sizeof (char)*(*s3+1));
    char* d = (char *)malloc (sizeof (char)*(*s4+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s5);
    v[*s5]='\0';
    memcpy (s, src, *s3);
    s[*s3]='\0';
    memcpy (d, dest, *s4);
    d[*s4]='\0';
    memcpy (k, key, *s6);
    k[*s6]='\0';
    *err = startLinkNB (*time, t, c, s, d, v, k);
}

void endLink_i (varPrec* time,
                char  * type, int* s2,
                char  * cont, int* s1,
                char  * src , int* s3,
                char  * dest, int* s4,
                char  * val , int* s5,
                char  * key , int* s6, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* k = (char *)malloc (sizeof (char)*(*s6+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    char* v = (char *)malloc (sizeof (char)*(*s5+1));
    char* s = (char *)malloc (sizeof (char)*(*s3+1));
    char* d = (char *)malloc (sizeof (char)*(*s4+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s5);
    v[*s5]='\0';
    memcpy (s, src, *s3);
    s[*s3]='\0';
    memcpy (d, dest, *s4);
    d[*s4]='\0';
    memcpy (k, key, *s6);
    k[*s6]='\0';
    *err = endLink (*time, t, c, s, d, v, k);
}

void endLinkNB_i (varPrec* time,
                  char  * type, int* s2,
                  char  * cont, int* s1,
                  char  * src , int* s3,
                  char  * dest, int* s4,
                  char  * val , int* s5,
                  char  * key , int* s6, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* k = (char *)malloc (sizeof (char)*(*s6+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    char* v = (char *)malloc (sizeof (char)*(*s5+1));
    char* s = (char *)malloc (sizeof (char)*(*s3+1));
    char* d = (char *)malloc (sizeof (char)*(*s4+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    memcpy (v, val, *s5);
    v[*s5]='\0';
    memcpy (s, src, *s3);
    s[*s3]='\0';
    memcpy (d, dest, *s4);
    d[*s4]='\0';
    memcpy (k, key, *s6);
    k[*s6]='\0';
    *err = endLinkNB (*time, t, c, s, d, v, k);
}



void setVar_i (varPrec* time,
               char  * type, int* s2,
               char  * cont, int* s1,
               varPrec* val, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    *err = setVar (*time, t, c, *val);
}

void setVarNB_i (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 varPrec* val, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    *err = setVarNB (*time, t, c, *val);
}



void addVar_i (varPrec* time,
               char  * type, int* s2,
               char  * cont, int* s1,
               varPrec* val, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    *err = addVar (*time, t, c, *val);
}

void addVarNB_i (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 varPrec* val, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    *err = addVarNB (*time, t, c, *val);
}



void subVar_i (varPrec* time,
               char  * type, int* s2,
               char  * cont, int* s1,
               varPrec* val, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    *err = subVar (*time, t, c, *val);
}

void subVarNB_i (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 varPrec* val, int* err){
    char* t = (char *)malloc (sizeof (char)*(*s2+1));
    char* c = (char *)malloc (sizeof (char)*(*s1+1));
    memcpy (t, type, *s2);
    t[*s2]='\0';
    memcpy (c, cont, *s1);
    c[*s1]='\0';
    *err = subVarNB (*time, t, c, *val);
}


void endTrace_i (int* err){
    *err = endTrace ();
}





void settracetype_i_ (int* type, int* err) __attribute__ ((weak, alias ("setTraceType_i")));


int gettracetype_i_ ()__attribute__ ((weak, alias ("getTraceType_i")));

void inittrace_i_ (char* f, int* len, int* err)__attribute__ ((weak, alias ("initTrace_i")));

void setcomm_i_ (MPI_Comm* c)__attribute__ ((weak, alias ("setComm_i")));

void addproctype_i_ (char* alias   , int* s1, 
                    char* contType, int* s2,
                    char* name    , int* s3, int* err)__attribute__ ((weak, alias ("addProcType_i")));

void addproctypenb_i_ (char* alias   , int* s1, 
                      char* contType, int* s2,
                      char* name    , int* s3, int* err)__attribute__ ((weak, alias ("addProcTypeNB_i")));

void addstatetype_i_ (char* alias   , int* s1, 
                     char* contType, int* s2,
                     char* name    , int* s3, int* err)__attribute__ ((weak, alias ("addStateType_i")));

void addstatetypenb_i_ (char* alias   , int* s1, 
                       char* contType, int* s2,
                       char* name    , int* s3, int* err)__attribute__ ((weak, alias ("addStateTypeNB_i")));

void addeventtype_i_ (char* alias   , int* s1, 
                     char* contType, int* s2,
                     char* name    , int* s3, int* err)__attribute__ ((weak, alias ("addEventType_i")));

void addeventtypenb_i_ (char* alias   , int* s1, 
                       char* contType, int* s2,
                       char* name    , int* s3, int* err)__attribute__ ((weak, alias ("addEventTypeNB_i")));

void addlinktype_i_ (char* alias   , int* s1, 
                    char* name    , int* s3,
                    char* contType, int* s2,
                    char* src     , int* s4,
                    char* dest    , int* s5, int* err)__attribute__ ((weak, alias ("addLinkType_i")));

void addlinktypenb_i_ (char* alias   , int* s1, 
                      char* name    , int* s3,
                      char* contType, int* s2,
                      char* src     , int* s4,
                      char* dest    , int* s5, int* err)__attribute__ ((weak, alias ("addLinkTypeNB_i")));

void addvartype_i_ (char* alias   , int* s1, 
                   char* name    , int* s3,
                   char* contType, int* s2, int* err)__attribute__ ((weak, alias ("addVarType_i")));

void addvartypenb_i_ (char* alias   , int* s1, 
                     char* name    , int* s3,
                     char* contType, int* s2, int* err)__attribute__ ((weak, alias ("addVarTypeNB_i")));

void addentityvalue_i_ (char* alias  , int* s1, 
                       char* entType, int* s2,
                       char* name   , int* s3,
                       char* color  , int* s4, int* err)__attribute__ ((weak, alias ("addEntityValue_i")));

void addentityvaluenb_i_ (char* alias  , int* s1, 
                         char* entType, int* s2,
                         char* name   , int* s3,
                         char* color  , int* s4, int* err)__attribute__ ((weak, alias ("addEntityValueNB_i")));


void addcontainer_i_ (varPrec* time     ,
                     char  * alias    , int* s1,
                     char  * type     , int* s2,
                     char  * container, int* s3,
                     char  * name     , int* s4,
                     char  * filename , int* s5, int* err)__attribute__ ((weak, alias ("addContainer_i")));

void addcontainernb_i_ (varPrec* time, 
                       char  * alias    , int* s1,
                       char  * type     , int* s2,
                       char  * container, int* s3,
                       char  * name     , int* s4,
                       char  * filename , int* s5, int* err)__attribute__ ((weak, alias ("addContainerNB_i")));

void destroycontainer_i_ (varPrec* time,
                         char * name , int* s1,
                         char * type , int* s2, int* err)__attribute__ ((weak, alias ("destroyContainer_i")));

void destroycontainernb_i_ (varPrec* time,
                           char * name , int* s1,
                           char * type , int* s2, int* err)__attribute__ ((weak, alias ("destroyContainerNB_i")));

void setstate_i_ (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 char  * val , int* s3, int* err)__attribute__ ((weak, alias ("setState_i")));

void setstatenb_i_ (varPrec* time,
                   char  * type, int* s2,
                   char  * cont, int* s1,
                   char  * val , int* s3, int* err)__attribute__ ((weak, alias ("setStateNB_i")));

void pushstate_i_ (varPrec* time,
                  char  * type, int* s2,
                  char  * cont, int* s1,
                  char  * val , int* s3, int* err)__attribute__ ((weak, alias ("pushState_i")));

void pushstatenb_i_ (varPrec* time,
                    char  * type, int* s2,
                    char  * cont, int* s1,
                    char  * val , int* s3, int* err)__attribute__ ((weak, alias ("pushStateNB_i")));

void popstate_i_ (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 char  * val , int* s3, int* err)__attribute__ ((weak, alias ("popState_i")));

void popstatenb_i_ (varPrec* time,
                   char  * type, int* s2,
                   char  * cont, int* s1,
                   char  * val , int* s3, int* err)__attribute__ ((weak, alias ("popStateNB_i")));

void addevent_i_ (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 char  * val , int* s3, int* err)__attribute__ ((weak, alias ("addEvent_i")));

void addeventnb_i_ (varPrec* time,
                   char  * type, int* s2,
                   char  * cont, int* s1,
                   char  * val , int* s3, int* err)__attribute__ ((weak, alias ("addEventNB_i")));

void startlink_i_ (varPrec* time,
                  char  * type, int* s2,
                  char  * cont, int* s1,
                  char  * src , int* s3,
                  char  * dest, int* s4,
                  char  * val , int* s5,
                  char  * key , int* s6, int* err)__attribute__ ((weak, alias ("startLink_i")));

void startlinknb_i_ (varPrec* time,
                    char  * type, int* s2,
                    char  * cont, int* s1,
                    char  * src , int* s3,
                    char  * dest, int* s4,
                    char  * val , int* s5,
                    char  * key , int* s6, int* err)__attribute__ ((weak, alias ("startLinkNB_i")));

void endlink_i_ (varPrec* time,
                char  * type, int* s2,
                char  * cont, int* s1,
                char  * src , int* s3,
                char  * dest, int* s4,
                char  * val , int* s5,
                char  * key , int* s6, int* err)__attribute__ ((weak, alias ("endLink_i")));

void endlinknb_i_ (varPrec* time,
                  char  * type, int* s2,
                  char  * cont, int* s1,
                  char  * src , int* s3,
                  char  * dest, int* s4,
                  char  * val , int* s5,
                  char  * key , int* s6, int* err)__attribute__ ((weak, alias ("endLinkNB_i")));

void setvar_i_ (varPrec* time,
               char  * type, int* s2,
               char  * cont, int* s1,
               varPrec* val, int* err)__attribute__ ((weak, alias ("setVar_i")));

void setvarnb_i_ (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 varPrec* val, int* err)__attribute__ ((weak, alias ("setVarNB_i")));

void addvar_i_ (varPrec* time,
               char  * type, int* s2,
               char  * cont, int* s1,
               varPrec* val, int* err)__attribute__ ((weak, alias ("addVar_i")));

void addvarnb_i_ (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 varPrec* val, int* err)__attribute__ ((weak, alias ("addVarNB_i")));

void subvar_i_ (varPrec* time,
               char  * type, int* s2,
               char  * cont, int* s1,
               varPrec* val, int* err)__attribute__ ((weak, alias ("subVar_i")));

void subvarnb_i_ (varPrec* time,
                 char  * type, int* s2,
                 char  * cont, int* s1,
                 varPrec* val, int* err)__attribute__ ((weak, alias ("subVarNB_i")));

void endtrace_i_ (int* err)__attribute__ ((weak, alias ("endTrace_i")));



