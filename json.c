#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#define HH_GENERIC_LIST
#include "array_list.h"
#include "json.h"



#include <execinfo.h>
#include <stdio.h>

#define MAX_BACKTRACE_LINES 128

// ============== basic buffer

typedef struct STRING
{
    char *buf;
    int len;
} STRING, *STRINGPTR;

void string_append(STRINGPTR w, const char *s, int len);
void string_appendfmt(STRINGPTR w, const char *fmt, ...);
void string_free(STRINGPTR w);

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
// ========= JSON STUFF

typedef struct JOBJ JOBJ, *JOBJPTR;
typedef struct JARR JARR, *JARRPTR;

/// @brief parses a json object from the given <string> returns NULL on error
JOBJPTR jobj_from(const char *str);
void jobj_free(JOBJPTR t);

typedef struct JOBJ_FIELD_VALUE
{
    union
    {
        STRING str;
        double number;
        JOBJPTR obj;
        JARRPTR arr;
    } value;
    /*
     *  0 str
     *  1 number
     *  2 obj
     *  3 arr
     */
    int valueKind;
} JOBJ_FIELD_VALUE;

typedef struct JOBJ_FIELD
{
    STRING fieldName;
    JOBJ_FIELD_VALUE value;
} JOBJ_FIELD, *JOBJ_FIELD_PTR;

LIST_OF(JOBJ_FIELD)
IMPL_LIST_OF(JOBJ_FIELD)

LIST_OF(JOBJ_FIELD_VALUE)
IMPL_LIST_OF(JOBJ_FIELD_VALUE)

typedef struct JARR
{
    JOBJ_FIELD_VALUE_LIST values;
} JARR, *JARRPTR;

typedef struct JOBJ
{
    JOBJ_FIELD_LIST fields;
} JOBJ, *JOBJPTR;

void error_report(const char *ch)
{
    fprintf(stderr, "error near %s\n", ch);

    exit(1);
}



void _js_read_string(const char *in, STRING *out, int *out_chars_consumed)
{
    int i = 0, c, excape = 0;
    int len = strlen(in);
    while (1)
    {
        c = in[i];
        i++;
        if (i >= len)
            error_report("expected \" got EOF");

        if (c == '\\')
        {
            // this is an escaped char, we do something wacky
            excape = 1;
        }
        else if (c == '"' && excape == 0)
        {
            // end of string
            break;
        }
        else if (excape)
        {
#define APPEND_CHAR(CHARTOINS) string_appendfmt(out, "%c", CHARTOINS)
            if (c == 'n')
                APPEND_CHAR('\n');
            else if (c == 'b')
                APPEND_CHAR('\b');
            else if (c == 'f')
                APPEND_CHAR('\f');
            else if (c == 'r')
                APPEND_CHAR('\r');
            else if (c == 't')
                APPEND_CHAR('\t');
            else if (c == '\\')
                APPEND_CHAR('\\');
            else if (c == '\'')
                APPEND_CHAR('\'');
            else if (c == '"')
                APPEND_CHAR('\"');
            else
                error_report(in);
            excape = 0;
        }
        else
        {
            APPEND_CHAR(c);
        }
    }

    if (excape)
        error_report(in);

    *out_chars_consumed = i;

#undef APPEND_CHAR
}

void _js_read_value(const char *in, JOBJ_FIELD_VALUE *out, int *out_chars_consumed)
{

    int i = 0, c, excape = 0;
    int len = strlen(in);
    while (1)
    {
        c = in[i];
        i++;
        if (i >= len)
            error_report("expected \" got EOF READ VALUE");
        else if (isspace(c))
            i++;
        else if (c == '\"')
        {
            i++;
            int str_chars = -1;
            _js_read_string(in + i, &out->value.str, &str_chars);
            if (str_chars == -1)
                error_report("ERROR READING VALUE");
            i += str_chars;
        }
    }

    *out_chars_consumed = i;
}

JOBJ_FIELD_PTR _jj_field(const char *str, int *charsconsumed)
{
    JOBJ_FIELD_PTR ret = malloc(sizeof(JOBJ_FIELD));
    memset(ret, 0,sizeof(JOBJ_FIELD));

    int i = 0, c, got_field_name = 0;
    while (1)
    {
        c = str[i];

        if (isspace(c))
        {
            i++;
        }
        else if (got_field_name)
        {
            if (c != ':')
                error_report(str + i);

            int charsUsed = -1;
            _js_read_value(str + i, &ret->value, &charsUsed);
            i += charsUsed;
        }
        else if (c == '"')
        {
            i++; // dont give it the starting "
            int charsUsed = -1;
            _js_read_string(str + i, &ret->fieldName, &charsUsed);
            i += charsUsed;
            got_field_name = 1;
        }
    }
}

void jobj_free(JOBJPTR j)
{
}

JOBJPTR jobj_from(const char *str)
{
    JOBJPTR ret = malloc(sizeof(JOBJ));
    memset(ret, 0,sizeof(JOBJ));

    int i = 0, c;

    int obj_start = 0;

    while (1)
    {
        c = str[i];
        if (isspace(c))
        {
            i++;
            continue;
        }
        if (c == '{')
        {
            if (obj_start)
                goto ERROR;
            obj_start = 1;
            i++;
            continue;
        }
        else if (c == '\"')
        {
            if (!obj_start)
                goto ERROR;

            int out_offset = -1;
            JOBJ_FIELD_PTR f = _jj_field(str + i, &out_offset);
            if (f == NULL)
                goto ERROR;

            i += out_offset;
            // TODO: check for , or } for what todo next <int parsed field = 1?>
            continue;
        }
    }

    return ret;
ERROR:
    fprintf(stderr, "%d, %c\n", i, c);
    jobj_free(ret);
    return NULL;
}

int main()
{
    string_test();
    const char *json = "{\n"
                       "\"string\" : \"string value\",\n"
                       "\"number\" : 5050.2410,\n"
                       "\"object\" : {},\n"
                       "\"arrayOfNumbers\" : [0,1,2,3,4],\n"
                       "\"arrayOfStrings\" : [\"A\",\"B\",\"C\"],\n"
                       "\"arrayOfObject\"  : [{},{},{}]\n"
                       "}\n";

    JOBJPTR s = jobj_from(json);

    if (s == NULL)
    {
        fprintf(stderr, "Error parsing object\n");
        exit(1);
    }
}