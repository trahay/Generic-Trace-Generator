#ifdef BUILD_PAJE

#define _GNU_SOURCE_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef USE_MPI
#include <mpi.h>
#endif

#include "paje_GTGBasic1.h"


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
#define FLUSH(file) \
	if(paje_flags & GTG_FLAG_NOTBUF) \
		fflush(file)

// Structure containing the info of a current line
typedef struct line_t{
    FILE*   file;  // File
    char*   value; // The value of the line
    int     size;  // Size of the value buffer
    varPrec time;  // Time (second element of the line)
}line_t;

typedef struct doubleLinkList_t{
    struct doubleLinkList_t* prev; // Previous element
    struct doubleLinkList_t* next; // Next element
    line_t current;         // Current line
}doubleLinkList_t;

// Insert a doubleLinkList_t item in the sorted doubleLinkedList starting with first
void insert (doubleLinkList_t** first, doubleLinkList_t* item){
    varPrec time;
    doubleLinkList_t* iter = *first;
    if (!first || !item){
        fprintf (stderr, "Invalid insert parameter. Leaving \n");
        exit(-1);
    }
    time = item->current.time;
    // If nil list, item is the first and leaving
    if (iter==NULL){
        *first=item;
        item->prev = NULL;
        item->next = NULL;
        return;
    }
    if (time<iter->current.time){
        item->prev = NULL;
        item->next = *first;
        *first = item;
    }
    // Positionning iterator to insert in a sorted list
    while (time>iter->current.time && iter->next)
        iter = iter->next;

    if (time<iter->current.time){
        iter->prev = iter->prev;
        item->next = iter;
    }
    else{
        item->prev = iter;
        item->next=NULL;
    }
}

void initMerge (char* file, int rk, doubleLinkList_t* li){
    char f[BUFFSIZE];

    sprintf (f, "%s_proc%d.ept", file, rk);
    li->current.file = fopen (f, "a+");
    if (!li->current.file){
        fprintf (stderr, "Failed to open generated trace. Leaving \n");
        exit (-1);
    }
    rewind (li->current.file);
    li->current.value = NULL;
}

void myGetLine (doubleLinkList_t* li){
    int toto;
    int ret;
    if (li->current.value)
        free (li->current.value);
    li->current.value = NULL;
    ret = getline (&(li->current.value), &(li->current.size), li->current.file);
    if (ret != -1)
        sscanf (li->current.value, "%d %13e", &toto, &(li->current.time));

}

