/*****************************************************************************
  fastx-utils.c

  fastx-utils command line interface.  

  (c) 2016-2018 - LEI ZHANG
  Logic Informatics Co.,Ltd.
  zhanglei@logicinformatics.com
  
  Licenced under The MIT License.
******************************************************************************/

#include <stdio.h>
#include <string.h>

int convert_main(int argc, char *argv[]);
int info_main(int argc, char *argv[]);
int counts_main(int argc, char *argv[]);
int joint_main(int argc, char *argv[]);
int length_main(int argc, char *argv[]);
int view_main(int argc, char *argv[]);
int fqchk_main(int argc, char *argv[]);
int demux_main(int argc, char *argv[]);
int filter_main(int argc, char *argv[]);
int dedup_main(int argc, char *argv[]);

int interleave_main(int argc, char *argv[]);
int deinterleave_main(int argc, char *argv[]);
int uniques_main(int argc, char *argv[]);
int rename_main(int argc, char *argv[]);
int label_main(int argc, char *argv[]);
int truncate_main(int argc, char *argv[]);
int comment_main(int argc, char *argv[]);
int replace_main(int argc, char *argv[]);
int reorder_main(int argc, char *argv[]);
int subseq_main(int argc, char *argv[]);
int fetch_main(int argc, char *argv[]);
int strip_stop_codon_main(int argc, char *argv[]);
int fake_main(int argc, char *argv[]);

int partition_main(int argc, char *argv[]);
int shred_main(int argc, char *argv[]);
int split_main(int argc, char *argv[]);
int pseudo_main(int argc, char *argv[]);
int segment_main(int argc, char *argv[]);
int individ_main(int argc, char *argv[]);

int reorient_main(int argc, char *argv[]);
int revcomp_main(int argc, char *argv[]);
int rna2dna_main(int argc, char *argv[]);
int mask_main(int argc, char *argv[]);
int region_mask_main(int argc, char *argv[]);
int bed_main(int argc, char *argv[]);
int avq_main(int argc, char *argv[]);
int tsv2fasta_main(int argc, char *argv[]);
int circular_main(int argc, char *argv[]);

