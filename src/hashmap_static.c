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

    size_t old_cap = hashmap_->cap;
    if (hashmap_->cap > __SIZE_MAX__ / 2) {
        hashmap_->cap = __SIZE_MAX__;
    } else if (!hashmap_->cap) {
        hashmap_->cap = 2;
    } else {
        hashmap_->cap *= 2;
    }

    arr **old_buckets = hashmap_->buckets;   

    hashmap_->buckets = (arr**)realloc(hashmap_->buckets, hashmap_->cap * sizeof(arr*));
    if (hashmap_->buckets == NULL) {
        hashmap_->buckets = old_buckets;
        hashmap_->cap = old_cap;
        
        return 1;
    }

    for (size_t j = old_cap; j < hashmap_->cap; ++j) {
        if ((hashmap_->buckets[j] = arr_init(2)) == NULL) {
            for (size_t k = old_cap; k < j; ++k) {
                arr_free(hashmap_->buckets[k]);
            }

            return 1;
        }
        hashmap_->buckets[j]->free_obj = hashmap_->free_pair;
    }

    pair **pairs = (pair**)malloc(hashmap_->len * sizeof(pair*));
    if (pairs == NULL) {
        return 1;
    }
    
    size_t i = 0;
    for (size_t j = 0; j < old_cap; ++j) {
        while (arr_len(hashmap_->buckets[j]) > 0) {
            pairs[i] = (pair*)arr_pop(hashmap_->buckets[j]);
            i++;  
        }        
    }

    hashmap_->len = 0;

    for (size_t j = 0; j < i; ++j) {
        if (hashmap_put(hashmap_, pairs[j]->key, pairs[j]->value)) {
            return 1;
        }
        free(pairs[j]);
    }

    free(pairs);

    return 0;
}

#endif
