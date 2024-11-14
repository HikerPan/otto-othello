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
MovePair_t *othelloPlayer_computerMove(othelloPlayer *player,othelloBoard *board,bool pass,char *moveHistory);

// 启动计时器并返回开始时间
struct timespec othelloPlayer_startTimer();

// 停止计时器并返回经过的时间（秒）
float othelloPlayer_stopTimer(struct timespec startTime);

// 执行带alpha-beta剪枝的深度限制极小极大搜索
MovePair_t *othelloPlayer_depthLimitedAlphaBeta(othelloPlayer *player,othelloBoard *theBoard,int depthLimit,struct timespec startTime,float timeLimit);


// class othelloPlayer {
//     public:
//         int color;
//         bool computer;

//         // Driver for moves, regardless of player
//         // std::pair<int, std::list<int>> move(othelloBoard &board,
//         //         std::unordered_map<int, std::list<int>> &legalMoves,
//         //         bool &pass, std::string &moveHistory);
//         MovePair_t *move(othelloBoard *board,
//                         MoveHash_t *legalMoves,
//                         bool pass, char *moveHistory);

//     private:
//         struct node {
//             bool isMaxNode;
//             int alpha;
//             int beta;
//             int score;
//             othelloBoard *board;
//         //     std::unordered_map<int, std::list<int>>::iterator prevIterator;
//         //     std::unordered_map<int, std::list<int>>::iterator moveIterator;
//         //     std::unordered_map<int, std::list<int>>::iterator lastMove;
//             MovePair_t *prevIterator;
//             MovePair_t *moveIterator;
//             MovePair_t *lastMove;
//         };

//         std::array<node, 64> nodeStack = {};
//         //std::array<std::array<int, 2>, 64> killerMoves = {};

//         othelloHeuristic heuristic;

//         othelloDatabase database;

//         // Prompts user for next move
//         // std::pair<int, std::list<int>> humanMove(
//         //         std::unordered_map<int, std::list<int>> &legalMoves, bool &pass);
//         MovePair_t *humanMove(MoveHash_t *legalMoves, bool &pass);

//         int coord2index(char *coord);

//         // Driver for the AI algorithm
//         // std::pair<int, std::list<int>> computerMove(othelloBoard &board,
//         //         std::unordered_map<int, std::list<int>> &legalMoves, bool &pass, std::string &moveHistory);
//         MovePair_t *computerMove(othelloBoard *board,
//                 MoveHash_t *legalMoves, bool pass, char *moveHistory);

//         // Returns time point
//         std::chrono::time_point<std::chrono::system_clock> startTimer();

//         // Returns time elapsed in seconds
//         float stopTimer(
//                 std::chrono::time_point<std::chrono::system_clock> startTime);

//         // Performs depth-limited minimax search with alpha-beta pruning
//         // Implemented using a stack to avoid recursion overhead
//         // Returns move for square -1 if time runs out
//         MovePair_t *depthLimitedAlphaBeta(
//                 othelloBoard *theBoard, int depthLimit,
//                 std::chrono::time_point<std::chrono::system_clock> startTime,
//                 float timeLimit);
// };

#endif //PLAYER_HPP
