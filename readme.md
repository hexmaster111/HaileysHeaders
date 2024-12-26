

# Haileys Headers !

## using.h

This is a header only lib that allows for automatic freeing of resources when execution leaves the usings scope. 


``` c
#include "using.h"

using(int *some = calloc(1, 32), free(some))
{
    some[0] = 1;
    printf("some %p\n", some);
}
```
This is a macro hack using the `for` keyword.

The macro expands into something like

``` c
int latch43 = 0;
for (int *some = calloc(1, 32); latch43 < 1; ++latch43, free(some))
{
    some[0] = 1;
    printf("some %p\n", some);
}
```

the local var (`latch43` in this case) is used to only run the for loop a single time

Other Examples

Using it with raylib
``` c

#include "using.h"
#include <raylib.h>

int main(int argc, char *argv[])
{

    using(InitWindow(800, 600, "Hello World"), CloseWindow())
    {
        SetTargetFPS(60);

        while (!WindowShouldClose())
        {
            using(BeginDrawing(), EndDrawing())
            {
                ClearBackground(WHITE);
                DrawFPS(10, 10);
            }
        }
    }
}
```


# todo: more docs on
## vec.h
## json.h json.c
## vt100.h vt100.c
## string.h string.c
## linked_list.h linked_list.c
## array_list.h array_list.c
