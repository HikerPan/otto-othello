#include "heuristic.hpp"
// Heuristic is from the perspective that the calling player is the maximizing
// player, regardless of the player's color

/**
 * @brief 评估井字棋局面的启发式函数
 *
 * 根据井字棋的当前局面和玩家颜色，评估局面并返回一个分数。
 *
 * @param board 井字棋局面对象
 * @param color 当前玩家的颜色
 * @return 返回一个评估分数
 */
int othelloHeuristic_evaluate(OTHELLO_BOARD &board, int color) {
    if (othelloBoard_terminalState()) {
        return 100000*othelloHeuristic_utility(board, color);
    }

    if (board.discsOnBoard <= 20) {
        // Opening game
        return 5*othelloHeuristic_mobility(board, color)
            + 5*othelloHeuristic_potentialMobility(board, color)
            + 20*othelloHeuristic_squareWeights(board, color)
            + 10000*othelloHeuristic_corners(board, color)
            + 10000*othelloHeuristic_stability(board, color);
    }
    else if (board.discsOnBoard <= 58) {
        // Midgame
        return 10*othelloHeuristic_discDifference(board, color)
            + 2*othelloHeuristic_mobility(board, color)
            + 2*othelloHeuristic_potentialMobility(board, color)
            + 10*othelloHeuristic_squareWeights(board, color)
            + 100*othelloHeuristic_parity(board)
            + 10000*othelloHeuristic_corners(board, color)
            + 10000*othelloHeuristic_stability(board, color);
    }
    else {
        // Endgame
        return 500*othelloHeuristic_discDifference(board, color)
            + 500*othelloHeuristic_parity(board)
            + 10000*othelloHeuristic_corners(board, color)
            + 10000*othelloHeuristic_stability(board, color);
    }
}

/**
 * @brief 计算井字棋的启发式效用值
 *
 * 根据当前棋盘状态计算并返回指定颜色的效用值。
 *
 * @param board 井字棋棋盘对象
 * @param color 颜色，1表示白棋，-1表示黑棋
 * @return 返回指定颜色的效用值，白棋的效用值为正数，黑棋的效用值为负数
 */
