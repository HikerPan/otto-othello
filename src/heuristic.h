#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP



#include "board.h"
#include "moves_hashmap.h"

typedef struct {
    MovePair_t *pMoves; // 用于存储合法的移动选项
    IntSetNode *intSets; // 可能需要使用其他适合C的结构来存储稳定的棋子
} othelloHeuristic;

int othelloHeuristic_evaluate(othelloBoard *board, int color);
int othelloHeuristic_utility(othelloBoard *board, int color);
int othelloHeuristic_discDifference(othelloBoard *board, int color);
int othelloHeuristic_mobility(othelloBoard *board, int color);
int othelloHeuristic_potentialMobility(othelloBoard *board, int color);
int othelloHeuristic_playerPotentialMobility(othelloBoard *board, int color);
int othelloHeuristic_stability(othelloBoard *board, int color);
void othelloHeuristic_stableDiscsFromCorner(othelloBoard *board, int corner, int color, IntSetNode **stableDiscs);
int othelloHeuristic_parity(othelloBoard *board);
int othelloHeuristic_squareWeights(othelloBoard *board, int color);
int othelloHeuristic_corners(othelloBoard *board, int color);



#endif // HEURISTIC_HPP
