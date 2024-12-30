#ifndef FILE_MAP_H
#define FILE_MAP_H

typedef struct file_memmap
{
    int len, fd;
    char *map;
} file_memmap;

void CloseFileMemMap(file_memmap *fc);
int OpenFileMemMap(file_memmap *fc, const char *fpath);
char *GetFileContent(file_memmap map);
#endif // FILE_MAP_H