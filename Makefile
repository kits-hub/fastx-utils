CC=		cc
CFLAGS=		-g -Wall -O2 -Wno-unused-function
CPPFLAGS=
INCLUDES=
PROG=		fastx-utils
BINDIR=		/usr/local/bin
LIBS=		-lz -lm
OBJS=		convert.o info.o counts.o joint.o length.o view.o fqchk.o demux.o filter.o     \
		dedup.o interleave.o deinterleave.o uniques.o rename.o label.o truncate.o      \
		comment.o replace.o reorder.o subseq.o fetch.o strip_stop_codon.o fake.o       \
		partition.o shred.o split.o pseudo.o segment.o individ.o reorient.o revcomp.o  \
		rna2dna.o mask.o region_mask.o bed.o avq.o tsv2fasta.o circular.o kstring.o utils.o

.SUFFIXES:.c .o
.PHONY:all clean

.c.o:
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $< -o $@

all:$(PROG)

fastx-utils:$(OBJS) fastx-utils.o
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

install:all
		install fastx-utils $(BINDIR)

clean:
		rm -fr *.o fastx-utils