#ifndef H_STRING_H
#define H_STRING_H
typedef struct STRING
{
    char *buf;
    int len;
} STRING, *STRINGPTR;

void string_append(STRINGPTR w, const char *s, int len);
void string_appendfmt(STRINGPTR w, const char *fmt, ...);
void string_free(STRINGPTR w);
#endif // H_STRING_H