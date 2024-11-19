#ifndef VEC_H
#define VEC_H

/*-------------usage--------------

    vec(int) ints = vec_init();
    vec_push(&ints, 1);
    vec_push(&ints, 2);
    vec_push(&ints, 3);
    vec_push(&ints, 4);

    int x, i;
    vec_foreach(&ints, x, i)
    {
        printf("[%d] : \'%d\'\n", i, x);
    }

    vec_free(&ints);

*/


#define vec(T)        \
    struct            \
    {                 \
        T *buf;       \
        int len, cap; \
    }

#define vec_init() {NULL, 0, 0}
#define vec_len(v) ((v)->len)
#define vec_at(v, i) ((v)->buf[i])

static int vec_expand(void **buf, int *len, int *cap, int mbsz)
{
    if (*len + 1 > *cap)
    {
        void *ptr;
        int n = (*cap == 0) ? 1 : *cap * 2;
        ptr = realloc(*buf, n * mbsz);
        if (!ptr)
            return -1; // Alloc failed!

        *buf = ptr;
        *cap = n;
    }
    return 0;
}

#define vec_unpack(v) (void **)&(v)->buf, &(v)->len, &(v)->cap, sizeof(*(v)->buf)
#define vec_push(v, val) vec_expand(vec_unpack(v)) ? -1 : ((v)->buf[(v)->len++] = (val), 0)
#define vec_pop(v) ((v)->buf[--(v)->len])
#define vec_peek(v) ((v)->buf[(v)->len - 1])
#define vec_free(v) (free((v)->buf), (v)->buf = NULL, (v)->len = 0, (v)->cap = 0)
#define vec_foreach(v, var, iter) \
    if ((v)->len > 0)             \
        for ((iter) = 0; (iter) < (v)->len && ((var) = (v)->buf[(iter)], 1); ++(iter))

#endif // VEC_H