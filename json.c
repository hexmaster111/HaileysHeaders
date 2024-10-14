#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#define HH_GENERIC_LIST
#include "array_list.h"
#include "array_list.c"
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

typedef enum JOBJ_FIELD_KIND
{
    JFK_STR,
    JFK_NUM,
    JFK_OBJ,
    JFK_ARR
} JOBJ_FIELD_KIND;

typedef struct JOBJ_FIELD_VALUE
{
    union
    {
        STRING str;
        double number;
        JOBJPTR obj;
        JARRPTR arr;
    } value;

    JOBJ_FIELD_KIND kind;
} JOBJ_FIELD_VALUE;

typedef struct JOBJ_FIELD
{
    STRING fieldName;
    JOBJ_FIELD_VALUE value;
} JOBJ_FIELD, *JOBJ_FIELD_PTR;

typedef struct JARR
{
    // list of JOBJ_FIELD_VALUE_PTRs
    ARRAY_LIST values;
} JARR, *JARRPTR;

typedef struct JOBJ
{
    // list of JOBJ_FIELD_PTR
    ARRAY_LIST fields;
} JOBJ, *JOBJPTR;

void error_report(const char *ch)
{
    fprintf(stderr, "error near %s\n", ch);

    exit(1);
}
void _js_read_number(const char *in, double *out, int *out_chars_consumed)
{
    char buff[24] = {0};

    int buffidx = 0, inidx = 0, c, single_dec = 0;

    while (1)
    {
        c = in[inidx];
        inidx++;
        if (isspace(c))
            break;
        else if (c == ',' || c == ']')
            break;
        else if (isdigit(c) || c == '.')
        {
            if (c == '.')
            {
                if (single_dec == 1)
                    error_report("INVALID NUMBER, Extra '.'");
                single_dec = 1;
            }
            buff[buffidx] = (char)c;
            buffidx++;
        }
        else
            break;
    }

    *out_chars_consumed = inidx - 1;
    char *end;
    *out = strtod(buff, &end);
    return;

ERROR:
    *out_chars_consumed = -1;
    *out = 0;
}
void _js_read_value(const char *in, JOBJ_FIELD_VALUE *out, int *out_chars_consumed);

// in is "[ {}, 1, "some", [] ]"
void _js_read_array(const char *in, JARR *out, int *out_chars_consumed)
{

    int i = 0, c;

    while (1)
    {
        c = in[i];
        if (isspace(c))
        {
            i++;
            continue;
        }

        if (c == '[' || c == '{' || isdigit(c) || c == '"')
        {
            JOBJ_FIELD_VALUE *f = malloc(sizeof(JOBJ_FIELD_VALUE));
            int obj_size = -1;
            if (!isdigit(c)) // consume the identifyer [, {, ", if its only a control char
                i++;
            _js_read_value(in + i, f, &obj_size);
            if (obj_size == -1)
                error_report("Failed to read value in array");
            i += obj_size;
            alist_append(&out->values, f);
        }
        else if (c == ',')
        {
            i++;
            continue;
        }
        else if (c == ']')
        {

            i++; // use the ]
            break;
        }
        else
            error_report("Unexpected char in array");
    }

    *out_chars_consumed = i;
}

JOBJPTR _jj_obj(const char *in, int *used);

void _js_read_object(const char *in, JOBJ *out, int *out_chars_consumed)
{
    out = _jj_obj(in, out_chars_consumed);
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
        if (i >= len)
            error_report("expected \" got EOF READ VALUE");
        else if (isspace(c))
        {
            i++;
            continue;
        }
        else if (isdigit(c)) // number value
        {
            int numChars = -1;
            _js_read_number(in + i, &out->value.number, &numChars);
            if (numChars == -1)
                error_report("ERROR READING NUMBER VALUE");
            i += numChars;
            out->kind = JFK_NUM;
            break;
        }
        else if (c == '[') // array value
        {
            int numChars = -1;
            out->value.arr = malloc(sizeof(JARR));
            _js_read_array(in, out->value.arr, &numChars);
            if (numChars == -1)
                error_report("ERROR READING array VALUE");
            i += numChars;
            out->kind = JFK_ARR;
            break;
        }
        else if (c == '{') // object value
        {
            i++;
            int numChars = -1;
            _js_read_object(in, out->value.obj, &numChars);
            if (numChars == -1)
                error_report("ERROR READING object VALUE");
            i += numChars;
            out->kind = JFK_OBJ;
            break;
        }
        else if (c == '\"') // this is a string value
        {
            i++;
            int str_chars = -1;
            _js_read_string(in + i, &out->value.str, &str_chars);
            if (str_chars == -1)
                error_report("ERROR READING STRING VALUE");
            i += str_chars;

            out->kind = JFK_STR;
            break;
        }
        else
        {
            error_report("expected obj, num, or array");
        }
    }

    *out_chars_consumed = i;
}

