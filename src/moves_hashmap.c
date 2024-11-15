
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
int move_hash_empty(MovePair_t *head) {
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
MovePair_t *find_begin(MovePair_t *moves) {
    MovePair_t *p_begin = NULL;

    if(NULL == moves){
        // printf("\n[find_begin] NULL == moves.\n");
        return NULL;
    }
    p_begin = moves;

    return p_begin;
}

MovePair_t *find_next(MovePair_t *moves_head, MovePair_t *current){
    MovePair_t *nextMove = NULL;
    MovePair_t *curMove = NULL;

    if(NULL == moves_head || current == NULL){
        return NULL;
    }

    LL_FOREACH_SAFE(moves_head,curMove,nextMove){
        if(current->position == curMove->position)
            return nextMove;
    }
    

    return NULL;
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
MovePair_t *find_end(MovePair_t  *moves) {
    MovePair_t *curMove = NULL;
    MovePair_t *nextMove = NULL;

    if(NULL == moves){
        // printf("\n[find_end] NULL == moves.\n");
        return NULL;
    }

    LL_FOREACH_SAFE(moves,curMove,nextMove){
        if(NULL == nextMove)
            return curMove;
    }

    
    return NULL;
}


// 查找合法走法
MovePair_t *find_move(MovePair_t *moves, int position) {
    MovePair_t *curMove = NULL;
    MovePair_t *nextMove = NULL;

    if(NULL == moves){
        // printf("\n[find_move] NULL == moves.\n");
        return NULL;
    }

    LL_FOREACH_SAFE(moves,curMove,nextMove){
        if(position == curMove->position)
            return curMove;
    }
    
    return NULL;
}


// 删除走法
void delete_move(MovePair_t *moves, int position) {

    MovePair_t *curMove = NULL;
    MovePair_t *nextMove = NULL;

    if(NULL == moves){
        printf("\n[delete_move] NULL == moves.\n");
        return;
    }

    LL_FOREACH_SAFE(moves,curMove,nextMove){
        if((NULL != curMove)&&(position == curMove->position))
        {
            LL_DELETE(moves,curMove);
            break;
        }    
    }

    

    return;   
}

// 清空所有走法
void clear_moves(MovePair_t **moves) {
    MovePair_t *curMove = NULL;
    MovePair_t *nextMove = NULL;

    if(NULL == moves){
        printf("\n[clear_moves] NULL == moves.\n");
        return ;
    }

    LL_FOREACH_SAFE(*moves,curMove,nextMove){
        if(NULL != curMove)
        {
            LL_DELETE(*moves,curMove);
            // free(curMove);
            // curMove = NULL;
        }    
    }

    return;
}

int size_moves(MovePair_t *hashTable) {
    int counter = 0;
    MovePair_t *curMove = NULL;
    LL_COUNT(hashTable,curMove,counter);
    return counter;
}

void print_moves(MovePair_t *moves) {
    MovePair_t *curMove = NULL;
    MovePair_t *nextMove = NULL;
    IntListNode_t *node = NULL;
    IntListNode_t *node_tmp;
    
    printf("\n\n[moves print start]\n");
    
    LL_FOREACH_SAFE(moves,curMove,nextMove){
        printf("[key] %d, ",curMove->position);
        printf("[flip_list]:");
        if(NULL == curMove->flip_list)
        {
            printf("NULL\n");
            continue;
        }
        LL_FOREACH_SAFE(curMove->flip_list, node, node_tmp){
            printf(" %d ,",node->flip_position);
        }
        printf("\n");
        
    }
    printf("[moves print end]\n\n");
}




int merge_flip_lists(IntListNode_t **dest_list, IntListNode_t *source_list) {


    if(NULL == source_list){
        printf("source_list can not be NULL\n");
        return 0;
    }

    LL_CONCAT(*dest_list,source_list);
    

    return 1;
}





void insert_moves(MovePair_t **hashTable, MovePair_t *moves_node) {
    // MovePair_t *entry = NULL;
    MovePair_t *findNode = NULL;

    if(NULL == moves_node){
        printf("\nNULL == moves_node\n");
        return;
    }

    // printf("\nmoves_node->position %d\n",moves_node->position);
    // 查找键是否已存在
    findNode = find_move(*hashTable,moves_node->position); 
    if (findNode == NULL) {
        // printf("key:%d not found, create pairs.\n",moves_node->position);
        // 如果不存在，则创建新条目
        // entry = (MoveHash_t *)malloc(sizeof(MoveHash_t));
        // entry->position = moves_node->position;
        // entry->moves_pair.position = moves_node->position;
        // LL_CONCAT(entry->moves_pair.flip_list,moves_node->flip_list);
        // // merge_flip_lists(&entry->moves_pair.flip_list,moves_node->flip_list);

        // HASH_ADD_INT(*hashTable, position,entry);
        LL_APPEND(*hashTable,moves_node);
    }
    else{
        // printf("key:%d found, merge lists.\n",moves_node->position);
        // merge_flip_lists(&entry->moves_pair.flip_list,moves_node->flip_list);
        LL_CONCAT(findNode->flip_list,moves_node->flip_list);
    }
    
    return;
}

// 添加元素到集合
void add_to_set(IntSetNode **set, int value) {
    IntSetNode *node = NULL;
    HASH_FIND_INT(*set, &value, node);
    if (node == NULL) {
        node = (IntSetNode *)malloc(sizeof(IntSetNode));
        node->key = value;
        HASH_ADD_INT(*set, key, node);
    }
}

// 检查元素是否在集合中
int is_in_set(IntSetNode *set, int value) {
    IntSetNode *node = NULL;
    HASH_FIND_INT(set, &value, node);
    return node != NULL;
}

// 从集合中删除元素
void remove_from_set(IntSetNode **set, int value) {
    IntSetNode *node = NULL;
    HASH_FIND_INT(*set, &value, node);
    if (node != NULL) {
        HASH_DEL(*set, node);
        free(node);
    }
}

// 清空集合
void clear_set(IntSetNode **set) {
    IntSetNode *current_node, *tmp;
    HASH_ITER(hh, *set, current_node, tmp) {
        HASH_DEL(*set, current_node);
        free(current_node);
    }
}

int count_set_elements(IntSetNode *set) {
    int count = 0;
    IntSetNode *current_node;
    for (current_node = set; current_node != NULL; current_node = current_node->hh.next) {
        count++;
    }
    return count;
}



