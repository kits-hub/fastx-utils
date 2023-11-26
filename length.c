#include "utils.h"
#include <limits.h>

int length_main(int argc, char *argv[]){

    int min = 0;
    int max = INT_MAX;

    int c;
    while ((c = getopt(argc, argv, "l:s:")) >= 0) {
        if (c == 's')      min = atoi(optarg);
        else if (c == 'l') max = atoi(optarg);
    }

    if ( optind == argc || argc > optind + 1) {
        
        fprintf(stderr, "\nUsage: fastx-utils length [options] <fasta/q>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -s INT  discard sequence short  than min, defalt: [0]\n");
        fprintf(stderr, "  -l INT  discard sequence longer than max, defalt: [INT_MAX]\n\n");
        
        return 1;
    
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    if(fp){
        
        kseq_t *kseq;
        kseq = kseq_init(fp); 
        
        int l;
        while ((l = kseq_read(kseq)) >= 0){
            
            if( kseq->seq.l < min ) continue;
            if( kseq->seq.l > max ) continue;
            
            print_kseq(kseq, stdout);

        }

        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 
    }
    
    return 0;

}