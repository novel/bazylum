#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>

#define	DATABASE	"bazylum.db"

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

	db = open_database();

	printf("stat\n");
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
