CFLAGS=-g -Wall

all:
	gcc $(CFLAGS) `pkg-config --libs --cflags x11 sqlite3` -o bazylumd bazylumd.c
	gcc $(CFLAGS) `pkg-config --libs --cflags x11 sqlite3` -o bazylum bazylum.c
