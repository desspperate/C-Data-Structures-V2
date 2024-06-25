#include <malloc.h>
#include <stddef.h>

#include "arr_static.c"

arr *arr_init(size_t initial_cap)
{
    if (initial_cap < 1) {
        return NULL;
    }

    arr *res = (arr*)malloc(sizeof(arr));
    if (res == NULL) {
        return NULL;
    }

    res->inner_arr = (void**)malloc(initial_cap * sizeof(void*));
    if (res->inner_arr == NULL) {
        free(res);
        return NULL;
    }

    res->len = 0;
    res->cap = initial_cap;

    return res;
}

void **arr_inner(arr *array)
{
    if (array == NULL) {
        return NULL;
    }

    return array->inner_arr;
}

size_t arr_len(arr* array)
{
    if (array == NULL || arr_inner(array) == NULL) {
        return 0;
    }

    return array->len;
}

size_t arr_cap(arr* array)
{
    if (array == NULL || arr_inner(array) == NULL) {
        return 0;
    }

    return array->cap;
}

int arr_append(arr *array, void *obj)
{
    if (array == NULL || arr_inner(array) == NULL || obj == NULL) {
        return 1;
    }

    if (arr_len(array) == arr_cap(array)) {
        if (__arr_expand(array)) {
            return 1;
        }
    } else if (arr_len(array) > arr_cap(array)) {
        return 1;
    }

    array->inner_arr[arr_len(array)] = obj;
    array->len++;

    return 0;
}

void *arr_get(arr *array, size_t index)
{
    if (array == NULL || arr_inner(array) == NULL) {
        return NULL;
    }

    if (index >= arr_len(array)) {
        return NULL;
    }

    return arr_inner(array)[index];
}

int arr_free(arr *array)
{
    if (array == NULL || arr_inner(array) == NULL) {
        return 1;
    }

    free(arr_inner(array));
    free(array);

    return 0;
}

int  arr_super_free(arr *array, int (*free_obj)(void*))
{
    if (array == NULL || arr_inner(array) == NULL || free_obj == NULL) {
        return 1;
    }

    for (size_t i = 0; i < arr_len(array); ++i) {
        if (free_obj(arr_get(array, i))) {
            return 1;
        }
    }

    return arr_free(array);
}
