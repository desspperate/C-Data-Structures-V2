#include <malloc.h>
#include <string.h>

#include "hashmap_static.c"

hashmap *hashmap_init(size_t init_cap, float expand_at, int (*hash_code)(void*), int (*keys_eq)(void*, void*), int (*free_pair)(void*))
{
    if (init_cap < 1 || expand_at <= 0.0f || hash_code == NULL || keys_eq == NULL || free_pair == NULL) {
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
            for (size_t j = 0; j < i; ++j) {
                arr_free(res->buckets[j]);
            }

            free(res->buckets);
            free(res);

            return NULL;
        }
        res->buckets[i]->free_obj = free_pair;
    }

    res->expand_at = expand_at;
    res->len = 0;
    res->cap = init_cap;
    res->hash_code = hash_code;
    res->keys_eq = keys_eq;
    res->free_pair = free_pair;

    return res;
}

int hashmap_put(hashmap *hashmap_, void *key, void *val)
{
    if (hashmap_ == NULL || hashmap_->buckets == NULL || hashmap_->hash_code == NULL || hashmap_->keys_eq == NULL || 
        hashmap_->len > hashmap_->cap || hashmap_->cap == 0 || hashmap_->expand_at <= 0.0 || hashmap_->expand_at > 1.0) {
        return 1;
    }

    if ((double)hashmap_->len / (double)hashmap_->cap >= hashmap_->expand_at) {
        if (__hashmap_rehash(hashmap_)) {
            return 1;
        }
    } 

    int hash;

    if ((hash = hashmap_->hash_code(key)) == -1) {
        return 1;
    }

    hash %= hashmap_->cap;

    for (size_t i = 0; i < arr_len(hashmap_->buckets[hash]); ++i) {
        pair *tmp_pair;

        if ((tmp_pair = (pair*)arr_get(hashmap_->buckets[hash], i)) == NULL) {
            return 1;
        }
 
        int keys_eq;

        if ((keys_eq = hashmap_->keys_eq(tmp_pair->key, key)) == -1) {
            return 1;
        }

        if (keys_eq) {
            pair *p = (pair*)malloc(sizeof(pair));
            p->key = key;
            p->value = tmp_pair->value;

            if (hashmap_->free_pair(p)) {
                free(p);
                return 1;
            }
            
            tmp_pair->value = val;
            
            return 0;
        }
    }
    
    pair *p = (pair*)malloc(sizeof(pair));
    p->key = key;
    p->value = val;

    if (arr_append(hashmap_->buckets[hash], p)) {
        return 1;
    }
    hashmap_->len++;

    return 0;
}

void *hashmap_get(hashmap *hashmap_, void *key)
{
    if (hashmap_ == NULL || hashmap_->buckets == NULL || hashmap_->hash_code == NULL || hashmap_->keys_eq == NULL || 
        hashmap_->len > hashmap_->cap || hashmap_->cap == 0 || hashmap_->expand_at <= 0.0 || hashmap_->expand_at > 1.0) {
        return NULL;
    }

    int hash;

    if ((hash = hashmap_->hash_code(key)) == -1) {
        return NULL;
    }

    hash %= hashmap_->cap;

    for (size_t i = 0; i < arr_len(hashmap_->buckets[hash]); ++i) {
        pair *tmp_pair;

        if ((tmp_pair = (pair*)arr_get(hashmap_->buckets[hash], i)) == NULL) {
            return NULL;
        }

        int keys_eq;

        if ((keys_eq = hashmap_->keys_eq(tmp_pair->key, key)) == -1) {
            return NULL;
        }

        if (keys_eq) {
            return tmp_pair->value;
        }
    }

    return NULL;
}

int hashmap_free(hashmap *hashmap_)
{
    if (hashmap_ == NULL || hashmap_->buckets == NULL || hashmap_->hash_code == NULL || hashmap_->keys_eq == NULL || 
        hashmap_->len > hashmap_->cap || hashmap_->cap == 0 || hashmap_->expand_at <= 0.0 || hashmap_->expand_at > 1.0) {
        return 1;
    }

    for (size_t i = 0; i < hashmap_->cap; ++i) {
        if (arr_free(hashmap_->buckets[i])) {
            return 1;
        }
    }
    free(hashmap_->buckets);
    free(hashmap_);

    return 0;
}

int hashmap_clear(hashmap *hashmap_)
{
    if (hashmap_ == NULL || hashmap_->buckets == NULL || hashmap_->hash_code == NULL || hashmap_->keys_eq == NULL || 
        hashmap_->len > hashmap_->cap || hashmap_->cap == 0 || hashmap_->expand_at <= 0.0 || hashmap_->expand_at > 1.0) {
        return 1;
    }

    for (size_t i = 0; i < hashmap_->cap; ++i) {
        if (arr_clear(hashmap_->buckets[i])) {
            return 1;
        }
    }

    return 0;
}

int hashmap_super_free(hashmap *hashmap_)
{
    if (hashmap_ == NULL || hashmap_->buckets == NULL || hashmap_->hash_code == NULL || hashmap_->keys_eq == NULL || 
        hashmap_->len > hashmap_->cap || hashmap_->cap == 0 || hashmap_->expand_at <= 0.0 || hashmap_->expand_at > 1.0) {
        return 1;
    }

    if (hashmap_clear(hashmap_)) {
        return 1;
    }

    return hashmap_free(hashmap_);
}
