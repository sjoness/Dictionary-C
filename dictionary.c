#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ht.h"
#include "dictionary.h"

#define TABLE_SIZE 20

/**
 * Dictionary data structure. Each key-value pair is known as an entry.
 */
typedef struct dictionary {
    ht *entry;
} dict;

dict *d;
char temp_word[MAX_WORD_SIZE+1], temp_description[MAX_DESC_SIZE+1];

/**
 * This function takes a given string and translates it to a key that can be
 * then used to identify a value in the hash table.
 *
 * @param key    a string that is to be translated to a key.
 * @return an integer value that will idenify a value in the hash table.
 */
char * get_key(char *key) {
#pragma message("Potentially redundant function")
    return key; // use the word being inserted, as the key
}

/**
 * Hash function to produce an integer value which will determine which bucket
 * the a key-value pair will be stored in.
 *
 * @param key    a key-value pair that has been translated to a key by the
 *                get_key function.
 * @return an integer value that represents which bucket to store the key-value
 *         pair.
 */
int hash(char *key) {
    int len = strlen(key);
    char p = key[0];
    unsigned int x = p << 7;

    while (len >= 0) {
        x = (1000003 * x) ^ p;
        p++; len--;
    }

    x ^= len;

    return x % TABLE_SIZE;
}

void d_initialise() {
    d = malloc(sizeof(dict));
    assert(d != NULL); // Assert that malloc did not fail.
    d->entry = new_ht(TABLE_SIZE, hash, get_key);
}

int d_read_from_file(const char *filename) {
    FILE *fp;

    fp = fopen(filename, "r"); // Open the file stream for reading.

    if (fp == NULL) {
        printf("Error: reading file \"%s\"\n", filename);
        ht_release(d->entry); // Release memory that was allocated for the dictionary.
        exit(1);
    } else {
        while (fscanf(fp, "%s %[^\n]", temp_word, temp_description) != EOF) {
            char *word = malloc(sizeof(char) * strlen(temp_word));
            char *description = malloc(sizeof(char) * strlen(temp_description));

            /* Check if the first character of the word being read is a dot
            If it is, do not add it to the dictionary and end reading from the
            file */
            if (!strncmp(temp_word, ".", 1)) {
                fclose(fp); // Close the file stream
                return 1;
            } else {
                strcpy(word, temp_word);
                strcpy(description, temp_description);

                // Check to see if the word being read in from the file,
                // exists in the dictionary
                if (ht_lookup(d->entry, word) != NULL) {
                    if (strcmp(word, ht_word_lookup(d->entry, word)) == 0) {
                        // If the word is already in the dictionary, replace
                        // it's definition with the new one being read in
                        ht_replace(d->entry, word, description);
                    }
                } else {
                    // Insert the new word and definition as it does not
                    // already exist in the dictionary
                    ht_insert(d->entry, word, description);
                }
            }
        }
    }

    fclose(fp); // Close the file stream

    return 1; // Return true, the file was successfully imported.
}

int d_lookup(const char *word, char *meaning) {
    if ((ht_lookup(d->entry, word)) != NULL) {
        strcpy(meaning, ht_lookup(d->entry, word));
        return 1; // the word was found in the dictionary
    } else {
        return 0; // the word was not found
    }
}
