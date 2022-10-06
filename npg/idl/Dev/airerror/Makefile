#$Log: Makefile,v $
# Revision 1.1  2000/05/17  17:02:04  tom
# Initial revision
#
#	Makefile:	airerror (IDL routines)
#	Author:		Tom Videen
#	Date:		Jan-96
#
#	Routines shared with fit_fudicials
#		analyze_dim
#		center_of_mass
#		center_of_mazz
#		exist_file
#		nonzero_coords
#		plot_fiducial

BINDIR = /usr/local/npg/idl
COMP = compile
PRO =	airerror.pro

SAV = ${PRO:.pro=.sav}
SCRIPTS = AIRERROR airerror

checkin:
	ci ${PRO} ${COMP} ${SCRIPTS}

checkout:
	co ${PRO} ${COMP} ${SCRIPTS}

release:
	chgrp tov ${SAV}
	mv ${SAV} ${BINDIR}

cleanall:
	rm -f ${PRO} ${COMP} ${SCRIPTS}
