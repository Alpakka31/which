OBJS = which.o
CC := gcc
CFLAGS := -Wall -Wextra -pedantic -Wformat=2 -O2
PREFIX := /usr/local
BINDIR := ${PREFIX}/bin
PROGNAME = which

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ${PROGNAME}

which.o: which.c
	$(CC) $(CFLAGS) -c ${PROGNAME}.c

install:
	install -Dm755 ${PROGNAME} $(DESTDIR)$(BINDIR)/${PROGNAME}

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/${PROGNAME}

clean:
	rm -f ${PROGNAME} $(OBJS)
