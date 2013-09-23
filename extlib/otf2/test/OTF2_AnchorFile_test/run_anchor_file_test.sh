#!/bin/sh

##
## This file is part of the Score-P software (http://www.score-p.org)
##
## Copyright (c) 2009-2012,
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

## file       test/OTF2_AnchorFile_test/run_anchor_file_test.sh
## maintainer Bert Wesarg <bert.wesarg@tu-dresden.de>

set -e

cleanup()
{
    rm -rf OTF2_AnchorFile_trace
}
trap cleanup EXIT

cleanup
echo "No directories at all:"
./OTF2_AnchorFile_test

cleanup
echo "One archive path directory:"
mkdir -p OTF2_AnchorFile_trace
./OTF2_AnchorFile_test

cleanup
echo "All archive path directories:"
mkdir -p OTF2_AnchorFile_trace/trace
./OTF2_AnchorFile_test

cleanup
echo "Archive trace directory exists (must fail):"
mkdir -p OTF2_AnchorFile_trace/trace/archive
! ./OTF2_AnchorFile_test
