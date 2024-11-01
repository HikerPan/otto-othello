#include "board.hpp"

// Constructor
/**
 * @brief 构造函数
 *
 * 初始化棋盘，将棋盘上的每个位置初始化为0。
 * positions是一个大小为64的数组，每个元素初始化为0。
 */
othelloBoard::othelloBoard() {
    // 初始化棋盘，将棋盘上的每个位置初始化为0
    // positions是一个大小为64的数组，每个元素初始化为0
    this->positions.resize(64, 0);
    // positions数组初始化完毕
}

// Display board: color is 1 for black, -1 for white
/**
 * @brief 显示棋盘
 *
 * 在控制台上显示棋盘的当前状态，包括黑棋、白棋、可选位置和分数。
 *
 * @param color 玩家颜色，1表示黑棋，-1表示白棋
 */
void othelloBoard::displayBoard(int color) {
    // 打印棋盘标题
    std::cout << "    A B C D E F G H" << std::endl;

    int row = 1;
    for (int i = 0; i < 64; i += 8) {
        // 打印棋盘行号
        // Green space
        std::cout << " " << row++ << " "
            << "\033[48;5;34m\033[38;5;232m \033[0m"; // 打印绿色空格

        // 遍历当前行的每一个格子
        for (int j = i; j < i+8; j++) {
            // 检查当前位置是否有棋子
            if (this->positions[j] == 1) {
                // 打印黑色棋子
                // Black disc followed by green space
                std::cout << "\033[48;5;34m\033[38;5;232m\u2022 \033[0m"; // 打印黑色棋子后跟绿色空格
            }
            else if (this->positions[j] == -1) {
                // 打印白色棋子
                // White disc followed by green space
                // std::cout << "\033[48;5;34m\033[38;5;256m\u2022 \033[0m"; // 打印白色棋子后跟绿色空格
                std::cout << "\033[48;5;34m\033[38;5;15m\u2022 \033[0m"; // 打印白色棋子后跟绿色空格
            }
            else if (this->moves.find(j) != this->moves.end() && color == 1) {
                // 打印黑色可落子标记
                // Black x followed by green space
                std::cout << "\033[48;5;34m\033[38;5;232m\u2613 \033[0m"; // 打印黑色X后跟绿色空格
            } 
            else if (this->moves.find(j) != this->moves.end() && color == -1) {
                // 打印白色可落子标记
                // White x followed by green space
                // std::cout << "\033[48;5;34m\033[38;5;256m\u2613 \033[0m"; // 打印白色X后跟绿色空格
                std::cout << "\033[48;5;34m\033[38;5;15m\u2613 \033[0m"; // 打印白色X后跟绿色空格
            }
            else {
                // 打印空白格子
                // Dot followed by green space
                std::cout << "\033[48;5;34m\033[38;5;232m\u00B7 \033[0m"; // 打印点后跟绿色空格
            }
        }

        // 在棋盘底部打印双方棋子数量
        if (i == 24) {
            std::cout << "\t\tBlack: " << std::count(this->positions.begin(),
                    this->positions.end(), 1); // 打印黑色棋子数量
        }
        else if (i == 32) {
            std::cout << "\t\tWhite: " << std::count(this->positions.begin(),
                    this->positions.end(), -1); // 打印白色棋子数量
        }

        std::cout << std::endl; // 换行
    }

    std::cout << std::endl; // 打印一个空行以分隔棋盘显示
}

// Display legal moves for player
/**
 * @brief 显示所有合法的移动
 *
 * 遍历所有合法的移动，并显示它们的编号、位置以及它们将翻转的棋子列表。
 *
 * @note 本函数没有返回值。
 */
