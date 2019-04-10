CC=gcc
FLAGS=-Wextra -Wshadow -std=gnu99
LIBS=
BIN=/usr/bin/
OBJS=

all: ./pt.c ${OBJS}
	${CC} ./pt.c ${OBJS} ${FLAGS} ${LIBS} -o ./p

clean:
	rm ./p
