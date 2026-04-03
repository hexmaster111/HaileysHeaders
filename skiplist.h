#include <stdio.h>

#ifndef SKIPLIST_H
#define SKIPLIST_H

#ifndef SKIPLIST_MALLOC
#include <stdlib.h>
#define SKIPLIST_MALLOC (malloc)
#define SKIPLIST_FREE (free)
#endif

#ifndef SKIPLIST_MEMSET
#include <string.h>
#define SKIPLIST_MEMSET (memset)
#endif

/*
layers 3] -------------------------------------------------- [ NULL
layers 2] --------------------------------------- [3] ------ [ NULL
layers 1] ----------------- [1] ----------------- [3] ------ [ NULL
layers 0] ------ [0] ------ [1] ------ [2] ------ [3] ------ [ NULL
*/

/*
layers 0] -------------------------------------------------- [ NULL
*/

typedef const char *(SkipList_ToString)(void *);
typedef int(SkipList_Compare)(void *, void *);

struct SkipListNode
{
    struct SkipListNode *next;
    struct SkipListNode *down;

    void *data;
};

struct SkipList
{
    size_t size;
    struct SkipListNode *top;

    /*
        neg if a gtr b
        pos if a les b
        0 if same
    */
    SkipList_Compare *compare;
};

typedef struct SkipListNode SkipListNode;
typedef struct SkipList SkipList;

int SkipList_Contains(SkipList *l, void *data);
void SkipList_Print(SkipList *l, SkipList_ToString tostring);
void SkipList_Init(SkipList *l, SkipList_Compare compare);
void SkipList_Insert(SkipList *l, void *data);
void SkipList_Remove(SkipList *l, void *data);

SkipListNode *SkipListNode_New(void *data, SkipListNode *next, SkipListNode *down);

#endif // SKIPLIST_H