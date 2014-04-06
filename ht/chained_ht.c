// A hash table using chaining to resolve collisions. We use a doubly linked list to store
// the collisions because that speeds up deletion times.  Note that we do not maintain the
// list in order because we do not expect the lists to become very large.  However, should
// large numbers of collisions be anticipated then we could re-implement with a binary
// search tree (or a height-balanced search tree - such as an avl tree).
// Author: drs

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "any.h"
#include "clist.h"
#include "ht.h"

struct ht_implementation
{
    clist **items;    // array of linked lists
    int size;         // current number of objects stored in the table
    hashfun h1;       // user-supplied primary hash function
    int max;          // user-supplied maximum table size
};

ht * new_ht(int max, hashfun h1)
{
    int i;
    ht * t = (ht*)malloc(sizeof(ht));
    t->items = (clist**)malloc(max * sizeof(clist*));
    t->size = 0;
    t->h1 = h1;
    t->max = max;
    for (i=0; i<max; i++)
        t->items[i] = new_clist();
    return t;
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

int ht_insert(ht *t, char *k, char *v)
{
    assert(t!=NULL);
    char *word = malloc(sizeof(char) * 40);
    char *description = malloc(sizeof(char) * 200);
    strcpy(word,k); strcpy(description, v);
    int b = t->h1(k);
    clist_goto_last(t->items[b]);
    clist_goto_prev(t->items[b]);
    clist_ins_before(t->items[b], word);
    clist_ins_before(t->items[b], description);
    
    (t->size) += 2;
    return 1;           // insertion always succeeds
}

int ht_delete(ht *t, char *k)
{
    assert(t!=NULL);
    int j;
    j =  t->h1(k);
    clist_goto_head(t->items[j]);
    while (clist_cursor_inlist(t->items[j])) {
        if (strcmp(clist_get_item(t->items[j]), k) == 0) {
            clist_delete(t->items[j]);
            (t->size)--;
            return 1;
        }
        else
            clist_goto_next(t->items[j]);
    }
    return 0;           // report failed delete
}

char * ht_lookup(ht *t, const char *k)
{
    assert(t!=NULL);
    char *a;
    char *x = malloc(sizeof(char) * strlen(k));
    assert(x != NULL);
    strcpy(x, k);
    int b = t->h1(x);
    clist_goto_head(t->items[b]);
    while (clist_cursor_inlist(t->items[b]))
        if (strcmp(clist_get_item(t->items[b]), x) == 0) {
            clist_goto_next(t->items[b]);
            return a = clist_get_item(t->items[b]);
        } else {
            clist_goto_next(t->items[b]);
        }

    return NULL;        // report failed lookup
}

void ht_profile(ht *t)
{
    assert(t!=NULL);
    int i,j;
    printf("\n");
    for (i=0; i < t->max; i++) {
        printf("%4i: ",i);
        for (j=0; j < clist_size(t->items[i]); j++)
            printf("X");
        printf("\n");
    }
    printf("Load Factor = %f\n", ht_loadfactor(t));
}

void ht_display(ht *t, int lo, int hi)
{
    int i,j;
    assert(0 <= lo && lo <= hi && hi < t->max);
    printf("Displaying hash table from %i to %i\n", lo, hi);
    for (i=0; i < t->max; i++) {
        printf("%8i: ",i);
        clist_goto_head(t->items[i]);
        while(clist_cursor_inlist(t->items[i])) {
            printf("%s <--> ", (char *) clist_get_item(t->items[i]));
            clist_goto_next(t->items[i]);
        }
        printf("!\n");
    }
}

void ht_release(ht *t)
{
    assert(t!=NULL);
    int i;
    clist * l;
    for (i=0; i < t->max; i++) {
        l = t->items[i];
        clist_goto_head(l);
        while (!clist_isempty(l))
            clist_delete(l);
        clist_release(l);
    }
    free(t);
}
