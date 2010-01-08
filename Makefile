CC = gcc
LD = gcc
CFLAGS = -g -Wall `pkg-config --cflags x11 sqlite3`
LDFLAGS = `pkg-config --libs x11 sqlite3`
RM = rm -f

COMMON_OBJS = utils.o config.o db.o list.o

bazylumd_OBJS = bazylumd.o $(COMMON_OBJS)
bazylum_OBJS = bazylum.o $(COMMON_OBJS)

all: bazylumd bazylum

bazylumd: $(bazylumd_OBJS)
	$(LD) $(LDFLAGS) -o bazylumd $(bazylumd_OBJS)
	
bazylum: $(bazylum_OBJS)
	$(LD) $(LDFLAGS) -o bazylum $(bazylum_OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) bazylum bazylumd *.o
