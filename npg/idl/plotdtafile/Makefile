PRGNAME = plotdtafile
# Author:	Tom Videen
# Date:		Mar 2007
# Expects:	set environmental variables in
#		source ~/.idlrc

HEADERS	= 
SCRIPTS	= ${PRGNAME}
PRO =	${PRGNAME}.pro \
		getnum.pro \
		getstr.pro \
	 	plot_bloodcrv.pro \
	 	print_bloodcrv.pro \
	 	read_bloodcrv.pro \
	 	read_dta.pro \
	 	read_headcrv.pro \
	 	zero_time.pro
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
