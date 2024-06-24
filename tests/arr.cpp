#include <gtest/gtest.h>

#include "../src/include/arr.h"

TEST(array, init)
{
    arr *array;

    EXPECT_TRUE((array = arr_init(0)) == NULL);
    
    EXPECT_TRUE((array = arr_init(__SIZE_MAX__)) == NULL);

    for (size_t i = 1; i < 10000; ++i) {
        EXPECT_TRUE((array = arr_init(i)) != NULL);
        EXPECT_TRUE(arr_inner(array) != NULL);
        EXPECT_TRUE(arr_len(array) == 0);
        EXPECT_TRUE(arr_cap(array) == i);

        free(array->inner_arr);
        free(array);
    }
}

TEST(array, append)
{
    arr *array = arr_init(1);

    for (size_t i = 0; i < 1000; ++i) {
        int *n = (int*)malloc(sizeof(int));
        *n = i;

        arr_append(array, n);
    }

    for (size_t i = 0; i < array->len; ++i) {
        int *n = (int*)array->inner_arr[i];
        free(n);
    }
    free(array->inner_arr);
    free(array);
}
