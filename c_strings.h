#include <string.h>

bool string_starts_with(const char *starts_with, const char *string)
{
    size_t starts_len = strlen(starts_with);
    size_t search_str_len = strlen(string);

    if (starts_len > search_str_len)
        return false;

    for (size_t i = 0; i < starts_len; i++)
    {
        if (string[i] != starts_with[i])
            return false;
    }

    return true;
}

const char *string_skip_leading_spaces(const char *s)
{
    while (isspace(s[0]))
        s += 1;
    return s;
}

int string_index_of(char index_of, const char *s)
{
    int idx = 0;

    while (s[0] != index_of)
    {
        s += 1;
        idx += 1;

        if (s[0] == '\0')
            return -1;
    }

    return idx;
}


int string_index_of_any(const char *index_of, const char *s)
{

    int idx = 0;

    int idx_of_len = strlen(index_of);

    while (s[0] != 0)
    {
        for (size_t i = 0; i < idx_of_len; i++)
        {
            if (s[0] == index_of[i])
                return idx;
        }

        s += 1;
        idx += 1;
    }

    return -1;
}

const char *string_skip_to(char skip_to, const char *s)
{
    while (s[0] != skip_to)
    {
        s += 1;
        if (s[0] == '\0')
            return s;
    }

    return s;
}


