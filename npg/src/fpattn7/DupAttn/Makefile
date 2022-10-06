PRGNAME = cpattn
#	Author:		Tom Videen
#	Date:		Mar-2000

BINPATH = ${LOCAL}/bin
LIBPATH = ${LOCAL}/lib
INCPATH = ${LOCAL}/include
VPATH   = ${INCPATH}
INCS    = -I${INCPATH}
#LIBS    = -L${LIBPATH} -lnrimg /home/npggw/tom/src/libimage/libimage.a -lcti -lnrc -lm -lnsl
LIBS    = -L${LIBPATH} -lnrimg -limage -lcti -lnrc -lm -lnsl
LPARMS  = -Nlevel=4 -u -x -erroff=E_STATIC_UNUSED
CC      = ${PURE} cc ${INCS} ${CFLAGS}
HEADERS = 
SCRIPTS = ${PRGNAME}
CSRCS   = ${PRGNAME}.c
OBJS    = ${CSRCS:.c=.o}
LINT    = ${CSRCS:.c=.ln}

${PRGNAME}.bin: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

.c.o: ${HEADERS}
	${CC} -c $<

release:
	mv ${PRGNAME}.bin ${BINPATH}
	chmod 755 ${BINPATH}/${PRGNAME}.bin
	chgrp videen ${BINPATH}/${PRGNAME}.bin

version:
	saversion ${BINPATH}/${PRGNAME}.bin Version

checkout:
	co ${CSRCS} ${SCRIPTS}

checkin:
	ci ${CSRCS} ${SCRIPTS}

lint:
	lint ${LPARMS} ${INCS} ${CSRCS}

clean:
	rm ${OBJS} ${LINT}
	/usr/local/pkg/purify/pure_remove_old_files . 0

cleanall:
	rm -f ${CSRCS} Version ${SCRIPTS}
