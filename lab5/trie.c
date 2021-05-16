#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct TrieNode {
    char data; // символ
    TrieNode *children[N_CHARACTERS]; // дети
    int is_leaf; // этот узел - конец слова
};

struct Trie {
    TrieNode *root;
    int size;
};

Trie *create_Trie() {
    // создание дерева
    Trie *trie = (Trie *) malloc(sizeof(Trie));
    trie->root = make_trieNode(' ');
    trie->size = 0;
    return trie;
}

Trie *clear_trie(Trie *trie) {
    trie->size = 0;
    free_trieNode(trie->root);
    trie->root = NULL;
    return trie;
}

TrieNode *make_trieNode(char data) {
    // Создание узла
    TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
    for (int i = 0; i < N_CHARACTERS; i++)
        node->children[i] = NULL;
    node->is_leaf = 0;
    node->data = data;
    return node;
}


void free_trieNode(TrieNode *node) {
    //Освобождение памяти, занимаемой узлом
    if (node == NULL) {
        free(node);
        return;
    }
    for (int i = 0; i < N_CHARACTERS; i++) {
        if (node->children[i] != NULL) {
            // очищае рукурсивно
            free_trieNode(node->children[i]);
        } else {
            continue;
        }
    }
    //node = NULL;
    free(node);
}

int get_char_position(char ch) {
    int ch_ = (int) tolower(ch);
    if (isdigit(ch)) return ch_ - 48;
    //Русские буквы не работают
    //if (ch_ < 0) return ch_ + 58;
    return (int) ch_ - 'a' + 10;
}

int trie_add_word(Trie *trie, char *word) {
    // вставка слова в дерево
    if (trie->root == NULL) trie->root = make_trieNode(' ');
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
    TrieNode *previous;
    TrieNode *previousDel = NULL;

    TrieNode *deleteFrom = NULL; // узел, который можно удалить, не повредив дерево

    for (int i = 0; word[i] != '\0'; i++) {
        //спускаемся вниз по дереву
        int position = get_char_position(word[i]);
        if (temp->children[position] == NULL) {
            // такого слова нет
            return 0;
        } else {
            // проверяем, можно ли удалить этот узел, не повредив дерево
            previous = temp;
            int j = 0;
            for (; j < N_CHARACTERS; j++) {
                if (temp->children[j] != NULL) {
                    if (temp->children[j]->data != word[i]) break;
                }
            }
            if (j == N_CHARACTERS) {
                if (deleteFrom == NULL && !temp->is_leaf) {
                    previousDel = previous;
                    deleteFrom = temp;
                } else if (temp->is_leaf) {
                    deleteFrom = NULL;
                }
            } else deleteFrom = NULL;
            // спускаемся
            temp = temp->children[position];
        }
    }
    int j = 0;
    for (; j < N_CHARACTERS; j++) {
        if (temp->children[j] != NULL) {
            break;
        }
    }
    if (j == N_CHARACTERS) {
        if (deleteFrom == NULL) {
            previousDel = previous;
            deleteFrom = temp;
        }
    } else deleteFrom = NULL;
    if (temp == NULL) {
        return 0;
    }
    if (!temp->is_leaf) {
        return 0;
    }

    temp->is_leaf = 0; // удаляем слово
    if (deleteFrom != NULL) {
        if (deleteFrom == trie->root) deleteFrom = deleteFrom->children[get_char_position(word[0])];
        int i = 0;
        for (; previousDel->children[i] != deleteFrom; i++);
        previousDel->children[i] = NULL;
        free_trieNode(deleteFrom);// очищаем память, если возможно
    }
    trie->size--;
    return 1;
}

wchar_t *concat(wchar_t *s1, wchar_t *s2) {
    const size_t len1 = wcslen(s1);
    const size_t len2 = wcslen(s2);
    wchar_t *result = malloc((len1 + len2 + 1) * sizeof(result)); // +1 для '\0'
    wcscpy(result, s1);
    wcscat(result, s2);
    return result;
}

void trie_print_(wchar_t *prefix, int isTail, TrieNode *node) {
    wchar_t *suffix;
    wchar_t *fix;
    if (isTail) {
        suffix = L"    ";
        fix = L"└── ";
    } else {
        suffix = L"│   ";
        fix = L"├── ";
    }

    wchar_t *data = L"";
    if (node->is_leaf) {
        data = L"<-";
    }

    wprintf(L"%s%s%c%s\n", prefix, fix, node->data, data);

    int size = 0;
    for (int i = 0; i < N_CHARACTERS; i++) {
        if (node->children[i] != NULL) size++;
    }

    for (int i = 0; i < N_CHARACTERS && size > 1; i++) {
        if (node->children[i] != NULL) {
            size--;
            wchar_t *new_prefix = concat(prefix, suffix);
            trie_print_(new_prefix, 0, node->children[i]);
            free(new_prefix);
        }
    }
    if (size != 0) {
        int last = 0;
        for (int i = 0; i < N_CHARACTERS; i++) {
            if (node->children[i] != NULL) {
                last = i;
            }
        }
        wchar_t *new_prefix = concat(prefix, suffix);
        trie_print_(new_prefix, 1, node->children[last]);
        free(new_prefix);
    }
}

void trie_print(Trie *trie) {
    if (trie->size == 0) {
        wprintf(L"Trie is empty!\n\n");
        return;
    }
    wchar_t *prefix = L"";
    trie_print_(prefix, 1, trie->root);
}