void clean_files (char* fileName, int nbFile){
    char cmd[BUFFSIZE];
    sprintf (cmd, "rm %s_root.ept %s_proc*.ept", filename,filename);
    system (cmd);
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
    int               size;
    int               i;          

    // Getting the first part of the trace (header+def)
    sprintf (tmp, "%s.trace", filename);
    res = fopen (tmp, "w+");
    sprintf (tmp2, "%s_root.ept", filename);
    oldF = fopen (tmp2,"a+");
    if (res==NULL || oldF==NULL){
        fprintf (stderr, "Failed to create the trace file. Leaving \n");
        exit (-1);
    }
    rewind (oldF);
    do{
        i = getline (&buf, &size, oldF);
        if (size==0 || i==-1)
            break;
        fprintf (res, "%s", buf);
    }while (buf[0] != EOF);
    fclose (oldF);
    // Initialising the parallel merge
    list = (doubleLinkList_t *)malloc (sizeof (doubleLinkList_t)*nbFile);
    for (i=0;i<nbFile;i++)
        initMerge (filename, i, list+i);

    for (i=0;i<nbFile;i++){
        myGetLine (list+i);
        insert (&first, list+i);
    }
    while (first){
        fprintf (res, "%s", first->current.value);
        myGetLine (first);
        iter = first;
        first = first->next;
        if (first)
            first->prev = NULL;
        if (iter->current.value[0] != '\0')
            insert (&first, iter);
//        if (first->current.file==EOF)
//            break;
    }
    fclose (res);
    clean_files (filename, nbFile);
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
	    if(!paje_flags & GTG_FLAG_USE_MPI) {
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

trace_return_t pajeInitTrace   (const char* filenam, int rk, gtg_flag_t flags, int fmt){

    char file[BUFFSIZE];
    trace_return_t ret = TRACE_ERR_OPEN;

    my_close ();
    rank = rk;
    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);
    procFile = NULL;

    paje_flags = flags;
    if( rank==0 ||
	! (flags & GTG_FLAG_USE_MPI)) {

        sprintf (file, "%s_root.ept", filename);
        headFile = fopen (file, "w");
        if (!headFile){
            fprintf (stderr, "Failed to open file %s. \n Leaving \n", file);
            return ret;
        }
        fprintf (headFile,"\
%%EventDef PajeDefineContainerType	1 \n\
%% 	  Alias 	string            \n\
%% 	  ContainerType string            \n\
%% 	  Name 		string            \n\
%%EndEventDef                             \n\
%%EventDef PajeDefineStateType		3 \n\
%% 	  Alias 	string            \n\
%% 	  ContainerType string            \n\
%% 	  Name 		string            \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajeDefineEventType		4 \n\
%% 	  Alias 	string            \n\
%% 	  ContainerType string            \n\
%% 	  Name 		string            \n\
%%EndEventDef                             \n\
%%EventDef PajeDefineEntityValue	6 \n\
%% 	  Alias         string            \n\
%% 	  EntityType 	string            \n\
%% 	  Name 		string            \n\
%% 	  Color 	color             \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajeCreateContainer		7 \n\
%% 	  Time          date              \n\
%% 	  Alias 	string            \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Name 		string            \n");
        if(fmt==FMT_VITE)
            fprintf (headFile,"\
%% 	  FileName	string            \n");
            fprintf (headFile,"\
%%EndEventDef                             \n\
%%EventDef PajeDestroyContainer		8 \n\
%% 	  Time          date              \n\
%% 	  Name 		string            \n\
%% 	  Type 		string            \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajeSetState			10\n\
%% 	  Time          date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value         string            \n\
%%EndEventDef                             \n\
%%EventDef PajePushState	      	11\n\
%% 	  Time          date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value         string            \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajePopState			12\n\
%% 	  Time          date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%%EndEventDef                             \n\
%%EventDef PajeNewEvent			20\n\
%% 	  Time          date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value         string            \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajeDefineLinkType		41\n\
%% 	  Alias               string      \n\
%% 	  Name 		      string      \n\
%% 	  ContainerType	      string      \n\
%% 	  SourceContainerType string      \n\
%% 	  DestContainerType   string      \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajeStartLink	       	42\n\
%%	  Time 		  date            \n\
%%	  Type 		  string          \n\
%%	  Container 	  string          \n\
%%	  SourceContainer string          \n\
%%	  Value 	  string          \n\
%%	  Key 		  string          \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajeEndLink			43\n\
%% 	  Time          date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  DestContainer string            \n\
%% 	  Value 	string            \n\
%% 	  Key 		string            \n\
%%EndEventDef                             \n\
%%EventDef PajeDefineVariableType	50\n\
%%	  Alias         string            \n\
%% 	  Name  	string            \n\
%% 	  ContainerType string            \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajeSetVariable		51\n\
%% 	  Time          date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value 	double            \n\
%%EndEventDef                             \n\
%%EventDef PajeAddVariable		52\n\
%% 	  Time          date              \n\
%%	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value 	double            \n\
%%EndEventDef                             \n");
        fprintf (headFile,"\
%%EventDef PajeSubVariable		53\n\
%% 	  Time 		date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value 	double            \n\
%%EndEventDef \n");

	FLUSH(headFile);
    }
    return my_open(rank);
}
//
//trace_return_t pajeSeqInitTrace   (const char* filenam, int rk){
//    char file[200];
//    trace_return_t ret = TRACE_ERR_OPEN;
//    rank = rk;
//    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
//    strcpy (filename, filenam);
//
//    sprintf (file, "%s.trace", filename);
//    procFile = fopen (file, "w");
//    headFile = procFile;
//    if (!procFile){
//        fprintf (stderr, "Failed to open file %s. \n Leaving \n", file);
//        return ret;
//    }
//    fprintf (procFile,"\
//%%EventDef PajeDefineContainerType	1 \n    \
//%% 	  Alias 	string            \n    \
//%% 	  ContainerType string            \n\
//%% 	  Name 		string            \n\
//%%EndEventDef                             \n\
//%%EventDef PajeDefineStateType		3 \n\
//%% 	  Alias 	string            \n\
//%% 	  ContainerType string            \n\
//%% 	  Name 		string            \n\
//%%EndEventDef                             \n");
//    fprintf (procFile,"\
//%%EventDef PajeDefineEventType		4 \n\
//%% 	  Alias 	string            \n\
//%% 	  ContainerType string            \n\
//%% 	  Name 		string            \n\
//%%EndEventDef                             \n\
//%%EventDef PajeDefineEntityValue	6 \n\
//%% 	  Alias         string            \n\
//%% 	  EntityType 	string            \n\
//%% 	  Name 		string            \n\
//%% 	  Color 	color             \n\
//%%EndEventDef                             \n");
//    fprintf (procFile,"\
//%%EventDef PajeCreateContainer		7 \n\
//%% 	  Time          date              \n\
//%% 	  Alias 	string            \n\
//%% 	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%% 	  Name 		string            \n\
//%%EndEventDef                             \n\
//%%EventDef PajeDestroyContainer		8 \n\
//%% 	  Time          date              \n\
//%% 	  Name 		string            \n\
//%% 	  Type 		string            \n\
//%%EndEventDef                             \n");
//    fprintf (procFile,"\
//%%EventDef PajeSetState			10\n\
//%% 	  Time          date              \n\
//%% 	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%% 	  Value         string            \n\
//%%EndEventDef                             \n\
//%%EventDef PajePushState	      	11\n\
//%% 	  Time          date              \n\
//%% 	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%% 	  Value         string            \n");
//    fprintf (procFile,"\
//%%EndEventDef                             \n\
//%%EventDef PajePopState			12\n\
//%% 	  Time          date              \n\
//%% 	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%%EndEventDef                             \n\
//%%EventDef PajeNewEvent			20\n\
//%% 	  Time          date              \n\
//%% 	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%% 	  Value         string            \n\
//%%EndEventDef                             \n");
//    fprintf (procFile,"\
//%%EventDef PajeDefineLinkType		41\n\
//%% 	  Alias               string      \n\
//%% 	  Name 		      string      \n\
//%% 	  ContainerType	      string      \n\
//%% 	  SourceContainerType string      \n\
//%% 	  DestContainerType   string      \n\
//%%EndEventDef                             \n");
//    fprintf (procFile,"\
//%%EventDef PajeStartLink	       	42\n\
//%%	  Time 		  date            \n\
//%%	  Type 		  string          \n\
//%%	  Container 	  string          \n\
//%%	  SourceContainer string          \n\
//%%	  Value 	  string          \n\
//%%	  Key 		  string          \n\
//%%EndEventDef                             \n");
//    fprintf (procFile,"\
//%%EventDef PajeEndLink			43\n\
//%% 	  Time          date              \n\
//%% 	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%% 	  DestContainer string            \n\
//%% 	  Value 	string            \n\
//%% 	  Key 		string            \n\
//%%EndEventDef                             \n\
//%%EventDef PajeDefineVariableType	50\n\
//%%	  Alias         string            \n\
//%% 	  Name  	string            \n\
//%% 	  ContainerType string            \n\
//%%EndEventDef                             \n");
//    fprintf (procFile,"\
//%%EventDef PajeSetVariable		51\n\
//%% 	  Time          date              \n\
//%% 	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%% 	  Value 	double            \n\
//%%EndEventDef                             \n\
//%%EventDef PajeAddVariable		52\n\
//%% 	  Time          date              \n\
//%%	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%% 	  Value 	double            \n\
//%%EndEventDef                             \n");
//    fprintf (procFile,"\
//%%EventDef PajeSubVariable		53\n\
//%% 	  Time 		date              \n\
//%% 	  Type 		string            \n\
//%% 	  Container 	string            \n\
//%% 	  Value 	double            \n\
//%%EndEventDef \n");
//    return TRACE_SUCCESS;
//}
//
trace_return_t pajeAddProcType   (const char* alias, const char* contType, 
                       const char* name){
    if(verbose)
        printf("addCtType : name %s, alias %s, contType %s\n", name, alias, contType);
    if (headFile){
        fprintf (headFile, "1 '%s' '%s' '%s' \n", alias, contType, name);

	FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddStateType   (const char* alias, const char* contType, 
                        const char* name){
    if(verbose)
	    printf("addStateType : alias %s, name %s, contType %s\n",  alias, name, contType);
    if (headFile){
        fprintf (headFile, "3 '%s' '%s' '%s' \n", alias, contType, name);

	FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddEventType   (const char* alias, const char* contType, 
                        const char* name){
    if(verbose)
	    printf("addEventType :  alias %s, name %s, contType %s\n", alias, name, contType);
    if (headFile){
        fprintf (headFile, "4 '%s' '%s' '%s' \n", alias, contType, name);

	FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddLinkType   (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType){
    if (verbose)
        fprintf (headFile, "Add link type: alias: %s name: %s contType: %s src: %s des: %s \n", alias, name, contType, srcContType, destContType);
    if (headFile){
        fprintf (headFile, "41 '%s' '%s' '%s' '%s' '%s' \n", alias, name, contType, srcContType, destContType);

	FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddVarType   (const char* alias   , const char* name,
                      const char* contType){
    if(verbose)
	    printf("addVarType : alias %s, name %s, contType %s\n", alias, name, contType);
    if (headFile){
        fprintf (headFile, "50 '%s' '%s' '%s' \n", alias, contType, name);

	FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddEntityValue   (const char* alias, const char* entType, 
                          const char* name , const char* color){
    if(verbose)
        printf("addEntityValue : color %s, alias %s, name %s, type %s\n", color, alias, name, entType);
    if (headFile){
        fprintf (headFile, "6 '%s' '%s' '%s' '%s'\n", alias, entType, name, color);

	FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeAddContainer   (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name, const char* file){
    if (headFile){
        fprintf (headFile, "7 %.13e '%s' '%s' '%s' '%s' '%s'\n", 
                 time, alias, type, container, name, file);

	FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeSeqAddContainer   (varPrec time, const char* alias    ,
                           const char*  type, const char* container,
                           const char*  name){
    if (headFile){
        fprintf (headFile, "7 %.13e '%s' '%s' '%s' '%s'\n", 
                 time, alias, type, container, name);

	FLUSH(headFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeDestroyContainer     (varPrec time, const char*  name,
                              const char*  type){
    if (headFile){
        fprintf (headFile, "8 %.13e '%s' '%s' \n", 
                 time, name, type);

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
        fprintf (procFile, "10 %.13e '%s' '%s' '%s'\n", 
                 time, type, cont, val);

	FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajePushState   (varPrec time, const char* type,
                     const char*  cont, const char* val){
    if(verbose)
        printf("MushState : type %s, container %s, val %s\n", type, cont, val);
    if (procFile){
        fprintf (procFile, "11 %.13e '%s' '%s' '%s'\n", 
                 time, type, cont, val);

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
        fprintf (procFile, "12 %.13e '%s' '%s'\n", 
                 time, type, cont);

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
        fprintf (procFile, "20 %.13e '%s' '%s' '%s'\n", 
                 time, type, cont, val);

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
        fprintf (procFile, "42 %.13e '%s' '%s' '%s' '%s' '%s'\n", 
                 time, type, cont, src, val, key);

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
        fprintf (procFile, "43 %.13e '%s' '%s' '%s' '%s' '%s'\n", 
                 time, type, cont, dest, val, key);

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
        fprintf (procFile, "51 %.13e '%s' '%s' %e\n", 
                 time, type, cont, val);

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
        fprintf (procFile, "52 %.13e '%s' '%s' %e\n", 
                 time, type, cont, val);

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
        fprintf (procFile, "53 %.13e '%s' '%s' %e\n", 
                 time, type, cont, val);

	FLUSH(procFile);
        return TRACE_SUCCESS;
    }
    return TRACE_ERR_WRITE;
}

trace_return_t pajeEndTrace (){
    int size;
    my_close ();
    // Wait for all proc to finish writing their trace
#ifdef USE_MPI
    MPI_Barrier (MPI_COMM_WORLD);
    MPI_Comm_rank (MPI_COMM_WORLD, &size);
    if(verbose)
        fprintf (stderr, "USING MPI \n");
    if (rank==0)
        merge (filename, size);
#else
    if(verbose)
        fprintf (stderr, "NOT USING MPI \n");
    merge (filename, 1);
#endif	/* USE_MPI */
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


#endif

