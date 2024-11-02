#ifndef MOVES_HASHMAP_H
#define MOVES_HASHMAP_H

#include "uthash.h"     // 引入 uthash.h
#include "utlist.h"     // 引入 utlist.h

// 定义链表节点，用于替代 std::list<int>
struct ListNode {
    int value;
    ListNode *next;
};

// 定义哈希表结构，用于替代 std::unordered_map<int, std::list<int>>
struct Move {
    int position;             // 键：走法位置
    ListNode *flip_list;      // 值：翻转位置链表
    UT_hash_handle hh;        // 必须的哈希表句柄
};

void add_move(int position, int *flips, int flip_count);
Move *find_move(int position);
void delete_move(int position);
void clear_moves();

#endif // MOVES_HASHMAP_H
