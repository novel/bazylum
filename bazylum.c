#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>

#include "db.h"
#include "config.h"

static int stat_callback(void *argno, int argc, char **argv, char **colname)
{
	assert(argc == 2);

	printf("%s: %d sec\n", argv[0], atoi(argv[1]));
	return 0;
}

void do_stat()
{
	sqlite3 *db;

	/* should be moved to main() when have more commands */
	db = db_open();

	db_query(db, "SELECT window_name,SUM(window_time) AS total_time "
			"FROM bazylum GROUP BY window_name "
		       	"ORDER BY total_time DESC ",
			stat_callback, 0);
}

int main(int argc, char **argv)
{
	char *command;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s command [args]\n", argv[0]);
		exit(1);
	}

	command = argv[1];

	if (strcmp(command, "stat") == 0) {
		do_stat();
	} else {
		/* later we will have smarter dispatcher */
		fprintf(stderr, "Unknown command: %s\n", command);
		exit(1);
	}

	return 0;
}
