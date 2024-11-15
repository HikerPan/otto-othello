#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "heuristic.h"
#include "player.h"
#include "moves_hashmap.h"

// Driver for player's move, regardless of player
/**
 * @brief 执行玩家的移动操作
 *
 * 根据玩家类型（电脑或人类），选择不同的移动策略，并更新历史移动记录。
 *
 * @param board 游戏棋盘
 * @param legalMoves 合法的移动选项
 * @param pass 是否轮到对手弃权
 * @param moveHistory 历史移动记录
 * @return 返回移动结果，包含移动的行索引和列索引列表
 */
MovePair_t *othelloPlayer_move(othelloPlayer *player,othelloBoard *board,MovePair_t *legalMoves,bool pass,char *moveHistory) {
    MovePair_t *moveChoice = NULL;

    if (player->computer) {
        moveChoice = othelloPlayer_computerMove(player, board, legalMoves, pass, moveHistory);
    } else {
        moveChoice = othelloPlayer_humanMove(player, legalMoves, &pass);
    }
    
    if (moveChoice == NULL)
        return NULL;

    char temp[8] = {0};
    sprintf(temp, "%d,", moveChoice->position);
    strcat(moveHistory, temp);
    printf("moveHistory: %s\n", moveHistory);

    return moveChoice;
}

// Prompts user for next move
/**
 * @brief 人机对战中，玩家选择合法走法
 *
 * 该函数用于玩家在对弈过程中选择合法的走法。如果当前没有合法的走法，则提示玩家输入任意非空字符串表示跳过回合。
 *
 * @param legalMoves 当前所有合法的走法
 * @param pass 是否选择跳过回合
 * @return std::pair<int, std::list<int>> 玩家选择的走法，第一个元素为走法的编号或坐标索引，第二个元素为对应的走法列表
 */

MovePair_t *othelloPlayer_humanMove(othelloPlayer *player,MovePair_t *legalMoves,bool *pass) {
    // 存储用户输入的字符串
    char str[20];
    // 存储用户选择的移动
    MovePair_t *move = NULL;
    // 记录用户输入的移动编号
    int moveNum = 0;
    // 记录用户输入的坐标索引
    int coordIndex = -1;
    // 记录用户输入是否有效
    bool validInput = false;

    // 如果没有合法移动
    if (move_hash_empty(legalMoves)) {
        // 输出没有合法移动的信息
        printf("No legal moves!\n");
        // 提示用户输入任意非空字符串表示放弃
        printf("\tEnter any non-empty string to pass: ");
        // 读取用户输入的字符串
        scanf("%19s", str); // 限制输入长度以防止缓冲区溢出

        // 清除输入缓冲区
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        printf("\n");
        // 设置pass为true表示用户选择放弃
        *pass = true;
        // 返回空的移动
        return move;
    }

    // 循环直到用户输入有效
    do {
        // 提示用户选择移动编号或坐标
        printf("\tSelect move number/square coordinate: ");
        // 读取用户输入的字符串
        scanf("%19s", str);

        // 清除输入缓冲区
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        // 将输入的字符串转换为坐标索引
        coordIndex = othelloPlayer_coord2index(str);

        // 尝试将输入转换为数字，表示移动编号
        moveNum = atoi(str);

        // 如果坐标索引有效且是合法移动
        if (coordIndex != -1 && find_move(legalMoves, coordIndex) != find_end(legalMoves)) {
            printf("\n");
            // 返回用户选择的移动
            return find_move(legalMoves, coordIndex);
        }
        // 如果输入既不是坐标也不是有效编号
        else if (moveNum < 1 || moveNum > size_moves(legalMoves)) {
            printf("\tInvalid input. Please try again.\n\n");
        }
        // 如果输入是有效编号但不在合法移动列表中
        else {
            validInput = true;
            printf("\n");
        }
    } while (!validInput);

    // 遍历合法移动列表，找到用户选择的移动编号对应的项
    int i = 0;
    MovePair_t *keyval = NULL;
    MovePair_t *tmp = NULL;
    
    // HASH_ITER(hh, legalMoves, keyval, tmp) {
    LL_FOREACH_SAFE(legalMoves, keyval, tmp){
        if (keyval != NULL) {
            move = keyval;
            i++;
            // 如果计数器等于用户输入的编号
            if (i == moveNum) {
                break;
            }
        }
    }

    // 返回用户选择的移动
    return move;
}

