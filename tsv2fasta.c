#include "utils.h"

int tsv2fasta_main (int argc, char *argv[]){

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: fastx-utils tsv2fasta <tsv>\n\n");
        return 1;    
    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen( argv[optind], "r" ) : gzdopen(fileno(stdin), "r"); 

    if(fp){

        kstring_t kt = {0, 0, 0};

        kstream_t *ks;
        ks = ks_init(fp);        
        int *fields, n;

        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
           
           if( kt.l == 0 || kt.s[0] == '#') continue;
           fields = ksplit(&kt, '\t' , &n);
           printf(">%s\n%s\n", kt.s, kt.s + fields[1]);

        }

        free(kt.s);
        ks_destroy(ks);
        gzclose(fp);
    
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ]);
        exit(-1);
    }
    
    return 0;
}
