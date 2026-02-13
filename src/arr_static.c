#ifndef _DSP_ARR_STATIC_C
#define _DSP_ARR_STATIC_C

#include "include/arr.h"

static int __arr_expand(arr *array)
{
    if (array == NULL || arr_inner(array) == NULL) {
        return 1;
    }

    void   **old_inner = arr_inner(array);
    size_t   old_cap = arr_cap(array);
    if (arr_cap(array) > __SIZE_MAX__ / 2) {
        array->cap = __SIZE_MAX__;
    } else if (arr_cap(array) == 0) {
        array->cap = 1;
    } else {
        array->cap *= 2;
    }

    array->inner_arr = (void**)realloc(arr_inner(array), arr_cap(array) * sizeof(void*));
    if (arr_inner(array) == NULL) {
        array->inner_arr = old_inner;
        array->cap = old_cap;
        return 1;
    }

    return 0;
}

#endif
