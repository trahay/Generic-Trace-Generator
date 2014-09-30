#ifdef BUILD_PAJE

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef USE_MPI
#include <mpi.h>
#endif

#ifndef HAVE_ASPRINTF
extern int asprintf(char **ret, const char *format, ...);
/* extern int vasprintf(char **ret, const char *format, va_list args); */
#endif /* HAVE_ASPRINTF */

#include "GTG.h"
#include "GTGPaje_Basic.h"
#include "GTGPaje.h"

#if HAVE_LIBZ
#include "GTGCompress.h"
#endif


extern int _is_paje_header_written;
void pajeInitHeaderData( int fmt );
void pajeWriteHeader( FILE *file );

/* set in GTGBasic1_c.c
 * verbose !=0 means debugging mode
 */
extern int verbose;

struct container_file{
  char container_name[1024];
  char file_name[1024];
  FILE* file;
  int closed;
};

/* File where each proc writes state/event/var/link changes. */
struct container_file procFile[100]; /* todo: don't harcode the size */
static int nb_containers = 0;

/* File where each proc writes the header and the definitions of the data. */
static FILE*  headFile;
/* File where each proc writes the footer and the % Include directives. */
static FILE*  footFile;
/* Root name */
static char*  filename;
/* mpi rank */
static int rank;
/* Name built for getName */
static char* nameTmp;
/* Flags that should be used. */
static gtg_flag_t paje_flags;
/* Max length of the names built */
#define BUFFSIZE 200


/* in non-buffered mode, flush a file */
#define FLUSH(file)                             \
    if(paje_flags & GTG_FLAG_NOTBUF)            \
        fflush(file)

#define PRINT_PAJE_HEADER()                     \
    if ( !_is_paje_header_written ) {           \
        pajeWriteHeader( headFile );            \
        FLUSH( headFile );                      \
    }

/* Structure containing the info of a current line */
typedef struct line_t{
    FILE*   file;   /* File                               */
    char*   value;  /* The value of the line              */
    size_t  size;   /* Size of the value buffer           */
    varPrec time;   /* Time (second element of the line)  */
    int     status; /* If EOF reached                     */
}line_t;

typedef struct doubleLinkList_t{
    struct doubleLinkList_t* prev; /* Previous element */
    struct doubleLinkList_t* next; /* Next element     */
    line_t current;                /* Current line     */
}doubleLinkList_t;

#define GET_FILE_DESCRIPTOR(f, cont)					\
  do {									\
    struct container_file* p_cont = __paje_compress_find_container(cont); \
    if(p_cont) {							\
      f = p_cont->file;							\
    }									\
  } while(0)

static struct container_file* __paje_compress_create_container_file(const char* cont) {

  struct container_file* res = &procFile[nb_containers];
  nb_containers++;

  res->closed = 0;
  strcpy(res->container_name, cont?cont:"");

  sprintf (res->file_name, "%s_%s.ept", filename, res->container_name);
  res->file = fopen (res->file_name, "w");
  if(!res->file)
    abort();

  return res;
}


static void __paje_compress_destroy_container(struct container_file* p_cont) {
  if(p_cont->closed > 0)
    /* this container is already destroyed */
    return;

  FLUSH(p_cont->file);
  fclose (p_cont->file);
  p_cont->closed = 1;
}


static struct container_file* __paje_compress_find_container(const char* cont) {
  int i;
  cont = cont? cont : "";
  for(i=0; i<nb_containers; i++) {
    if(!strcmp(cont, procFile[i].container_name))
      return &procFile[i];
  }

  return NULL;
}


#if HAVE_LIBZ
static z_stream z;
static int compression_ratio = 0;

/* compress data and write to disk */
void paje_print(FILE* file, const char*format, ...)
{
    int len = 0;
    int ret = 0;
    va_list args;
    char* tmp_buf = NULL;
    va_start(args, format);

    if(compression_ratio) {
        len = vasprintf(&tmp_buf, format, args);
        va_end(args);

        ret = gtg_compress_m2f(&z, tmp_buf, len, file);
        if (ret < 0) {
            fprintf(stderr, "gtg: an error occured while compressing\n");
        }
    } else {
        /* don't compress, write directly to disk */
        vfprintf(file, format, args);
        va_end(args);
    }
}

