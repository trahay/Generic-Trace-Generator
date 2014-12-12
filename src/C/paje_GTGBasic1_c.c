#ifdef BUILD_PAJE

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>


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
void pajeInitHeaderData( int fmt, gtg_flag_t paje_flags);
void pajeWriteHeader( FILE *file );

/* set in GTGBasic1_c.c
 * verbose !=0 means debugging mode
 */
extern int verbose;
/* Max length of the names built */
#define BUFFSIZE 1024

/* File where each proc writes the header and the definitions of the data. */
static FILE*  headFile=NULL;
/* name of the root file*/
static char  headFilename[BUFFSIZE];

/* base name */
static char*  filename;
/* mpi rank */
static int rank;
/* Name built for getName */
static char* nameTmp;
/* Flags that should be used. */
static gtg_flag_t paje_flags;


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

struct container_file{
  char container_name[1024];
  char file_name[1024];
  FILE* file;
  int closed;
  struct container_file *next;
};

/* File where each proc writes state/event/var/link changes. */
static struct container_file *procFile=NULL; /* TODO: don't hardcode the size */
static int nb_containers = 0;
static int nb_allocated_containers = 0;

/* returns 1 if str is null (or contains "0" or "(null)") */
static int null_string(const char*str) {
  return (! str                 ||
	  strcmp(str, "0") == 0 ||
	  strcmp(str, "")  == 0 ||
	  strcmp(str, "(null)") == 0);
}

#define GET_FILE_DESCRIPTOR(f, cont)					\
  do {									\
    struct container_file* p_cont = __paje_find_container(cont);	\
    if(! p_cont) {							\
      p_cont = __paje_create_container_file(cont);			\
    }									\
    f = p_cont->file;							\
  } while(0)


/* find the container_file structure that corresponds to cont */
static struct container_file* __paje_find_container(const char* cont) {
  int i;
  cont = cont? cont : "";
  for(i=0; i<nb_containers; i++) {
    if(!strcmp(cont, procFile[i].container_name))
      return &procFile[i];
  }
  return NULL;
}

/* Create a new container_file structure that corresponds to cont */
static struct container_file* __paje_create_container_file(const char* cont) {

  struct container_file* res = NULL;
  if(nb_containers >= nb_allocated_containers) {
    nb_allocated_containers *= 2 ;
    nb_allocated_containers=(nb_allocated_containers?nb_allocated_containers:100);
    procFile = realloc(procFile, sizeof(struct container_file)*nb_allocated_containers);
    assert(procFile);
  }
  assert(nb_containers<nb_allocated_containers);

  res = &procFile[nb_containers];
  nb_containers++;
  res->closed = 0;
  strcpy(res->container_name, cont?cont:"");

  if(paje_flags & GTG_FLAG_PAJE_MULTIPLE_FILES) {
    /* each container write to a separate file */
    sprintf (res->file_name, "%s_%s.trace", filename, res->container_name);
    res->file = fopen (res->file_name, "w");
    if(!res->file)
      abort();
  } else {
    /* all the containers write to the same file */
    sprintf (res->file_name, "%s.trace", headFilename);
    res->file = headFile;
  }
  return res;
}


