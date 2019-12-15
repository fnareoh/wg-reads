#!/usr/bin/env python3
import sys, pysam

def show(samfile):
  for r in samfile.fetch(until_eof=True):
    print(r.get_reference_positions()[0]),
    print(r.query_alignment_sequence)


def main():
  if len(sys.argv) !=2:
    print("Usage: %s reads_bamfile " % sys.argv[0],)
    return 1
  fastafile = sys.argv[1]
  samfile = pysam.AlignmentFile(sys.argv[1], "rb")
  show(samfile)
  samfile.close()


if __name__ == '__main__':
  main()

