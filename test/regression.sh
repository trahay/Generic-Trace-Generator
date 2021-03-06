#!/bin/sh

########################################
#   Script to test if the results in the traces generated are the same   #
########################################


### Recompiling the GTG library
#cd ..
#autoreconf -vfi
#./configure
#make

### Compiling the tests
#cd test
make

### Generation Of the traces
echo " "
echo " Generation of traces "
cd bin && ./testAll && ./testEvent 1 && ./testLink 1 && ./testState 1 && ./testVar 1 
cd ..

### Diff with the saved traces to make sure the traces generated are the same
errors=0
echo " "
echo " Diff of the traces "
echo " For the whole trace :"
diff bin/testAll.trace rootTraces/testAll.trace || errors=1

echo " DONE "
echo " For the events :"
diff bin/testEvent.trace rootTraces/testEvent.trace  || errors=1
echo " DONE "
echo " For the links :"
diff bin/testLink.trace rootTraces/testLink.trace  || errors=1
echo " DONE "
echo " For the states :"
diff bin/testState.trace rootTraces/testState.trace || errors=1
echo " DONE "
echo " For the counter :"
diff bin/testVar.trace rootTraces/testVar.trace || errors=1
echo " DONE "

if [ $errors -eq 0 ]; then
    echo  " Test passed :-)"
else
    echo  " Test failed :-("
fi

echo " Cleaning ... "
make clean

