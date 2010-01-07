#include <stdlib.h>
#include <stdio.h>

#include <sqlite3.h>

#include "db.h"
#include "utils.h"

sqlite3 *open_database()
{
	int rc;
	sqlite3 *db;
	char *database_path;

	database_path = expanduser(DATABASE);
	rc = sqlite3_open(database_path, &db);
	free(database_path);

	if (SQLITE_OK != rc) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	return db;
}

void db_query(sqlite3 *db, const char *query,
		int (*callback)(void*,int,char**,char**),
		void *data)
{
	int rc;
	char *err;

	rc = sqlite3_exec(db, query, callback, data, &err);
	if (SQLITE_OK != rc) {
		fprintf(stderr, "Error executing query: %s\n", err);
		sqlite3_free(err);
		exit(1);
	}
}
