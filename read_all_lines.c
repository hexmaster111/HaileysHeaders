#include <stdio.h>
#include <stdlib.h>

void FreeAllLines(char **lines, int count)
{
	for (int i = 0; i < count; i++)
	{
		free(lines[i]);
	}

	free(lines);
}


char **ReadAllLines(const char *fname, int *out_linecount)
{
	FILE *f = fopen(fname, "r");

	if (f == NULL)
		return NULL;

	int arraylen = 1024;
	int strlen = 1024;

	int lines_used = 0;
	int chars_read = 0;

	char **ret = malloc(sizeof(char *) * arraylen);
	ret[lines_used] = malloc(strlen);

	while (1)
	{

		if (feof(f))
		{
			// we are all done reading, finlise the last line
			ret[lines_used] = realloc(ret[lines_used], chars_read); // shrink string to be size read
			ret[lines_used][chars_read - 1] = 0;					// terminate the string
			lines_used++;
			break;
		}

		char c = fgetc(f);

		if (ferror(f))
		{
			perror("fgetc");
			return NULL;
		}

		if (chars_read > strlen)
		{
			strlen *= 2;
			ret[lines_used] = realloc(ret[lines_used], strlen);
		}

		if ((c == '\r' || c == '\n') && chars_read > 0)
		{
			// store this line

			ret[lines_used] = realloc(ret[lines_used], chars_read + 1); // shrink string to be size read
			ret[lines_used][chars_read] = 0;							// terminate the string

			strlen = 1024; // reset var
			chars_read = 0;

			lines_used += 1;

			if (lines_used >= arraylen) // grow the continaing array
			{
				arraylen *= 2;
				ret = realloc(ret, arraylen * sizeof(char *));
			}
			ret[lines_used] = malloc(strlen); // alloc the next string
			continue;
		}
		else if (c == '\r' || c == '\n' && 0 >= chars_read)
		{
			// we just got a stray \r or \n
			continue;
		}

		ret[lines_used][chars_read] = c;
		chars_read += 1;
	}

	// resize the return array to fit used
	ret = realloc(ret, lines_used * sizeof(char *));

	*out_linecount = lines_used;
	return ret;
}
