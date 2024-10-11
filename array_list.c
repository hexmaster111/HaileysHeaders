#include "array_list.h"
#include <assert.h>

#ifndef arrlist_assert
#define arrlist_assert(cond) assert(cond);
#endif // arrlist_assert

void alist_init(ARRLIST_PTR l)
{
    l->data = malloc(sizeof(VOIDPTR));
    l->cap = 1;
    l->len = 0;
}
void alist_deinit(ARRLIST_PTR l)
{
    free(l->data);
    l->cap = 0;
    l->data = NULL;
    l->len = 0;
}

void alist_free(ARRLIST_PTR l)
{
    free(l->data);
    free(l);
}
ARRLIST_PTR alist_new()
{
    ARRLIST_PTR list = malloc(sizeof(LIST));
    list->data = malloc(sizeof(VOIDPTR));
    list->cap = 1;
    list->len = 0;
    return list;
}

VOIDPTR alist_at(ARRLIST_PTR l, int idx)
{
    arrlist_assert(l->len - 1 >= idx && "Index outside the bounds of the array!");
    arrlist_assert(0 <= idx && "Index outside the bounds of the array!");

    return l->data[idx];
}

void alist_append(ARRLIST_PTR l, VOIDPTR d)
{
    // the len is larger then the cap, realloc and give more space
    if (l->len + 1 > l->cap)
    {
        int newCap = (l->cap * 2) + l->cap;

        VOIDPTR *more = realloc(l->data, sizeof(VOIDPTR) * newCap);
        if (more == NULL)
            abort();

        l->data = more;
        l->cap = newCap;
    }

    l->data[l->len] = d;
    l->len = l->len + 1;
}

void alist_insert(ARRLIST_PTR l, int idx, VOIDPTR d)
{

    arrlist_assert(l->len - 1 >= idx && "Index outside the bounds of the array!");
    arrlist_assert(0 <= idx && "Index outside the bounds of the array!");

    if (l->len + 1 > l->cap)
    {
        int newCap = (l->cap * 2) + l->cap;

        VOIDPTR *more = realloc(l->data, sizeof(VOIDPTR) * newCap);
        if (more == NULL)
            abort();

        l->data = more;
        l->cap = newCap;
    }

    int remain = l->len - idx;
    // moving right to left
    for (size_t i = l->len; i > remain - 2; i--)
    {
        // printf("l->data[%d] = l->data[%d]\n", i, i - 1);
        l->data[i] = l->data[i - 1];
    }

    l->data[idx] = d;

    l->len = l->len + 1;
}

VOIDPTR alist_remove(ARRLIST_PTR l, int idx)
{
    arrlist_assert(l->len - 1 >= idx && "Index outside the bounds of the array!");
    arrlist_assert(0 <= idx && "Index outside the bounds of the array!");

    VOIDPTR ret = l->data[idx];

    l->data[idx] = NULL;

    // shift the array down
    int remain = l->len - idx;
    for (size_t i = idx; i < remain + 1; i++)
    {
        // printf("l->data[%d] = l->data[%d]\n", i, i + 1);
        l->data[i] = l->data[i + 1];
    }

    l->data[l->len - 1] = NULL;

    l->len = l->len - 1;

    return ret;
}
