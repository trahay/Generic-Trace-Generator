#!/bin/sh

res=`cat $1`
if [ $# -gt 1 ] ; then
    IFS=. read current revision age < $1
    eval $2
else
    echo -n $res
fi
