#include "heuristic.h"
#include "moves_hashmap.h"
// Heuristic is from the perspective that the calling player is the maximizing
// player, regardless of the player's color

// int othelloHeuristic::evaluate(othelloBoard *board, int color) {
//     if (board->terminalState()) {
//         return 100000*utility(board, color);
//     }

//     if (board->discsOnBoard <= 20) {
//         // Opening game
//         return 5*mobility(board, color)
//             + 5*potentialMobility(board, color)
//             + 20*squareWeights(board, color)
//             + 10000*corners(board, color)
//             + 10000*stability(board, color);
//     }
//     else if (board->discsOnBoard <= 58) {
//         // Midgame
//         return 10*discDifference(board, color)
//             + 2*mobility(board, color)
//             + 2*potentialMobility(board, color)
//             + 10*squareWeights(board, color)
//             + 100*parity(board)
//             + 10000*corners(board, color)
//             + 10000*stability(board, color);
//     }
//     else {
//         // Endgame
//         return 500*discDifference(board, color)
//             + 500*parity(board)
//             + 10000*corners(board, color)
//             + 10000*stability(board, color);
//     }
// }

int othelloHeuristic_evaluate(othelloBoard *board, int color) {
    if (othelloBoard_terminalState(board)) {
        return 100000 * othelloHeuristic_utility(board, color);
    }

    if (board->discsOnBoard <= 20) {
        // Opening game
        return 5 * othelloHeuristic_mobility(board, color)
            + 5 * othelloHeuristic_potentialMobility(board, color)
            + 20 * othelloHeuristic_squareWeights(board, color)
            + 10000 * othelloHeuristic_corners(board, color)
            + 10000 * othelloHeuristic_stability(board, color);
    } else if (board->discsOnBoard <= 58) {
        // Midgame
        return 10 * othelloHeuristic_discDifference(board, color)
            + 2 * othelloHeuristic_mobility(board, color)
            + 2 * othelloHeuristic_potentialMobility(board, color)
            + 10 * othelloHeuristic_squareWeights(board, color)
            + 100 * othelloHeuristic_parity(board)
            + 10000 * othelloHeuristic_corners(board, color)
            + 10000 * othelloHeuristic_stability(board, color);
    } else {
        // Endgame
        return 500 * othelloHeuristic_discDifference(board, color)
            + 500 * othelloHeuristic_parity(board)
            + 10000 * othelloHeuristic_corners(board, color)
            + 10000 * othelloHeuristic_stability(board, color);
    }
}


// int othelloHeuristic::utility(othelloBoard *board, int color) {
//     // int util = std::accumulate(board.positions.begin(),
//     //         board.positions.end(), 0);
//     int util = board_accumulate(board->positions,1,64,0);

//     if (color == 1) {
//         return util; 
//     }
//     else {
//         return -1*util;
//     }
// }
int othelloHeuristic_utility(othelloBoard *board, int color) {
    int util = board_accumulate(board->positions, 1, 64, 0);
    return (color == 1) ? util : -1 * util;
}

// Relative disc difference between the two players
// int othelloHeuristic::discDifference(othelloBoard *board, int color) {
//     // int blackCount = std::count(board.positions.begin(),
//     //         board.positions.end(), 1);
//     // int whiteCount = std::count(board.positions.begin(),
//     //         board.positions.end(), -1);

//     int blackCount = board_count(board->positions,1,64,1);
//     int whiteCount = board_count(board->positions,1,64,-1);

//     if (color == 1) {
//         return 100 * (blackCount - whiteCount) / (blackCount + whiteCount);
//     }
//     else {
//         return 100 * (whiteCount - blackCount) / (blackCount + whiteCount);
//     }
// }
int othelloHeuristic_discDifference(othelloBoard *board, int color) {
    int blackCount = board_count(board->positions, 1, 64, 1);
    int whiteCount = board_count(board->positions, 1, 64, -1);

    if (color == 1) {
        return 100 * (blackCount - whiteCount) / (blackCount + whiteCount);
    } else {
        return 100 * (whiteCount - blackCount) / (blackCount + whiteCount);
    }
}

