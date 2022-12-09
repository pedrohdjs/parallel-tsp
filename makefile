# Compilador de c
CC = gcc
# Tamanho do grafo
N = 5
# Número de processos
P = $(shell nproc)
# Lista de Hosts
HOST_LIST = -H hal02,hal03,hal04,hal05,hal06,hal07,hal08,hal09
WARNING_FLAGS = -Wextra -Wall

seq:
	$(CC) $(WARNING_FLAGS) ./pcv-seq.c -o pcv
run-seq: seq
	./pcv $(N)
par:
	mpicc $(WARNING_FLAGS) -fopenmp ./pcv-par.c -o pcv
run-par: par
	mpirun -np $(P) $(HOST_LIST)  ./pcv $(N)

.PHONY: pcv
