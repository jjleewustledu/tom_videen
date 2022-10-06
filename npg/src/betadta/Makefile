#	Makefile:	betadta
#	Author:		Tom Videen
#	Date:		Dec-99
#	Expects:
#		setenv LOCAL /usr/local/npg

PRGNAME = betadta
BINPATH = ${LOCAL}/bin
LIBPATH = ${LOCAL}/lib
INCPATH = ${LOCAL}/include
VPATH   = ${INCPATH}
INCS    = -I${INCPATH}
LIBS    = -L${LIBPATH} -lpe
FF      = f77 -i4 -O -e
FSRCS =	betadta.f \
		deconv.f  \
		fftpe.f   \
		gettke.f  \
		polfit.f  \
		bldenter.f \
		bldsecs.f
OBJS    = ${FSRCS:.f=.o}

${PRGNAME}: ${OBJS}
	${FF} -o $@ ${OBJS} ${LIBS}

.f.o:
	${FF} -c $<

release:
	mv ${PRGNAME} ${BINPATH}
	chmod 755 ${BINPATH}/${PRGNAME}
	chgrp videen ${BINPATH}/${PRGNAME}

version:
	saversion ${BINPATH}/${PRGNAME} Version

checkout:
	co ${FSRCS}

checkin:
	ci ${FSRCS}

clean:
	rm ${OBJS} 

cleanall:
	rm -f ${CSRCS} Version ${SCRIPTS}
