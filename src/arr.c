#include <malloc.h>
#include <stddef.h>

struct arr {
    void   **inner_arr;
    size_t   len;
    size_t   cap;
};

typedef struct arr arr;

arr *arr_init(size_t initial_cap)
{
    arr *res = (arr*)malloc(sizeof(arr));
    res->inner_arr = (void**)malloc(initial_cap * sizeof(void*));
    res->len = 0;
    res->cap = initial_cap;

    if (res->inner_arr == NULL) {
        if (res != NULL) {
            free(res);
        }
        return NULL;
    }

    return res;
}

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

int arr_append(arr *array, void *obj)
{
    if (array->len == array->cap) {
        if (__arr_expand(array)) {
            return 1;
        }
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