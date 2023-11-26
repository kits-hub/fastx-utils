#include "utils.h"

typedef struct{
    FILE *fh[2];
} HANDLE;

KHASH_MAP_INIT_STR(handle, HANDLE)

void demux_make_fs(const char *d);
void demux_index(khash_t(handle) *h, const char *d, const char *f);
void demux_destroy(khash_t(handle) *h);
void demux (khash_t(handle) *h, const char *f, int strand);

int demux_main ( int argc, char *argv[] ){
    
    char *fwd       = NULL;
    char *rwd       = NULL;
    char *index     = NULL;
    char *directory = NULL;
    
    int c;
    while ((c = getopt(argc, argv, "d:i:1:2:")) >= 0) {
        if (c == 'd')      directory = optarg;
        else if (c == 'i') index     = optarg;
        else if (c == '1') fwd       = optarg;
        else if (c == '2') rwd       = optarg;
    }

    if (index == NULL || fwd == NULL || rwd == NULL) {

        fprintf(stderr, "\nUsage: fastx-utils demux [options]\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "    -d  output directory for demux reads. default: [./]\n");
        fprintf(stderr, "    -i  index sheet [sample\\tindex]. \n");
        fprintf(stderr, "    -1  forword reads. \n");
        fprintf(stderr, "    -2  reverse reads. \n\n");
        return 1;
    
    }

    if(directory != NULL) demux_make_fs(directory);

    khash_t(handle) *h;
    h = kh_init(handle);
 
    demux_index(h, directory, index);
    
    demux(h, fwd, 0);
    demux(h, rwd, 1);

    demux_destroy(h);

    return 0;
}

void demux_make_fs(const char *d){

    kstring_t kv = {0, 0, 0};
    ksprintf(&kv, "mkdir -p %s", d);
    if(  system(kv.s) != 0 ) exit (-1);
    free(kv.s);

}

void demux_index(khash_t(handle) *h, const char *d, const char *f) {

    gzFile     fp;    
    fp = strcmp(f, "-")? gzopen(f, "r") : gzdopen(fileno(stdin), "r"); 

    if (fp) {

        kstream_t *ks;
        ks = ks_init(fp);

        kstring_t kt = {0, 0, 0};
        kstring_t kv = {0, 0, 0};
        kstring_t fw = {0, 0, 0};
        kstring_t rw = {0, 0, 0};

        int *fields, n, ret;
        khint_t k;

        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
            
            if(kt.s[0] == '#') continue; 
            fields  = ksplit(&kt, '\t', &n);

            if(n != 2){
                fprintf(stderr, "[ERR]: index file format error!\n");
                exit(1);
             }

            kv.l = 0;
            if(d == NULL) ksprintf(&kv, "./%s", kt.s); 
            else ksprintf(&kv, "%s/%s", d, kt.s);

            fw.l = rw.l = 0;
            ksprintf(&fw, "%s_R1.fastq", kv.s);
            ksprintf(&rw, "%s_R2.fastq", kv.s);

            k = kh_put(handle, h, kt.s + fields[1], &ret);
            kh_key(h, k) = strdup(kt.s + fields[1]);
            kh_val(h, k).fh[0] = fopen(fw.s, "w");
            kh_val(h, k).fh[1] = fopen(rw.s, "w");
  
         }
         ks_destroy(ks);
         gzclose(fp);

         free(kt.s);
         free(kv.s);
         free(fw.s);
         free(rw.s);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", f);
        exit(1);
    }

}

void demux (khash_t(handle) *h, const char *f, int strand) {

    gzFile fp;
    fp = strcmp(f, "-")? gzopen(f, "r") : gzdopen(fileno(stdin), "r"); 

    int *fields, n;
    khint_t k;

    if(fp){

        kseq_t *kseq;
        kseq = kseq_init(fp);

        kstring_t comment = {0, 0, 0};

        int l;
        while ((l = kseq_read(kseq)) >= 0){
          
          comment.l = 0;
          kputs(kseq->comment.s, &comment);

          fields = ksplit(&comment, ':', &n);
          k = kh_get(handle, h, comment.s + fields[3]);
          if( k != kh_end(h) ) print_kseq(kseq, kh_val(h, k).fh[strand]);
        
        }

        free(comment.s);
        kseq_destroy(kseq);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", f);
        exit(-1); 
    }


}

void demux_destroy(khash_t(handle) *h){
    
    khint_t k;
    if  (h == 0) return;
    for (k = 0; k < kh_end(h); ++k){
        if (kh_exist(h, k)){
            free( (char*)kh_key(h, k) );
            fclose( kh_val(h, k).fh[0] );
            fclose( kh_val(h, k).fh[1] );
        }
    }
    kh_destroy(handle, h);
}
