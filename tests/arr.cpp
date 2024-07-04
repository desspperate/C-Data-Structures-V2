#include <gtest/gtest.h>

#include "../src/include/arr.h"
#include "../src/arr_static.c"

TEST(array, init)
{
    arr *array;

    EXPECT_TRUE((array = arr_init(0)) == NULL);
    
    EXPECT_TRUE((array = arr_init(__SIZE_MAX__)) == NULL);

    for (size_t i = 1; i < 10000; ++i) {
        EXPECT_TRUE((array = arr_init(i)) != NULL);

        EXPECT_TRUE(arr_inner(array) != NULL);

        EXPECT_EQ(arr_len(array), 0);

        EXPECT_EQ(arr_cap(array), i);

        arr_free(array);
    }
}

TEST(array, inner)
{
    EXPECT_TRUE(arr_inner(NULL) == NULL);

    arr *array = arr_init(1);

    EXPECT_TRUE(arr_inner(array) == array->inner_arr);

    arr_free(array);
}

TEST(array, len)
{
    EXPECT_EQ(arr_len(NULL), 0);

    arr *array = arr_init(1);

    EXPECT_EQ(arr_len(array), array->len);

    free(arr_inner(array));
    array->inner_arr = NULL;

    EXPECT_EQ(arr_len(array), 0);

    free(array);
}

TEST(array, cap)
{
    EXPECT_EQ(arr_cap(NULL), 0);

    arr *array = arr_init(1);

    EXPECT_EQ(arr_cap(array), array->cap);

    free(arr_inner(array));
    array->inner_arr = NULL;

    EXPECT_EQ(arr_cap(array), 0);

    free(array);
}

int free_int(void *int_ptr)
{
    if (int_ptr == NULL) {
        return 1;
    }

    free(int_ptr);

    return 0;
}

TEST(array, __expand)
{
    EXPECT_TRUE(__arr_expand(NULL));
 
    arr *array = arr_init(1);
    
    int *n = (int*)malloc(sizeof(int));
    *n = 10;
    arr_append(array, n);

    size_t cap_prediction = arr_cap(array) * 2;

    while (!__arr_expand(array)) {
        EXPECT_EQ(arr_cap(array), cap_prediction);
        cap_prediction = arr_cap(array) * 2;
    }

    EXPECT_EQ(arr_cap(array), cap_prediction / 2);
    
    EXPECT_EQ(*(int*)arr_get(array, 0), 10);

    arr_free(array);

    array = arr_init(1);

    free(arr_inner(array));
    array->inner_arr = NULL;

    EXPECT_TRUE(arr_append(array, n));

    free(n);
    free(array);

    array = (arr*)malloc(sizeof(arr));
    array->len = 0;
    array->cap = 0;
    array->inner_arr = (void**)malloc(arr_cap(array) * sizeof(void*));

    EXPECT_TRUE(!__arr_expand(array));
    EXPECT_EQ(arr_cap(array), 1);

    arr_free(array);

    array = arr_init(10);

    EXPECT_TRUE(!__arr_expand(array));
    EXPECT_TRUE(!__arr_expand(array));
    EXPECT_TRUE(!__arr_expand(array));

    EXPECT_EQ(arr_len(array), 0);
    EXPECT_EQ(arr_cap(array), 80);

    for (size_t i = 0; i < arr_cap(array); ++i) {
        n = (int*)malloc(sizeof(int));
        *n = i;

        EXPECT_EQ(arr_append(array, n), 0);
        EXPECT_EQ(*(int*)arr_get(array, i), i);
    }

    EXPECT_EQ(arr_len(array), 80);
    EXPECT_EQ(arr_cap(array), 80);

    array->free_obj = free_int;
    arr_super_free(array);
}

TEST(array, append)
{
    EXPECT_TRUE(arr_append(NULL, NULL));

    arr *array = arr_init(1);

    EXPECT_TRUE(arr_append(array, NULL));

    EXPECT_TRUE(arr_append(NULL, array));
    
    int *n = (int*)malloc(sizeof(int));
    *n = 10;

    array->len = arr_cap(array) + 1;
    EXPECT_EQ(arr_append(array, n), 1);
    array->len = 0;

    free(arr_inner(array));
    array->inner_arr = NULL;

    EXPECT_TRUE(arr_append(array, n));
    free(n);

    free(array);
    
    array = arr_init(1);

    for (size_t i = 0; i < 800; ++i) {
        n = (int*)malloc(sizeof(int));
        *n = i;

        EXPECT_EQ(arr_append(array, n), 0);

        EXPECT_TRUE(arr_len(array) <= arr_cap(array));

        for (size_t j = 0; j <= i; j++) {
            EXPECT_EQ(*(int*)arr_get(array, j), j);
        }
    }

    array->free_obj == free_int;
    arr_super_free(array);
}

