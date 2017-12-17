CC=gcc
CCFLAGS=-Wall -Wextra -pedantic -std=c99 -o3 `pkg-config --cflags gtk+-2.0`
LDFLAGS=`pkg-config --libs gtk+-2.0` -lm

VPATH= src
OBJ_FILES_IMG=compute.o misc.o main.o
OBJ_FILES_AUTO=compute.o misc.o auto.o

all: viz auto

.c.o:
	${CC} ${CCFLAGS} -o $@ -c $<

viz: ${OBJ_FILES_IMG}
	${CC} -o $@ ${OBJ_FILES_IMG} ${LDFLAGS} -g

auto: ${OBJ_FILES_AUTO}
	${CC} -o $@ ${OBJ_FILES_AUTO} ${LDFLAGS} -g

.PHONY: clean
clean:
	rm -rf *.o viz auto