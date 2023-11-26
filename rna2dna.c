#include "utils.h"

int rna2dna_main(int argc, char *argv[]){

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils rna2dna <in.fa>\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    
    if(fp){

        kseq_t *kseq;
        kseq = kseq_init(fp);
        int i, l;

        while ((l = kseq_read(kseq)) >= 0){
            for (i = 0; i < kseq->seq.l; ++i)
                if(kseq->seq.s[i] == 'u' || kseq->seq.s[i] == 'U') kseq->seq.s[i] -= 1;
            print_kseq(kseq, stdout);
        }
        
        kseq_destroy(kseq);
        gzclose(fp);

    }
    return 0;
}