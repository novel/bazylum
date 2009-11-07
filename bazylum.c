#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <sqlite3.h>

#define DATABASE	"bazylum.db"

void sql_query(sqlite3 *db, const char *query)
{
}

int main(int argc, char **argv)
{
	Display *dpy;
	int screen;
	Atom     actual_type;
	Atom *props;
	int      actual_format;
	long     nitems;
	long     bytes;
	long     *data;
	char *prev_window = NULL;
	int      status;
	int i, n;
	Window active_window;
	int timeout = 1;
	int active_time = 0;
	sqlite3 *db;
	int rc;

	dpy = XOpenDisplay(NULL);
	screen = DefaultScreen(dpy);	

	Window w = 0;
	
	w = RootWindow(dpy, screen);

	/* init database connection */
	rc = sqlite3_open(DATABASE, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	for (;;) {
		status = XGetWindowProperty(
			dpy,
			w,
			XInternAtom(dpy, "_NET_ACTIVE_WINDOW", True),
			0,
			(~0L),
			False,
			AnyPropertyType,
			&actual_type,
			&actual_format,
			&nitems,
			&bytes,
			(char**)&data);


	//	printf("hello world\n");
	//	 for (i=0; i < nitems; i++){
	  //              printf("data[%d] %d\n", i, data[i]);
		active_window = data[0];              

		props = XListProperties(dpy, active_window, &n);	
	//	printf("n = %d\n", n);

	//	for (i = 0; i < n; i++) {
	//		printf("%s\n", XGetAtomName(dpy, props[i]));
	//	}
	//        }

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
			(char**)&data);


	//	 for (i=0; i < nitems; i++)
	  //              printf("data[%d] %d\n", i, data[i]);

		if (prev_window != NULL) {	
			if (strcmp(data, prev_window) == 0) {
				active_time += timeout;
			} else {
				printf("%s has been active for %d seconds\n", prev_window, active_time);
				active_time = 0;
			}
		}

		//printf("current: %s, previous: %s\n", data, prev_window);
		prev_window = strdup(data);

		sleep(timeout);
	}

	XCloseDisplay(dpy);

	return 0;
}
