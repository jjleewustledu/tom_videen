# $Header: /export/home/npggw/tom/src/recon3d/RCS/Makefile,v 1.7 1995/01/25 15:37:02 ty7777 Exp $
# $Log: Makefile,v $
# Revision 1.7  1995/01/25  15:37:02  ty7777
# Added processing for GetVersion.c.
#
# Revision 1.6  1995/01/24  21:53:47  ty7777
# Added GetVersion.
#
# Revision 1.5  1993/03/26  21:51:39  ty7777
# Updated version of Makefile.
#
# Revision 1.4  1993/03/19  23:05:36  ty7777
# Add "yes" to rm.
#
# Revision 1.3  1993/03/19  22:22:32  ty7777
# Added ranlib to make it also work for Sun.
#
# Revision 1.2  1993/03/19  21:44:47  ty7777
# New Makefile.
#
# Revision 1.1  1993/03/18  15:48:16  ty7777
# Initial revision
#
#
#   Makefile: libmine.a (Byars library for HP)
#   Author:   Tom Yang and Tom Videen
#   Date:     03-Feb-93
#	      Modified by Tom Yang on March 19, 1993 to make it work with the genreal Makefile.
#

SRCPATH = ..
INCLUDE = ${SRCPATH}/include
CFLAGS = -O -I${INCLUDE}
BINPATH = ${SRCPATH}/include
VPATH = ${INCLUDE}
.SUFFIXES: .c .o

OBJS = crash.o \
       lib3d.o \
       make3dfilter.o \
       makefilter.o \
       mat_convert_float_scan.o \
       mat_dir_lookup.o \
       matnum_3d.o \
       matpkg.o \
       matrix.o \
       matrix_extra.o \
       matspec.o \
       rtsacs.o \
       sino_dets.o

CSRCS = ${OBJS:.o=.c} GetVersion.c

libmine.a: ${OBJS} GetVersion.o
	ar ruv ${BINPATH}/$@ $?
	ranlib ${BINPATH}/$@ 

.c.o:
	cc ${CFLAGS} -c $<

GetVersion.o: GetVersion.c
	cc -O -I/usr/local/include -c GetVersion.c

checkout:
	co ${CSRCS}

clean :
	yes | rm ${OBJS}
