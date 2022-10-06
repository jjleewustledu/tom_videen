# Update installed scripts
# Author:	Tom Videen
# Date:		Nov 2002
# Expects:	set environmental variables in
#				source ~/.buildrc

SHELL	= /bin/sh
SCRIPTS = align_oxy \
		align_oxy2 \
		align_ho \
		check_oxy2 \
		get_oxy \
		total_shift_oxy2

default:
	@-echo "Options are checkout, checkin, release, cleanall"

checkout:
	co ${SCRIPTS}

unlock:
	co -l ${SCRIPTS}

checkin:
	ci ${SCRIPTS}

release: 
	@-rm -f Unchanged
	@for i in ${SCRIPTS}; do \
		if (cmp -s $$i ${BINPATH}/$$i); \
		then echo $$i >> Unchanged; \
		else echo "Updating ${BINPATH}/$$i"; \
			cp -f $$i ${BINPATH}/$$i; \
			chmod 755 ${BINPATH}/$$i; \
			chgrp videen ${BINPATH}/$$i; \
		fi; \
	done
	@if (ls Unchanged); then \
		echo "---------"; \
		cat Unchanged; \
	fi

cleanall:
	rm -f ${SCRIPTS} Unchanged
