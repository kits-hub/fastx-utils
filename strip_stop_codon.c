#include "utils.h"

int strip_stop_codon_main(int argc, char *argv[]){
    
    if (argc != 2 ) {
        fprintf(stderr, "\nUsage: fastx-utils strip_stop_codon <fasta>\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[1], "-")? gzopen(argv[1], "r") : gzdopen(fileno(stdin), "r");
    if(fp){
        
        kseq_t *kseq;
        kseq = kseq_init(fp);    
        int l;
        while ((l = kseq_read(kseq)) >= 0){
            
            if(kseq->seq.s[kseq->seq.l - 1] == '*' || kseq->seq.s[kseq->seq.l - 1] == '.'){
                 kseq->seq.s[kseq->seq.l - 1] = '\0';
                 kseq->seq.l -= 1;
            }
            
            int i = 0;
            for (i = 0; i <= kseq->seq.l; ++i)
                if(kseq->seq.s[i] == '*' || kseq->seq.s[i] == '.') kseq->seq.s[i] = 'X';

            printf(">%s", kseq->name.s);
            if(kseq->comment.l > 0) printf(" %s", kseq->comment.s);
            printf("\n%s\n", kseq->seq.s);

        }
        kseq_destroy(kseq);
        gzclose(fp);
    }else
       fprintf(stderr, "[ERR]: can't open file %s\n", argv[1]);

    return 0;
}
