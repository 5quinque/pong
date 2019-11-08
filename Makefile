CC=gcc
FLAGS=-Wextra -Wshadow -std=gnu99
LIBS=
BIN=/usr/bin/
OBJS=

all: ./src/pt.c ${OBJS}
	${CC} ./src/pt.c ${OBJS} ${FLAGS} ${LIBS} -o ./bin/p

clean:
	rm ./bin/p
