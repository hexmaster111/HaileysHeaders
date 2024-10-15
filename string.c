
#include "string.h"

#include <string.h>
#include <stdarg.h>

void string_test()
{
    STRING s = {0};
    string_append(&s, "string", strlen("string"));

    assert(memcmp("string", s.buf, strlen("string")) == 0);
    assert(s.len == strlen("string"));
    string_free(&s);

    string_appendfmt(&s, "CHAR: %c", 's');
    assert(memcmp("CHAR: s", s.buf, strlen("CHAR: s")) == 0);
    assert(s.len == strlen("CHAR: s"));
    string_free(&s);

    string_appendfmt(&s, "%c", 't');
    string_appendfmt(&s, "%c", 'e');
    string_appendfmt(&s, "%c", 's');
    string_appendfmt(&s, "%c", 't');
    string_appendfmt(&s, "%c", ' ');
    string_appendfmt(&s, "%c", '1');
    string_appendfmt(&s, "%c", '2');
    string_appendfmt(&s, "%c", '3');

    assert(s.len == strlen("test 123"));
    assert(memcmp("test 123", s.buf, strlen("test 123")) == 0);
    string_free(&s);
}

void string_append(STRINGPTR wb, const char *s, int len)
{
    char *new = realloc(wb->buf, wb->len + len);

    if (new == NULL)
        return;

    memcpy(new + wb->len, s, len);
    wb->buf = new;
    wb->len += len;
}

void string_appendfmt(STRINGPTR wb, const char *fmt, ...)
{
    va_list ap;
    int n = 0;
    size_t size = 0;
    char *p = NULL;

    /* determin required size */
    va_start(ap, fmt);
    n = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (n < 0)
        return; /* nothing todo, there where no bytes to add */

    size = (size_t)n + 1;

    char *new = realloc(wb->buf, wb->len + size);
    if (new == NULL)
        exit(20); /* error getting more space! */

    va_start(ap, fmt);
    vsnprintf(new + wb->len, size, fmt, ap);
    va_end(ap);

    wb->len += size - 1;
    wb->buf = new;
}

void string_free(STRINGPTR wb)
{
    free(wb->buf);
    wb->buf = NULL;
    wb->len = 0;
}
