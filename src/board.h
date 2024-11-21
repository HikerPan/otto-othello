#ifndef BOARD_HPP
#define BOARD_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>    // for bool type
#include <stddef.h>     // for size_t type
#include "moves_hashmap.h"
#include <math.h>



#define OTHELLO_BOARD_SIZE      (64)


// 定义 othelloBoard 结构体
typedef struct {
    int positions[OTHELLO_BOARD_SIZE]; // 棋盘上的所有棋子，1 表示黑棋，-1 表示白棋
    int discsOnBoard;                  // 棋盘上已有棋子的数量
    float timeLimit;                   // 时间限制
    bool passes[2];                    // 记录最近两次是否为 pass
    MovePair_t *moves;                 // 哈希表存储所有可能的走法
    bool initFlag;
} othelloBoard;

// 函数声明
void othelloBoard_init(othelloBoard *board);
void othelloBoard_displayBoard(othelloBoard *board, int color);
void othelloBoard_displayLegalMoves(othelloBoard *board);
void othelloBoard_findLegalMoves(othelloBoard *board, int color, MovePair_t **pMoves);
void othelloBoard_findLegalMoveInDirection(othelloBoard *board, int disc, int color, int direction, MovePair_t **pMoves);
void othelloBoard_updateBoard(othelloBoard *board, int color, MovePair_t *move);
bool othelloBoard_terminalState(othelloBoard *board);
void othelloBoard_index2coord(int index, int *colNum, int *rowNum);
int board_count(int *p, size_t start, size_t end, int target);
int board_accumulate(int *p, size_t start, size_t end, int init_value);



#ifdef __cplusplus
}
#endif

#endif // BOARD_HPP
