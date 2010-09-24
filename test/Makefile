LIB=../src/.libs/libgtg.a 
INC=
BIN=bin
CC =mpicc


.phony	: bin obj

all	: dir testState testEvent testLink testVar testAll testState_parall

dir	:
		mkdir -p $(BIN)


testAll		: testAll.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(INC) $(LIB)

testState	: testState.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(INC) $(LIB)

testState_parall	: testState_parall.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(INC) $(LIB)

testEvent	: testEvent.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(INC) $(LIB)

testLink	: testLink.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(INC) $(LIB)

testVar		: testVar.c
		$(CC) $^ -o $(BIN)/$@ $(CFLAGS) $(INC) $(LIB)

clean	:
		rm -rf $(BIN) *~