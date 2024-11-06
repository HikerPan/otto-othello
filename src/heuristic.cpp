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
    std::cout << "Evaluating board for color: " << color << "\n";
    std::cout << "Discs on board: " << board.discsOnBoard << "\n";

    if (board.terminalState()) {
        int utilityValue = utility(board, color);
        std::cout << "Terminal state reached. Utility: " << utilityValue << "\n";
        return 100000 * utilityValue;
    }

    if (board.discsOnBoard <= 20) {
        // Opening game
        int mobilityScore = mobility(board, color);
        int potentialMobilityScore = potentialMobility(board, color);
        int squareWeightsScore = squareWeights(board, color);
        int cornersScore = corners(board, color);
        int stabilityScore = stability(board, color);

        std::cout << "Opening game phase\n";
        std::cout << "Mobility: " << mobilityScore << "\n";
        std::cout << "Potential Mobility: " << potentialMobilityScore << "\n";
        std::cout << "Square Weights: " << squareWeightsScore << "\n";
        std::cout << "Corners: " << cornersScore << "\n";
        std::cout << "Stability: " << stabilityScore << "\n";

        return 5 * mobilityScore
            + 5 * potentialMobilityScore
            + 20 * squareWeightsScore
            + 10000 * cornersScore
            + 10000 * stabilityScore;
    }
    else if (board.discsOnBoard <= 58) {
        // Midgame
        int discDifferenceScore = discDifference(board, color);
        int mobilityScore = mobility(board, color);
        int potentialMobilityScore = potentialMobility(board, color);
        int squareWeightsScore = squareWeights(board, color);
        int parityScore = parity(board);
        int cornersScore = corners(board, color);
        int stabilityScore = stability(board, color);

        std::cout << "Midgame phase\n";
        std::cout << "Disc Difference: " << discDifferenceScore << "\n";
        std::cout << "Mobility: " << mobilityScore << "\n";
        std::cout << "Potential Mobility: " << potentialMobilityScore << "\n";
        std::cout << "Square Weights: " << squareWeightsScore << "\n";
        std::cout << "Parity: " << parityScore << "\n";
        std::cout << "Corners: " << cornersScore << "\n";
        std::cout << "Stability: " << stabilityScore << "\n";

        return 10 * discDifferenceScore
            + 2 * mobilityScore
            + 2 * potentialMobilityScore
            + 10 * squareWeightsScore
            + 100 * parityScore
            + 10000 * cornersScore
            + 10000 * stabilityScore;
    }
    else {
        // Endgame
        int discDifferenceScore = discDifference(board, color);
        int parityScore = parity(board);
        int cornersScore = corners(board, color);
        int stabilityScore = stability(board, color);

        std::cout << "Endgame phase\n";
        std::cout << "Disc Difference: " << discDifferenceScore << "\n";
        std::cout << "Parity: " << parityScore << "\n";
        std::cout << "Corners: " << cornersScore << "\n";
        std::cout << "Stability: " << stabilityScore << "\n";

        return 500 * discDifferenceScore
            + 500 * parityScore
            + 10000 * cornersScore
            + 10000 * stabilityScore;
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
    printf("\n[othelloHeuristic::mobility]\n");
    // 寻找黑棋的合法走法
    board.findLegalMoves(1, &pMoves);
    // 统计黑棋的合法走法数量
    int blackMoves = pMoves.size();
    // 输出调试信息：黑棋的合法走法数量
    printf("Black LegalMoves: %d\n", blackMoves);
    // 清空合法走法列表
    pMoves.clear();

    // 寻找白棋的合法走法
    board.findLegalMoves(-1, &pMoves);
    // 统计白棋的合法走法数量
    int whiteMoves = pMoves.size();
    // 输出调试信息：白棋的合法走法数量
    printf("White LegalMoves: %d\n", whiteMoves);
    // 清空合法走法列表
    pMoves.clear();

    // 输出调试信息：当前玩家颜色
    printf("Current player color: %d\n", color);

    // 判断当前玩家颜色
    if (color == 1) {
        // 输出调试信息：黑棋的移动优势百分比
        int mobilityScore = 100 * (blackMoves - whiteMoves) / (blackMoves + whiteMoves + 1);
        printf("Mobility score for Black: %d\n", mobilityScore);
        return mobilityScore;
    }
    else {
        // 输出调试信息：白棋的移动优势百分比
        int mobilityScore = 100 * (whiteMoves - blackMoves) / (blackMoves + whiteMoves + 1);
        printf("Mobility score for White: %d\n", mobilityScore);
        return mobilityScore;
    }
}


