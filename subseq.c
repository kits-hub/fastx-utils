#include "utils.h"

int subseq_main(int argc, char *argv[]){
    
    int type = 1;
    int c;
    while ((c = getopt(argc, argv, "s")) >= 0) {
        if (c == 's') type = 0;
    }

    if ( optind == argc || argc != optind + 2) {
        fprintf(stderr, "\nUsage: fastx-utils subseq [options] <in.fa/fq> <name.list>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -s   supplementary set option.\n\n");
        return 1;
    }

    khash_t(set) *h;
    h = kh_init(set);
    khint_t k;

    kstream_t *ks;    
    int absent;

    gzFile     fp;    
    fp = strcmp(argv[optind + 1], "-")? gzopen(argv[ optind + 1 ], "r") : gzdopen(fileno(stdin), "r"); 

    kstring_t  kt  = {0, 0, 0};
    if( fp ){

        ks            = ks_init(fp);
        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
            if( kt.l == 0 ) continue;
            k = kh_put(set, h, kt.s, &absent);
            if(absent) kh_key(h, k) = strdup(kt.s);
        }

        ks_destroy(ks);
        gzclose(fp);

    }

    if(kh_size(h) == 0 && type == 1) return 0;

    kseq_t   *kseq;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r"); 
    
    if( fp ){
        kseq  = kseq_init(fp);
        int   l;
        while ((l = kseq_read(kseq)) >= 0) {
        
            k = kh_get(set, h, kseq->name.s);
            
            if(k != kh_end(h) && type == 1){
                print_kseq(kseq, stdout);
            }else if(k == kh_end(h) && type == 0){
                print_kseq(kseq, stdout);
            }
        }
        
        kseq_destroy(kseq);
        gzclose(fp);

    }
    
    kh_set_destroy(h);
    free(kt.s);
    return 0;

}
