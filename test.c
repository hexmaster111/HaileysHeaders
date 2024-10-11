#include <stdio.h>
#include <assert.h>
#include "array_list.h"
#include "linked_list.h"

// clear; rm test; cc array_list.c linked_list.c test.c -otest; valgrind ./test

void arrlist_unit_test()
{
    ARRLIST_PTR l = alist_new();

    alist_append(l, (void *)1);
    alist_append(l, (void *)2);
    alist_append(l, (void *)3);
    alist_append(l, (void *)4);
    alist_append(l, (void *)5);

    assert(l->len == 5);

    assert(l->data[0] == (void *)1);
    assert(l->data[1] == (void *)2);
    assert(l->data[2] == (void *)3);
    assert(l->data[3] == (void *)4);
    assert(l->data[4] == (void *)5);

    assert(alist_at(l, 0) == (void *)1);
    assert(alist_at(l, 1) == (void *)2);
    assert(alist_at(l, 2) == (void *)3);
    assert(alist_at(l, 3) == (void *)4);
    assert(alist_at(l, 4) == (void *)5);

    assert(alist_remove(l, 2) == (void *)3);
    assert(l->data[4] == NULL);

    assert(l->len == 4);

    assert(l->data[0] == (void *)1);
    assert(l->data[1] == (void *)2);
    assert(l->data[2] == (void *)4);
    assert(l->data[3] == (void *)5);

    alist_insert(l, 1, (void *)9);

    assert(l->len == 5);

    assert(l->data[0] == (void *)1);
    assert(l->data[1] == (void *)9);
    assert(l->data[2] == (void *)2);
    assert(l->data[3] == (void *)4);
    assert(l->data[4] == (void *)5);

    alist_free(l);
    printf("array_list tests passed\n");
}

// =========== LINKED LIST TESTS ================= //
void *itterated_values[4] = {0};
int ip = 0;

void llist_itterate_test(void *i)
{
    itterated_values[ip] = i;
    ip++;
}

void llist_basic_intigration_test()
{
    LLIST l = {0};

    assert(llist_is_empty(&l) == 1 && "llist is empty");
    assert(l.len == 0);

    llist_append(&l, (void *)1);
    assert(l.len == 1);
    assert(llist_is_empty(&l) == 0 && "llist is not empty");

    llist_append(&l, (void *)2);
    assert(l.len == 2);

    llist_append(&l, (void *)3);
    assert(l.len == 3);
    assert(llist_at(&l, 0) == (void *)1);
    assert(llist_at(&l, 1) == (void *)2);
    assert(llist_at(&l, 2) == (void *)3);

    llist_prepend(&l, (void *)-1);
    assert(l.len == 4);
    assert(llist_at(&l, 0) == (void *)-1);
    assert(llist_at(&l, 1) == (void *)1);
    assert(llist_at(&l, 2) == (void *)2);
    assert(llist_at(&l, 3) == (void *)3);

    llist_insert(&l, (void *)6, 3);
    assert(l.len == 5);
    assert(llist_at(&l, 0) == (void *)-1);
    assert(llist_at(&l, 1) == (void *)1);
    assert(llist_at(&l, 2) == (void *)2);
    assert(llist_at(&l, 3) == (void *)6);
    assert(llist_at(&l, 4) == (void *)3);

    llist_insert(&l, (void *)7, 4);
    assert(l.len == 6);
    assert(llist_at(&l, 0) == (void *)-1);
    assert(llist_at(&l, 1) == (void *)1);
    assert(llist_at(&l, 2) == (void *)2);
    assert(llist_at(&l, 3) == (void *)6);
    assert(llist_at(&l, 4) == (void *)7);
    assert(llist_at(&l, 5) == (void *)3);

    llist_remove(&l, 3);
    assert(l.len == 5);
    assert(llist_at(&l, 0) == (void *)-1);
    assert(llist_at(&l, 1) == (void *)1);
    assert(llist_at(&l, 2) == (void *)2);
    assert(llist_at(&l, 3) == (void *)7);
    assert(llist_at(&l, 4) == (void *)3);

    llist_remove(&l, 0);
    assert(l.len == 4);
    assert(llist_at(&l, 0) == (void *)1);
    assert(llist_at(&l, 1) == (void *)2);
    assert(llist_at(&l, 2) == (void *)7);
    assert(llist_at(&l, 3) == (void *)3);

    llist_swap(&l, 1, 2);
    assert(llist_at(&l, 0) == (void *)1);
    assert(llist_at(&l, 1) == (void *)7);
    assert(llist_at(&l, 2) == (void *)2);
    assert(llist_at(&l, 3) == (void *)3);

    llist_swap(&l, 0, 1);
    assert(llist_at(&l, 0) == (void *)7);
    assert(llist_at(&l, 1) == (void *)1);
    assert(llist_at(&l, 2) == (void *)2);
    assert(llist_at(&l, 3) == (void *)3);

    llist_swap(&l, 0, 3);
    assert(llist_at(&l, 0) == (void *)3);
    assert(llist_at(&l, 1) == (void *)1);
    assert(llist_at(&l, 2) == (void *)2);
    assert(llist_at(&l, 3) == (void *)7);

    llist_itterate(&l, llist_itterate_test);

    assert(itterated_values[0] == (void *)3);
    assert(itterated_values[1] == (void *)1);
    assert(itterated_values[2] == (void *)2);
    assert(itterated_values[3] == (void *)7);

    printf("llist_basic_intigration_test passed\n");

    llist_free(&l);
}

int comparator(void *a, void *b) { return (int)a - (int)b; }

void llist_basic_intigration_test_sorting()
{
    LLIST l = {0};
    llist_append(&l, (void *)5);
    llist_append(&l, (void *)2);
    llist_append(&l, (void *)7);
    llist_append(&l, (void *)32);
    llist_append(&l, (void *)8);
    llist_append(&l, (void *)5);
    llist_append(&l, (void *)3);
    llist_append(&l, (void *)33);

    llist_sort(&l, comparator);

    assert(llist_at(&l, 0) == (void *)2);
    assert(llist_at(&l, 1) == (void *)3);
    assert(llist_at(&l, 2) == (void *)5);
    assert(llist_at(&l, 3) == (void *)5);
    assert(llist_at(&l, 4) == (void *)7);
    assert(llist_at(&l, 5) == (void *)8);
    assert(llist_at(&l, 6) == (void *)32);
    assert(llist_at(&l, 7) == (void *)33);

    llist_free(&l);

    printf("llist_basic_intigration_test_sorting passed\n");
}

int main()
{
    arrlist_unit_test();
    llist_basic_intigration_test();
    llist_basic_intigration_test_sorting();
}