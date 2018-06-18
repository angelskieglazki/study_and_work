CC = gcc
CFLAGS = -I../lib -g -O2 -D_REENTRANT -Wall
LIBS = -lpthread

PROGS = day_time_cli

all: 	${PROGS}

day_time_cli: 	day_time_cli.o
				${CC} ${CFLAGS} -o $@ day_time_cli.o ${LIBS}
clean:
	rm -f ${PROGS}
