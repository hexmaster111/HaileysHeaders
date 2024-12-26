

# Haileys Headers !

## using.h

This is a header only lib that allows for automatic freeing of resources when execution leaves the usings scope. 


``` c
#include "using.h"

using(int *some = calloc(1, 32), free(some), some = NULL)
{
    some[0] = 1;
    printf("some %p\n", some);
}
```
This is a macro hack using the `for` keyword.

The macro expands into something like

``` c
int latch43 = 0;
for (int *some = calloc(1, 32); latch43 < 1; ++latch43, free(some), some = ((void *)0))
{
    some[0] = 1;
    printf("some %p\n", some);
}
```

the local var (`latch43` in this case) is used to only run the for loop a single time


# todo: more docs on
## vec.h
## json.h json.c
## vt100.h vt100.c
## string.h string.c
## linked_list.h linked_list.c
## array_list.h array_list.c
