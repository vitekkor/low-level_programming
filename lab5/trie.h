#ifndef LOW_LEVEL_PROGRAMMING_TRIE_H
#define LOW_LEVEL_PROGRAMMING_TRIE_H

#include <string.h>

// 26 English and 10 digits
#define N_CHARACTERS 36
#define MAX_WORD_LENGTH 1024
typedef struct Trie Trie;

typedef struct TrieNode TrieNode;

Trie *create_Trie();

Trie *clear_trie(Trie *trie);

TrieNode *make_trieNode(char data);

void free_trieNode(TrieNode *node);

int get_char_position(char ch);

int trie_add_word(Trie *trie, char *word);

int trie_search(Trie *trie, char *word);

int trie_delete_word(Trie *trie, char *word);

wchar_t *concat(wchar_t *s1, wchar_t *s2);

void trie_print_(wchar_t *prefix, int isTail, TrieNode *node);

void trie_print(struct Trie *trie);

#endif
