#include "board.h"
#include "uthash.h"
#include "moves_hashmap.h"
// Constructor
/**
 * @brief 构造函数
 *
 * 初始化棋盘，将棋盘上的每个位置初始化为0。
 * positions是一个大小为64的数组，每个元素初始化为0。
 */
void othelloBoard_init(othelloBoard *board) {
    memset(board->positions, 0, sizeof(int) * OTHELLO_BOARD_SIZE);
    board->discsOnBoard = 4;
    board->timeLimit = 0.0;
    board->passes[0] = false;
    board->passes[1] = false;
    board->moves = NULL;
}

// Display board: color is 1 for black, -1 for white
/**
 * @brief 显示棋盘
 *
 * 在控制台上显示棋盘的当前状态，包括黑棋、白棋、可选位置和分数。
 *
 * @param color 玩家颜色，1表示黑棋，-1表示白棋
 */
void othelloBoard_displayBoard(othelloBoard *board, int color) {
    // 打印棋盘标题
    printf("    A B C D E F G H\n");

    int row = 1;
    for (int i = 0; i < 64; i += 8) {
        printf(" %d \033[48;5;34m\033[38;5;232m \033[0m", row++);

        for (int j = i; j < i + 8; j++) {
            if (board->positions[j] == 1) {
                printf("\033[48;5;34m\033[38;5;232m\u2022 \033[0m");
            } else if (board->positions[j] == -1) {
                printf("\033[48;5;34m\033[38;5;15m\u2022 \033[0m");
            } else if (NULL != find_move(board->moves, j) && color == 1) {
                printf("\033[48;5;34m\033[38;5;232m\u2613 \033[0m");
            } else if (NULL != find_move(board->moves, j) && color == -1) {
                printf("\033[48;5;34m\033[38;5;15m\u2613 \033[0m");
            } else {
                printf("\033[48;5;34m\033[38;5;232m\u00B7 \033[0m");
            }
        }

        if (i == 24) {
            printf("\t\tBlack: %d", board_count(board->positions, 1, 64, 1));
        } else if (i == 32) {
            printf("\t\tWhite: %d", board_count(board->positions, 1, 64, -1));
        }

        printf("\n");
    }
    printf("\n");
}

// Display legal moves for player
/**
 * @brief 显示所有合法的移动
 *
 * 遍历所有合法的移动，并显示它们的编号、位置以及它们将翻转的棋子列表。
 *
 * @note 本函数没有返回值。
 */
void othelloBoard_displayLegalMoves(othelloBoard *board) {
    char colCoord[] = "ABCDEFGH";
    char rowCoord[] = "12345678";
    int colNum = 0, rowNum = 0;
    int moveNum = 1;
    MovePair_t *keyval = NULL;
    MovePair_t *tmp = NULL;

    printf("Legal moves:\n");

    // HASH_ITER(hh, board->moves, keyval, tmp) {
    LL_FOREACH_SAFE(board->moves, keyval, tmp){
        othelloBoard_index2coord(keyval->position, &colNum, &rowNum);
        printf("\t%d\t%c%c will flip: ", moveNum++, colCoord[colNum], rowCoord[rowNum]);

        IntListNode_t *node;
        LL_FOREACH(keyval->flip_list, node) {
            othelloBoard_index2coord(node->flip_position, &colNum, &rowNum);
            printf("%c%c ", colCoord[colNum], rowCoord[rowNum]);
        }

        printf("\n");
    }
    printf("\n");
}


// Finds all legal moves, writing to a reference to a hash table with
// legal moves as keys, and a list of all discs to be flipped as values.
/**
 * @brief 查找当前玩家可以下的合法走法
 *
 * 为给定颜色的玩家查找并返回所有可能的合法走法。
 *
 * @param color 当前玩家的颜色
 * @param pMoves 用于存储合法走法的指针，键为棋盘位置，值为可能的走法列表
 */
void othelloBoard_findLegalMoves(othelloBoard *board, int color, MovePair_t **pMoves) {
    // 清除上一手棋的合法走法
    // clear_moves(pMoves);

    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++) {
        if (board->positions[i] == color) {
            // 检查行
            othelloBoard_findLegalMoveInDirection(board, i, color, -1, pMoves);
            othelloBoard_findLegalMoveInDirection(board, i, color, 1, pMoves);

            // 检查列
            othelloBoard_findLegalMoveInDirection(board, i, color, -8, pMoves);
            othelloBoard_findLegalMoveInDirection(board, i, color, 8, pMoves);

            // 检查对角线
            othelloBoard_findLegalMoveInDirection(board, i, color, -9, pMoves);
            othelloBoard_findLegalMoveInDirection(board, i, color, 9, pMoves);
            othelloBoard_findLegalMoveInDirection(board, i, color, -7, pMoves);
            othelloBoard_findLegalMoveInDirection(board, i, color, 7, pMoves);
        }
    }
}


// Helper function to find a legal move given a disc, its color and a direction.
// Writes the legal move and a list of all discs to be flipped as a pair to the
// reference to a hash table.
/**
 * @brief 在给定方向上查找合法的移动
 *
 * 在指定的方向上查找所有合法的移动，并记录这些移动可以翻转的棋子列表。
 *
 * @param disc 当前棋子的索引
 * @param color 当前棋子的颜色（非零值，正数代表白方，负数代表黑方）
 * @param direction 移动方向（正数或负数，表示向上、向下、向左或向右移动）
 * @param pMoves 指向存储合法移动和翻转棋子列表的哈希表的指针
 */
