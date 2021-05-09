#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TrieNode {
    TrieNode *children[N]; // дети
    int is_leaf; // этот узел - конец слова
};

struct Trie {
    TrieNode *root;
    int size;
};

Trie *create_Trie() {
    // создание дерева
    Trie *trie = (Trie *) malloc(sizeof(Trie));
    trie->root = make_trieNode();
    trie->size = 0;
    return trie;
}

Trie *clear_trie(Trie *trie) {
    trie->size = 0;
    free_trieNode(trie->root);
    trie->root = NULL;
    return trie;
}

TrieNode *make_trieNode() {
    // Создание узла
    TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
    for (int i = 0; i < N; i++)
        node->children[i] = NULL;
    node->is_leaf = 0;
    return node;
}


void free_trieNode(TrieNode *node) {
    //Освобождение памяти, занимаемой узлом
    for (int i = 0; i < N; i++) {
        if (node->children[i] != NULL) {
            // очищае рукурсивно
            free_trieNode(node->children[i]);
        } else {
            continue;
        }
    }
    free(node);
}

int get_char_position(char ch) {
    int ch_ = (int) tolower(ch);
    if (isdigit(ch)) return ch_ - 48;
    //Русские буквы не работают
    //if (ch_ < 0) return ch_ + 58;
    return (int) ch - 'a' + 10;
}

int trie_add_word(Trie *trie, char *word) {
    // вставка слова в дерево
    if (trie->root == NULL) trie->root = make_trieNode();
    TrieNode *temp = trie->root;

    for (int i = 0; word[i] != '\0'; i++) {
        int idx = get_char_position(word[i]); // получаем позицию для символа
        if (temp->children[idx] == NULL) {
            // если при попытке спуститься вниз по дереву для вставляемого слова
            // не оказывается узла, то создаём этот узел
            temp->children[idx] = make_trieNode(word[i]);
        }
        // если же узел есть, то просто спускаемся дальше вниз
        temp = temp->children[idx];
    }
    if (temp->is_leaf)
        return 0;
    else {
        trie->size++;
        temp->is_leaf = 1;// обозначаем, что это слово
    }
    return 1;
}

int trie_search(Trie *trie, char *word) {
    // Поиск слова в дереве: 1 - найдено, 0 - не найдено
    if (trie->root == NULL) return 0;
    TrieNode *temp = trie->root;

    for (int i = 0; word[i] != '\0'; i++) {
        // спускаемся вниз по дереву
        int position = get_char_position(word[i]);
        if (temp->children[position] == NULL) {
            // если при спуске не оказывается следующего узла, то значит слова нет
            return 0;
        }
        temp = temp->children[position];
    }
    if (temp != NULL && temp->is_leaf == 1) {
        // проверяем удался ли спуск и является ли узел концом слова
        return 1;
    }
    return 0;
}

int trie_delete_word(Trie *trie, char *word) {
    if (trie->size == 0) {
        return 2;
    }

    TrieNode *temp = trie->root;

    TrieNode *deleteFrom = NULL; // узел, который можно удалить, не повредив дерево

    for (int i = 0; word[i] != '\0'; i++) {
        //спускаемся вниз по дереву
        int position = get_char_position(word[i]);
        if (temp->children[position] == NULL) {
            // такого слова нет
            return 0;
        } else {
            // проверяем, можно ли удалить этот узел, не повредив дерево
            int j = 0;
            for (; j < N; j++) {
                if (j != i && temp->children[j] != NULL) break;
            }
            if (j != i && j == N && temp->children[j] == NULL) deleteFrom = temp;
            // спускаемся
            temp = temp->children[position];
        }
    }
    if (temp == NULL) {
        return 0;
    }
    if (!temp->is_leaf) {
        return 0;
    }

    temp->is_leaf = 0; // удаляем слово
    if (deleteFrom != NULL) free_trieNode(deleteFrom); // очищаем память, если возможно
    trie->size--;
    return 1;
}

char get_char(int position) {
    if (position < 10) return position + 48;
    return position + 'a' - 10;
}

void trie_print_(TrieNode *root, char *str, int level) {
    // Если узел является концом слова, то добавляем символ конца строки и выводим на экран
    if (root->is_leaf) {
        str[level] = '\0';
        printf("%s\n", str);
    }

    int i;
    // перебераем все символы
    for (i = 0; i < N; i++) {
        // если можно спуститься ниже, то добавляем символ в строку
        // и спускаемся на уровень ниже и  рекурсивно вызваем печать
        if (root->children[i] != NULL) {
            str[level] = get_char(i);
            trie_print_(root->children[i], str, level + 1);
        }
    }
}

void trie_print(Trie *trie) {
    if (trie->size == 0) {
        printf("Дерево пусто!\n");
        return;
    }
    char *str;
    int level = 0;
    str = (char *) malloc(MAX_WORD_LENGTH * sizeof(char));
    trie_print_(trie->root, str, level);
    free(str);
}