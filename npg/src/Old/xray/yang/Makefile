#	Makefile:	xray
#	Author:		Tom Yang
#	Date:		November 13, 1992

BINPATH = /usr/local/npg
LIBPATH = /usr/local/npg/lib/xray
INCLUDE	= -I/usr/local/include 
HDRS    = xray.h 
SRCS	= SetParameters.c drawpet.c drawxray.c getmagf.c interp.c \
	  rdraster.c rotation.c xray.c xrayparm.c xwd.c
RSCS    = xray.callbacks xray.code xray.defaults xray.widgets 
OBJS    = ${SRCS:.c=.o}
LIBS    = -L/usr/lib/Motif1.2 -lXm -L/usr/lib/X11R5 -lXt -L/home/npggw/tom/src/xray/yang -lnpg -L/usr/lib/X11R5 -lX11 -lecat7 -lm
CC      = cc $(INCLUDE) -O -D_NO_PROTO

xray.bin: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

SetParameters.o: SetParameters.c $(HDRS) 
	$(CC) -c SetParameters.c 

drawpet.o: drawpet.c $(HDRS)
	$(CC) -c drawpet.c 

drawxray.o: drawxray.c $(HDRS)
	$(CC) -c drawxray.c 

getmagf.o: getmagf.c $(HDRS)
	$(CC) -c getmagf.c 

interp.o: interp.c $(HDRS)
	$(CC) -c interp.c 

rdraster.o: rdraster.c $(HDRS)
	$(CC) -c rdraster.c 

rotation.o: rotation.c $(HDRS)
	$(CC) -c rotation.c 

xray.o: xray.c $(RSCS) $(HDRS)
	$(CC) -c xray.c

xrayparm.o: xrayparm.c $(HDRS)
	$(CC) -c xrayparm.c

xwd.o: xwd.c $(HDRS) 
	$(CC) -c xwd.c

release:
	mv xray.bin ${BINPATH}
	cp xray.defaults ${LIBPATH}
	chmod 755 ${BINPATH}/xray.bin
	chgrp videen ${BINPATH}/xray.bin
	saversion ${BINPATH}/xray.bin Version

clean:
	rm $(OBJS)

checkout:
	co $(SRCS) $(HDRS) $(RSCS)
