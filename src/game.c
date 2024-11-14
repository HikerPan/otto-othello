#include "game.h"


// Constructor
/**
 * @brief 构造函数
 *
 * 初始化国际象棋游戏棋盘。
 * 将棋盘大小设置为64个位置，并将所有位置初始化为0。
 */
// othelloGame::othelloGame() {
//     // 初始化棋盘，将棋盘上的所有位置都初始化为0
//     // 0代表该位置为空
//     // this->board.positions.resize(64, 0);
//     memset(this->board.positions,0,OTHELLO_BOARD_SIZE*sizeof(int));
//     memset(this->moveHistory,0,MAX_MOVE_HISTORY_LEN*sizeof(char));
// }

// 初始化游戏
void othelloGame_init(othelloGame *game) {
    
    othelloBoard_init(&game->board);
    
    // memset(game->board.positions, 0, OTHELLO_BOARD_SIZE * sizeof(int));
    memset(game->moveHistory, 0, MAX_MOVE_HISTORY_LEN * sizeof(char));
    game->toMove = 1;
    game->gameOver = false;
}

// Initialize new game
/**
 * @brief 开始新游戏
 *
 * 初始化棋盘、玩家、移动方和时间限制。
 *
 * @param blackComputer 是否让黑方为电脑控制
 * @param whiteComputer 是否让白方为电脑控制
 * @param timeLimit 每方的时间限制（秒）
 */
// void othelloGame::newGame(bool blackComputer, bool whiteComputer,
//         float timeLimit) {
//     // 初始化棋盘
//     // Initialize board
//     std::vector<int> setup(64, 0);
//     setup[27] = -1;
//     setup[28] = 1;
//     setup[35] = 1;
//     setup[36] = -1;
    
//     // this->board.positions.swap(setup);
//     for(size_t i = 0;i<OTHELLO_BOARD_SIZE;i++)
//     {
//         this->board.positions[i] = setup[i];
//     }

//     // 初始化玩家
//     // Initialize players
//     this->blackPlayer.color = 1;
//     this->blackPlayer.computer = blackComputer;
//     this->whitePlayer.color = -1;
//     this->whitePlayer.computer = whiteComputer;

//     // 初始化当前玩家
//     // Initialize player to move
//     this->toMove = 1;

//     // 初始化时间限制
//     // Initialize time limit
//     this->board.timeLimit = timeLimit;
// }

// 开始新游戏
void othelloGame_newGame(othelloGame *game, bool blackComputer, bool whiteComputer, float timeLimit) {
    // 初始化棋盘
    int setup[64] = {0};
    setup[27] = -1;
    setup[28] = 1;
    setup[35] = 1;
    setup[36] = -1;

    for (size_t i = 0; i < OTHELLO_BOARD_SIZE; i++) {
        game->board.positions[i] = setup[i];
    }

    // 初始化玩家
    game->blackPlayer.color = 1;
    game->blackPlayer.computer = blackComputer;
    game->whitePlayer.color = -1;
    game->whitePlayer.computer = whiteComputer;

    // 初始化当前玩家和时间限制
    game->toMove = 1;
    game->board.timeLimit = timeLimit;
}

// Load game from file
/**
 * @brief 加载游戏状态
 *
 * 从指定文件加载游戏状态，包括棋盘布局、玩家设置和玩家轮次等信息。
 *
 * @param fileName 文件名，指定要加载的游戏状态文件路径
 * @param blackComputer 黑方是否为电脑控制
 * @param whiteComputer 白方是否为电脑控制
 */
// void othelloGame::loadGame(std::string fileName, bool blackComputer,
//         bool whiteComputer) {
//     std::ifstream ifs(fileName.c_str());

//     if (!ifs.good()) {
//         std::cout << "File does not exist!" << std::endl;
//         return;
//     }

//     // Load board
//     std::vector<int> setup(64, 0);
//     std::string str;
//     char ch;
//     int idx = 0;

//     for (int i = 0; i < 8; i++) {
//         std::getline(ifs, str);
//         for (int j = 0; j < 16; j += 2) {
//             ch = str[j];
//             if (ch == '1') {
//                 setup[idx] = 1;
//             }
//             else if (ch == '2') {
//                 setup[idx] = -1;
//             }
//             else if (ch == '0') {
//                 setup[idx] = 0;
//             }
//             else {
//                 std::cout << "Invalid file format! Refer to the README." << std::endl;
//                 return;
//             }

