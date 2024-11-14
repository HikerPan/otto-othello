#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int promptNewGame();
// void initializeGame(int choice, othelloGame &game,
//         bool &blackComputer, bool &whiteComputer, float &timeLimit);
// bool promptAIPlayer(int player);
// float promptAITimeLimit();

int promptNewGame();
void initializeGame(int choice, othelloGame *game, bool *blackComputer, bool *whiteComputer, float *timeLimit);
bool promptAIPlayer(int player);
float promptAITimeLimit();

/**
 * @brief 主函数，用于运行井字棋游戏
 *
 * 该函数初始化井字棋游戏，提示用户输入游戏设置，并开始游戏。游戏循环进行，直到一方获胜或双方都无法下棋为止。
 *
 * @return 返回值始终为0，表示程序正常结束
 */


int main() {
    othelloGame game;
    bool blackComputer = false, whiteComputer = false;
    float timeLimit = 0.0;


    othelloGame_init(&game);

    int newGame = promptNewGame();
    initializeGame(newGame, &game, &blackComputer, &whiteComputer, &timeLimit);

    if (game.toMove == -1) {
        othelloBoard_findLegalMoves(&game.board,-1, &game.board.moves);
        othelloBoard_displayBoard(&game.board,-1);
        othelloBoard_displayLegalMoves(&game.board);
        othelloGame_move(&game,-1);
        othelloGame_checkGameOver(&game);
    }

    while (!game.gameOver) {

        othelloBoard_findLegalMoves(&game.board,1, &game.board.moves);
        othelloBoard_displayBoard(&game.board,1);
        othelloBoard_displayLegalMoves(&game.board);
        othelloGame_move(&game,1);
        othelloGame_checkGameOver(&game);

        if (game.gameOver) {
            break;
        }

        othelloBoard_findLegalMoves(&game.board,-1, &game.board.moves);
        othelloBoard_displayBoard(&game.board,-1);
        othelloBoard_displayLegalMoves(&game.board);
        othelloGame_move(&game,-1);
        othelloGame_checkGameOver(&game);
    }

    return 0;
}


// Prompt user for new or loaded game
// int promptNewGame() {
//     int choice = 0;

//     std::string str;
//     bool validInput = false;
//     do {
//         std::cout << "Start a new game, or load a game from a save file?"
//             << std::endl;
//         std::cout << "\t1 -> Start" << std::endl;
//         std::cout << "\t2 -> Load" << std::endl;
//         std::cout << "\tSelection: ";
//         std::cin >> str;
//         while (std::cin.get() != '\n');          // clear buffer
//         std::istringstream iss(str);
//         iss >> choice;

//         if (choice == 1 || choice == 2) {
//             validInput = true;
//         }
//         else {
//             std::cout << "\tInvalid input. Please try again.\n" << std::endl;
//         }
//     }
//     while (!validInput);
//     std::cout << std::endl;

//     return choice;
// }
int promptNewGame() {
    int choice = 0;
    char str[10];
    int validInput = 0;

    do {
        printf("Start a new game, or load a game from a save file?\n");
        printf("\t1 -> Start\n");
        printf("\t2 -> Load\n");
        printf("\tSelection: ");
        scanf("%9s", str);
        getchar();  // clear buffer
        choice = atoi(str);

        if (choice == 1 || choice == 2) {
            validInput = 1;
        } else {
            printf("\tInvalid input. Please try again.\n\n");
        }
    } while (!validInput);
    printf("\n");

    return choice;
}

// Initialize game appropriately
// void initializeGame(int choice, othelloGame &game,
//         bool &blackComputer, bool &whiteComputer, float &timeLimit) {
//     if (choice == 1) {
//         blackComputer = promptAIPlayer(1);
//         whiteComputer = promptAIPlayer(-1);

//         if (blackComputer || whiteComputer) {
//             timeLimit = promptAITimeLimit();
//         }

