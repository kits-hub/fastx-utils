#include "utils.h"

int partition_counts(char *file);

int partition_main(int argc, char *argv[]){

    char* ftype="fasta";
    int c;
    while ((c = getopt(argc, argv, "f:")) >= 0) {
        
        if (c == 'f') ftype = optarg;

    }

    if ( optind == argc || argc != optind + 3) {
        fprintf(stderr, "\nUsage: fastx-utils partition [options] <fasta/q> <partition: 10> <prefix>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -f  fastx file suffix: default: [fasta];\n\n");
        return 1;
    }

    if(strcmp(argv[optind], "-") == 0){
        fprintf(stderr, "[ERR]: not support streaming\n");
        return 1;
    }
    
    int count, max;

    max   = atoi(argv[optind+1]);
    count = partition_counts(argv[optind]);
   
    count = ( count % max == 0) ? 
                    count/max :
                    count/max + 1;
    
    char    *prefix        = NULL;
    prefix = argv[optind+2];

    kstring_t chunk_name  = {0, 0, 0};
    FILE *output = NULL;
    int     current_chunk  = 1;
    int     current_index  = 0;
    
    ksprintf(&chunk_name,"%s_%d.%s", prefix, current_chunk++, ftype);
    
    if ((output = fopen(chunk_name.s,  "w")) == NULL) {
        fprintf(stderr,"[ERR]: can't open file %s\n", chunk_name.s);
        exit(1);
    }

    gzFile fp;
    fp    = gzopen(argv[optind], "r");
    if(fp){
    
        kseq_t *kseq;
        kseq   = kseq_init(fp);    
        int l;
        while ((l = kseq_read(kseq)) >= 0){
            
            current_index++;
            if(current_index <= count){
                print_kseq(kseq, output);    
                continue;
            }
            
            fclose(output);
            current_index = 1;

            chunk_name.l  = 0;
            ksprintf(&chunk_name,"%s_%d.%s", prefix, current_chunk++, ftype);
            if ((output = fopen(chunk_name.s, "w")) == NULL) {
                fprintf(stderr,"[ERR]: can't open file %s\n", chunk_name.s);
                exit(1);
            }
            print_kseq(kseq, output);

        }
        
        kseq_destroy(kseq);
        gzclose(fp);
    }else{

        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 
    
    }

    printf("%d\n", --current_chunk);
    free(chunk_name.s);
    return 0;
}

inline int partition_counts(char *file){

    int count = 0;
    
    gzFile fp;
    fp    = gzopen(file, "r");
    if(fp){
        kseq_t *kseq;
        kseq   = kseq_init(fp);
        int l;
        while ((l = kseq_read(kseq)) >= 0) count++;
    }else{

        fprintf(stderr, "[ERR]: can't open file %s\n", file);
        exit(-1); 
    
    }
    
    return count;

}