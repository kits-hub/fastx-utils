#include "utils.h"

int dedup_main(int argc, char *argv[]){
    
    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils dedup <in.fa/fq>\n\n");
        return 1;
    }

    khash_t(set) *h;
    h = kh_init(set);
    khint_t k;

    int absent;

    gzFile     fp;
    kseq_t   *kseq;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r"); 
    
    if( fp ){
        kseq  = kseq_init(fp);
        int   l;
        while ((l = kseq_read(kseq)) >= 0) {

            k = kh_get(set, h, kseq->name.s);
            if(k == kh_end(h)){
                print_kseq(kseq, stdout);
                k = kh_put(set, h, kseq->name.s, &absent);
                kh_key(h, k) = strdup( kseq->name.s );
            }

        }
        
        kseq_destroy(kseq);
        gzclose(fp);

    }else{

        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 

    }
    
    kh_set_destroy(h);
    return 0;

}
