#include "utils.h"

static int qual;
int fqchk_main(int argc, char *argv[]){

    int offset = 33;
    int c;
    while ((c = getopt(argc, argv, "q:")) >= 0)
        if (c == 'q') offset = atoi(optarg);

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils fqchk [options] <in.fq>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -q INT     offset value: 33 for Sanger quality, 64 for Illumina quality, default: [33]\n\n");
        return 1;
    }

    int l = 0;

    int64_t lines   = 0;
    int64_t bases   = 0;
    int64_t q20     = 0;
    int64_t q30     = 0;

    int     min_len = 2000;
    int     max_len = 0;

    int i, ret;

    khash_t(int64) *h;
    h = kh_init(int64);
    khint_t k;

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    kseq_t *kseq;
    kseq = kseq_init(fp);

    while ((l = kseq_read(kseq)) >= 0){

        if(kseq->seq.l != kseq->qual.l){
            fprintf(stderr,"[ERR]: bad fastq file format!\n");
            exit(-1);
        }

        lines += 1;
        bases += kseq->seq.l;

        if(kseq->seq.l > max_len) max_len=kseq->seq.l;
        if(kseq->seq.l < min_len) min_len=kseq->seq.l;

        for (i = 0; i < kseq->seq.l; ++i){
            
            k = kh_get(int64, h, i);
            if( k == kh_end(h) ){

                k = kh_put(int64, h, i, &ret);
                int64_t *stats    = (int64_t *) calloc (47, sizeof(int64_t));
                kh_key(h, k)  = i;
                kh_val(h, k)  = stats;
            
            }

            switch( kseq->seq.s[i]){
                case 'A':  kh_val(h, k)[0]++;  break;
                case 'T':  kh_val(h, k)[1]++;  break;
                case 'C':  kh_val(h, k)[2]++;  break;
                case 'G':  kh_val(h, k)[3]++;  break;
                case 'N':  kh_val(h, k)[4]++;  break;
                default:   fprintf(stderr, "[ERR]: bad base %c!", kseq->seq.s[i]);
            }

            if(qual > 64 && offset != 64){
                fprintf(stderr, "[ERR]: need specify -q 64!\n");
                return 0;
            }
            qual = kseq->qual.s[i] - offset;
            
            if(qual >= 20) q20++;
            if(qual >= 30) q30++;

            kh_val(h, k)[qual + 4]++;

        }

    }
    
    kseq_destroy(kseq);

    printf("#sequence: %lld; base: %lld; min_len: %d; max_len: %d; avg_len: %.2lf; Q20: %.4lf; Q30: %.4lf;\n", 
                                                                                       (long long)lines,
                                                                                       (long long)bases,
                                                                                       min_len,
                                                                                       max_len,
                                                                                       (double)bases/lines,
                                                                                       (double)q20/bases,
                                                                                       (double)q30/bases);
    printf("#POS\tA\tT\tC\tG\tN");
    for (i = 1; i <= 42; ++i) printf("\tQ%d", i);
    putchar('\n');


    int j;

    for (i = 0; i< max_len; ++i) {
        k = kh_get(int64, h, i);
        if( k != kh_end(h) ){
            
            printf("%d", i + 1);
            for (j = 0; j < 47 ; ++j){
                printf("\t%zu", kh_val(h, k)[j] );
            }
            putchar('\n');

        }else{
            fprintf(stderr, "[ERR]: bad position %d", i + 1);
        }
    }

    kh_int64_destroy(h);
    gzclose(fp);
    return 0;
}