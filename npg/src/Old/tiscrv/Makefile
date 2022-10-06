#	$Log$
#
#		Makefile:	tiscrv
#		Author:		Tom Videen
#		Date:			19-Oct-93

FF = f77 -O -e 
PET_LIB = -lpe -lpetutil
CTI_LIB	= -ldc -llibrary -lnil
SUNVIEW	= -lsuntool -lsunwindow -lpixrect -lm

FSRCS =	tiscrv.f \
				rdata.f

OBJS = ${FSRCS:.f=.o} 

tiscrv.bin: ${OBJS}
	${FF} -o $@  ${OBJS} ${PET_LIB} ${CTI_LIB} ${SUNVIEW}

.f.o :
	${FF} -c $<

release:
	chmod 775 tiscrv.bin
	mv tiscrv.bin /usr/local/bin/tiscrv.bin
	saversion /usr/local/bin/tiscrv.bin Version

checkout:
	co ${FSRCS}
 
checkin:
	ci ${FSRCS}
 
clean: 
	rm ${OBJS}
