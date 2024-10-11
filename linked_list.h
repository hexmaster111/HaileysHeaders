#ifndef LLIST_H
#define LLIST_H

typedef struct LINKED_LIST_CONTAINER *LINKED_LIST_CONTAINERPTR;

typedef struct LINKED_LIST_CONTAINER
{
    LINKED_LIST_CONTAINERPTR next;
    void *data;
} LINKED_LIST_CONTAINER, *LINKED_LIST_CONTAINERPTR;

typedef struct LINKED_LIST
{
    int len;
    LINKED_LIST_CONTAINERPTR head;
} LLIST, *LLISTPTR;

typedef int (*LLIST_COMPARISON_FUNC)(void *, void *);
typedef void (*LLIST_ITTERATOR_FUNC)(void *);

void llist_free(LLISTPTR t);

/// @brief appends <data> to the end of the list
void llist_append(LLISTPTR t, void *data);

/// @brief prepends <data> to the front of the list
void llist_prepend(LLISTPTR t, void *data);

/// @brief insertes <data> into the list at <idx>
void llist_insert(LLISTPTR t, void *data, int idx);

/// @brief returns data from list at idx
void *llist_at(LLISTPTR t, int idx);

/// @brief removes data from list at <idx>
void llist_remove(LLISTPTR t, int idx);

/// @brief swaps the position of data at <x> with data at pos <y>
void llist_swap(LLISTPTR t, int x, int y);

/// @brief returns 1 if there is content in the list else 0
int llist_is_empty(LLISTPTR t);

/// @brief quicksorts <t> using the <f> comparison
void llist_sort(LLISTPTR t, LLIST_COMPARISON_FUNC f);

/// @brief calls <i> for every element in <t>
void llist_itterate(LLISTPTR t, LLIST_ITTERATOR_FUNC i);

#endif // LLIST_H