#include "heuristic.hpp"
// Heuristic is from the perspective that the calling player is the maximizing
// player, regardless of the player's color

int othelloHeuristic::evaluate(othelloBoard &board, int color) {
    if (board.terminalState()) {
        return 100000*utility(board, color);
    }

    if (board.discsOnBoard <= 20) {
        // Opening game
        return 5*mobility(board, color)
            + 5*potentialMobility(board, color)
            + 20*squareWeights(board, color)
            + 10000*corners(board, color)
            + 10000*stability(board, color);
    }
    else if (board.discsOnBoard <= 58) {
        // Midgame
        return 10*discDifference(board, color)
            + 2*mobility(board, color)
            + 2*potentialMobility(board, color)
            + 10*squareWeights(board, color)
            + 100*parity(board)
            + 10000*corners(board, color)
            + 10000*stability(board, color);
    }
    else {
        // Endgame
        return 500*discDifference(board, color)
            + 500*parity(board)
            + 10000*corners(board, color)
            + 10000*stability(board, color);
    }
}

int othelloHeuristic::utility(othelloBoard &board, int &color) {
    int util = std::accumulate(board.positions.begin(),
            board.positions.end(), 0);

    if (color == 1) {
        return util; 
    }
    else {
        return -1*util;
    }
}

// Relative disc difference between the two players
int othelloHeuristic::discDifference(othelloBoard &board, int &color) {
    int blackCount = std::count(board.positions.begin(),
            board.positions.end(), 1);
    int whiteCount = std::count(board.positions.begin(),
            board.positions.end(), -1);

    if (color == 1) {
        return 100 * (blackCount - whiteCount) / (blackCount + whiteCount);
    }
    else {
        return 100 * (whiteCount - blackCount) / (blackCount + whiteCount);
    }
}

// Number of possible moves
int othelloHeuristic::mobility(othelloBoard &board, int &color) {
    board.findLegalMoves(1, &pMoves);
    int blackMoves = pMoves.size();
    pMoves.clear();

    board.findLegalMoves(-1, &pMoves);
    int whiteMoves = pMoves.size();
    pMoves.clear();

    if (color == 1) {
        return 100 * (blackMoves - whiteMoves) / (blackMoves + whiteMoves + 1);
    }
    else {
        return 100 * (whiteMoves - blackMoves) / (blackMoves + whiteMoves + 1);
    }
}


/**
 * 计算给定颜色的玩家在棋盘上的潜在行动力
 * 
 * @param board 棋盘对象，包含当前棋盘的布局信息
 * @param color 玩家的颜色，1 表示黑色，-1 表示白色
 * @return 玩家的潜在行动力值
 */
int othelloHeuristic::potentialMobility(othelloBoard &board, int color) {
    int myPotentialMobility = playerPotentialMobility(board, color);
    int opponentPotentialMobility = playerPotentialMobility(board, -color);

    return 100 * (myPotentialMobility - opponentPotentialMobility)
        / (myPotentialMobility + opponentPotentialMobility + 1);
}



/**
 * 计算给定颜色的玩家在棋盘上的潜在行动力
 * 
 * @param board 棋盘对象，包含当前棋盘的布局信息
 * @param color 玩家的颜色，1 表示黑色，-1 表示白色
 * @return 玩家的潜在行动力值
 */
