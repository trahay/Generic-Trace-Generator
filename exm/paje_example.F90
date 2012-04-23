!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! Example to use the API in Fortran in the PAJE format
!!


program main

!! To manage this C include
!! Call the file .F90 (and not .f90) or add a preprocessor flag  (-cpp for ifort for example)
#include <GTGPaje_Color.h>
  use gtg_f
  implicit none
  real (8) :: time
  real (8) :: val
  integer :: ierr
  integer :: buf = 0

!! Init  
   
  call setTraceType_f (PAJE, buf) 
  call initTrace_f ("fpaje", ierr) 
!! Creating proc types
  call addContType_f ("CT_NET", "0", "Network", ierr) 
  call addContType_f ("CT_NODE", "CT_NET", "Node", ierr) 
  call addContType_f ("CT_PROC", "CT_NODE", "Proc", ierr) 
!! Creating state types
  call addStateType_f ("ST_NodeState", "CT_NODE", "Node state", ierr) 
  call addStateType_f ("ST_ProcState", "CT_PROC", "Procstate", ierr) 
!! Creating Entity value types
  call addEntityValue_f ("SN_0", "ST_NodeState", "Sleep", P_RED, ierr) 
  call addEntityValue_f ("SN_1", "ST_NodeState", "WaitLocal",&
      &                  P_BLUE, ierr) 
  call addEntityValue_f ("SN_2", "ST_NodeState", "WaitDistant", &
      &                  P_GREEN, ierr) 
  call addEntityValue_f ("SP_3", "ST_ProcState", "Produit", P_DARKPINK,&
      &                  ierr) 
  call addEntityValue_f ("SP_4", "ST_ProcState", "Somme", P_BROWN, ierr) 
  call addEntityValue_f ("SP_5", "ST_ProcState", "Difference",&
      &                  P_ORANGE, ierr) 
!! Creating link types
  call addLinkType_f ("L_0", "Fanin", "CT_NET", "CT_PROC", "CT_PROC", &
      &               ierr) 
  call addLinkType_f ("L_1", "Bloc", "CT_NET", "CT_PROC", "CT_PROC", &
      &               ierr) 
!! Creating var type
  call addVarType_f ("V_Mem", "Memoire", "CT_NODE", ierr) 
!! Creating event types
  call addEventType_f ("E_0", "CT_PROC", "Lapin", ierr) 
  call addEventType_f ("E_1", "CT_PROC", "Chocolat", ierr) 
  time = 0.000000000000
!! Adding containers
  call addContainer_f (time, "C_Net0", "CT_NET", "0", "Ensemble0", "0",&
      &                ierr) 
  call addContainer_f (time, "C_N0", "CT_NODE", "C_Net0", "Node0", "0",&
      &                ierr) 
  call addContainer_f (time, "C_P0", "CT_PROC", "C_N0", "Proc0", "0", &
      &                ierr)
  call addContainer_f (time, "C_P1", "CT_PROC", "C_N0", "Proc1", "0", &
      &                ierr)
  call addContainer_f (time, "C_P2", "CT_PROC", "C_N0", "Proc2", "0", &
      &                ierr)
  call addContainer_f (time, "C_N1", "CT_NODE", "C_Net0", "Node1", "0",&
      &                ierr) 
  call addContainer_f (time, "C_P3", "CT_PROC", "C_N1", "Proc3", "0", &
      &                ierr) 
  call addContainer_f (time, "C_P4", "CT_PROC", "C_N1", "Proc4", "0", &
      &                ierr) 
  call addContainer_f (time, "C_P5", "CT_PROC", "C_N1", "Proc5", "0", &
      &                ierr) 

