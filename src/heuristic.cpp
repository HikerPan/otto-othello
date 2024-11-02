#include "heuristic.hpp"
#include "utarray.h"
// Heuristic is from the perspective that the calling player is the maximizing
// player, regardless of the player's color

/**
 * @brief 评估当前棋盘的得分
 *
 * 根据当前棋盘状态和颜色，评估当前棋盘的得分。
 *
 * @param board 棋盘对象
 * @param color 当前玩家颜色
 *
 * @return 返回一个整数，表示当前棋盘的得分
 */
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
    // int util = std::accumulate(board.positions.begin(),
    //         board.positions.end(), 0);
    int util = 0;
    for (int i = 0; i < utarray_len(board.positions); ++i) {
        int *value = (int *)utarray_eltptr(board.positions, i);
        if (value) {
            util += *value;
        }
    }

    if (color == 1) {
        return util; 
    }
    else {
        return -1*util;
    }
}

// Relative disc difference between the two players
int othelloHeuristic::discDifference(othelloBoard &board, int &color) {
    // int blackCount = std::count(board.positions.begin(),
    //         board.positions.end(), 1);
    int blackCount = count_in_utarray(board.positions,1);
    // int whiteCount = std::count(board.positions.begin(),
    //         board.positions.end(), -1);
    int whiteCount = count_in_utarray(board.positions,-1);

    if (color == 1) {
        return 100 * (blackCount - whiteCount) / (blackCount + whiteCount);
    }
    else {
        return 100 * (whiteCount - blackCount) / (blackCount + whiteCount);
    }
}

// Number of possible moves
/**
 * @brief 计算棋子的移动性启发式值
 *
 * 根据当前棋盘状态计算指定颜色棋子的移动性启发式值。
 * 移动性启发式值基于黑白棋子的合法移动数量差，差值越大，启发式值越高。
 *
 * @param board 棋盘对象
 * @param color 当前计算移动性启发式值的棋子颜色，1表示黑子，-1表示白子
 * @return 返回移动性启发式值
 */
int othelloHeuristic::mobility(othelloBoard &board, int &color) {
    // 寻找白棋的合法走法
    board.findLegalMoves(1, &pMoves);
    // 统计白棋的合法走法数量
    int blackMoves = pMoves.size();
    // 清空合法走法列表
    pMoves.clear();

    // 寻找黑棋的合法走法
    board.findLegalMoves(-1, &pMoves);
    // 统计黑棋的合法走法数量
    int whiteMoves = pMoves.size();
    // 清空合法走法列表
    pMoves.clear();

    // 判断当前玩家颜色
    if (color == 1) {
        // 返回黑棋的移动优势百分比
        return 100 * (blackMoves - whiteMoves) / (blackMoves + whiteMoves + 1);
    }
    else {
        // 返回白棋的移动优势百分比
        return 100 * (whiteMoves - blackMoves) / (blackMoves + whiteMoves + 1);
    }
}

int othelloHeuristic::potentialMobility(othelloBoard &board, int color) {
    int myPotentialMobility = playerPotentialMobility(board, color);
    int opponentPotentialMobility = playerPotentialMobility(board, -color);

    return 100 * (myPotentialMobility - opponentPotentialMobility)
        / (myPotentialMobility + opponentPotentialMobility + 1);
}

/**
 * @brief 计算玩家在棋盘上潜在的移动性
 *
 * 根据棋盘的当前状态，计算指定颜色玩家在棋盘上潜在的移动性。
 *
 * @param board 棋盘对象
 * @param color 玩家颜色，-1 表示白棋，1 表示黑棋
 *
 * @return 潜在的移动性数值
 */
