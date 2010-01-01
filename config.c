#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "utils.h"

short int does_config_exist()
{
	struct stat sb;
	char *config_path;
	int status;

	config_path = expanduser(CONFIG_PATH);

	printf("Using config dir: %s\n", config_path);

	status = stat(config_path, &sb);

	free(config_path);

	if (status == -1) {
		return 0;
	}
	
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		return 1;

	return 0;
}

void create_config()
{
	char *config_path;
	int status;

	config_path = expanduser(CONFIG_PATH);

	status = mkdir(config_path, S_IRWXU);
	free(config_path);

	if (status == -1) {
		perror("mkdir");
		exit(1);
	}
}

void create_config_if_not_exists()
{
	if (does_config_exist() != 1) {
		create_config();
	}
}
