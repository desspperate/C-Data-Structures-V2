#ifndef _DSP_HASHMAP_STATIC_C
#define _DSP_HASHMAP_STATIC_C

#include <stdio.h>
#include <malloc.h>

#include "include/hashmap.h"

static int __hashmap_rehash(hashmap *hashmap_)
{
    if (hashmap_ == NULL || hashmap_->buckets == NULL || hashmap_->hash_code == NULL) {
        return 1;
    }

    pair *pairs = (pair*)malloc(hashmap_->len * sizeof(pair));
    size_t i = 0;
    for (size_t j = 0; j < hashmap_->cap; ++j) {
        for (size_t k = 0; k < arr_len(hashmap_->buckets[j]); ++k) {
            pairs[i] = *(pair*)arr_get(hashmap_->buckets[j], k);
            i++;
        }
    }   

    for (size_t j = 0; j < i; j++) {
        printf("KEY: %s VALUE: %s\n", (char*)(pairs[j].key), (char*)(pairs[j].value));
    }

    return 0;
}

#endif
