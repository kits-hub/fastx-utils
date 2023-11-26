#include "utils.h"

int view_main(int argc, char *argv[]){

    char *addtion = NULL;
    int c, length, sequence, comment;
    length = sequence = comment = 0;

    while ((c = getopt(argc, argv, "a:cls")) >= 0) {
        
        if (c == 'c')      comment  = 1;
        else if(c == 'l')  length   = 1;
        else if(c == 's')  sequence = 1;
        else if(c == 'a')  addtion  = optarg;

    }

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils view [options] <fasta/q>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -a STR   print addtional fileds\n");
        fprintf(stderr, "  -c       print comment fileds\n");
        fprintf(stderr, "  -l       print length fileds\n");
        fprintf(stderr, "  -s       print sequence fileds\n\n");
        return 1;
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    if(fp){

        kseq_t *kseq;
        kseq = kseq_init(fp);
        kstring_t kt = {0,0,0};

        int l;
        while ((l = kseq_read(kseq)) >= 0){
            
            kt.l  =  0;
            kputs(kseq->name.s, &kt);

            if(comment){
                if(kseq->comment.l > 0)
                   ksprintf(&kt, "\t%s", kseq->comment.s);
                else
                   ksprintf(&kt, "\tNULL"); 
            }  
            if(length)   ksprintf(&kt, "\t%d", (int)kseq->seq.l);
            if(sequence) ksprintf(&kt, "\t%s", kseq->seq.s);
            
            if(addtion== NULL) puts(kt.s);
            else fprintf(stdout, "%s\t%s\n", addtion, kt.s);

        }

        free(kt.s);
        kseq_destroy(kseq);
        gzclose(fp);
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind]);
        exit(-1); 
    }

    return 0;
}
