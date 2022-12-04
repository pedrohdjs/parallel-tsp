.PHONY: seq, par

seq:
	gcc ./pcv-seq.c -o pcv

par:
	mpicc ./pcv-par.c -o pcv