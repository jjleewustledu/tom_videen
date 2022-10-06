PRGNAME	= weightedmean
# Author:	Tom Videen
# Date:		Feb 2004
# Expects:	set environmental variables:	source ~/.buildrc

VER = 1.3
LIBS	= -lm -lnsl
HEADERS	= 
SCRIPTS	= ${PRGNAME}
CSRCS	= ${PRGNAME}.c

OBJS	= ${CSRCS:.c=.o}
LINT	= ${CSRCS:.c=.ln}
INCS	= -I${INCPATH}
BCC		= ${PURE} ${CC} ${INCS} ${CFLAGS}
SPLPRM	= "-mustfreefresh"

${PRGNAME}.bin: ${OBJS}
	${BCC} -o $@ ${OBJS} ${LIBS}

.c.o: ${HEADERS}
	${BCC} -c $<

release:
	mv ${PRGNAME}.bin ${BINPATH}
	chmod 755 ${BINPATH}/${PRGNAME}.bin
	chgrp videen ${BINPATH}/${PRGNAME}.bin

script:
	cp ${PRGNAME} ${BINPATH}
	chmod 755 ${BINPATH}/${PRGNAME}
	chgrp videen ${BINPATH}/${PRGNAME}

version:
	saversion ${BINPATH}/${PRGNAME}.bin Version ${VER}

checkout:
	co ${CSRCS} ${SCRIPTS} ${HEADERS}

lock:
	co -l ${CSRCS} ${SCRIPTS} ${HEADERS}

checkin:
	ci ${CSRCS} ${SCRIPTS} ${HEADERS}

sync:
	ci -f -r${VER} ${CSRCS} ${SCRIPTS} ${HEADERS}

lint:
	lint ${LPARMS} ${INCS} ${CSRCS}

splint:
	splint ${SPLPARMS} ${SPLPRM} ${INCS} ${CSRCS} | tee splint.log

clean:
	rm ${OBJS} ${LINT}
	/usr/local/pkg/purify/pure_remove_old_files . 0

cleanall:
	rm -f ${CSRCS} ${SCRIPTS} ${HEADERS} Version

