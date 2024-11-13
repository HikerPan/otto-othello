
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
int flip_list_empty(IntListNode_t *head) {
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
int move_hash_empty(MoveHash_t *head) {
    return head == NULL;
}

void list_push_front(IntListNode_t **head, int flip_position) {
    IntListNode_t *node = (IntListNode_t *)malloc(sizeof(IntListNode_t));
    if (node) {
        node->flip_position = flip_position;
        node->next = NULL;
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
MovePair_t *find_begin(MoveHash_t *moves) {
    MovePair_t *p_begin = NULL;

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
MovePair_t *find_end(MoveHash_t *moves) {
    MovePair_t *p_end = NULL;
    MoveHash_t *curMove = NULL;
    MoveHash_t *nextMove = NULL;

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
// void add_move(MoveHash_t *moves_hash, int position, int *flips, int flip_count) {
//     MoveHash_t *move = NULL;

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
//         move = (MoveHash_t *)malloc(sizeof(MoveHash_t));
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
//         IntListNode_t *node = (IntListNode_t*)malloc(sizeof(IntListNode_t));
//         if (node == NULL) {
//             printf("Memory allocation failed for IntListNode_t\n");
//             return;
//         }
//         node->value = flips[i];
//         LL_APPEND(move->flip_list, node);  // 使用 utlist 添加到链表末尾
//     }
// }

// 查找合法走法
MovePair_t *find_move(MoveHash_t *moves, int position) {
    MoveHash_t *move;
    MovePair_t *moves_pair = NULL;
    HASH_FIND_INT(moves, &position, move);
    if(NULL != move)
    {
        moves_pair = &move->moves_pair;

        return moves_pair;
    }
    return NULL;
}


// 删除走法
void delete_move(MoveHash_t *moves, int position) {

    MoveHash_t *move = NULL;
    MovePair_t *moves_pair = NULL;

    // 通过key值查找moves
    HASH_FIND_INT(moves, &position, move);

    // 如果找到，则先删除翻转列表，然后删除moves
    if(NULL != move){
        moves_pair = &move->moves_pair;
        IntListNode_t *node, *tmp;

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
void clear_moves(MoveHash_t *moves) {
    MoveHash_t *move, *tmp;
    HASH_ITER(hh, moves, move, tmp) {
        delete_move(moves,moves->position);
    }
}

int size_moves(MoveHash_t *hashTable) {
    return HASH_COUNT(hashTable);  // 使用 HASH_COUNT 获取哈希表大小
}

void print_moves(MoveHash_t *moves) {
    MoveHash_t *move = NULL;
    MoveHash_t *tmp = NULL;
    MovePair_t *moves_pair = NULL;
    IntListNode_t *node = NULL;
    IntListNode_t *node_tmp;
    
    printf("\n\n[moves print start]\n");
    HASH_ITER(hh, moves, move, tmp) {
        moves_pair = &move->moves_pair;
        printf("[key] %d, [position] %d, ", move->position,move->moves_pair.position);
        printf("[flip_list]:");
        if(NULL == moves_pair->flip_list)
        {
            printf("NULL\n");
            continue;
        }
        LL_FOREACH_SAFE(moves_pair->flip_list, node, node_tmp){
            printf(" %d ,",node->flip_position);
        }
        printf("\n");
        
    }
    printf("[moves print end]\n\n");
}




int merge_flip_lists(IntListNode_t **dest_list, IntListNode_t *source_list) {
    // IntListNode_t *node = NULL;
    // IntListNode_t *tmp = NULL;

    if(NULL == source_list){
        printf("source_list can not be NULL\n");
        return 0;
    }

    LL_CONCAT(*dest_list,source_list);
    // LL_FOREACH_SAFE(source_list, node,tmp) {
    //     IntListNode_t *new_node = (IntListNode_t *)malloc(sizeof(IntListNode_t));
    //     if(NULL == new_node)
    //     {
    //         printf("no enough memery for new node.\n");
    //         return 0;
    //     }
    //     new_node->flip_position = node->flip_position;
    //     new_node->next = NULL;
    //     LL_APPEND(dest_list, new_node);

    //     free(node);
    // }

    return 1;
}


// void merge(MoveHash_t **targetHashTable, MoveHash_t *sourceHashTable) {
//     MoveHash_t *srcEntry, *tmp;

//     HASH_ITER(hh, sourceHashTable, srcEntry, tmp) {
//         MoveHash_t *tgtEntry;

//         // 查找 target 中是否已存在相同的键
//         HASH_FIND_INT(*targetHashTable, &srcEntry->key, tgtEntry);
//         if (tgtEntry == NULL) {
//             // 如果键不存在，将 srcEntry 复制到 target
//             tgtEntry = (MoveHash_t *)malloc(sizeof(MoveHash_t));
//             tgtEntry->key = srcEntry->key;
//             tgtEntry->values = NULL;
//             HASH_ADD_INT(*targetHashTable, key, tgtEntry);
//         }

//         // 将源列表中的节点逐一复制到目标列表
//         IntListNode_t *srcNode;
//         LL_FOREACH(srcEntry->values, srcNode) {
//             IntListNode_t *newNode = (IntListNode_t *)malloc(sizeof(ListNode));
//             newNode->value = srcNode->value;
//             newNode->next = NULL;
//             LL_APPEND(tgtEntry->values, newNode);
//         }
//     }
// }

void insert_moves(MoveHash_t **hashTable, MovePair_t *moves_node) {
    MoveHash_t *entry;

    if(NULL == moves_node){
        printf("\nNULL == moves_node\n");
        return;
    }

    printf("\nmoves_node->position %d\n",moves_node->position);
    // 查找键是否已存在
    HASH_FIND_INT(*hashTable, &moves_node->position, entry);
    if (entry == NULL) {
        printf("key:%d not found, create pairs.\n",moves_node->position);
        // 如果不存在，则创建新条目
        entry = (MoveHash_t *)malloc(sizeof(MoveHash_t));
        entry->position = moves_node->position;
        entry->moves_pair.position = moves_node->position;
        LL_CONCAT(entry->moves_pair.flip_list,moves_node->flip_list);
        // merge_flip_lists(&entry->moves_pair.flip_list,moves_node->flip_list);

        HASH_ADD_INT(*hashTable, position,entry);
    }
    else{
        printf("key:%d found, merge lists.\n",moves_node->position);
        // merge_flip_lists(&entry->moves_pair.flip_list,moves_node->flip_list);
        LL_CONCAT(entry->moves_pair.flip_list,moves_node->flip_list);
    }

    

}


