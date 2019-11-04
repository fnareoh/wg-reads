CPP        = g++

SOURCES    := ${wildcard *.cpp}

.PHONY:    all clean

all:       main

main:      ref reads

prepare_ref:
	./faext.py $(ref_fasta) $(ref) -Uv -s 24 -n 1 --before="" --after=""

ref:
	$(info ************  Reference only  ************)
	./bwt.o $(ref)

reads:
	$(info ************  Reference and reads  ************)
	python format_reads.py $(reads) > reads.in
	cat $(ref) reads.in > ref_and_reads.in
	$(CPP) bwt.cpp -o bwt.o
	./bwt.o ref_and_reads.in


clean:
	-rm bwt.o reads.in ref_and_reads.in
