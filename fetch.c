#include "utils.h"

int fetch_main(int argc, char *argv[]){
    
    if ( optind == argc || argc != optind + 2) {

        fprintf(stderr, "\nUsage: fastx-utils fetch <fa/fq> <id>\n\n");
        return 1;
    } 

    gzFile fp;
    fp = strcmp(argv[ optind ], "-")? gzopen(argv[ optind ], "r") : gzdopen(fileno(stdin), "r");

    if(fp){

        kseq_t *kseq;
        kseq = kseq_init(fp);
        
        int l;
        while ((l = kseq_read(kseq)) >= 0){

            if(strcmp(kseq->name.s, argv[optind + 1]) == 0){
               print_kseq(kseq, stdout);
               break;
            }
        
        }

        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ]);
        exit(-1); 
    }

    return 0;

}