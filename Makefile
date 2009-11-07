CFLAGS=-g

all:
	gcc $(CFLAGS) `pkg-config --libs --cflags gdk-2.0 sqlite3` -o bazylum bazylum.c
