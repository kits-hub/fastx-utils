#include "utils.h"
#define MAXSTRLEN 1024*32

static int Q = 5;
static double E=0.5, N=0.1;

int filter (kseq_t *kseq ){

        int Ns = 0, Qs = 0;
        int i;
        for (i = 0; i < kseq->seq.l; ++i){
            if( kseq->seq.s[i] == 'N') Ns++;
            if( (kseq->qual.s[i] - 33) <= Q ) Qs++;
        }

        int r;
        r = ( (Ns * 1.0)/kseq->seq.l > N ||  (Qs * 1.0)/kseq->seq.l > E ) ? 1 : 0;
        return r;

}

int filter_main (int argc, char *argv[]){



    int c;
    while ((c = getopt(argc, argv, "q:e:n:")) >= 0) {
        if (c == 'q') Q = atoi(optarg);
        if (c == 'e') E = atof(optarg);
        if (c == 'n') N = atof(optarg);
    }

    if ( optind == argc || argc > optind + 3) {
        fprintf(stderr, "\nUsage: fastx-utils filter [options] <in1.fq> <in2.fq> <prefix>\n\n");
        fprintf(stderr, "\nOptions:\n");
        fprintf(stderr, "  -q INT     < Qvalue for filter. [5].\n");
        fprintf(stderr, "  -e FLOAT   Discard reads with bases qvalue low than -q  >%%<FLOAT>. [0.5].\n");
        fprintf(stderr, "  -n FLOAT   Discard reads with N >%%<FLOAT>  [0.1].\n\n");
        return 1;
    }


    char forward_name[MAXSTRLEN];
    sprintf(forward_name,"%s_R1.fastq", argv[ optind + 2 ]);
    char reverse_name[MAXSTRLEN];
    sprintf(reverse_name,"%s_R2.fastq", argv[ optind + 2 ]);

    FILE *forward = fopen(forward_name, "w");
    FILE *reverse = fopen(reverse_name, "w");

    gzFile fp1, fp2;
    kseq_t *kseq[2];

    fp1 = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    fp2 = strcmp(argv[optind + 1], "-")? gzopen(argv[optind + 1], "r") : gzdopen(fileno(stdin), "r");
    
    if(fp1 && fp2){
        
        kseq[0] = kseq_init(fp1);
        kseq[1] = kseq_init(fp2);

        while (kseq_read(kseq[0]) >= 0) {
            
            if (kseq_read(kseq[1]) < 0) {
                fprintf(stderr, "[W::%s]: the 2nd file has fewer records.\n", __func__);
                break;
            }
            
            if( filter(kseq[0]) ) continue;
            if( filter(kseq[1]) ) continue;
    
            print_kseq(kseq[0], forward);
            print_kseq(kseq[1], reverse);
        }
        
        if (kseq_read(kseq[1]) >= 0)
            fprintf(stderr, "[W::%s]: the 1st file has fewer records.\n", __func__);

        kseq_destroy(kseq[0]); gzclose(fp1);
        kseq_destroy(kseq[1]); gzclose(fp2);

     }else{

        fprintf(stderr, "[ERR]: can't open file %s or %s \n", argv[optind], argv[optind+1]);
        exit(-1);
     
     }

     fclose(forward);
     fclose(reverse);

     return 0;
}