int othelloHeuristic::playerPotentialMobility(othelloBoard &board, int color) {
    // std::vector<int> boardInterior = {18, 19, 20, 21,
    //                                   26, 27, 28, 29,
    //                                   34, 35, 36, 37,
    //                                   42, 43, 44, 45};
    UT_array *boardInterior;


    int here = 0, up = 0, down = 0, left = 0, right = 0,
        upperLeft = 0, upperRight = 0, lowerLeft = 0, lowerRight = 0;
    int potentialMobility = 0;
    int *value = NULL;


    // 初始化存储整数的 utarray
    utarray_new(boardInterior, &ut_int_icd);

    // 添加元素到 boardInterior 数组
    int values[] = {18, 19, 20, 21, 
                    26, 27, 28, 29, 
                    34, 35, 36, 37, 
                    42, 43, 44, 45};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        utarray_push_back(boardInterior, &values[i]);
    }

    // 使用索引访问数组元素，而非范围for循环
    for (int i = 0; i < utarray_len(boardInterior); i++) {
        int square = *((int*)utarray_eltptr(boardInterior, i));
        value = (int *)utarray_eltptr(board.positions, square);
        if (value) {
            here = *value;
        }
        // here = board.positions[square];
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square-8);
        if (value) {
            up = *value;
        }
        // up = board.positions[square-8];
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square+8);
        if (value) {
            down = *value;
        }
        // down = board.positions[square+8];
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square-1);
        if (value) {
            left = *value;
        }
        // left = board.positions[square-1];
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square+1);
        if (value) {
            right = *value;
        }
        // right = board.positions[square+1];
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square-9);
        if (value) {
            upperLeft = *value;
        }
        // upperLeft = board.positions[square-9];
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square-7);
        if (value) {
            upperRight = *value;
        }
        // upperRight = board.positions[square-7];
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square+7);
        if (value) {
            lowerLeft = *value;
        }
        // lowerLeft = board.positions[square+7];
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square+9);
        if (value) {
            lowerRight = *value;
        }
        // lowerRight = board.positions[square+9];

        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
        if (here == -color && upperLeft == 0)
            potentialMobility++;
        if (here == -color && upperRight == 0)
            potentialMobility++;
        if (here == -color && lowerLeft == 0)
            potentialMobility++;
        if (here == -color && lowerRight == 0)
            potentialMobility++;
    }
    utarray_free(boardInterior);  // 释放 boardInterior 的内存

    // 用 utarray 替换 std::vector<int> topRow
    UT_array *topRow;
    utarray_new(topRow, &ut_int_icd);
    int topRowValues[] = {10, 11, 12, 13};
    for (int i = 0; i < sizeof(topRowValues) / sizeof(topRowValues[0]); i++) {
        utarray_push_back(topRow, &topRowValues[i]);
    }
    // 遍历 topRow
    for (int i = 0; i < utarray_len(topRow); i++) {
        int square = *((int*)utarray_eltptr(topRow, i));

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square);
        if(value)
            here = *value;
        
        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square-1);
        if(value)
            left = *value;
        // left = board.positions[square-1];

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square+1);
        if(value)
            right = *value;
        // right = board.positions[square+1];
        if (here == -color && left == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
    }
    utarray_free(topRow);  // 释放 topRow 的内存

    // 用 utarray 替换 std::vector<int> bottomRow
    UT_array *bottomRow;
    utarray_new(bottomRow, &ut_int_icd);
    int bottomRowValues[] = {50, 51, 52, 53};
    for (int i = 0; i < sizeof(bottomRowValues) / sizeof(bottomRowValues[0]); i++) {
        utarray_push_back(bottomRow, &bottomRowValues[i]);
    }

    // 遍历 bottomRow
    for (int i = 0; i < utarray_len(bottomRow); i++) {
        int square = *((int*)utarray_eltptr(bottomRow, i));

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square);
        if(value)
            here = *value;
        // here = board.positions[square];

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square-1);
        if(value)
            left = *value;
        // left = board.positions[square-1];

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square+1);
        if(value)
            right = *value;
        // right = board.positions[square+1];
        if (here == -color && left == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
    }
    utarray_free(bottomRow);  // 释放 bottomRow 的内存

    // 用 utarray 替换 std::vector<int> leftColumn
    UT_array *leftColumn;
    utarray_new(leftColumn, &ut_int_icd);
    int leftColumnValues[] = {17, 25, 33, 41};
    for (int i = 0; i < sizeof(leftColumnValues) / sizeof(leftColumnValues[0]); i++) {
        utarray_push_back(leftColumn, &leftColumnValues[i]);
    }

    // 遍历 leftColumn
    for (int i = 0; i < utarray_len(leftColumn); i++) {
        int square = *((int*)utarray_eltptr(leftColumn, i));

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square);
        if(value)
            here = *value;
        // here = board.positions[square];

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square-8);
        if(value)
            up = *value;
        // up = board.positions[square-8];

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square+8);
        if(value)
            down = *value;
        // down = board.positions[square+8];
        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
    }
    utarray_free(leftColumn);  // 释放 leftColumn 的内存

    // 用 utarray 替换 std::vector<int> rightColumn
    UT_array *rightColumn;
    utarray_new(rightColumn, &ut_int_icd);
    int rightColumnValues[] = {22, 30, 38, 46};
    for (int i = 0; i < sizeof(rightColumnValues) / sizeof(rightColumnValues[0]); i++) {
        utarray_push_back(rightColumn, &rightColumnValues[i]);
    }

    // 遍历 rightColumn
    for (int i = 0; i < utarray_len(rightColumn); i++) {
        int square = *((int*)utarray_eltptr(rightColumn, i));

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square);
        if(value)
            here = *value;
        // here = board.positions[square];

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square-8);
        if(value)
            up = *value;
        // up = board.positions[square-8];

        value = NULL;
        value = (int *)utarray_eltptr(board.positions, square+8);
        if(value)
            down = *value;
        // down = board.positions[square+8];
        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
    }
    utarray_free(rightColumn);  // 释放 rightColumn 的内存


    return potentialMobility;
}

