LIB=$(LDFLAGS) -lgtg
GTG_INC = ../inc
GTG_LIB = ../src/.libs
INC= -I$(GTG_INC)
BIN=bin
CC =mpicc
LDFLAGS += -L$(GTG_LIB)
LD_LIBRARY_PATH = $(GTG_LIB)
export LD_LIBRARY_PATH
#-Wl,-rpath=/home/glaurt7/Bureau/carriere/projets_perso/GTG/gtg/trunk/src/.libs

TESTS = testState testEvent testLink testVar testAll
MPI_TESTS = testState_parall testEvent_parall testEvent_mpi

.phony	: bin obj

all	: dir $(TESTS) $(MPI_TESTS)

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

check   : all
	for i in $(TESTS) ; do \
		for type in 1 2 ; do \
			echo running $$i $$type ; \
			bin/$$i $$type ; \
		done ; \
	done

	for i in $(MPI_TESTS) ; do \
		for type in 1 2 ; do \
			echo running $$i $$type ; \
			mpirun --host localhost:6 -np 6 bin/$$i $$type ; \
		done ; \
	done

clean	:
		rm -rf $(BIN) *~
