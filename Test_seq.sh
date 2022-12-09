#!/usr/bin/env sh

make seq
for PROCESS in $(seq 8 12)
do
    time $(
    for ((i=0; i<$1; i++))
    do
            ./pcv $PROCESS > /dev/null
    done
    ) 
done 
