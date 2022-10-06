#	Makefile:	pcimages (IDL routines)
#	Author:		Tom Videen
#	Date:		May 2000

BINDIR  = /usr/local/npg/idl
PRO     = pcimages.pro
COMP    = compile.pro
SCRIPTS = pcimages
SAV     = pcimages.sav
 
${SAV}: ${PRO}
	idl_compile pcimages.pro
 
checkin:
	ci ${PRO} ${COMP} ${SCRIPTS}

checkout:
	co ${PRO} ${COMP} ${SCRIPTS}

release:
	mv ${SAV} ${BINDIR}
	chmod 644 ${BINDIR}/${SAV}
	chgrp videen ${BINDIR}/${SAV}

cleanall:
	rm -f ${PRO} ${COMP} ${SCRIPTS}
