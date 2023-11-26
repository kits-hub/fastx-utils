#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>

#include "kstring.h"
#include "khash.h"
#include "kseq.h"

KHASH_MAP_INIT_INT(int64, int64_t *)
void kh_int64_destroy(khash_t(int64) *h);

KHASH_MAP_INIT_STR(reg, char *)
void kh_reg_destroy(khash_t(reg) *h);

KHASH_MAP_INIT_STR(reg32,  int)
void kh_reg32_destroy(khash_t(reg32) *h);

KHASH_SET_INIT_STR(set)
void kh_set_destroy(khash_t(set) *h);

KHASH_MAP_INIT_STR(kreg, kstring_t)
void kh_kreg_destroy(khash_t(kreg) *h);

KSEQ_INIT(gzFile, gzread)
void print_kseq(kseq_t *s, FILE *file);

void rev_com(kstring_t  *ks);
void rev (kstring_t  *ks);