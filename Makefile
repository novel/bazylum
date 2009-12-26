CFLAGS=-g -Wall

all:
	gcc $(CFLAGS) `pkg-config --libs --cflags gdk-2.0 sqlite3` -o bazylumd bazylumd.c
