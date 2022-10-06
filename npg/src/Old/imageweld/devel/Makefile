#$Id: Makefile,v 1.3 1993/10/20 16:04:14 ty7777 Exp $
#$Log: Makefile,v $
# Revision 1.3  1993/10/20  16:04:14  ty7777
# Release version for correcting movement artifacts.
#
# Revision 1.2  1993/07/23  16:15:53  ty7777
# imageweld Makefile, adding release.
#
# Revision 1.1  1993/05/05  15:19:24  ty7777
# Initial revision
#
#
#		Makefile:	imageweld
#		Author:		Tom Yang
#		Date:		05-May-93
#
INCLUDES = -I/usr/local/include -I/usr/include/X11R5 -I/usr/include/Motif1.2
CC = cc -O $(INCLUDES) 
FC = f77 -I4 -O -e
CSRCS = BuildTransform.c GetIndexingScanOffset.c GetSamplingMode.c WeldImages.c \
	getfactor.c imageweld.c readmat.c
FSRCS = imgval.f param6opr.f 
OBJS  = ${FSRCS:.f=.o} ${CSRCS:.c=.o}

imageweld.bin: ${OBJS}
	$(FC) -o $@ ${OBJS} -lpetarc -lhp -lcti -lm

.c.o:
	$(CC) -c $<

.f.o:
	$(FC) -c $<

release:
	cp imageweld.bin /usr/local/bin
	saversion imageweld.bin Version

clean:
	rm ${OBJS}

checkout:
	co $(CSRCS) $(FSRCS) 

checkin:
	ci $(CSRCS) $(FSRCS) 
