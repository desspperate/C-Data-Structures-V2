#include <malloc.h>
#include <stddef.h>

struct arr {
    void   **inner_arr;
    size_t   len;
    size_t   cap;
};

typedef struct arr arr;

static int __arr_expand(arr *array)
{
    if (array->cap > __SIZE_MAX__ / 2) {
        array->cap = __SIZE_MAX__;
    } else if (array->cap == 0) {
        array->cap = 2;
    } else {
        array->cap *= 2;
    }

    array->inner_arr = (void**)realloc(array->inner_arr, array->cap * sizeof(void*));
    if (array->inner_arr == NULL) {
        return 1;
    }

    return 0;
}

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

int arr_append(arr *array, void *obj)
{
    if (array->len == array->cap) {
        if (__arr_expand(array)) {
            return 1;
        }
    } else if (array->len > array->cap) {
        return 1;
    }

    array->inner_arr[array->len] = obj;
    array->len += 1;

    return 0;
}

void *arr_get(arr *array, size_t index)
{
    if (index >= array->len) {
        return NULL;
    }

    return array->inner_arr[index];
}