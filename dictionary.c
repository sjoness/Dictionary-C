#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ht.h"
#include "dictionary.h"

#define TABLE_SIZE 20

ht *dictionary;

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
    dictionary = new_ht(TABLE_SIZE, hash);
    assert(dictionary != NULL);
}

int d_read_from_file(const char *filename) {
    FILE *fp;
    char stringForKey[MAX_WORD_SIZE+1], desc[MAX_DESC_SIZE+1];

    fp = fopen(filename, "r"); // Open the file stream for reading.

    if (fp != NULL) {
        while (fscanf(fp, "%s %[^\n]", stringForKey, desc) != EOF) {
            /* Check if the first character of the word being read is a dot
            If it is, do not add it to the dictionary and end reading from the
            file */
            if (!strncmp(stringForKey, ".", 1)) {
                fclose(fp); // Close the file stream
                return 1;
            } else {
                ht_insert(dictionary, stringForKey, desc);
            }
        }
    } else {
        printf("Error: reading file \"%s\"\n", filename);
        ht_release(dictionary); // Release memory that was allocated for the dictionary.
        exit(1);
    }

    fclose(fp); // Close the file stream

    return 1; // Return true, the file was successfully imported.
}

int d_lookup(const char *word, char *meaning) {
    if ((ht_lookup(dictionary, word)) != NULL) {
        strcpy(meaning, ht_lookup(dictionary, word));
        return 1; // the word was found in the dictionary
    } else {
        return 0; // the word was not found
    }
}
