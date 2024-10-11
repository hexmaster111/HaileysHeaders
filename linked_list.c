#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

LINKED_LIST_CONTAINERPTR newcontainer()
{
    LINKED_LIST_CONTAINERPTR new = malloc(sizeof(LINKED_LIST_CONTAINER));
    new->data = NULL;
    new->next = NULL;
    return new;
}

LINKED_LIST_CONTAINERPTR llist_container_at(LLISTPTR t, int idx)
{
    LINKED_LIST_CONTAINERPTR c = t->head;

    while (idx)
    {
        idx = idx - 1;
        c = c->next;
    }

    return c;
}

void llist_free(LLISTPTR t)
{
    LINKED_LIST_CONTAINERPTR c = t->head;

    for (size_t i = 0; i < t->len; i++)
    {
        LINKED_LIST_CONTAINERPTR tofree = c;
        c = c->next;
        free(tofree);
    }

    t->head = NULL;
    t->len = 0;
}

void llist_append(LLISTPTR t, void *data)
{
    if (t->head == NULL) /* this is a new list with no head */
    {
        t->head = newcontainer();
        t->head->data = data;
        t->len = 1;
        return;
    }

    LINKED_LIST_CONTAINERPTR tmp = t->head;

    while (tmp->next)
    {
        tmp = tmp->next;
    }

    LINKED_LIST_CONTAINERPTR new = newcontainer();
    t->len = t->len + 1;
    new->data = data;
    tmp->next = new;
}

void llist_prepend(LLISTPTR t, void *data)
{
    if (t->head == NULL) /* this is a new list with no head */
    {
        t->head = newcontainer();
        t->head->data = data;
        t->len = 1;
        return;
    }

    LINKED_LIST_CONTAINERPTR new = newcontainer();
    new->data = data;
    new->next = t->head;

    t->head = new;
    t->len = t->len + 1;
}

void *llist_at(LLISTPTR t, int idx)
{
    if (idx > t->len)
        abort();

    LINKED_LIST_CONTAINERPTR c = t->head;

    while (idx)
    {
        idx = idx - 1;
        c = c->next;
    }

    return c->data;
}

int llist_is_empty(LLISTPTR t)
{
    return t->len == 0;
}

void llist_insert(LLISTPTR t, void *data, int idx)
{
    t->len += 1;

    LINKED_LIST_CONTAINERPTR c = t->head;
    idx--;

    while (idx)
    {
        c = c->next;
        idx = idx - 1;
    }

    LINKED_LIST_CONTAINERPTR new = newcontainer();
    new->data = data;
    new->next = c->next;
    c->next = new;
}

void llist_remove(LLISTPTR t, int idx)
{
    t->len -= 1;

    if (idx == 0)
    {
        LINKED_LIST_CONTAINERPTR f = t->head;
        t->head = f->next;
        free(f);
        return;
    }

    LINKED_LIST_CONTAINERPTR c = t->head;

    idx--;

    while (idx)
    {
        c = c->next;
        idx = idx - 1;
    }

    LINKED_LIST_CONTAINERPTR tofree = c->next;

    c->next = tofree->next;
    free(tofree);
}

void llist_swap(LLISTPTR t, int x, int y)
{
    LINKED_LIST_CONTAINERPTR f = llist_container_at(t, x),
                             s = llist_container_at(t, y);

    void *tmp = s->data;

    s->data = f->data;
    f->data = tmp;
}

// ===================== QUICK SORT IMPL ========================= //
int llist_quick_sort_partition(LLISTPTR l, int low, int high, LLIST_COMPARISON_FUNC comp)
{
    int i = low, j = high;

    void *pv = llist_at(l, low);

    while (i < j)
    {
        // find the first element that is gtr then the pivot
        while (comp(llist_at(l, i), pv) < 1 && i <= high - 1)
        {
            i++;
        }

        // find the first element smaller then pvt (from last)
        while (comp(llist_at(l, j), pv) > 0 && j >= low + 1)
        {
            j--;
        }

        if (i < j)
        {
            llist_swap(l, i, j);
        }
    }
    llist_swap(l, low, j);
    return j;
}

void llist_quick_sort(LLISTPTR l, int low, int high, LLIST_COMPARISON_FUNC comp)
{
    if (low < high)
    {
        int partIdx = llist_quick_sort_partition(l, low, high, comp);
        llist_quick_sort(l, low, partIdx - 1, comp);
        llist_quick_sort(l, partIdx + 1, high, comp);
    }
}

void llist_sort(LLISTPTR t, LLIST_COMPARISON_FUNC comp)
{
    llist_quick_sort(t, 0, t->len - 1, comp);
}

void llist_itterate(LLISTPTR t, LLIST_ITTERATOR_FUNC ittrf)
{
    // todo: have a way to check if the list was modifyed while itterating
    for (size_t i = 0; i < t->len; i++)
    {
        ittrf(llist_at(t, i));
    }
}
