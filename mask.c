#include <ctype.h>
#include <wctype.h>
#include "utils.h"

int mask_main(int argc, char *argv[]){
    
    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils base-mask <fasta/q>\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");   
    if(fp){
        kseq_t *kseq;
        kseq = kseq_init(fp);
        int l;

        while ((l = kseq_read(kseq)) >= 0){
            
            int i = 0;
            for (i = 0; i <= kseq->seq.l; ++i){
                
                if( !iswalpha( kseq->seq.s[i] ) ) continue;
                char base = toupper( kseq->seq.s[i] );
                switch(base){
                    case 'A' :
                        break;
                    case 'T' :
                        break;
                    case 'C' :
                        break;
                    case 'G' :
                        break;
                    case 'N' :
                        break;
                    default :
                        kseq->seq.s[i] = 'N';
                }
            }
            print_kseq(kseq, stdout);
        }
        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
    }
   
    return 0;
}