trace_return_t pajeSetCompress(int val)
{
    compression_ratio = val;
    if(val)
        gtg_compress_init(&z, compression_ratio);
    return TRACE_SUCCESS;
}

#else  /* HAVE_LIBZ */

#define paje_print fprintf

trace_return_t pajeSetCompress(int val)
{
    return TRACE_ERR_WRITE;
}

#endif

/* Insert a doubleLinkList_t item in the sorted doubleLinkedList starting with first */
void insert (doubleLinkList_t** first, doubleLinkList_t* item){
    varPrec time;
    doubleLinkList_t* iter = *first;
    if (!first || !item){
        fprintf (stderr, "Invalid insert parameter. Leaving\n");
        exit(-1);
    }
    time = item->current.time;
    /* If nil list, item is the first and leaving */
    if (iter==NULL){
        *first=item;
        item->prev = NULL;
        item->next = NULL;
        /*  printf (stderr, "Nil list\n"); */
        return;
    }
    /* If to insert in first pos */
    if (time<=iter->current.time){
        item->prev = NULL;
        item->next = *first;
        if (item->next)
            (item->next)->prev = item;
        *first = item;
        return;
    }
    /* Positionning iterator to insert in a sorted list */
    while (time>iter->current.time && iter->next)
        iter = iter->next;

    if (time<=iter->current.time){
        item->prev = iter->prev;
        item->next = iter;
        iter->prev = item;
        if (item->prev)
            (item->prev)->next = item;
    }
    else{
        iter->next = item;
        item->prev = iter;
        item->next=NULL;
    }
}

void initMerge (char* file, int rk, doubleLinkList_t* li){
    char f[BUFFSIZE];

    sprintf (f, "%s_proc%d.ept", file, rk);
    li->current.file = fopen (f, "a+");
    if (!li->current.file){
        fprintf (stderr, "Failed to open generated trace. Leaving\n");
        exit (-1);
    }
    rewind (li->current.file);
    li->current.value = NULL;
    li->current.status = 1;
}


#ifndef HAVE_GETLINE
ssize_t getline(char** buf, size_t* size, FILE* stream)
{
    char *ret;
    int alloc_size = 1024;
    int nb_char = 0;

    if(!(*buf)) {
        /* we need to allocate the buffer */
        alloc_size = 1024;
        *buf = malloc(sizeof(char) * alloc_size);
    } else {
        alloc_size = *size;
    }
    *buf[0]=0;

    do {
        if(nb_char + 512 >= alloc_size) {
            /* the next fgets may overflow the buffer, so let's
             allocate more space */
            alloc_size *= 2;
            *buf = realloc(*buf, alloc_size);
            *size = alloc_size;
        }

        /* read at most 512 bytes */
        ret = fgets((*buf)+nb_char, 512, stream);

        if(!ret){
            /* error while reading or EOF */
            *size = 1;
            (*buf)[0] = EOF;
            return -1;
        }
        nb_char = strlen(*buf);
    } while((*buf)[nb_char -1] !='\n');

    return nb_char;
}
#endif	/* HAVE_GETLINE */

void myGetLine (doubleLinkList_t* li){
    int toto;
    int ret;
    if (li->current.value)
        free (li->current.value);
    li->current.value = NULL;
    ret = getline (&(li->current.value), &(li->current.size), li->current.file);

    if (li->current.size==0 || ret ==-1){
        li->current.status=0;
    }

    if ((li->current.size)>0)
        sscanf (li->current.value, "%d %lf", &toto, &(li->current.time));
    else
        fprintf (stderr, "failed to read\n");
}

/* need to run 'rm -f' force because there is no file some time*/
void clean_files (char* fileName, int nbFile){
    int ret;
    char cmd[BUFFSIZE];
    sprintf (cmd, "rm -f %s_root.ept %s_foot.ept %s_proc*.ept", filename, filename, filename);
    ret = system(cmd);
    if (ret == -1) {
        perror("cleanfiles");
    }
}

/* copy the content of filename into dest */
void append(const char* filename, FILE*dest) {
  char *buf=NULL;
  size_t size = BUFFSIZE;
  FILE *oldF = fopen (filename, "r");
  if(oldF == NULL)
    abort();

  if (dest==NULL || oldF==NULL){
    fprintf (stderr, "Failed to create the trace file. Leaving\n");
    exit (-1);
  }

  buf = malloc(size);
  do{
    int i = getline (&buf, &size, oldF);
    if (size==0 || i==-1)
      break;
    paje_print (dest, "%s", buf);
  }while (buf[0] != EOF);
  free(buf);

  fclose (oldF);
}

