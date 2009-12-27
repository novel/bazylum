#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>

#define	DATABASE	"bazylum.db"

static int stat_callback(void *argno, int argc, char **argv, char **colname)
{
	assert(argc == 2);

	printf("%s: %d sec\n", argv[0], atoi(argv[1]));
	return 0;
}

sqlite3 *open_database()
{
	int rc;
	sqlite3 *db;

	rc = sqlite3_open(DATABASE, &db);

	if (SQLITE_OK != rc) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	return db;
}

void do_stat()
{
	sqlite3 *db;
	int rc;
	char *err;

	/* should be moved to main() when have more commands */
	db = open_database();

	rc = sqlite3_exec(db, "SELECT window_name,SUM(window_time) AS total_time FROM bazylum "
			"GROUP BY window_name ORDER BY total_time DESC", stat_callback,
			0, &err);
	if (SQLITE_OK != rc) {
		fprintf(stderr, "Error executing query: %s\n", err);
		sqlite3_free(err);
		exit(1);
	}
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
