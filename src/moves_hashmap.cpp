
#include "stdio.h"
#include "uthash.h"     // 引入 uthash.h
#include "utlist.h"     // 引入 utlist.h
#include "moves_hashmap.h"

/**
 * @brief 判断翻转链表是否为空
 *
 * 检查传入的链表头节点是否为空，如果为空则返回 1，否则返回 0。
 *
 * @param head 链表头节点指针
 * @return 如果链表为空返回 1，否则返回 0
 */
int flip_list_empty(IntListNode *head) {
    return head == NULL;
}

/**
 * @brief 判断哈希表是否为空
 *
 * 检查传入的哈希表头指针是否为空，如果为空，则哈希表为空，返回1；否则返回0。
 *
 * @param head 哈希表的头指针
 * @return 如果哈希表为空，返回1；否则返回0
 */
int move_hash_empty(MoveHash *head) {
    return head == NULL;
}

void list_push_front(IntListNode **head, int flip_position) {
    IntListNode *node = (IntListNode *)malloc(sizeof(IntListNode));
    if (node) {
        node->flip_position = flip_position;
        LL_PREPEND(*head, node);  // 将节点添加到链表头部
    }
}


/**
 * @brief 查找哈希移动列表的起始位置
 *
 * 在给定的哈希移动列表中找到起始位置并返回该位置的指针。
 *
 * @param moves 哈希移动列表的指针
 * @return 返回哈希移动列表的起始位置的指针
 */
MOVES_PAIR_T *find_begin(MoveHash *moves) {
    MOVES_PAIR_T *p_begin = NULL;

    if(NULL == moves){
        printf("\n[find_begin] NULL == moves.\n");
        return NULL;
    }
    p_begin = &moves->moves_pair;

    return p_begin;
}

/**
 * @brief 在链表中查找并返回最后一个元素的指针
 *
 * 从给定的链表头节点开始，遍历链表直到最后一个节点，并返回该节点的指针。
 *
 * @param moves 链表头节点的指针
 *
 * @return 返回链表的最后一个节点的指针。如果链表为空，则返回 NULL。
 */
MOVES_PAIR_T *find_end(MoveHash *moves) {
    MOVES_PAIR_T *p_end = NULL;
    MoveHash *curMove = NULL;
    MoveHash *nextMove = NULL;

    if(NULL == moves){
        printf("\n[find_end] NULL == moves.\n");
        return NULL;
    }

    // 遍历哈希表
    HASH_ITER(hh,moves,curMove,nextMove){
        if(NULL == nextMove){
            p_end = &curMove->moves_pair;
            return p_end;
        }
    }
    
    return NULL;
}

// 添加合法走法
// void add_move(MoveHash *moves_hash, int position, int *flips, int flip_count) {
//     MoveHash *move = NULL;

//     if(NULL == moves_hash)
//     {
//         printf("\nNULL == moves\n");
//         return;
//     }

//     // 查找哈希表中是否存在此位置的走法
//     HASH_FIND_INT(moves_hash, &position, move);
//     // 不存在
//     if (!move) {
//         // 如果没有该位置的走法，则分配内存并初始化
//         move = (MoveHash *)malloc(sizeof(MoveHash));
//         if (move == NULL) {
//             printf("Memory allocation failed for move\n");
//             return;
//         }
//         move->position = position;
//         // move->move_next = NULL;
//         move->flip_list = NULL;  // 初始化为空链表
//         HASH_ADD_INT(moves_hash, position, move);  // 添加新项到哈希表

//     }

//     // 将翻转位置添加到链表
//     for (int i = 0; i < flip_count; ++i) {
//         IntListNode *node = (IntListNode*)malloc(sizeof(IntListNode));
//         if (node == NULL) {
//             printf("Memory allocation failed for IntListNode\n");
//             return;
//         }
//         node->value = flips[i];
//         LL_APPEND(move->flip_list, node);  // 使用 utlist 添加到链表末尾
//     }
// }

// 查找合法走法
MOVES_PAIR_T *find_move(MoveHash *moves, int position) {
    MoveHash *move;
    MOVES_PAIR_T *moves_pair = NULL;
    HASH_FIND_INT(moves, &position, move);
    if(NULL != move)
    {
        moves_pair = &move->moves_pair;

        return moves_pair;
    }
    return NULL;
}


