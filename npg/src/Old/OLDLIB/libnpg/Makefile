#$Header: /home/npggw/tom/src/libnpg/RCS/Makefile,v 1.20 1995/11/10 23:47:18 tom Exp tom $
#$Log: Makefile,v $
# Revision 1.20  1995/11/10  23:47:18  tom
# *** empty log message ***
#
#	Makefile:	libnpg.a
#	Author:		Tom Yang
#	Date:		14-Jan-93
#
#	Note:  		build_shortnames.c is compiled with the -g option to get rid of UMR warning

LIBPATH = /usr/local/npg/lib
INCPATH = /home/npggw/tom/include
INCLUDES = -I${INCPATH} -I/usr/include/Motif1.2
CC = cc ${INCLUDES} -O -D_NO_PROTO
FC = f77 -I4 -O -e
#FC = f77 -I4 -O +es

CSRCS = DrawSymbol.c	\
		Ecat2Pett6.c	\
		FillHeadPett6.c	\
		GetImagePett6.c	\
		GetImageSuperPett.c	\
		GetPetParameters.c	\
		GetTextStart.c	\
		GetVersion.c	\
		Lowered.c	\
		OpenEcat.c	\
		ProcessImage.c	\
		ProcessOxygen.c	\
		PutImagePett6.c	\
		Transformation.c	\
		build_filter_filename.c	\
		build_shortnames.c	\
		clip_suffix.c	\
		comment_info.c	\
		copyrec.c	\
		create_matrixx_data.c	\
		determine_type.c	\
		filter_image.c	\
		filter_suffix.c	\
		get_finfo.c	\
		get_voxsize.c	\
		getatlas_hd6.c	\
		getatlas_t88.c	\
		gethdr.c	\
		getifhdr.c	\
		getimage.c	\
		getimage_type.c	\
		getimagedim.c	\
		getrealimg.c	\
		getsxr.c	\
		getuserid.c	\
		hd6tot88.c	\
		hdr2ifh.c	\
		ieee2pe.c	\
		init_subheader.c	\
		intdate.c	\
		isecat.c	\
		itoa.c	\
		llist.c	\
		makelist.c	\
		package.c	\
		padimage.c	\
		pe2ieee.c	\
		pix_buttons.c	\
		read_header2.c	\
		readatl.c	\
		readifhdr.c	\
		readifimg.c	\
		readimglist.c	\
		readmat.c	\
		readparm.c	\
		reflection.c	\
		savelist.c	\
		savesxr.c	\
		slice2z.c	\
		threshold.c	\
		trim.c	\
		updateifhdr.c	\
		writeifhdr.c	\
		writeifimg.c	\
		writerealimg.c	\
		xmstr2str.c	\
		xor.c	\
		z2slice.c	\
		zoom.c

FSRCS = fftsun.f	\
		getimg.f	\
		imgval.f	\
		param6opr.f

OBJS  = ${FSRCS:.f=.o} ${CSRCS:.c=.o} 

libnpg.a: ${OBJS}
	ar rcv $@ ${OBJS}

build_shortnames.o: build_shortnames.c
	cc ${INCLUDES} -g -D_NO_PROTO -c build_shortnames.c

.c.o:
	${CC} -c $<

.f.o:
	${FC} -c $<

release: 
	mv libnpg.a ${LIBPATH}
	ranlib ${LIBPATH}/libnpg.a
	saversion ${LIBPATH}/libnpg.a Version

checkin: 
	ci ${CSRCS} ${FSRCS}

checkout: 
	co ${CSRCS} ${FSRCS}

clean: 
	rm ${OBJS}

cleanall:
	rm -f ${CSRCS} ${FSRCS} Version
