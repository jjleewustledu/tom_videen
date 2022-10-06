LIBPATH = /usr/local/npg/lib
BINPATH = /usr/local/npg/bin
LIBS = -L${LIBPATH} -lpe -lm -lnsl

FSRCS =	atenprof.f atensize.f backproj.f binprof.f \
	coratten.f corscan.f detnorm.f fconvolv.f interprf.f \
	mask6a.f normave.f recdyn.f slicesep.f startdrhp.f

OBJS = ${FSRCS:.f=.o}

SCRIPTS = pronorm6 protran6 recon6 startdr6 cleanid6

all: atenprof atensize backproj binprof \
	coratten corscan detnorm mask6a normave \
	recdyn slicesep startdrhp.exe

atenprof: atenprof.o
	f77 ${FFLAGS} -o atenprof atenprof.o ${LIBS}
 
atensize: atensize.o
	f77 ${FFLAGS} -o atensize atensize.o ${LIBS}
 
backproj: backproj.o fconvolv.o interprf.o
	f77 ${FFLAGS} -o backproj backproj.o fconvolv.o interprf.o ${LIBS}

binprof: binprof.o
	f77 ${FFLAGS} -o binprof binprof.o ${LIBS}
		  
coratten: coratten.o
	f77 ${FFLAGS} -o coratten coratten.o ${LIBS}

corscan: corscan.o
	f77 ${FFLAGS} -o corscan corscan.o ${LIBS}
		  
detnorm: detnorm.o
	f77 ${FFLAGS} -o detnorm detnorm.o ${LIBS}
		  
mask6a:	  mask6a.o
	f77 ${FFLAGS} -o mask6a mask6a.o ${LIBS}
		  
normave: normave.o
	f77 ${FFLAGS} -o normave normave.o ${LIBS}
		  
recdyn:	recdyn.o
	f77 ${FFLAGS} -o recdyn recdyn.o ${LIBS}
		  
slicesep: slicesep.o
	f77 ${FFLAGS} -o slicesep slicesep.o ${LIBS}

startdrhp.exe: startdrhp.o
	f77 ${FFLAGS} -o startdrhp.exe startdrhp.o ${LIBS}
		  
checkin:
	ci ${FSRCS} ${SCRIPTS}

checkout:
	co ${FSRCS} ${SCRIPTS}

clean:
	rm ${OBJS}

cleanall:
	rm -f ${FSRCS} ${SCRIPTS}

release:
	mv atenprof ${BINPATH}
	mv atensize ${BINPATH}
	mv backproj ${BINPATH}
	mv binprof ${BINPATH}
	mv coratten ${BINPATH}
	mv corscan ${BINPATH}
	mv detnorm ${BINPATH}
	mv mask6a ${BINPATH}
	mv normave ${BINPATH}
	mv recdyn ${BINPATH}
	mv slicesep ${BINPATH}
	mv startdrhp.exe ${BINPATH}

