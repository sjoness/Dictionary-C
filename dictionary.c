#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ht.h"
#include "dictionary.h"

#define TABLE_SIZE 500000

// Global instance of a hashtable. The global scope allows the dictionary to
// be accessed by all functions in this file.
ht *dictionary;

/**
 * My research for a sufficient string hash function has lead me to find
 * this algorithm was found on:
 *      http://www.laurentluce.com/posts/python-dictionary-implementation/
 * 
 * this is my implementation.
 */
unsigned int hash(char *key) {
    int string_length = strlen(key);
    char char_pointer = key[0];
    unsigned int hash_value = char_pointer << 7;

    while (string_length > 0) {
        hash_value += (1000003 * hash_value) ^ char_pointer;
        char_pointer++; string_length--;
    }

    hash_value ^= strlen(key);

    return hash_value % TABLE_SIZE;
}

void d_initialise() {
    dictionary = new_ht(TABLE_SIZE, hash);
    assert(dictionary != NULL);
}

int d_read_from_file(const char *filename) {
    FILE *file_pointer;
    char *word = malloc(sizeof(char) * MAX_WORD_SIZE);
    char *meaning = malloc(sizeof(char) * MAX_DESC_SIZE);
    assert(word != NULL);
    assert(meaning != NULL);

    file_pointer = fopen(filename, "r");

    if (file_pointer != NULL) {
        while (fscanf(file_pointer, "%s %[^\n]", word, meaning) != EOF) {
            if (!strncmp(word, ".", 1)) {
                fclose(file_pointer); // Close the file stream
                return 1;
            } else {
                ht_insert(dictionary, word, meaning);
            }
        }
    } else {
        printf("Error: reading file \"%s\"\n", filename);
        ht_release(dictionary);
        exit(1);
    }

    fclose(file_pointer); // If control reaches here, the file did not end with a period

    return 1; // Return true, the file was successfully imported.
}

int d_lookup(const char *word, char *meaning) {
    if ((ht_lookup(dictionary, word)) != NULL) {
        strcpy(meaning, ht_lookup(dictionary, word));
        return 1;
    } else {
        return 0;
    }
}