// Number of possible moves

int othelloHeuristic_mobility(othelloBoard *board, int color) {
    MoveHash_t *pMoves = NULL;
    othelloBoard_findLegalMoves(board,1, &pMoves);
    int blackMoves = size_moves(pMoves);
    clear_moves(pMoves);

    othelloBoard_findLegalMoves(board,-1, &pMoves);
    int whiteMoves = size_moves(pMoves);
    clear_moves(pMoves);

    if (color == 1) {
        return 100 * (blackMoves - whiteMoves) / (blackMoves + whiteMoves + 1);
    } else {
        return 100 * (whiteMoves - blackMoves) / (blackMoves + whiteMoves + 1);
    }
}

// int othelloHeuristic::potentialMobility(othelloBoard *board, int color) {
//     int myPotentialMobility = playerPotentialMobility(board, color);
//     int opponentPotentialMobility = playerPotentialMobility(board, -color);

//     return 100 * (myPotentialMobility - opponentPotentialMobility)
//         / (myPotentialMobility + opponentPotentialMobility + 1);
// }

// 计算潜在的机动性
int othelloHeuristic_potentialMobility(othelloBoard *board, int color) {
    int myPotentialMobility = othelloHeuristic_playerPotentialMobility(board, color);
    int opponentPotentialMobility = othelloHeuristic_playerPotentialMobility(board, -color);

    return 100 * (myPotentialMobility - opponentPotentialMobility)
           / (myPotentialMobility + opponentPotentialMobility + 1);
}

// int othelloHeuristic::playerPotentialMobility(othelloBoard *board, int color) {
//     // std::vector<int> boardInterior = {18, 19, 20, 21,
//     //                                   26, 27, 28, 29,
//     //                                   34, 35, 36, 37,
//     //                                   42, 43, 44, 45};
//     int boardInterior[] = {18, 19, 20, 21,
//                             26, 27, 28, 29,
//                             34, 35, 36, 37,
//                             42, 43, 44, 45};

//     int here = 0, up = 0, down = 0, left = 0, right = 0,
//         upperLeft = 0, upperRight = 0, lowerLeft = 0, lowerRight = 0;
//     int potentialMobility = 0;
//     int square = 0;
//     // size_t i = 0;

//     // for (int square : boardInterior) {
//     for(size_t i = 0; i < sizeof(boardInterior)/sizeof(boardInterior[0]); i++){
//         square = boardInterior[i];
        
//         here = board->positions[square];
//         up = board->positions[square-8];
//         down = board->positions[square+8];
//         left = board->positions[square-1];
//         right = board->positions[square+1];
//         upperLeft = board->positions[square-9];
//         upperRight = board->positions[square-7];
//         lowerLeft = board->positions[square+7];
//         lowerRight = board->positions[square+9];

//         if (here == -color && up == 0)
//             potentialMobility++;
//         if (here == -color && down == 0)
//             potentialMobility++;
//         if (here == -color && right == 0)
//             potentialMobility++;
//         if (here == -color && right == 0)
//             potentialMobility++;
//         if (here == -color && upperLeft == 0)
//             potentialMobility++;
//         if (here == -color && upperRight == 0)
//             potentialMobility++;
//         if (here == -color && lowerLeft == 0)
//             potentialMobility++;
//         if (here == -color && lowerRight == 0)
//             potentialMobility++;
//     }

//     // std::vector<int> topRow = {10, 11, 12, 13};
//     int topRow [] = {10, 11, 12, 13};
//     // for (int square : topRow) {
//     for(size_t i = 0; i < sizeof(topRow)/sizeof(topRow[0]); i++){
//         square = topRow[i];
//         here = board->positions[square];
//         left = board->positions[square-1];
//         right = board->positions[square+1];
//         if (here == -color && left == 0)
//             potentialMobility++;
//         if (here == -color && right == 0)
//             potentialMobility++;
//     }

