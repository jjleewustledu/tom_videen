LIBNAME = libmatrix
# Author:	Tom Videen
# Date:		Nov 2002
# Set environmental variables in:	source ~/.buildrc

CSRCS = dmatrix_add.c \
		dmatrix_mult.c \
		dmatrix_print.c \
		dvector_dist.c \
		matrix_add.c \
		matrix_mult.c \
		matrix_print.c \
		matrix_sub.c \
		matrix_trans.c \
		vector_dist.c \
		vector_mult.c \
		c3tensor.c \
		smatrix.c  \
		free_c3tensor.c  \
		free_smatrix.c

HEADERS	= matrixlib.h

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

clean:
	rm ${OBJS} ${LINT}
	/usr/local/pkg/purify/pure_remove_old_files . 0

cleanall:
	rm -f ${CSRCS} ${HEADERS} Version