TEST(array, get)
{
    EXPECT_TRUE(arr_get(NULL, 0) == NULL);

    arr *array = arr_init(1);

    EXPECT_TRUE(arr_get(array, 0) == NULL);
    EXPECT_TRUE(arr_get(array, 1) == NULL);
    EXPECT_TRUE(arr_get(array, 2) == NULL);

    for (size_t i = 0; i < 1000; ++i) {
        int *n = (int*)malloc(sizeof(int));
        *n = i;

        arr_append(array, n);
        EXPECT_EQ(*(int*)(arr_get(array, i)), i);
        EXPECT_EQ(*(int*)(arr_get(array, i)), *(int*)arr_inner(array)[i]);

        free(n);
    }

    free(arr_inner(array));
    array->inner_arr = NULL;

    EXPECT_TRUE(arr_get(array, 0) == NULL);
    EXPECT_TRUE(arr_get(array, 1) == NULL);
    EXPECT_TRUE(arr_get(array, 2) == NULL);

    free(array);
}

TEST(array, free)
{
    EXPECT_EQ(arr_free(NULL), 1);

    arr *array = arr_init(200);

    EXPECT_EQ(arr_free(array), 0);

    array = arr_init(200);

    free(arr_inner(array));
    array->inner_arr = NULL;

    EXPECT_EQ(arr_free(array), 1);
}

TEST(array, clear)
{
    EXPECT_EQ(arr_clear(NULL), 1);

    arr *array = arr_init(200);

    EXPECT_EQ(arr_clear(array), 1);
    
    for (size_t i = 0; i < 200; ++i) {
        int *n = (int*)malloc(sizeof(int));
        *n = i;

        arr_append(array, n);
    }

    array->free_obj = free_int;
    EXPECT_EQ(arr_clear(array), 0);
    
    arr_free(array);

    array = arr_init(200);

    free(arr_inner(array));
    array->inner_arr = NULL;

    EXPECT_EQ(arr_clear(array), 1);
}

TEST(array, super_free)
{
    EXPECT_EQ(arr_super_free(NULL), 1);

    arr *array = arr_init(200);

    for (size_t i = 0; i < 200; ++i) {
        int *n = (int*)malloc(sizeof(int));
        *n = i;

        arr_append(array, n);
    }

    EXPECT_EQ(arr_super_free(array), 1);

    array->free_obj = free_int;
    EXPECT_EQ(arr_super_free(array), 0);

    array = arr_init(200);

    free(arr_inner(array));
    array->inner_arr = NULL;

    EXPECT_EQ(arr_super_free(array), 1);
}

TEST(array, buble_sort)
{
    arr *array = arr_init(1);

    EXPECT_TRUE(array != NULL);
    EXPECT_TRUE(arr_inner(array) != NULL);
    EXPECT_EQ(arr_len(array), 0);
    EXPECT_EQ(arr_cap(array), 1);

    int *a, *b, *c, *d, *e, *f;
    a = (int*)malloc(sizeof(int));
    b = (int*)malloc(sizeof(int));
    c = (int*)malloc(sizeof(int));
    d = (int*)malloc(sizeof(int));
    e = (int*)malloc(sizeof(int));
    f = (int*)malloc(sizeof(int));
    *a = 2;
    *b = 4;
    *c = 2;
    *d = 7;
    *e = 3;
    *f = 6;
    EXPECT_TRUE(!arr_append(array, a));
    EXPECT_TRUE(!arr_append(array, b));
    EXPECT_TRUE(!arr_append(array, c));
    EXPECT_TRUE(!arr_append(array, d));
    EXPECT_TRUE(!arr_append(array, e));
    EXPECT_TRUE(!arr_append(array, f));

    EXPECT_TRUE(array != NULL);
    EXPECT_TRUE(arr_inner(array) != NULL);
    EXPECT_EQ(arr_len(array), 6);
    EXPECT_EQ(arr_cap(array), 8);

    for (size_t i = 0; i < arr_len(array); ++i) {
        for (size_t j = 0; j < arr_len(array) - i - 1; ++j) {
            if (*(int*)arr_get(array, j) > *(int*)arr_get(array, j + 1)) {
                int *tmp = (int*)arr_get(array, j);

                arr_inner(array)[j] = arr_get(array, j + 1);

                arr_inner(array)[j + 1] = tmp;
            }
        }
    }

    EXPECT_EQ(*(int*)arr_get(array, 0), 2);
    EXPECT_EQ(*(int*)arr_get(array, 1), 2);
    EXPECT_EQ(*(int*)arr_get(array, 2), 3);
    EXPECT_EQ(*(int*)arr_get(array, 3), 4);
    EXPECT_EQ(*(int*)arr_get(array, 4), 6);
    EXPECT_EQ(*(int*)arr_get(array, 5), 7);

    array->free_obj = free_int;
    EXPECT_EQ(arr_super_free(array), 0);
}