int othelloHeuristic::potentialMobility(othelloBoard &board, int color) {
    printf("\n[othelloHeuristic::potentialMobility]\n");
    // 计算当前玩家的潜在移动能力
    int myPotentialMobility = playerPotentialMobility(board, color);
    // 输出调试信息：当前玩家的潜在移动能力
    printf("Potential mobility for color %d: %d\n", color, myPotentialMobility);

    // 计算对手的潜在移动能力
    int opponentPotentialMobility = playerPotentialMobility(board, -color);
    // 输出调试信息：对手的潜在移动能力
    printf("Potential mobility for opponent color %d: %d\n", -color, opponentPotentialMobility);

    // 计算潜在移动能力得分
    int potentialMobilityScore = 100 * (myPotentialMobility - opponentPotentialMobility)
        / (myPotentialMobility + opponentPotentialMobility + 1);
    // 输出调试信息：潜在移动能力得分
    printf("Potential mobility score: %d\n", potentialMobilityScore);

    return potentialMobilityScore;
}


/**
 * @brief 计算玩家在棋盘上潜在的移动性
 *
 * 根据棋盘的当前状态，计算指定颜色玩家在棋盘上潜在的移动性。
 * 移动性考虑的是甜蜜十六区的十六个字和这个区域上下左右的各4个子
 *
 * @param board 棋盘对象
 * @param color 玩家颜色，-1 表示白棋，1 表示黑棋
 *
 * @return 潜在的移动性数值
 */
int othelloHeuristic::playerPotentialMobility(othelloBoard &board, int color) {
    UT_array *boardInterior;

    int here = 0, up = 0, down = 0, left = 0, right = 0,
        upperLeft = 0, upperRight = 0, lowerLeft = 0, lowerRight = 0;
    int potentialMobility = 0;
    int *value = NULL;


    // 初始化存储整数的 utarray
    utarray_new(boardInterior, &ut_int_icd);

    // 添加元素到 boardInterior 数组(黑白棋中的甜蜜十六区，即中间的十六个格子)
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
        // 输出调试信息：当前 square 位置和 here 的值
        printf("Interior Square: %d, here: %d, up %d, down %d, left %d, right %d,upperLeft %d, upperRight %d, lowerLeft %d, lowerRight %d\n", 
                            square,here,up,down,left,right,upperLeft,upperRight,lowerLeft,lowerRight);

        if (here == -color && up == 0) {
            potentialMobility++;
            printf("Increase Potential Mobility (Up): %d\n", potentialMobility);
        }
        if (here == -color && down == 0) {
            potentialMobility++;
            printf("Increase Potential Mobility (Down): %d\n", potentialMobility);
        }
        if (here == -color && left == 0) {
            potentialMobility++;
            printf("Increase Potential Mobility (Left): %d\n", potentialMobility);
        }
        if (here == -color && right == 0) {
            potentialMobility++;
            printf("Increase Potential Mobility (Right): %d\n", potentialMobility);
        }
        if (here == -color && upperLeft == 0) {
            potentialMobility++;
            printf("Increase Potential Mobility (UpperLeft): %d\n", potentialMobility);
        }
        if (here == -color && upperRight == 0) {
            potentialMobility++;
            printf("Increase Potential Mobility (UpperRight): %d\n", potentialMobility);
        }
        if (here == -color && lowerLeft == 0) {
            potentialMobility++;
            printf("Increase Potential Mobility (LowerLeft): %d\n", potentialMobility);
        }
        if (here == -color && lowerRight == 0) {
            potentialMobility++;
            printf("Increase Potential Mobility (LowerRight): %d\n", potentialMobility);
        }
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
        
        // printf("Interior Square: %d, Here: %d, up %d, down %d, left %d, right %d,upperLeft %d, upperRight %d, lowerLeft %d, lowerRight %d\n", 
        //                 square,here,up,down,left,right,upperLeft,upperRight,lowerLeft,lowerRight);

        // right = board.positions[square+1];
        if (here == -color && left == 0)
        {
            potentialMobility++;
            printf("Increase Potential Mobility (topRow left): %d\n", potentialMobility);
        }
        if (here == -color && right == 0)
        {
            potentialMobility++;
            printf("Increase Potential Mobility (topRow left): %d\n", potentialMobility);
        }
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
        {
            potentialMobility++;
            printf("Increase Potential Mobility (bottomRow left): %d\n", potentialMobility);
        }
        if (here == -color && right == 0)
        {
            potentialMobility++;
            printf("Increase Potential Mobility (bottomRow right): %d\n", potentialMobility);
        }
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
        {
            potentialMobility++;
            printf("Increase Potential Mobility (leftColumn up): %d\n", potentialMobility);
        }
        if (here == -color && down == 0)
        {
            potentialMobility++;
            printf("Increase Potential Mobility (leftColumn down): %d\n", potentialMobility);
        }
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
        {
            potentialMobility++;
            printf("Increase Potential Mobility (rightColumn up): %d\n", potentialMobility);
        }
        if (here == -color && down == 0)
        {
            potentialMobility++;
            printf("Increase Potential Mobility (rightColumn up): %d\n", potentialMobility);
        }
    }
    utarray_free(rightColumn);  // 释放 rightColumn 的内存


    return potentialMobility;
}

