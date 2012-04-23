#ifdef BUILD_PAJE

#define _GNU_SOURCE_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef USE_MPI
#include <mpi.h>
#endif

#include "GTG.h"
#include "GTGPaje.h"

int _is_paje_header_written = 0;

struct gtg_paje_edp_s {
    struct gtg_paje_edp_s *next;
    char *name;
    enum gtg_paje_fieldtype_e type;    
};
typedef struct gtg_paje_edp_s gtg_paje_edp_t;

struct gtg_paje_eventdef_s {
    char *name;
    int id;
    gtg_paje_edp_t *first;
    gtg_paje_edp_t *last;
};
typedef struct gtg_paje_eventdef_s gtg_paje_eventdef_t;

gtg_paje_eventdef_t eventdefs[GTG_PAJE_EVTDEF_NBR] = {
    { "PajeDefineContainerType", 1, NULL, NULL},
    { "PajeDefineStateType",     3, NULL, NULL},
    { "PajeDefineEventType",     4, NULL, NULL},
    { "PajeDefineEntityValue",   6, NULL, NULL},
    { "PajeCreateContainer",     7, NULL, NULL},
    { "PajeDestroyContainer",    8, NULL, NULL},
    { "PajeSetState",           10, NULL, NULL},
    { "PajePushState",          11, NULL, NULL},
    { "PajePopState",           12, NULL, NULL},
    { "PajeNewEvent",           20, NULL, NULL},
    { "PajeDefineLinkType",     41, NULL, NULL},
    { "PajeStartLink",          42, NULL, NULL},
    { "PajeEndLink",            43, NULL, NULL},
    { "PajeDefineVariableType", 50, NULL, NULL},
    { "PajeSetVariable",        51, NULL, NULL},
    { "PajeAddVariable",        52, NULL, NULL},
    { "PajeSubVariable",        53, NULL, NULL},
};

char *FieldType[GTG_PAJE_FIELDTYPE_NBR] = {
  "int",
  "hex",
  "date",
  "double",
  "string",
  "color"  
};

#define printExtra( file, event )                               \
    if ( (eventdefs[event].first) != NULL ) {                   \
        gtg_paje_edp_t *e, *n;                                  \
        for( e = eventdefs[event].first; e != NULL; ) {         \
            n = e->next;                                        \
            fprintf(file, "%% %s %s\n", e->name, FieldType[e->type]); \
            e = n;                                              \
        }                                                       \
    }

/*
 * args needs to be set to 0 before the first call
 */
void pajeEventDefAddParam( enum gtg_paje_evtdef_e event, const char *name, 
                           enum gtg_paje_fieldtype_e type )
{
    gtg_paje_edp_t *node = (gtg_paje_edp_t*)malloc(sizeof(gtg_paje_edp_t));

    if (_is_paje_header_written) {
        fprintf(stderr, "GTG [PAJE/VITE]: You cannot add parameters to definitions after the header has been written to the file\n");
        return;
    }

    node->next = NULL;
    node->name = strdup(name);
    node->type = type;

    if( eventdefs[event].first == NULL ) {
        eventdefs[event].first = node;
        eventdefs[event].last = node;
    } else {
        eventdefs[event].last->next = node;
        eventdefs[event].last = node;
    }
}

void pajeEventDefDeleteParams( enum gtg_paje_evtdef_e event )
{
    gtg_paje_edp_t *e, *n;

    for( e = eventdefs[event].first; e != NULL; ) {
        n = e->next;
        
        free(e->name);
        free(e);

        e = n;
    }
    eventdefs[event].first = NULL;
    eventdefs[event].last = NULL;
}

void pajeEventDefClean()
{
    int i;
    for(i=0; i<GTG_PAJE_EVTDEF_NBR; i++) {
        pajeEventDefDeleteParams( i );
    }
}

void pajeInitHeaderData( int fmt )
{
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineContainerType, "Alias", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineContainerType, "ContainerType", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineContainerType, "Name", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineStateType, "Alias", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineStateType, "ContainerType", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineStateType, "Name", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEventType, "Alias", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEventType, "ContainerType", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEventType, "Name", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEntityValue, "Alias", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEntityValue, "EntityType", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEntityValue, "Name", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEntityValue, "Color", GTG_PAJE_FIELDTYPE_Color );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Alias", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Name", GTG_PAJE_FIELDTYPE_String );
    if ( fmt == FMT_VITE )
        pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "FileName", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DestroyContainer, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DestroyContainer, "Name", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DestroyContainer, "Type", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetState, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetState, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetState, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetState, "Value", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PushState, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PushState, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PushState, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PushState, "Value", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PopState, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PopState, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PopState, "Container", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_NewEvent, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_NewEvent, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_NewEvent, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_NewEvent, "Value", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "Alias", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "Name", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "ContainerType", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "SourceContainerType", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "DestContainerType", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "SourceContainer", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Value", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Key", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "DestContainer", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Value", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Key", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineVariableType, "Alias", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineVariableType, "Name", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineVariableType, "ContainerType", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetVariable, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetVariable, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetVariable, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetVariable, "Value", GTG_PAJE_FIELDTYPE_Double );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_AddVariable, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_AddVariable, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_AddVariable, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_AddVariable, "Value", GTG_PAJE_FIELDTYPE_Double );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SubVariable, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SubVariable, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SubVariable, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SubVariable, "Value", GTG_PAJE_FIELDTYPE_Double );
}

void pajeWriteHeader( FILE *file )
{
    int i;
    
    _is_paje_header_written = 1;
    for(i=0; i<GTG_PAJE_EVTDEF_NBR; i++) {
        fprintf(file, "%%EventDef %s %d\n",
                eventdefs[i].name, eventdefs[i].id);
        printExtra( file, i );
        fprintf(file, "%%EndEventDef\n");
    }
    pajeEventDefClean();
}

#endif /* BUILD_PAJE */
