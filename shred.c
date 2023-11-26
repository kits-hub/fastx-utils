#include "utils.h"

int shred_main(int argc, char *argv[]){

    if (argc != 4 && isatty(fileno(stdin))) {
        fprintf(stderr, "\nUsage: fastx-utils shred <fasta> <length> <overlap>\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    
    if(fp){
    
        int length, overlap, l;
        length  = atoi(argv[2]);
        overlap = atoi(argv[3]);

        if(length <= overlap){
            fprintf(stderr, "[ERR]: length:[%d] shorter than overlap:[%d]\n", length, overlap);
            exit(-1);
        } 

        kstring_t kt = {0, 0, 0};

        kseq_t *kseq;
        kseq = kseq_init(fp);
        while ((l  = kseq_read(kseq)) >= 0){
            
            int i     = 0;
            while( i + overlap <  l  ){
                
                kt.l = 0;
                int r    = length;
                if( i + length >= l) r = l - i;

                kputsn(kseq->seq.s + i, r, &kt);
                fprintf(stdout, ">%s:%d-%d\n%s\n", kseq->name.s,
                                                   i + 1,
                                                   i + r,
                                                   kt.s);
                i = i + length - overlap;

            }

        }

        kseq_destroy(kseq);
        gzclose(fp);
        free(kt.s);
    }
    return 0;
}