//             idx++;
//         }
//     }
//     this->board.discsOnBoard = 64 - std::count(setup.begin(), setup.end(), 0);
//     // this->board.positions.swap(setup);
//     for(size_t i = 0;i<OTHELLO_BOARD_SIZE;i++)
//     {
//         this->board.positions[i] = setup[i];
//     }

//     // Initialize players
//     this->blackPlayer.color = 1;
//     this->blackPlayer.computer = blackComputer;
//     this->whitePlayer.color = -1;
//     this->whitePlayer.computer = whiteComputer;

//     // Load player to move
//     if (std::getline(ifs, str)) {
//         ch = str[0];
//         if (ch == '1') {
//             this->toMove = 1;
//         }
//         else if (ch == '2') {
//             this->toMove = -1;
//         }
//         else {
//             std::cout << "Player to move must be 1 (black) or 2 (white)!"
//                 << std::endl;
//             ifs.close();
//             return;
//         }
//     }
//     else {
//         std::cout << "Save file does not specify player to move!"
//             << std::endl;
//         ifs.close();
//         return;
//     }

//     // Load time limit
//     if (std::getline(ifs, str)) {
//         if (stof(str) > 0) {
//             this->board.timeLimit = stof(str);
//         }
//         else {
//             std::cout << "Time limit must be a positive number!"
//                 << std::endl;
//             ifs.close();
//             return;
//         }
//     }
//     else {
//         std::cout << "Save file does not specify computer time limit!"
//             << std::endl;
//         ifs.close();
//         return;
//     }

//     ifs.close();
// }
// 从文件加载游戏
void othelloGame_loadGame(othelloGame *game, const char *fileName, bool blackComputer, bool whiteComputer) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        printf("File does not exist!\n");
        return;
    }

    // 加载棋盘
    int setup[64] = {0};
    char line[16];
    int idx = 0;

    for (int i = 0; i < 8; i++) {
        if (fgets(line, sizeof(line), file)) {
            for (int j = 0; j < 16; j += 2) {
                if (line[j] == '1') {
                    setup[idx] = 1;
                } else if (line[j] == '2') {
                    setup[idx] = -1;
                } else if (line[j] == '0') {
                    setup[idx] = 0;
                } else {
                    printf("Invalid file format! Refer to the README.\n");
                    fclose(file);
                    return;
                }
                idx++;
            }
        }
    }
    game->board.discsOnBoard = OTHELLO_BOARD_SIZE - board_count(setup, 1, OTHELLO_BOARD_SIZE, 0);

    for (size_t i = 0; i < OTHELLO_BOARD_SIZE; i++) {
        game->board.positions[i] = setup[i];
    }

    // 初始化玩家
    game->blackPlayer.color = 1;
    game->blackPlayer.computer = blackComputer;
    game->whitePlayer.color = -1;
    game->whitePlayer.computer = whiteComputer;

    // 加载当前玩家
    if (fgets(line, sizeof(line), file)) {
        if (line[0] == '1') {
            game->toMove = 1;
        } else if (line[0] == '2') {
            game->toMove = -1;
        } else {
            printf("Player to move must be 1 (black) or 2 (white)!\n");
            fclose(file);
            return;
        }
    } else {
        printf("Save file does not specify player to move!\n");
        fclose(file);
        return;
    }

    // 加载时间限制
    if (fgets(line, sizeof(line), file)) {
        float timeLimit = atof(line);
        if (timeLimit > 0) {
            game->board.timeLimit = timeLimit;
        } else {
            printf("Time limit must be a positive number!\n");
            fclose(file);
            return;
        }
    } else {
        printf("Save file does not specify computer time limit!\n");
        fclose(file);
        return;
    }

    fclose(file);
}

// Make a move
/**
 * @brief 在五子棋游戏中执行一步棋
 *
 * 根据玩家颜色，调用对应的玩家对象进行移动，并更新棋盘状态。
 *
 * @param color 玩家颜色，1 表示黑方，-1 表示白方
 */
// void othelloGame::move(int color) {
//     // 定义一个pair类型变量move，用于存储移动结果
//     // std::pair<int, std::list<int>> move;
//     MovePair_t *move = NULL;

