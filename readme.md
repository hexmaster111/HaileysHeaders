

# Haileys Headers !

## json_simple.h

A tiny header only lib for reading and writing json

Can read FLAT JSON, a json with only one level
Can Write any json without allocations done by the lib

### Writing
``` c
#define JSON_IMPL
#include "json_simple.h"

// just a buffer to write into 
char buf[1024*1024*3] = {0};

JsonBuildObject_Start(buf);
    JsonBuildObject_AddPropertyRaw(buf, "k", "1", 0);
    JsonBuildObject_AddProperty(buf, "label", "Label", 0);
    JsonBuildObject_AddProperty(buf, "something", "123", 0);
    JsonBuildObject_AddPropertyRaw(buf, "obj", "{}", 0);
    JsonBuildObject_AddPropertyRaw(buf, "arr", "[]", 1);
JsonBuildObject_End(buf);

printf("%s\n", buf);

// outputs:   {"k":1,"label":"Label","something":"123","obj":{},"arr":[]}
```

### Reading

Reading only supporst string properties ! A simple scanning reader

``` c
#define JSON_IMPL
#include "json_simple.h"

const char *json = "{\"k\":\"1\",\"label\":\"Label\",\"something\":\"123\"}";
char *k = NULL;
int k_len = 0;
char *label = NULL;
int label_len = 0;
ParseJson(json, strlen(json), "k", &k, &k_len);
ParseJson(json, strlen(json), "label", &label, &label_len);
printf("k: %.*s, label: %.*s\n",
       k_len, k,
       label_len, label);

```



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

## file_memmap.h file_memmap.c

This lets the user open files using OpenFileMemMap, and then interact with the file content from GetFileContent

Usage Example
```c

file_memmap m = {0};
OpenFileMemMap(&m, "te2.c");
puts(GetFileContent(m));
CloseFileMemMap(&m);

```

## taskchain.h (demo in taskchaindemo.c)

### HTCL (Haileys Task Chain lib)
Inspired by c#'s Tasks

#### Requirements
C99, if you want to use the same task creation sematntics as in the TaskChain structure demo.  It would get messy if you wanted to declair all the Task Chain parts as there own seprate varables.

##### Using

Tasks are just methods that return a `TaskResault`

```c
TaskResualt NeverFails()
{
    puts(__func__);
    return (TaskResualt){.resault = TaskResualt_Ok};
}
```

Tasks are structured into a `struct TaskChain`
```c
TskChn demotask = {
    .task = NeverFails,
    .Okey = &(TskChn){
        .task = FailsSometimes,
        .Fail = TaskFailedMethod,
        .Okey = &(TskChn){
            .task = FailsSometimes,
            .Fail = TaskFailedMethod}}};
```


A `struct TaskChain` can then be executed using the `RunChain` method

```c
RunChain(&demotask);
```
## read_all_lines.c read_all_lines.h

Just a helper that i find myself writing alot, reads all the file lines and splits on windows and linux line endings.

```c
int main(int argc, char *argv[])
{
	int linecount = 0;
	char **lines = ReadAllLines("test.txt", &linecount);

	for (int i = 0; i < linecount; i++)
	{
		puts(lines[i]);
	}

	FreeAllLines(lines, linecount);

	lines = NULL;
	return 0;
}
```

# todo: more docs on
## vec.h
## json.h json.c -- supports complex objects
## vt100.h vt100.c
## string.h string.c
## linked_list.h linked_list.c
## array_list.h array_list.c
