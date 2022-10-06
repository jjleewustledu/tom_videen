#$Log$
#	Makefile:	xray
#	Date:		Apr-96
#	Author:		Tom Videen

BINPATH = /usr/local/npg
INCPATH = /usr/local/npg/include
SYSLIBS = /usr/lib
LIBPATH = /usr/local/lib
INCS = -I${INCPATH}
LIBS = -L${SYSLIBS}/Motif1.2 -lXm -L${SYSLIBS}/X11R5 -lXt -L${LIBPATH} -lnpg -L${SYSLIBS}/X11R5 -lX11 -lecat7 -lm

CC = ${PURE} cc ${INCS} ${CFLAGS}

RSCS =	xray.callbacks	\
		xray.code	\
		xray.defaults	\
		xray.widgets 

SRCS =	SetParameters.c \
		drawpet.c \
		drawxray.c \
		getmagf.c \
		interp.c \
		rdraster.c \
		rotation.c \
		xray.c \
		xrayparm.c \
		xwd.c

OBJS = ${SRCS:.c=.o}

xray.bin: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

.c.o:
	${CC} -c $<

release:
	mv xray.bin ${BINPATH}
	chmod 755 ${BINPATH}/xray.bin
	chgrp videen ${BINPATH}/xray.bin
	cp xray.defaults ${LIBPATH}/xray
	saversion ${BINPATH}/xray.bin Version

checkout:
	co ${SRCS} ${HDRS} ${RSCS}

cflow:
	cflow ${INCS} ${SRCS}

lint:
	lint ${INCS} ${SRCS}

clean:
	rm ${OBJS}

cleanall:
	rm -f ${SRCS} ${HDRS} ${RSCS}
