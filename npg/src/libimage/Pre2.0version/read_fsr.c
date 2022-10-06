/*  $Id: read_fsr.c,v 1.1 2003/07/01 19:14:19 tom Exp $
    $Log: read_fsr.c,v $
 * Revision 1.1  2003/07/01  19:14:19  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		read_fsr
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Read file structure
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/read_fsr.c,v 1.1 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <endian.h>

int             read_fsr (fd, dir, recnum)
	FILE           *fd;
	struct directory *dir;
	int             recnum;
{
	long            dskptr;
	long            buf[LENREC_L];
	int             stat, i, j;
	long            nbytes = LENREC;

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
		swap_byte ((char *) buf, nbytes);
		swap_word ((short *) buf, nbytes / 2);
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