/**
 * @brief 将坐标字符串转换为棋盘上的索引
 *
 * 将一个表示棋盘坐标的字符串转换为棋盘上的索引。棋盘大小为8x8，坐标以字符串形式给出，如"A1"、"B2"等。
 *
 * @param coord 坐标字符串，格式为'列号行号'，列号为大写或小写字母（A-H），行号为数字（1-8）
 * @return 索引值，如果坐标无效，则返回-1
 */
// int othelloPlayer::coord2index(std::string coord) {
// int othelloPlayer::coord2index(char *coord) {

//     if(NULL == coord)
//     {
//         printf("[coord2index] NULL == coord");
//         return -1;
//     }
//     // 检查坐标字符串长度是否为2
//     // if (coord.length() != 2) {
//     if (strlen(coord) != 2) {
//         return -1;
//     }

//     int index = 0;
//     // 根据坐标的第一个字符确定行索引
//     // switch(coord.at(0)) {
//     switch(coord[0]) {
//         case 'A':
//         case 'a':
//             // A或a代表第一行
//             index = 0;
//             break;
//         case 'B':
//         case 'b':
//             // B或b代表第二行
//             index = 1;
//             break;
//         case 'C':
//         case 'c':
//             // C或c代表第三行
//             index = 2;
//             break;
//         case 'D':
//         case 'd':
//             // D或d代表第四行
//             index = 3;
//             break;
//         case 'E':
//         case 'e':
//             // E或e代表第五行
//             index = 4;
//             break;
//         case 'F':
//         case 'f':
//             // F或f代表第六行
//             index = 5;
//             break;
//         case 'G':
//         case 'g':
//             // G或g代表第七行
//             index = 6;
//             break;
//         case 'H':
//         case 'h':
//             // H或h代表第八行
//             index = 7;
//             break;
//         default:
//             // 如果第一个字符不在A-H之间，则返回-1
//             return -1;
//     }

//     // 根据坐标的第二个字符确定列索引
//     // switch(coord.at(1)) {
//     switch(coord[1]) {
//         case '1':
//             // 1代表第一列
//             index += 0;
//             break;
//         case '2':
//             // 2代表第二列
//             index += 8;
//             break;
//         case '3':
//             // 3代表第三列
//             index += 16;
//             break;
//         case '4':
//             // 4代表第四列
//             index += 24;
//             break;
//         case '5':
//             // 5代表第五列
//             index += 32;
//             break;
//         case '6':
//             // 6代表第六列
//             index += 40;
//             break;
//         case '7':
//             // 7代表第七列
//             index += 48;
//             break;
//         case '8':
//             // 8代表第八列
//             index += 56;
//             break;
//         default:
//             // 如果第二个字符不在1-8之间，则返回-1
//             return -1;
//     }

//     return index;
// }

int othelloPlayer_coord2index(char *coord) {
    if (coord == NULL || strlen(coord) != 2) {
        return -1;
    }

    int index = 0;
    switch (coord[0]) {
        case 'A': case 'a': index = 0; break;
        case 'B': case 'b': index = 1; break;
        case 'C': case 'c': index = 2; break;
        case 'D': case 'd': index = 3; break;
        case 'E': case 'e': index = 4; break;
        case 'F': case 'f': index = 5; break;
        case 'G': case 'g': index = 6; break;
        case 'H': case 'h': index = 7; break;
        default: return -1;
    }

    switch (coord[1]) {
        case '1': index += 0; break;
        case '2': index += 8; break;
        case '3': index += 16; break;
        case '4': index += 24; break;
        case '5': index += 32; break;
        case '6': index += 40; break;
        case '7': index += 48; break;
        case '8': index += 56; break;
        default: return -1;
    }

    return index;
}