static void __paje_destroy_container(struct container_file* p_cont) {
  if(p_cont->closed > 0)
    /* this container is already destroyed */
    return;

  FLUSH(p_cont->file);
  fclose (p_cont->file);
  p_cont->closed = 1;
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

trace_return_t my_open (int rk){
    trace_return_t ret = TRACE_ERR_OPEN;

    if (!headFile){
      sprintf (headFilename, "%s_proc%d.ept", filename, rk);
      headFile = fopen (headFilename, "w");
    }
    if (!headFile)
      return ret;
    ret = TRACE_SUCCESS;
    return ret;
}

trace_return_t my_close (){
    trace_return_t ret = TRACE_ERR_CLOSE;
    if (headFile){
        ret = (trace_return_t) fclose (headFile);
        headFile = NULL;
    }
    return ret;
}

trace_return_t pajeInitTrace   (const char* filenam, int rk, gtg_flag_t flags, int fmt) {
    trace_return_t ret = TRACE_ERR_OPEN;

    my_close ();
    rank = rk;	/* fixme: why there is need to copy filenam ????*/
    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);
    headFile = NULL;

    paje_flags = flags;
    if( rank==0 ||
        (! (flags & GTG_FLAG_USE_MPI) && !(flags & GTG_FLAG_PAJE_MULTIPLE_FILES))) {
        sprintf (headFilename, "%s_root.ept", filename);
        headFile = fopen (headFilename, "w");
        if (!headFile){
            fprintf (stderr, "Failed to open file %s. \n Leaving\n", headFilename);
            return ret;
        }

        pajeInitHeaderData( fmt, flags);

        FLUSH(headFile);
    } else {
      /* another process writes the header, so we don't have to */
      _is_paje_header_written = 1;
    }
    return my_open(rank);
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

trace_return_t pajeAddContainer_generic(varPrec time, const char* alias,
					const char*  type, const char* container,
					const char*  name, const char* file){
    PRINT_PAJE_HEADER();

    if (headFile){
      fprintf (headFile, "%d %.13e \"%s\" \"%s\"",
	       paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type);

      if(container && strcmp(container, "(null)")!=0 ) {
	fprintf(headFile, " \"%s\"", container);
      }else{
	fprintf(headFile, " 0");
      }

      fprintf(headFile, " \"%s\"", alias);

      if(! null_string(file)) {
	fprintf(headFile, " \"%s\"", file);
      }

      fprintf(headFile,"\n");
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
    if(null_string(file)) {
      struct container_file* p_cont = __paje_create_container_file(alias);
      if(paje_flags & GTG_FLAG_PAJE_MULTIPLE_FILES) {
	return pajeAddContainer_generic(time, alias, type, container, name, p_cont->file_name);
      }
    }
    return pajeAddContainer_generic(time, alias, type, container, name, file);
  }
  return TRACE_ERR_WRITE;
}

trace_return_t pajeSeqAddContainer   (varPrec time, const char* alias    ,
                                      const char*  type, const char* container,
                                      const char*  name){
    PRINT_PAJE_HEADER();

    if (headFile){
      struct container_file* p_cont = __paje_create_container_file(alias);
      if(paje_flags & GTG_FLAG_PAJE_MULTIPLE_FILES) {
	return pajeAddContainer_generic(time, alias, type, container, name, p_cont->file_name);
      } else {
	return pajeAddContainer_generic(time, alias, type, container, name, NULL);
      }
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeDestroyContainer     (varPrec time, const char*  name,
                                         const char*  type){
    PRINT_PAJE_HEADER();

    if (headFile){
      struct container_file* p_cont = __paje_find_container(name);

      fprintf (p_cont->file, "%d %.13e \"%s\" \"%s\"\n",
	       paje_eventdefs[GTG_PAJE_EVTDEF_DestroyContainer].id, time, name, type);

      FLUSH(p_cont->file);
      __paje_destroy_container(p_cont);

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
    printf("f==NULL (cont=%s)\n", cont);
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

int acquire_lock(const char* lockfile) {
  int lock_fd, rc;
  lock_fd = open(lockfile, O_RDWR | O_CREAT, 0666);
  if(lock_fd < 0) {
    printf("open(%s) returned %d (%s)\n", lockfile, errno, strerror(errno));
    abort();
  }

  rc = flock(lock_fd, LOCK_EX);
  if(rc != 0) {
    printf("flock returned %d\n", rc);
    abort();
  }
  return lock_fd;
}

void release_lock(const char* lockfile, int lock_fd){
  int rc = flock(lock_fd, LOCK_UN);
  if(rc !=0){
    printf("unlock returned %d\n", rc);
    abort();
  }
  /* todo: delete the lock file */
}

#define MAX_LINE_SIZE 1024
struct paje_line_t{
  char *line;
  int len;			/* length of the line */
  size_t size;			/* allocated size of line */
  int code;
  double timestamp;
  int eof;
};

static void __init_paje_line(struct paje_line_t*l) {
  l->size=MAX_LINE_SIZE;
  l->line=malloc(sizeof(char)*l->size);
  l->len=0;
  l->code=-1;
  l->timestamp=-1;
  l->eof=0;
}

void read_line(FILE*f, struct paje_line_t *l) {
  int rc;
  l->len = getline(&l->line, &l->size, f);
  if(l->len<0) {
    if(!feof(f)) {
      printf("getline failed!\n");
      abort();
    }
    l->eof=1;
    return;
  }
  /* getline managed to read something */
  rc = sscanf(l->line, "%d %lf", &l->code, &l->timestamp);
  if(rc != 2) {
    /* this is probably because the line starts with % */
    l->code=-1;
    l->timestamp=-1;
  }
}

void write_line(FILE*f, struct paje_line_t*l) {
  fprintf(f, "%s", l->line);
}

/* check if input1 (resp. input2) is empty and if so, rename input2 (resp.
 * input1) into output)
 * return 0 if none of the files were renamed
 */
static int __try_to_rename(const char *input1,
			   const char *input2,
			   const char *output) {
  /* check if input1 exists */
  struct stat st;
  int rc = stat(input1, &st);
  if(rc <0 && errno == ENOENT) {
    /* input1 does not exist. we can simply rename input2 to output */
    rc = rename(input2, output);
    assert(rc == 0);
    return 1;
  }

  /* check if input2 exists */
  rc = stat(input2, &st);
  if(rc <0 && errno == ENOENT) {
    /* input2 does not exist. we can simply rename input1 to output */
    rc = rename(input1, output);
    assert(rc == 0);
    return 1;
  }

  /* both files exist */
  return 0;
}

/* merge input1 and input2 into output */
static void __merge_files(const char *input1,
			  const char *input2,
			  const char *output) {
  FILE* input1_file;
  FILE* input2_file;
  FILE* output_file;
  struct paje_line_t line_input, line_merge;

  if(__try_to_rename(input1, input2, output)) {
    goto out;
  }

  /* both input1 and input2 exist */

  /* open the merge file */
  input1_file = fopen(input1, "r");
  assert(input1_file);

  /* open the input file */
  input2_file = fopen(input2, "r");
  assert(input2_file);

  /* open the output file */
  output_file = fopen(output, "w");
  assert(output_file);

  /* merge input2_file and input1_file */
  __init_paje_line(&line_input);
  __init_paje_line(&line_merge);
  read_line(input2_file, &line_input);
  read_line(input1_file, &line_merge);

  /* merge input and merge files */
  while(!line_input.eof && ! line_merge.eof) {
    if(line_input.timestamp<line_merge.timestamp) {
      write_line(output_file, &line_input);
      read_line(input2_file, &line_input);
    } else {
      write_line(output_file, &line_merge);
      read_line(input1_file, &line_merge);
    }
  }

  /* copy the remaining of the files */
  while(!line_input.eof) {
    write_line(output_file, &line_input);
    read_line(input2_file, &line_input);
  }

  while(!line_merge.eof) {
    write_line(output_file, &line_merge);
    read_line(input1_file, &line_merge);
  }

  /* close files and delete merge and input */
  fclose(output_file);
  fclose(input2_file);
  fclose(input1_file);
  unlink(input1);
  unlink(input2);

 out:
  return;
}

/* merge headFilename to the other headFile (from other processes) into a file named
 * <base_filename>.trace
 */
static void merge_contrib(const char *base_filename) {
  int lock_fd;
  int rc;
  char merged_filename[BUFFSIZE]; /* the other file to merge */
  char output_filename[BUFFSIZE]; /* the resulting file */
  char lockfile[BUFFSIZE];

  /* <filename>.trace will contain the merge of <filename>.ept and <filename>.trace.merged
   */

  /* todo: check if compression still works */
#if HAVE_LIBZ
  if(compression_ratio)
    sprintf (output_filename, "%s.trace.z", base_filename);
  else
#endif
    sprintf (output_filename, "%s.trace", base_filename);
  sprintf (merged_filename, "%s.trace.merged", base_filename);
  sprintf (lockfile, "%s.trace.lock", base_filename);

  /* make sure we have exclusive access to files */
  lock_fd = acquire_lock(lockfile);
  assert(lock_fd>=0);
  {
    /* Currently, <filename>.trace contains the merge of the other *.ept files,
     * let's rename it to <filename>.trace.merge
     */
    rc = rename(output_filename, merged_filename);
    if(rc != 0 && errno != ENOENT) {
      /* rename failed */
      fprintf(stderr, "gtg: failed to rename %s into %s\n", output_filename, merged_filename);
      abort();
    }

    __merge_files(merged_filename, headFilename, output_filename);
  }
  release_lock(lockfile, lock_fd);
}

static trace_return_t
__pajeEndTrace_generic (int should_merge) {
  int i;
  my_close ();
  /* Wait for all proc to finish writing their trace */
  if(paje_flags & GTG_FLAG_PAJE_MULTIPLE_FILES) {
    for(i=0;i<nb_containers; i++) {
      __paje_destroy_container(&procFile[i]);
    }
  }

  if(should_merge) {
    merge_contrib(filename);
  }

  if (nameTmp)
    free (nameTmp);
  nameTmp = NULL;
  if (filename)
    free (filename);
  filename = NULL;
  return TRACE_SUCCESS;
}

trace_return_t pajeEndTrace (){
  return __pajeEndTrace_generic(1);
}

trace_return_t viteEndTrace (){
  return __pajeEndTrace_generic(0);
}

#endif
