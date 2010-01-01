#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *expanduser(const char *path)
{
	char *home, *result;
	size_t result_len;	

	if (path[0] != '~')
		return strdup(path);
	path++;

	home = getenv("HOME");
	if (home == NULL) {
		fprintf(stderr, "Error: environment variable HOME is not set\n");
		exit(1);
	}

	result_len = strlen(home) + strlen(path) + 1;
	result = malloc(result_len * sizeof(char *));
	snprintf(result, result_len, "%s%s", home, path);

	return result;	
}
