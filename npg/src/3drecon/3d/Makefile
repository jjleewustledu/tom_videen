# $Header: /home/npggw/tom/src/3drecon/RCS/Makefile,v 1.6 1995/01/30 15:56:36 ty7777 Exp $
# $Log: Makefile,v $
# Revision 1.6  1995/01/30  15:56:36  ty7777
# Added import_3dscan_nonorm.c.
#
# Revision 1.5  1995/01/25  15:38:48  ty7777
# Deleted -lhp.
#
# Revision 1.4  1995/01/24  19:41:39  ty7777
# Added libhp.a
#
# Revision 1.3  1993/03/19  23:04:22  ty7777
# Add yes to rm.
#
# Revision 1.2  1993/03/19  21:43:53  ty7777
# Makefile modified by Tom Yang to make it work general makefile.
#
# Revision 1.1  1993/03/18  15:42:44  ty7777
# Initial revision
#
#
#   Makefile: 3D reconstruction programs
#   Authors:  Tom Yang and Tom Videen
#   Date:     03-Feb-93

SRCPATH = ..
BINPATH = ${SRCPATH}/bin
INCLUDE = ${SRCPATH}/include
CFLAGS  = -O -DNO_CS -I${INCLUDE} 
LIBS    = ${SRCPATH}/include/libmine.a ${SRCPATH}/include/libcsemu.a -lm
VPATH   = ${BINPATH}
.SUFFIXES: .c

CSRCS = apply_fp_attn.c \
        bkproj.c \
        bkproj3d.c \
        fit_scans.c \
        compute_3d_eff.c \
        extract_summed_planes.c \
        fproj_volume.c \
        import_3dscan.c \
        import_3dscan_nonorm.c \
        list_sinograms.c \
        matnumx.c

EXECS = ${CSRCS:.c= } 

all: ${EXECS}

.c:
	cc $< ${CFLAGS} ${LIBS} -o ../bin/$@

checkout:
	co ${CSRCS}

clean:
	cd ${BINPATH}; yes | rm ${EXECS}
