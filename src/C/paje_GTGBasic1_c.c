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

/* File where each proc writes state/event/var/link changes. */
static FILE*  procFile;
/* File where each proc writes the header and the definitions of the data. */
static FILE*  headFile;
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
    sprintf (cmd, "rm -f %s_root.ept %s_proc*.ept", filename,filename);
    ret = system(cmd);
    if (ret == -1) {
        perror("cleanfiles");
    }
}

void merge (char* filename, int nbFile){
    doubleLinkList_t* first = NULL;
    doubleLinkList_t* list;
    doubleLinkList_t* iter;
    FILE            * res;
    FILE            * oldF;
    char            * buf = NULL;
    char              tmp[BUFFSIZE];
    char              tmp2[BUFFSIZE];
    size_t            size;
    int               i;

    /* Getting the first part of the trace (header+def) */
#if HAVE_LIBZ
    if(compression_ratio)
        sprintf (tmp, "%s.trace.z", filename);
    else
#endif
        sprintf (tmp, "%s.trace", filename);

    res = fopen (tmp, "w+");
    sprintf (tmp2, "%s_root.ept", filename);
    oldF = fopen (tmp2,"a+");
    if (res==NULL || oldF==NULL){
        fprintf (stderr, "Failed to create the trace file. Leaving\n");
        exit (-1);
    }
    rewind (oldF);
    do{
        i = getline (&buf, &size, oldF);
        if (size==0 || i==-1)
            break;
        paje_print (res, "%s", buf);
    }while (buf[0] != EOF);
    fclose (oldF);
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

trace_return_t my_open (int rk){
    trace_return_t ret = TRACE_ERR_OPEN;
    char f[BUFFSIZE];

    if (!procFile){
        if(!(paje_flags & GTG_FLAG_USE_MPI)) {
            procFile = headFile;
        } else {
            sprintf (f, "%s_proc%d.ept", filename, rk);
            procFile = fopen (f, "w");
        }
        if (!procFile)
            return ret;
        ret = TRACE_SUCCESS;
    }
    return ret;
}

trace_return_t my_close (){
    trace_return_t ret = TRACE_ERR_CLOSE;
    if (procFile){
        ret = (trace_return_t) fclose (procFile);
        procFile = NULL;
    }
    return ret;
}

trace_return_t pajeInitTrace   (const char* filenam, int rk, gtg_flag_t flags, int fmt) {
    char file[BUFFSIZE];
    trace_return_t ret = TRACE_ERR_OPEN;

    my_close ();
    rank = rk;	/* fixme: why there is need to copy filenam ????*/
    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);
    procFile = NULL;

    paje_flags = flags;
    if( rank==0 ||
        ! (flags & GTG_FLAG_USE_MPI)) {

        sprintf (file, "%s_root.ept", filename);
        headFile = fopen (file, "w");
        if (!headFile){
            fprintf (stderr, "Failed to open file %s. \n Leaving\n", file);
            return ret;
        }

        pajeInitHeaderData( fmt );

        FLUSH(headFile);
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

trace_return_t pajeAddVarType   (const char* alias   , const char* name,
                                 const char* contType){
    PRINT_PAJE_HEADER();

    if(verbose)
        printf("addVarType : alias %s, name %s, contType %s\n", alias, name, contType);
    if (headFile){
        fprintf (headFile, "%d \"%s\" \"%s\" \"%s\"\n",
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
        if(container && strcmp(container, "(null)")!=0 )
            fprintf (headFile, "%d %.13e \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
                     paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type, container, alias, file);
        else
            fprintf (headFile, "%d %.13e \"%s\" \"%s\" 0 \"%s\" \"%s\"\n",
                     paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type, alias, file);

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
        if(container && strcmp(container, "(null)")!=0 )
            fprintf (headFile, "%d %.13e \"%s\" \"%s\" \"%s\" \"%s\"\n",
                     paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type, container, alias);
        else
            fprintf (headFile, "%d %.13e \"%s\" \"%s\" 0 \"%s\"\n",
                     paje_eventdefs[GTG_PAJE_EVTDEF_CreateContainer].id, time, name, type, alias);

        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeDestroyContainer     (varPrec time, const char*  name,
                                         const char*  type){
    PRINT_PAJE_HEADER();

    if (headFile){
        fprintf (headFile, "%d %.13e \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_DestroyContainer].id, time, name, type);

        FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeSetState   (varPrec time, const char* type,
                               const char*  cont, const char* val){
    if(verbose)
        printf("SetState : type %s, container %s, val %s\n", type, cont, val);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_SetState].id, time, type, cont, val);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajePushState   (varPrec time, const char* type,
                                const char*  cont, const char* val){
    if(verbose)
        printf("PushState : type %s, container %s, val %s\n", type, cont, val);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_PushState].id, time, type, cont, val);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajePopState   (varPrec time, const char* type,
                               const char*  cont){
    if(verbose)
        printf("PopState : type %s, container %s\n", type, cont);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_PopState].id, time, type, cont);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddEvent   (varPrec time, const char* type,
                               const char*  cont, const char* val){
    if(verbose)
        printf("AddEvent : type %s, cont %s, val %s\n", type, cont, val);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_NewEvent].id, time, type, cont, val);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeStartLink   (varPrec time, const char* type,
                                const char*   cont, const char* src,
                                const char*   val , const char* key){
    if (verbose)
        printf ("Start link: type: %s container: %s src: %s val: %s key %s\n",
                type, cont, src, val, key);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_StartLink].id, time, type, cont, src, val, key);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeEndLink   (varPrec time, const char* type,
                              const char*   cont, const char* dest,
                              const char*   val , const char* key){
    if (verbose)
        printf ("End link type: %s container: %s src: %s val: %s key %s\n",
                type, cont, dest, val, key);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_EndLink].id, time, type, cont, dest, val, key);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeSetVar   (varPrec time, const char*  type,
                             const char*  cont, varPrec val){
    if (verbose)
        printf ("SetVar: type: %s container: %s value: %e\n",
                type, cont, val);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\" %e\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_SetVariable].id, time, type, cont, val);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddVar   (varPrec time, const char*  type,
                             const char*  cont, varPrec val){
    if (verbose)
        printf ("AddVar: type: %s container: %s value: %e\n",
                type, cont, val);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\" %e\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_AddVariable].id, time, type, cont, val);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeSubVar   (varPrec time, const char*  type,
                             const char*  cont, varPrec val){
    if (verbose)
        printf ("SubVar: type: %s container: %s value: %e\n",
                type, cont, val);
    if (procFile){
        fprintf (procFile, "%d %.13e \"%s\" \"%s\" %e\n",
                 paje_eventdefs[GTG_PAJE_EVTDEF_SubVariable].id, time, type, cont, val);

        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddComment   (const char*  comment){
    if (verbose)
        printf ("AddComment: %s\n",comment);

    if (procFile){
        fprintf(procFile,"#%s\r\n",comment);
        FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}


trace_return_t pajeEndTrace (){
    int size = 1;
    my_close ();
    /* Wait for all proc to finish writing their trace */
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
    my_close ();
    if (filename)
        free (filename);
    if (nameTmp)
        free (nameTmp);
    filename = NULL;
    nameTmp = NULL;
    return TRACE_SUCCESS;
}


/*
 * Hack that will be removed
 */
FILE *pajeGetProcFile() {
    return procFile;
}

#endif
