#include "utils.h"

int truncate_main(int argc, char *argv[]){

    int D, L, B;
    D=L=B=0;

    int c;
    while ((c = getopt(argc, argv, "l:db")) >= 0) {
        if (c == 'd') D = 1;
        else if (c == 'b') B = 1;
        else if (c == 'l') L = atoi(optarg);
    }

    if ( optind == argc || argc > optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils truncate [options] <fasta/q>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -b      Truncate sequence from left side, default: [right]\n");
        fprintf(stderr, "  -d      Discard sequence short than L , defalt: [0]\n");
        fprintf(stderr, "  -l INT  Truncate sequence length to length 'L', defalt: [NONE]\n\n");
        return 1;
    }


    if(!L){
        fprintf(stderr, "[ERR]: need set '-l' option. \n");
        exit(-1);
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    if(fp){
        
        kseq_t *kseq;
        kseq = kseq_init(fp); 
        int l;

        while ((l = kseq_read(kseq)) >= 0){

            if(D && kseq->seq.l < L) continue;
              
            if(kseq->seq.l > L){
                if(B){
                    
                    if(kseq->qual.l > 0)
                        printf("@%s\n%s\n+\n%s\n", kseq->name.s, kseq->seq.s  + kseq->seq.l  - L, kseq->qual.s + kseq->qual.l - L);
                    else
                        printf(">%s\n%s\n", kseq->name.s, kseq->seq.s  + kseq->seq.l  - L);

                }else{
                    
                    kseq->seq.s[L]  = '\0';
                    if(kseq->qual.l > 0) kseq->qual.s[L] = '\0';
                    print_kseq(kseq, stdout);
                
                }
            }else print_kseq(kseq, stdout);

        }
        
        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 
    }
    
    return 0;

}