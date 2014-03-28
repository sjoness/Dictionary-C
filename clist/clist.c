// Linked implementation of an circular list with a sentinel node
// Author: drs

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "clist.h"

struct node
{
    any item;
    struct node * next;
    struct node * prev;
};

struct clist_implementation
{
    struct node * sentinel;
    struct node * cursor;
    int size;
};

clist * new_clist()
{
    clist * c = (clist *)malloc(sizeof(clist));
    if (c==NULL) {
        printf("new_clist: malloc failure.\n");
        exit(1);
    }
    c->size = 0;
    c->sentinel = (struct node *) malloc(sizeof(struct node));
    if (c->sentinel==NULL) {
        printf("new_clist(sentinel): malloc failure.\n");
        exit(1);
    }
    c->sentinel->item = NULL;
    c->sentinel->next = c->sentinel;
    c->sentinel->prev = c->sentinel;
    c->cursor = c->sentinel;
    return c;
}
 
int clist_size(clist *c)
{
    assert(c!=NULL);
    return c->size;
}
 
int clist_isempty(clist *c)
{
    assert(c!=NULL);
    return c->size == 0;
}

void clist_goto_head(clist *c)
{
    assert(c!=NULL);
    c->cursor = c->sentinel->next;
}

void clist_goto_last(clist *c)
{
    assert(c!=NULL);
    c->cursor = c->sentinel->prev;
}

void clist_goto_next(clist *c)
{
    assert(c!=NULL);
    c->cursor = c->cursor->next;
}

void clist_goto_prev(clist *c)
{
    assert(c!=NULL);
    c->cursor = c->cursor->prev;
}

int clist_cursor_inlist(clist *c)
{
    assert(c!=NULL);
    return c->cursor != c->sentinel;
}

any clist_get_item(clist *c)
{
    assert(c!=NULL);
    return (clist_cursor_inlist(c)) ? c->cursor->item : NULL;
}

void clist_ins_before(clist *c, any  item)
{
    assert(c!=NULL);
    struct node * n = (struct node *) malloc(sizeof(struct node));
    if (n==NULL) {
        printf("clist_ins_before: malloc failure.\n");
        exit(1);
    }
    n->item = item;
    n->next = c->cursor;
    n->prev = c->cursor->prev;
    c->cursor->prev->next = n;
    c->cursor->prev = n;
    c->size++;
}

void clist_ins_after(clist *c, any  item)
{
    assert(c!=NULL);
    struct node * n = (struct node *) malloc(sizeof(struct node));
    if (n==NULL) {
        printf("clist_ins_after: malloc failure.\n");
        exit(1);
    }
    n->item = item;
    n->prev = c->cursor;
    n->next = c->cursor->next;
    c->cursor->next->prev = n;
    c->cursor->next = n;
    c->size++;
}

int clist_delete(clist *c)
{
    assert(c!=NULL);
    struct node * p = c->cursor;
    if (clist_cursor_inlist(c))
    {
        c->cursor = c->cursor->next;
        p->prev->next = p->next;
        p->next->prev = p->prev;
        free(p);
        c->size--;
        return 1;
    }
    else
        return 0;
}

void clist_iterate(clist *c, modify f)
{
    assert(c!=NULL);
    struct node * n = c->cursor;
    clist_goto_head(c);
    while (clist_cursor_inlist(c))
    {
        f(c->cursor->item);
        clist_goto_next(c);
    }
    c->cursor = n;
}

int clist_find(clist *c, pred p)
{
    assert(c!=NULL);
    clist_goto_head(c);
    while (clist_cursor_inlist(c) && (!p(c->cursor->item)))
        clist_goto_next(c);
    return clist_cursor_inlist(c);    
}

int clist_find_next(clist *c, pred p)
{
    assert(c!=NULL);
    struct node * n = c->cursor;
    while (clist_cursor_inlist(c) && (!p(c->cursor->item)))
        clist_goto_next(c);
    if (clist_cursor_inlist(c))
        return 1;
    else {
        c->cursor = n;
        return 0;
    }
}

void clist_print(clist *c, void (* item_print)(any item))
{
    assert(c!=NULL);
    struct node * n = c->cursor;
    printf("CL[");
    clist_goto_head(c);
    if (clist_cursor_inlist(c)) {
        item_print(c->cursor->item);
        clist_goto_next(c);
        while (clist_cursor_inlist(c)) {
            printf(", ");
            item_print(c->cursor->item);
            clist_goto_next(c);
        }
    }
    printf("]");
    c->cursor = n;
}

void clist_release(clist *c)
{
    assert(c!=NULL);
    free(c->sentinel);
    free(c);
}

