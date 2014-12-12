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

gtg_paje_eventdef_t paje_eventdefs[GTG_PAJE_EVTDEF_NBR] = {
    { "PajeDefineContainerType", 0, NULL, NULL},
    { "PajeDefineStateType",     1, NULL, NULL},
    { "PajeDefineEventType",     2, NULL, NULL},
    { "PajeDefineVariableType",  3, NULL, NULL},
    { "PajeDefineLinkType",      4, NULL, NULL},
    { "PajeDefineEntityValue",   5, NULL, NULL},

    { "PajeCreateContainer",     7, NULL, NULL},
    { "PajeDestroyContainer",    8, NULL, NULL},
    { "PajeSetState",           10, NULL, NULL},
    { "PajePushState",          11, NULL, NULL},
    { "PajePopState",           12, NULL, NULL},
    { "PajeResetState",         13, NULL, NULL},
    { "PajeNewEvent",           20, NULL, NULL},
    { "PajeSetVariable",        30, NULL, NULL},
    { "PajeAddVariable",        31, NULL, NULL},
    { "PajeSubVariable",        32, NULL, NULL},
    { "PajeStartLink",          40, NULL, NULL},
    { "PajeEndLink",            41, NULL, NULL},
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
    if ( (paje_eventdefs[event].first) != NULL ) {                   \
        gtg_paje_edp_t *e, *n;                                  \
        for( e = paje_eventdefs[event].first; e != NULL; ) {         \
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

    if( paje_eventdefs[event].first == NULL ) {
        paje_eventdefs[event].first = node;
        paje_eventdefs[event].last = node;
    } else {
        paje_eventdefs[event].last->next = node;
        paje_eventdefs[event].last = node;
    }
}

void pajeEventDefDeleteParams( enum gtg_paje_evtdef_e event )
{
    gtg_paje_edp_t *e, *n;

    for( e = paje_eventdefs[event].first; e != NULL; ) {
        n = e->next;

        free(e->name);
        free(e);

        e = n;
    }
    paje_eventdefs[event].first = NULL;
    paje_eventdefs[event].last = NULL;
}

void pajeEventDefClean()
{
    int i;
    for(i=0; i<GTG_PAJE_EVTDEF_NBR; i++) {
        pajeEventDefDeleteParams( i );
    }
}

void pajeInitHeaderData( int fmt, gtg_flag_t paje_flags)
{
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineContainerType, "Name",  GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineContainerType, "Type",  GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineContainerType, "Alias", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineStateType, "Name", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineStateType, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineStateType, "Alias", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEventType, "Name", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEventType, "Type", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEventType, "Alias", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineVariableType, "Name",  GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineVariableType, "Type",  GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineVariableType, "Color", GTG_PAJE_FIELDTYPE_Color  );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineVariableType, "Alias", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "Name",               GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "Type",               GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "StartContainerType", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "EndContainerType",   GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineLinkType, "Alias",              GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEntityValue, "Type",  GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEntityValue, "Name",  GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEntityValue, "Color", GTG_PAJE_FIELDTYPE_Color  );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DefineEntityValue, "Alias", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Time",      GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Name",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "Alias",     GTG_PAJE_FIELDTYPE_String );

    if ( fmt == FMT_VITE || paje_flags & GTG_FLAG_PAJE_MULTIPLE_FILES) {
      pajeEventDefAddParam( GTG_PAJE_EVTDEF_CreateContainer, "FileName", GTG_PAJE_FIELDTYPE_String );
    }

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DestroyContainer, "Time", GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DestroyContainer, "Name", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_DestroyContainer, "Type", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetState, "Time",      GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetState, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetState, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetState, "Value",     GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PushState, "Time",      GTG_PAJE_FIELDTYPE_Date   );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PushState, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PushState, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PushState, "Value",     GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PopState, "Time",      GTG_PAJE_FIELDTYPE_Date   );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PopState, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_PopState, "Container", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_ResetState, "Time",      GTG_PAJE_FIELDTYPE_Date   );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_ResetState, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_ResetState, "Container", GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_NewEvent, "Time",      GTG_PAJE_FIELDTYPE_Date );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_NewEvent, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_NewEvent, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_NewEvent, "Value",     GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetVariable, "Time",      GTG_PAJE_FIELDTYPE_Date   );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetVariable, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetVariable, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SetVariable, "Value",     GTG_PAJE_FIELDTYPE_Double );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_AddVariable, "Time",      GTG_PAJE_FIELDTYPE_Date   );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_AddVariable, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_AddVariable, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_AddVariable, "Value",     GTG_PAJE_FIELDTYPE_Double );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SubVariable, "Time",      GTG_PAJE_FIELDTYPE_Date   );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SubVariable, "Type",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SubVariable, "Container", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_SubVariable, "Value",     GTG_PAJE_FIELDTYPE_Double );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Time",           GTG_PAJE_FIELDTYPE_Date   );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Type",           GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Container",      GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "StartContainer", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Value",          GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_StartLink, "Key",            GTG_PAJE_FIELDTYPE_String );

    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Time",         GTG_PAJE_FIELDTYPE_Date   );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Type",         GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Container",    GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "EndContainer", GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Value",        GTG_PAJE_FIELDTYPE_String );
    pajeEventDefAddParam( GTG_PAJE_EVTDEF_EndLink, "Key",          GTG_PAJE_FIELDTYPE_String );

}

void pajeWriteHeader( FILE *file )
{
    int i;

    _is_paje_header_written = 1;
    for(i=0; i<GTG_PAJE_EVTDEF_NBR; i++) {
        fprintf(file, "%%EventDef %s %d\n",
                paje_eventdefs[i].name, paje_eventdefs[i].id);
        printExtra( file, i );
        fprintf(file, "%%EndEventDef\n");
    }
    pajeEventDefClean();
}

#endif /* BUILD_PAJE */
