#include "utils.h"

int uniques_main(int argc, char *argv[]){
    
    char *label = "Uniq";
    int membership = 0;

    int c;
    while ((c = getopt(argc, argv, "l:m")) >= 0) {
        
        if (c == 'm') membership =1;
        if (c == 'l') label=optarg;

    }

    if ( optind == argc || argc > optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils uniques [options] <in.fa/q>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -l STR  sequence label, default: [Uniq]\n");
        fprintf(stderr, "  -m      print membership to stderr.\n\n");
        return 1;
    }

    khash_t(reg32) *h;
    h = kh_init(reg32);
    
    khint_t k;
    int index = 1;

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r"); 
    if( fp ){
        
        kseq_t *kseq;
        kseq  = kseq_init(fp);
        int   l, ret;
        while ((l = kseq_read(kseq)) >= 0) {
        
            k = kh_put(reg32, h, kseq->seq.s, &ret);
            
            if(ret == 0){
                if(membership) fprintf(stderr, "%s\t%s_%d\n", kseq->name.s, label, kh_val(h, k));
            }else if(ret){
                
                kh_key(h, k) = strdup(kseq->seq.s);
                kh_val(h, k) = index++;
                printf(">%s_%d\n%s\n", label, kh_val(h, k), kseq->seq.s);
                if(membership) fprintf(stderr, "%s\t%s_%d\n", kseq->name.s, label, kh_val(h, k));
            
            }
        
        }

        kseq_destroy(kseq);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(1);
    }

    kh_reg32_destroy(h);
    return 0;

}