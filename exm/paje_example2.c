/*
 * Small example to generate a trace in the Paje format
 * Author Kevin Coulomb
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <GTG.h>
#include <GTGPaje_Color.h>

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

/* Main function to generate a trace in the Paje format called cpaje2.trace */
int main (int argc, char** argv){
(void) argc;
(void) argv;
    double time;
    double timer;
    int    i;
    char   txt [TXTSIZE];
    char   proc[TXTSIZE];
    char   name[TXTSIZE];
    char   src [TXTSIZE];
    char   dest[TXTSIZE];
    char   key [TXTSIZE];

    /* Initialisation */
    setTraceType (PAJE);
    CHECK_RETURN (initTrace ("cpaje2", 0, GTG_FLAG_NONE));
    /* Creating types used */
    CHECK_RETURN (addContType ("CT_NET", "0", "Network"));
    CHECK_RETURN (addContType ("CT_NODE", "CT_NET", "Node"));
    CHECK_RETURN (addContType ("CT_PROC", "CT_NODE", "Proc"));
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
    CHECK_RETURN (addLinkType ("L_0", "Fanin", "CT_NET", "CT_PROC", "CT_PROC"));
    CHECK_RETURN (addLinkType ("L_1", "Bloc", "CT_NET", "CT_PROC", "CT_PROC"));
    CHECK_RETURN (addVarType ("V_Mem", "Memoire", "CT_NODE"));
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
    for (i=0;i<50;i++){
        sprintf (txt, "C_N%d", i%2);
        CHECK_RETURN (setVar (time, "V_Mem", txt, i%5));    /* Modification of the variables */
        time += 1.0;
    }

    clear (txt, TXTSIZE);
    time = 1.00000000;
    for (i=0; i<200; i++){
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
        CHECK_RETURN (setState (time, txt, proc, name));        /* State changes modifications */
        time += 0.25;
    }

    clear (txt , TXTSIZE);
    clear (proc, TXTSIZE);
    clear (name, TXTSIZE);
    time  = 1.00000000;
    timer = 1.50000000;
    for (i=0;i<30;i++){
        sprintf (name, "L_%d", i%2);
        sprintf (txt , "C_Net0");
        sprintf (src , "C_P%d", (i+2)%6);
        sprintf (dest, "C_P%d", (i+5)%6);
        sprintf (proc, "%d", i);
        sprintf (key , "%d", i);

        /* Adding some communications */
        CHECK_RETURN (startLink (time, name, txt, src, dest, proc, key));
        CHECK_RETURN (endLink (timer, name, txt, src, dest, proc, key));

        time  += 1.22000000;
        timer += 1.53000000;
    }

    clear (name, TXTSIZE);
    clear (proc, TXTSIZE);
    clear (key , TXTSIZE);
    time = 0.57000000;
    for (i=0;i<50;i++){
        sprintf (name, "E_%d", i%2);
        sprintf (proc, "C_P%d", i%6);
        sprintf (key , "%d", i%13);
        CHECK_RETURN (addEvent (time, name, proc, key));    /* Adding events */
        time += 1.000000000;
    }

    /* Ending the trace */
    CHECK_RETURN (endTrace ());
    return EXIT_SUCCESS;
}
