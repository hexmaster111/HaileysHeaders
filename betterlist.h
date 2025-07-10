
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#define ListDef(TYPE)                                                     \
    typedef struct ListOf##TYPE                                           \
    {                                                                     \
        TYPE *items;                                                      \
        ssize_t count, cap;                                               \
    } ListOf##TYPE;                                                       \
    void TYPE##_ListPush(ListOf##TYPE *list, TYPE item);                  \
    TYPE TYPE##_ListPop(ListOf##TYPE *list);                              \
    void TYPE##_ListRemoveAt(ListOf##TYPE *list, size_t idx);             \
    void TYPE##_ListInsertAt(ListOf##TYPE *list, size_t idx, TYPE value); \
    size_t TYPE##_ListLength(ListOf##TYPE *list);                         \
    void TYPE##_ListFree(ListOf##TYPE *list);                             \
    TYPE *TYPE##_ListAt(ListOf##TYPE *list, size_t idx)

#define ListImpl(TYPE)                                                            \
    void TYPE##_ListPush(ListOf##TYPE *list, TYPE item)                           \
    {                                                                             \
        if (list->count >= list->cap)                                             \
        {                                                                         \
            list->cap = list->cap ? list->cap * 2 : 4;                            \
            list->items = (TYPE *)realloc(list->items, list->cap * sizeof(TYPE)); \
            assert(list->items);                                                  \
        }                                                                         \
        list->items[list->count++] = item;                                        \
    }                                                                             \
                                                                                  \
    TYPE TYPE##_ListPop(ListOf##TYPE *list)                                       \
    {                                                                             \
        assert(list->count > 0);                                                  \
        return list->items[--list->count];                                        \
    }                                                                             \
                                                                                  \
    void TYPE##_ListRemoveAt(ListOf##TYPE *list, size_t idx)                      \
    {                                                                             \
        assert(idx < list->count);                                                \
        for (size_t i = idx; i < list->count - 1; ++i)                            \
        {                                                                         \
            list->items[i] = list->items[i + 1];                                  \
        }                                                                         \
        --list->count;                                                            \
    }                                                                             \
                                                                                  \
    void TYPE##_ListInsertAt(ListOf##TYPE *list, size_t idx, TYPE value)          \
    {                                                                             \
        assert(idx <= list->count);                                               \
        if (list->count >= list->cap)                                             \
        {                                                                         \
            list->cap = list->cap ? list->cap * 2 : 4;                            \
            list->items = (TYPE *)realloc(list->items, list->cap * sizeof(TYPE)); \
            assert(list->items);                                                  \
        }                                                                         \
        for (size_t i = list->count; i > idx; --i)                                \
        {                                                                         \
            list->items[i] = list->items[i - 1];                                  \
        }                                                                         \
        list->items[idx] = value;                                                 \
        ++list->count;                                                            \
    }                                                                             \
                                                                                  \
    size_t TYPE##_ListLength(ListOf##TYPE *list)                                  \
    {                                                                             \
        return list->count;                                                       \
    }                                                                             \
                                                                                  \
    void TYPE##_ListFree(ListOf##TYPE *list)                                      \
    {                                                                             \
        free(list->items);                                                        \
        list->items = NULL;                                                       \
        list->count = list->cap = 0;                                              \
    }                                                                             \
    TYPE *TYPE##_ListAt(ListOf##TYPE *list, size_t idx)                           \
    {                                                                             \
        assert(idx < list->count);                                                \
        return &list->items[idx];                                                 \
    }                                                                             \
    TYPE *TYPE##_ListAtRef(ListOf##TYPE *list, size_t idx)                        \
    {                                                                             \
        assert(idx < list->count);                                                \
        return &list->items[idx];                                                 \
    }

#ifdef LIST_TESTS
ListDef(int);
ListImpl(int);

void test_list()
{

    // Test 1: Push elements into the list
    ListOfint list = {NULL, 0, 0};
    int_ListPush(&list, 10);
    int_ListPush(&list, 20);
    int_ListPush(&list, 30);
    assert(int_ListLength(&list) == 3);
    assert(list.items[0] == 10);
    assert(list.items[1] == 20);
    assert(list.items[2] == 30);

    // Test 2: Remove an element at index 1
    int_ListRemoveAt(&list, 1);
    assert(int_ListLength(&list) == 2);
    assert(list.items[0] == 10);
    assert(list.items[1] == 30);

    // Test 3: Insert an element at index 1
    int_ListInsertAt(&list, 1, 25);
    assert(int_ListLength(&list) == 3);
    assert(list.items[0] == 10);
    assert(list.items[1] == 25);
    assert(list.items[2] == 30);

    // Test 4: Pop an element from the list
    int popped = int_ListPop(&list);
    assert(popped == 30);
    assert(int_ListLength(&list) == 2);
    assert(list.items[0] == 10);
    assert(list.items[1] == 25);

    // Test 5: Free the list
    int_ListFree(&list);
    assert(list.items == NULL);
    assert(list.count == 0);
    assert(list.cap == 0);

    printf("All tests passed successfully!\n");
}
#endif // LIST_TESTS