# Library:	libimage
# Author:	Tom Videen
# Date:		Aug 2008
# Expects:	set environmental variables in
#		source ~/.ildrc

# IDL compiler alters bytes 1063, 1065, 1067; hence, cmp starts at 1069
OFFSET	= 1068
SHELL	= /bin/sh

HEADERS = 
PRO =	analyzehdr__define.pro \
		analyze_dim.pro \
		getanalyze.pro \
		getecat6.pro \
		getecat6frm.pro \
		getecat7.pro \
		getecat7frm.pro \
		getfiletype.pro \
		getframe.pro \
		getnumframes.pro \
		gethdr.pro \
		getinterfile.pro \
		getpett6.pro \
		getsingleframe.pro \
		header__define.pro \
		m6header__define.pro \
		m6header_init.pro \
		m7header__define.pro \
		m7header_init.pro \
		pett6header__define.pro \
		pett6header_init.pro \
		putinterfile.pro \
		vdisplay__define.pro \
		vimage__define.pro \
		voi__define.pro \
		voi_process.pro
COMP    = compile.pro
HEADERS = IMAGE_UTILS.h
SAV     = ${PRO:.pro=.sav}
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
