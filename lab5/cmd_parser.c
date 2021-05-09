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
        printf("Неполная команда. Введите имя файла.\n");
        return;
    }
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Ошибка! Невозможно открыть файл!\n");
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
        printf("Слово \"%s\" успешно добавлено!\n", word);
    else
        printf("Слово \"%s\" уже содержится в дереве\n", word);
}

void delete_handler(int delete, char *word) {
    // обработка результата trie_delete_word
    switch (delete) {
        case 0:
            printf("В дереве нет слова \"%s\"\n", word);
            break;
        case 1:
            printf("Слово \"%s\" было удалено\n", word);
            break;
        default:
            printf("Дерево пусто!\n");
    }
}

void search_handler(int search, char *word) {
    // обработка результата trie_search
    if (search)
        printf("слово \"%s\" найдено\n", word);
    else
        printf("Слово \"%s\" не найдено\n", word);
}

int parse_command(char *line, Trie *trie) {
    // парсинг командной строки
    char *command = calloc(12, sizeof(char));
    int i = 0;
    if (line[i] != '-') {
        printf("Неверная комманда\nВведите -h, чтобы посмотреть доступные команды\n");
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
            printf("Неполная команда. Необходимо ввести вставляемое слово\n");
            free(command);
            return 1;
        }
        add_handler(trie_add_word(trie, word), word);
        free(command);
        return 1;
    }
    if (strcmp(command, "-clear") == 0) {
        printf("Удаление всех слов из префиксоного дерева...\n");
        clear_trie(trie);
        printf("Префиксное дерево очищено!\n");
        free(command);
        return 1;
    }
    if (strcmp(command, "-delete") == 0) {
        i++;
        char *word = readArgument(i, line);
        if (word[0] == '\0') {
            printf("Неполная команда. Необходимо ввести удаляемое слово\n");
            free(command);
            return 1;
        }
        delete_handler(trie_delete_word(trie, word), word);
        free(command);
        return 1;
    }
    if (strcmp(command, "-exit") == 0) {
        printf("Удаление всех слов из префиксоного дерева...\n");
        clear_trie(trie);
        printf("Префиксное дерево очищено!\nВыход.\n");
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
        printf("Trie implementation by vitekkor\nSupports digits and only English letters\nПоддерживаемые команды:\n");
        printf("-add word - добавить слово\n");
        printf("-clear - удалить все слова в дереве\n");
        printf("-delete word - удалить слово\n");
        printf("-exit - выход\n");
        printf("-file-add filename - добавить слова из файла\n");
        printf("-file-delete filename - удалить слова из файла из дерева\n");
        printf("-file-search filename - поиск слов из файла в дереве\n");
        printf("-h - показать это сообщение\n");
        printf("-print - вывод слов, содержащихся в дереве\n");
        printf("-search word - поиск слова в дереве\n");
        free(command);
        return 1;
    }
    if (strcmp(command, "-print") == 0) {
        printf("Trie:\n");
        trie_print(trie);
        free(command);
        return 1;
    }
    if (strcmp(command, "-search") == 0) {
        i++;
        char *word = readArgument(i, line);
        if (word[0] == '\0') {
            printf("Неполная команда. Введите искомое слово\n");
            free(command);
            return 1;
        }
        printf("Поиск слова \"%s\": ", word);
        search_handler(trie_search(trie, word), word);
        free(command);
        return 1;
    }
    printf("Неизвестная команда\n");
    return 1;
}