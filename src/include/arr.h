#ifndef _DSP_ARR_H
#define _DSP_ARR_H

#include <stddef.h>

struct arr {
    void   **inner_arr;
    size_t   len;
    size_t   cap;
};

typedef struct arr arr;

extern arr *arr_init(size_t initial_cap);

extern void **arr_inner(arr *array);

extern size_t arr_len(arr *array);

extern size_t arr_cap(arr *array);

extern int arr_append(arr *array, void *obj);

extern void *arr_get(arr *array, size_t index);

#endif