int othelloHeuristic::playerPotentialMobility(othelloBoard &board, int color) {
    // 定义棋盘内部的位置，这些位置可能存在潜在的行动力
    std::vector<int> boardInterior = {18, 19, 20, 21,
                                      26, 27, 28, 29,
                                      34, 35, 36, 37,
                                      42, 43, 44, 45};

    // 初始化各种方向的潜在行动力计数器
    int here = 0, up = 0, down = 0, left = 0, right = 0,
        upperLeft = 0, upperRight = 0, lowerLeft = 0, lowerRight = 0;
    int potentialMobility = 0;

    // 遍历棋盘内部的每个位置
    for (int square : boardInterior) {
        // 获取当前位置的棋子颜色
        here = board.positions[square];
        // 获取当前位置上方的棋子颜色
        up = board.positions[square-8];
        // 获取当前位置下方的棋子颜色
        down = board.positions[square+8];
        // 获取当前位置左侧的棋子颜色
        left = board.positions[square-1];
        // 获取当前位置右侧的棋子颜色
        right = board.positions[square+1];/**
 * 计算给定颜色的玩家在棋盘上的潜在行动力
 * 
 * @param board 棋盘对象，包含当前棋盘的布局信息
 * @param color 玩家的颜色，1 表示黑色，-1 表示白色
 * @return 玩家的潜在行动力值
 */
int othelloHeuristic::potentialMobility(othelloBoard &board, int color) {
    int myPotentialMobility = playerPotentialMobility(board, color);
    int opponentPotentialMobility = playerPotentialMobility(board, -color);

    return 100 * (myPotentialMobility - opponentPotentialMobility)
        / (myPotentialMobility + opponentPotentialMobility + 1);
}

        // 获取当前位置左上方的棋子颜色
        upperLeft = board.positions[square-9];
        // 获取当前位置右上方的棋子颜色
        upperRight = board.positions[square-7];
        // 获取当前位置左下方的棋子颜色
        lowerLeft = board.positions[square+7];
        // 获取当前位置右下方的棋子颜色
        lowerRight = board.positions[square+9];

        // 如果当前位置的棋子颜色与玩家颜色相反，且上方没有棋子，则增加潜在行动力
        if (here == -color && up == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且下方没有棋子，则增加潜在行动力
        if (here == -color && down == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且右侧没有棋子，则增加潜在行动力
        if (here == -color && right == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且左侧没有棋子，则增加潜在行动力
        if (here == -color && left == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且左上方没有棋子，则增加潜在行动力
        if (here == -color && upperLeft == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且右上方没有棋子，则增加潜在行动力
        if (here == -color && upperRight == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且左下方没有棋子，则增加潜在行动力
        if (here == -color && lowerLeft == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且右下方没有棋子，则增加潜在行动力
        if (here == -color && lowerRight == 0)
            potentialMobility++;
    }

    // 定义棋盘顶部的位置，这些位置可能存在潜在的行动力
    std::vector<int> topRow = {10, 11, 12, 13};
    // 遍历棋盘顶部的每个位置
    for (int square : topRow) {
        // 获取当前位置的棋子颜色
        here = board.positions[square];
        // 获取当前位置左侧的棋子颜色
        left = board.positions[square-1];
        // 获取当前位置右侧的棋子颜色
        right = board.positions[square+1];
        // 如果当前位置的棋子颜色与玩家颜色相反，且左侧没有棋子，则增加潜在行动力
        if (here == -color && left == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且右侧没有棋子，则增加潜在行动力
        if (here == -color && right == 0)
            potentialMobility++;
    }

    // 定义棋盘底部的位置，这些位置可能存在潜在的行动力
    std::vector<int> bottomRow = {50, 51, 52, 53};
    // 遍历棋盘底部的每个位置
    for (int square : bottomRow) {
        // 获取当前位置的棋子颜色
        here = board.positions[square];
        // 获取当前位置左侧的棋子颜色
        left = board.positions[square-1];
        // 获取当前位置右侧的棋子颜色
        right = board.positions[square+1];
        // 如果当前位置的棋子颜色与玩家颜色相反，且左侧没有棋子，则增加潜在行动力
        if (here == -color && left == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且右侧没有棋子，则增加潜在行动力
        if (here == -color && right == 0)
            potentialMobility++;
    }

    // 定义棋盘左侧的位置，这些位置可能存在潜在的行动力
    std::vector<int> leftColumn = {17, 25, 33, 41};
    // 遍历棋盘左侧的每个位置
    for (int square : leftColumn) {
        // 获取当前位置的棋子颜色
        here = board.positions[square];
        // 获取当前位置上方的棋子颜色
        up = board.positions[square-8];
        // 获取当前位置下方的棋子颜色
        down = board.positions[square+8];
        // 如果当前位置的棋子颜色与玩家颜色相反，且上方没有棋子，则增加潜在行动力
        if (here == -color && up == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且下方没有棋子，则增加潜在行动力
        if (here == -color && down == 0)
            potentialMobility++;
    }

    // 定义棋盘右侧的位置，这些位置可能存在潜在的行动力
    std::vector<int> rightColumn = {22, 30, 38, 46};
    // 遍历棋盘右侧的每个位置
    for (int square : leftColumn) {
        // 获取当前位置的棋子颜色
        here = board.positions[square];
        // 获取当前位置上方的棋子颜色
        up = board.positions[square-8];
        // 获取当前位置下方的棋子颜色
        down = board.positions[square+8];
        // 如果当前位置的棋子颜色与玩家颜色相反，且上方没有棋子，则增加潜在行动力
        if (here == -color && up == 0)
            potentialMobility++;
        // 如果当前位置的棋子颜色与玩家颜色相反，且下方没有棋子，则增加潜在行动力
        if (here == -color && down == 0)
            potentialMobility++;
    }

    // 返回玩家的潜在行动力值
    return potentialMobility;
}


// Computes a lower bound on the number of stable discs
/**
 * @brief 计算棋盘上指定颜色的棋子稳定性差值
 *
 * 计算指定颜色的棋子在棋盘上的稳定性与对手棋子的稳定性之差。稳定性指棋子被对方翻转的可能性大小，
 * 稳定性高的棋子更难以被对方翻转。
 *
 * @param board 棋盘对象，类型为othelloBoard
 * @param color 棋子颜色，白色为-1，黑色为1
 * @return 返回指定颜色的棋子稳定性与对手棋子稳定性的差值
 */
int othelloHeuristic::stability(othelloBoard &board, int color) {
    // 清除之前的稳定棋子集合
    stableDiscs.clear();

    // 从左上角开始寻找稳定棋子
    // 从左下角开始寻找稳定棋子
    stableDiscsFromCorner(board, 0, color);
    // 从右下角开始寻找稳定棋子
    stableDiscsFromCorner(board, 7, color);
    // 从右上角开始寻找稳定棋子
    stableDiscsFromCorner(board, 56, color);
    // 从棋盘中心位置开始寻找稳定棋子（假设棋盘是8x8）
    stableDiscsFromCorner(board, 63, color);

    // 计算己方稳定棋子的数量
    int myStables = stableDiscs.size();

    // 清除之前的稳定棋子集合，为计算对方稳定棋子数量做准备
    stableDiscs.clear();

    // 对手方颜色取反
    // 从左上角开始寻找对手的稳定棋子
    stableDiscsFromCorner(board, 0, -color);
    // 从左下角开始寻找对手的稳定棋子
    stableDiscsFromCorner(board, 7, -color);
    // 从右下角开始寻找对手的稳定棋子
    stableDiscsFromCorner(board, 56, -color);
    // 从右上角开始寻找对手的稳定棋子
    stableDiscsFromCorner(board, 63, -color);

    // 计算对手稳定棋子的数量
    int opponentStables = stableDiscs.size();

    // 返回己方稳定棋子数量减去对手稳定棋子数量的差值
    return myStables - opponentStables;
}

// Finds the number of stable discs given a corner
/**
 * @brief 从棋盘的一个角落开始，寻找稳定棋子
 *
 * 从指定的角落开始，沿着角落所在的一行和一列遍历棋盘，寻找属于指定颜色的稳定棋子，并将它们添加到稳定棋子集合中。
 *
 * @param board 棋盘对象
 * @param corner 角落的位置索引，取值范围为0到63
 * @param color 棋子颜色，0表示黑色，1表示白色
 */
void othelloHeuristic::stableDiscsFromCorner(othelloBoard &board, int corner,
        int color) {
    // 初始化方向变量
    bool down, right;
    // 根据角点位置设置移动方向
    if (corner == 0) {
        // 左上角
        down = true;
        right = true;
    }
    else if (corner == 7) {
        // 右上角
        down = true;
        right = false;
    }
    else if (corner == 56) {
        // 左下角
        down = false;
        right = true;
    }
    else {
        // 右下角
        down = false;
        right = false;
    }

    // 初始化水平和垂直增量及停止位置
    int horizIncr = 1, horizStop = 7, vertIncr = 8, vertStop = 56;
    // 如果不向右移动，则水平增量和停止位置取反
    if (!right) {
        horizIncr *= -1;
        horizStop *= -1;
    }
    // 如果不向下移动，则垂直增量和停止位置取反
    if (!down) {
        vertIncr *= -1;
        vertStop *= -1;
    }

    // 水平遍历
    // Iterate horizontally
    for (int i = corner; i != corner + horizIncr + horizStop; i += horizIncr) {
        // 如果当前位置为我们的棋子
        // If there is a disc of our color on this square
        if (board.positions[i] == color) {
            // 垂直遍历
            // Iterate vertically
            for (int j = i; j != i + vertStop; j+= vertIncr) {
                // 如果当前位置为我们的棋子且不在稳定棋子集合中
                // If there is a disc of our color on this square,
                // and it is not in the set of stable discs
                if (board.positions[j] == color
                        && stableDiscs.find(j) == stableDiscs.end()) {
                    // 添加到稳定棋子集合中
                    // Insert it to the set
                    stableDiscs.insert(j);
                }
                // 如果遇到对方棋子，则跳出内层循环
                // If we come across a disc of the opposite color, break
                else {
                    break;
                }
            }
        }
        // 如果当前位置不是我们的棋子，则跳出外层循环
        // Otherwise, break
        else {
            break;
        }
    }
}

/**
 * @brief 判断棋盘上剩余空格数的奇偶性
 *
 * 判断棋盘上剩余空格数的奇偶性，如果剩余空格数为偶数，则返回-1；如果剩余空格数为奇数，则返回1。
 *
 * @param board 棋盘对象
 * @return int 返回-1或1，表示剩余空格数的奇偶性
 */
int othelloHeuristic::parity(othelloBoard &board) {
    // 计算棋盘上剩余的空位数量
    int squaresRemaining = 64 - board.discsOnBoard;

    // 如果剩余空位数量为偶数，则返回-1
    if (squaresRemaining % 2 == 0) {
        // 剩余空位为偶数，返回-1
        return -1;
    }
    else {
        // 剩余空位为奇数，返回1
        return 1;
    }
}

// Assigns a weight to every square on the board
/**
 * @brief 根据棋盘状态和玩家颜色计算启发式评估值
 *
 * 根据给定的棋盘状态和玩家颜色，使用预设的权重数组计算启发式评估值。
 * 权重数组根据棋盘位置对评估值的影响进行设定，中心位置权重最高，边缘位置权重最低。
 * 如果棋盘上的某些位置已经被棋子占据，则对应位置的权重将被清零。
 *
 * @param board 棋盘状态
 * @param color 玩家颜色，-1代表白色，1代表黑色
 * @return 启发式评估值
 */
int othelloHeuristic::squareWeights(othelloBoard &board, int &color) {
    // 定义权重数组
    std::vector<int> weights = {
         200, -100, 100,  50,  50, 100, -100,  200,
        -100, -200, -50, -50, -50, -50, -200, -100,
         100,  -50, 100,   0,   0, 100,  -50,  100,
          50,  -50,   0,   0,   0,   0,  -50,   50,
          50,  -50,   0,   0,   0,   0,  -50,   50,
         100,  -50, 100,   0,   0, 100,  -50,  100,
        -100, -200, -50, -50, -50, -50, -200, -100,
         200, -100, 100,  50,  50, 100, -100,  200,
    };

    // 如果棋盘第0个位置非空，则设置相应权重为0
    if (board.positions[0] != 0) {
        // 设置第0行相关权重为0
        weights[1]  = 0;
        weights[2]  = 0;
        weights[3]  = 0;
        // 设置第1行相关权重为0
        weights[8]  = 0;
        weights[9]  = 0;
        weights[10] = 0;
        weights[11] = 0;
        // 设置第3行相关权重为0
        weights[16] = 0;
        weights[17] = 0;
        weights[18] = 0;
        // 设置第4行相关权重为0
        weights[24] = 0;
        weights[25] = 0;
    }

    // 如果棋盘第7个位置非空，则设置相应权重为0
    if (board.positions[7] != 0) {
        // 设置第1列相关权重为0
        weights[4]  = 0;
        weights[5]  = 0;
        weights[6]  = 0;
        // 设置第2列相关权重为0
        weights[12] = 0;
        weights[13] = 0;
        weights[14] = 0;
        weights[15] = 0;
        // 设置第7列相关权重为0
        weights[21] = 0;
        weights[22] = 0;
        weights[23] = 0;
        // 设置第8列相关权重为0
        weights[30] = 0;
        weights[31] = 0;
    }

    // 如果棋盘第56个位置非空，则设置相应权重为0
    if (board.positions[56] != 0) {
        // 设置第7行相关权重为0
        weights[32] = 0;
        weights[33] = 0;
        // 设置第8行相关权重为0
        weights[40] = 0;
        weights[41] = 0;
        weights[42] = 0;
        // 设置第9行相关权重为0
        weights[48] = 0;
        weights[49] = 0;
        weights[50] = 0;
        weights[51] = 0;
        // 设置第1行相关权重为0
        weights[57] = 0;
        weights[58] = 0;
        weights[59] = 0;
    }

    // 如果棋盘第63个位置非空，则设置相应权重为0
    if (board.positions[63] != 0) {
        // 设置第8列相关权重为0
        weights[38] = 0;
        weights[39] = 0;
        // 设置第9列相关权重为0
        weights[45] = 0;
        weights[46] = 0;
        weights[47] = 0;
        // 设置第10列相关权重为0
        weights[52] = 0;
        weights[53] = 0;
        weights[54] = 0;
        weights[55] = 0;
        // 设置第11列相关权重为0
        weights[60] = 0;
        weights[61] = 0;
        weights[62] = 0;
    }

    // 如果当前玩家颜色为1，则计算权重总和
    if (color == 1) {
        // 使用内积计算权重总和
        return std::inner_product(board.positions.begin(),
                board.positions.end(), weights.begin(), 0);
    }
    // 如果当前玩家颜色不为1，则取权重总和的相反数
    else {
        // 使用内积计算权重总和并取反
        return -1*std::inner_product(board.positions.begin(),
                board.positions.end(), weights.begin(), 0);
    }
}

/**
 * @brief 计算棋盘四个角上当前颜色的棋子数相对于对手棋子数的优势分数
 *
 * 计算棋盘四个角（0, 7, 56, 63）上当前颜色的棋子数相对于对手棋子数的优势分数。
 * 优势分数通过公式 (当前颜色棋子数 - 对手棋子数) * 100 / (当前颜色棋子数 + 对手棋子数 + 1) 计算得出。
 *
 * @param board 棋盘对象
 * @param color 当前计算优势分数的颜色（1表示黑色，-1表示白色）
 * @return 返回计算得到的优势分数
 */
int othelloHeuristic::corners(othelloBoard &board, int &color) {
    // 定义角上的位置数组
    std::vector<int> corners = {0, 7, 56, 63};

    // 初始化黑子和白子角上位置计数器
    int blackCorners = 0;
    int whiteCorners = 0;

    // 遍历角上的位置
    for (int corner : corners) {
        // 如果角上位置是黑子
        if (board.positions[corner] == 1) {
            blackCorners++;
        }
        // 如果角上位置是白子
        else if (board.positions[corner] == -1) {
            whiteCorners++;
        }
    }

    // 如果是黑方
    if (color == 1) {
        // 计算并返回黑方优势分数
        return 100 * (blackCorners - whiteCorners)
            / (blackCorners + whiteCorners + 1);
    }
    // 如果是白方
    else {
        // 计算并返回白方优势分数
        return 100 * (whiteCorners - blackCorners)
            / (blackCorners + whiteCorners + 1);
    }
}
