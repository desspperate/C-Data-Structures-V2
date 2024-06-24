#include <gtest/gtest.h>

#include "../src/include/arr.h"

TEST(array, init)
{
    arr *array;

    EXPECT_TRUE((array = arr_init(0)) == NULL);
    
    EXPECT_TRUE((array = arr_init(__SIZE_MAX__)) == NULL);

    for (int i = 1; i < 10000; ++i) {
        EXPECT_TRUE((array = arr_init(i)) != NULL);
        EXPECT_TRUE(array->inner_arr != NULL);
        EXPECT_TRUE(array->len == 0);
        EXPECT_TRUE(array->cap == i);

        free(array->inner_arr);
        free(array);
    }
}

TEST(array, append)
{
    arr *array = arr_init(1);
    int a = 7;
    arr_append(array, &a);
    EXPECT_TRUE(array->len == 1);
}
