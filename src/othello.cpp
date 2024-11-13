#include "game.hpp"

int promptNewGame();
void initializeGame(int choice, othelloGame &game,
        bool &blackComputer, bool &whiteComputer, float &timeLimit);
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
    othelloBoard board;
    // 初始化棋盘
    othelloGame game;
    // 初始化游戏，询问用户是否愿意开始新游戏
    bool blackComputer = false, whiteComputer = false;
    float timeLimit = 0.0;

    // 初始化游戏，适当提示用户
    // Initialize game, prompting user appropriately
    int newGame = promptNewGame();
    initializeGame(newGame, game, blackComputer, whiteComputer, timeLimit);

    // 开始游戏
    // Play game
    // 如果轮到电脑下棋，先执行电脑下棋
    if (game.toMove == -1) {
        // 寻找合法移动
        game.board.findLegalMoves(-1, &game.board.moves);
        // 显示棋盘
        game.board.displayBoard(-1);
        // 显示合法移动
        game.board.displayLegalMoves();
        // 电脑下棋
        game.move(-1);

        // 检查游戏是否结束
        game.checkGameOver();
    }

    // 当游戏未结束时循环进行
    //while (!game.passes[0] && !game.passes[1]) {
    while (!game.gameOver) {
        // 寻找黑方合法移动
        game.board.findLegalMoves(1, &game.board.moves);
        // 显示棋盘
        game.board.displayBoard(1);
        // 显示合法移动
        game.board.displayLegalMoves();
        // 黑方下棋
        game.move(1);
        // 检查游戏是否结束
        game.checkGameOver();

        // 如果游戏结束，跳出循环
        if (game.gameOver) {
            break;
        }

        // 寻找白方合法移动
        game.board.findLegalMoves(-1, &game.board.moves);
        // 显示棋盘
        game.board.displayBoard(-1);
        // 显示合法移动
        game.board.displayLegalMoves();
        // 白方下棋
        game.move(-1);
        // 检查游戏是否结束
        game.checkGameOver();
    }

    return 0;
}

// Prompt user for new or loaded game
int promptNewGame() {
    int choice = 0;

    std::string str;
    bool validInput = false;
    do {
        std::cout << "Start a new game, or load a game from a save file?"
            << std::endl;
        std::cout << "\t1 -> Start" << std::endl;
        std::cout << "\t2 -> Load" << std::endl;
        std::cout << "\tSelection: ";
        std::cin >> str;
        while (std::cin.get() != '\n');          // clear buffer
        std::istringstream iss(str);
        iss >> choice;

        if (choice == 1 || choice == 2) {
            validInput = true;
        }
        else {
            std::cout << "\tInvalid input. Please try again.\n" << std::endl;
        }
    }
    while (!validInput);
    std::cout << std::endl;

    return choice;
}

// Initialize game appropriately
void initializeGame(int choice, othelloGame &game,
        bool &blackComputer, bool &whiteComputer, float &timeLimit) {
    if (choice == 1) {
        blackComputer = promptAIPlayer(1);
        whiteComputer = promptAIPlayer(-1);

        if (blackComputer || whiteComputer) {
            timeLimit = promptAITimeLimit();
        }

        game.newGame(blackComputer, whiteComputer, timeLimit);
    }
    else {
        std::string fileName;
        std::cout << "Enter file name: ";
        std::cin >> fileName;
        while (std::cin.get() != '\n');          // clear buffer
        std::cout << std::endl;

        blackComputer = promptAIPlayer(1);
        whiteComputer = promptAIPlayer(-1);

        game.loadGame(fileName, blackComputer, whiteComputer);
    }
}

// Prompts user if black/white is the computer
bool promptAIPlayer(int player) {
    bool isAI = false;
    std::string str;
    char ch;
    bool validInput = false;

    do {
        std::cout << "Is " << ((player == 1) ? "black" : "white") << " the computer?" << std::endl;
        std::cout << "\ty -> Yes" << std::endl;
        std::cout << "\tn -> No" << std::endl;
        std::cout << "\tSelection: ";
        std::cin >> str;
        while (std::cin.get() != '\n');          // clear buffer
        std::istringstream iss(str);
        iss >> ch;

        if (ch == 'y') {
            isAI = true;
            validInput = true;
        }
        else if (ch == 'n') {
            isAI = false;
            validInput = true;
        }
        else {
            std::cout << "\tInvalid input. Please try again.\n" << std::endl;
        }
    }
    while (!validInput);
    std::cout << std::endl;

    return isAI; 
}

// Prompts user for a time limit for a computer/AI player
float promptAITimeLimit() {
    float limit = 0;
    std::string str;
    bool validInput = false;

    do {
        std::cout << "Enter time limit for computer (seconds per move): ";
        std::cin >> str;
        while (std::cin.get() != '\n');          // clear buffer
        std::istringstream iss(str);
        iss >> limit;

        if (!iss.eof() || limit <= 0) {
            std::cout << "Invalid input. Please try again.\n" << std::endl;
        }
        else {
            validInput = true;
        }
    }
    while (!validInput);
    std::cout << std::endl;

    return limit;
}
