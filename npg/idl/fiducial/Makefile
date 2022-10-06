PRGNAME = fiducial
# Author:	Tom Videen
# Date:		Dec 2005
# Expects:	set environmental variables in
#		source ~/.idlrc

HEADERS	= FID.h
SCRIPTS	= ${PRGNAME}
PRO    = fiducial.pro \
		fid_display.pro \
		fid_line.pro	\
		fid_points.pro	\
		fid_scale.pro	\
		fid_tilt.pro
COMP	= compile.pro
SAV		= ${PRGNAME}.sav
VPATH	= ${NPG_DIR}
 
${SAV}: ${PRO} ${HEADERS}
	idl_compile ${PRGNAME}.pro
 
checkin:
	ci ${PRO} ${COMP} ${SCRIPTS} ${HEADERS}

checkout:
	co ${PRO} ${COMP} ${SCRIPTS} ${HEADERS}

lock:
	co -l ${PRO} ${COMP} ${SCRIPTS} ${HEADERS}

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
