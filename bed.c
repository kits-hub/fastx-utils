#include "utils.h"

int bed_main(int argc, char *argv[]){

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils bed <fasta/q>\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    if(fp){

        kstring_t kt = {0,0,0};

        kseq_t *kseq;
        kseq = kseq_init(fp);
        
        int *fields, n, i;
        char *p;

        int l;
        while ((l = kseq_read(kseq)) >= 0){

            /** NZ_MDCH01000001.1:1-100 **/

            fields = ksplit(&kseq->name, ':', &n);

            kt.l   = 0;
            kputs(kseq->name.s, &kt);
            for (i = 1; i < n - 1; ++i){
                kputc(':', &kt);
                kputs(kseq->name.s + fields[i], &kt);
            }            
            p = strrchr(kseq->name.s + fields[n - 1], '-');

            printf("%s\t%d\t%s\n", kt.s,
                                   atoi(kseq->name.s + fields[ n - 1]) - 1,
                                   p + 1);

        }

        free(kt.s);
        kseq_destroy(kseq);
        gzclose(fp);
    }else{
     
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 
    
    }

    return 0;
}
