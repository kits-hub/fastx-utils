#include "utils.h"

int revcomp_main(int argc, char *argv[]){
        
    if ( argc != 2 ) {

        fprintf(stderr, "\nUsage: fastx-utils revcomp <sequence>\n\n");
        return 1;
    }

    kstring_t kt={0, 0, 0};
    kputs(argv[1], &kt);
    rev_com(&kt);
    printf("%s\n", kt.s);
    free(kt.s);
    return 0;

}