void othelloBoard_findLegalMoveInDirection(othelloBoard *board, int disc, int color, int direction, MovePair_t **pMoves) {
    // 初始化一个合法的移动和翻转的棋子列表
    MovePair_t *legalMove = NULL;
    IntListNode_t *flippedDiscs = NULL;

    int currentSquare = 0;
    int row1 = 0, col1 = 0, row2 = 0, col2 = 0;

    // 沿给定方向遍历棋格，防止棋盘边缘的棋格越界
    for (int i = disc + direction; i < 64 && i > -1; i += direction) {
        // 防止棋盘行和列的边界超出
        othelloBoard_index2coord(i - direction, &col1, &row1);
        othelloBoard_index2coord(i, &col2, &row2);
        if (abs(col1 - col2) > 1 || abs(row1 - row2) > 1) {
            break;
        }

        // 沿给定方向移动，记录相反颜色的棋子
        currentSquare = board->positions[i];
        if (currentSquare == color         // 找到跟自己同色的棋子，则结束循环
            || (currentSquare == 0 && flip_list_empty(flippedDiscs))) {  // 找到空位，且翻转列表为空，则结束循环
            break;
        } else if (currentSquare == -color) {   //找到对手棋子，则放入翻转列表
            list_push_front(&flippedDiscs, i);
            continue;
        } else if (currentSquare == 0 && !flip_list_empty(flippedDiscs)) {    //遇到空位，且翻转列表不为空
            MovePair_t *it = find_move(*pMoves, i);

            // if (it != NULL && it != find_end(pMoves)) {
            if (it != NULL) {
                merge_flip_lists(&it->flip_list, flippedDiscs);
            } else {
                legalMove = (MovePair_t *)malloc(sizeof(MovePair_t));
                if (legalMove == NULL) {
                    printf("malloc failed\n");
                    return;
                }
                // printf("[findLegalMoveInDirection] malloc move node at [ 0x%x ]\n",legalMove);
                legalMove->position = i;
                legalMove->flip_list = flippedDiscs;
                insert_moves(pMoves, legalMove);
            }

            break;
        }
    }
}

// Update positions after a move
/**
 * @brief 更新棋盘状态
 *
 * 根据输入的移动信息更新棋盘状态。
 *
 * @param color 当前玩家颜色
 * @param move 移动信息，包含移动的位置和翻转的棋子列表
 *             - move.first 表示移动的位置
 *             - move.second 表示翻转的棋子列表
 */
void othelloBoard_updateBoard(othelloBoard *board, int color, MovePair_t *move) {
    int square = -1;
    IntListNode_t *flippedDiscs = NULL;

    // if(NULL == board || NULL == move)
    // {
    //     printf("error pointer.\n");
    //     return;
    // }

    // 获取移动的位置
    square = move->position;
    flippedDiscs = move->flip_list;

    if (flippedDiscs == NULL) {
        // printf("flippedDiscs is NULL\n");
        return;
    }

    // 将移动位置设置为当前玩家颜色
    board->positions[square] = color;

    // 遍历翻转的棋子列表并将棋子颜色设置为当前玩家颜色
    IntListNode_t *disc = NULL;
    IntListNode_t *tmp = NULL;
    LL_FOREACH_SAFE(flippedDiscs, disc, tmp) {
        if (disc != NULL) {
            // 将翻转的棋子位置设置为当前玩家颜色
            board->positions[disc->flip_position] = color;
        }
    }
}


// Checks if game is a terminal state
/**
 * @brief 判断棋盘是否到达终局状态
 *
 * 检查两位玩家是否都放弃了当前回合，如果都放弃了，则棋盘到达终局状态
 *
 * @return 如果棋盘到达终局状态，返回true；否则返回false
 */
bool othelloBoard_terminalState(othelloBoard *board) {
    // 如果两位玩家都选择了放弃，则游戏结束
    if (board->passes[0] && board->passes[1]) {
        // 两位玩家都选择了放弃，返回 true 表示游戏结束
        return true;
    }
    // 否则，游戏尚未结束，返回 false
    return false;
}


// Helper function to convert board square index to coordinates
/**
 * @brief 将棋盘上的索引转换为行列号
 *
 * 将给定的索引值转换为对应的列号和行号。
 *
 * @param index 给定的索引值，索引值范围从0到63。
 * @param colNum 用于存储转换后的列号，范围从0到7。
 * @param rowNum 用于存储转换后的行号，范围从0到7。
 */
void othelloBoard_index2coord(int index, int *colNum, int *rowNum) {
    // 将 index 除以 8 取余数得到列号
    *colNum = index % 8;
    // 将 index 除以 8 取整得到行号
    *rowNum = index / 8;
}


int board_count(int *p, size_t start, size_t end, int target)
{
    int count = 0;
    for(size_t i = start-1;i<end;i++)
    {
        if(p[i] == target)
            count++;
    }

    return count;
}

int board_accumulate(int *p, size_t start, size_t end, int init_value)
{
    int count = init_value;
    for(size_t i = start-1;i<end;i++)
    {
        count+=p[i];
    }

    return count;
}


