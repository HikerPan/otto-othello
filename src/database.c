#include "database.h"

// othelloDatabase::othelloDatabase() {
//     this->loadOpenings();
// }

// 初始化 othelloDatabase 结构体
void othelloDatabase_init(othelloDatabase *db) {
    db->openingBook = NULL;
}

// 加载开局库
void othelloDatabase_loadOpenings(othelloDatabase *db) {
    FILE *file = fopen("openings.txt", "r");
    if (file == NULL) {
        printf("Failed to open openings file\n");
        return;
    }

    char key[64];
    int value;
    while (fscanf(file, "%63s %d", key, &value) != EOF) {
        opening_t *entry = (opening_t *)malloc(sizeof(opening_t));
        if (entry == NULL) {
            printf("Memory allocation failed\n");
            fclose(file);
            return;
        }
        strncpy(entry->key, key, sizeof(entry->key) - 1);
        entry->key[sizeof(entry->key) - 1] = '\0'; // 确保字符串以空字符结尾
        entry->value = value;

        HASH_ADD_STR(db->openingBook, key, entry);
    }

    fclose(file);
}

// 清空开局库
void othelloDatabase_clear(othelloDatabase *db) {
    opening_t *entry, *tmp;
    HASH_ITER(hh, db->openingBook, entry, tmp) {
        HASH_DEL(db->openingBook, entry);
        free(entry);
    }
}

// /**
//  * @brief 加载开局库
//  *
//  * 从指定文件中加载开局数据到开局库中。
//  *
//  * 文件格式应为每行包含两部分，第一部分为历史走法，第二部分为对应的下一步走法（以整数形式表示）。
//  * 每两步走法占一行，以换行符分隔。
//  *
//  * @note 该函数将文件 "../lib/openings.txt" 中的数据读入到成员变量 openingBook 中。
//  */
// void othelloDatabase::loadOpenings() {
//     // 打开文件
//     std::ifstream csv("../lib/openings.txt");

//     // 定义字符串变量用于存储过去的走法和下一个走法
//     std::string pastMoves;
//     std::string nextMove;
//     // 定义整型变量用于存储解析后的下一个走法
//     int nextMoveInt;

//     // 循环读取文件中的每一行
//     while (getline(csv, pastMoves)) {
//         // 读取下一个走法
//         getline(csv, nextMove);
//         // 将字符串类型的下一个走法转换为整型
//         nextMoveInt = std::stoi(nextMove);

//         // 输出过去的走法（此行代码被注释掉）
//         //std::cout << pastMoves << std::endl;
//         // 输出解析后的下一个走法（此行代码被注释掉）
//         //std::cout << nextMoveInt << std::endl;

//         // 将过去的走法和下一个走法添加到开局数据库中
//         // {pastMoves, nextMoveInt} 表示一对键值对，其中键为过去的走法，值为下一个走法
//         openingBook.insert({pastMoves, nextMoveInt});
//     }
// }
