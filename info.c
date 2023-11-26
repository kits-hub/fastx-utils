#include "utils.h"

int info_main(int argc, char *argv[]){

    if (argc != 2) {
        fprintf(stderr, "\nUsage: fastx-utils info <fasta/q>\n\n");
        exit(-1);
    }

    gzFile fp;
    fp = strcmp(argv[1], "-")? gzopen(argv[1], "r") : gzdopen(fileno(stdin), "r");
    
    if(fp){

        int64_t lines   = 0;
        int64_t bases   = 0;
        int64_t gc      = 0;

        long long int     min_len = UINT_MAX;
        long long int     max_len = 0;

        kseq_t *kseq;
        kseq = kseq_init(fp);
        int i,l;

        while ((l = kseq_read(kseq)) >= 0){

            lines += 1;
            bases += kseq->seq.l;
            if(kseq->seq.l > max_len) max_len=kseq->seq.l;
            if(kseq->seq.l < min_len) min_len=kseq->seq.l;

            for (i = 0; i < kseq->seq.l; ++i){
                if(kseq->seq.s[i] == 'G' || kseq->seq.s[i] == 'C' || kseq->seq.s[i] == 'g' ||  kseq->seq.s[i] == 'c') gc++;
            }

        }
        
        kseq_destroy(kseq);
        gzclose(fp);

        printf("#sequence\tbase\tmin_len\tmax_len\tavg_len\tGC(%%)\n%lld\t%lld\t%lld\t%lld\t%.2lf\t%.2lf\n", 
                                                                                           (long long)lines,
                                                                                           (long long)bases,
                                                                                           min_len,
                                                                                           max_len,
                                                                                           (double)bases/lines,
                                                                                           (double) 100 * gc/bases);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[1]);
        exit(-1); 
    }

    return 0;
}