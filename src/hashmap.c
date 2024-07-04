#include <malloc.h>

#include "hashmap_static.c"

hashmap *hashmap_init(size_t init_cap, float expand_at, int (*hash_code)(void*))
{
    if (init_cap < 1 || expand_at <= 0.0f || hash_code == NULL) {
        return NULL;
    }

    hashmap *res = (hashmap*)malloc(sizeof(hashmap));
    if (res == NULL) {
        return NULL;
    }

    res->buckets = (arr**)malloc(init_cap * sizeof(arr*));
    if (res->buckets == NULL) {
        free(res);
        return NULL;
    }

    for (size_t i = 0; i < init_cap; ++i) {
        if ((res->buckets[i] = arr_init(2)) == NULL) {
            for (size_t j = 0; i < i; ++i) {
                arr_free(res->buckets[j]);
            }

            free(res->buckets);
            free(res);

            return NULL;
        }
    }

    res->expand_at = expand_at;
    res->len = 0;
    res->cap = init_cap;
    res->hash_code = hash_code;

    return res;
}

int hashmap_put(hashmap *hashmap_, pair pair_)
{
    if (hashmap_ == NULL || hashmap_->buckets == NULL || hashmap_->hash_code == NULL || 
        hashmap_->len > hashmap_->cap || hashmap_->expand_at <= -1) {
        return 1;
    } else if (pair_.key == NULL || pair_.value == NULL) {
        return 1;
    }

    if ((double)hashmap_->len / (double)hashmap_->cap >= hashmap_->expand_at) {
        if (__hashmap_rehash(hashmap_)) {
            return 1;
        }
    }

    return 0;
}
