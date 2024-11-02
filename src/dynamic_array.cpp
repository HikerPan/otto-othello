
#include "stdio.h"
#include "dynamic_array.h"

// 声明一个动态数组
UT_array *array;

void initialize_array() {
    // 初始化数组，存储 int 类型数据
    utarray_new(array, &ut_int_icd);
}

void add_to_array(int value) {
    utarray_push_back(array, &value);
}

void print_array() {
    int *p = nullptr;
    for (p = (int*)utarray_front(array); p != nullptr; p = (int*)utarray_next(array, p)) {
        printf("%d ",*p);
    }
    printf("\n");
}

void clear_array() {
    utarray_clear(array);  // 清空数组
    utarray_free(array);   // 释放数组内存
}
