#include "utils.h"
#include <ctype.h>

int region_mask_main(int argc, char *argv[]){

    int base  = 0;
    int mask  = 0;

    int c;
    while ((c = getopt(argc, argv, "sb:")) >= 0) {
        if (c == 's')     mask = 1;
        else if(c == 'b') base = atoi( optarg );
    }

    if ( optind == argc || argc != optind + 2 ) {
        
        fprintf(stderr, "\n");       
        fprintf(stderr, "Usage: fastx-utils region-mask <bed> <fasta/q>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -s     softmask mode.\n");
        fprintf(stderr, "  -b INT base mode, default: [0] .\n\n");
        return 1;

    }

    khash_t(kreg) *h;
    h = kh_init(kreg);
    khint_t k;

    int *fields, n, ret;

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r"); 
    if(fp){
        
        kstream_t *ks;
        kstring_t kt    = {0, 0, 0};
        ks = ks_init(fp);
        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
                
                fields = ksplit(&kt, '\t' ,&n);               
                k = kh_put(kreg, h, kt.s, &ret);

                if(ret){
                    kh_key(h, k)   = strdup(kt.s);
                    kstring_t ktmp = {0, 0, 0};                 
                    ksprintf(&ktmp, "%s,%s", kt.s + fields[1], kt.s + fields[2]);
                    kh_val(h, k) = ktmp;

                }else if(ret == 0){
                    ksprintf(&kh_val(h, k), ",%s,%s", kt.s + fields[1], kt.s + fields[2]);
                }
        
        }

        free(kt.s);
        ks_destroy(ks);
        gzclose(fp);
    
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(1);
    }

    fp = strcmp(argv[optind + 1], "-")? gzopen(argv[optind + 1], "r") : gzdopen(fileno(stdin), "r"); 
    if (fp) {
        
        kseq_t *kseq;
        kseq  = kseq_init(fp);
        int   l;
        int   i, j;
        while ((l = kseq_read(kseq)) >= 0) {
           
            k = kh_get(kreg, h, kseq->name.s);

            if(k == kh_end(h)){
                print_kseq(kseq, stdout);
                continue;
            }

            fields = ksplit(&kh_val(h, k), ',' , &n);
            for (i = 0; i < n -1; i += 2){

                int start = atoi(kh_val(h, k).s + fields[i]) - base;
                int end   = atoi(kh_val(h, k).s + fields[i + 1]);
                for (j = start; j < end; ++j){
                    kseq->seq.s[j] = (mask) ? tolower(kseq->seq.s[j]) : 'N';
                }
            }
            print_kseq(kseq, stdout);

        }
        
        kseq_destroy(kseq);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind + 1]);
        exit(1);
    }

    kh_kreg_destroy( h );
    return 0;
}