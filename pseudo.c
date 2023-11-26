#include "utils.h"

int pseudo_main(int argc, char *argv[]){
    
    int   n_counts = 150;
    char* label    = "pseudo";
   
    int c;
    while ((c = getopt(argc, argv, "n:l:")) >= 0) {
        if (c == 'n')    n_counts = atoi(optarg);        
        else if (c == 'l')  label = optarg;

    }
    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils pseudo  [options]  <fasta>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -n  padding number of 'N's: default: [150];\n");
        fprintf(stderr, "  -l  pseudo-sequence label: default: [pseudo];\n\n");
        return 1;
    }


    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    kseq_t *ks;    
    kstring_t kt = {0, 0, 0};

    ks = kseq_init(fp);

    int l, i;
    while ((l = kseq_read(ks)) >= 0){
        kputs(ks->seq.s, &kt);
        for (i = 0; i < n_counts; ++i) kputc('N', &kt);
    }
    
    kt.s[kt.l - n_counts] = '\0';
    kt.l -= n_counts;

    printf(">%s\n%s\n", label, kt.s);

    kseq_destroy(ks);
    free(kt.s);
    gzclose(fp);
    return 0;
}