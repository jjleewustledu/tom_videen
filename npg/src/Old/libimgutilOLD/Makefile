LIBNAME = libimgutil
# Author:	Tom Videen
# Date:		Nov 2002
# Set environmental variables in:	source ~/.buildrc

CSRCS	=	buildnames.c	\
		build_fname.c	\
		build_imgnames.c	\
		build_mskname.c	\
		build_mnames.c	\
		fillimglist.c	\
		filterimg.c	\
		filteredname.c	\
		filter_suffix.c	\
		freesxr.c	\
		getatlas_dh68.c	\
		getatlas_hd6.c	\
		getatlas_t88.c	\
		getatlas_t88br.c	\
		get_imglist.c	\
		get_maplist.c	\
		gethdr.c	\
		gethdrpath.c	\
		getmap.c	\
		getpath.c	\
		getsxr.c	\
		getuser.c	\
		intdate.c	\
		itoa.c	\
		lowercase.c	\
		petinfo.c	\
		procPET.c  \
		procPET2.c  \
		procOxygen.c	\
		procOxygen2.c	\
		processPET.c  \
		processOxygen.c	\
		readhdr.c	\
		readimglist.c	\
		readmaplist.c	\
		readprm.c	\
		recout.c	\
		savelist.c	\
		savesxr.c	\
		stringtype.c	\
		writehdr.c

HEADERS	= 
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