void merge (char* filename, int nbFile){
    doubleLinkList_t* first = NULL;
    doubleLinkList_t* list;
    doubleLinkList_t* iter;
    FILE            * res;
    char              tmp[BUFFSIZE];
    char              tmp2[BUFFSIZE];
    char              tmp3[BUFFSIZE];
    int               i;

    /* Getting the first part of the trace (header+def) */
#if HAVE_LIBZ
    if(compression_ratio)
        sprintf (tmp, "%s.trace.z", filename);
    else
#endif
      sprintf (tmp, "%s.trace", filename);

    sprintf (tmp2, "%s_root.ept", filename);
    sprintf (tmp3, "%s_foot.ept", filename);

    if(nbFile == 1) {
      int retval= rename(tmp2, tmp);
      if(retval)
	abort();

      res = fopen (tmp, "a");
      goto out;
      return;
    }

    res = fopen (tmp, "w+");
    append(tmp2, res);

    /* Initialising the parallel merge */
    list = (doubleLinkList_t *)malloc (sizeof (doubleLinkList_t)*nbFile);
    for (i=0;i<nbFile;i++)
        initMerge (filename, i, list+i);

    for (i=0;i<nbFile;i++){
        myGetLine (list+i);
        insert (&first, list+i);
    }

    /*  fprintf (stderr, "Nb file = %d \n La liste :\n", nbFile); */
    /*  for (i=0;i<nbFile;i++)                                     */
    /*      fprintf (stderr, "L1 = %s\n", list[i].current.value); */

    while (first && first->current.status != 0){
        paje_print (res, "%s", first->current.value);
        myGetLine (first);
        if (first->current.status==0){
            first=first->next;
            if (first)
                first->prev = NULL;
        }
        else{
            iter = first;
            first = first->next;
            if (first)
                first->prev = NULL;
            if (first && iter->current.status != 0)
                insert (&first, iter);
        }
        /* for (i=0;i<nbFile;i++) */
        /*     fprintf (stderr, "L1 = %s, st=%d\n", list[i].current.value,list[i].current.status); */
        /* fprintf (stderr, "<---------------------------->\n\n"); */
    }

 out:
    /* copy the footer */
    append(tmp3, res);

    fclose (res);
    clean_files (filename, nbFile);
}

paje_color_t Paje_get_color(gtg_color_t p_color) {
    /** \todo it is not const*/
    int ret;
    paje_color_t res = NULL;
    ret = asprintf(&res, "%1.1f %1.1f %1.1f",
                   (float)GTG_COLOR_GET_RED(  p_color->rgb)/256,
                   (float)GTG_COLOR_GET_GREEN(p_color->rgb)/256,
                   (float)GTG_COLOR_GET_BLUE( p_color->rgb)/256);
    (void)ret;
    return res;
}

char*
pajeGetName (int procNb){
    if (nameTmp)
        free (nameTmp);
    nameTmp = (char *)malloc (sizeof (char)*BUFFSIZE);
    if (filename){
        sprintf (nameTmp, "%s_proc%d.ept", filename, procNb);
        if(verbose)
            fprintf (stderr, "Name built : %s \n", nameTmp);
        return nameTmp;
    }
    return "0";
}

trace_return_t pajeInitTrace   (const char* filenam, int rk, gtg_flag_t flags, int fmt) {
    char file[BUFFSIZE];
    trace_return_t ret = TRACE_ERR_OPEN;

    rank = rk;	/* fixme: why there is need to copy filenam ????*/
    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);

    paje_flags = flags;
    if( rank==0 ||
        ! (flags & GTG_FLAG_USE_MPI)) {

        sprintf (file, "%s_root.ept", filename);
        headFile = fopen (file, "w");
        if (!headFile){
            fprintf (stderr, "Failed to open file %s. \n Leaving\n", file);
            return ret;
        }

        sprintf (file, "%s_foot.ept", filename);
        footFile = fopen (file, "w");
        if (!footFile){
            fprintf (stderr, "Failed to open file %s. \n Leaving\n", file);
            return ret;
        }

        pajeInitHeaderData( fmt );
        FLUSH(headFile);
        FLUSH(footFile);
    }
    return TRACE_SUCCESS;
}

