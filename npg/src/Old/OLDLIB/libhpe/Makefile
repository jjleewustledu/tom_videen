#$Id: Makefile,v 1.6 1993/11/18 19:47:46 ty7777 Exp $
#$Log: Makefile,v $
# Revision 1.6  1993/11/18  19:47:46  ty7777
# After finishing testing.
#
# Revision 1.5  1993/11/10  18:33:43  ty7777
# Before testing.
#
# Revision 1.4  1993/03/26  20:50:08  ty7777
# Before moving to short format.
#
# Revision 1.3  1993/03/05  22:04:48  ty7777
# Sun version of Makefile for libpe.a.
#
# Revision 1.2  1992/10/13  18:56:39  ty7777
# Change check_out to checkout.
#
# Revision 1.1  1992/10/13  18:55:00  ty7777
# Initial revision
#
#		Makefile:	libpe
#		Author:		Tom Yang
#		Date:		12-Apr-92

BINPATH = /usr/local/npg
#CFLAGS = -O -I/usr/local/include -I/usr/include/X11R5
#FFLAGS = -C -O -e
CFLAGS = -DHP -O -I/usr/local/include -I/usr/include/X11R5
FFLAGS = -DHP -C -O +es

CSRCS = atoi.c checkpath.c clseek.c cramtek.c cread.c cwrite.c fitoa.c \
	freverse.c getch.c getcnw.c gettime.c interrupt.c pefloat.c \
	print.c putch.c

FSRCS = chfield.f cursoff.f curson.f ersfield.f framtek.f getchar.f \
	getcolor.f getint.f getreal.f getstrng.f gtstring.f ianyl.f \
	ianyr.f inanyl.f inanyr.f inchar.f infield.f intfield.f itoc.f \
	logfield.f openfile.f prompt.f query.f relfield.f relformat.f \
	sysio.f tcontrol.f termset.f termtype.f trmfind.f upblank.f \
	uppercas.f writeint.f writerel.f wrtfield.f yesno.f

OBJS= ${FSRCS:.f=.o} ${CSRCS:.c=.o}

libpe.a : ${OBJS}
	ar ruv $@ $?

.c.o :
	cc ${CFLAGS} -c $<

.f.o :
	fort77 ${FFLAGS} -c $<

release:
	cp libpe.a ${BINPATH}
	ranlib ${BINPATH}/libpe.a

clean:
	yes | rm ${OBJS}

checkout:
	co ${CSRCS} ${FSRCS}