//     // std::vector<int> bottomRow = {50, 51, 52, 53};
//     int bottomRow [] = {50, 51, 52, 53};
    
//     // for (int square : bottomRow) {
//     for(size_t i = 0; i < sizeof(bottomRow)/sizeof(bottomRow[0]); i++){
//         square = bottomRow[i];
    
//         here = board->positions[square];
//         left = board->positions[square-1];
//         right = board->positions[square+1];
//         if (here == -color && left == 0)
//             potentialMobility++;
//         if (here == -color && right == 0)
//             potentialMobility++;
//     }

//     // std::vector<int> leftColumn = {17, 25, 33, 41};
//     int leftColumn [] = {17, 25, 33, 41};
    
//     // for (int square : leftColumn) {
//     for(size_t i = 0; i < sizeof(leftColumn)/sizeof(leftColumn[0]); i++){
//         square = leftColumn[i];
    
//         here = board->positions[square];
//         up = board->positions[square-8];
//         down = board->positions[square+8];
//         if (here == -color && up == 0)
//             potentialMobility++;
//         if (here == -color && down == 0)
//             potentialMobility++;
//     }

//     // std::vector<int> rightColumn = {22, 30, 38, 46};
//     int rightColumn [] = {22, 30, 38, 46};
    
//     // for (int square : rightColumn) {
//     for(size_t i = 0; i < sizeof(rightColumn)/sizeof(rightColumn[0]); i++){
//         square = rightColumn[i];
    
//         here = board->positions[square];
//         up = board->positions[square-8];
//         down = board->positions[square+8];
//         if (here == -color && up == 0)
//             potentialMobility++;
//         if (here == -color && down == 0)
//             potentialMobility++;
//     }

//     return potentialMobility;
// }

