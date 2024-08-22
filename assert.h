#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#ifndef ASSERT_H
// header
#define ASSERT_H

#define assert(cond) __assert(cond, #cond, __LINE__, __FILE__)

#endif // ASSERT_H

// c file
#ifdef ASSERTIMPL

void __assert(bool cond, const char *str, int line, const char *file)
{
    if (cond)
        return;
    fprintf(stderr, "ASSERT FAIL: %s, %s:%d\nbacktrace:\n", str, file, line);
    void *callstack[128];
    int i, frames = backtrace(callstack, 128);
    char **strs = backtrace_symbols(callstack, frames);
    for (i = 0; i < frames; ++i)
    {
        printf("%s\n", strs[i]);
    }
    free(strs);
    abort();
}
#endif // ASSERTIMPL