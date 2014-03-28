// Linked implementation of an circular list with a sentinel node
// Author: drs

#ifndef CLIST_H
#define CLIST_H

#include "any.h"

typedef struct clist_implementation clist;
typedef void (*modify)(any a);
typedef int (*pred)(any a);
            
clist * new_clist           ();
int     clist_size          (clist *c);
int     clist_isempty       (clist *c);
void    clist_goto_head     (clist *c);
void    clist_goto_last     (clist *c);
void    clist_goto_next     (clist *c);
void    clist_goto_prev     (clist *c);
int     clist_cursor_inlist (clist *c);
any     clist_get_item      (clist *c);
void    clist_ins_before    (clist *c, any  item);
void    clist_ins_after     (clist *c, any  item);
int     clist_delete        (clist *c);
void    clist_iterate       (clist *c, modify f);
int     clist_find          (clist *c, pred p);
int     clist_find_next     (clist *c, pred p);
void    clist_print         (clist *c, void (* item_print)(any item));
void    clist_release       (clist *c);

#endif

