/*  $Id: put_sh.c,v 1.3 1999/12/13 17:45:50 tom Exp $
    $Log: put_sh.c,v $
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1996/07/19  14:17:18  tom
 * fix bug in dtype checking
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		put_sh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write image subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/put_sh.c,v 1.3 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>
#include <libimage.h>

int             put_sh (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     dtype;
	int             fformat;

	fformat = file->mh->file_format;
	dtype = (DataSetType) file->mh->filetype;

	if (update_sh (file, indx) == ERROR) {
		fprintf (stderr, "ERROR [put_sh]: Error updating subheader of %s\n", file->name);
		return (ERROR);
	}
	if (fformat == MATRIX_REV7) {
		if (dtype != PetVolume) {
			fprintf (stderr, "ERROR [put_sh]: Unsupported data type %d in %s\n", dtype, file->name);
			return (ERROR);
		}
		if ((use_sh7 (file)) == ERROR) {
			fprintf (stderr, "ERROR [put_sh]: Error converting %s\n", file->name);
			return (ERROR);
		}
		if ((put_sh7 (file, indx)) == ERROR) {
			fprintf (stderr, "ERROR [put_sh]: Error writing subheader for plane %d of %s\n",
				 indx->pln, file->name);
			return (ERROR);
		}
	} else if (fformat == MATRIX_REV6) {
		if (dtype != PetImage) {
			fprintf (stderr, "ERROR [put_sh]: Unsupported data type %d in %s\n", dtype, file->name);
			return (ERROR);
		}
		if ((use_sh6 (file)) == ERROR) {
			fprintf (stderr, "ERROR [put_sh]: Error converting %s\n", file->name);
			return (ERROR);
		}
		if ((put_sh6 (file, indx)) == ERROR) {
			fprintf (stderr, "ERROR [put_sh]: Error writing subheader for plane %d of %s\n",
				 indx->pln, file->name);
			return (ERROR);
		}
#ifdef UNDEF
	} else if (fformat == INTERFILE) {
		if ((use_shif (file)) == ERROR) {
			fprintf (stderr, "ERROR [put_sh]: Error converting %s\n", file->name);
			return (ERROR);
		}
		if ((put_shif (file, indx)) == ERROR) {
			fprintf (stderr, "ERROR [put_sh]: Error writing subheader for plane %d of %s\n",
				 indx->pln, file->name);
			return (ERROR);
		}
#endif
	} else {

		fprintf (stderr, "ERROR [put_sh]: Unsupported file format %d in %s\n",
			 fformat, file->name);
		return (ERROR);
	}
	return (OK);
}