// Driver for the AI algorithm
/**
 * @brief 电脑进行一步棋
 *
 * 根据棋盘、合法走法、是否过路以及走法历史记录，电脑进行一步棋。
 *
 * @param board 棋盘对象
 * @param legalMoves 当前棋盘状态下所有合法的走法
 * @param pass 是否过路
 * @param moveHistory 走法历史记录
 * @return 返回电脑走法的行列索引对
 */

MovePair_t *othelloPlayer_computerMove(othelloPlayer *player,othelloBoard *board,MovePair_t *legalMoves,bool pass,char *moveHistory) {
    struct timespec startTime;
    clock_gettime(CLOCK_REALTIME, &startTime);

    MovePair_t *move = NULL;
    MovePair_t *bestMove = NULL;

    if (move_hash_empty(legalMoves)) {
        printf("No legal moves!\n\tComputer passes.\n");
        pass = true;
        return bestMove;
    } else if (size_moves(legalMoves) == 1) {
        printf("Only one legal move!\n\tComputer takes only legal move.\n");
        bestMove = find_begin(legalMoves);
    } else {
        int maxDepth = 64 - board->discsOnBoard;
        if (maxDepth < 10) {
            printf("Searching remainder of game tree...\n");
            bestMove = othelloPlayer_depthLimitedAlphaBeta(player, board, maxDepth, startTime, board->timeLimit);
        } else {
            for (int depthLimit = 1; depthLimit <= maxDepth; depthLimit++) {
                move = othelloPlayer_depthLimitedAlphaBeta(player, board, depthLimit, startTime, board->timeLimit);
                if (move == NULL || move->position == -1) {
                    printf("\tSearch aborted.\n");
                    break;
                } else {
                    bestMove = move;
                }
                if (othelloPlayer_stopTimer(startTime) > 0.5 * board->timeLimit) {
                    break;
                }
            }
        }
    }

    if (bestMove != NULL) {
        int colNum = 0, rowNum = 0;
        othelloBoard_index2coord(bestMove->position, &colNum, &rowNum);
        printf("\tComputer takes: %c%d\n\n", 'A' + colNum, rowNum + 1);
    }

    return bestMove;
}


// Returns time point
/**
 * @brief 开始计时
 *
 * 使用系统时钟获取当前时间作为计时起点，并返回这个时间点。
 *
 * @return std::chrono::time_point<std::chrono::system_clock> 计时起点的时间点
 */
// std::chrono::time_point<std::chrono::system_clock> othelloPlayer::startTimer() {
//     // 返回当前系统时钟的时间点
//     return std::chrono::system_clock::now();
//     // startTimer 函数结束
// }

struct timespec othelloPlayer_startTimer() {
    struct timespec startTime;
    // 使用 CLOCK_REALTIME 或者 CLOCK_MONOTONIC 获取当前时间
    clock_gettime(CLOCK_REALTIME, &startTime);
    return startTime;
}

// Returns time elapsed in seconds
/**
 * @brief 停止计时器并返回经过的时间（秒）
 *
 * 根据传入的开始时间，计算从开始时间到当前时间的总经过时间（秒），并返回。
 *
 * @param startTime 开始时间
 * @return 返回从startTime到当前时间的经过时间（秒）
 */


float othelloPlayer_stopTimer(struct timespec startTime) {
    // 获取当前时间作为结束时间
    struct timespec endTime;
    clock_gettime(CLOCK_REALTIME, &endTime);

    // 计算时间差并转换为秒
    float elapsedSeconds = (endTime.tv_sec - startTime.tv_sec) +
                           (endTime.tv_nsec - startTime.tv_nsec) / 1e9;

    // 返回经过的时间（秒）
    return elapsedSeconds;
}

