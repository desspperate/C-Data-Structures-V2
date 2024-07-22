#include <gtest/gtest.h>
#include <string.h>
#include <time.h>

#include "../src/include/hashmap.h"
#include "../src/hashmap_static.c"

int str_hash(void *str)
{
    if (str == NULL) {
        return -1;
    }

    int res = 0;

    for (size_t i = 0; i < strlen((char*)str); ++i) {
        res += ((char*)str)[i] * (i + 1);
    }

    return res;
}

int str_eq(void *str1, void *str2)
{
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }

    return !strcmp((char*)str1, (char*)str2);
}

int free_pair(void* p_)
{
    pair *p = (pair*)p_;
    if (p == NULL || p->key == NULL || p->value == NULL) {
        return 1;
    }

    free(p->key);
    free(p->value);
    free(p);

    return 0;
}

TEST(hashmap, init)
{
    srand(time(NULL));

    hashmap *map = hashmap_init(2, 0.75, str_hash, str_eq, free_pair);

    for (size_t i = 0; i < 10000; ++i) {
        char *key = (char*)malloc(5 * sizeof(char));
        key[0] = rand() % 26 + 'a';
        key[1] = rand() % 26 + 'a';
        key[2] = rand() % 26 + 'a';
        key[3] = rand() % 26 + 'a';
        key[4] = 0;

        char *val = (char*)malloc(5 * sizeof(char));
        val[0] = rand() % 10 + '0';
        val[1] = rand() % 10 + '0';
        val[2] = rand() % 10 + '0';
        val[3] = rand() % 10 + '0';
        val[4] = 0;

        pair *p = (pair*)malloc(sizeof(pair));
        p->key = key;
        p->value = val;

        if (hashmap_put(map, p)) {
            printf("Put error!");
            exit(EXIT_FAILURE);
        }
    }

    printf("Hashmap state | len: %zu, cap: %zu, expand_at: %f\n", map->len, map->cap, map->expand_at);
    printf("Internal function addresses | hash_code: %p, keys_eq: %p, free_pair: %p\n", map->hash_code, map->keys_eq, map->free_pair);
    for (size_t i = 0; i < map->cap; ++i) {
        if (arr_len(map->buckets[i]) > 100) {
            printf("Bucket number %zu contains:\n", i + 1);
            for (size_t j = 0; j < arr_len(map->buckets[i]); ++j) {
                printf("%zu. key: '%s' value: '%s'\n", j + 1, (char*)((pair*)arr_get(map->buckets[i], j))->key, (char*)((pair*)arr_get(map->buckets[i], j))->value);
            }
        }
    }

    for (size_t i = 0; i < map->cap; ++i) {
        if (arr_super_free(map->buckets[i])) {
            puts("Free eror!\n");
            exit(EXIT_FAILURE);
        }
    }
    free(map->buckets);
    free(map);
}