//     // 判断当前轮到哪方玩家下棋
//     if (color == 1) {
//         // 如果是黑方下棋
//         std::cout << "Black to move" << std::endl;
//         // 调用黑方玩家的move方法，获取移动结果
//         move = this->blackPlayer.move(&this->board, this->board.moves,
//                 this->board.passes[0], this->moveHistory);
//     } 
//     else if (color == -1) {
//         // 如果是白方下棋
//         std::cout << "White to move" << std::endl;
//         // 调用白方玩家的move方法，获取移动结果
//         move = this->whitePlayer.move(&this->board, this->board.moves,
//                 this->board.passes[0], this->moveHistory);
//     }

//     // 判断当前是否允许下棋
//     if (!this->board.passes[0]) {
//         // 如果允许下棋，则更新棋盘
//         this->board.updateBoard(color, move);
//     }
// }

void othelloGame_move(othelloGame *game, int color) {
    MovePair_t *move = NULL;

    if (color == 1) {
        printf("Black to move\n");
        move = othelloPlayer_move(&game->blackPlayer, &game->board, game->board.moves, game->board.passes[0], game->moveHistory);
    } else if (color == -1) {
        printf("White to move\n");
        move = othelloPlayer_move(&game->whitePlayer, &game->board, game->board.moves, game->board.passes[0], game->moveHistory);
    }

    if (!game->board.passes[0]) {
        othelloBoard_updateBoard(&game->board, color, move);
    }
}


// Update status of the game
/**
 * @brief 检查游戏是否结束
 *
 * 如果双方都无法下棋，则游戏结束。
 *
 * 首先检查双方是否都已经放弃下棋（即 passes[0] 和 passes[1] 是否都为 true）。
 * 如果是，则计算黑棋和白棋的数量，然后显示棋盘，并宣布获胜方或平局。
 * 如果游戏未结束，则根据当前下棋方（passes[0]）的情况，更新棋盘上的棋子数量，
 * 并准备下一轮下棋（即切换 passes[0] 和 passes[1] 的值）。
 */
// void othelloGame::checkGameOver() {
//     // 如果双方都放弃了落子
//     if (this->board.passes[0] && this->board.passes[1]) {
//         // 统计黑子和白子的数量
//         // int blackCount = std::count(this->board.positions.begin(),
//         //         this->board.positions.end(), 1);
//         // int whiteCount = std::count(this->board.positions.begin(),
//         //         this->board.positions.end(), -1);

//         int blackCount = board_count(this->board.positions,1,64,1);
//         int whiteCount = board_count(this->board.positions,1,64,-1);

//         // 显示棋盘
//         this->board.displayBoard(1);

//         // 判断胜负
//         if (blackCount > whiteCount) {
//             std::cout << "Black wins!" << std::endl;
//         }
//         else if (blackCount < whiteCount) {
//             std::cout << "White wins!" << std::endl;
//         }
//         else {
//             std::cout << "Tie!" << std::endl;
//         }
//         // 输出双方棋子的数量
//         std::cout << "Black: " << blackCount << "\t"
//             << "White: " << whiteCount << std::endl;

//         // 设置游戏结束标志为true
//         this->gameOver = true;
//     }
//     else {
//         // 如果黑方未放弃落子
//         if (!this->board.passes[0]) {
//             // 棋盘上棋子数量加一
//             this->board.discsOnBoard++;
//         }

//         // 设置白方放弃标志为黑方放弃标志
//         this->board.passes[1] = this->board.passes[0];
//         // 重置黑方放弃标志为false
//         this->board.passes[0] = false;
//     }
// }

void othelloGame_checkGameOver(othelloGame *game) {
    if (game->board.passes[0] && game->board.passes[1]) {
        int blackCount = board_count(game->board.positions, 1, OTHELLO_BOARD_SIZE, 1);
        int whiteCount = board_count(game->board.positions, 1, OTHELLO_BOARD_SIZE, -1);

        othelloBoard_displayBoard(&game->board, 1);

        if (blackCount > whiteCount) {
            printf("Black wins!\n");
        } else if (blackCount < whiteCount) {
            printf("White wins!\n");
        } else {
            printf("Tie!\n");
        }
        printf("Black: %d\tWhite: %d\n", blackCount, whiteCount);
        game->gameOver = true;
    } else {
        if (!game->board.passes[0]) {
            game->board.discsOnBoard++;
        }
        game->board.passes[1] = game->board.passes[0];
        game->board.passes[0] = false;
    }
}
