#ifndef _DSP_HASHMAP_H
#define _DSP_HASHMAP_H

#include "arr.h"

typedef struct hashmap {
    arr     **buckets;
    double    expand_at;
    size_t    len;
    size_t    cap;
    int       (*hash_code)(void*);
} hashmap;

typedef struct pair {
    void *key;
    void *value;
} pair;

extern hashmap *hashmap_init(size_t init_cap, float expand_at, int (*hash_code)(void*));

extern int hashmap_put(hashmap *hashmap_, pair pair_);

#endif
