#include "file_memmap.h"

#include <stdbool.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void CloseFileMemMap(file_memmap *fc)
{
    munmap(fc, fc->len);
    close(fc->fd);

    fc->map = NULL;
    fc->len = 0;
    fc->fd = 0;
}

// -1 on error 0 on ok
int OpenFileMemMap(file_memmap *fc, const char *fpath)
{
    int fd = open(fpath, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return -1;
    }

    struct stat sb;

    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        return -1;
    }

    void *map = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (map == MAP_FAILED)
    {
        close(fd);
        return -1;
    }

    fc->map = (char*)map;
    fc->len = sb.st_size;
    fc->fd = fd;

    return 0;
}

char *GetFileContent(file_memmap map) { return map.map; }
