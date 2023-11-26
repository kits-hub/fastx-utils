#include "utils.h"

int label_main(int argc, char *argv[]){
  

    int before = 0;
    int c;
    while ((c = getopt(argc, argv, "b")) >= 0) {
        if (c == 'b') before = 1;
    }

    if ( optind == argc || argc != optind + 2) {

        fprintf(stderr, "\nUsage: fastx-utils label [options] <fasta/q> <label>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -b  label before sequence name(left side).\n\n");
        return 1;
    } 

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    
    if( fp ){
        
        char *label = argv[optind + 1];
        kseq_t *kseq;
        kseq = kseq_init(fp);
        
        int l = 0;
        while ((l = kseq_read(kseq)) >= 0){
           
            ( kseq->qual.l ) ?  fputc('@', stdout) : fputc('>', stdout);
            (before)         ?  fprintf(stdout, "%s%s", label,  kseq->name.s ):
                                fprintf(stdout, "%s%s", kseq->name.s, label);
            
            if (kseq->comment.l) fprintf(stdout, " %s", kseq->comment.s);

            ( kseq->qual.l ) ?  fprintf(stdout, "\n%s\n+\n%s\n", kseq->seq.s, kseq->qual.s):
                                fprintf(stdout, "\n%s\n", kseq->seq.s); 

        }

        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 
    }

    return 0;

}