# Library:	libastro
# Author:	NASA (http://heasarc.gsf.nasa.gov/cgi-bin/search/search.pl)
# Date:		Feb 2008
# Expects:	set environmental variables in
#		source ~/.idlrc

# IDL compiler alters bytes 1063, 1065, 1067; hence, cmp starts at 1069
OFFSET	= 1068
SHELL	= /bin/sh

HEADERS = 
PRO		= loess.pro	\
		med.pro	\
		rob_mapfit.pro	\
		rob_quarticfit.pro	\
		robust_planefit.pro	\
		robust_regress.pro	\
		robust_sigma.pro
COMP	= compile.pro
SAV		= ${PRO:.pro=.sav}
compile : ${SAV}

%.sav : %.pro
	idl_compile $<
 
checkin:
	ci ${PRO} ${COMP} ${HEADERS}

checkout:
	co ${PRO} ${COMP} ${HEADERS}

release: 
	@-rm -f Unchanged
	@for i in ${SAV}; do \
		if (cmp -s $$i ${NPG_DIR}/$$i ${OFFSET} ${OFFSET}); \
		then echo $$i >> Unchanged; \
		else echo "Updating ${NPG_DIR}/$$i"; \
			cp -f $$i ${NPG_DIR}/$$i; \
			chmod 644 ${NPG_DIR}/$$i; \
			chgrp videen ${NPG_DIR}/$$i; \
		fi; \
	done
	@if (ls Unchanged); then \
		echo "---------"; \
		cat Unchanged; \
	fi

clean:
	rm -f ${SAV}

cleanall:
	rm -f ${PRO} ${COMP} ${HEADERS} ${SAV} Unchanged

