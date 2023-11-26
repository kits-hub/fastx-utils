#include "utils.h"
#include "kvec.h"

int circular_main(int argc, char *argv[]){
    
    char *kmer = "127";
    int c;
    while ((c = getopt(argc, argv, "k:")) >= 0) {
        if (c == 'k') kmer = optarg;
    }

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils circular [options] <fasta>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -k   kmers in spades assembly [default: 127].\n\n");
        return 1;
    }

    kvec_t( int ) vt;
    kv_init(vt);
    kstring_t kt = {0, 0, 0};
    kputs(kmer, &kt);
    
    int *fields, n, i;
    fields = ksplit(&kt, ',', &n);
    for (i = 0; i < n; ++i) kv_push(int, vt, atoi(kt.s + fields[i]) );

    gzFile fp;
    fp = strcmp(argv[ optind ], "-")? gzopen(argv[ optind ], "r") : gzdopen(fileno( stdin ), "r");

    if(fp){

        kseq_t *kseq;
        kseq = kseq_init(fp);
        
        int l; 
        while ((l = kseq_read(kseq)) >= 0){

            for (i = n - 1; i >= 0; --i){
                int k = kv_A(vt, i);
                if( kseq->seq.l < 2 * k) continue;
                if( strncmp(kseq->seq.s, kseq->seq.s + kseq->seq.l - k, k) == 0 ){
                    kseq->seq.s[ kseq->seq.l - k ] = '\0';
                    print_kseq(kseq, stdout);
                    break;
                }
            }
        }

        kseq_destroy(kseq);
        gzclose(fp);
    
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ]);
        exit(-1); 
    }

    kv_destroy(vt);
    return 0;
}

