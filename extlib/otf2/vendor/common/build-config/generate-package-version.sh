#!/bin/sh

res=`cat $1`
if [ $# -gt 1 ] ; then
    IFS=. read major minor bugfix < $1
    eval $2
else
    echo -n $res
fi