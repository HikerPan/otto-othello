#ifndef _OTHELLO_BOARD_H
#define _OTHELLO_BOARD_H

#include <unordered_map>
#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <algorithm>

#define OTHELLO_BOARD_X_SIZE            (8)
#define OTHELLO_BOARD_Y_SIZE            (8)


#define OTHELLO_BOARD_SIZE              (OTHELLO_BOARD_X_SIZE*OTHELLO_BOARD_Y_SIZE)
#define OTHELLO_BOARD_BEGIN_INDEX       (0)
#define OTHELLO_BOARD_END_INDEX         (OTHELLO_BOARD_SIZE-1)

#define OTHELLO_BOARD_BLACK             (1)
#define OTHELLO_BOARD_WHITE             (-1)

typedef struct 
{
        int positions[OTHELLO_BOARD_SIZE];
        int discsOnBoard;
        float timeLimit;
        bool passes[2];
}OTHELLO_BOARD;


class othelloBoard {
    public:
        // positions specifies all pieces on the board. Squares on the
        // board are indexed from 0 to 63, left to right, top to bottom.
        // positions[i] is 1 for a black disc, -1 for a white disc
        std::vector<int> positions;

        int discsOnBoard = 4;
        float timeLimit = 0.0;

        // passes[0] and passes[1] are true if the most recent/second most
        // recent ply was a pass, resp.
        bool passes[2] = {false, false};

        // moves is a hash table specifying all possible moves from the
        // current board position. Possible moves are keys, and a list of
        // all pieces to be flipped are values.
        std::unordered_map<int, std::list<int>> moves;

        // Constructor
        othelloBoard();

        // Display board
        void displayBoard(int color);

        // Display legal moves for player
        void displayLegalMoves();

        // Finds all legal moves, writing to a reference to a hash table with
        // legal moves as keys, and a list of all discs to be flipped as values.
        void findLegalMoves(int color,
                std::unordered_map<int, std::list<int>> *pMoves);

        // Helper function to find a legal move given a disc, its color and a direction.
        // Writes the legal move and a list of all discs to be flipped as a pair to the
        // reference to a hash table.
        void findLegalMoveInDirection(int &disc, int &color, int direction,
                std::unordered_map<int, std::list<int>> *pMoves);

        // Update board after a move
        void updateBoard(int color, std::pair<int, std::list<int>> move);

        bool terminalState();

        // Helper function to convert board square index to coordinate
        // strings
        void index2coord(int index, int &colNum, int &rowNum);
};

#endif // BOARD_HPP
