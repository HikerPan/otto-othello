cmake_minimum_required(VERSION 3.10)
project(OttoOthello C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# 包含项目头文件目录
include_directories(src)



# 查找 src 目录下的所有 .cpp 文件并存储在 SOURCES 变量中
# file(GLOB SOURCES "src/*.cpp")
# 指定源文件
set(SOURCES
    src/board.c
    src/database.c
    src/game.c
    src/heuristic.c
    src/othello.c
    src/player.c
    src/moves_hashmap.c
)

# 显示指定 moves_hashmap.c 文件为 C 文件
set_source_files_properties(src/moves_hashmap.c PROPERTIES LANGUAGE C)

# 创建可执行文件 OttoOthello 并链接源文件
add_executable(OttoOthello ${SOURCES})

# 设置编译选项
# -g 用于调试，-O0 禁用优化（方便调试），-Wall 启用警告
target_compile_options(OttoOthello PRIVATE -g -O0 -Wall)

target_include_directories(OttoOthello PRIVATE src)

# 自定义的 clean-all 目标，用于清除所有生成的文件
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning all generated files..."
)