// Computes a lower bound on the number of stable discs
/**
 * @brief 计算当前棋盘上指定颜色的棋子稳定性得分
 * 稳定性主要考虑角落棋子，四个角上的棋子不能被翻转，最稳定
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

    printf("\n [stability] color %d, myStables %d, opponentStables %d\n",color,myStables,opponentStables);

    // 返回己方稳定棋子数量减去对方稳定棋子数量
    return myStables - opponentStables;
}

// Finds the number of stable discs given a corner
/**
 * @brief 从棋盘角落计算稳定棋子数量
 *
 * 从指定的角落位置开始，计算该角落周围属于指定颜色的稳定棋子数量，并将其添加到稳定棋子集合中。
 *
 * @param board 棋盘对象
 * @param corner 角落位置索引
 * @param color 棋子颜色
 */
void othelloHeuristic::stableDiscsFromCorner(othelloBoard &board, int corner,
        int color) {
    bool down, right;

    // 位置0(西北角)，从上向下，从左向右扫描
    if (corner == 0) {
        down = true;
        right = true;
    }
    // 位置7(东北角)，从上向下，从右向左扫描
    else if (corner == 7) {
        down = true;
        right = false;
    }
    // 位置56(西南角)，从下向上，从左向右扫描
    else if (corner == 56) {
        down = false;
        right = true;
    }
    // 位置63(东南角)，从下向上，从右向左扫描
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

/**
 * @brief 判断棋盘上剩余空位数的奇偶性
 *
 * 根据棋盘上剩余空位数（64 - 当前棋盘上棋子数）的奇偶性，返回相应的值。
 * 如果剩余空位数为偶数，则返回 -1；如果为奇数，则返回 1。
 *
 * @param board othelloBoard 类型，表示当前棋盘状态
 * @return int 类型，返回 -1 或 1，表示剩余空位数的奇偶性
 */
int othelloHeuristic::parity(othelloBoard &board) {
    int squaresRemaining = 64 - board.discsOnBoard;

    if (squaresRemaining % 2 == 0) {
        return -1;
    }
    else {
        return 1;
    }
}


/**
 * @brief 计算棋盘的得分（启发式评估）
 *
 * 根据棋盘的状态和当前玩家的颜色，计算棋盘的得分。
 *
 * @param board 引用传递的 othelloBoard 对象，表示当前的棋盘状态
 * @param color 引用传递的整数，表示当前玩家的颜色（1 表示白色，0 表示黑色）
 *
 * @return 返回棋盘的得分，整数类型
 *
 * 此函数首先初始化一个 utarray 动态数组用于存储权重值，然后将预设的权重值添加到该数组中。
 * 接着，根据棋盘的四个角落是否被占据，更新权重值。
 * 然后，通过计算棋盘状态与权重值之间的内积，得到棋盘的得分。
 * 如果当前玩家是黑色棋子，则直接返回得分；如果是白色棋子，则返回得分的相反数。
 * 最后，释放 utarray 动态数组并返回最终的得分。
 */
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
