#include "utils.h"

int fake_main(int argc, char *argv[]){

    int qual = 'I';
    int c;
    while ((c = getopt(argc, argv, "q:")) >= 0) {
        if (c == 'q')  qual = optarg[0];
    }

    if ( optind == argc || argc != optind + 1) {

        fprintf(stderr, "\nUsage: fastx-utils fake [options] <fa/fq>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -q CHAR FASTQ quality . default: [I]\n\n");
        return 1;
    } 

    if (!( qual >= 33 && qual <= 127)) {
        fprintf(stderr, "[ERR]: not support the qual: %c\n", qual); 
    }

    gzFile fp;
    fp = strcmp(argv[ optind ], "-")? gzopen(argv[ optind ], "r") : gzdopen(fileno(stdin), "r");

    if(fp){

        kseq_t *kseq;
        kseq = kseq_init(fp);
        
        int l;
        while ((l = kseq_read(kseq)) >= 0){

            if (kseq->qual.m < kseq->seq.m) {
                kseq->qual.m = kseq->seq.m;
                kseq->qual.s = (char*)realloc(kseq->qual.s, kseq->qual.m);
            }
            kseq->qual.l = kseq->seq.l;
            memset(kseq->qual.s, qual, kseq->qual.l);
            kseq->qual.s[kseq->qual.l] = 0;

            print_kseq(kseq, stdout);

        }

        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ]);
        exit(-1); 
    }

    return 0;

}