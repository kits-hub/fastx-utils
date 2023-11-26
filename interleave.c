#include "utils.h"

int interleave_main (int argc, char *argv[]){
    
    int fashion = 0;
    int c;
    while ((c = getopt(argc, argv, "f")) >= 0) {
        if (c == 'f') fashion = 1;
    }

    if ( optind == argc || argc != optind + 2) {

        fprintf(stderr, "\nUsage: fastx-utils interleave [options] <in1.fq> <in2.fq>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -f  old fashion interleave fastq format\n\n");
        return 1;
    } 

    gzFile fp1, fp2;
    fp1 = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    fp2 = strcmp(argv[optind + 1], "-")? gzopen(argv[optind + 1], "r") : gzdopen(fileno(stdin), "r");
    
    kseq_t *seq[2];
    seq[0] = kseq_init(fp1);
    seq[1] = kseq_init(fp2);
    
    while (kseq_read(seq[0]) >= 0) {
        
        if (kseq_read(seq[1]) < 0) {
            fprintf(stderr, "[W::%s]: the 2nd file has fewer records.\n", __func__);
            break;
        }

        if(fashion){
            printf("@%s/1\n%s\n+\n%s\n", seq[0]->name.s, seq[0]->seq.s, seq[0]->qual.s);
            printf("@%s/2\n%s\n+\n%s\n", seq[1]->name.s, seq[1]->seq.s, seq[1]->qual.s);            
        }else{
            printf("@%s %s\n%s\n+\n%s\n", seq[0]->name.s, seq[0]->comment.s,  seq[0]->seq.s, seq[0]->qual.s);
            printf("@%s %s\n%s\n+\n%s\n", seq[1]->name.s, seq[1]->comment.s,  seq[1]->seq.s, seq[1]->qual.s); 
        }


    }
    
    if (kseq_read(seq[1]) >= 0)
        fprintf(stderr, "[W::%s]: the 1st file has fewer records.\n", __func__);
    kseq_destroy(seq[0]); gzclose(fp1);
    kseq_destroy(seq[1]); gzclose(fp2);

    return 0;
}
