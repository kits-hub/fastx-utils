#include "utils.h"
#include <limits.h>

static int qual;
int avq_main(int argc, char *argv[]){

    int offset = 33;
    int c;
    while ((c = getopt(argc, argv, "q:")) >= 0)
        if (c == 'q') offset = atoi(optarg);

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: avq-len [options] <in.fq>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -q INT     offset value: 33 for Sanger quality, 64 for Illumina quality, default: [33]\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    kseq_t *kseq;
    kseq = kseq_init(fp);

    printf("reads_id\tqvalue\tlength\n");

    int num = 1;
    int i, l;
    while ((l = kseq_read(kseq)) >= 0){

        if(kseq->seq.l != kseq->qual.l){
            fprintf(stderr,"[ERR]: bad fastq file format!\n");
            exit(-1);
        }

        int64_t q = 0;
        for (i = 0; i < kseq->seq.l; ++i){
            
            if(qual > 64 && offset != 64){
                fprintf(stderr, "[ERR]: need specify -q 64!\n");
                return 0;
            }
            
            q += kseq->qual.s[i] - offset;

        }

        printf("%d\t%.1f\t%zu\n", num++, 1.0 * q/kseq->seq.l , kseq->seq.l);

    }
    
    kseq_destroy(kseq);
    gzclose(fp);
    return 0;
}