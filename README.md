###  fastx-utils: tools for fasta/q manipulation

<hr>

#### 1. install

```sh
git clone https://github.com/kits-hub/fastx-utils
cd fastx-utils
make
```

#### 2、 interface

current: `version：0.0.2`


```text
Usage:   fastx-utils <command> <arguments>
Version: 0.0.2

Command:
      -- FASTA/Q Summary.
         view          extract sequence information.
         fqchk         fastq QC (base/quality summary).
         joint         link paired-reads with padding 'N'.
         convert       common transformation of FASTA/Q.
         info          calculate reads counts/base info.
         counts        calculate sequence counts.
         length        discard sequence with length region.
         avq           calculate average quality and length.

      -- FASTA/Q retrieve.
         filter        filter PE reads with Ns and Qs.
         dedup         deduplication with sequence label.
         fetch         fetch by specified identifier in command line.
         reorder       reorder sequence by name.
         subseq        extract sequence with specified identifier set.
         uniques       Find uniques sequences.

      -- FASTA/Q Edit.
         fake          convert fasta to fake fastq with qual.
         interleave    interleave two PE FASTA/Q files.
         deinterleave  deinterleave interleaved FASTA/Q file.
         rename        rename sequence identifier with specified prefix
                       and then, _1, _2, _3..., return identifier map.
         label         add a label before/after name to relabel sequence.
         truncate      truncate sequence to specified length L.
         comment       add comment information.
         replace       replace sequence name.
         strip_stop_codon strip stop codon in AA sequence.

      -- FASTA/Q segmentation.
         partition     partition fasta/q file to N files.
         shred         shred long sequence with overlapped sequence.
         split         split fasta/q file with specified size.
         pseudo        make pseudo-sequence from a fasta file.
         segment       split pseudo-genome to specified equal part.
         demux         fastq demultiplex using index.
         individ       split fasta sequence, one file per sequence.

      -- auxiliary utils.
         reorient      reorient fastq strandness using reference.
         revcomp       revcomp DNA sequence.
         rna2dna       convert RNA to DNA sequence.
         bed           convert shred location to bed.
         region-mask   mask base to N/lowcase in bed region.
         tsv2fasta     tsv convert to fasta.
         circular      identify circular contigs from SPAdes assembler
                       and the repeated k-mer found at terminal.


Licenced:
(c) 2016-2023 - LEI ZHANG
Logic Informatics Co.,Ltd.
zhanglei@logicinformatics.com
```