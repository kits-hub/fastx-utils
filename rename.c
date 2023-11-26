#include "utils.h"

int rename_main(int argc, char *argv[]){
  
    int retain, mapping, c;
    retain = mapping = 0;

    while ((c = getopt(argc, argv, "rm")) >= 0) {
        if (c == 'r')      retain  = 1;
        else if (c == 'm') mapping = 1;
    }

    if ( optind == argc || argc != optind + 2) {

        fprintf(stderr, "\nUsage: fastx-utils rename [options] <fasta/q> <prefix>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "    -r  retain original sequence identifier.\n");
        fprintf(stderr, "    -m  print the mapping info to stderr streaming.\n\n");
        return 1;
    } 

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    
    if( fp ){
        
        char *prefix = argv[optind + 1];
        int64_t index=0;
        kseq_t *kseq;
        kseq = kseq_init(fp);
        
        int l = 0;
        while ((l = kseq_read(kseq)) >= 0){
            
            (kseq->qual.l > 0) ?  fputc('@', stdout)  :  fputc('>', stdout);
            
            if(retain)
                fprintf(stdout, "%s_%zu %s", prefix, ++index, kseq->name.s);
            else
                fprintf(stdout, "%s_%zu", prefix, ++index);

            if(kseq->comment.l > 0) fprintf(stdout, " %s", kseq->comment.s);

            fprintf(stdout, "\n%s\n", kseq->seq.s);
            if(mapping) fprintf(stderr,  "%s\t%s_%zu\n",  kseq->name.s, prefix, index);

            if(kseq->qual.l > 0) printf("+\n%s\n", kseq->qual.s);

        }

        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ]);
        exit(-1); 
    }

    return 0;

}