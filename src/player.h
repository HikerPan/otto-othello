#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include "database.h"
#include "heuristic.h"
#include "moves_hashmap.h"
#include "board.h"

#define INT_MIN    (-65535)
#define INT_MAX     (65535)

// 定义 othelloPlayer 结构体
typedef struct othelloPlayer {
    int color;
    bool computer;

    // Heuristic and database for the player
    othelloHeuristic heuristic;
    othelloDatabase database;

    // Stack of nodes for AI search
    struct node {
        bool isMaxNode;
        int alpha;
        int beta;
        int score;
        othelloBoard *board;
        MovePair_t *prevIterator;
        MovePair_t *moveIterator;
        MovePair_t *lastMove;
    } nodeStack[OTHELLO_BOARD_SIZE];  // 64-element node stack for depth-limited minimax
} othelloPlayer;

// 声明函数接口

// 生成移动，适用于玩家
MovePair_t *othelloPlayer_move(othelloPlayer *player,othelloBoard *board,MoveHash_t *legalMoves,bool pass,char *moveHistory);

// 获取用户下的下一步移动
MovePair_t *othelloPlayer_humanMove(othelloPlayer *player,MoveHash_t *legalMoves,bool *pass);

// 将棋盘坐标转换为索引
int othelloPlayer_coord2index(char *coord);

// 生成AI算法的移动
MovePair_t *othelloPlayer_computerMove(othelloPlayer *player,othelloBoard *board,MoveHash_t *legalMoves,bool pass,char *moveHistory);

// 启动计时器并返回开始时间
struct timespec othelloPlayer_startTimer();

// 停止计时器并返回经过的时间（秒）
float othelloPlayer_stopTimer(struct timespec startTime);

// 执行带alpha-beta剪枝的深度限制极小极大搜索
MovePair_t *othelloPlayer_depthLimitedAlphaBeta(othelloPlayer *player,othelloBoard *board, int depthLimit,struct timespec startTime, float timeLimit) ;



#endif //PLAYER_HPP
