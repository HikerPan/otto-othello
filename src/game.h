#ifndef GAME_HPP
#define GAME_HPP

#include <stdbool.h>
#include "player.h"
#include "board.h"

#define MAX_MOVE_HISTORY_LEN    (10*OTHELLO_BOARD_SIZE)

typedef struct {
    othelloBoard board;
    othelloPlayer blackPlayer;
    othelloPlayer whitePlayer;
    char moveHistory[MAX_MOVE_HISTORY_LEN]; // 替换 std::string 为字符数组

    int toMove;       // 表示当前轮到谁，1 代表黑棋，-1 代表白棋
    bool gameOver;    // 表示游戏是否结束
} othelloGame;

// 初始化 othelloGame，相当于 C++ 的构造函数
void othelloGame_init(othelloGame *game);
// 开始新游戏，设置 AI 玩家、黑白玩家和时间限制
void othelloGame_newGame(othelloGame *game, bool blackComputer, bool whiteComputer, float timeLimit);
// 从文件加载游戏，读取文件中的棋盘状态和其他设置
void othelloGame_loadGame(othelloGame *game, const char *fileName, bool blackComputer, bool whiteComputer);
// 执行一次走子操作
void othelloGame_move(othelloGame *game, int color);
// 检查游戏是否结束
void othelloGame_checkGameOver(othelloGame *game);

// class othelloGame {
//     public:
//         othelloBoard board;
//         othelloPlayer blackPlayer;
//         othelloPlayer whitePlayer;
//         // std::string moveHistory = "";
//         char moveHistory[MAX_MOVE_HISTORY_LEN];

//         int toMove = 1;
//         bool gameOver = false;

//         // Constructor
//         othelloGame();

//         // Initialize board
//         void newGame(bool blackComputer, bool whiteComputer, float timeLimit);

//         // Load game from file
//         // 0, 1, 2 for an empty square, a white disc, and a black disc, resp.
//         // Last two lines give the player to move (1 for black, 2 for white)
//         // and clock time limit for the AI, respectively.
//         void loadGame(std::string fileName, bool blackComputer, bool whiteComputer);

//         // Make a move
//         void move(int color);

//         // Update status of the game
//         void checkGameOver();
// };

#endif // GAME_HPP
