#!/usr/bin/env sh

for PROCESS in $(seq 6 10)
do
    make par
    (time (
    for ((i=0; i<20; i++))
    do
            mpirun -np 4 -H hal02,hal03,hal04,hal05 ./pcv $PROCESS > /dev/null
    done
    )) > "data/par-4-$PROCESS.txt"
    (time (
    for ((i=0; i<20; i++))
    do
            mpirun -np 6 -H hal02,hal03,hal04,hal05,hal06,hal07 ./pcv $PROCESS > /dev/null
    done
    )) > "data/par-6-$PROCESS.txt"
    (time (
    for ((i=0; i<20; i++))
    do
            mpirun -np 8 -H hal02,hal03,hal04,hal05,hal06,hal07,hal08,hal09 ./pcv $PROCESS > /dev/null
    done
    )) > "data/par-8-$PROCESS.txt"
done