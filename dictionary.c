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

void d_initialise() {
    d = malloc(sizeof(dict));
    assert(d != NULL); // Assert that malloc did not fail.
    d->entry = new_ht(TABLE_SIZE, hash, get_key);
}
