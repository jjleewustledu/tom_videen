#$Log: Makefile,v $
#	Makefile:	makeprmlst
#	Author:		Tom Videen
#	Date:		Aug-96

BINPATH = /usr/local/npg
LIBPATH = /usr/local/npg/lib
INCPATH = /usr/local/npg/include
INCS = -I${INCPATH}
LOCAL = /home/petsun19/tom/src
LIBS = -L${LIBPATH} -lnrimg -limage -lcti -limgutil -lm
CC = ${PURE} cc ${INCS} ${CFLAGS}
 
SCRIPTS = makeprmlst
CSRCS =  makeprmlst.c
OBJS = ${CSRCS:.c=.o}

makeprmlst.bin: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

.c.o:
	${CC} -c $<

release:
	cp makeprmlst.bin ${BINPATH}
	saversion ${BINPATH}/makeprmlst.bin Version

lint:
	lint ${INCS} ${CSRCS}

checkout:
	co ${CSRCS} ${SCRIPTS}

checkin:
	ci ${CSRCS} ${SCRIPTS}

clean:
	rm ${OBJS}

cleanall:
	rm -f ${CSRCS} ${SCRIPTS} Version
