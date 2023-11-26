#include "utils.h"

int individ_main(int argc, char *argv[]){

    if ( optind == argc || argc != optind + 2) {
        fprintf(stderr, "\nUsage: individ <fasta> <fd>\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");
    if(fp){
        
        int     l;
        kstring_t fn = {0,0,0};
        FILE *output = NULL;
      
        char *fd = argv[ optind + 1 ];
        kseq_t *kseq = kseq_init(fp);    

        while ((l = kseq_read(kseq)) >= 0){
            
            fn.l = 0;
            ksprintf(&fn,"%s/%s.fasta", fd, kseq->name.s);  
            if ((output = fopen(fn.s, "w")) == NULL) {
                fprintf(stderr,"[ERR]: can't open file %s\n", fn.s);
                exit(1);
            }
            
            print_kseq(kseq, output);
            
            fclose(output);
        }

        free(fn.s);
        kseq_destroy(kseq);
        gzclose(fp);
    
    }
    return 0;
}
