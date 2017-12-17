
#---------------------------------------------------------
#makefile permettant la compilation du programme principal
#---------------------------------------------------------
.KEEP_STATE:

#definition des variables
CC=gcc
OBJ=../OBJ
EXE=../EXE
CCFLAGS=-Wall -g `pkg-config --cflags gtk+-2.0`
LDFLAGS=`pkg-config --libs gtk+-2.0` -lm

#compilation
OBJ_FILES_IMG=compute.o misc.o main.o
OBJ_FILES_AUTO=compute.o misc.o auto.o

all: ImaProjet.exe ImaProjet_auto.exe dirty_obj

.c.o:
	${CC} ${CCFLAGS} -o $@ -c $<

ImaProjet.exe:${OBJ_FILES_IMG}
	${CC} -o ${EXE}/$@ ${OBJ_FILES_IMG} ${LDFLAGS} -g

ImaProjet_auto.exe:${OBJ_FILES_AUTO}
	${CC} -o ${EXE}/$@ ${OBJ_FILES_AUTO} ${LDFLAGS} -g

dirty_obj:
	mv *.o ${OBJ}

clean:
	\rm -f ${OBJ}/*.o ../*/core ${EXE}/*.exe

install:
	cp *.h ${INC}

.SUFFIXES: .exe .o .c .h