int othelloHeuristic_utility(OTHELLO_BOARD &board, int &color) {
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
/**
 * @brief 计算黑白棋子的差值启发式函数
 *
 * 根据当前棋盘上的黑子和白子数量计算差值启发式值。
 *
 * @param board 棋盘对象
 * @param color 当前玩家颜色（1代表黑方，-1代表白方）
 * @return 返回一个浮点数，表示当前玩家相对于对手的棋子数量差值百分比
 */
int othelloHeuristic_discDifference(OTHELLO_BOARD &board, int &color) {
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
int othelloHeuristic_mobility(OTHELLO_BOARD &board, int &color) {
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

int othelloHeuristic_potentialMobility(OTHELLO_BOARD &board, int color) {
    int myPotentialMobility = othelloHeuristic_playerPotentialMobility(board, color);
    int opponentPotentialMobility = othelloHeuristic_playerPotentialMobility(board, -color);

    return 100 * (myPotentialMobility - opponentPotentialMobility)
        / (myPotentialMobility + opponentPotentialMobility + 1);
}

int othelloHeuristic_playerPotentialMobility(OTHELLO_BOARD &board, int color) {
    std::vector<int> boardInterior = {18, 19, 20, 21,
                                      26, 27, 28, 29,
                                      34, 35, 36, 37,
                                      42, 43, 44, 45};

    int here = 0, up = 0, down = 0, left = 0, right = 0,
        upperLeft = 0, upperRight = 0, lowerLeft = 0, lowerRight = 0;
    int potentialMobility = 0;

    for (int square : boardInterior) {
        here = board.positions[square];
        up = board.positions[square-8];
        down = board.positions[square+8];
        left = board.positions[square-1];
        right = board.positions[square+1];
        upperLeft = board.positions[square-9];
        upperRight = board.positions[square-7];
        lowerLeft = board.positions[square+7];
        lowerRight = board.positions[square+9];

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

    std::vector<int> topRow = {10, 11, 12, 13};
    for (int square : topRow) {
        here = board.positions[square];
        left = board.positions[square-1];
        right = board.positions[square+1];
        if (here == -color && left == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
    }

    std::vector<int> bottomRow = {50, 51, 52, 53};
    for (int square : bottomRow) {
        here = board.positions[square];
        left = board.positions[square-1];
        right = board.positions[square+1];
        if (here == -color && left == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
    }

    std::vector<int> leftColumn = {17, 25, 33, 41};
    for (int square : leftColumn) {
        here = board.positions[square];
        up = board.positions[square-8];
        down = board.positions[square+8];
        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
    }

    std::vector<int> rightColumn = {22, 30, 38, 46};
    for (int square : leftColumn) {
        here = board.positions[square];
        up = board.positions[square-8];
        down = board.positions[square+8];
        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
    }

    return potentialMobility;
}

// Computes a lower bound on the number of stable discs
int othelloHeuristic_stability(othelloBoard &board, int color) {
    stableDiscs.clear();

    stableDiscsFromCorner(board, 0, color);
    stableDiscsFromCorner(board, 7, color);
    stableDiscsFromCorner(board, 56, color);
    stableDiscsFromCorner(board, 63, color);

    int myStables = stableDiscs.size();

    stableDiscsFromCorner(board, 0, -color);
    stableDiscsFromCorner(board, 7, -color);
    stableDiscsFromCorner(board, 56, -color);
    stableDiscsFromCorner(board, 63, -color);

    int opponentStables = stableDiscs.size();

    return myStables - opponentStables;
}

// Finds the number of stable discs given a corner
void othelloHeuristic_stableDiscsFromCorner(othelloBoard &board, int corner,
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
        if (board.positions[i] == color) {
            // Iterate vertically
            for (int j = i; j != i + vertStop; j+= vertIncr) {
                // If there is a disc of our color on this square,
                // and it is not in the set of stable discs
                if (board.positions[j] == color
                        && stableDiscs.find(j) == stableDiscs.end()) {
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

int othelloHeuristic_parity(othelloBoard &board) {
    int squaresRemaining = 64 - board.discsOnBoard;

    if (squaresRemaining % 2 == 0) {
        return -1;
    }
    else {
        return 1;
    }
}

static int std_inner_product(OTHELLO_BOARD &board, int &weights)
{
    int result = 0;
    for(size_t i = 0;i<OTHELLO_BOARD_SIZE;i++)
    {
        result += board.positions[i]*weights[i];
    }
}

// Assigns a weight to every square on the board
int othelloHeuristic_squareWeights(OTHELLO_BOARD &board, int &color) {
    int weights[OTHELLO_BOARD_SIZE] = {
         200, -100, 100,  50,  50, 100, -100,  200,
        -100, -200, -50, -50, -50, -50, -200, -100,
         100,  -50, 100,   0,   0, 100,  -50,  100,
          50,  -50,   0,   0,   0,   0,  -50,   50,
          50,  -50,   0,   0,   0,   0,  -50,   50,
         100,  -50, 100,   0,   0, 100,  -50,  100,
        -100, -200, -50, -50, -50, -50, -200, -100,
         200, -100, 100,  50,  50, 100, -100,  200,
    };

    if (board.positions[0] != 0) {
        weights[1]  = 0;
        weights[2]  = 0;
        weights[3]  = 0;
        weights[8]  = 0;
        weights[9]  = 0;
        weights[10] = 0;
        weights[11] = 0;
        weights[16] = 0;
        weights[17] = 0;
        weights[18] = 0;
        weights[24] = 0;
        weights[25] = 0;
    }

    if (board.positions[7] != 0) {
        weights[4]  = 0;
        weights[5]  = 0;
        weights[6]  = 0;
        weights[12] = 0;
        weights[13] = 0;
        weights[14] = 0;
        weights[15] = 0;
        weights[21] = 0;
        weights[22] = 0;
        weights[23] = 0;
        weights[30] = 0;
        weights[31] = 0;
    }

    if (board.positions[56] != 0) {
        weights[32] = 0;
        weights[33] = 0;
        weights[40] = 0;
        weights[41] = 0;
        weights[42] = 0;
        weights[48] = 0;
        weights[49] = 0;
        weights[50] = 0;
        weights[51] = 0;
        weights[57] = 0;
        weights[58] = 0;
        weights[59] = 0;
    }

    if (board.positions[63] != 0) {
        weights[38] = 0;
        weights[39] = 0;
        weights[45] = 0;
        weights[46] = 0;
        weights[47] = 0;
        weights[52] = 0;
        weights[53] = 0;
        weights[54] = 0;
        weights[55] = 0;
        weights[60] = 0;
        weights[61] = 0;
        weights[62] = 0;
    }

    if (color == 1) {
        // return std::inner_product(board.positions.begin(),
        //         board.positions.end(), weights.begin(), 0);
        return  std_inner_product(board,weights);
    }
    else {
        // return -1*std::inner_product(board.positions.begin(),
        //         board.positions.end(), weights.begin(), 0);
        return  -1*std_inner_product(board,weights);
    }
}

/**
 * @brief 计算棋盘上四个角落对井字棋玩家的影响
 *
 * 根据当前棋盘状态和玩家颜色，计算四个角落对井字棋玩家的影响。
 *
 * @param board 棋盘对象，引用传递
 * @param color 玩家颜色，1 表示黑棋，-1 表示白棋
 * @return 返回一个整数，表示角落对玩家的影响值
 */
int othelloHeuristic_corners(OTHELLO_BOARD &board, int &color) {
    // 定义四个角落的索引
    int corners[4] = {0, 7, 56, 63};
    // 黑子在角落的数量
    int blackCorners = 0;
    // 白子在角落的数量
    int whiteCorners = 0;

    // 遍历角落索引数组
    for(size_t i = 0; i < sizeof(corners) / sizeof(int); i++){
        // 获取当前角落的索引
        int corner = corners[i];
        // 如果角落上是黑子
        if (board.positions[corner] == 1) {
            blackCorners++;
        }
        // 如果角落上是白子
        else if (board.positions[corner] == -1) {
            whiteCorners++;
        }
    }

    // 如果是黑方
    if (color == 1) {
        // 返回黑子在角落的优势得分
        return 100 * (blackCorners - whiteCorners)
            / (blackCorners + whiteCorners + 1);
    }
    // 如果是白方
    else {
        // 返回白子在角落的优势得分
        return 100 * (whiteCorners - blackCorners)
            / (blackCorners + whiteCorners + 1);
    }
}
