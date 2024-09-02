#include <stdio.h>    //assert
#include <stdlib.h>   //assert
#include <execinfo.h> //assert

#ifndef LIST_H
// header
#define LIST_H

// GENERIC DATA TYPE LIST

#define LIST_OF(_LIST_TYPE)                                            \
    typedef struct _LIST_TYPE##_LIST                                   \
    {                                                                  \
        size_t cap, len;                                               \
        _LIST_TYPE *data;                                              \
    } _LIST_TYPE##_LIST, *_LIST_TYPE##_LIST_PTR;                       \
    void _LIST_TYPE##_list_init(_LIST_TYPE##_LIST_PTR l);              \
    void _LIST_TYPE##_list_deinit(_LIST_TYPE##_LIST_PTR l);            \
    void _LIST_TYPE##_list_free(_LIST_TYPE##_LIST_PTR l);              \
    _LIST_TYPE##_LIST_PTR _LIST_TYPE##_list_new();                     \
    _LIST_TYPE _LIST_TYPE##_list_at(_LIST_TYPE##_LIST_PTR l, int idx); \
    void _LIST_TYPE##_list_add(_LIST_TYPE##_LIST_PTR l, _LIST_TYPE d);

#define IMPL_LIST_OF(_LIST_TYPE)                                                       \
    void _LIST_TYPE##_list_init(_LIST_TYPE##_LIST_PTR l)                               \
    {                                                                                  \
        l->data = malloc(sizeof(_LIST_TYPE));                                             \
        l->cap = 1;                                                                    \
        l->len = 0;                                                                    \
    }                                                                                  \
    void _LIST_TYPE##_list_deinit(_LIST_TYPE##_LIST_PTR l)                             \
    {                                                                                  \
        free(l->data);                                                                 \
        l->cap = 0;                                                                    \
        l->data = NULL;                                                                \
        l->len = 0;                                                                    \
    }                                                                                  \
    void _LIST_TYPE##_list_free(_LIST_TYPE##_LIST_PTR l)                               \
    {                                                                                  \
        free(l->data);                                                                 \
        free(l);                                                                       \
    }                                                                                  \
    _LIST_TYPE##_LIST_PTR _LIST_TYPE##_list_new()                                      \
    {                                                                                  \
        _LIST_TYPE##_LIST_PTR list = malloc(sizeof(_LIST_TYPE##_LIST));                \
        list->data = malloc(sizeof(_LIST_TYPE));                                       \
        list->cap = 1;                                                                 \
        list->len = 0;                                                                 \
        return list;                                                                   \
    }                                                                                  \
    _LIST_TYPE _LIST_TYPE##_list_at(_LIST_TYPE##_LIST_PTR l, int idx)                  \
    {                                                                                  \
        assert(l->len - 1 >= (size_t)idx && "Index outside the bounds of the array!"); \
        assert(0 <= (size_t)idx && "Index outside the bounds of the array!");          \
        return l->data[idx];                                                           \
    }                                                                                  \
    void _LIST_TYPE##_list_add(_LIST_TYPE##_LIST_PTR l, _LIST_TYPE d)                  \
    {                                                                                  \
        if (l->len + 1 > l->cap)                                                       \
        {                                                                              \
            size_t newCap = (l->cap * 2) + l->cap;                                     \
                                                                                       \
            _LIST_TYPE *more = realloc(l->data, sizeof(_LIST_TYPE) * newCap);          \
            if (more == NULL)                                                          \
                abort();                                                               \
                                                                                       \
            l->data = more;                                                            \
            l->cap = newCap;                                                           \
        }                                                                              \
                                                                                       \
        l->data[l->len] = d;                                                           \
        l->len = l->len + 1;                                                           \
    }                                                                                  \
    void _LIST_TYPE##_list_insert_at(_LIST_TYPE##_LIST_PTR l, int idx, _LIST_TYPE d)   \
    {                                                                                  \
                                                                                       \
        assert(l->len - 1 >= (size_t)idx && "Index outside the bounds of the array!"); \
        assert(0 <= (size_t)idx && "Index outside the bounds of the array!");          \
                                                                                       \
        if (l->len + 1 > l->cap)                                                       \
        {                                                                              \
            size_t newCap = (l->cap * 2) + l->cap;                                     \
                                                                                       \
            _LIST_TYPE *more = realloc(l->data, sizeof(_LIST_TYPE) * newCap);          \
            if (more == NULL)                                                          \
                abort();                                                               \
                                                                                       \
            l->data = more;                                                            \
            l->cap = newCap;                                                           \
        }                                                                              \
                                                                                       \
        size_t remain = l->len - idx;                                                  \
        for (size_t i = l->len; i > remain - 2; i--)                                   \
        {                                                                              \
            l->data[i] = l->data[i - 1];                                               \
        }                                                                              \
                                                                                       \
        l->data[idx] = d;                                                              \
                                                                                       \
        l->len = l->len + 1;                                                           \
    }



