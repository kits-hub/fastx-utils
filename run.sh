cc -c -g -Wall -O2 -Wc++-compat  kstring.c   -o  kstring.o

cc -c -g  -O2 -Wc++-compat  info.c   -o  info.o
cc -c -g -Wall -O2 -Wc++-compat  view.c   -o  view.o
cc -c -g  -O2 -Wc++-compat  length.c   -o  length.o
cc -c -g  -O2 -Wc++-compat  counts.c   -o  counts.o
cc -c -g  -O2 -Wc++-compat  fqchk.c   -o  fqchk.o
cc -c -g  -O2 -Wc++-compat  demultiplex.c   -o  demultiplex.o
cc -c -g  -O2 -Wc++-compat  filter.c   -o  filter.o
cc -c -g  -O2 -Wc++-compat  dedup.c    -o  dedup.o
cc -c -g  -O2 -Wc++-compat  joint.c   -o  joint.o

cc -c -g -Wall -O2 -Wc++-compat  interleave.c -o  interleave.o
cc -c -g -Wall -O2 -Wc++-compat  deinterleave.c -o  deinterleave.o
cc -c -g -Wall -O2 -Wc++-compat  uniques.c -o  uniques.o

cc -c -g -O2 -Wc++-compat  rename.c   -o  rename.o
cc -c -g -Wall -O2 -Wc++-compat  label.c       -o  label.o
cc -c -g -Wall -O2 -Wc++-compat  truncate.c    -o  truncate.o
cc -c -g -Wall -O2 -Wc++-compat  strip_stop_codon.c    -o  strip_stop_codon.o

cc -c -g -Wall -O2 -Wc++-compat  replace.c   -o  replace.o
cc -c -g -Wall -O2 -Wc++-compat  reorder.c   -o  reorder.o

cc -c -g -Wall -O2 -Wc++-compat  subseq.c    -o subseq.o
cc -c -g -Wall -O2 -Wc++-compat  comment.c   -o comment.o
cc -c -g -Wall -O2 -Wc++-compat  fetch.c     -o fetch.o

cc -c -g -Wall -O2 -Wc++-compat  partition.c -o  partition.o
cc -c -g -Wall -O2 -Wc++-compat  shred.c     -o  shred.o
cc -c -g -Wall -O2 -Wc++-compat  split.c     -o  split.o
cc -c -g -Wall -O2 -Wc++-compat  pseudo.c    -o  pseudo.o
cc -c -g -Wall -O2 -Wc++-compat  reorient.c    -o  reorient.o
cc -c -g -O2 -Wc++-compat  revcomp.c     -o revcomp.o
cc -c -g -O2 -Wc++-compat  utils.c       -o  utils.o
cc -c -g -O2 -Wc++-compat  convert.c     -o  convert.o
cc -c -g -O2 -Wc++-compat  rna2dna.c     -o rna2dna.o
cc -c -g -O2 -Wc++-compat  mask.c        -o mask.o
cc -c -g -O2 -Wc++-compat  region_mask.c -o region_mask.o
cc -c -g -O2 -Wc++-compat  fake.c        -o fake.o
cc -c -g -O2 -Wc++-compat  bed.c         -o bed.o
cc -c -g -O2 -Wc++-compat  segment.c     -o segment.o
cc -c -g -O2 -Wc++-compat  avq.c         -o avq.o
cc -c -g -O2 -Wc++-compat  tsv2fasta.c   -o tsv2fasta.o
cc -c -g -O2 -Wc++-compat  circular.c   -o circular.o

cc -c -g -Wall -O2 -Wc++-compat  fastx-utils.c -o  fastx-utils.o
cc  -g -Wall -O2 -Wc++-compat  *.o -o  fastx-utils -lz -lm