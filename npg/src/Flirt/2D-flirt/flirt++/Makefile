LIBNAME = libflirt++.a

ROOTPATH = /home/npggw/tom/src/Flirt/2D-flirt
FFTWPATH = /usr/local/npg/fftw-3.1.2
LIBPATH = ${ROOTPATH}/lib

INCS = -I/usr/include -I${ROOTPATH}/include -I${FFTWPATH}/include
LIBS = -L/usr/lib -L${ROOTPATH}/lib -L${FFTWPATH}/lib -lflirt -lfftw3 -ltiff -lm
CPP  = CC -O3 ${INCS}

CSRCS = \
	Array.cpp \
	Displacement.cpp \
	DistanceMeasure.cpp \
	Image.cpp \
	Log.cpp \
	Optimizer.cpp \
	Smoother.cpp \
	Solver.cpp \
	StopRule.cpp \
	UpdateRule.cpp

OBJS = ${CSRCS:.cpp=.o}

${LIBNAME}: ${OBJS}
	ar rcv $@ ${OBJS}

${OBJS}: ${CSRCS}
	${CPP} -c ${CSRCS} 

release: ${LIBNAME}
	mv ${LIBNAME} ${LIBPATH}
	ranlib ${LIBPATH}/${LIBNAME}

version:
	saversion ${LIBPATH}/${LIBNAME}.a Version

checkout:
	co ${CSRCS}

checkin:
	ci ${CSRCS} 

sync:
	ci -f -r${VER} ${CSRCS}

lock:
	co -l ${CSRCS}

clean: 
	rm -f *~ *.o ${OBJS}

