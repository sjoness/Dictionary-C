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
char stringForKey[MAX_WORD_SIZE+1], desc[MAX_DESC_SIZE+1];

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
        return 0; // Return false if the file was not successfully imported.
    } else {
        while (fscanf(fp, "%s %[^\n]", stringForKey, desc) != EOF) { // store the word in word and the description in desc.
            char *description;
            description = (char *)malloc(sizeof(char) * strlen(desc));

            if (!strncmp(stringForKey, ".", 1)) {
                fclose(fp); // Close the file stream
                return 1;
            } else {
                strcpy(description, desc);
                ht_insert(d->entry, stringForKey, description);
            }
        }
    }

    fclose(fp); // Close the file stream

    return 1; // Return true, the file was successfully imported.
}
