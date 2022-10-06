#$Log: Makefile,v $
#	Makefile:	libnpg.a
#	Author:		Tom Yang
#	Date:	Copied from TY's home directory 11-Sep-96 (but doesn't work yet...)
#
# Note: Please compile build_shortnames.c with the -g option to get rid of
# the UMR warning.

LIBPATH = /usr/local/npg/lib
INCLUDE	= -I/usr/local/include -I/usr/openwin/include 

CC = cc $(INCLUDE) -O -D_NO_PROTO
FC = f77 -I4 -O -e

CSRCS = DrawSymbol.c Ecat2Pett6.c FillHeadPett6.c GetImagePett6.c GetImageSuperPett.c\
GetPetParameters.c GetTextStart.c GetVersion.c Lowered.c OpenEcat.c ProcessImage.c\
ProcessOxygen.c PutImagePett6.c Transformation.c build_filter_filename.c\
build_shortnames.c clip_suffix.c comment_info.c copyrec.c create_matrixx_data.c determine_type.c\
filter_image.c filter_suffix.c getifhdr.c get_voxsize.c getatlas_hd6.c getatlas_t88.c\
gethdr.c getimage.c getimage_type.c getimagedim.c getrealimg.c getsxr.c getuserid.c hd6tot88.c\
hdr2ifh.c ieee2pe.c init_subheader.c intdate.c isecat.c itoa.c llist.c matrix_mult.c\
package.c padimage.c pe2ieee.c pix_buttons.c readatl.c read_header2.c readifhdr.c readifimg.c readimglist.c \
readmat.c readparm.c reflection.c savelist.c savesxr.c slice2z.c threshold.c trim.c updateifhdr.c \
writeifhdr.c writeifimg.c writerealimg.c xmstr2str.c xor.c z2slice.c zoom.c \
image_type.c

FSRCS = get_date_log.f getimg.f fftsun.f imgval.f param6opr.f

OBJS  = ${FSRCS:.f=.o} build_shortnames.o ${CSRCS:.c=.o} 

libnpg.a: $(OBJS)
	ar rv $@ $(OBJS)

build_shortnames.o: build_shortnames.c
	cc $(INCLUDE) -g -D_NO_PROTO -c build_shortnames.c

.c.o:
	$(CC) -c $<

.f.o:
	$(FC) -c $<

release: 
	mv libnpg.a ${LIBPATH}
	ranlib ${LIBPATH}/libnpg.a
	saversion ${LIBPATH}/libnpg.a Version

clean: 
	rm $(OBJS) *.a

checkout: 
	co $(CSRCS) build_shortnames.c $(FSRCS)
