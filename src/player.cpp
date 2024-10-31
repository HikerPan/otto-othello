#include "heuristic.hpp"
#include "player.hpp"

// Driver for player's move, regardless of player
/**
 * @brief 执行玩家的移动操作
 *
 * 根据玩家类型（电脑或人类），选择不同的移动策略，并更新历史移动记录。
 *
 * @param board 游戏棋盘
 * @param legalMoves 合法的移动选项
 * @param pass 是否轮到对手弃权
 * @param moveHistory 历史移动记录
 * @return 返回移动结果，包含移动的行索引和列索引列表
 */
std::pair<int, std::list<int>> othelloPlayer::move(othelloBoard &board,
        std::unordered_map<int, std::list<int>> &legalMoves,
        bool &pass, std::string &moveHistory) {

    // 初始化移动选择
    std::pair<int, std::list<int>> moveChoice;

    // 如果是电脑玩家
    if (this->computer) {
        // 执行电脑玩家的移动逻辑
        moveChoice = this->computerMove(board, legalMoves, pass, moveHistory);
    }
    // 如果是人类玩家
    else {
        // 执行人类玩家的移动逻辑
        moveChoice = this->humanMove(legalMoves, pass);
    }

    // 将移动记录添加到历史记录中
    moveHistory.append(std::to_string(moveChoice.first) + ",");

    // 返回移动选择
    return moveChoice;
}

// Prompts user for next move
/**
 * @brief 人机对战中，玩家选择合法走法
 *
 * 该函数用于玩家在对弈过程中选择合法的走法。如果当前没有合法的走法，则提示玩家输入任意非空字符串表示跳过回合。
 *
 * @param legalMoves 当前所有合法的走法
 * @param pass 是否选择跳过回合
 * @return std::pair<int, std::list<int>> 玩家选择的走法，第一个元素为走法的编号或坐标索引，第二个元素为对应的走法列表
 */
std::pair<int, std::list<int>> othelloPlayer::humanMove(
        std::unordered_map<int, std::list<int>> &legalMoves, bool &pass) {
    // 存储用户输入的字符串
    std::string str;
    // 存储用户选择的移动
    std::pair<int, std::list<int>> move;
    // 记录用户输入的移动编号
    int moveNum = 0;
    // 记录用户输入的坐标索引
    int coordIndex = -1;
    // 记录用户输入是否有效
    bool validInput = false;

    // 如果没有合法移动
    if (legalMoves.empty()) {
        // 输出没有合法移动的信息
        std::cout << "No legal moves!" << std::endl;
        // 提示用户输入任意非空字符串表示放弃
        std::cout << "\tEnter any non-empty string to pass: ";
        // 读取用户输入的字符串
        std::cin >> str;
        // 清除输入缓冲区
        while (std::cin.get() != '\n');          // clear buffer
        // 输出换行符
        std::cout << std::endl;
        // 设置pass为true表示用户选择放弃
        pass = true;
        // 返回空的移动
        return move;
    }

    // 循环直到用户输入有效
    do {
        // 提示用户选择移动编号或坐标
        std::cout << "\tSelect move number/square coordinate: ";
        // 读取用户输入的字符串
        std::cin >> str;
        // 清除输入缓冲区
        while (std::cin.get() != '\n');          // clear buffer

        // 将输入的字符串转换为坐标索引
        coordIndex = coord2index(str);
        // 创建输入字符串流
        std::istringstream iss(str);
        // 尝试从输入字符串中读取移动编号
        iss >> moveNum;

        // 如果坐标索引有效且是合法移动
        if (coordIndex != -1
                && legalMoves.find(coordIndex) != legalMoves.end()) {
            // 输出换行符
            std::cout << std::endl;
            // 返回用户选择的移动
            return *legalMoves.find(coordIndex);
        }
        // 如果输入既不是坐标也不是有效编号
        else if (!iss.eof() || moveNum > legalMoves.size() || moveNum < 1) {
            // 提示用户输入无效，请重新输入
            std::cout << "\tInvalid input. Please try again.\n" << std::endl;
        }
        // 如果输入是有效编号但不在合法移动列表中
        else {
            // 设置validInput为true表示输入有效
            validInput = true;
            // 输出换行符
            std::cout << std::endl;
        }
    }
    // 循环直到validInput为true
    while (!validInput);

    // 遍历合法移动列表
    int i = 0;
    for (std::pair<int, std::list<int>> keyval : legalMoves) {
        // 保存当前遍历到的移动
        move = keyval;
        // 递增计数器
        i++;
        // 如果计数器等于用户输入的编号
        if (i == moveNum) {
            // 跳出循环
            break;
        }
    }

    // 返回用户选择的移动
    return move;
}