!! Setting var, events states, links on proc/nodes
  time = 0.0111742
  val = 1.0
  call setVar_f (time, "V_Mem", "C_N0", val, ierr) 
  time = 0.0211
  call setState_f (time, "ST_NodeState", "C_N1", "SN_1", ierr) 
  time = 0.0212742
  call setState_f (time, "ST_NodeState", "C_N0", "SN_0", ierr) 
  time = 0.0214742
  val = 2.0
  call setVar_f (time, "V_Mem", "C_N0", val, ierr) 
  time = 0.0311742
  call setState_f (time, "ST_NodeState", "C_N1", "SN_2", ierr) 
  time = 0.0411742
  call setState_f (time, "ST_NodeState", "C_N0", "SN_1", ierr) 
  time = 0.0311742
  val = 3.0
  call setVar_f (time, "V_Mem", "C_N0", val, ierr) 
  time = 0.0411742
  val = 1.0
  call setVar_f (time, "V_Mem", "C_N1", val, ierr) 
  time = 0.0511742
  call setState_f (time, "ST_NodeState", "C_N1", "SN_0", ierr) 
  time = 0.0511742
  val = 2.0
  call setVar_f (time, "V_Mem", "C_N1", val, ierr) 
  time = 0.0611742
  call setState_f (time, "ST_NodeState", "C_N0", "SN_2", ierr) 
  time = 0.0611742
  val = 3.0
  call setVar_f (time, "V_Mem", "C_N1", val, ierr) 
  time = 0.0151742
  call setState_f (time, "ST_ProcState", "C_P0", "SP_3", ierr) 
  time = 0.0161742
  call setState_f (time, "ST_ProcState", "C_P1", "SP_3", ierr) 
  time = 0.0161742
  call setState_f (time, "ST_ProcState", "C_P0", "SP_5", ierr) 
  time = 0.0171742
  call setState_f (time, "ST_ProcState", "C_P3", "SP_5", ierr) 
  time = 0.0171742
  call setState_f (time, "ST_ProcState", "C_P4", "SP_3", ierr) 
  time = 0.0181742
  call setState_f (time, "ST_ProcState", "C_P5", "SP_4", ierr) 
  time = 0.0181742
  call setState_f (time, "ST_ProcState", "C_P2", "SP_3", ierr) 
  time = 0.0191742
  call setState_f (time, "ST_ProcState", "C_P3", "SP_4", ierr) 
  time = 0.0191742
  call setState_f (time, "ST_ProcState", "C_P1", "SP_4", ierr) 
  time = 0.0111742
  call setState_f (time, "ST_ProcState", "C_P0", "SP_3", ierr) 
  time = 0.0111742
  call setState_f (time, "ST_ProcState", "C_P0", "SP_3", ierr) 
  time = 0.0211742
  call setState_f (time, "ST_ProcState", "C_P5", "SP_4", ierr) 
  time = 0.0211742
  call setState_f (time, "ST_ProcState", "C_P4", "SP_5", ierr) 
  time = 0.0311742
  call setState_f (time, "ST_ProcState", "C_P3", "SP_5", ierr) 
  time = 0.0311742
  call setState_f (time, "ST_ProcState", "C_P2", "SP_5", ierr) 
  time = 0.0411742
  call setState_f (time, "ST_ProcState", "C_P0", "SP_3", ierr) 
  time = 0.0411742
  call setState_f (time, "ST_ProcState", "C_P1", "SP_3", ierr) 
  time = 0.0511742
  call setState_f (time, "ST_ProcState", "C_P1", "SP_4", ierr) 
  time = 0.0511742
  call setState_f (time, "ST_ProcState", "C_P2", "SP_4", ierr) 
  time = 0.0611742
  call setState_f (time, "ST_ProcState", "C_P0", "SP_3", ierr) 
  time = 0.0611742
  call setState_f (time, "ST_ProcState", "C_P3", "SP_3", ierr) 
  time = 0.0711742
  call setState_f (time, "ST_ProcState", "C_P0", "SP_4", ierr) 
  time = 0.0711742
  call setState_f (time, "ST_ProcState", "C_P5", "SP_5", ierr) 
  time = 0.0811742
  call setState_f (time, "ST_ProcState", "C_P4", "SP_5", ierr) 
  time = 0.0811742
  call setState_f (time, "ST_ProcState", "C_P2", "SP_4", ierr) 
  time = 0.0911742
  call setState_f (time, "ST_ProcState", "C_P0", "SP_3", ierr) 
  time = 0.02
  call startLink_f (time, "L_0", "C_Net0", "C_P2", "C_P4", "12", "0", &
      &             ierr) 
  time = 0.03
  call startLink_f (time, "L_1", "C_Net0", "C_P4", "C_P2", "13", "1", &
      &             ierr) 
  time = 0.04
  call startLink_f (time, "L_0", "C_Net0", "C_P0", "C_P5", "14", "2", &
      &             ierr) 
  time = 0.05
  call startLink_f (time, "L_1", "C_Net0", "C_P3", "C_P1", "15", "3", &
      &             ierr) 
  time = 0.03
  call endLink_f (time, "L_0", "C_Net0", "C_P2", "C_P4", "12", "0", &
      &           ierr) 
  time = 0.04
  call endLink_f (time, "L_1", "C_Net0", "C_P4", "C_P2", "13", "1", &
      &           ierr) 
  time = 0.05
  call endLink_f (time, "L_0", "C_Net0", "C_P0", "C_P5", "14", "2", &
      &           ierr) 
  time = 0.06
  call endLink_f (time, "L_1", "C_Net0", "C_P3", "C_P1", "15", "3", &
      &           ierr) 
  time = 0.02
  call addEvent_f (time, "E_0", "C_P0", "11", ierr) 
  time = 0.05
  call addEvent_f (time, "E_0", "C_P2", "10", ierr) 
  time = 0.07
  call addEvent_f (time, "E_1", "C_P1", "1", ierr) 
  time = 0.08
  call addEvent_f (time, "E_1", "C_P5", "3", ierr) 

!! Cleaning
  call endTrace_f (ierr) 
  
end program main

  
