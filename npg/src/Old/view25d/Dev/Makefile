#$Id: Makefile,v 1.5 1994/10/19 20:16:17 ty7777 Exp $
#$Log: Makefile,v $
# Revision 1.5  1994/10/19  20:16:17  ty7777
# Regrouped HDRS, SRCS, etc.
#
# Revision 1.4  1994/10/19  19:40:13  ty7777
# Added parameters for compiling on the HPs.
#
#
#
#		Makefile:	view25d
#		Author:		Tom Yang
#		Date:		Feruary 1, 1991
#
#
INCLUDE	= -I/usr/local/include 
HDRS    = app.h gammaz.h portal.h view25d.h 
SRCS    = colormap.c convert16to8.c interp.c portal.c view25d.c xwd.c zoom.c
RSCS    = view25d.callbacks view25d.code view25d.defaults view25d.widgets 
OBJS    = ${SRCS:.c=.o}
LIBS    = -L/usr/lib/Motif1.2 -lXm -L/usr/lib/X11R5 -lXt -lhp -L/usr/lib/X11R5 -lX11 -lcti -lm
CC      = cc $(INCLUDE) -O -D_NO_PROTO
#CC     = cc -Wp,-H,256000 $(INCLUDE) -O -D_NO_PROTO

view25d.bin: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) 
#	$(CC) -Wl,-a,archive -o $@ $(OBJS) $(LIBS)

convert16to8.o: convert16to8.c portal.h
	$(CC) -c convert16to8.c

colormap.o: colormap.c 
	$(CC) -c colormap.c

portal.o: portal.c portal.h
	$(CC) -c portal.c

interp.o: interp.c
	$(CC) -c interp.c

zoom.o: zoom.c
	$(CC) -c zoom.c

xwd.o: xwd.c portal.h
	$(CC) -c xwd.c

view25d.o: view25d.c $(RSCS)
	$(CC) -c view25d.c

release:
	/usr/bin/cp view25d.bin /usr/local/bin
	/usr/bin/cp view25d.defaults /usr/local/lib/view25d/view25d.defaults
	saversion view25d.bin Version

cflow:
	cflow ${INCLUDE} ${SRCS}

checkout:
	co $(HDRS) $(SRCS) $(RSCS)

clean:
	rm $(OBJS) 

cleanall:
	rm -f $(HDRS) $(SRCS) $(RSCS)