/**
 * @brief 将坐标字符串转换为棋盘上的索引
 *
 * 将一个表示棋盘坐标的字符串转换为棋盘上的索引。棋盘大小为8x8，坐标以字符串形式给出，如"A1"、"B2"等。
 *
 * @param coord 坐标字符串，格式为'列号行号'，列号为大写或小写字母（A-H），行号为数字（1-8）
 * @return 索引值，如果坐标无效，则返回-1
 */
int othelloPlayer::coord2index(std::string coord) {
    // 检查坐标字符串长度是否为2
    if (coord.length() != 2) {
        return -1;
    }

    int index = 0;
    // 根据坐标的第一个字符确定行索引
    switch(coord.at(0)) {
        case 'A':
        case 'a':
            // A或a代表第一行
            index = 0;
            break;
        case 'B':
        case 'b':
            // B或b代表第二行
            index = 1;
            break;
        case 'C':
        case 'c':
            // C或c代表第三行
            index = 2;
            break;
        case 'D':
        case 'd':
            // D或d代表第四行
            index = 3;
            break;
        case 'E':
        case 'e':
            // E或e代表第五行
            index = 4;
            break;
        case 'F':
        case 'f':
            // F或f代表第六行
            index = 5;
            break;
        case 'G':
        case 'g':
            // G或g代表第七行
            index = 6;
            break;
        case 'H':
        case 'h':
            // H或h代表第八行
            index = 7;
            break;
        default:
            // 如果第一个字符不在A-H之间，则返回-1
            return -1;
    }

    // 根据坐标的第二个字符确定列索引
    switch(coord.at(1)) {
        case '1':
            // 1代表第一列
            index += 0;
            break;
        case '2':
            // 2代表第二列
            index += 8;
            break;
        case '3':
            // 3代表第三列
            index += 16;
            break;
        case '4':
            // 4代表第四列
            index += 24;
            break;
        case '5':
            // 5代表第五列
            index += 32;
            break;
        case '6':
            // 6代表第六列
            index += 40;
            break;
        case '7':
            // 7代表第七列
            index += 48;
            break;
        case '8':
            // 8代表第八列
            index += 56;
            break;
        default:
            // 如果第二个字符不在1-8之间，则返回-1
            return -1;
    }

    return index;
}

// Driver for the AI algorithm
/**
 * @brief 电脑进行一步棋
 *
 * 根据棋盘、合法走法、是否过路以及走法历史记录，电脑进行一步棋。
 *
 * @param board 棋盘对象
 * @param legalMoves 当前棋盘状态下所有合法的走法
 * @param pass 是否过路
 * @param moveHistory 走法历史记录
 * @return 返回电脑走法的行列索引对
 */
