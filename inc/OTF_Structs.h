/**
 *  \file OTF_Structs.h
 *  \version 0.1
 *  \brief
 *  OTF_Structs gives the global types and functions needed to have the OTF implementation. \n
 *
 *  \authors
 *    Developers are : \n
 *        Francois Rue      - francois.rue@labri.fr \n
 *        Francois Trahay   - francois.trahay@labri.fr \n
 *        Johnny   Jazeix   - jazeix@enseirb-matmeca.fr \n
 *        Kevin    Coulomb  - coulomb@enseirb-matmeca.fr \n
 *        Mathieu  Faverge  - faverge@labri.fr \n
 *        Olivier  Lagrasse - lagrasse@enseirb-matmeca.fr \n
 *
 */
#ifndef _OTFSTRUCTS_
#define _OTFSTRUCTS_

#include <stdint.h>

/* ContainerTypes */
#define MAX_PROCESSTYPE 100

typedef struct ContainerType {
    char *name;
    char *alias;
} ContainerType_t;

/*! Containers */
#define MAX_PROCESS 1000

typedef struct Container {
    char *name;
    char *alias;
    int ctType;
} Container_t;

/*! StateTypes */
#define MAX_STATESTYPE 100

typedef struct StateType { /* Func group */
    char *name;
    char *alias;
    int   groupId;
} StateType_t;

/*! States */
#define MAX_STATES 1000
typedef struct State {
    int value;
    int cont;
    int stateType;
} State_t;

/*! EntityValue, contains the name of the functions/states */
typedef struct EntityValue {
    char *name;
    char *alias;
    int   groupId;
} EntityValue_t;

/*! Events/Markers */
#define MAX_EVENTTYPE 10
typedef struct EventType {
    char *name;
    char *alias;
    int   contType;
} EventType_t;


/*! Variables/Counters */
#define MAX_VARIABLETYPE 10
typedef struct VariableType {
    char *name;
    char *alias;
    int   contType;
} VariableType_t;

typedef struct otf_color {
    char *colorID;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} otf_color_t;

#endif
