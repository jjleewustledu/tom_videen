PRGNAME = blood
# Author:	Tom Videen
# Date:		Apr 2009
# Expects:	set environmental variables in	source ~/.buildrc

VER = 2.3
LIBS    = -L${LIBPATH} -lpe -lnsl
SCRIPTS	= ${PRGNAME}
FSRCS	= ${PRGNAME}.f \
		bldaddpt.f	\
		bldalter.f	\
		bldcalc.f	\
		bldcurve.f	\
		blddecor.f	\
		blddelpt.f	\
		bldenter.f	\
		bldlist.f	\
		bldnew.f	\
		bldparm.f	\
		bldrepro.f	\
		bldsave.f	\
		bldsecs.f	\
		bldshift.f
INCS	= blood.inc

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
	co ${FSRCS} ${SCRIPTS} ${INCS}

checkin:
	ci ${FSRCS} ${SCRIPTS} ${INCS}

lock:
	co -l ${FSRCS} ${SCRIPTS} ${INCS}

sync:
	ci -f -r${VER} ${FSRCS} ${SCRIPTS} ${INCS}

clean:
	rm ${OBJS}

cleanall:
	rm -f ${FSRCS} ${SCRIPTS} Version ${INCS}

