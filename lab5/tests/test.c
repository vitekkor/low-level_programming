#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"
#include "../trie.h"

int run_all_tests() {
    printf("Running tests...\n");
    return test_add() + test_delete() + test_search();
}

int test_add() {
    Trie *trie = create_Trie();
    char path_in[] = "..\\test_data\\test_add.txt";
    char path_out[] = "..\\test_data\\test_add_expected.txt";
    FILE *file_in = fopen(path_in, "r");
    FILE *file_out = fopen(path_out, "r");
    char word[MAX_WORD_LENGTH];
    while (fgets(word, MAX_WORD_LENGTH, file_in) != NULL) {
        word[strlen(word) - 1] = '\0';
        int result = trie_add_word(trie, word);
        fgets(word, MAX_WORD_LENGTH, file_out);
        if (result != atoi(word)) {
            printf("Add test failed\n");
            return 1;
        }
    }
    clear_trie(trie);
    free(trie);
    fclose(file_in);
    free(file_in);
    fclose(file_out);
    free(file_out);
    printf("Add test passed\n");
    return 0;
}

int test_delete() {
    Trie *trie = create_Trie();
    char path_in[] = "..\\test_data\\test_add.txt";
    FILE *file_in = fopen(path_in, "r");
    char word[MAX_WORD_LENGTH];
    while (fgets(word, MAX_WORD_LENGTH, file_in) != NULL) {
        word[strlen(word) - 1] = '\0';
        trie_add_word(trie, word);
    }
    fclose(file_in);
    char path_delete_in[] = "..\\test_data\\test_delete.txt";
    char path_delete_out[] = "..\\test_data\\test_delete_expected.txt";
    FILE *file_delete_in = fopen(path_delete_in, "r");
    FILE *file_delete_out = fopen(path_delete_out, "r");
    while (fgets(word, MAX_WORD_LENGTH, file_delete_in) != NULL) {
        word[strlen(word) - 1] = '\0';
        int result = trie_delete_word(trie, word);
        fgets(word, MAX_WORD_LENGTH, file_delete_out);
        if (result != atoi(word)) {
            printf("Delete test failed\n");
            return 1;
        }
    }
    clear_trie(trie);
    free(trie);
    fclose(file_delete_in);
    fclose(file_delete_out);
    free(file_delete_out);
    free(file_delete_in);
    free(file_in);
    printf("Delete test passed\n");
    return 0;
}

int test_search() {
    Trie *trie = create_Trie();
    char path_in[] = "..\\test_data\\test_add.txt";
    FILE *file_in = fopen(path_in, "r");
    char word[MAX_WORD_LENGTH];
    while (fgets(word, MAX_WORD_LENGTH, file_in) != NULL) {
        word[strlen(word) - 1] = '\0';
        trie_add_word(trie, word);
    }
    fclose(file_in);
    char path_delete_in[] = "..\\test_data\\test_search.txt";
    char path_delete_out[] = "..\\test_data\\test_search_expected.txt";
    FILE *file_delete_in = fopen(path_delete_in, "r");
    FILE *file_delete_out = fopen(path_delete_out, "r");
    int result;
    while (fgets(word, MAX_WORD_LENGTH, file_delete_in) != NULL) {
        word[strlen(word) - 1] = '\0';
        if (strcmp(word, "delete") == 0) {
            result = trie_search(trie, word);
            fgets(word, MAX_WORD_LENGTH, file_delete_out);
            if (result != atoi(word)) {
                printf("Search test failed\n");
                return 1;
            }
            trie_delete_word(trie, word);
        }
        result = trie_search(trie, word);
        fgets(word, MAX_WORD_LENGTH, file_delete_out);
        if (result != atoi(word)) {
            printf("Search test failed\n");
            return 1;
        }
    }
    clear_trie(trie);
    free(trie);
    fclose(file_delete_in);
    fclose(file_delete_out);
    free(file_delete_out);
    free(file_delete_in);
    free(file_in);
    printf("Search test passed\n");
    return 0;
}