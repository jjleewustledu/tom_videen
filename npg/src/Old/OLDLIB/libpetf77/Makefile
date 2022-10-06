# $Id: Makefile,v 1.4 1995/12/01 15:04:44 tom Exp $
# $Log: Makefile,v $
# Revision 1.4  1995/12/01  15:04:44  tom
# *** empty log message ***
#
# Revision 1.3  1995/10/31  15:11:52  tom
# X11R5
#
# Revision 1.2  1995/10/31  15:02:41  tom
# tov version
#
#
#	Makefile:	libpetf77.a
#	Author:		Tom Videen
#	Date:		31-Oct-95
#	History:
#		Modified Tom Yang version of 14-Oct-95
#	Requires:	matrix_64.h, matrix6.h, interfile.h

LIBPATH = /usr/local/npg/lib
INCPATH	= /home/npggw/tom/include
CC = cc -I${INCPATH} -I/usr/include/X11R5 -O -D_NO_PROTO
FC = f77 -O -e 

CSRCS = DrawSymbol.c Ecat2Pett6.c FillHeadPett6.c GetImagePett6.c \
	GetImageSuperPett.c OpenEcat.c PutImagePett6.c comment_info.c \
	create_matrixx_data.c getimage.c getimage_type.c getuserid.c \
	get_voxsize.c get_zorigin.c ieee2pe.c itoa.c init_subheader.c \
	intdate.c llist.c package.c pe2ieee.c pet_matrix_open.c \
	pix_buttons.c reflection.c xmstr2str.c zoom.c

FSRCS = getimg.f 
OBJS = ${FSRCS:.f=.o} ${CSRCS:.c=.o}

libpetf77.a: ${OBJS}
	ar rcv $@ ${OBJS}

.c.o:
	${CC} -c $<

.f.o:
	${FC} -c $<

release: 
	mv libpetf77.a ${LIBPATH}
	ranlib ${LIBPATH}/libpetf77.a
	saversion ${LIBPATH}/libpetf77.a Version

checkin: 
	ci ${CSRCS} ${FSRCS}

checkout: 
	co ${CSRCS} ${FSRCS}

clean:
	rm ${OBJS}

cleanall:
	rm -f ${CSRCS} ${FSRCS} Version
