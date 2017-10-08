#include "player.hpp"

// Driver for player's move, regardless of player
std::pair<int, std::list<int>> othelloPlayer::move(
        const std::unordered_map<int, std::list<int>> &legalMoves,
        bool &pass) {

    std::pair<int, std::list<int>> moveChoice;
    if (this->computer) {
        moveChoice = this->computerMove(legalMoves, pass);
    }
    else {
        moveChoice = this->humanMove(legalMoves, pass);
    }

    return moveChoice;
}

// Prompts user for next move
std::pair<int, std::list<int>> othelloPlayer::humanMove(
        const std::unordered_map<int, std::list<int>> &legalMoves,
        bool &pass) {

    std::string str;
    std::pair<int, std::list<int>> move;
    int choice = 0;
    bool validInput = false;
    int i = 0;

    if (legalMoves.empty()) {
        std::cout << "No legal moves!" << std::endl;
        std::cout << "\tEnter any string to pass: ";
        std::cin >> str;
        std::cout << std::endl;
        pass = true;
        return move;
    }

    do {
        std::cout << "\tSelect move number: ";
        std::cin >> str;
        std::istringstream iss(str);
        iss >> choice;

        if (!iss.eof() || choice > legalMoves.size() || choice < 1) {
            std::cout << "\tInvalid input. Please try again.\n" << std::endl;
        }
        else {
            validInput = true;
            std::cout << std::endl;
        }
    }
    while (!validInput);

    for (std::pair<int, std::list<int>> keyval : legalMoves) {
        move = keyval;
        i++;
        if (i == choice) {
            break;
        }
    }

    return move;
}

// Performs minimax search with alpha-beta pruning
std::pair<int, std::list<int>> othelloPlayer::computerMove(
        const std::unordered_map<int, std::list<int>> &legalMoves,
        bool &pass) {
    std::pair<int, std::list<int>> move;

    if (legalMoves.empty()) {
        std::cout << "No legal moves!" << std::endl;
        std::cout << "\tComputer passes." << std::endl << std::endl;
        pass = true;
        return move;
    }

    // FIXME just takes first move... for now.
    std::cout << "\tComputer makes random move..." << std::endl << std::endl;

    for (auto keyval : legalMoves) {
        move = keyval;
        break;
    }
    return move;
}

int othelloPlayer::minimax() {
    return 0;
}

int othelloPlayer::maxValue() {
    return 0;
}

int othelloPlayer::minValue() {
    return 0;
}
