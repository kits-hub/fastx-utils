#include "utils.h"

int comment_main(int argc, char *argv[]){
    
    if ( argc != 3) {
        fprintf(stderr, "\nUsage: fastx-utils comment <db> <in.fa/q>\n\n");
        return 1;
    }

    khash_t(reg) *h;
    h = kh_init(reg);
    khint_t k;

    gzFile     fp;    
    fp = strcmp(argv[1], "-")? gzopen(argv[ 1 ], "r") : gzdopen(fileno(stdin), "r"); 
    if( fp ){

        kstream_t *ks;
        int ret;
        ks            = ks_init(fp);
        kstring_t  kt  = {0, 0, 0};

        char *p;
        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
            if( kt.l == 0 ) continue;
          
            ks_tokaux_t aux;
            p = kstrtok(kt.s, "\t", &aux);
            kt.s[aux.p - p] = '\0';

            k = kh_put(reg, h, kt.s, &ret);
            kh_key(h, k) = strdup(p);
            kh_val(h, k) = strdup(aux.p + 1);


        }
        free(kt.s);
        ks_destroy(ks);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[1]);
        exit(-1); 
    }

    if(kh_size(h) == 0) return 0;

    fp = strcmp(argv[2], "-")? gzopen(argv[2], "r") : gzdopen(fileno(stdin), "r"); 
    
    if( fp ){
        
        kseq_t *kseq;
        kseq  = kseq_init(fp);
        int   l;
        while ((l = kseq_read(kseq)) >= 0) {
        
            k = kh_get(reg, h, kseq->name.s);         
            
            if(k != kh_end(h) ){
                if( kseq->comment.l != 0)
                    kputc( ' ', &kseq->comment);
                kputs( kh_val(h, k), &kseq->comment);
            }

            print_kseq(kseq, stdout);
        }
        
        kseq_destroy(kseq);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[2]);
        exit(-1); 
    }

    kh_reg_destroy(h);

    return 0;

}