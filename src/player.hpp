#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <array>
#include <chrono>
#include <climits>
#include <iterator>
#include <sstream>
#include <cstring>
#include "database.hpp"
#include "heuristic.hpp"
#include "moves_hashmap.h"

class othelloPlayer {
    public:
        int color;
        bool computer;

        // Driver for moves, regardless of player
        // std::pair<int, std::list<int>> move(othelloBoard &board,
        //         std::unordered_map<int, std::list<int>> &legalMoves,
        //         bool &pass, std::string &moveHistory);
        MovePair_t *move(othelloBoard *board,
                        MoveHash_t *legalMoves,
                        bool pass, char *moveHistory);

    private:
        struct node {
            bool isMaxNode;
            int alpha;
            int beta;
            int score;
            othelloBoard *board;
        //     std::unordered_map<int, std::list<int>>::iterator prevIterator;
        //     std::unordered_map<int, std::list<int>>::iterator moveIterator;
        //     std::unordered_map<int, std::list<int>>::iterator lastMove;
            MovePair_t *prevIterator;
            MovePair_t *moveIterator;
            MovePair_t *lastMove;
        };

        std::array<node, 64> nodeStack = {};
        //std::array<std::array<int, 2>, 64> killerMoves = {};

        othelloHeuristic heuristic;

        othelloDatabase database;

        // Prompts user for next move
        // std::pair<int, std::list<int>> humanMove(
        //         std::unordered_map<int, std::list<int>> &legalMoves, bool &pass);
        MovePair_t *humanMove(MoveHash_t *legalMoves, bool &pass);

        int coord2index(char *coord);

        // Driver for the AI algorithm
        // std::pair<int, std::list<int>> computerMove(othelloBoard &board,
        //         std::unordered_map<int, std::list<int>> &legalMoves, bool &pass, std::string &moveHistory);
        MovePair_t *computerMove(othelloBoard *board,
                MoveHash_t *legalMoves, bool pass, char *moveHistory);

        // Returns time point
        std::chrono::time_point<std::chrono::system_clock> startTimer();

        // Returns time elapsed in seconds
        float stopTimer(
                std::chrono::time_point<std::chrono::system_clock> startTime);

        // Performs depth-limited minimax search with alpha-beta pruning
        // Implemented using a stack to avoid recursion overhead
        // Returns move for square -1 if time runs out
        MovePair_t *depthLimitedAlphaBeta(
                othelloBoard *theBoard, int depthLimit,
                std::chrono::time_point<std::chrono::system_clock> startTime,
                float timeLimit);
};

#endif //PLAYER_HPP
