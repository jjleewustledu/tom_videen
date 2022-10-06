#	Makefile:	betacal
#	Author:		Tom Videen
#	Date:		Dec-99
#	Expects:
#		setenv LOCAL /usr/local/npg
#		setenv CFLAGS -O
#		setenv FFLAGS -C -O -e
#	Optional:
#		setenv CFLAGS -g

PRGNAME = betacal
BINPATH = ${LOCAL}/bin
LIBPATH = ${LOCAL}/lib
LIBS    = -L${LIBPATH} -lpe -lnsl
LPARMS  = -Nlevel=4 -u -x -erroff=E_STATIC_UNUSED
CC      = ${PURE} cc ${INCS} ${CFLAGS}
FF		= f77 ${FFLAGS}
FSRCS   = ${PRGNAME}.f \
		polfit.f
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
	rm -f ${FSRCS} Version 
