#ifndef LOW_LEVEL_PROGRAMMING_CMD_PARSER_H
#define LOW_LEVEL_PROGRAMMING_CMD_PARSER_H

#include "trie.h"

char *readArgument(int startIndex, const char *line);

void from_file(int (*action)(Trie *, char *), void (*action_handler)(int, char *), Trie *trie, char *line, int i);

void add_handler(int add, char *word);

void delete_handler(int delete, char *word);

void search_handler(int search, char *word);

int parse_command(char *line, Trie *trie);

#endif
