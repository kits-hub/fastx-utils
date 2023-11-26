#include "utils.h"

#define MAXSTRLEN 1024*32

int deinterleave_main(int argc, char *argv[]){

    int f = 0;
    int c;
    while ((c = getopt(argc, argv, "f")) >= 0) {
        if (c == 'f') f = 1;
    }

    if ( optind == argc || argc != optind + 2) {

        fprintf(stderr, "\nUsage: fastx-utils deinterleave [options] <fasta/q> <prefix>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -f  specify input file as fasta format\n\n");
        return 1;
    }

    char *prefix  =  argv[optind + 1];
    char forward_name[MAXSTRLEN];
    char reverse_name[MAXSTRLEN];

    if( f == 0 ){
        sprintf(forward_name,"%s_R1.fastq", prefix);
        sprintf(reverse_name,"%s_R2.fastq", prefix);
    }else{
        sprintf(forward_name,"%s_R1.fasta", prefix);
        sprintf(reverse_name,"%s_R2.fasta", prefix);
    }

    FILE *forward = fopen(forward_name, "w");
    FILE *reverse = fopen(reverse_name, "w");

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    if(fp){
 
        kseq_t *kseq;
        kseq = kseq_init(fp);    
        int l;
        while ((l = kseq_read(kseq)) >= 0){
            print_kseq(kseq, forward);
            if ( (l = kseq_read(kseq)) >= 0 )
                print_kseq(kseq, reverse);
            else
                fprintf(stderr, "[ERR]: fasta/q format with errors!\n");
        }
        
        fclose(forward);
        fclose(reverse);
        kseq_destroy(kseq);
        gzclose(fp);
    }
    return 0;

}