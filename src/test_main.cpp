#include <iostream>
#include "moves_hashmap.h"
#include "dynamic_array.h"
#include "dynamic_string.h"

int main() {
    // ===== 测试动态数组 =====
    std::cout << "Testing dynamic array..." << std::endl;
    initialize_array();
    add_to_array(1);
    add_to_array(2);
    add_to_array(3);
    print_array();  // 期望输出：1 2 3
    clear_array();
    
    // ===== 测试动态字符串 =====
    std::cout << "Testing dynamic string..." << std::endl;
    initialize_string();
    append_to_string("Hello, ");
    append_to_string("world!");
    print_string();  // 期望输出：Hello, world!
    clear_string();
    
    // ===== 测试合法走法哈希表 =====
    std::cout << "Testing moves hashmap..." << std::endl;
    int flips1[] = {2, 4, 6};
    int flips2[] = {1, 3, 5};

    add_move(10, flips1, 3);
    add_move(20, flips2, 3);
    print_moves();  // 期望输出每个位置和翻转列表
    delete_move(10);
    print_moves();  // 期望输出只剩位置 20
    clear_moves();
    
    return 0;
}