// Computes a lower bound on the number of stable discs
/**
 * @brief 计算当前棋盘上指定颜色的棋子稳定性得分
 *
 * 根据棋盘上指定颜色的棋子稳定性计算得分。稳定性得分是通过比较当前玩家和对手的稳定棋子数量差异来计算的。
 *
 * @param board 棋盘对象
 * @param color 指定颜色的棋子（1 表示黑棋，-1 表示白棋）
 * @return 当前玩家与对手稳定棋子数量的差值
 */
int othelloHeuristic::stability(othelloBoard &board, int color) {
    // 清空稳定棋子集合
    stableDiscs.clear();

    // 从四个角落寻找稳定棋子
    // 左上角
    stableDiscsFromCorner(board, 0, color);
    // 右上角
    stableDiscsFromCorner(board, 7, color);
    // 左下角
    stableDiscsFromCorner(board, 56, color);
    // 右下角
    stableDiscsFromCorner(board, 63, color);

    // 计算己方稳定棋子数量
    int myStables = stableDiscs.size();

    // 再次清空稳定棋子集合
    stableDiscs.clear();

    // 从四个角落寻找对方稳定棋子
    // 左上角
    stableDiscsFromCorner(board, 0, -color);
    // 右上角
    stableDiscsFromCorner(board, 7, -color);
    // 左下角
    stableDiscsFromCorner(board, 56, -color);
    // 右下角
    stableDiscsFromCorner(board, 63, -color);

    // 计算对方稳定棋子数量
    int opponentStables = stableDiscs.size();

    // 返回己方稳定棋子数量减去对方稳定棋子数量
    return myStables - opponentStables;
}

// Finds the number of stable discs given a corner
void othelloHeuristic::stableDiscsFromCorner(othelloBoard &board, int corner,
        int color) {
    bool down, right;
    if (corner == 0) {
        down = true;
        right = true;
    }
    else if (corner == 7) {
        down = true;
        right = false;
    }
    else if (corner == 56) {
        down = false;
        right = true;
    }
    else {
        down = false;
        right = false;
    }

    int horizIncr = 1, horizStop = 7, vertIncr = 8, vertStop = 56;
    if (!right) {
        horizIncr *= -1;
        horizStop *= -1;
    }
    if (!down) {
        vertIncr *= -1;
        vertStop *= -1;
    }

    // Iterate horizontally
    for (int i = corner; i != corner + horizIncr + horizStop; i += horizIncr) {
        // If there is a disc of our color on this square
        int *value = (int *)utarray_eltptr(board.positions, i);
        if (value && *value == color) {
        // if (board.positions[i] == color) {
            // Iterate vertically
            for (int j = i; j != i + vertStop; j+= vertIncr) {
                // If there is a disc of our color on this square,
                // and it is not in the set of stable discs
                int *value_v = (int *)utarray_eltptr(board.positions, j);
                if ((value_v && *value_v == color)&&(stableDiscs.find(j) == stableDiscs.end())) {
                // if (board.positions[j] == color
                //         && stableDiscs.find(j) == stableDiscs.end()) {
                    // Insert it to the set
                    stableDiscs.insert(j);
                }
                // If we come across a disc of the opposite color, break
                else {
                    break;
                }
            }
        }
        // Otherwise, break
        else {
            break;
        }
    }
}

int othelloHeuristic::parity(othelloBoard &board) {
    int squaresRemaining = 64 - board.discsOnBoard;

    if (squaresRemaining % 2 == 0) {
        return -1;
    }
    else {
        return 1;
    }
}


