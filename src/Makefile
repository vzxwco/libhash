CC = gcc
CFLAGS = -W -Wall -O2

PROG = hash

SRCS = $(shell ls *.c)
HDRS = $(SRCS:.c=.h)
OBJS = $(SRCS:.c=.o)


.PHONY: clean love

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf *.o *~ core $(PROG) $(CLIENT) a.out

love:
	@echo "not war."

