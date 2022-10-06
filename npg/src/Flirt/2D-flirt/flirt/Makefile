LIBNAME = libflirt.a

ROOTPATH = /home/npggw/tom/src/Flirt/2D-flirt
FFTWPATH = /usr/local/npg/fftw-3.1.2
LIBPATH  = ${ROOTPATH}/lib

LIBS  = -L/usr/lib -L${ROOTPATH}/lib -L${FFTWPATH}/lib -lfftw3 -ltiff -lm
INCS  = -I/usr/include -I${ROOTPATH}/include -I${FFTWPATH}/include
COPTS = -O3
BCC   = CC ${INCS} ${COPTS}

CSRCS = \
	imagetools.c \
	curvature.c \
	diffusive.c \
	tools.c \
	arrays.c \
	bspline.c \
	io.c \
	elastic.c 

OBJS = ${CSRCS:.c=.o}

${LIBNAME}: ${OBJS}
	ar rcv $@ ${OBJS}

.c.o: 
	${BCC} -c $<

release: ${LIBNAME}	
	mv ${LIBNAME} ${LIBPATH}
	ranlib ${LIBPATH}/${LIBNAME}

version:
	saversion ${LIBPATH}/${LIBNAME} Version

checkout:
	co ${CSRCS}

checkin:
	ci ${CSRCS}

sync:
	ci -f -r${VER} ${CSRCS}

lock:
	co -l ${CSRCS}

clean: 
	rm -f *~ ${OBJS}

