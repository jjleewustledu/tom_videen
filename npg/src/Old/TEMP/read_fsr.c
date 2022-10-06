/*  $Id$
    $Log$ */
/* =============================================================================
 *	Function:		read_fsr
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Read file structure 
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/rdrec.c,v 1.3 2002/12/19 17:37:41 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <endian.h>

int             read_fsr (fd, dir, recnum)
	FILE           *fd;	
	struct directory      *dir;
	int             recnum;
{
	long            dskptr;
	long			buf[LENREC_L];
	int             stat, i, j;

	dskptr = (recnum - 1) * LENREC;
	stat = fseek (fd, dskptr, SEEK_SET);
	if (stat != 0) {
		(void) fprintf (stderr, "ERROR [read_fsr]: fseek %d\n", stat);
		return (ERROR);
	}

	stat = fread (buf, sizeof (long), LENREC_L, fd);
	if (stat != LENREC_L) {
		(void) fprintf (stderr, "ERROR [read_fsr]: fread %d\n", stat);
		return (ERROR);
	} 

    /* CONSTCOND */
    if (__BYTE_ORDER == __LITTLE_ENDIAN) {
        swap_byte ((char *) buf, LENREC);
        swap_word ((short *) buf, LENREC_S);
    }

	dir->hdr.numfree = buf[0];
	dir->hdr.forptr = buf[1];
	dir->hdr.bakptr = buf[2];
	dir->hdr.numused = buf[3];
	j = 4;
	for (i = 0; i < DNOD_REC; i++) {
		dir->entry[i].matnum = buf[j++];
		dir->entry[i].begrec = buf[j++];
		dir->entry[i].endrec = buf[j++];
		dir->entry[i].flag = buf[j++];
	}
	return (OK);
}
