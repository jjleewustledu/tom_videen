#	$Log$
#
#		Makefile:	pixnoise
#		Author:		Tom Videen
#		Date:			19-Jan-93

FFLAGS= -O -e +E0

PET_LIB = -lpe -lpetutil
CTI_LIB	= -ldc -llibrary -lnil
SUNVIEW	= -lsuntool -lsunwindow -lpixrect -lm

OBJS =	pixnoise.o

pixnoise.bin: ${OBJS}
	f77 -O -o pixnoise.bin ${OBJS} ${PET_LIB} ${CTI_LIB} ${SUNVIEW}

pixnoise.o : pixnoise.f
	f77 ${FFLAGS} -c pixnoise.f
 
release:
	chmod 755 pixnoise.bin
	mv pixnoise.bin /home/petsun2/tom/bin

clean: 
	rm ${OBJS}
