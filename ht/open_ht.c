// A hash table using open addressing.  Note, that specialised versions could be made more
// efficient.  For example, the key extraction function could be a macro and the hash
// functions could be macros too (or inline functions).  Furthermore, in this file we have
// combined all three open-addressing methods and used a flag and a switch statement to
// select the appropriate strategy.  A more efficient version would be used in real-life
// that would be tailored for the application and, hence, would not repeatedly use clock
// cycles looking up which strategy was to be used.  We have presented them together here
// primarily for educational purposes - so that the single file may be used to create
// different hash tables using different strategies and so that the implementations may be
// viewed together - highlighting the differences as well as the similarities.
// Author: drs

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ht.h"

struct ht_implementation
{
    any * items;      // array of stored objects
    any deleted;      // static address reserved to indicate deleted locations
    int size;         // current number of objects stored in the table
    hashfun h1;       // user-supplied primary hash function
    getkeyfun getkey; // user-supplied function to extract a key from an object
    int probing;      // probe type: LINEAR_PROBING | QUADRATIC_PROBING | SECONDARY_HASHING
    int qpc1;         // quadratic probing constant c1
    int qpc2;         // quadratic probing constant c2
    hashfun h2;       // user-supplied secondary hash function    (NULL if not used)
    int max;          // user-supplied maximum table size
};

static any deleted;   // <- technique used in Kyle Loudon's Mastering Algorithms with C
                      // (O'Reilly, 1999)

ht * new_ht(int max, hashfun h1, getkeyfun getkey)
{
    int i;
    ht * t = (ht*)malloc(sizeof(ht));
    t->deleted = &deleted;
    t->items = (any*)malloc(max * sizeof(any));
    t->size = 0;
    t->h1 = h1;
    t->getkey = getkey;
    t->probing = LINEAR_PROBING; // by default
    t->h2 = NULL;
    t->qpc1 = 0;
    t->qpc2 = 0;
    t->max = max;
    for (i=0; i<max; i++)
        t->items[i] = NULL;
    return t;
}

void ht_use_quadratic_probing(ht *t, int c1, int c2)
{
    assert(t!=NULL);
    if (ht_isempty(t)) {
        t->probing = QUADRATIC_PROBING;
        t->qpc1 = c1;
        t->qpc2 = c2;
    }
    else
        printf("error: ht_use_quadratic_probing - table not empty\n");
}

void ht_use_double_hashing(ht *t, hashfun h2)
{
    assert(t!=NULL);
    if (ht_isempty(t)) {
        t->probing = SECONDARY_HASHING;    
        t->h2 = h2;
    }
    else
        printf("error: ht_use_double_hashing - table not empty\n");
}

int ht_isempty(ht *t)
{
    assert(t!=NULL);
    return (t->size == 0);
}
    
int  ht_isfull(ht *t)
{
    assert(t!=NULL);
    return t->size==t->max;
}
    
int  ht_size(ht *t)
{
    assert(t!=NULL);
    return t->size;
}

float ht_loadfactor(ht *t)
{
    assert(t!=NULL);
    return ((float)(t->size) / (float)(t->max));
}

int ht_insert(ht *t, any x)
{
    assert(t!=NULL);
    int i, j;
    int k = t->getkey(x);
    for(i=0; i < t->max; i++) {
        switch (t->probing) {
        case LINEAR_PROBING    : j = (t->h1(k) + i)                             % t->max;
                                 break;
        case QUADRATIC_PROBING : j = (t->h1(k) + t->qpc1 * i + t->qpc2 * i * i) % t->max;
                                 break;
        case SECONDARY_HASHING : j = (t->h1(k) + (i * t->h2(k)))                % t->max;
        }
        if (t->items[j] == NULL || t->items[j] == t->deleted) {
            t->items[j] = x;
            (t->size)++;
            return 1;                   // report successful insertion
        }
        else // keep looking
            continue;
    }
    return 0;                           // report failure to insert item
}

int ht_delete(ht *t, int k)
{
    assert(t!=NULL);
    int i, j;
    for(i=0; i < t->max; i++) {
        switch (t->probing) {
        case LINEAR_PROBING    : j = (t->h1(k) + i)                             % t->max;
                                 break;
        case QUADRATIC_PROBING : j = (t->h1(k) + t->qpc1 * i + t->qpc2 * i * i) % t->max;
                                 break;
        case SECONDARY_HASHING : j = (t->h1(k) + (i * t->h2(k)))                % t->max;
        }
        if (t->items[j] == NULL)        // item was not there
            return 0;                   // report failure to delete item
        else if (t->items[j] == t->deleted)
            continue;
        else if (k == t->getkey(t->items[j])) {
            t->items[j] = t->deleted;
            (t->size)--;
            return 1;                   // report successful deletion
        }
        else // keep looking
            continue;
    }
}

any ht_lookup(ht *t, int k)
{
    assert(t!=NULL);
    int j, i;
    for(i=0; i < t->max; i++) {
        switch (t->probing) {
        case LINEAR_PROBING    : j = (t->h1(k) + i)                             % t->max;
                                 break;
        case QUADRATIC_PROBING : j = (t->h1(k) + t->qpc1 * i + t->qpc2 * i * i) % t->max;
                                 break;
        case SECONDARY_HASHING : j = (t->h1(k) + (i * t->h2(k)))                % t->max;
        }
        if (t->items[j] == NULL)
            return NULL;
        else if (t->items[j] == t->deleted)
            continue;
        else if (k == t->getkey(t->items[j]))
            return t->items[j];         // return the located item
        else // keep looking
            continue;
    }
    return NULL;                        // report failed lookup
}

void ht_profile(ht *t)
{
    assert(t!=NULL);
    int i;
    for (i=0; i < t->max; i++)
        if (t->items[i] == NULL)
            printf(".");
        else if (t->items[i] == t->deleted)
            printf(":");
        else
            printf("*");
    printf("\n'.' = empty; ':' = deleted; '*' = occupied\n");
    printf("Load Factor = %f\n", ht_loadfactor(t));
}

void ht_display(ht *t, int lo, int hi)
{
    int i;
    assert(0 <= lo && lo <= hi && hi < t->max);
    printf("Displaying hash table from %i to %i\n", lo, hi);
    for (i=lo; i<=hi; i++)
        if (t->items[i] == NULL)
           printf("%8i\tempty\n",i);
        else
           printf("%8i\t%i\n",i,t->getkey(t->items[i]));
}

void ht_release(ht *t)
{
    assert(t!=NULL);
    free(t->items);
    free(t);
}

