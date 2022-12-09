#!/usr/bin/env sh

for PROCESS in $(seq 6 10)
do
    make seq
    (time (
    for ((i=0; i<20; i++))
    do
            ./pcv $PROCESS > /dev/null
    done
    )) > "data/seq-$PROCESS.txt"
done 