#!/bin/sh

##
## This file is part of the Score-P software (http://www.score-p.org)
##
## Copyright (c) 2009-2011,
##    RWTH Aachen University, Germany
##    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
##    Technische Universitaet Dresden, Germany
##    University of Oregon, Eugene, USA
##    Forschungszentrum Juelich GmbH, Germany
##    German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
##    Technische Universitaet Muenchen, Germany
##
## See the COPYING file in the package base directory for details.
##

## file       test/OTF2_Event_Seek_test/run_otf2_event_seek_test.sh
## maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>

cleanup()
{
    rm -rf OTF2_Event_Seek_trace
}
trap cleanup EXIT

cleanup
./OTF2_Event_Seek_test
ret=$?
#if test $ret != 0; then
#    ./otf2-print -A -O OTF2_Event_Seek_trace/TestTrace.otf2
#fi

exit $ret
