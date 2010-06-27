#ifdef BUILD_PAJE

#include "../../inc/paje_GTGBasic1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* File where each proc writes state/event/var/link changes. */
static FILE*  procFile;
/* File where each proc writes the header and the definitions of the data. */ 
static FILE*  headFile;
/* Root name */
static char*  filename;
/* Rank MPI */
static int    rank;
/* Size of MPI communiecator */
static int    size;
/* MPI communicator */
static MPI_Comm comm;

int my_open (){
    int ret = PAJE_ERR_OPEN;
    char f[200];
    if (!procFile){
        sprintf (f, "%s_proc%d.ept", filename, rank);
        procFile = fopen (f, "a+");
        if (!procFile)
            return ret;
        ret = PAJE_SUCCESS;
    }
    return ret;
}

int my_close (){
    int ret = PAJE_ERR_CLOSE;
    if (procFile){
        ret = fclose (procFile);
        procFile = NULL;
    }
    return ret;
}

int pajeInitTrace   (char* filenam){
    char file[200];
    int ret = PAJE_ERR_OPEN;
    my_close ();
    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);

    if (rank == 0){
        sprintf (file, "%s_root.ept", filename);
        headFile = fopen (file, "a+");
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
%% 	  Name 		string            \n\
%% 	  Filename	string            \n\
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
%% 	  Value         string            \n\
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
    }
    return my_open ();
}

