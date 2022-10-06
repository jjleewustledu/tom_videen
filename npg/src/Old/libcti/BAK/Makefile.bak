LIBNAME = libcti
# Author:	Tom Videen
# Date:		Nov 2002
# Set environmental variables in:	source ~/.buildrc
#	Ignore warnings in acs.c and matrix.c.
#	This library contains functions calling the ACS which are not used by us.

HEADERS	= ecatAcs.h ecatArc.h ecatMcs.h ecatRecon.h ecatRfa.h ecatRts.h ecatrpc.h reconRpc.h
CSRCS1	= acs.c 
CSRCS	= acsRpc.c bucketUtilities.c crash.c dead_time_corrections.c delta_time.c \
	displayMatrixData.c ecatAcs_xdr.c ecatMcs_xdr.c ecatRfa_xdr.c ecatRts_xdr.c \
	extract_num.c make_matlist.c make_num_list.c matspec.c mat_get_spec.c matrix.c \
	matrix_extra.c mcs.c numcomp.c parse_list.c parse_matnum.c planeDefs.c rfa.c \
	rts.c scanUtils.c stringUtils.c

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

