CC = gcc
LIBS=-lGL -lGLU -lglut  -LGL -lX11 -lX11 -lm
LIBSD= -I/user/X11R6/include -L/usr/lib -L/usr/X11R6/lib

tp1 : tp1.c
	${CC} tp1.c -o tp1 ${LIBS} ${LIBSDIR}

clean :
	rm -f tp1 \#*\#
