#	$Log$
#
#		Makefile:	normecat
#		Author:		Tom Videen
#		Date:			28-Jan-93

FFLAGS= -O -e +E0

PET_LIB = -lpe -lpetutil
CTI_LIB	= -ldc -llibrary -lnil
SUNVIEW	= -lsuntool -lsunwindow -lpixrect -lm

OBJS =	normecat.o

normecat.bin: ${OBJS}
	f77 -O -o normecat.bin ${OBJS} ${PET_LIB} ${CTI_LIB} ${SUNVIEW}

normecat.o : normecat.f
	f77 ${FFLAGS} -c normecat.f
 
release:
	chmod 755 normecat.bin
	mv normecat.bin /home/petsun2/tom/bin

clean: 
	rm ${OBJS}