void othelloBoard::displayLegalMoves() {
    // 定义列和行的坐标字符串
    std::string colCoord = "ABCDEFGH";
    std::string rowCoord = "12345678";

    int colNum = 0, rowNum = 0; // 列和行的索引变量
    int moveNum = 1; // 合法移动的编号
    std::list<int> flippedDiscs; // 记录翻转的棋子

    std::cout << "Legal moves:" << std::endl;

    // 遍历moves中的所有移动
    for (auto keyval : this->moves) {
        // 将索引转换为坐标
        index2coord(keyval.first, colNum, rowNum);
        std::cout << "\t" << moveNum++ << "\t" << colCoord[colNum] << rowCoord[rowNum];

        // 获取当前移动翻转的棋子列表
        flippedDiscs = keyval.second;
        std::cout << " will flip: ";

        // 遍历翻转的棋子列表
        for (int disc : flippedDiscs) {
            // 将索引转换为坐标
            index2coord(disc, colNum, rowNum);
            std::cout << colCoord[colNum] << rowCoord[rowNum] << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

// Finds all legal moves, writing to a reference to a hash table with
// legal moves as keys, and a list of all discs to be flipped as values.
/**
 * @brief 查找当前玩家可以下的合法走法
 *
 * 为给定颜色的玩家查找并返回所有可能的合法走法。
 *
 * @param color 当前玩家的颜色
 * @param pMoves 用于存储合法走法的指针，键为棋盘位置，值为可能的走法列表
 */
void othelloBoard::findLegalMoves(int color,
        std::unordered_map<int, std::list<int>> *pMoves) {
    // 清除上一手棋的合法走法
    // Clear legal moves from previous ply
    this->moves.clear();

    for (int i = 0; i < 64; i++) {
        if (this->positions[i] == color) {
            // 检查行
            // Check rows
            findLegalMoveInDirection(i, color, -1, pMoves);
            findLegalMoveInDirection(i, color, 1, pMoves);

            // 检查列
            // Check columns
            findLegalMoveInDirection(i, color, -8, pMoves);
            findLegalMoveInDirection(i, color, 8, pMoves);

            // 检查对角线
            // Check diagonals
            findLegalMoveInDirection(i, color, -9, pMoves);
            findLegalMoveInDirection(i, color, 9, pMoves);
            findLegalMoveInDirection(i, color, -7, pMoves);
            findLegalMoveInDirection(i, color, 7, pMoves);
        }
    }
}

// Helper function to find a legal move given a disc, its color and a direction.
// Writes the legal move and a list of all discs to be flipped as a pair to the
// reference to a hash table.
/**
 * @brief 在给定方向上查找合法的移动
 *
 * 在指定的方向上查找所有合法的移动，并记录这些移动可以翻转的棋子列表。
 *
 * @param disc 当前棋子的索引
 * @param color 当前棋子的颜色（非零值，正数代表白方，负数代表黑方）
 * @param direction 移动方向（正数或负数，表示向上、向下、向左或向右移动）
 * @param pMoves 指向存储合法移动和翻转棋子列表的哈希表的指针
 */
void othelloBoard::findLegalMoveInDirection(int &disc, int &color, int direction,
        std::unordered_map<int, std::list<int>> *pMoves) {
    // 初始化一个合法的移动和翻转的棋子列表
    std::pair<int, std::list<int>> legalMove;
    std::list<int> flippedDiscs;
    // 当前棋格位置初始化为0
    int currentSquare = 0;
    // 用于转换棋格索引到行列的临时变量
    int row1 = 0, col1 = 0, row2 = 0, col2 = 0;

    // 沿给定方向遍历棋格
    for (int i = disc + direction; i < 64 && i > -1; i += direction) {
        // 防止棋盘边缘的棋格越界
        // Guard against wrapping around the board
        index2coord(i-direction, col1, row1);
        index2coord(i, col2, row2);
        // 如果当前棋格和前一个棋格的行或列变化超过1，则跳出循环
        if (abs(col1 - col2) > 1 || abs(row1 - row2) > 1) {
            break;
        }

        // 沿给定方向继续移动，记住任何相反颜色的棋子。如果遇到相同颜色的棋子则跳出循环
        // Keep moving in given direction, remembering any discs of the
        // opposite color. Break if we see any discs of our color.
        currentSquare = this->positions[i];
        if (currentSquare == color ||
                (currentSquare == 0 && flippedDiscs.empty())) {
            break;
        }
        // 如果遇到相反颜色的棋子，则记录其位置
        else if (currentSquare == -color) {
            flippedDiscs.push_front(i);
            continue;
        }
        // 如果遇到一个空棋格，并且已经记录了翻转的棋子，则此位置是一个合法的移动
        // 插入到哈希表中，但需要先检查是否已经存在该移动
        // If we see an empty square, it is a legal move: insert it into the
        // moves hash table.
        // NB: we must check to see if the move is already in the map.
        // Second condition is to resolve edge case of
        // disc immediately adjacent to original disc.
        else if (currentSquare == 0 && !flippedDiscs.empty()) {
            std::unordered_map<int, std::list<int>>::iterator it = pMoves->find(i);

            // 如果该移动已经存在，则合并翻转的棋子列表
            if (it != pMoves->end()) {
                it->second.merge(flippedDiscs);
            }
            // 否则，插入新的合法移动和翻转的棋子列表
            else {
                legalMove.first = i;
                legalMove.second = flippedDiscs;
                pMoves->insert(legalMove);
            }

            // 跳出循环
            break;
        }
    }
}

// Update positions after a move
/**
 * @brief 更新棋盘状态
 *
 * 根据输入的移动信息更新棋盘状态。
 *
 * @param color 当前玩家颜色
 * @param move 移动信息，包含移动的位置和翻转的棋子列表
 *             - move.first 表示移动的位置
 *             - move.second 表示翻转的棋子列表
 */
void othelloBoard::updateBoard(int color, std::pair<int, std::list<int>> move) {
    // 获取移动的位置
    int square = move.first;
    // 获取翻转的棋子列表
    std::list<int> flippedDiscs = move.second;

    // 将移动位置设置为当前玩家颜色
    this->positions[square] = color;

    // 遍历翻转的棋子列表
    for (auto disc : flippedDiscs) {
        // 将翻转的棋子位置设置为当前玩家颜色
        this->positions[disc] = color;
    }
}

// Checks if game is a terminal state
/**
 * @brief 判断棋盘是否到达终局状态
 *
 * 检查两位玩家是否都放弃了当前回合，如果都放弃了，则棋盘到达终局状态
 *
 * @return 如果棋盘到达终局状态，返回true；否则返回false
 */
bool othelloBoard::terminalState() {
    // 如果两位玩家都选择了放弃，则游戏结束
    if (this->passes[0] && this->passes[1]) {
        // 两位玩家都选择了放弃，返回true表示游戏结束
        return true;
    }
    // 否则，游戏尚未结束，返回false
    return false;
}

// Helper function to convert board square index to coordinates
/**
 * @brief 将棋盘上的索引转换为行列号
 *
 * 将给定的索引值转换为对应的列号和行号。
 *
 * @param index 给定的索引值，索引值范围从0到63。
 * @param colNum 用于存储转换后的列号，范围从0到7。
 * @param rowNum 用于存储转换后的行号，范围从0到7。
 */
void othelloBoard::index2coord(int index, int &colNum, int &rowNum) {
    // 将index除以8取余数得到列号
    // 计算列号
    colNum = index % 8;
    // 将index除以8取整得到行号
    // 计算行号
    rowNum = index / 8;
}
