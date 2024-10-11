#include <stdlib.h>

#ifndef LIST_H
// header
#define LIST_H

typedef void *VOIDPTR;

typedef struct ARRAY_LIST
{
    size_t cap, len;
    VOIDPTR *data;
} ARRAY_LIST, *ARRLIST_PTR;

void alist_init(ARRLIST_PTR l);
void alist_deinit(ARRLIST_PTR l);
void alist_free(ARRLIST_PTR l);
ARRLIST_PTR alist_new();

/// @brief returns the value of the <l> at <idx>
VOIDPTR alist_at(ARRLIST_PTR l, int idx);

/// @brief preprends <d> to the end of <l>
void alist_append(ARRLIST_PTR l, VOIDPTR d);

/// @brief inserts <d> into <l> at <idx>
void alist_insert(ARRLIST_PTR l, int idx, VOIDPTR d);

/// @brief removes the element from <l> at <idx>
VOIDPTR alist_remove(ARRLIST_PTR l, int idx);

// GENERIC DATA TYPE LIST
#ifdef HH_GENERIC_LIST
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
        l->data = malloc(sizeof(_LIST_TYPE));                                          \
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
#endif // HH_GENERIC_LIST

#endif // LIST_H
