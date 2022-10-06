# $Header: /home/npggw/tom/src/libhp/RCS/Makefile,v 1.22 1995/10/31 15:30:39 tom Exp $
# $Log: Makefile,v $
# Revision 1.22  1995/10/31  15:30:39  tom
# tov version
#
#
#	Makefile:	libhp.a
#	Author:		Tom Videen
#	Date:		31-Oct-95
#	History:
#		Modification of Tom Yang version of Oct 95
#
#	Note: Compile build_shortnames.c with -g to get rid of UMR warning.

LIBPATH = /home/npggw/tom/lib
INCPATH = /home/npggw/tom/inc
INCLUDES = -I${INCPATH} -I/usr/include/X11R5 -I/usr/include/Motif1.2
CC = cc ${INCLUDES} -O -D_NO_PROTO
FC = f77 -I4 -O -e
#FC = f77 -I4 -O +es

CSRCS = DrawSymbol.c Ecat2Pett6.c FillHeadPett6.c GetImagePett6.c GetImageSuperPett.c\
	GetPetParameters.c GetTextStart.c GetVersion.c Lowered.c OpenEcat.c ProcessImage.c\
	ProcessOxygen.c PutImagePett6.c Transformation.c build_filter_filename.c\
	build_shortnames.c clip_suffix.c comment_info.c copyrec.c create_matrixx_data.c determine_type.c\
	filter_image.c filter_suffix.c get_finfo.c getifhdr.c get_voxsize.c getatlas_hd6.c getatlas_t88.c\
	gethdr.c getimage.c getimage_type.c getimagedim.c getrealimg.c getsxr.c getuserid.c hd6tot88.c\
	hdr2ifh.c ieee2pe.c init_subheader.c intdate.c isecat.c itoa.c llist.c makelist.c matrix_mult.c\
	package.c padimage.c pe2ieee.c pix_buttons.c readatl.c read_header2.c readifhdr.c readifimg.c\
	readimglist.c readmat.c readparm.c reflection.c savelist.c savesxr.c slice2z.c threshold.c trim.c\
	updateifhdr.c writeifhdr.c writeifimg.c writerealimg.c xmstr2str.c xor.c z2slice.c zoom.c

FSRCS = fftsun.f getimg.f imgval.f param6opr.f
OBJS  = ${FSRCS:.f=.o} ${CSRCS:.c=.o} 

libhp.a: ${OBJS}
	ar rcv $@ ${OBJS}

build_shortnames.o: build_shortnames.c
	cc ${INCLUDES} -g -D_NO_PROTO -c build_shortnames.c

.c.o:
	${CC} -c $<

.f.o:
	${FC} -c $<

release: 
	mv libhp.a ${LIBPATH}
	ranlib ${LIBPATH}/libhp.a
	saversion ${LIBPATH}/libhp.a Version

checkin:
	ci ${CSRCS} ${FSRCS}

checkout: 
	co ${CSRCS} ${FSRCS}

clean: 
	rm ${OBJS}

cleanall:
	rm -f ${CSRCS} ${FSRCS} Version
