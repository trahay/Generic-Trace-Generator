#ifdef BUILD_PAJE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "paje_GTGBasic1.h"

/* File where each proc writes state/event/var/link changes. */
static FILE*  procFile;
/* File where each proc writes the header and the definitions of the data. */ 
static FILE*  headFile;
/* Root name */
static char*  filename;

int my_open (){
    int ret = PAJE_ERR_OPEN;
    char f[200];
    if (!procFile){
        sprintf (f, "%s_proc0.ept", filename);
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

int pajeInitTrace   (const char* filenam){
    char file[200];
    int ret = PAJE_ERR_OPEN;
    my_close ();
    filename = (char *)malloc (sizeof (char)* (strlen (filenam)+1));
    strcpy (filename, filenam);

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

    return my_open();
}

int pajeSeqInitTrace   (const char* filenam){
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

int pajeAddProcType   (const char* alias, const char* contType, 
                       const char* name){
    if (headFile){
        fprintf (headFile, "1 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddProcTypeNB (const char* alias, const char* contType, 
                       const char* name){
    if (headFile){
        fprintf (headFile, "1 %s %s '%s' \n" , alias, contType, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddStateType   (const char* alias, const char* contType, 
                        const char* name){
    if (headFile){
        fprintf (headFile, "3 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }    
    return PAJE_ERR_WRITE;
}

int pajeAddStateTypeNB (const char* alias, const char* contType, 
                        const char* name){
    if (headFile){
        fprintf (headFile, "3 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddEventType   (const char* alias, const char* contType, 
                        const char* name){
    if (headFile){
        fprintf (headFile, "4 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddEventTypeNB (const char* alias, const char* contType, 
                        const char* name){
     
    if (headFile){
        fprintf (headFile, "4 %s %s '%s' \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddLinkType   (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType){
    if (headFile){
        fprintf (headFile, "41 %s %s %s %s %s \n", alias, name, contType, srcContType, destContType);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddLinkTypeNB (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType){
     
    if (headFile){
        fprintf (headFile, "41 %s %s %s %s %s \n", alias, name, contType, srcContType, destContType);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddVarType   (const char* alias   , const char* name,
                      const char* contType){
    if (headFile){
        fprintf (headFile, "50 %s %s %s \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddVarTypeNB (const char* alias   , const char* name,
                      const char* contType){
     
    if (headFile){
        fprintf (headFile, "50 %s %s %s \n", alias, contType, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}

int pajeAddEntityValue   (const char* alias, const char* entType, 
                          const char* name , const char* color){
    if (headFile){
        fprintf (headFile, "6 %s %s %s '%s'\n", alias, entType, name, color);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddEntityValueNB (const char* alias, const char* entType, 
                          const char* name , const char* color){
     
    if (headFile){
        fprintf (headFile, "6 %s %s %s '%s'\n", alias, entType, name, color);
        return PAJE_SUCCESS;
    }
    fflush (headFile);     
    return PAJE_ERR_WRITE;
}

int pajeAddContainer   (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name, const char* file){
    if (headFile){
        fprintf (headFile, "7 %.13e %s %s %s %s %s\n", 
                 time, alias, type, container, name, file);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddContainerNB (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name, const char* file){
     
    if (headFile){
        fprintf (headFile, "7 %.13e %s %s %s %s %s\n", 
                 time, alias, type, container, name, file);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}


int pajeSeqAddContainer   (varPrec time, const char* alias    ,
                           const char*  type, const char* container,
                           const char*  name){
    if (headFile){
        fprintf (headFile, "7 %.13e %s %s %s '%s'\n", 
                 time, alias, type, container, name);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeSeqAddContainerNB (varPrec time, const char* alias    ,
                           const char*  type, const char* container,
                           const char*  name){
     
    if (headFile){
        fprintf (headFile, "7 %.13e %s %s %s '%s' \n", 
                 time, alias, type, container, name);
        return PAJE_SUCCESS;
    }
    fflush (headFile);
    return PAJE_ERR_WRITE;
}


int pajeDestroyContainer     (varPrec time, const char*  name,
                              const char*  type){
    if (headFile){
        fprintf (headFile, "8 %.13e %s %s \n", 
                 time, name, type);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeDestroyContainerNB   (varPrec time, const char*  name,
                              const char*  type){
     
    if (headFile){
        fprintf (headFile, "8 %.13e %s %s \n", 
                 time, name, type);
        return PAJE_SUCCESS;
    }
    fflush (headFile);     
    return PAJE_ERR_WRITE;
}

int pajeSetState   (varPrec time, const char* type,
                    const char*  cont, const char* val){
    if (procFile){
        fprintf (procFile, "10 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeSetStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val){
     
    if (procFile){
        fprintf (procFile, "10 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajePushState   (varPrec time, const char* type,
                     const char*  cont, const char* val){
    if (procFile){
        fprintf (procFile, "11 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajePushStateNB (varPrec time, const char* type,
                     const char*  cont, const char* val){
     
    if (procFile){
        fprintf (procFile, "11 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajePopState   (varPrec time, const char* type,
                    const char*  cont, const char* val){
    if (procFile){
        fprintf (procFile, "11 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajePopStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val){
     
    if (procFile){
        fprintf (procFile, "12 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);     
    return PAJE_ERR_WRITE;
}

int pajeAddEvent   (varPrec time, const char* type,
                    const char*  cont, const char* val){
    if (procFile){
        fprintf (procFile, "20 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddEventNB (varPrec time, const char* type,
                    const char*  cont, const char* val){
     
    if (procFile){
        fprintf (procFile, "20 %.13e %s %s %s\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajeStartLink   (varPrec time, const char* type,
                     const char*   cont, const char* src,
                     const char*   val , const char* key){
    if (procFile){
        fprintf (procFile, "42 %.13e %s %s %s %s %s\n", 
                 time, type, cont, src, val, key);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeStartLinkNB (varPrec time, const char* type,
                     const char*   cont, const char* src,
                     const char*   val , const char* key){
     
    if (procFile){
        fprintf (procFile, "42 %.13e %s %s %s %s %s\n", 
                 time, type, cont, src, val, key);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajeEndLink   (varPrec time, const char* type,
                   const char*   cont, const char* dest,
                   const char*   val , const char* key){
    if (procFile){
        fprintf (procFile, "43 %.13e %s %s %s %s %s\n", 
                 time, type, cont, dest, val, key);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeEndLinkNB (varPrec time, const char* type,
                   const char*   cont, const char* dest,
                   const char*   val , const char* key){
     
    if (procFile){
        fprintf (procFile, "43 %.13e %s %s %s %s %s\n", 
                 time, type, cont, dest, val, key);
        return PAJE_SUCCESS;
    }
    fflush (procFile);     
    return PAJE_ERR_WRITE;
}

int pajeSetVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
    if (procFile){
        fprintf (procFile, "51 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeSetVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
     
    if (procFile){
        fprintf (procFile, "51 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);
    return PAJE_ERR_WRITE;
}

int pajeAddVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
    if (procFile){
        fprintf (procFile, "52 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeAddVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
     
    if (procFile){
        fprintf (procFile, "52 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    fflush (procFile);     
    return PAJE_ERR_WRITE;
}

int pajeSubVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
    if (procFile){
        fprintf (procFile, "53 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }
    return PAJE_ERR_WRITE;
}

int pajeSubVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val){
     
    if (procFile){
        fprintf (procFile, "53 %.13e %s %s %e\n", 
                 time, type, cont, val);
        return PAJE_SUCCESS;
    }     
    return PAJE_ERR_WRITE;
}

int pajeEndTrace (){
    my_close ();
    if (filename)
        free (filename);
    filename = NULL;
    return PAJE_SUCCESS;
}


#endif

