#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "config.h"
#include "utils.h"

#define DATABASE	CONFIG_PATH "/bazylum.db"
#define VERSION		"bazylum 0.1"

void log_window_activity(sqlite3 *db, char *window_name, int timeout)
{
	int rc;
	char *err;
	char *query = sqlite3_mprintf("INSERT INTO bazylum(window_name, window_time) VALUES ('%q', %d)", window_name, timeout);

	/* printf("query: %s\n", query); */

	rc = sqlite3_exec(db, query, 0, 0, &err);

	if (SQLITE_OK != rc) {
		fprintf(stderr, "Error inserting data: %s\n", err);
		sqlite3_free(err);
		exit(1);
	}

	sqlite3_free(query);
}

sqlite3* init_database()
{
	int rc;
	sqlite3 *db;
	char *err, *database_path;

	database_path = expanduser(DATABASE);

	rc = sqlite3_open(database_path, &db);
	free(database_path);

	if (SQLITE_OK != rc) {
		fprintf(stderr, "%s: %s\n", DATABASE, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	/* create table if doesn't exist */
	rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS bazylum(id integer primary key, \
		       window_name varchar(255) not null, window_time int not null, \
		       \"timestamp\" TEXT DEFAULT (strftime('%Y-%m-%dT%H:%M','now', 'localtime')));",
		       0, 0, &err);

	if (SQLITE_OK != rc) {
		fprintf(stderr, "Wasn't able to create a schema: %s\n", err);
		sqlite3_free(err);
		exit(1);
	}

	return db;
}

char* x11_get_active_window_name(Display *dpy, Window root_window)
{
	Atom actual_type;
	int actual_format, status;
	unsigned long nitems, bytes;
	long *data;
	Window active_window;
	char *window_name;

	status = XGetWindowProperty(
		dpy,
		root_window,
		XInternAtom(dpy, "_NET_ACTIVE_WINDOW", True),
		0,
		(~0L),
		False,
		AnyPropertyType,
		&actual_type,
		&actual_format,
		&nitems,
		&bytes,
		(unsigned char**)&data);

	active_window = data[0];

	/*printf("win 0x%lx, status: %d\n", active_window, status);*/
	XFree(data);

	if ((int)active_window == 0) {
		return strdup("desktop");
	}

	status = XGetWindowProperty(
		dpy,
		active_window,
		XInternAtom(dpy, "WM_CLASS", True),
		0,
		(~0L),
		False,
		AnyPropertyType,
		&actual_type,
		&actual_format,
		&nitems,
		&bytes,
		(unsigned char**)&data);

	window_name = strdup((char *)data);

	XFree(data);
	
	return window_name;
}

void usage(void)
{
	fprintf(stderr, "usage: bazylumd [-v] [-f]\n");
	exit(1);
}

int main(int argc, char **argv)
{
	Display *dpy;
	int screen;
	char *prev_window = NULL;
	char *cur_window;
	int timeout = 1;
	int active_time = 0;
	sqlite3 *db;
	int opt;
	short foreground = 0;
	pid_t pid, sid;

	while ((opt = getopt(argc, argv, "vf")) != -1) {
		switch (opt) {
			case 'v':
				printf("%s\n", VERSION);
				exit(0);
			case 'f':
				foreground = 1;
				break;
			default:
				usage();
		}
	}

	create_config_if_not_exists();

	if (foreground != 1) {
		pid = fork();
		if (pid < 0) {
			exit(1);
		} else if (pid > 0) {
			exit(0);
		}

		umask(0);

		sid = setsid();
		if (sid < 0) {
			exit(1);
		}
	}

	dpy = XOpenDisplay(NULL);
	
	if (dpy == NULL) {
		fprintf(stderr, "Error: cannot open display\n");
		exit(1);
	}
	
	screen = DefaultScreen(dpy);

	Window w = 0;
	
	w = RootWindow(dpy, screen);

	/* init database connection */
	db = init_database();

	/* main loop */
	for (;;) {
		cur_window = x11_get_active_window_name(dpy, w);

		if (prev_window != NULL) {
			if (strcmp(cur_window, prev_window) == 0) {
				active_time += timeout;
			} else {
				if (active_time != 0)
					log_window_activity(db, prev_window, active_time);
				active_time = 0;
			}

			free(prev_window);
		}


		prev_window = strdup(cur_window);

		free(cur_window);

		sleep(timeout);
	}

	XCloseDisplay(dpy);
	sqlite3_close(db);

	return 0;
}