std::pair<int, std::list<int>> othelloPlayer::computerMove(othelloBoard &board,
        std::unordered_map<int, std::list<int>> &legalMoves, bool &pass,
        std::string &moveHistory) {
    // 开始计时
    std::chrono::time_point<std::chrono::system_clock> startTime
        = this->startTimer();

    // 初始化移动对象
    std::pair<int, std::list<int>> move;
    std::pair<int, std::list<int>> bestMove;

    // 查询开局数据库
    std::unordered_map<std::string, int>::iterator query
        = this->database.openingBook.find(moveHistory);

    // 如果没有合法移动
    if (legalMoves.empty()) {
        std::cout << "No legal moves!" << std::endl;
        std::cout << "\tComputer passes.\n" << std::endl;
        pass = true;
        return bestMove;
    }
    // 如果只有一个合法移动
    else if (legalMoves.size() == 1) {
        std::cout << "Only one legal move!" << std::endl;
        std::cout << "\tComputer takes only legal move." << std::endl;
        bestMove = *legalMoves.begin();
    }
    // 如果开局已知
    else if (query != this->database.openingBook.end()) {
        std::cout << "Known opening!" << std::endl;
        std::cout << "\tComputer takes next move from opening book."
            << std::endl;
        bestMove = *legalMoves.find(query->second);
    }
    // 其他情况
    else {
        // 计算最大搜索深度
        int maxDepth = 64 - board.discsOnBoard;

        // 如果最大深度小于10，则搜索到终端状态
        if (maxDepth < 10) {
            // 搜索到终端状态
            std::cout << "Searching remainder of game tree..." << std::endl;
            std::cout << "\tSearching to depth " << maxDepth;

            bestMove = this->depthLimitedAlphaBeta(board, maxDepth, startTime,
                    board.timeLimit);

            std::cout << "\t\tSearch complete." << std::endl;
        }
        // 否则，使用迭代加深搜索
        else {
            // 搜索游戏树
            std::cout << "Searching game tree..." << std::endl;

            // 迭代加深搜索
            for (int depthLimit = 1; depthLimit <= maxDepth; depthLimit++) {
                std::cout << "\tSearching to depth " << depthLimit;

                move = this->depthLimitedAlphaBeta(board, depthLimit, startTime,
                        board.timeLimit);

                // 如果搜索被中止
                if (move.first == -1) {
                    std::cout << "\t\tSearch aborted." << std::endl;
                    break;
                }
                // 否则，更新最佳移动
                else {
                    std::cout << "\t\tSearch complete." << std::endl;
                    bestMove = move;
                }

                // 如果时间过半，则停止搜索
                if (this->stopTimer(startTime) > 0.5*board.timeLimit) {
                    break;
                }
            }
        }
    }

    // 打印消耗时间
    std::cout << "\tTime elapsed: " << this->stopTimer(startTime) << " sec"
        << std::endl;

    // 将索引转换为坐标并打印
    int rowNum = 0, colNum = 0;
    std::string colCoord = "ABCDEFGH";
    std::string rowCoord = "12345678";
    board.index2coord(bestMove.first, colNum, rowNum);
    std::cout << "\tComputer takes: " << colCoord[colNum] << rowCoord[rowNum]
        << "\n" << std::endl;

    // 返回最佳移动
    return bestMove;
}

// Returns time point
/**
 * @brief 开始计时
 *
 * 使用系统时钟获取当前时间作为计时起点，并返回这个时间点。
 *
 * @return std::chrono::time_point<std::chrono::system_clock> 计时起点的时间点
 */
std::chrono::time_point<std::chrono::system_clock> othelloPlayer::startTimer() {
    // 返回当前系统时钟的时间点
    return std::chrono::system_clock::now();
    // startTimer 函数结束
}

// Returns time elapsed in seconds
/**
 * @brief 停止计时器并返回经过的时间（秒）
 *
 * 根据传入的开始时间，计算从开始时间到当前时间的总经过时间（秒），并返回。
 *
 * @param startTime 开始时间
 * @return 返回从startTime到当前时间的经过时间（秒）
 */
float othelloPlayer::stopTimer(
        std::chrono::time_point<std::chrono::system_clock> startTime) {
    // 获取当前时间作为结束时间
    std::chrono::time_point<std::chrono::system_clock> endTime =
        std::chrono::system_clock::now();

    // 计算时间差，单位为秒
    // 创建一个持续时间的实例，其值为endTime与startTime的差值，单位为秒
    std::chrono::duration<float> elapsedSeconds = endTime - startTime;

    // 返回经过的时间（秒）
    return elapsedSeconds.count();
}

// Performs depth-limited minimax search with alpha-beta pruning
// Implemented iteratively to avoid recursion overhead
// Returns move for square -1 if time runs out
// TODO implement killer move heuristic
/**
 * @brief 在给定棋盘和时间限制下，使用深度限制的α-β剪枝算法搜索最佳走法
 *
 * 使用深度限制的α-β剪枝算法在给定棋盘和时间限制下搜索最佳走法。算法通过递归搜索可能的走法，并评估每种走法的启发式值，最终返回最佳走法。
 *
 * @param board 当前棋盘状态
 * @param depthLimit 搜索的最大深度
 * @param startTime 开始搜索的时间点
 * @param timeLimit 搜索的最大时间限制（秒）
 * @return 返回最佳走法，由位置坐标和可能的后续走法列表组成
 */
