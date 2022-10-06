/*	$Log: petinfo.c,v $
 * Revision 1.4  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.3  1997/03/06  20:42:37  tom
 * look for frames1-3
 *
 * Revision 1.2  1996/06/04  22:49:19  tom
 * add length of string parameter
 *
 * Revision 1.1  1996/05/23  15:26:31  tom
 * Initial revision
 * */
/*	===============================================================================
 *	Module:			petinfo.c
 *	Date:			May-96
 *	Author:			Tom Videen
 *	Description:	Get scan info from headers of PET image files
 *	===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Id: petinfo.c,v 1.4 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>

int             petinfo (fname, scanner, acq, recon, fltr, len)
	char           *fname;
	char           *scanner;
	char           *acq;
	char           *recon;
	char           *fltr;
	int             len;

{
	ImageFile      *file;
	Index           indx;

	indx.frm = 1;
	indx.pln = 1;
	indx.gate = 1;
	indx.data = 0;
	indx.bed = 0;

	file = make_imgfile (fname);
	if (open_imgfile (file) == ERROR) {
		fprintf (stderr, "ERROR [petinfo]: Could not open %s\n", fname);
		return (ERROR);
	}
	if (get_mh (file) == ERROR) {
		fprintf (stderr, "ERROR [petinfo]: Could not get MainHeader from %s\n", fname);
		return (ERROR);
	}
	if (existimgfrm (file, &indx) == ERROR) {
		indx.frm = 2;
		if (existimgfrm (file, &indx) == ERROR) {
			indx.frm = 3;
			if (existimgfrm (file, &indx) == ERROR) {
				fprintf (stderr, "ERROR [petinfo]: cannot find %s frame 1, 2 or 3\n", fname);
				return (ERROR);
			}
		}
	}
	if (get_sh (file, &indx) == ERROR) {
		fprintf (stderr, "ERROR [petinfo]: Could not read file subheader in %s\n", fname);
		return (ERROR);
	}
	(void) strncpy (scanner, file->mh->scanner, len - 1);
	(void) strncpy (acq, file->mh->acquisition, len - 1);
	(void) strncpy (recon, file->mh->reconstruction, len - 1);
	(void) strncpy (fltr, file->mh->filtername, len - 1);
	free (file->name);
	free_imgfile (file);
	return (OK);
}
