#ifndef _READ_ALL_LINES_H
#define _READ_ALL_LINES_H

void FreeAllLines(char **lines, int count);
char **ReadAllLines(const char *fname, int *out_linecount);

#endif //_READ_ALL_LINES_H