//         game.newGame(blackComputer, whiteComputer, timeLimit);
//     }
//     else {
//         std::string fileName;
//         std::cout << "Enter file name: ";
//         std::cin >> fileName;
//         while (std::cin.get() != '\n');          // clear buffer
//         std::cout << std::endl;

//         blackComputer = promptAIPlayer(1);
//         whiteComputer = promptAIPlayer(-1);

//         game.loadGame(fileName, blackComputer, whiteComputer);
//     }
// }
void initializeGame(int choice, othelloGame *game, bool *blackComputer, bool *whiteComputer, float *timeLimit) {
    if (choice == 1) {
        *blackComputer = promptAIPlayer(1);
        *whiteComputer = promptAIPlayer(-1);

        if (*blackComputer || *whiteComputer) {
            *timeLimit = promptAITimeLimit();
        }

        othelloGame_newGame(game,*blackComputer,*whiteComputer,*timeLimit);
    } else {
        char fileName[100];
        printf("Enter file name: ");
        scanf("%99s", fileName);
        getchar();  // clear buffer
        printf("\n");

        *blackComputer = promptAIPlayer(1);
        *whiteComputer = promptAIPlayer(-1);

        othelloGame_loadGame(game,fileName, *blackComputer, *whiteComputer);
    }
}

// Prompts user if black/white is the computer
// bool promptAIPlayer(int player) {
//     bool isAI = false;
//     std::string str;
//     char ch;
//     bool validInput = false;

//     do {
//         std::cout << "Is " << ((player == 1) ? "black" : "white") << " the computer?" << std::endl;
//         std::cout << "\ty -> Yes" << std::endl;
//         std::cout << "\tn -> No" << std::endl;
//         std::cout << "\tSelection: ";
//         std::cin >> str;
//         while (std::cin.get() != '\n');          // clear buffer
//         std::istringstream iss(str);
//         iss >> ch;

//         if (ch == 'y') {
//             isAI = true;
//             validInput = true;
//         }
//         else if (ch == 'n') {
//             isAI = false;
//             validInput = true;
//         }
//         else {
//             std::cout << "\tInvalid input. Please try again.\n" << std::endl;
//         }
//     }
//     while (!validInput);
//     std::cout << std::endl;

//     return isAI; 
// }

// // Prompts user for a time limit for a computer/AI player
// float promptAITimeLimit() {
//     float limit = 0;
//     std::string str;
//     bool validInput = false;

//     do {
//         std::cout << "Enter time limit for computer (seconds per move): ";
//         std::cin >> str;
//         while (std::cin.get() != '\n');          // clear buffer
//         std::istringstream iss(str);
//         iss >> limit;

//         if (!iss.eof() || limit <= 0) {
//             std::cout << "Invalid input. Please try again.\n" << std::endl;
//         }
//         else {
//             validInput = true;
//         }
//     }
//     while (!validInput);
//     std::cout << std::endl;

//     return limit;
// }
bool promptAIPlayer(int player) {
    char str[10];
    char ch;
    int validInput = 0;
    bool isAI = false;

    do {
        printf("Is %s the computer?\n", (player == 1) ? "black" : "white");
        printf("\ty -> Yes\n");
        printf("\tn -> No\n");
        printf("\tSelection: ");
        scanf("%9s", str);
        getchar();  // clear buffer
        ch = str[0];

        if (ch == 'y') {
            isAI = true;
            validInput = 1;
        } else if (ch == 'n') {
            isAI = false;
            validInput = 1;
        } else {
            printf("\tInvalid input. Please try again.\n\n");
        }
    } while (!validInput);
    printf("\n");

    return isAI; 
}

// Prompts user for a time limit for a computer/AI player
float promptAITimeLimit() {
    char str[10];
    float limit = 0;
    int validInput = 0;

    do {
        printf("Enter time limit for computer (seconds per move): ");
        scanf("%9s", str);
        getchar();  // clear buffer
        limit = atof(str);

        if (limit <= 0) {
            printf("Invalid input. Please try again.\n\n");
        } else {
            validInput = 1;
        }
    } while (!validInput);
    printf("\n");

    return limit;
}
