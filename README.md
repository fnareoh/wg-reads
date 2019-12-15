# wg-reads

The file `bwt.cpp` computes very nively the XBWT for a set of read and it's
reference genome. It creates an object of every suffix of the reference and the
reads and computes the suffix array by just sorting them according to a custom
order that will look into the reference genome for context if you arrive at the
end of a read suffix.

The input format is the reference genome on the first line and then on all
following line, the position of the read in the genome followed by the read.

## Usage

Both `bwt.cpp` and `bwt.py` contain a valid implementation but the python one
was build as a prototype and is expected to be much slower.

```
g++ bwt.cpp
./a.out data/example.in
python bwt.py data/example.in
```

`bwt.cpp` does not output the BWT constructed only the number of runs in it.
During the computation it outputs a progression of the computation based on the
number of comparison made so far.

If you need to create a file to the input format we used from a set of read to
the bam format, you can use the `format_reads.py` script (require the `pysam`
python2 module).

```
python2 format_reads.py reads.bam > reads.in
```

Be carefull to be a valid input the references still has to be added on the
first line.