int othelloHeuristic_playerPotentialMobility(othelloBoard *board, int color) {
    int boardInterior[] = {18, 19, 20, 21,
                           26, 27, 28, 29,
                           34, 35, 36, 37,
                           42, 43, 44, 45};
    int topRow[] = {10, 11, 12, 13};
    int bottomRow[] = {50, 51, 52, 53};
    int leftColumn[] = {17, 25, 33, 41};
    int rightColumn[] = {22, 30, 38, 46};

    int here, up, down, left, right, upperLeft, upperRight, lowerLeft, lowerRight;
    int potentialMobility = 0;
    int square;

    // 检查棋盘内部区域
    for (size_t i = 0; i < sizeof(boardInterior) / sizeof(boardInterior[0]); i++) {
        square = boardInterior[i];

        here = board->positions[square];
        up = board->positions[square - 8];
        down = board->positions[square + 8];
        left = board->positions[square - 1];
        right = board->positions[square + 1];
        upperLeft = board->positions[square - 9];
        upperRight = board->positions[square - 7];
        lowerLeft = board->positions[square + 7];
        lowerRight = board->positions[square + 9];

        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
        if (here == -color && left == 0)
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

    // 检查顶部行
    for (size_t i = 0; i < sizeof(topRow) / sizeof(topRow[0]); i++) {
        square = topRow[i];
        here = board->positions[square];
        left = board->positions[square - 1];
        right = board->positions[square + 1];

        if (here == -color && left == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
    }

    // 检查底部行
    for (size_t i = 0; i < sizeof(bottomRow) / sizeof(bottomRow[0]); i++) {
        square = bottomRow[i];
        here = board->positions[square];
        left = board->positions[square - 1];
        right = board->positions[square + 1];

        if (here == -color && left == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
    }

    // 检查左列
    for (size_t i = 0; i < sizeof(leftColumn) / sizeof(leftColumn[0]); i++) {
        square = leftColumn[i];
        here = board->positions[square];
        up = board->positions[square - 8];
        down = board->positions[square + 8];

        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
    }

    // 检查右列
    for (size_t i = 0; i < sizeof(rightColumn) / sizeof(rightColumn[0]); i++) {
        square = rightColumn[i];
        here = board->positions[square];
        up = board->positions[square - 8];
        down = board->positions[square + 8];

        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
    }

    return potentialMobility;
}
// // Computes a lower bound on the number of stable discs
// int othelloHeuristic::stability(othelloBoard *board, int color) {
//     stableDiscs.clear();

//     stableDiscsFromCorner(board, 0, color);
//     stableDiscsFromCorner(board, 7, color);
//     stableDiscsFromCorner(board, 56, color);
//     stableDiscsFromCorner(board, 63, color);

//     int myStables = stableDiscs.size();

//     stableDiscsFromCorner(board, 0, -color);
//     stableDiscsFromCorner(board, 7, -color);
//     stableDiscsFromCorner(board, 56, -color);
//     stableDiscsFromCorner(board, 63, -color);

//     int opponentStables = stableDiscs.size();

//     return myStables - opponentStables;
// }

int othelloHeuristic_stability(othelloBoard *board, int color) {
    IntSetNode *stableDiscs = NULL;

    othelloHeuristic_stableDiscsFromCorner(board, 0, color, &stableDiscs);
    othelloHeuristic_stableDiscsFromCorner(board, 7, color, &stableDiscs);
    othelloHeuristic_stableDiscsFromCorner(board, 56, color, &stableDiscs);
    othelloHeuristic_stableDiscsFromCorner(board, 63, color, &stableDiscs);

    int myStables = count_set_elements(stableDiscs);
    clear_set(&stableDiscs);

    othelloHeuristic_stableDiscsFromCorner(board, 0, -color, &stableDiscs);
    othelloHeuristic_stableDiscsFromCorner(board, 7, -color, &stableDiscs);
    othelloHeuristic_stableDiscsFromCorner(board, 56, -color, &stableDiscs);
    othelloHeuristic_stableDiscsFromCorner(board, 63, -color, &stableDiscs);

    int opponentStables = count_set_elements(stableDiscs);
    clear_set(&stableDiscs);

    return myStables - opponentStables;
}


// Finds the number of stable discs given a corner
// void othelloHeuristic::stableDiscsFromCorner(othelloBoard *board, int corner,
//         int color) {
//     bool down, right;
//     if (corner == 0) {
//         down = true;
//         right = true;
//     }
//     else if (corner == 7) {
//         down = true;
//         right = false;
//     }
//     else if (corner == 56) {
//         down = false;
//         right = true;
//     }
//     else {
//         down = false;
//         right = false;
//     }

//     int horizIncr = 1, horizStop = 7, vertIncr = 8, vertStop = 56;
//     if (!right) {
//         horizIncr *= -1;
//         horizStop *= -1;
//     }
//     if (!down) {
//         vertIncr *= -1;
//         vertStop *= -1;
//     }

//     // Iterate horizontally
//     for (int i = corner; i != corner + horizIncr + horizStop; i += horizIncr) {
//         // If there is a disc of our color on this square
//         if (board->positions[i] == color) {
//             // Iterate vertically
//             for (int j = i; j != i + vertStop; j+= vertIncr) {
//                 // If there is a disc of our color on this square,
//                 // and it is not in the set of stable discs
//                 if (board->positions[j] == color
//                         && stableDiscs.find(j) == stableDiscs.end()) {
//                     // Insert it to the set
//                     stableDiscs.insert(j);
//                 }
//                 // If we come across a disc of the opposite color, break
//                 else {
//                     break;
//                 }
//             }
//         }
//         // Otherwise, break
//         else {
//             break;
//         }
//     }
// }
void othelloHeuristic_stableDiscsFromCorner(othelloBoard *board, int corner, int color, IntSetNode **stableDiscs) {
    bool down, right;
    if (corner == 0) {
        down = true;
        right = true;
    } else if (corner == 7) {
        down = true;
        right = false;
    } else if (corner == 56) {
        down = false;
        right = true;
    } else {
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
        if (board->positions[i] == color) {
            // Iterate vertically
            for (int j = i; j != i + vertStop; j += vertIncr) {
                // If there is a disc of our color on this square,
                // and it is not in the set of stable discs
                if (board->positions[j] == color && !is_in_set(*stableDiscs, j)) {
                    // Insert it to the set
                    add_to_set(stableDiscs, j);
                } else {
                    break;
                }
            }
        } else {
            break;
        }
    }
}


// int othelloHeuristic::parity(othelloBoard *board) {
//     int squaresRemaining = 64 - board->discsOnBoard;

//     if (squaresRemaining % 2 == 0) {
//         return -1;
//     }
//     else {
//         return 1;
//     }
// }

// 计算棋盘上剩余的空白格子的奇偶性
int parity(othelloBoard *board) {
    int squaresRemaining = 64 - board->discsOnBoard;

    if (squaresRemaining % 2 == 0) {
        return -1; // 偶数
    } else {
        return 1; // 奇数
    }
}


// Assigns a weight to every square on the board
// int othelloHeuristic::squareWeights(othelloBoard *board, int color) {
//     // std::vector<int> weights = {
//     //      200, -100, 100,  50,  50, 100, -100,  200,
//     //     -100, -200, -50, -50, -50, -50, -200, -100,
//     //      100,  -50, 100,   0,   0, 100,  -50,  100,
//     //       50,  -50,   0,   0,   0,   0,  -50,   50,
//     //       50,  -50,   0,   0,   0,   0,  -50,   50,
//     //      100,  -50, 100,   0,   0, 100,  -50,  100,
//     //     -100, -200, -50, -50, -50, -50, -200, -100,
//     //      200, -100, 100,  50,  50, 100, -100,  200,
//     // };
//     int weights[] = {
//          200, -100, 100,  50,  50, 100, -100,  200,
//         -100, -200, -50, -50, -50, -50, -200, -100,
//          100,  -50, 100,   0,   0, 100,  -50,  100,
//           50,  -50,   0,   0,   0,   0,  -50,   50,
//           50,  -50,   0,   0,   0,   0,  -50,   50,
//          100,  -50, 100,   0,   0, 100,  -50,  100,
//         -100, -200, -50, -50, -50, -50, -200, -100,
//          200, -100, 100,  50,  50, 100, -100,  200,
//     };
    

//     if (board->positions[0] != 0) {
//         weights[1]  = 0;
//         weights[2]  = 0;
//         weights[3]  = 0;
//         weights[8]  = 0;
//         weights[9]  = 0;
//         weights[10] = 0;
//         weights[11] = 0;
//         weights[16] = 0;
//         weights[17] = 0;
//         weights[18] = 0;
//         weights[24] = 0;
//         weights[25] = 0;
//     }

//     if (board->positions[7] != 0) {
//         weights[4]  = 0;
//         weights[5]  = 0;
//         weights[6]  = 0;
//         weights[12] = 0;
//         weights[13] = 0;
//         weights[14] = 0;
//         weights[15] = 0;
//         weights[21] = 0;
//         weights[22] = 0;
//         weights[23] = 0;
//         weights[30] = 0;
//         weights[31] = 0;
//     }

//     if (board->positions[56] != 0) {
//         weights[32] = 0;
//         weights[33] = 0;
//         weights[40] = 0;
//         weights[41] = 0;
//         weights[42] = 0;
//         weights[48] = 0;
//         weights[49] = 0;
//         weights[50] = 0;
//         weights[51] = 0;
//         weights[57] = 0;
//         weights[58] = 0;
//         weights[59] = 0;
//     }

//     if (board->positions[63] != 0) {
//         weights[38] = 0;
//         weights[39] = 0;
//         weights[45] = 0;
//         weights[46] = 0;
//         weights[47] = 0;
//         weights[52] = 0;
//         weights[53] = 0;
//         weights[54] = 0;
//         weights[55] = 0;
//         weights[60] = 0;
//         weights[61] = 0;
//         weights[62] = 0;
//     }

//     int count = 0;
//     for(size_t i =0;i<OTHELLO_BOARD_SIZE;i++)
//     {
//         count = board->positions[i]*weights[i];
//     }

//     if (color == 1) {
//         // return std::inner_product(board.positions.begin(),
//         //         board.positions.end(), weights.begin(), 0);
//         return count;
//     }
//     else {
//         // return -1*std::inner_product(board.positions.begin(),
//         //         board.positions.end(), weights.begin(), 0);
//         return -1*count;
//     }
// }

int squareWeights(othelloBoard *board, int color) {
    int weights[64] = {
         200, -100, 100,  50,  50, 100, -100,  200,
        -100, -200, -50, -50, -50, -50, -200, -100,
         100,  -50, 100,   0,   0, 100,  -50,  100,
          50,  -50,   0,   0,   0,   0,  -50,   50,
          50,  -50,   0,   0,   0,   0,  -50,   50,
         100,  -50, 100,   0,   0, 100,  -50,  100,
        -100, -200, -50, -50, -50, -50, -200, -100,
         200, -100, 100,  50,  50, 100, -100,  200,
    };

    // Adjust weights for corners
    if (board->positions[0] != 0) {
        int cornerOffsets1[] = {1, 2, 3, 8, 9, 10, 11, 16, 17, 18, 24, 25};
        for (int i = 0; i < 12; i++) {
            weights[cornerOffsets1[i]] = 0;
        }
    }
    if (board->positions[7] != 0) {
        int cornerOffsets2[] = {4, 5, 6, 12, 13, 14, 15, 21, 22, 23, 30, 31};
        for (int i = 0; i < 12; i++) {
            weights[cornerOffsets2[i]] = 0;
        }
    }
    if (board->positions[56] != 0) {
        int cornerOffsets3[] = {32, 33, 40, 41, 42, 48, 49, 50, 51, 57, 58, 59};
        for (int i = 0; i < 12; i++) {
            weights[cornerOffsets3[i]] = 0;
        }
    }
    if (board->positions[63] != 0) {
        int cornerOffsets4[] = {38, 39, 45, 46, 47, 52, 53, 54, 55, 60, 61, 62};
        for (int i = 0; i < 12; i++) {
            weights[cornerOffsets4[i]] = 0;
        }
    }

    // Calculate the weighted sum based on positions and weights
    int count = 0;
    for (size_t i = 0; i < 64; i++) {
        count += board->positions[i] * weights[i];
    }

    return (color == 1) ? count : -count;
}

// int othelloHeuristic::corners(othelloBoard *board, int &color) {
//     // std::vector<int> corners = {0, 7, 56, 63};
//     int corners[] = {0, 7, 56, 63};
//     int blackCorners = 0;
//     int whiteCorners = 0;

//     // for (int corner : corners) {
//     for (int i = 0; i < 4; i++) {
//         int corner = corners[i];
//         if (board->positions[corner] == 1) {
//             blackCorners++;
//         }
//         else if (board->positions[corner] == -1) {
//             whiteCorners++;
//         }
//     }

//     if (color == 1) {
//         return 100 * (blackCorners - whiteCorners)
//             / (blackCorners + whiteCorners + 1);
//     }
//     else {
//         return 100 * (whiteCorners - blackCorners)
//             / (blackCorners + whiteCorners + 1);
//     }
// }

int othelloHeuristic_corners(othelloBoard *board, int color) {
    int corners[] = {0, 7, 56, 63};
    int blackCorners = 0;
    int whiteCorners = 0;

    // 遍历角落位置，统计黑白方占据的角落数
    for (int i = 0; i < 4; i++) {
        int corner = corners[i];
        if (board->positions[corner] == 1) {
            blackCorners++;
        }
        else if (board->positions[corner] == -1) {
            whiteCorners++;
        }
    }

    // 根据当前颜色返回适当的角落优势值
    if (color == 1) {
        return 100 * (blackCorners - whiteCorners) / (blackCorners + whiteCorners + 1);
    }
    else {
        return 100 * (whiteCorners - blackCorners) / (blackCorners + whiteCorners + 1);
    }
}