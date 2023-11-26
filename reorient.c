#include "utils.h"

int reorient_main(int argc, char *argv[]){

    if ( optind == argc || argc != optind + 2 ) { 
        fprintf(stderr, "\n");       
        fprintf(stderr, "Usage: fastx-utils reorient <map> <fastq>\n\n");
        return 1;
    }

    khash_t(reg) *h;
    h = kh_init(reg);
    khint_t k;

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r"); 
    if(fp){
        
        kstream_t *ks;
        kstring_t kt    = {0, 0, 0};
        ks = ks_init(fp);
        int ret;
        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
                
                ks_tokaux_t aux;
                kstrtok(kt.s, "\t", &aux);
                kt.s[aux.p - kt.s] = '\0';
                k = kh_put(reg, h, kt.s, &ret);
                
                if(ret){
                    kh_key(h, k) = strdup(kt.s);
                    kh_val(h, k) = strdup(aux.p + 1);
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
        while ((l = kseq_read(kseq)) >= 0) {
        
            k = kh_get(reg, h, kseq->name.s);
            if( k == kh_end(h) )  continue;
            
            if(kh_val(h, k)[0] == '-'){
                rev_com(&kseq->seq);
                if(kseq->qual.l > 0) rev(&kseq->qual);
            }
            print_kseq(kseq, stdout);

        }
        kseq_destroy(kseq);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind + 1]);
        exit(1);
    }

    kh_reg_destroy( h );
    return 0;
}