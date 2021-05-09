cmake_minimum_required(VERSION 3.19)
project(low_level_programming C)

set(CMAKE_C_STANDARD 11)

add_executable(low_level_programming trie.c main.c trie.h cmd_parser.c cmd_parser.h tests/test.c tests/test.h)