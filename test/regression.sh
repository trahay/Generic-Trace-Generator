#!/bin/sh

##########################################################################
#   Script to test if the results in the traces generated are the same   #
##########################################################################


### Recompiling the GTG library
cd ..
autoreconf -vfi
./configure
make

### Compiling the tests
cd test
make

### Generation Of the traces
echo " "
echo " Genration of traces "
cd bin && ./testAll && ./testEvent 1 && ./testLink 1 && ./testState 1 && ./testVar 1 
cd ..

### Diff with the saved traces to make sure the traces generated are the same

echo " "
echo " Diff of the traces "
echo " For the whole trace :"
diff bin/testAll.trace rootTraces/testAll.trace 
echo " DONE "
echo " For the events :"
diff bin/testEvent.trace rootTraces/testEvent.trace
echo " DONE "
echo " For the links :"
diff bin/testLink.trace rootTraces/testLink.trace
echo " DONE "
echo " For the states :"
diff bin/testState.trace rootTraces/testState.trace
echo " DONE "
echo " For the counter :"
diff bin/testVar.trace rootTraces/testVar.trace
echo " DONE "

make clean