typedef void *VOIDPTR;

typedef struct LIST
{
    size_t cap, len;
    VOIDPTR *data;
} LIST, *LISTPTR;

void list_init(LISTPTR l);
void list_deinit(LISTPTR l);
void list_free(LISTPTR l);
LISTPTR list_new();
VOIDPTR list_at(LISTPTR l, int idx);
void list_add(LISTPTR l, VOIDPTR d);
void list_insert_at(LISTPTR l, int idx, VOIDPTR d);
VOIDPTR list_remove_at(LISTPTR l, int idx);
void list_unit_test();
#endif // LIST_H

// c file
#ifdef HLISTIMPL
void list__assert(bool cond, const char *str, int line, const char *file)
{
    if (cond)
        return;
    fprintf(stderr, "ASSERT FAIL: %s, %s:%d\nbacktrace:\n", str, file, line);
    void *callstack[128];
    int i, frames = backtrace(callstack, 128);
    char **strs = backtrace_symbols(callstack, frames);
    for (i = 0; i < frames; ++i)
    {
        printf("%s\n", strs[i]);
    }
    free(strs);
    abort();
}
#define list_assert(cond) list__assert(cond, #cond, __LINE__, __FILE__)

void list_init(LISTPTR l)
{
    l->data = malloc(sizeof(VOIDPTR));
    l->cap = 1;
    l->len = 0;
}
void list_deinit(LISTPTR l)
{
    free(l->data);
    l->cap = 0;
    l->data = NULL;
    l->len = 0;
}

void list_free(LISTPTR l)
{
    free(l->data);
    free(l);
}
LISTPTR list_new()
{
    LISTPTR list = malloc(sizeof(LIST));
    list->data = malloc(sizeof(VOIDPTR));
    list->cap = 1;
    list->len = 0;
    return list;
}

VOIDPTR list_at(LISTPTR l, int idx)
{
    list_assert(l->len - 1 >= idx && "Index outside the bounds of the array!");
    list_assert(0 <= idx && "Index outside the bounds of the array!");

    return l->data[idx];
}

void list_add(LISTPTR l, VOIDPTR d)
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

void list_insert_at(LISTPTR l, int idx, VOIDPTR d)
{

    list_assert(l->len - 1 >= idx && "Index outside the bounds of the array!");
    list_assert(0 <= idx && "Index outside the bounds of the array!");

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

VOIDPTR list_remove_at(LISTPTR l, int idx)
{
    list_assert(l->len - 1 >= idx && "Index outside the bounds of the array!");
    list_assert(0 <= idx && "Index outside the bounds of the array!");

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

void list_unit_test()
{
    LISTPTR l = list_new();

    list_add(l, (void *)1);
    list_add(l, (void *)2);
    list_add(l, (void *)3);
    list_add(l, (void *)4);
    list_add(l, (void *)5);

    list_assert(l->len == 5);

    list_assert(l->data[0] == (void *)1);
    list_assert(l->data[1] == (void *)2);
    list_assert(l->data[2] == (void *)3);
    list_assert(l->data[3] == (void *)4);
    list_assert(l->data[4] == (void *)5);

    list_assert(list_at(l, 0) == (void *)1);
    list_assert(list_at(l, 1) == (void *)2);
    list_assert(list_at(l, 2) == (void *)3);
    list_assert(list_at(l, 3) == (void *)4);
    list_assert(list_at(l, 4) == (void *)5);

    list_assert(list_remove_at(l, 2) == (void *)3);
    list_assert(l->data[4] == NULL);

    list_assert(l->len == 4);

    list_assert(l->data[0] == (void *)1);
    list_assert(l->data[1] == (void *)2);
    list_assert(l->data[2] == (void *)4);
    list_assert(l->data[3] == (void *)5);

    list_insert_at(l, 1, (void *)9);

    list_assert(l->len == 5);

    list_assert(l->data[0] == (void *)1);
    list_assert(l->data[1] == (void *)9);
    list_assert(l->data[2] == (void *)2);
    list_assert(l->data[3] == (void *)4);
    list_assert(l->data[4] == (void *)5);

    list_free(l);
}
#endif // HLISTIMPL