#include <gtest/gtest.h>
#include <string.h>

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

TEST(hashmap, init)
{
    hashmap *map = hashmap_init(2, 0.75, str_hash);

    char *key = (char*)malloc(4 * sizeof(char));
    key[0] = 'k';
    key[1] = 'e';
    key[2] = 'y';
    key[3] = 0;

    char *val = (char*)malloc(4 * sizeof(char));
    val[0] = 'v';
    val[1] = 'a';
    val[2] = 'l';
    val[3] = 0;

    pair p = { key, val };

    arr_append(map->buckets[0], &p);
    map->len++;

    __hashmap_rehash(map);
}
