#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <numeric>
#include <unordered_set>
#include "othello_board.h"

typedef struct{
    // TODO:
    // std::unordered_set<int> stableDiscs;
    // std::unordered_map<int, std::list<int>> pMoves;

}OTHELLO_HEURISTIC_T;

int othelloHeuristic_evaluate(OTHELLO_BOARD &board, int color);
int othelloHeuristic_utility(OTHELLO_BOARD &board, int &color);
int othelloHeuristic_discDifference(OTHELLO_BOARD &board, int &color);
int othelloHeuristic_mobility(OTHELLO_BOARD &board, int &color);
int othelloHeuristic_potentialMobility(OTHELLO_BOARD &board, int color);
int othelloHeuristic_playerPotentialMobility(OTHELLO_BOARD &board, int color);
int othelloHeuristic_stability(OTHELLO_BOARD &board, int color);
void othelloHeuristic_stableDiscsFromCorner(OTHELLO_BOARD &board, int corner, int color);
int othelloHeuristic_parity(OTHELLO_BOARD &board);
int othelloHeuristic_squareWeights(OTHELLO_BOARD &board, int &color);
int othelloHeuristic_corners(OTHELLO_BOARD &board, int &color);

class othelloHeuristic {
    public:
        int evaluate(othelloBoard &board, int color);

    private:
        std::unordered_set<int> stableDiscs;
        std::unordered_map<int, std::list<int>> pMoves;

        int utility(othelloBoard &board, int &color);
        int discDifference(othelloBoard &board, int &color);
        int mobility(othelloBoard &board, int &color);
        int potentialMobility(othelloBoard &board, int color);
        int playerPotentialMobility(othelloBoard &board, int color);
        int stability(othelloBoard &board, int color);
        void stableDiscsFromCorner(othelloBoard &board,
                int corner, int color);
        int parity(othelloBoard &board);
        int squareWeights(othelloBoard &board, int &color);
        int corners(othelloBoard &board, int &color);
};

#endif // HEURISTIC_HPP
