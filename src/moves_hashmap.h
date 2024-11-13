#ifndef MOVES_HASHMAP_H
#define MOVES_HASHMAP_H

#include "uthash.h"     // 引入 uthash.h
#include "utlist.h"     // 引入 utlist.h

// 定义链表节点，用于替代 std::list<int>
typedef struct _IntListNode_ {
    int flip_position;      // 翻转位置
    struct _IntListNode_ *next;
}IntListNode_t;

typedef struct _MovePair_t
{
    int position;             // 键：走法位置
    IntListNode_t *flip_list;      // 值：翻转位置链表
}MovePair_t;

// 定义哈希表结构，用于替代 std::unordered_map<int, std::list<int>>
typedef struct _move_hash_t_ {
    int position;             // 键：走法位置
    MovePair_t moves_pair; 
    UT_hash_handle hh;        // 必须的哈希表句柄
}MoveHash_t;



int move_hash_empty(MoveHash_t *head);
int flip_list_empty(IntListNode_t *head);
void list_push_front(IntListNode_t **head, int value);
MovePair_t *find_begin(MoveHash_t *moves);
MovePair_t *find_end(MoveHash_t *moves);
void add_move(MoveHash_t *moves, int position, int *flips, int flip_count);
MovePair_t *find_move(MoveHash_t *moves, int position);
void delete_move(MoveHash_t *moves, int position);
void clear_moves(MoveHash_t *moves);
void print_moves(MoveHash_t *moves);
int merge_flip_lists(IntListNode_t **dest_list, IntListNode_t *source_list);
void insert_moves(MoveHash_t **hashTable, MovePair_t *moves_node);
int size_moves(MoveHash_t *hashTable);
#endif // MOVES_HASHMAP_H