std::pair<int, std::list<int>> othelloPlayer::depthLimitedAlphaBeta(
        othelloBoard &board, int depthLimit,
        std::chrono::time_point<std::chrono::system_clock> startTime,
        float timeLimit) {

    // 初始化根节点
    // Initialize root node
    this->nodeStack[0].isMaxNode = true;
    this->nodeStack[0].alpha = INT_MIN;
    this->nodeStack[0].beta = INT_MAX;
    this->nodeStack[0].score = INT_MIN;
    this->nodeStack[0].board = board;
    this->nodeStack[0].moveIterator = this->nodeStack[0].board.moves.begin();
    this->nodeStack[0].prevIterator = this->nodeStack[0].moveIterator;
    this->nodeStack[0].lastMove = this->nodeStack[0].board.moves.end();

    int depth = 0;
    int leafScore = 0;
    std::unordered_map<int, std::list<int>>::iterator bestMove =
        this->nodeStack[0].board.moves.begin();

    // 当尚未评估根节点的所有子节点时
    // While we have not evaluated all the root's children
    while (true) {
        // 如果已评估完所有子节点
        // If we have evaluated all children
        if (this->nodeStack[depth].moveIterator
                == this->nodeStack[depth].lastMove) {
            if (depth-- == 0) {
                if (this->nodeStack[1].score > this->nodeStack[0].score
                        || (this->nodeStack[1].score == this->nodeStack[0].score
                            && rand() % 2 == 0)) {
                    this->nodeStack[0].score = this->nodeStack[1].score;
                    bestMove = this->nodeStack[0].prevIterator;
                }

                if (this->nodeStack[0].score > this->nodeStack[0].alpha) {
                    this->nodeStack[0].alpha = this->nodeStack[0].score;
                }

                break;
            }

            if (this->nodeStack[depth].isMaxNode) {
                if (this->nodeStack[depth+1].score > this->nodeStack[depth].score
                        || (this->nodeStack[depth+1].score == this->nodeStack[depth].score
                            && rand() % 2 == 0)) {
                    this->nodeStack[depth].score = this->nodeStack[depth+1].score;
                    if (depth == 0) {
                        bestMove = this->nodeStack[0].prevIterator;
                    }
                }

                if (this->nodeStack[depth].score > this->nodeStack[depth].alpha) {
                    this->nodeStack[depth].alpha = this->nodeStack[depth].score;
                }
            }
            else {
                if (this->nodeStack[depth+1].score < this->nodeStack[depth].score) {
                    this->nodeStack[depth].score = this->nodeStack[depth+1].score;
                }

                if (this->nodeStack[depth].score < this->nodeStack[depth].beta) {
                    this->nodeStack[depth].beta = this->nodeStack[depth].score;
                }
            }
        }
        // 如果可以剪枝
        // If we can prune
        else if (this->nodeStack[depth].beta <= this->nodeStack[depth].alpha) {
            if (depth-- == 0) {
                if (this->nodeStack[1].score > this->nodeStack[0].score
                    || (this->nodeStack[1].score == this->nodeStack[0].score
                        && rand() % 2 == 0)) {
                    this->nodeStack[0].score = this->nodeStack[1].score;
                    bestMove = this->nodeStack[0].prevIterator;
                }

                if (this->nodeStack[0].score > this->nodeStack[0].alpha) {
                    this->nodeStack[0].alpha = this->nodeStack[0].score;
                }

                //this->killerMoves[1][1] = this->killerMoves[1][0];
                //this->killerMoves[1][0] = this->nodeStack[1].prevIterator->first;

                break; // FIXME should it be break or continue here???
            }

            if (this->nodeStack[depth].isMaxNode) {
                if (this->nodeStack[depth+1].score > this->nodeStack[depth].score
                    || (this->nodeStack[depth+1].score == this->nodeStack[depth].score
                        && rand() % 2 == 0)) {
                    this->nodeStack[depth].score = this->nodeStack[depth+1].score - 1;
                    if (depth == 0) {
                        bestMove = this->nodeStack[0].prevIterator;
                    }
                }

                if (this->nodeStack[depth].score > this->nodeStack[depth].alpha) {
                    this->nodeStack[depth].alpha = this->nodeStack[depth].score;
                }

                //this->killerMoves[depth+1][1] = this->killerMoves[depth+1][0];
                //this->killerMoves[depth+1][0] = this->nodeStack[depth+1].prevIterator->first;
            }
            else {
                if (this->nodeStack[depth+1].score < this->nodeStack[depth].score) {
                    this->nodeStack[depth].score = this->nodeStack[depth+1].score + 1;
                }

                if (this->nodeStack[depth].score < this->nodeStack[depth].beta) {
                    this->nodeStack[depth].beta = this->nodeStack[depth].score;
                }
            }
        }
        else {
            // 生成下一个节点，增加迭代器
            // Generate next node, increment iterators
            this->nodeStack[depth+1].board = this->nodeStack[depth].board;
            this->nodeStack[depth+1].board.updateBoard(
                    (this->nodeStack[depth].isMaxNode ? this->color : -this->color),
                    *this->nodeStack[depth].moveIterator);
            this->nodeStack[depth].prevIterator = this->nodeStack[depth].moveIterator;
            this->nodeStack[depth].moveIterator++;

            // 如果下一个深度未达到深度限制
            // If the next depth is not at the depth limit
            if (depth + 1 < depthLimit) {
                depth++;

                // 初始化栈中的下一个节点
                // Initialize next node in stack
                this->nodeStack[depth].isMaxNode = !this->nodeStack[depth-1].isMaxNode;
                this->nodeStack[depth].score =
                    (this->nodeStack[depth].isMaxNode ? INT_MIN : INT_MAX);
                this->nodeStack[depth].alpha = this->nodeStack[depth-1].alpha;
                this->nodeStack[depth].beta = this->nodeStack[depth-1].beta;
                this->nodeStack[depth].board.findLegalMoves(
                        (this->nodeStack[depth].isMaxNode ? this->color : -this->color),
                        &this->nodeStack[depth].board.moves);

                /*
                std::unordered_map<int, std::list<int>> foo1
                    = this->nodeStack[depth].board.moves.find(this->killerMoves[depth][0]);
                std::unordered_map<int, std::list<int>> foo2
                    = this->nodeStack[depth].board.moves.find(this->killerMoves[depth][1]);

                if (foo1 != this->nodeStack[depth].lastMove
                    && foo2 != this->nodeStack[depth].lastMove) {
                    std::iter_swap(this->nodeStack[depth].board.moves.begin(), foo1);
                    std::iter_swap(std::next(this->nodeStack[depth].board.moves.begin()), foo2);
                }
                else if (foo1 != this->nodeStack[depth].lastMove) {
                    std::iter_swap(this->nodeStack[depth].board.moves.begin(), foo1);
                }
                else if (foo2 != this->nodeStack[depth].lastMove) {
                    std::iter_swap(this->nodeStack[depth].board.moves.begin(), foo2);
                }
                */

                this->nodeStack[depth].moveIterator =
                    this->nodeStack[depth].board.moves.begin();
                this->nodeStack[depth].prevIterator =
                    this->nodeStack[depth].moveIterator;
                this->nodeStack[depth].lastMove = this->nodeStack[depth].board.moves.end();
            }
            else {
                // 节点为叶节点：评估启发式函数并更新值
                // The node is a leaf: evaluate heuristic and update values
                leafScore = this->heuristic.evaluate(
                        this->nodeStack[depth+1].board, this->color);

                if (this->nodeStack[depth].isMaxNode) {
                    if (leafScore > this->nodeStack[depth].score) {
                        this->nodeStack[depth].score = leafScore;
                        if (depth == 0) {
                            bestMove = this->nodeStack[0].prevIterator;
                        }
                    }

                    if (this->nodeStack[depth].score > this->nodeStack[depth].alpha) {
                        this->nodeStack[depth].alpha = this->nodeStack[depth].score;
                    }
                }
                else {
                    if (leafScore < this->nodeStack[depth].score) {
                        this->nodeStack[depth].score = leafScore;
                    }

                    if (this->nodeStack[depth].score < this->nodeStack[depth].beta) {
                        this->nodeStack[depth].beta = this->nodeStack[depth].score;
                    }
                }
            }
        }

        // 如果时间即将耗尽，则失败
        // If we are almost out of time, failure
        if (this->stopTimer(startTime) > 0.998*timeLimit) {
            std::pair<int, std::list<int>> move;
            move.first = -1;
            return move;
        }
    }

    return *bestMove;
}
