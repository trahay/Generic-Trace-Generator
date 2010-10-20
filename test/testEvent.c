/*
 * Small test of the events generation
 * Author Kevin Coulomb
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "GTG.h"

/* Size of the buffer used */
#define TXTSIZE 200

/* Small function to clean the buffer */
void clear (char* buf, int size){
    int i;
    for (i=0;i<size;i++)
        buf[i]='\0';
}

/* Small macro to test the return value */
#define CHECK_RETURN(val) {if (val!=TRACE_SUCCESS){fprintf (stderr, "Function failed line %d. Leaving \n", __LINE__);exit (-1);}}

/* Main function to generate a trace containing some container and only states and events. Output format is otf if argv(1)=2, paje otherwise */
int main (int argc, char** argv){
    double time;
    double timer;
    int    i;
    char   txt [TXTSIZE];
    char   proc[TXTSIZE];
    char   name[TXTSIZE];
    char   src [TXTSIZE];
    char   dest[TXTSIZE];
    char   key [TXTSIZE];
    int traceT;

    if (argc<2){
        fprintf (stderr, "Usage : ./testEvent <trace type>. \n 1 for paje, 2 for otf. \n");
        exit (-1);
    }

    switch (atoi (argv[1])){
    case 1 :
        traceT = PAJE;
        break;
    case 2 :
        traceT = OTF;
        break;
    default :
        traceT = PAJE;
        break;

    }

    /* Initialisation */
    setTraceType (traceT, BUFF);
    CHECK_RETURN (initTrace ("testEvent", 0));
    /* Creating types used */
    CHECK_RETURN (addProcType ("CT_NET", "0", "Network"));
    CHECK_RETURN (addProcType ("CT_NODE", "CT_NET", "Node"));
    CHECK_RETURN (addProcType ("CT_PROC", "CT_NODE", "Proc"));
    CHECK_RETURN (addStateType ("ST_NodeState", "CT_NODE", "Node state"));
    CHECK_RETURN (addStateType ("ST_ProcState", "CT_PROC", "Procstate"));
    CHECK_RETURN (addEntityValue ("SN_0", "ST_NodeState", "Sleep", GTG_RED));
    CHECK_RETURN (addEntityValue ("SN_1", "ST_NodeState", "WaitLocal", GTG_PINK));
    CHECK_RETURN (addEntityValue ("SN_2", "ST_NodeState", "WaitDistant", GTG_BLACK));
    CHECK_RETURN (addEntityValue ("SP_3", "ST_ProcState", "Produit", GTG_ORANGE));
    CHECK_RETURN (addEntityValue ("SP_4", "ST_ProcState", "Somme", GTG_GREEN));
    CHECK_RETURN (addEntityValue ("SP_5", "ST_ProcState", "Difference", GTG_BLUE));
    CHECK_RETURN (addEntityValue ("SP_6", "ST_ProcState", "Division", GTG_TEAL));
    CHECK_RETURN (addEntityValue ("SP_7", "ST_ProcState", "Modulo", GTG_PURPLE));
    /* Adding event types */
    CHECK_RETURN (addEventType ("E_0", "CT_PROC", "Rabbit"));
    CHECK_RETURN (addEventType ("E_1", "CT_PROC", "Chocolate"));
    /* Building containers tree */
    CHECK_RETURN (addContainer (0.00000, "C_Net0", "CT_NET", "0", "Ensemble0", "0"));
    CHECK_RETURN (addContainer (0.00000, "C_N0", "CT_NODE", "C_Net0", "Node0", "0"));
    CHECK_RETURN (addContainer (0.00000, "C_P0", "CT_PROC", "C_N0", "Proc0", "0"));
    CHECK_RETURN (addContainer (0.00000, "C_P1", "CT_PROC", "C_N0", "Proc1", "0"));
    CHECK_RETURN (addContainer (0.00000, "C_P2", "CT_PROC", "C_N0", "Proc2", "0"));
    CHECK_RETURN (addContainer (0.00000, "C_N1", "CT_NODE", "C_Net0", "Node1", "0"));
    CHECK_RETURN (addContainer (0.00000, "C_P3", "CT_PROC", "C_N1", "Proc3", "0"));
    CHECK_RETURN (addContainer (0.00000, "C_P4", "CT_PROC", "C_N1", "Proc4", "0"));
    CHECK_RETURN (addContainer (0.00000, "C_P5", "CT_PROC", "C_N1", "Proc5", "0"));


    clear (txt, TXTSIZE);
    time = 1.00000000;
    for (i=0; i<200; i++){
        clear (name, TXTSIZE);
        clear (proc, TXTSIZE);
        if (i%10 == 0){
            sprintf (txt , "ST_NodeState");
            sprintf (proc, "C_N%d", (i%20)?0:1);
            sprintf (name, "SN_%d", i%3);
        }
        else{
            sprintf (txt , "ST_ProcState");
            sprintf (proc, "C_P%d", i%6);
            sprintf (name, "SP_%d", i%5+3);
        }
        CHECK_RETURN (setState (time, txt, proc, name));  
        clear (name, TXTSIZE);
        clear (proc, TXTSIZE);
        if (i%5){
            sprintf (name, "E_%d", i%2);
            sprintf (proc, "C_P%d", i%6);
            sprintf (key , "%d", i%13);
            CHECK_RETURN (addEvent (time+0.5, name, proc, key));    /* Adding events */
        }

        time += 0.25;
    }

    /* Ending the trace */
    CHECK_RETURN (endTrace ());
    return EXIT_SUCCESS;
}
