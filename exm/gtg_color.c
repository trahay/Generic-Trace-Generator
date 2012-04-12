#include <stdio.h>
#include <stdlib.h>
#include <GTG.h>



int main (int argc, char** argv){
(void) argc;
(void) argv;
    setTraceType (PAJE);
    initTrace ("gtg_color", 0, GTG_FLAG_NONE);
    addContType ("CT_NET", "0", "Network");
    addContType ("CT_NODE", "CT_NET", "Node");
    addContType ("CT_PROC", "CT_NODE", "Proc");
    addStateType ("ST_NodeState", "CT_NODE", "Node state");
    addStateType ("ST_ProcState", "CT_PROC", "Procstate");

    addEntityValue ("SN_0", "ST_NodeState", "black", GTG_BLACK);
    addEntityValue ("SN_1", "ST_NodeState", "red", GTG_RED);
    addEntityValue ("SN_2", "ST_NodeState", "green", GTG_GREEN);
    addEntityValue ("SN_3", "ST_NodeState", "blue", GTG_BLUE);
    addEntityValue ("SN_4", "ST_NodeState", "white", GTG_WHITE);
    addEntityValue ("SN_5", "ST_NodeState", "teal", GTG_TEAL);
    addEntityValue ("SN_6", "ST_NodeState", "darkgrey", GTG_DARKGREY);
    addEntityValue ("SN_7", "ST_NodeState", "yellow", GTG_YELLOW);
    addEntityValue ("SN_8", "ST_NodeState", "purple", GTG_PURPLE);
    addEntityValue ("SN_9", "ST_NodeState", "lightbrown", GTG_LIGHTBROWN);
    addEntityValue ("SN_10", "ST_NodeState", "lightgrey", GTG_LIGHTGREY);
    addEntityValue ("SN_11", "ST_NodeState", "darkblue", GTG_DARKBLUE);
    addEntityValue ("SN_12", "ST_NodeState", "pink", GTG_PINK);
    addEntityValue ("SN_13", "ST_NodeState", "darkpink", GTG_DARKPINK);
    addEntityValue ("SN_14", "ST_NodeState", "seablue", GTG_SEABLUE);
    addEntityValue ("SN_15", "ST_NodeState", "kaki", GTG_KAKI);
    addEntityValue ("SN_16", "ST_NodeState", "redblood", GTG_REDBLOOD);
    addEntityValue ("SN_17", "ST_NodeState", "brown", GTG_BROWN);
    addEntityValue ("SN_18", "ST_NodeState", "grenat", GTG_GRENAT);
    addEntityValue ("SN_19", "ST_NodeState", "orange", GTG_ORANGE);
    addEntityValue ("SN_20", "ST_NodeState", "mauve", GTG_MAUVE);
    addEntityValue ("SN_21", "ST_NodeState", "lightpink", GTG_LIGHTPINK);

    addContainer (0.00000, "C_Net0", "CT_NET", "0", "Ensemble0", "0");
    addContainer (0.00000, "C_N0", "CT_NODE", "C_Net0", "Node0", "0");


    addContainer (0.00000, "C_P0", "CT_PROC", "C_N0", "Proc0", "0");

    int i;
    for(i=0; i<21;i++) {
	    char* nodeState = NULL;
	    asprintf(&nodeState, "SN_%d", i);
	    setState ((float)i, "ST_NodeState", "C_P0", nodeState);
	    free(nodeState);
    }

    endTrace ();
    return EXIT_SUCCESS;
}
