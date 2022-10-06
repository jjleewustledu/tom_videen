LIBNAME = libcti
# Author:	Tom Videen
# Date:		Dec 2002
# Set environmental variables in:	source ~/.buildrc

HEADERS	= 
CSRCS	= matrix.c \
		matrix_extra.c \
		displayMatrixData.c

OBJS	= ${CSRCS:.c=.o}
LINT	= ${CSRCS:.c=.ln}
INCS	= -I${INCPATH}
BCC		= ${PURE} ${CC} ${INCS} ${CFLAGS}

${LIBNAME}.a: ${OBJS}
	ar rcv $@ ${OBJS}

.c.o: ${HEADERS}
	${BCC} -c $<

release:
	mv ${LIBNAME}.a ${LIBPATH}
	chmod 644 ${LIBPATH}/${LIBNAME}.a
	chgrp videen ${LIBPATH}/${LIBNAME}.a
	ranlib ${LIBPATH}/${LIBNAME}.a

version:
	saversion ${LIBPATH}/${LIBNAME}.a Version

checkout:
	co ${CSRCS} ${HEADERS}

checkin:
	ci ${CSRCS} ${HEADERS}

lock:
	co -l ${CSRCS} ${HEADERS}

lint:
	lint ${LPARMS} -u ${INCS} ${CSRCS}

splint:
	splint ${SPLPARMS} ${INCS} ${CSRCS}

clean:
	rm ${OBJS} ${LINT}
	/usr/local/pkg/purify/pure_remove_old_files . 0

cleanall:
	rm -f ${CSRCS} ${HEADERS} Version

