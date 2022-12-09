#!/usr/bin/env sh

make par
for PROCESS in $(seq 8 12)
do
    time $(
    for ((i=0; i<$1; i++))
    do
            mpirun -np $2 --hostfile $3 ./pcv $PROCESS > /dev/null
    done
    )
done
