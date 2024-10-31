#include "game.hpp"

// Constructor
/**
 * @brief 构造函数
 *
 * 初始化国际象棋游戏棋盘。
 * 将棋盘大小设置为64个位置，并将所有位置初始化为0。
 */
othelloGame::othelloGame() {
    // 初始化棋盘，将棋盘上的所有位置都初始化为0
    // 0代表该位置为空
    this->board.positions.resize(64, 0);
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
void othelloGame::newGame(bool blackComputer, bool whiteComputer,
        float timeLimit) {
    // 初始化棋盘
    // Initialize board
    std::vector<int> setup(64, 0);
    setup[27] = -1;
    setup[28] = 1;
    setup[35] = 1;
    setup[36] = -1;
    this->board.positions.swap(setup);

    // 初始化玩家
    // Initialize players
    this->blackPlayer.color = 1;
    this->blackPlayer.computer = blackComputer;
    this->whitePlayer.color = -1;
    this->whitePlayer.computer = whiteComputer;

    // 初始化当前玩家
    // Initialize player to move
    this->toMove = 1;

    // 初始化时间限制
    // Initialize time limit
    this->board.timeLimit = timeLimit;
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
void othelloGame::loadGame(std::string fileName, bool blackComputer,
        bool whiteComputer) {
    std::ifstream ifs(fileName.c_str());

    if (!ifs.good()) {
        std::cout << "File does not exist!" << std::endl;
        return;
    }

    // Load board
    std::vector<int> setup(64, 0);
    std::string str;
    char ch;
    int idx = 0;

    for (int i = 0; i < 8; i++) {
        std::getline(ifs, str);
        for (int j = 0; j < 16; j += 2) {
            ch = str[j];
            if (ch == '1') {
                setup[idx] = 1;
            }
            else if (ch == '2') {
                setup[idx] = -1;
            }
            else if (ch == '0') {
                setup[idx] = 0;
            }
            else {
                std::cout << "Invalid file format! Refer to the README." << std::endl;
                return;
            }

            idx++;
        }
    }
    this->board.discsOnBoard = 64 - std::count(setup.begin(), setup.end(), 0);
    this->board.positions.swap(setup);

    // Initialize players
    this->blackPlayer.color = 1;
    this->blackPlayer.computer = blackComputer;
    this->whitePlayer.color = -1;
    this->whitePlayer.computer = whiteComputer;

    // Load player to move
    if (std::getline(ifs, str)) {
        ch = str[0];
        if (ch == '1') {
            this->toMove = 1;
        }
        else if (ch == '2') {
            this->toMove = -1;
        }
        else {
            std::cout << "Player to move must be 1 (black) or 2 (white)!"
                << std::endl;
            ifs.close();
            return;
        }
    }
    else {
        std::cout << "Save file does not specify player to move!"
            << std::endl;
        ifs.close();
        return;
    }

    // Load time limit
    if (std::getline(ifs, str)) {
        if (stof(str) > 0) {
            this->board.timeLimit = stof(str);
        }
        else {
            std::cout << "Time limit must be a positive number!"
                << std::endl;
            ifs.close();
            return;
        }
    }
    else {
        std::cout << "Save file does not specify computer time limit!"
            << std::endl;
        ifs.close();
        return;
    }

    ifs.close();
}

// Make a move
/**
 * @brief 在五子棋游戏中执行一步棋
 *
 * 根据玩家颜色，调用对应的玩家对象进行移动，并更新棋盘状态。
 *
 * @param color 玩家颜色，1 表示黑方，-1 表示白方
 */
void othelloGame::move(int color) {
    // 定义一个pair类型变量move，用于存储移动结果
    std::pair<int, std::list<int>> move;

    // 判断当前轮到哪方玩家下棋
    if (color == 1) {
        // 如果是黑方下棋
        std::cout << "Black to move" << std::endl;
        // 调用黑方玩家的move方法，获取移动结果
        move = this->blackPlayer.move(this->board, this->board.moves,
                this->board.passes[0], this->moveHistory);
    } 
    else if (color == -1) {
        // 如果是白方下棋
        std::cout << "White to move" << std::endl;
        // 调用白方玩家的move方法，获取移动结果
        move = this->whitePlayer.move(this->board, this->board.moves,
                this->board.passes[0], this->moveHistory);
    }

    // 判断当前是否允许下棋
    if (!this->board.passes[0]) {
        // 如果允许下棋，则更新棋盘
        this->board.updateBoard(color, move);
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
void othelloGame::checkGameOver() {
    // 如果双方都放弃了落子
    if (this->board.passes[0] && this->board.passes[1]) {
        // 统计黑子和白子的数量
        int blackCount = std::count(this->board.positions.begin(),
                this->board.positions.end(), 1);
        int whiteCount = std::count(this->board.positions.begin(),
                this->board.positions.end(), -1);

        // 显示棋盘
        this->board.displayBoard(1);

        // 判断胜负
        if (blackCount > whiteCount) {
            std::cout << "Black wins!" << std::endl;
        }
        else if (blackCount < whiteCount) {
            std::cout << "White wins!" << std::endl;
        }
        else {
            std::cout << "Tie!" << std::endl;
        }
        // 输出双方棋子的数量
        std::cout << "Black: " << blackCount << "\t"
            << "White: " << whiteCount << std::endl;

        // 设置游戏结束标志为true
        this->gameOver = true;
    }
    else {
        // 如果黑方未放弃落子
        if (!this->board.passes[0]) {
            // 棋盘上棋子数量加一
            this->board.discsOnBoard++;
        }

        // 设置白方放弃标志为黑方放弃标志
        this->board.passes[1] = this->board.passes[0];
        // 重置黑方放弃标志为false
        this->board.passes[0] = false;
    }
}
