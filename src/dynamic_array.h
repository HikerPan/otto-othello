#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H


#ifdef __cplusplus
extern "C" {
#endif
#include "utarray.h"  // 引入 utarray.h

#ifdef __cplusplus
}
#endif
void initialize_array();
void add_to_array(int value);
void print_array();
void clear_array();
void swapUTArray(UT_array **positions, UT_array **setup);
int count_in_utarray(UT_array *array, int target);
#endif