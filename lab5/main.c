#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "cmd_parser.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Trie *trie = create_Trie();
    char *line;
    line = malloc(MAX_WORD_LENGTH * sizeof(char));
    parse_command("-h\n", NULL);
    while (1) {
        line = fgets(line, MAX_WORD_LENGTH, stdin);
        if (!parse_command(line, trie)) return 0;
    }
}