int othelloHeuristic::squareWeights(othelloBoard &board, int &color) {
    // 初始化 utarray 动态数组 weights，替代 std::vector<int>
    UT_array *weights;
    utarray_new(weights, &ut_int_icd);

    // 将权重值添加到 weights 数组
    int values[] = {
         200, -100, 100,  50,  50, 100, -100,  200,
        -100, -200, -50, -50, -50, -50, -200, -100,
         100,  -50, 100,   0,   0, 100,  -50,  100,
          50,  -50,   0,   0,   0,   0,  -50,   50,
          50,  -50,   0,   0,   0,   0,  -50,   50,
         100,  -50, 100,   0,   0, 100,  -50,  100,
        -100, -200, -50, -50, -50, -50, -200, -100,
         200, -100, 100,  50,  50, 100, -100,  200,
    };
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        utarray_push_back(weights, &values[i]);
    }

    // 更新权重值，如果角落被占据
    int *value0 = (int *)utarray_eltptr(board.positions, 0);
    if (value0 && *value0 != 0) {
    // if (board.positions[0] != 0) {
        int indices[] = {1, 2, 3, 8, 9, 10, 11, 16, 17, 18, 24, 25};
        for (int i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {
            *(int *)utarray_eltptr(weights, indices[i]) = 0;
        }
    }
    int *value7 = (int *)utarray_eltptr(board.positions, 7);
    if (value7 && *value7 != 0) {
    // if (board.positions[7] != 0) {
        int indices[] = {4, 5, 6, 12, 13, 14, 15, 21, 22, 23, 30, 31};
        for (int i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {
            *(int *)utarray_eltptr(weights, indices[i]) = 0;
        }
    }
    int *value56 = (int *)utarray_eltptr(board.positions, 56);
    if (value56 && *value56 != 0) {
    // if (board.positions[56] != 0) {
        int indices[] = {32, 33, 40, 41, 42, 48, 49, 50, 51, 57, 58, 59};
        for (int i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {
            *(int *)utarray_eltptr(weights, indices[i]) = 0;
        }
    }
    int *value63 = (int *)utarray_eltptr(board.positions, 63);
    if (value63 && *value63 != 0) {
    // if (board.positions[63] != 0) {
        int indices[] = {38, 39, 45, 46, 47, 52, 53, 54, 55, 60, 61, 62};
        for (int i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {
            *(int *)utarray_eltptr(weights, indices[i]) = 0;
        }
    }

    // 计算内积（用循环替代 std::inner_product）
    int score = 0;
    for (int i = 0; i < 64; i++) {
        int *boardValue = (int *)utarray_eltptr(board.positions, i);
        // int boardValue = board.positions[i];
        int weightValue = *(int *)utarray_eltptr(weights, i);
        score += *boardValue * weightValue;
    }

    // 如果是白色棋子，得分取负
    if (color != 1) {
        score = -score;
    }

    // 释放 weights 数组
    utarray_free(weights);

    return score;
}
int othelloHeuristic::corners(othelloBoard &board, int &color) {
    // 使用 utarray 替换 std::vector<int> corners
    UT_array *corners;
    utarray_new(corners, &ut_int_icd);

    // 初始化 corners 数组，包含四个角的位置
    int cornerValues[] = {0, 7, 56, 63};
    for (int i = 0; i < sizeof(cornerValues) / sizeof(cornerValues[0]); i++) {
        utarray_push_back(corners, &cornerValues[i]);
    }

    int blackCorners = 0;
    int whiteCorners = 0;

    // 遍历 corners 数组
    for (int i = 0; i < utarray_len(corners); i++) {
        int corner = *(int *)utarray_eltptr(corners, i);

        // 获取 positions 中对应角落的值
        int *pos_value = (int *)utarray_eltptr(board.positions, corner);
        if (pos_value && *pos_value == 1) {
        // if (board.positions[corner] == 1) {
            blackCorners++;
        }
        else if (pos_value && *pos_value == -1) {
        // else if (board.positions[corner] == -1) {
            whiteCorners++;
        }
    }

    // 根据颜色计算角落分数
    int result;
    if (color == 1) {
        result = 100 * (blackCorners - whiteCorners) / (blackCorners + whiteCorners + 1);
    }
    else {
        result = 100 * (whiteCorners - blackCorners) / (blackCorners + whiteCorners + 1);
    }

    utarray_free(corners);  // 释放 corners 的内存

    return result;
}
