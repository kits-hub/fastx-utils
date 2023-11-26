#include "utils.h"

int convert_main(int argc, char *argv[]){
    
    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils convert <fasta/q>\n\n");
        exit(-1);
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    
    if(fp){

        kseq_t *kseq;
        kseq = kseq_init(fp);
        int l;
        while ((l = kseq_read(kseq)) >= 0){

            if(!kseq->qual.l){
                fprintf(stderr, "[ERR]: not valid fastq format. %s\n", argv[optind]);
                exit(-1);    
            }

            kseq->qual.l = 0;
            print_kseq(kseq, stdout);

        }
        
        kseq_destroy(kseq);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 
    }

    return 0;
}