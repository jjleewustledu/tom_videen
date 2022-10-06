PRGNAME = vidi
# Author:	Tom Videen
# Date:		May 2005
# Expects:	set environmental variables in
#		source ~/.idlrc

VER = 2.5
HEADERS	= VIDI.h
SCRIPTS	= ${PRGNAME}
PRO		= vidi.pro \
		vidi_atlas.pro \
		vidi_defvoi.pro \
		vidi_display.pro \
		vidi_edit.pro \
		vidi_mask.pro \
		vidi_morph.pro \
		vidi_morph2d.pro \
		vidi_outline.pro \
		vidi_peaks.pro \
		vidi_points.pro \
		vidi_process.pro \
		vidi_scale.pro \
		vidi_segment.pro \
		vidi_threshold.pro \
		vidi_viewer.pro \
		vidi_viewer_display.pro \
		getatlhdr.pro
COMP	= compile.pro
SAV		= ${PRGNAME}.sav
VPATH	= ${NPG_DIR}
 
${SAV}: ${PRO} ${HEADERS} IMAGE_UTILS.h
	idl_compile ${PRGNAME}.pro
 
checkin:
	ci ${PRO} ${COMP} ${SCRIPTS} ${HEADERS}

checkout:
	co ${PRO} ${COMP} ${SCRIPTS} ${HEADERS}

lock:
	co -l ${PRO} ${COMP} ${SCRIPTS} ${HEADERS}

sync:
	ci -f -r${VER} ${PRO} ${COMP} ${SCRIPTS} ${HEADERS}

release: ${SAV}
	mv ${SAV} ${NPG_DIR}
	@ cd ${NPG_DIR}; chmod 644 $?; chgrp videen $?

script: ${SCRIPTS}
	cp ${SCRIPTS} ${BINPATH}
	@ cd ${BINPATH}; chmod 755 $?; chgrp videen $?

clean:
	rm -f ${SAV}

cleanall:
	rm -f ${PRO} ${COMP} ${SCRIPTS} ${HEADERS}

