#	Makefile:	libty.a
#	Author:		Tom Videen
#	Date:		14-Jan-93
#	Contains:	Tom Yang functions in libnpg and libecat7 which were not put in libimg
#
#	Note:  		build_shortnames.c is compiled with the -g option to get rid of UMR warning

LIBPATH = /usr/local/npg/lib
INCPATH = /home/npggw/tom/include
INCLUDES = -I${INCPATH} -I/usr/include/Motif1.2
CC = cc ${INCLUDES} -O -D_NO_PROTO
FC = f77 -I4 -O -e
#FC = f77 -I4 -O +es

CSRCS =	DrawSymbol.c	\
		GetImageSuperPett.c	\
		GetPetParameters.c	\
		GetTextStart.c	\
		GetVersion.c	\
		Lowered.c	\
		Transformation.c	\
		c3101.c \
		c_uvlc.c    \
		clip_suffix.c	\
		copyrec.c	\
		crash.c \
		d_uvlc.c    \
		ecat_model.c    \
		filter_image.c	\
		get_finfo.c	\
		get_voxsize.c	\
		getatlas_hd6.c	\
		getatlas_t88.c	\
		hd6tot88.c	\
		hdr2ifh.c	\
		llist.c	\
		make3dfilter.c  \
		make_matlist.c  \
		make_num_list.c \
		makefilter.c    \
		makelist.c	\
		mat_get_spec.c  \
		matpkg.c    \
		matrix_xdr.c    \
		numcomp.c   \
		package.c	\
		padimage.c	\
		parse_list.c    \
		parse_matnum.c  \
		pix_buttons.c	\
		plandefs.c  \
		read_header2.c	\
		readatl.c	\
		readmat.c	\
		rfa_xdr.c   \
		rts_cmd.c   \
		savelist.c	\
		savesxr.c	\
		sino_dets.c \
		slice2z.c	\
		threshold.c	\
		trim.c	\
		uvlc.c  \
		xmstr2str.c	\
		xor.c	\
		z2slice.c	\
		zoom.c

FSRCS = fftsun.f	\
		getimg.f	\
		imgval.f	\
		param6opr.f

HDRS = analyze.h    \
        compress.h  \
        ecatAcs.h   \
        ecatRts.h   \
        ecat_model.h    \
        fproj3d.h   \
        interfile.h \
        load_volume.h   \
        matpkg.h    \
        matrix.h    \
        matrix_64.h \
        matrix_xdr.h    \
        plandefs.h  \
        rfa_xdr.h   \
        scdef.h \
        scfun.h \
        scssl.h \
        sincos.h    \
        stdtypes.h  \
        uvlc.h

OBJS  = ${FSRCS:.f=.o} ${CSRCS:.c=.o} 

libty.a: ${OBJS}
	ar rcv $@ ${OBJS}

dclz.o : dclz.c
	${CC} -Dmain=dclz_main dclz.c

.c.o:
	${CC} -c $<

.f.o:
	${FC} -c $<

release: 
	mv libty.a ${LIBPATH}
	ranlib ${LIBPATH}/libty.a
	saversion ${LIBPATH}/libty.a Version

checkin: 
	ci ${CSRCS} ${FSRCS}

checkout: 
	co ${CSRCS} ${FSRCS}

clean: 
	rm ${OBJS}

cleanall:
	rm -f ${CSRCS} ${FSRCS} Version
