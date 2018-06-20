CC = gcc
CFLAGS = -I../lib -g -O2 -D_REENTRANT -Wall
LIBS = -lpthread

PROGS = day_time_cli day_time_srv

all: 	${PROGS}

day_time_cli: 	day_time_cli.o
				${CC} ${CFLAGS} -o $@ day_time_cli.o ${LIBS}
day_time_srv: 	day_time_srv.o
				${CC} ${CFLAGS} -o $@ day_time_srv.o ${LIBS}

clean:
	rm -f ${PROGS}
