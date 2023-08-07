LIB=$(LDFLAGS) -lgtg
INC= -I../install/include/
BIN=bin
CC =mpicc
LDFLAGS += -L../install/lib
#-Wl,-rpath=/home/glaurt7/Bureau/carriere/projets_perso/GTG/gtg/trunk/src/.libs

.phony	: bin obj

all	: dir testState testEvent testLink testVar testAll testState_parall testEvent_parall testEvent_mpi

dir	:
		mkdir -p $(BIN)

testAll		: testAll.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(CPPFLAGS) $(INC) $(LIB)

testState	: testState.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(CPPFLAGS) $(INC) $(LIB)

testState_parall	: testState_parall.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(CPPFLAGS) $(INC) $(LIB)

testEvent_parall	: testEvent_parall.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(CPPFLAGS) $(INC) $(LIB)

testEvent_mpi	: testEvent_mpi.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(CPPFLAGS) $(INC) $(LIB)

testEvent	: testEvent.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(CPPFLAGS) $(INC) $(LIB)

testLink	: testLink.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(CPPFLAGS) $(INC) $(LIB)

testVar		: testVar.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(CPPFLAGS) $(INC) $(LIB)

clean	:
		rm -rf $(BIN) *~
