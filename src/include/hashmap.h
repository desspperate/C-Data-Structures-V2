#ifndef _DSP_HASHMAP_H
#define _DSP_HASHMAP_H

#include "arr.h"

typedef struct pair {
    void *key;
    void *value;
} pair;

typedef struct hashmap {
    arr     **buckets;
    double    expand_at;
    size_t    len;
    size_t    cap;
    int       (*hash_code)(void*);
    int       (*keys_eq)(void*, void*);
    int       (*free_pair)(void*);
} hashmap;

extern hashmap *hashmap_init(size_t init_cap, float expand_at, int (*hash_code)(void*), int (*keys_eq)(void*, void*), int (*free_pair)(void*));

extern int hashmap_put(hashmap *hashmap_, void *key, void *val);

extern void *hashmap_get(hashmap *hashmap_, void *key);

extern int hashmap_free(hashmap *hashmap_);

extern int hashmap_clear(hashmap *hashmap_);

extern int hashmap_super_free(hashmap *hashmap_);

#endif
