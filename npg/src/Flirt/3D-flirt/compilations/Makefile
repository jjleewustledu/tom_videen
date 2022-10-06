VER = 1.4
ROOTPATH = /home/npggw/tom/src/Flirt/3D-flirt
FFTWPATH = /usr/local/npg/fftw-3.1.2

INCS = -I/usr/include -I${ROOTPATH}/include -I${FFTWPATH}/include
LIBS = -L/usr/lib -L${ROOTPATH}/lib -L${FFTWPATH}/lib -lflirt++ -lflirt -lfftw3 -ltiff
CPP  = CC -O3

CSRCS = \
	applywarp.cc \
	elastic3D.cc

all: applywarp elastic3D

elastic3D: 
	${CPP} ${INCS} elastic3D.cc -o elastic3D ${LIBS}
	mv elastic3D /usr/local/npg/bin

applywarp:
	${CPP} ${INCS} applywarp.cc -o applywarp ${LIBS}
	mv applywarp /usr/local/npg/bin

OBJS = ${CSRCS:.cc=.o}

checkout:
	co ${CSRCS}

checkin:
	ci ${CSRCS} 

sync:
	ci -f -r${VER} ${CSRCS}

lock:
	co -l ${CSRCS}

clean: 
	rm ${OBJS}

