#include "utils.h"

int counts_main(int argc, char *argv[]){
    
    char *label = NULL;
    int   base  = 0;

    int c;
    while ((c = getopt(argc, argv, "l:b")) >= 0) {
        if (c == 'l')  label = optarg;
        else if(c == 'b') base = 1;
    }

    if ( optind == argc || argc > optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils counts [options] <fastq>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -l STR  Label for the fasta/q file. default: [NULL]\n");
        fprintf(stderr, "  -b      Enable display the base counts. default: []\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    if(fp){
        
        kseq_t *kseq;
        kseq = kseq_init(fp);

        int64_t total = 0;
        int64_t lines = 0;

        int l = 0;
        while ((l = kseq_read(kseq)) >= 0){
            total += kseq->seq.l;
            lines += 1;
        }

        kstring_t kt = {0, 0, 0};
        if(label != NULL) ksprintf(&kt, "%s\t", label);
        ksprintf(&kt, "%lld", (long long)lines);
        if(base) ksprintf(&kt, "\t%lld", (long long)total);

        puts(kt.s);

        free(kt.s);
        kseq_destroy(kseq);
        gzclose(fp);

    }else{

        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 

    }

    return 0;
}