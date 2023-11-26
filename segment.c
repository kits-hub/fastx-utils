#include <ctype.h>
#include "utils.h"
#include <math.h>

int segment_main(int argc, char *argv[]){
    
    int n = 20; 
    int c;
    while ((c = getopt(argc, argv, "n:")) >= 0) {
        if (c == 'n') n = atoi(optarg);
    }

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils [options] segment <fasta>\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -n INT  segment number, default: [20]\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");   
    if(fp){
        
        kseq_t *kseq;
        kseq = kseq_init(fp);
        kstring_t kt = {0, 0, 0};

        int l;
        int i    = 1;
        int loct = 0;
        int len  = 0;

        if((l = kseq_read(kseq)) >= 0){

            int size = ceil( kseq->seq.l * 1.0 / n );
            
            while( loct < kseq->seq.l ){
                
                if(loct + size < kseq->seq.l ) len = size;
                else len = kseq->seq.l - loct;
                
                kt.l = 0;
                kputsn(kseq->seq.s + loct, len, &kt);

                printf(">%s_%d\n%s\n", kseq->name.s, i, kt.s);
                loct += len;
                ++i;
            }
        
        }
        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
    }
   
    return 0;
}