// Performs depth-limited minimax search with alpha-beta pruning
// Implemented iteratively to avoid recursion overhead
// Returns move for square -1 if time runs out
// TODO implement killer move heuristic
/**
 * @brief 在给定棋盘和时间限制下，使用深度限制的α-β剪枝算法搜索最佳走法
 *
 * 使用深度限制的α-β剪枝算法在给定棋盘和时间限制下搜索最佳走法。算法通过递归搜索可能的走法，并评估每种走法的启发式值，最终返回最佳走法。
 *
 * @param board 当前棋盘状态
 * @param depthLimit 搜索的最大深度
 * @param startTime 开始搜索的时间点
 * @param timeLimit 搜索的最大时间限制（秒）
 * @return 返回最佳走法，由位置坐标和可能的后续走法列表组成
 */


MovePair_t *othelloPlayer_depthLimitedAlphaBeta(othelloPlayer *player,othelloBoard *board, int depthLimit,struct timespec startTime, float timeLimit) {
    // 初始化根节点
    player->nodeStack[0].isMaxNode = 1;
    player->nodeStack[0].alpha = INT_MIN;
    player->nodeStack[0].beta = INT_MAX;
    player->nodeStack[0].score = INT_MIN;
    player->nodeStack[0].board = board;
    player->nodeStack[0].moveIterator = find_begin(player->nodeStack[0].board->moves);
    player->nodeStack[0].prevIterator = player->nodeStack[0].moveIterator;
    player->nodeStack[0].lastMove = find_end(player->nodeStack[0].board->moves);

    int depth = 0;
    int leafScore = 0;
    MovePair_t *bestMove = find_begin(player->nodeStack[0].board->moves);

    while (1) {
        // 如果已评估完所有子节点
        if (player->nodeStack[depth].moveIterator == player->nodeStack[depth].lastMove) {
            if (depth-- == 0) {
                if (player->nodeStack[1].score > player->nodeStack[0].score
                        || (player->nodeStack[1].score == player->nodeStack[0].score
                            && rand() % 2 == 0)) {
                    player->nodeStack[0].score = player->nodeStack[1].score;
                    bestMove = player->nodeStack[0].prevIterator;
                }

                if (player->nodeStack[0].score > player->nodeStack[0].alpha) {
                    player->nodeStack[0].alpha = player->nodeStack[0].score;
                }

                break;
            }

            if (player->nodeStack[depth].isMaxNode) {
                if (player->nodeStack[depth+1].score > player->nodeStack[depth].score
                        || (player->nodeStack[depth+1].score == player->nodeStack[depth].score
                            && rand() % 2 == 0)) {
                    player->nodeStack[depth].score = player->nodeStack[depth+1].score;
                    if (depth == 0) {
                        bestMove = player->nodeStack[0].prevIterator;
                    }
                }

                if (player->nodeStack[depth].score > player->nodeStack[depth].alpha) {
                    player->nodeStack[depth].alpha = player->nodeStack[depth].score;
                }
            } else {
                if (player->nodeStack[depth+1].score < player->nodeStack[depth].score) {
                    player->nodeStack[depth].score = player->nodeStack[depth+1].score;
                }

                if (player->nodeStack[depth].score < player->nodeStack[depth].beta) {
                    player->nodeStack[depth].beta = player->nodeStack[depth].score;
                }
            }
        }
        // 如果可以剪枝
        else if (player->nodeStack[depth].beta <= player->nodeStack[depth].alpha) {
            if (depth-- == 0) {
                if (player->nodeStack[1].score > player->nodeStack[0].score
                    || (player->nodeStack[1].score == player->nodeStack[0].score
                        && rand() % 2 == 0)) {
                    player->nodeStack[0].score = player->nodeStack[1].score;
                    bestMove = player->nodeStack[0].prevIterator;
                }

                if (player->nodeStack[0].score > player->nodeStack[0].alpha) {
                    player->nodeStack[0].alpha = player->nodeStack[0].score;
                }

                break;
            }

            if (player->nodeStack[depth].isMaxNode) {
                if (player->nodeStack[depth+1].score > player->nodeStack[depth].score
                    || (player->nodeStack[depth+1].score == player->nodeStack[depth].score
                        && rand() % 2 == 0)) {
                    player->nodeStack[depth].score = player->nodeStack[depth+1].score - 1;
                    if (depth == 0) {
                        bestMove = player->nodeStack[0].prevIterator;
                    }
                }

                if (player->nodeStack[depth].score > player->nodeStack[depth].alpha) {
                    player->nodeStack[depth].alpha = player->nodeStack[depth].score;
                }

            } else {
                if (player->nodeStack[depth+1].score < player->nodeStack[depth].score) {
                    player->nodeStack[depth].score = player->nodeStack[depth+1].score + 1;
                }

                if (player->nodeStack[depth].score < player->nodeStack[depth].beta) {
                    player->nodeStack[depth].beta = player->nodeStack[depth].score;
                }
            }
        }
        else {
            // 生成下一个节点，增加迭代器
            player->nodeStack[depth+1].board = player->nodeStack[depth].board;
            othelloBoard_updateBoard(player->nodeStack[depth+1].board,
                                    (player->nodeStack[depth].isMaxNode ? player->color : -player->color),
                                    player->nodeStack[depth].moveIterator);
            player->nodeStack[depth].prevIterator = player->nodeStack[depth].moveIterator;
            player->nodeStack[depth].moveIterator = find_next(player->nodeStack[depth].board->moves, player->nodeStack[depth].prevIterator);

            // 如果下一个深度未达到深度限制
            if (depth + 1 < depthLimit) {
                depth++;

                player->nodeStack[depth].isMaxNode = !player->nodeStack[depth-1].isMaxNode;
                player->nodeStack[depth].score = (player->nodeStack[depth].isMaxNode ? INT_MIN : INT_MAX);
                player->nodeStack[depth].alpha = player->nodeStack[depth-1].alpha;
                player->nodeStack[depth].beta = player->nodeStack[depth-1].beta;
                othelloBoard_findLegalMoves(player->nodeStack[depth].board,
                                        (player->nodeStack[depth].isMaxNode ? player->color : -player->color),
                                        &player->nodeStack[depth].board->moves);
                player->nodeStack[depth].moveIterator = find_begin(player->nodeStack[depth].board->moves);
                player->nodeStack[depth].prevIterator = player->nodeStack[depth].moveIterator;
                player->nodeStack[depth].lastMove = find_end(player->nodeStack[depth].board->moves);
            }
            else {
                leafScore = othelloHeuristic_evaluate(player->nodeStack[depth+1].board, player->color);
                

                if (player->nodeStack[depth].isMaxNode) {
                    if (leafScore > player->nodeStack[depth].score) {
                        player->nodeStack[depth].score = leafScore;
                        if (depth == 0) {
                            bestMove = player->nodeStack[0].prevIterator;
                        }
                    }

                    if (player->nodeStack[depth].score > player->nodeStack[depth].alpha) {
                        player->nodeStack[depth].alpha = player->nodeStack[depth].score;
                    }
                } else {
                    if (leafScore < player->nodeStack[depth].score) {
                        player->nodeStack[depth].score = leafScore;
                    }

                    if (player->nodeStack[depth].score < player->nodeStack[depth].beta) {
                        player->nodeStack[depth].beta = player->nodeStack[depth].score;
                    }
                }
            }
        }

        // 如果时间即将耗尽，则返回失败标志
        struct timespec currentTime;
        clock_gettime(CLOCK_REALTIME, &currentTime);

        // 计算从 startTime 到 currentTime 的经过时间（秒）
        float elapsedTime = (currentTime.tv_sec - startTime.tv_sec) +
                            (currentTime.tv_nsec - startTime.tv_nsec) / 1e9;

        // 如果时间即将耗尽，则返回失败标志
        if (elapsedTime > 0.998 * timeLimit) {
            MovePair_t *move = (MovePair_t *)malloc(sizeof(MovePair_t));
            if (move == NULL) {
                printf("malloc fail\n");
                return NULL;
            }
            move->position = -1;
            return move;
        }
    }

    return bestMove;
}
