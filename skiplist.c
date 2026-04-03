#include "skiplist.h"

SkipListNode *SkipList_SlideRight(SkipList *l, SkipListNode *now, void *data);

SkipListNode *SkipListNode_New(void *data, SkipListNode *next, SkipListNode *down)
{
    SkipListNode *ret = SKIPLIST_MALLOC(sizeof(SkipListNode));
    SKIPLIST_MEMSET(ret, 0, sizeof(SkipListNode));

    ret->data = data;
    ret->next = next;
    ret->down = down;
    return ret;
}

int SkipList_5050() { return rand() % 2; }

SkipListNode *SkipList_AddLink(SkipList *l, SkipListNode *now, void *data)
{
    SkipListNode *new = 0, *down = 0;

    if (now->down == 0)
    {
        new = SkipListNode_New(data, now->next, 0);
        now->next = new;
    }
    else
    {
        down = SkipList_AddLink(l, SkipList_SlideRight(l, now->down, data), data);
        if (down != 0 && SkipList_5050())
        {
            new = SkipListNode_New(data, now->next, down);
            now->next = new;
        }
    }

    return new;
}

SkipListNode *SkipList_SlideRight(SkipList *l, SkipListNode *now, void *data)
{
    while ((now->next != 0) && (0 < l->compare(now->next->data, data)))
        now = now->next;
    return now;
}

void SkipList_Insert(SkipList *l, void *data)
{
    SkipListNode *down = 0, *new = 0;
    down = SkipList_AddLink(l, SkipList_SlideRight(l, l->top, data), data);

    if (down != 0 && SkipList_5050())
    {
        SkipListNode *newTop = SKIPLIST_MALLOC(sizeof(SkipListNode));
        SKIPLIST_MEMSET(newTop, 0, sizeof(SkipListNode));

        new = SkipListNode_New(data, 0, down);
        newTop->next = new;
        newTop->down = l->top;
        l->top = newTop;
    }

    l->size++;
}

void SkipList_Remove(SkipList *l, void *data)
{
    SkipListNode *now, *temp;
    now = l->top;

    while (now)
    {
        now = SkipList_SlideRight(l, now, data);
        if ((now->next != 0) && (l->compare(now->next->data, data) == 0))
        {
            temp = now->next;
            now->next = now->next->next;
            SKIPLIST_FREE(temp);
            if (now->down == NULL)
            {
                l->size--;
            }
        }
        now = now->down;
    }
}

void SkipList_Init(SkipList *l, SkipList_Compare compare)
{
    l->size = 0;
    l->compare = compare;
    l->top = SkipListNode_New(0, 0, 0);
}

void SkipList_Print(SkipList *l, SkipList_ToString tostring)
{
    SkipListNode *list = l->top, *node = 0;

    while (list != 0)
    {
        node = list->next;
        while (node != 0)
        {
            printf("%s    ", tostring(node->data));

            node = node->next;
        }
        list = list->down;
        printf("\n");
    }
}

int SkipList_Contains(SkipList *l, void *data)
{
    SkipListNode *now = l->top;

    while (now)
    {
        now = SkipList_SlideRight(l, now, data);
        if ((now->next != 0) && (l->compare(now->next->data, data) == 0))
            return 1;
        now = now->down;
    }
    return 0;
}