static int usage(){
    
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage:   fastx-utils <command> <arguments>\n");
    fprintf(stderr, "Version: 0.0.1-r2\n\n");
    fprintf(stderr, "Command:\n");
    fprintf(stderr, "      -- FASTA/Q Summary.\n");
    fprintf(stderr, "         view          extract sequence information.\n");
    fprintf(stderr, "         fqchk         fastq QC (base/quality summary).\n");
    fprintf(stderr, "         joint         link paired-reads with padding 'N'.\n");
    fprintf(stderr, "         convert       common transformation of FASTA/Q.\n");
    fprintf(stderr, "         info          calculate reads counts/base info.\n");
    fprintf(stderr, "         counts        calculate sequence counts.\n");
    fprintf(stderr, "         length        discard sequence with length region.\n");
    fprintf(stderr, "         avq           calculate average quality and length.\n");
    fprintf(stderr, "\n");

    fprintf(stderr, "      -- FASTA/Q retrieve.\n");
    fprintf(stderr, "         filter        filter PE reads with Ns and Qs. \n");
    fprintf(stderr, "         dedup         deduplication with sequence label. \n");
    fprintf(stderr, "         fetch         fetch by specified identifier in command line.\n");
    fprintf(stderr, "         reorder       reorder sequence by name.\n");
    fprintf(stderr, "         subseq        extract sequence with specified identifier set.\n");
    fprintf(stderr, "         uniques       Find uniques sequences.\n");
    fprintf(stderr, "\n");

    fprintf(stderr, "      -- FASTA/Q Edit.\n");
    fprintf(stderr, "         fake          convert fasta to fake fastq with qual.\n");
    fprintf(stderr, "         interleave    interleave two PE FASTA/Q files.\n");
    fprintf(stderr, "         deinterleave  deinterleave interleaved FASTA/Q file.\n");
    fprintf(stderr, "         rename        rename sequence identifier with specified prefix\n");
    fprintf(stderr, "                       and then, _1, _2, _3..., return identifier map.\n");
    fprintf(stderr, "         label         add a label before/after name to relabel sequence.\n");
    fprintf(stderr, "         truncate      truncate sequence to specified length L.\n");
    fprintf(stderr, "         comment       add comment information.\n");
    fprintf(stderr, "         replace       replace sequence name.\n");
    fprintf(stderr, "         strip_stop_codon strip stop codon in AA sequence.\n");

    fprintf(stderr, "\n");
    fprintf(stderr, "      -- FASTA/Q segmentation.\n");
    fprintf(stderr, "         partition     partition fasta/q file to N files.\n");
    fprintf(stderr, "         shred         shred long sequence with overlapped sequence.\n");
    fprintf(stderr, "         split         split fasta/q file with specified size.\n");
    fprintf(stderr, "         pseudo        make pseudo-sequence from a fasta file.\n");
    fprintf(stderr, "         segment       split pseudo-genome to specified equal part.\n");
    fprintf(stderr, "         demux         fastq demultiplex using index.\n");
    fprintf(stderr, "         individ       split fasta sequence, one file per sequence.\n");
    fprintf(stderr, "\n");

    fprintf(stderr, "      -- auxiliary utils.\n");
    fprintf(stderr, "         reorient      reorient fastq strandness using reference.\n");
    fprintf(stderr, "         revcomp       revcomp DNA sequence.\n");
    fprintf(stderr, "         rna2dna       convert RNA to DNA sequence.\n");
    fprintf(stderr, "         bed           convert shred location to bed.\n");
    fprintf(stderr, "         region-mask   mask base to N/lowcase in bed region.\n");
    fprintf(stderr, "         tsv2fasta     tsv convert to fasta.\n");
    fprintf(stderr, "         circular      identify circular contigs from SPAdes assembler \n");
    fprintf(stderr, "                       and the repeated k-mer found at terminal.\n");

    fprintf(stderr, "\n");

    fprintf(stderr, "\nLicenced:\n");
    fprintf(stderr, "(c) 2016-2023 - LEI ZHANG\n");
    fprintf(stderr, "Logic Informatics Co.,Ltd.\n");
    fprintf(stderr, "zhanglei@logicinformatics.com\n");
    fprintf(stderr, "\n");

    return 1;

}

int main(int argc, char *argv[]){

    if (argc < 2) return usage();
    
    if (strcmp(argv[1], "info") == 0) info_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "convert") == 0) convert_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "counts") == 0) counts_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "joint") == 0) joint_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "length") == 0) length_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "view") == 0) view_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "fqchk") == 0) fqchk_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "demux") == 0) demux_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "filter") == 0) filter_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "dedup") == 0) dedup_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "fake") == 0) fake_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "avq") == 0) avq_main(argc - 1, argv + 1);

    else if (strcmp(argv[1], "interleave") == 0) interleave_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "uniques") == 0) uniques_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "deinterleave") == 0) deinterleave_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "rename") == 0) rename_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "label") == 0) label_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "truncate") == 0) truncate_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "comment") == 0) comment_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "replace") == 0) replace_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "reorder") == 0) reorder_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "subseq") == 0) subseq_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "fetch") == 0) fetch_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "strip_stop_codon") == 0) strip_stop_codon_main(argc - 1, argv + 1);

    else if (strcmp(argv[1], "partition") == 0) partition_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "shred") == 0) shred_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "split") == 0) split_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "pseudo") == 0) pseudo_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "individ") == 0) individ_main(argc - 1, argv + 1);

    else if (strcmp(argv[1], "reorient") == 0) reorient_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "revcomp") == 0) revcomp_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "rna2dna") == 0) rna2dna_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "mask") == 0) mask_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "region-mask") == 0) region_mask_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "bed") == 0) bed_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "segment") == 0) segment_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "tsv2fasta") == 0) tsv2fasta_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "circular") == 0) circular_main(argc - 1, argv + 1);
    else {
        fprintf(stderr, "[main] unrecognized command '%s'. Abort!\n", argv[1]);
        return 1;
    }
    return 0;

}