JOBJ_FIELD_PTR _jj_field(const char *str, int *charsconsumed)
{
    JOBJ_FIELD_PTR ret = malloc(sizeof(JOBJ_FIELD));
    memset(ret, 0, sizeof(JOBJ_FIELD));

    int i = 0, c, got_field_name = 0, got_field_name_sepprator = 0;
    while (1)
    {
        c = str[i];

        if (isspace(c))
        {
            i++;
        }
        else if (got_field_name_sepprator && (c == ':' || isspace(c)))
        {
            i++;
        }
        else if (got_field_name_sepprator)
        {
            int charsUsed = -1;
            _js_read_value(str + i, &ret->value, &charsUsed);
            if (charsUsed == -1)
                error_report("error reading value");
            i += charsUsed;
            break;
        }
        else if (got_field_name)
        {
            if (c != ':')
                error_report(str + i);

            if (got_field_name_sepprator)
                error_report(str + i);

            got_field_name_sepprator = 1;
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
    *charsconsumed = i;
    return ret;
}

void jobj_free(JOBJPTR j)
{
}

JOBJPTR _jj_obj(const char *in, int *used)
{
    JOBJPTR ret = malloc(sizeof(JOBJ));
    memset(ret, 0, sizeof(JOBJ));
    alist_init(&ret->fields);

    int i = 0, c;

    int obj_start = 0, got_field = 0;

    while (1)
    {
        c = in[i];
        if (isspace(c))
        {
            i++;
            continue;
        }
        else if (c == '}')
        {
            break;
        }
        else if (c == ',' && got_field)
        {
            got_field = 0;
            i++;
        }
        else if (c == '{')
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
            JOBJ_FIELD_PTR f = _jj_field(in + i, &out_offset);
            if (f == NULL)
                goto ERROR;

            alist_append(&ret->fields, f);
            i += out_offset;
            got_field = 1;
            // TODO: check for , or } for what todo next <int parsed field = 1?>
            continue;
        }
        else if (c == '\0')
        {
            error_report("UNEXPECTED END OF STRING");
        }
        else
        {
            // unknown char in json!
            error_report("Unknown char in jobj!");
        }
    }

    *used = i;
    return ret;

ERROR:
    fprintf(stderr, "%d, %c\n", i, c);
    jobj_free(ret);
    return NULL;
}

JOBJPTR jobj_from(const char *str)
{
    int discard = -1;
    return _jj_obj(str, &discard);
}

void regression_test_number_object()
{
    const char *jobj_num = "{\"number\" : 123.456}";
    JOBJPTR numb = jobj_from(jobj_num);
    assert(numb != NULL);
    assert(numb->fields.len == 1);
    JOBJ_FIELD_PTR f = alist_at(&numb->fields, 0);
    assert(f != NULL);
    assert(memcmp("number", f->fieldName.buf, strlen("number")) == 0 && "Field name");
    assert(f->value.kind == JFK_NUM);
    assert(f->value.value.number == 123.456 && "Number parsed");
}

void regression_test_string_object()
{
    const char *jobj_num = "{\"string\" : \"string value!\"}";
    JOBJPTR test = jobj_from(jobj_num);
    assert(test != NULL);
    assert(test->fields.len == 1);
    JOBJ_FIELD_PTR f = alist_at(&test->fields, 0);
    assert(f != NULL);
    assert(memcmp("string", f->fieldName.buf, strlen("string")) == 0 && "Field name");
    assert(f->value.kind == JFK_STR);
    assert(memcmp("string value!", f->value.value.str.buf, strlen("string value!")) == 0 && "string value");
}

void regression_test_string_number_object()
{
    const char *testjson = "{\"string\":\"string val\", \"number\":123.456}";
    JOBJPTR testobj = jobj_from(testjson);
    assert(testobj != NULL);
    assert(testobj->fields.len == 2);

    JOBJ_FIELD_PTR f0 = alist_at(&testobj->fields, 0);
    JOBJ_FIELD_PTR f1 = alist_at(&testobj->fields, 1);

    assert(f0->value.kind == JFK_STR);
    assert(memcmp("string", f0->fieldName.buf, strlen("string")) == 0 && "str field name");
    assert(memcmp("string val", f0->value.value.str.buf, strlen("string val")) == 0 && "string value");

    assert(f1->value.kind == JFK_NUM);
    assert(memcmp("number", f1->fieldName.buf, strlen("number")) == 0 && "numb field name");
    assert(f1->value.value.number == 123.456);
}

void regression_test_array_of_numbers_object()
{
    const char *testjson = "{\"numbers\":[10,11,12,1,2,3,4]}";
    JOBJPTR testobj = jobj_from(testjson);
    assert(testobj != NULL);
    assert(testobj->fields.len == 1);

    JOBJ_FIELD_PTR f0 = alist_at(&testobj->fields, 0);

    assert(f0->value.kind == JFK_ARR);
    assert(memcmp("numbers", f0->fieldName.buf, strlen("numbers")) == 0 && "array field name");

    assert(f0->value.value.arr->values.len == 7);

    double expected[] = {10, 11, 12, 1, 2, 3, 4};

    for (size_t i = 0; i < 7; i++)
    {
        JOBJ_FIELD_VALUE *val = alist_at(&f0->value.value.arr->values, i);
        assert(val->kind == JFK_NUM);
        assert(val->value.number == expected[i]);
    }
}

void regression_test()
{
    regression_test_number_object();
    printf("number object : OK\n");
    regression_test_string_object();
    printf("string object : OK\n");
    regression_test_string_number_object();
    printf("string number object : OK\n");
    regression_test_array_of_numbers_object();
    printf("number array : OK\n");

    printf("JSON REGRESSION TEST COMPLEATE\n");
}

int main()
{
    string_test();
    regression_test();
    const char *json = "{\n"
                       "\"arrayOfStrings\" : [\"A\",\"B\",\"C\"],\n"
                       "\"number\" : 5050.24119,\n"
                       "\"arrayOfNumbers\" : [0,1,2,3,4],\n"
                       "\"string\" : \"string value\",\n"
                       "\"numbereasy?\" : 1,\n"
                       "\"object\" : {},\n"
                       "\"arrayOfObject\"  : [{},{},{}]\n"
                       "}\n";

    JOBJPTR s = jobj_from(json);

    if (s == NULL)
    {
        fprintf(stderr, "Error parsing object\n");
        exit(1);
    }
}