#include "cmd_parser.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

char *readArgument(int startIndex, const char *line) {
    //чтение аргумента командной строки
    if (startIndex > (int) strlen(line)) return NULL;
    char *argument;
    argument = calloc((MAX_WORD_LENGTH - startIndex), sizeof(char));
    int j = 0;
    for (int i = startIndex; line[i] != '\n' && line[i] != ' '; i++) {
        argument[j] = line[i];
        j++;
    }
    return argument;
}

void from_file(int (*action)(Trie *, char *), void (*action_handler)(int, char *), Trie *trie, char *line, int i) {
    //выполнение дейстивй с файлом
    char *fileName = readArgument(i, line);
    if (fileName[0] == '\0') {
        wprintf(L"Incomplete command. Enter a name for the file.\n\n");
        return;
    }
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        wprintf(L"Error! Unable to open file!\n\n");
        return;
    }
    char word[MAX_WORD_LENGTH];
    // читаем файл
    while (fgets(word, MAX_WORD_LENGTH, file) != NULL) {
        word[strlen(word) - 1] = '\0';
        //выполняем действие. action handler - метод, обрабатывающий результат action
        action_handler(action(trie, word), word);
    }
}

void add_handler(int add, char *word) {
    // обработка результата trie_add_word
    if (add)
        wprintf(L"The word \"%S\" was added successfully!\n\n", (wchar_t *) word);
    else
        wprintf(L"The word \"%S\" is already in the trie\n\n", word);
}

void delete_handler(int delete, char *word) {
    // обработка результата trie_delete_word
    switch (delete) {
        case 0:
            wprintf(L"The trie does not contain a word \"%S\"\n\n", word);
            break;
        case 1:
            wprintf(L"The word \"%S\" has been deleted\n\n", word);
            break;
        default:
            wprintf(L"Trie is empty!\n\n");
    }
}

void search_handler(int search, char *word) {
    // обработка результата trie_search
    if (search)
        wprintf(L"The word \"%S\" found\n\n", word);
    else
        wprintf(L"The word \"%S\" not found\n\n", word);
}

int parse_command(char *line, Trie *trie) {
    // парсинг командной строки
    char *command = calloc(12, sizeof(char));
    int i = 0;
    if (line[i] != '-') {
        wprintf(L"Invalid command\nEnter -h to see the available commands\n\n");
        return 1;
    }
    int j = 0;
    // считываем команду
    for (; line[i] != '\n' && line[i] != ' '; i++) {
        command[j] = line[i];
        j++;
    }
    // определяем, что за команда и выполняем соответсвующие действия
    if (strcmp(command, "-add") == 0) {
        i++;
        char *word = readArgument(i, line);
        if (word[0] == '\0') {
            wprintf(L"Incomplete command. You must enter the WORD to be inserted\n\n");
            free(command);
            return 1;
        }
        add_handler(trie_add_word(trie, word), word);
        free(command);
        return 1;
    }
    if (strcmp(command, "-clear") == 0) {
        wprintf(L"Removing all words from the trie ...\n");
        clear_trie(trie);
        wprintf(L"The trie has been cleared!\n\n");
        free(command);
        return 1;
    }
    if (strcmp(command, "-delete") == 0) {
        i++;
        char *word = readArgument(i, line);
        if (word[0] == '\0') {
            wprintf(L"Incomplete command. You must enter the WORD to be deleted\n\n");
            free(command);
            return 1;
        }
        delete_handler(trie_delete_word(trie, word), word);
        free(command);
        return 1;
    }
    if (strcmp(command, "-exit") == 0) {
        wprintf(L"Removing all words from the trie ...\n\n");
        clear_trie(trie);
        wprintf(L"The trie has been cleared!\nExit.\n");
        free(command);
        free(trie);
        return 0;
    }
    if (strcmp(command, "-file-add") == 0) {
        i++;
        from_file(&trie_add_word, &add_handler, trie, line, i);
        free(command);
        return 1;
    }
    if (strcmp(command, "-file-delete") == 0) {
        i++;
        from_file(&trie_delete_word, &delete_handler, trie, line, i);
        free(command);
        return 1;
    }
    if (strcmp(command, "-file-search") == 0) {
        i++;
        from_file(&trie_search, &search_handler, trie, line, i);
        free(command);
        return 1;
    }
    if (strcmp(command, "-h") == 0) {
        wprintf(L"Trie implementation by vitekkor\nSupports digits and only English letters\nSupported commands\n");
        wprintf(L"-add WORD - add word to trie\n");
        wprintf(L"-clear - delete all words in the trie\n");
        wprintf(L"-delete WORD - remove word from trie\n");
        wprintf(L"-exit - exit the program\n");
        wprintf(L"-file-add FILENAME - add all words from file\n");
        wprintf(L"-file-delete FILENAME - remove words contained in file from trie\n");
        wprintf(L"-file-search FILENAME - search for words from a file in a trie\n");
        wprintf(L"-h - show this message\n");
        wprintf(L"-print - print trie\n");
        wprintf(L"-search WORD - search for a WORD in the tree\n\n");
        free(command);
        return 1;
    }
    if (strcmp(command, "-print") == 0) {
        wprintf(L"Trie:\n");
        trie_print(trie);
        free(command);
        return 1;
    }
    if (strcmp(command, "-search") == 0) {
        i++;
        char *word = readArgument(i, line);
        if (word[0] == '\0') {
            wprintf(L"Incomplete command. Enter your search WORD\n\n");
            free(command);
            return 1;
        }
        wprintf(L"Searching a word \"%S\": ", (wchar_t *) word);
        search_handler(trie_search(trie, word), word);
        free(command);
        return 1;
    }
    wprintf(L"Unknown command\n\n");
    return 1;
}