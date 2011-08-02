#include <stdio.h>
#include <stdlib.h>
#include <GTG.h>

int main (int argc, char** argv){
    int rk;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rk);

    setTraceType (VITE, BUFF);
    setComm (MPI_COMM_WORLD);
    initTrace ("cvite");
    if (rk==0){
        addContType ("CT_NET", "0", "Network");
        addContType ("CT_NODE", "CT_NET", "Node");
        addContType ("CT_PROC", "CT_NODE", "Proc");
        addStateType ("ST_NodeState", "CT_NODE", "Node state");
        addStateType ("ST_ProcState", "CT_PROC", "Procstate");
        addEntityValue ("SN_0", "ST_NodeState", "Sleep", "0.5 0.5 0.5");
        addEntityValue ("SN_1", "ST_NodeState", "WaitLocal", "0.0 0.5 0.9");
        addEntityValue ("SN_2", "ST_NodeState", "WaitDistant", "0.9 0.5 0.0");
        addEntityValue ("SP_3", "ST_ProcState", "Produit", "0.5 0.9 0.0");
        addEntityValue ("SP_4", "ST_ProcState", "Somme", "0.9 0.9 0.0");
        addEntityValue ("SP_5", "ST_ProcState", "Difference", "0.1 0.5 0.4");
        addLinkType ("L_0", "Fanin", "CT_NET", "CT_PROC", "CT_PROC");
        addLinkType ("L_1", "Bloc", "CT_NET", "CT_PROC", "CT_PROC");
        addVarType ("V_Mem", "Memoire", "CT_NODE");
        addEventType ("E_0", "CT_PROC", "Lapin");
        addEventType ("E_1", "CT_PROC", "Chocolat");
    
        addContainer (0.00000, "C_Net0", "CT_NET", "0", "Ensemble0", "0");
        addContainer (0.00000, "C_N0", "CT_NODE", "C_Net0", "Node0", "0");
        addContainer (0.00000, "C_N1", "CT_NODE", "C_Net0", "Node1", "0");
        addContainer (0.00000, "C_P0", "CT_PROC", "C_N0", "Proc0", "cvite_proc0");
        addContainer (0.00000, "C_P1", "CT_PROC", "C_N0", "Proc1", "cvite_proc1");
        addContainer (0.00000, "C_P2", "CT_PROC", "C_N0", "Proc2", "cvite_proc2");

        addContainer (0.00000, "C_P3", "CT_PROC", "C_N1", "Proc3", "cvite_proc3");
        addContainer (0.00000, "C_P4", "CT_PROC", "C_N1", "Proc4", "cvite_proc4");
        addContainer (0.00000, "C_P5", "CT_PROC", "C_N1", "Proc5", "cvite_proc5");
    }
    setVar (0.0111742, "V_Mem", "C_N0", 1.0000000);
    setState (0.01120000, "ST_NodeState", "C_N1", "SN_1");
    setState (0.02121000, "ST_NodeState", "C_N0", "SN_0");
    setVar (0.0212742, "V_Mem", "C_N0", 2.0000000);
    setState (0.03122000, "ST_NodeState", "C_N1", "SN_2");
    setState (0.04123000, "ST_NodeState", "C_N0", "SN_1");
    setVar (0.0313742, "V_Mem", "C_N0", 3.0000000);
    setVar (0.0414742, "V_Mem", "C_N1", 1.0000000);
    setState (0.05124000, "ST_NodeState", "C_N1", "SN_0");
    setVar (0.0515742, "V_Mem", "C_N1", 2.0000000);
    setState (0.06124500, "ST_NodeState", "C_N0", "SN_2");
    setVar (0.0616742, "V_Mem", "C_N1", 3.0000000);
    switch (rk){
    case 0:
    setState (0.00130000, "ST_ProcState", "C_P0", "SP_3");
    setState (0.00150000, "ST_ProcState", "C_P0", "SP_5");
    setState (0.00230000, "ST_ProcState", "C_P0", "SP_3");
    setState (0.00330000, "ST_ProcState", "C_P0", "SP_3");
    setState (0.00830000, "ST_ProcState", "C_P0", "SP_3");
    setState (0.03130000, "ST_ProcState", "C_P0", "SP_3");
    setState (0.05130000, "ST_ProcState", "C_P0", "SP_4");
    setState (0.09130000, "ST_ProcState", "C_P0", "SP_3");
    startLink (0.040000, "L_0", "C_Net0", "C_P0", "C_P5", "14", "2");
    endLink (0.050000, "L_0", "C_Net0", "C_P0", "C_P5", "14", "2");
    addEvent (0.030000, "E_0", "C_P0", "11");
    break;
    case 1 :
    setState (0.00140000, "ST_ProcState", "C_P1", "SP_3");
    setState (0.00210000, "ST_ProcState", "C_P1", "SP_4");
    setState (0.00930000, "ST_ProcState", "C_P1", "SP_3");
    setState (0.01030000, "ST_ProcState", "C_P1", "SP_4");
    addEvent (0.070000, "E_1", "C_P1", "1");
    break;
    case 3 :
    setState (0.00160000, "ST_ProcState", "C_P3", "SP_5");
    setState (0.00200000, "ST_ProcState", "C_P3", "SP_4");
    setState (0.00630000, "ST_ProcState", "C_P3", "SP_5");
    setState (0.04130000, "ST_ProcState", "C_P3", "SP_3");
    startLink (0.050000, "L_1", "C_Net0", "C_P3", "C_P1", "15", "3");
    endLink (0.060000, "L_1", "C_Net0", "C_P3", "C_P1", "15", "3");
    break;
    case 4 :
    setState (0.00170000, "ST_ProcState", "C_P4", "SP_3");
    setState (0.00530000, "ST_ProcState", "C_P4", "SP_5");
    setState (0.07130000, "ST_ProcState", "C_P4", "SP_5");
    startLink (0.030000, "L_1", "C_Net0", "C_P4", "C_P2", "13", "1");
    endLink (0.030000, "L_0", "C_Net0", "C_P2", "C_P4", "12", "0");
    break;
    case 5 :
    setState (0.00180000, "ST_ProcState", "C_P5", "SP_4");
    setState (0.00430000, "ST_ProcState", "C_P5", "SP_4");
    setState (0.06130000, "ST_ProcState", "C_P5", "SP_5");
    addEvent (0.090000, "E_1", "C_P5", "3");
    break;
    case 2 :
    setState (0.00190000, "ST_ProcState", "C_P2", "SP_3");
    setState (0.00730000, "ST_ProcState", "C_P2", "SP_5");
    setState (0.02130000, "ST_ProcState", "C_P2", "SP_4");
    setState (0.08130000, "ST_ProcState", "C_P2", "SP_4");
    startLink (0.020000, "L_0", "C_Net0", "C_P2", "C_P4", "12", "0");
    endLink (0.040000, "L_1", "C_Net0", "C_P4", "C_P2", "13", "1");
    addEvent (0.050000, "E_0", "C_P2", "10");
    break;

    default :
        break;
    }
    endTrace ();
    MPI_Finalize ();
    return EXIT_SUCCESS;
}


