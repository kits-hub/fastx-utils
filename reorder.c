#include "utils.h"
#include "kvec.h"

int reorder_main(int argc, char const *argv[]){
    
    if(argc != 3) {
        fprintf(stderr, "\nUsage: fastx-utils reorder <fasta/q> <list>\n\n");
        exit(1);
    }

    khash_t(set) *h;
    h = kh_init(set);
    khint_t k;

    int i, absent;
    kstring_t  kt  = {0, 0, 0};
    
    kvec_t(const char *) vs;
    kv_init(vs);

    gzFile     fp; 
    fp = strcmp(argv[2], "-")? gzopen(argv[2], "r") : gzdopen(fileno(stdin), "r"); 
    if( fp ){
        
        kstream_t  *ks;
        ks = ks_init(fp);
        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
            
            if( kt.l == 0 ) continue;
            k = kh_put(set, h, kt.s, &absent);
            if(absent == 1){
                char *p= strdup(kt.s);
                kh_key(h, k) = p;
                kv_push(const char*, vs, p);
            }
        
        }
        gzclose(fp);
    }
    if(kh_size(h) == 0) return 0;
   
    khash_t(reg) *map;
    map = kh_init(reg);

    fp = strcmp(argv[1], "-")? gzopen(argv[1], "r") : gzdopen(fileno(stdin), "r"); 
    if( fp ){
        
        kseq_t   *kseq;
        kseq   = kseq_init(fp);
        int   l;    
        kstring_t  kk  = {0, 0, 0};

        while ((l = kseq_read(kseq)) >= 0) {           
            k = kh_get(set, h, kseq->name.s);
            
            kk.l = 0;
            if(k != kh_end(h)){
               
               k = kh_put(reg, map, kseq->name.s, &absent);
               (kseq->qual.l > 0 ) ? kputc('@', &kk) : kputc('>', &kk);
               kputs(kseq->name.s, &kk);
               if(kseq->comment.l > 0 ) ksprintf(&kk, " %s",  kseq->comment.s );             
               kputc('\n', &kk);
               kputs(kseq->seq.s, &kk);

               if(kseq->qual.l > 0 ) ksprintf(&kk, "\n+\n%s", kseq->qual.s);

               kh_key(map, k) = strdup(kseq->name.s);
               kh_val(map, k) = strdup(kk.s);
           
           }
        }
        
        free(kk.s);
        kseq_destroy(kseq);
        gzclose(fp);

    }

    for (i = 0; i < kv_size(vs); ++i){

        k = kh_get(reg, map, kv_A(vs, i));
        if( k != kh_end(map) ) puts(kh_val(map, k));

    }

    kh_set_destroy(h);
    kv_destroy(vs);

    free(kt.s);
    kh_reg_destroy(map);
    return 0;
}
