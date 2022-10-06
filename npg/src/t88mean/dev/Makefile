# $Log: Makefile,v $
# Revision 2.3  1996/10/16  17:20:54  tom
# grp
#
# Revision 2.2  1996/10/16  17:18:19  tom
# version
#
# Revision 2.1  1996/09/25  18:57:03  tom
# lnsl
#
# Revision 2.0  1996/05/10  22:00:42  tom
# new libimage
#
#	Makefile:	t88mean
#	Author:		Tom Videen
#	Date:		May-96
#   Sun setenv:
#       PURE    ""  ("purify -first_only")
#       CFLAGS  -O  ("-g")
#	History:
#		22-Feb-94	Eliminate local getatlas_t88.c and use function in libhp.a
#		22-Sep-94	Add atlas_scalers and matrix_transform
#		10-Nov-95	Add BINPATH, LIBPATH, INCPATH, etc.
#		27-Feb-96	Replace libnpg and libecat7 with libcti and libimg
#		May-96		Replace libimg with libimage

BINPATH = /usr/local/npg/bin
LIBPATH = /usr/local/npg/lib
INCPATH = /usr/local/npg/include
INCS = -I${INCPATH}
LIBS = -L${LIBPATH} -limgutil -lmatrix -lnrimg -limage -lcti -lnrc -lm -lnsl
CC = ${PURE} cc ${INCS} ${CFLAGS}

CSRCS = t88mean.c \
		adjust_markers.c \
		atlas_scalers.c	\
		atlas_transform.c \
		getair.c \
		getatlas_none.c \
		get_input_lists.c \
		getprms_t88.c \
		image_scale.c \
		matrix_transform.c	\
		noatlas_transform.c	\
		printmat.c	\
		sxr_xcenter.c

SCRIPTS = t88mean
OBJS = ${CSRCS:.c=.o}

t88mean.bin: ${OBJS} 
	${CC} -o $@ ${OBJS} ${LIBS}

.c.o:
	${CC} -c $<

release:
	mv t88mean.bin ${BINPATH}
	chmod 775 ${BINPATH}/t88mean.bin
	chgrp videen ${BINPATH}/t88mean.bin

version:
	saversion ${BINPATH}/t88mean.bin Version

checkout:
	co ${CSRCS} ${SCRIPTS}

checkin:
	ci ${CSRCS} ${SCRIPTS}

cflow:
	cflow ${INCS} ${CSRCS}

lint:
	lint ${INCS} ${CSRCS}

clean:
	rm ${OBJS}

cleanall:
	rm -f ${CSRCS} Version ${SCRIPTS}

