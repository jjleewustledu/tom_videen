PRGNAME = betacal
# Author:	Tom Videen
# Date:		Feb 2004
# Expects:	set environmental variables in	source ~/.buildrc

VER = 2.3
LIBS    = -L${LIBPATH} -lpe -lnsl
SCRIPTS	= ${PRGNAME}
FSRCS	= ${PRGNAME}.f \
		polfit.f

OBJS	= ${FSRCS:.f=.o}
BF77	= ${PURE} ${F77} ${FFLAGS}

${PRGNAME}.bin: ${OBJS}
	${BF77} -o $@ ${OBJS} ${LIBS}

.f.o: 
	${BF77} -c $<

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
	co ${FSRCS} ${SCRIPTS}

checkin:
	ci ${FSRCS} ${SCRIPTS}

lock:
	co -l ${FSRCS} ${SCRIPTS}

sync:
	ci -f -r${VER} ${FSRCS} ${SCRIPTS}

clean:
	rm ${OBJS}

cleanall:
	rm -f ${FSRCS} ${SCRIPTS} Version

