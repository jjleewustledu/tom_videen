#	$Log$
#
#		Makefile:	imgrot
#		Author:		Tom Videen
#		Date:			19-Jul-93

FF = f77 -g -e
CTI_LIB	= -ldc -llibrary -lnil
SUNVIEW	= -lsuntool -lsunwindow -lpixrect -lm

FSRCS =	imgroto.f \
				imgrot.f \
				imgrote.f \
				imgrotr.f \
				alignh1.f \
				polfit.f \
				rmssub1.f \
				rmssub.f \
				split.f \
				matopr.f

OBJS = ${FSRCS:.f=.o} ${CSRCS:.c=.o}

imgrot.bin: ${OBJS}
	${FF} -o $@ ${OBJS} -lpetutil -lpe $(CTI_LIB) $(SUNVIEW)

.f.o:
	${FF} -c $<

checkin:
	ci -m"revision" ${FSRCS}

checkout:
	co -r1.1 ${FSRCS}

clean:
	rm ${OBJS}
