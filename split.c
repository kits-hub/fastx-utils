#include "utils.h"

int split_main(int argc, char *argv[]){

    char* ftype="fasta";
    int c;
    while ((c = getopt(argc, argv, "f:")) >= 0) {
        
        if (c == 'f') ftype = optarg;

    }

    if ( optind == argc || argc != optind + 3) {
        fprintf(stderr, "\nUsage: fastx-utils split [options] <fasta/q> <size> <prefix>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -f  fastx file suffix: default: [fasta];\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    if(fp){
        
        char    *prefix        = NULL;
        int     max            = 0;
        int     current_chunk  = 1;
        int     current_index  = 0;
        int     l;
        
        kstring_t chunk_name = {0,0,0};
        FILE *output = NULL;
      
        max    = atoi(argv[optind+1]);
        prefix = argv[optind+2];

        ksprintf(&chunk_name,"%s_%d.%s", prefix, current_chunk++, ftype);  
        if ((output = fopen(chunk_name.s, "w")) == NULL) {
            fprintf(stderr,"[ERR]: can't open file %s\n", chunk_name.s);
            exit(1);
        }

        kseq_t *kseq;
        kseq = kseq_init(fp);    
        while ((l = kseq_read(kseq)) >= 0){
            
            current_index++;
            if(current_index <= max){
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

        free(chunk_name.s);
        kseq_destroy(kseq);
        gzclose(fp);
        printf("%d\n", --current_chunk);
    }

    return 0;
}
