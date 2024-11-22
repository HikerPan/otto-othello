#ifndef MOVES_HASHMAP_H
#define MOVES_HASHMAP_H

#include "uthash.h"     // 引入 uthash.h
#include "utlist.h"     // 引入 utlist.h
// #include "board.h"
#include <stdbool.h>    // for bool type
#ifdef __cplusplus
extern "C" {
#endif



#define MAX_FLIPS 32

typedef unsigned char uint8_t;

// 定义链表节点，用于替代 std::list<int>
// typedef struct _IntListNode_ {
//     int flip_position;      // 翻转位置
//     struct _IntListNode_ *next;
// }IntListNode_t;

typedef struct _MovePair_t_
{
    int position;                       // 键：走法位置
    uint8_t flip_array[MAX_FLIPS];      // 最高位为 1，其余表示使用，否则表示未使用
    struct _MovePair_t_ *next;     // 值：下一个走法
}MovePair_t;


typedef struct {
    int key;       // 存储整数
    UT_hash_handle hh; // uthash 处理器
} IntSetNode;



int move_hash_empty(MovePair_t *head);
int flip_list_empty(uint8_t *flip_array);
void list_push_front(uint8_t *flip_array, int flip_position);
MovePair_t *find_begin(MovePair_t *moves);
MovePair_t *find_end(MovePair_t *moves);
void add_move(MovePair_t *moves, int position, int *flips, int flip_count);
MovePair_t *find_move(MovePair_t *moves, int position);
void delete_move(MovePair_t *moves, int position);
void clear_moves(MovePair_t **moves);
void print_moves(MovePair_t *moves);
int merge_flip_lists(uint8_t *flip_target, uint8_t *flip_src);
// void insert_moves(MovePair_t **hashTable, MovePair_t *moves_node);
void insert_moves(MovePair_t **hashTable, MovePair_t *moves_node);
int size_moves(MovePair_t *hashTable);
MovePair_t *find_next(MovePair_t *moves, MovePair_t *current);
void copy_moves(MovePair_t **targetMoves, MovePair_t *srcMove);
void add_to_set(IntSetNode **set, int value);
int is_in_set(IntSetNode *set, int value);
void remove_from_set(IntSetNode **set, int value);
void clear_set(IntSetNode **set);
int count_set_elements(IntSetNode *set);
 
#ifdef __cplusplus
}
#endif

#endif // MOVES_HASHMAP_H
