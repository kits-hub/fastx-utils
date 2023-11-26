#include "utils.h"

int replace_main(int argc, char *argv[]){

    int R = 0;
    int C = 0;

    int c;
    while ((c = getopt(argc, argv, "cr")) >= 0) {
        
        if(c == 'r') R = 1;
        else if (c == 'c') C = 1;

    }

    if ( optind == argc || argc != optind + 2 ) {
        
        fprintf(stderr, "\n");       
        fprintf(stderr, "Usage: fastx-utils replace [options] <db> <fasta>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -r  remove unmapped sequence.\n");
        fprintf(stderr, "  -c  remove comment.\n\n");
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
            
            if(k == kh_end(h) && R)  continue;
            
            fputc('>', stdout);
            (k != kh_end(h) ) ? fputs(kh_val(h, k), stdout) : fputs(kseq->name.s, stdout);
            
            if( kseq->comment.l != 0  &&  C != 1 )
                printf(" %s\n", kseq->comment.s);
            else printf("\n");
            
            puts(kseq->seq.s);
          
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