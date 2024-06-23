#include <stdlib.h>
#include <stdio.h>

#include "../src/include/arr.h"

int main()
{
    arr *array = arr_init(1);

    for (int i = 0; i < 5000; ++i) {
        int* a = (int*)malloc(sizeof(int));
        *a = i;
        if (arr_append(array, a)) {
            printf("arr_append() error\n");
        }
    }

    for (int i = 0; i < 5000; ++i) {
        void *obj = arr_get(array, i);
        if (obj == NULL) {
            printf("arr_get(%d) error\n", i);
        } else {
            free(obj);
        }
    }

    return 0;
}