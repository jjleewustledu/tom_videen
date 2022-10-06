LIBNAME = libimage
# Author:	Tom Videen
# Date:		Dec 2002
# Set environmental variables in:	source ~/.buildrc

CSRCS =	close_imgfile.c	\
		comment2info.c	\
		create_dir7.c	\
		existfif.c	\
		existfrm6.c	\
		existfrm7.c	\
		existimgfrm.c	\
		existp6.c	\
		fill_mh6.c	\
		fill_mh7.c	\
		fill_hp6.c	\
		fill_sh6.c	\
		fill_sh7.c	\
		firstimgfrm.c	\
		flipimg.c	\
		free_imgfile.c	\
		get_ifh.c	\
		get_hp6.c	\
		get_mh.c	\
		get_mh6.c	\
		get_mh7.c	\
		get_sa.c	\
		get_sa6.c	\
		get_sa7.c	\
		get_sh.c	\
		get_sh6.c	\
		get_sh7.c	\
		get_ss.c	\
		get_ss6.c	\
		get_ss7.c	\
		getfiletype.c	\
		getmatpos.c	\
		getstr.c	\
		ieee2pe.c	\
		info2comment.c	\
		make_hp6.c	\
		make_imgfile.c	\
		make_mh.c	\
		make_mh6.c	\
		make_mh7.c	\
		make_sa6.c	\
		make_sa7.c	\
		make_sh.c	\
		make_sh6.c	\
		make_sh7.c	\
		make_ss6.c	\
		make_ss7.c	\
		mhnames6.c	\
		mhnames7.c	\
		numcode7.c	\
		open_if.c	\
		open_imgfile.c	\
		open_m6.c	\
		open_m7.c	\
		open_p6.c	\
		padplane.c	\
		pe2ieee.c	\
		put_hp6.c	\
		put_mh.c	\
		put_mhif.c	\
		put_mh6.c	\
		put_mh7.c	\
		put_sa7.c	\
		put_sh.c	\
		put_sh6.c	\
		put_sh7.c	\
		put_ss.c	\
		put_ss6.c	\
		put_ss7.c	\
		rdattn6.c	\
		rdattn7.c	\
		rdimg.c	\
		rdimg6.c	\
		rdimg7.c	\
		rdimgif.c	\
		rdimgp6.c	\
		rdrec.c	\
		rdscan6.c	\
		rdscan7.c	\
		save_rcs.c	\
		scale_sh.c	\
		scale_sh6.c	\
		scale_sh7.c	\
		update_mh.c	\
		update_mh6.c	\
		update_mh7.c	\
		update_hp6.c	\
		update_sh.c	\
		update_sh6.c	\
		update_sh7.c	\
		use_mh6.c	\
		use_mh7.c	\
		use_mhif.c	\
		use_mhp6.c	\
		use_sa6.c	\
		use_sa7.c	\
		use_sh6.c	\
		use_sh7.c	\
		use_shif.c	\
		use_shp6.c	\
		use_ss6.c	\
		use_ss7.c	\
		wrattn6.c	\
		wrattn7.c	\
		wrimg6.c	\
		wrimg7.c	\
		wrimgif.c	\
		wrimgp6.c	\
		wrscan.c	\
		wrscan6.c	\
		wrscan7.c	\
		wrtrec.c
HEADERS	= 

OBJS	= ${CSRCS:.c=.o}
LINT	= ${CSRCS:.c=.ln}
INCS	= -I${INCPATH}
BCC		= ${PURE} ${CC} ${INCS} ${CFLAGS}

${LIBNAME}.a: ${OBJS}
	ar rcv $@ ${OBJS}

.c.o: ${HEADERS}
	${BCC} -c $<

release:
	mv ${LIBNAME}.a ${LIBPATH}
	chmod 644 ${LIBPATH}/${LIBNAME}.a
	chgrp videen ${LIBPATH}/${LIBNAME}.a
	ranlib ${LIBPATH}/${LIBNAME}.a

version:
	saversion ${LIBPATH}/${LIBNAME}.a Version

checkout:
	co ${CSRCS} ${HEADERS}

checkin:
	ci ${CSRCS} ${HEADERS}

lock:
	co -l ${CSRCS} ${HEADERS}

lint:
	lint ${LPARMS} -u ${INCS} ${CSRCS}

splint:
	splint ${SPLPARMS} ${INCS} ${CSRCS}

clean:
	rm ${OBJS} ${LINT}
	/usr/local/pkg/purify/pure_remove_old_files . 0

cleanall:
	rm -f ${CSRCS} ${HEADERS} Version

