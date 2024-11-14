#ifndef DATABASE_H
#define DATABASE_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "uthash.h"
#include "board.h"

typedef struct {
    char key[64];    // 对应于原来的 `std::string`
    int value;
    UT_hash_handle hh; // UTHash 句柄
} opening_t;

typedef struct {
    opening_t *openingBook; // 开局库的哈希表
} othelloDatabase;

// 函数声明
void othelloDatabase_init(othelloDatabase *db);
void othelloDatabase_loadOpenings(othelloDatabase *db);

#ifdef __cplusplus
}
#endif

#endif // DATABASE_H
