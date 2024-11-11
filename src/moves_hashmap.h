#ifndef MOVES_HASHMAP_H
#define MOVES_HASHMAP_H

#include "uthash.h"     // 引入 uthash.h
#include "utlist.h"     // 引入 utlist.h

// 定义链表节点，用于替代 std::list<int>
struct IntListNode {
    int flip_position;      // 翻转位置
    IntListNode *next;
};

typedef struct moves_pair_t
{
    int position;             // 键：走法位置
    IntListNode *flip_list;      // 值：翻转位置链表
}MOVES_PAIR_T;

// 定义哈希表结构，用于替代 std::unordered_map<int, std::list<int>>
struct MoveHash {
    int position;             // 键：走法位置
    // IntListNode *flip_list;      // 值：翻转位置链表
    MOVES_PAIR_T moves_pair; 
    UT_hash_handle hh;        // 必须的哈希表句柄
};



int move_hash_empty(MoveHash *head);
int flip_list_empty(IntListNode *head);
void list_push_front(IntListNode **head, int value);
MOVES_PAIR_T *find_begin(MoveHash *moves)
MOVES_PAIR_T *find_end(MoveHash *moves);
void add_move(MoveHash *moves, int position, int *flips, int flip_count);
MOVES_PAIR_T *find_move(MoveHash *moves, int position);
void delete_move(MoveHash *moves, int position);
void clear_moves(MoveHash *moves);
void print_moves(MoveHash *moves);
// void insert(MoveHash **hashTable, int key, int value);
void merge_flip_lists(IntListNode **dest_list, IntListNode *source_list);
void merge(MoveHash **targetHashTable, MoveHash *sourceHashTable);
void insert_moves(MoveHash *hashTable, MOVES_PAIR_T *moves_node);
int size_moves(MoveHash *hashTable);
#endif // MOVES_HASHMAP_H