// 删除走法
void delete_move(MoveHash *moves, int position) {

    MoveHash *move = NULL;
    MOVES_PAIR_T *moves_pair = NULL;

    // 通过key值查找moves
    HASH_FIND_INT(moves, &position, move);

    // 如果找到，则先删除翻转列表，然后删除moves
    if(NULL != move){
        moves_pair = &move->moves_pair;
        IntListNode *node, *tmp;

        // 遍历并释放链表中的每个节点
        LL_FOREACH_SAFE(moves_pair->flip_list, node, tmp) {
            free(node);  // 使用 free 释放链表节点
        }

        // 从哈希表中删除该 Move
        HASH_DEL(moves, move);

        // 释放 move 结构体
        free(move);
    }

    return;   
}

// 清空所有走法
void clear_moves(MoveHash *moves) {
    MoveHash *move, *tmp;
    HASH_ITER(hh, moves, move, tmp) {
        delete_move(moves,moves->position);
    }
}

int size_moves(MoveHash *hashTable) {
    return HASH_COUNT(hashTable);  // 使用 HASH_COUNT 获取哈希表大小
}

void print_moves(MoveHash *moves) {
    MoveHash *move = NULL;
    MoveHash *tmp = NULL;
    MOVES_PAIR_T *moves_pair = NULL;
    IntListNode *node = NULL;
    IntListNode *node_tmp;
    
    printf("\n [moves print start]\n");
    HASH_ITER(hh, moves, move, tmp) {
        moves_pair = &move->moves_pair;
        printf("[key] %d, [position] %d, ", move->position,move->moves_pair.position);
        printf("[flip_list]:");
        LL_FOREACH_SAFE(moves_pair->flip_list, node, node_tmp){
            printf(" %d ,",node->flip_position);
        }
        printf("\n");
        
    }
    printf("\n [moves print end]\n");
}




int merge_flip_lists(IntListNode *dest_list, IntListNode *source_list) {
    IntListNode *node = NULL;
    IntListNode *tmp = NULL;

    if(NULL == dest_list || NULL == source_list){
        printf("invalid paramter\n");
        return 0;
    }
    LL_FOREACH_SAFE(source_list, node,tmp) {
        IntListNode *new_node = (IntListNode *)malloc(sizeof(IntListNode));
        if(NULL == new_node)
        {
            printf("no enough memery for new node.\n");
            return 0;
        }
        new_node->flip_position = node->flip_position;
        new_node->next = NULL;
        LL_APPEND(dest_list, new_node);

        free(node);
    }

    return 1;
}


// void merge(MoveHash **targetHashTable, MoveHash *sourceHashTable) {
//     MoveHash *srcEntry, *tmp;

//     HASH_ITER(hh, sourceHashTable, srcEntry, tmp) {
//         MoveHash *tgtEntry;

//         // 查找 target 中是否已存在相同的键
//         HASH_FIND_INT(*targetHashTable, &srcEntry->key, tgtEntry);
//         if (tgtEntry == NULL) {
//             // 如果键不存在，将 srcEntry 复制到 target
//             tgtEntry = (MoveHash *)malloc(sizeof(MoveHash));
//             tgtEntry->key = srcEntry->key;
//             tgtEntry->values = NULL;
//             HASH_ADD_INT(*targetHashTable, key, tgtEntry);
//         }

//         // 将源列表中的节点逐一复制到目标列表
//         IntListNode *srcNode;
//         LL_FOREACH(srcEntry->values, srcNode) {
//             IntListNode *newNode = (IntListNode *)malloc(sizeof(ListNode));
//             newNode->value = srcNode->value;
//             newNode->next = NULL;
//             LL_APPEND(tgtEntry->values, newNode);
//         }
//     }
// }

void insert_moves(MoveHash *hashTable, MOVES_PAIR_T *moves_node) {
    MoveHash *entry;

    if(NULL == hashTable || NULL == moves_node)
    {
        printf("\nNULL == hashTable || NULL == moves_node\n");
        return;
    }

    // 查找键是否已存在
    HASH_FIND_INT(hashTable, moves_node->position, entry);
    if (entry == NULL) {
        // 如果不存在，则创建新条目
        entry = (MoveHash *)malloc(sizeof(MoveHash));
        entry->position = moves_node->position;
        entry->moves_pair.position = moves_node->position;
        merge_flip_lists(entry->moves_pair.flip_list,moves_node->flip_list);
    }

    HASH_ADD_INT(hashTable,moves_node->position,entry);

}