int pajeSeqInitTrace   (char* filenam){
    char file[200];
    int ret = PAJE_ERR_OPEN;
    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);

    sprintf (file, "%s.trace", filename);
    procFile = fopen (file, "a+");
    headFile = procFile;
    if (!procFile){
        fprintf (stderr, "Failed to open file %s. \n Leaving \n", file);
        return ret;
    }
    fprintf (procFile,"\
%%EventDef PajeDefineContainerType	1 \n    \
%% 	  Alias 	string            \n    \
%% 	  ContainerType string            \n\
%% 	  Name 		string            \n\
%%EndEventDef                             \n\
%%EventDef PajeDefineStateType		3 \n\
%% 	  Alias 	string            \n\
%% 	  ContainerType string            \n\
%% 	  Name 		string            \n\
%%EndEventDef                             \n");
    fprintf (procFile,"\
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
    fprintf (procFile,"\
%%EventDef PajeCreateContainer		7 \n\
%% 	  Time          date              \n\
%% 	  Alias 	string            \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Name 		string            \n\
%%EndEventDef                             \n\
%%EventDef PajeDestroyContainer		8 \n\
%% 	  Time          date              \n\
%% 	  Name 		string            \n\
%% 	  Type 		string            \n\
%%EndEventDef                             \n");
    fprintf (procFile,"\
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
%% 	  Value         string            \n");
    fprintf (procFile,"\
%%EndEventDef                             \n\
%%EventDef PajePopState			12\n\
%% 	  Time          date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value         string            \n\
%%EndEventDef                             \n\
%%EventDef PajeNewEvent			20\n\
%% 	  Time          date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value         string            \n\
%%EndEventDef                             \n");
    fprintf (procFile,"\
%%EventDef PajeDefineLinkType		41\n\
%% 	  Alias               string      \n\
%% 	  Name 		      string      \n\
%% 	  ContainerType	      string      \n\
%% 	  SourceContainerType string      \n\
%% 	  DestContainerType   string      \n\
%%EndEventDef                             \n");
    fprintf (procFile,"\
%%EventDef PajeStartLink	       	42\n\
%%	  Time 		  date            \n\
%%	  Type 		  string          \n\
%%	  Container 	  string          \n\
%%	  SourceContainer string          \n\
%%	  Value 	  string          \n\
%%	  Key 		  string          \n\
%%EndEventDef                             \n");
    fprintf (procFile,"\
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
    fprintf (procFile,"\
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
    fprintf (procFile,"\
%%EventDef PajeSubVariable		53\n\
%% 	  Time 		date              \n\
%% 	  Type 		string            \n\
%% 	  Container 	string            \n\
%% 	  Value 	double            \n\
%%EndEventDef \n");
    return PAJE_SUCCESS;
}

int pajeAddProcType   (char* alias, char* contType, 
                       char* name){
    if (headFile){
        fprintf (headFile, "1 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddProcTypeNB (char* alias, char* contType, 
                       char* name){
    if (headFile){
        fprintf (headFile, "1 %s %s '%s' \n" , alias, contType, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddStateType   (char* alias, char* contType, 
                        char* name){
    if (headFile){
        fprintf (headFile, "3 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }    
    return PAJE_ERR_WRITE;
}

int pajeAddStateTypeNB (char* alias, char* contType, 
                        char* name){
    if (headFile){
        fprintf (headFile, "3 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddEventType   (char* alias, char* contType, 
                        char* name){
    if (headFile){
        fprintf (headFile, "4 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddEventTypeNB (char* alias, char* contType, 
                        char* name){
     
    if (headFile){
        fprintf (headFile, "4 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddLinkType   (char* alias   , char* name,
                       char* contType, char* srcContType,
                       char* destContType){
    if (headFile){
        fprintf (headFile, "41 %s %s %s %s %s \n", alias, name, contType, srcContType, destContType);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddLinkTypeNB (char* alias   , char* name,
                       char* contType, char* srcContType,
                       char* destContType){
     
    if (headFile){
        fprintf (headFile, "41 %s %s %s %s %s \n", alias, name, contType, srcContType, destContType);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddVarType   (char* alias   , char* name,
                      char* contType){
    if (headFile){
        fprintf (headFile, "50 %s %s %s \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddVarTypeNB (char* alias   , char* name,
                      char* contType){
     
    if (headFile){
        fprintf (headFile, "50 %s %s %s \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddEntityValue   (char* alias, char* entType, 
                          char* name , char* color){
    if (headFile){
        fprintf (headFile, "6 %s %s %s '%s'\n", alias, entType, name, color);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddEntityValueNB (char* alias, char* entType, 
                          char* name , char* color){
     
    if (headFile){
        fprintf (headFile, "6 %s %s %s '%s'\n", alias, entType, name, color);
        return PAJE_SUCCESS;
    }
    fflush (headFile);     
    return PAJE_ERR_WRITE;
}

int pajeAddContainer   (varPrec time, char* alias    ,
                        char*  type, char* container,
                        char*  name, char* file){
    if (headFile){
        fprintf (headFile, "7 %.13e %s %s %s %s %s\n", 
                 time, alias, type, container, name, file);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddContainerNB (varPrec time, char* alias    ,
                        char*  type, char* container,
                        char*  name, char* file){
     
    if (headFile){
        fprintf (headFile, "7 %.13e %s %s %s %s %s\n", 
                 time, alias, type, container, name, file);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}


int pajeSeqAddContainer   (varPrec time, char* alias    ,
                           char*  type, char* container,
                           char*  name){
    if (headFile){
        fprintf (headFile, "7 %.13e %s %s %s '%s'\n", 
                 time, alias, type, container, name);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeSeqAddContainerNB (varPrec time, char* alias    ,
                           char*  type, char* container,
                           char*  name){
     
    if (headFile){
        fprintf (headFile, "7 %.13e %s %s %s '%s' \n", 
                 time, alias, type, container, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}


int pajeDestroyContainer     (varPrec time, char*  name,
                              char*  type){
    if (headFile){
        fprintf (headFile, "8 %.13e %s %s \n", 
                 time, name, type);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeDestroyContainerNB   (varPrec time, char*  name,
                              char*  type){
     
    if (headFile){
        fprintf (headFile, "8 %.13e %s %s \n", 
                 time, name, type);
        return PAJE_SUCCESS;
    }
    fflush (headFile);     
    return PAJE_ERR_WRITE;
}

int pajeSetState   (varPrec time, char* type,
                    char*  cont, char* val){
    if (procFile){
        fprintf (procFile, "10 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeSetStateNB (varPrec time, char* type,
                    char*  cont, char* val){
     
    if (procFile){
        fprintf (procFile, "10 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajePushState   (varPrec time, char* type,
                     char*  cont, char* val){
    if (procFile){
        fprintf (procFile, "11 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajePushStateNB (varPrec time, char* type,
                     char*  cont, char* val){
     
    if (procFile){
        fprintf (procFile, "11 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajePopState   (varPrec time, char* type,
                    char*  cont, char* val){
    if (procFile){
        fprintf (procFile, "11 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajePopStateNB (varPrec time, char* type,
                    char*  cont, char* val){
     
    if (procFile){
        fprintf (procFile, "12 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);     
    return PAJE_ERR_WRITE;
}

int pajeAddEvent   (varPrec time, char* type,
                    char*  cont, char* val){
    if (procFile){
        fprintf (procFile, "20 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddEventNB (varPrec time, char* type,
                    char*  cont, char* val){
     
    if (procFile){
        fprintf (procFile, "20 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajeStartLink   (varPrec time, char* type,
                     char*   cont, char* src,
                     char*   val , char* key){
    if (procFile){
        fprintf (procFile, "42 %.13e %s %s %s %s %s\n", 
                 time, type, cont, src, val, key);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeStartLinkNB (varPrec time, char* type,
                     char*   cont, char* src,
                     char*   val , char* key){
     
    if (procFile){
        fprintf (procFile, "42 %.13e %s %s %s %s %s\n", 
                 time, type, cont, src, val, key);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajeEndLink   (varPrec time, char* type,
                   char*   cont, char* dest,
                   char*   val , char* key){
    if (procFile){
        fprintf (procFile, "43 %.13e %s %s %s %s %s\n", 
                 time, type, cont, dest, val, key);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeEndLinkNB (varPrec time, char* type,
                   char*   cont, char* dest,
                   char*   val , char* key){
     
    if (procFile){
        fprintf (procFile, "43 %.13e %s %s %s %s %s\n", 
                 time, type, cont, dest, val, key);
        return PAJE_SUCCESS;
    }
    fflush (procFile);     
    return PAJE_ERR_WRITE;
}

int pajeSetVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
    if (procFile){
        fprintf (procFile, "51 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeSetVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
     
    if (procFile){
        fprintf (procFile, "51 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajeAddVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
    if (procFile){
        fprintf (procFile, "52 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
     
    if (procFile){
        fprintf (procFile, "52 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);     
    return PAJE_ERR_WRITE;
}

int pajeSubVar   (varPrec time, char*  type,
                  char*  cont, varPrec val){
    if (procFile){
        fprintf (procFile, "53 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeSubVarNB (varPrec time, char*  type,
                  char*  cont, varPrec val){
     
    if (procFile){
        fprintf (procFile, "53 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }     
    return PAJE_ERR_WRITE;
}

int pajeSetComm (MPI_Comm c){
    comm = c;
    MPI_Comm_rank (comm, &rank);
    MPI_Comm_size (comm, &size);
    return PAJE_SUCCESS;
}

int pajeEndTrace (){
    my_close ();
    if (filename)
        free (filename);
    filename = NULL;
    return PAJE_SUCCESS;
}


#endif

