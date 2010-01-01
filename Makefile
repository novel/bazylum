CFLAGS = -g -Wall
RM = rm -f

bazylumd_OBJS = bazylumd.o utils.o
bazylum_OBJS = bazylum.o utils.o

all: bazylumd bazylum

bazylumd: $(bazylumd_OBJS)
	gcc $(CFLAGS) `pkg-config --libs --cflags x11 sqlite3` -o bazylumd $(bazylumd_OBJS) 
	
bazylum: $(bazylum_OBJS)
	gcc $(CFLAGS) `pkg-config --libs --cflags x11 sqlite3` -o bazylum $(bazylum_OBJS)

.c.o:
	gcc $(CFLAGS) `pkg-config --cflags x11 sqlite3` -c $*.c

clean:
	$(RM) bazylum bazylumd *.o
