
#include "stdio.h"
#include "uthash.h"     // 引入 uthash.h
#include "utlist.h"     // 引入 utlist.h
#include "moves_hashmap.h"

// 初始化哈希表根指针
Move *moves = NULL;

// 添加合法走法
void add_move(int position, int *flips, int flip_count) {
    Move *move;

    // 检查是否已有该位置的走法
    HASH_FIND_INT(moves, &position, move);
    if (!move) {
        move = new Move;
        move->position = position;
        move->flip_list = NULL; // 初始化为空链表
        HASH_ADD_INT(moves, position, move);
    }

    // 将翻转位置添加到链表
    for (int i = 0; i < flip_count; ++i) {
        ListNode *node = new ListNode;
        node->value = flips[i];
        LL_APPEND(move->flip_list, node);  // 使用 utlist 添加到链表
    }
}

// 查找合法走法
Move *find_move(int position) {
    Move *move;
    HASH_FIND_INT(moves, &position, move);
    return move;
}

// 删除走法
void delete_move(int position) {
    Move *move = find_move(position);
    if (move) {
        ListNode *node, *tmp;
        LL_FOREACH_SAFE(move->flip_list, node, tmp) {
            delete node;  // 释放链表节点
        }
        HASH_DEL(moves, move);  // 从哈希表中删除
        delete move;
    }
}

// 清空所有走法
void clear_moves() {
    Move *move, *tmp;
    HASH_ITER(hh, moves, move, tmp) {
        delete_move(move->position);
    }
}

void print_moves() {
    Move *move;
    // 遍历哈希表
    for (move = moves; move != NULL; move = (Move*)(move->hh.next)) {
        printf("Move at position %d: ", move->position);

        // 遍历翻转列表
        ListNode *current = move->flip_list;
        while (current != NULL) {
            printf("%d ", current->value);
            current = current->next;
        }
        printf("\n");
    }
}
