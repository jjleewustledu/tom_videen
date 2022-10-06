# Library:	libutils
# Author:	Tom Videen
# Date:		Nov 2002
# Expects:	set environmental variables in
#		source ~/.idlrc

# IDL compiler alters bytes 1063, 1065, 1067; hence, cmp starts at 1069
OFFSET	= 1068
SHELL	= /bin/sh

HEADERS = 
PRO		= intersect.pro \
		mapfitw.pro	\
		runlength.pro \
		setintersection.pro \
		type.pro
#		sfit_xy.pro 
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