trace_return_t pajeAddContType(const char* alias, const char* contType,
                               const char* name) {
    PRINT_PAJE_HEADER();

    if(verbose)
        printf("addCtType : name %s, alias %s, contType %s\n", name, alias, contType);
    if (headFile){
        fprintf (headFile, "%d \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_DefineContainerType].id, name, contType ? contType : "0", alias);

        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddStateType   (const char* alias, const char* contType,
                                   const char* name){
    PRINT_PAJE_HEADER();

    if(verbose)
        printf("addStateType : alias %s, name %s, contType %s\n",  alias, name, contType);
    if (headFile){
        fprintf (headFile, "%d \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_DefineStateType].id, name, contType, alias);

        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddEventType   (const char* alias, const char* contType,
                                   const char* name){
    PRINT_PAJE_HEADER();

    if(verbose)
        printf("addEventType :  alias %s, name %s, contType %s\n", alias, name, contType);
    if (headFile){
        fprintf (headFile, "%d \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_DefineEventType].id, name, contType, alias);

        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddVarType   (const char* alias   , const char* contType,
                                 const char* name){
    PRINT_PAJE_HEADER();

    if(verbose)
        printf("addVarType : alias %s, name %s, contType %s\n", alias, name, contType);
    if (headFile){
        fprintf (headFile, "%d \"%s\" \"%s\" \"0 0 0\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_DefineVariableType].id, name, contType, alias);

        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddLinkType   (const char* alias   , const char* name,
                                  const char* contType, const char* srcContType,
                                  const char* destContType){
    PRINT_PAJE_HEADER();

    if (verbose)
        printf ("Add link type: alias: %s name: %s contType: %s src: %s des: %s\n", alias, name, contType, srcContType, destContType);
    if (headFile){
        fprintf (headFile, "%d \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_DefineLinkType].id, name, contType, srcContType, destContType, alias);

        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddEntityValue   (const char* alias, const char* entType,
                                     const char* name , const char* color){
    PRINT_PAJE_HEADER();

    if(verbose)
        printf("addEntityValue : color %s, alias %s, name %s, type %s\n", color, alias, name, entType);
    if (headFile){
        fprintf (headFile, "%d \"%s\" \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_DefineEntityValue].id, entType, name, color, alias);

        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddContainer(varPrec time, const char* alias,
                                const char*  type, const char* container,
                                const char*  name, const char* file){
    PRINT_PAJE_HEADER();

    if (headFile){
      struct container_file* p_cont = __paje_compress_create_container_file(alias);

      if(container && strcmp(container, "(null)")!=0 ) {
	fprintf (headFile, "%d %.13e \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
		 paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type, container, alias, file);
	fprintf (footFile, "%% Include \"%s\"\n", p_cont->file_name);
	FLUSH(headFile);
	FLUSH(footFile);
      } else {
	fprintf (headFile, "%d %.13e \"%s\" \"%s\" 0 \"%s\" \"%s\"\n",
		 paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type, alias, file);
      }
      FLUSH(headFile);

      return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeSeqAddContainer   (varPrec time, const char* alias    ,
                                      const char*  type, const char* container,
                                      const char*  name){
    PRINT_PAJE_HEADER();

    if (headFile){
      struct container_file* p_cont = __paje_compress_create_container_file(alias);

      if(container && strcmp(container, "(null)")!=0 ) {
            fprintf (headFile, "%d %.13e \"%s\" \"%s\" \"%s\" \"%s\"\n",
                     paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type, container, alias);
	    fprintf (footFile, "%% Include \"%s\"\n", p_cont->file_name);
	    FLUSH(headFile);
	    FLUSH(footFile);
      } else {
	fprintf (headFile, "%d %.13e \"%s\" \"%s\" 0 \"%s\"\n",
		 paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type, alias);
      }

      FLUSH(headFile);
      return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeDestroyContainer     (varPrec time, const char*  name,
                                         const char*  type){
    PRINT_PAJE_HEADER();

    if (headFile){
    struct container_file* p_cont = __paje_compress_find_container(name);

    fprintf (p_cont->file, "%d %.13e \"%s\" \"%s\"\n",
	     paje_eventdefs[GTG_PAJE_EVTDEF_DestroyContainer].id, time, name, type);

    FLUSH(p_cont->file);
    __paje_compress_destroy_container(p_cont);

        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeSetState   (varPrec time, const char* type,
                               const char*  cont, const char* val){
    FILE *f=NULL;
    if(verbose)
        printf("SetState : type %s, container %s, val %s\n", type, cont, val);

    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_SetState].id, time, type, cont, val);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajePushState   (varPrec time, const char* type,
                                const char*  cont, const char* val){
    FILE *f=NULL;
    if(verbose)
        printf("PushState : type %s, container %s, val %s\n", type, cont, val);

    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_PushState].id, time, type, cont, val);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajePopState   (varPrec time, const char* type,
                               const char*  cont){
    FILE *f=NULL;
    if(verbose)
        printf("PopState : type %s, container %s\n", type, cont);

    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_PopState].id, time, type, cont);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddEvent   (varPrec time, const char* type,
                               const char*  cont, const char* val){
    FILE *f=NULL;
    if(verbose)
        printf("AddEvent : type %s, cont %s, val %s\n", type, cont, val);


    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_NewEvent].id, time, type, cont, val);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeStartLink   (varPrec time, const char* type,
                                const char*   cont, const char* src,
                                const char*   val , const char* key){
    FILE *f=NULL;
    if (verbose)
        printf ("Start link: type: %s container: %s src: %s val: %s key %s\n",
                type, cont, src, val, key);

    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_StartLink].id, time, type, cont, src, val, key);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeEndLink   (varPrec time, const char* type,
                              const char*   cont, const char* dest,
                              const char*   val , const char* key){
    FILE *f=NULL;
    if (verbose)
        printf ("End link type: %s container: %s src: %s val: %s key %s\n",
                type, cont, dest, val, key);

    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_EndLink].id, time, type, cont, dest, val, key);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeSetVar   (varPrec time, const char*  type,
                             const char*  cont, varPrec val){
    FILE *f=NULL;
    if (verbose)
        printf ("SetVar: type: %s container: %s value: %e\n",
                type, cont, val);

    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\" %e\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_SetVariable].id, time, type, cont, val);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddVar   (varPrec time, const char*  type,
                             const char*  cont, varPrec val){
    FILE *f=NULL;
    if (verbose)
        printf ("AddVar: type: %s container: %s value: %e\n",
                type, cont, val);

    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\" %e\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_AddVariable].id, time, type, cont, val);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeSubVar   (varPrec time, const char*  type,
                             const char*  cont, varPrec val){
    FILE *f=NULL;
    if (verbose)
        printf ("SubVar: type: %s container: %s value: %e\n",
                type, cont, val);

    GET_FILE_DESCRIPTOR(f, cont);

    if (f){
        fprintf (f, "%d %.13e \"%s\" \"%s\" %e\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_SubVariable].id, time, type, cont, val);

        FLUSH(f);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddComment   (const char*  comment){
    if (verbose)
        printf ("AddComment: %s\n",comment);

    if (headFile){
        fprintf(headFile,"#%s\r\n",comment);
        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}


trace_return_t pajeEndTrace (){
    int size = 1;
    int i;

    FLUSH(headFile);
    fclose(headFile);
    FLUSH(footFile);
    fclose(footFile);

    /* Wait for all proc to finish writing their trace */
    for(i=0;i<nb_containers; i++) {
      __paje_compress_destroy_container(&procFile[i]);
    }

#ifdef USE_MPI
    if(paje_flags & GTG_FLAG_USE_MPI) {
        MPI_Barrier (MPI_COMM_WORLD);
        MPI_Comm_size (MPI_COMM_WORLD, &size);
        if(verbose)
            fprintf (stderr, "USING MPI\n");
    }
    if (rank==0)
#endif	/* USE_MPI */

      merge (filename, size);

    if (nameTmp)
        free (nameTmp);
    nameTmp = NULL;
    if (filename)
        free (filename);
    filename = NULL;
    return TRACE_SUCCESS;
}

trace_return_t viteEndTrace (){
    if (filename)
        free (filename);
    if (nameTmp)
        free (nameTmp);
    filename = NULL;
    nameTmp = NULL;
    return TRACE_SUCCESS;
}
#endif
