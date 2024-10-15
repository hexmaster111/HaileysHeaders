#ifndef JSON_H
#define JSON_H

#include "string.h"

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
#endif // JSON_H