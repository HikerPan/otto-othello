#include "stdio.h"
#include "dynamic_string.h"

UT_string *my_string;

void initialize_string() {
    utstring_new(my_string);  // 初始化动态字符串
    utstring_printf(my_string, "Hello, World!");  // 写入内容
}

void append_to_string(const char *text) {
    utstring_printf(my_string, "%s", text);  // 拼接字符串
}

void print_string() {
    // std::cout << utstring_body(my_string) << std::endl;
    printf("%s\n",utstring_body(my_string));
}

void clear_string() {
    utstring_clear(my_string);  // 清空内容
    utstring_free(my_string);   // 释放内存
}
