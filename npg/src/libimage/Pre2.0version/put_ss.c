/*  $Id: put_ss.c,v 1.2 2001/11/21 17:01:28 tom Exp $
    $Log: put_ss.c,v $
 * Revision 1.2  2001/11/21  17:01:28  tom
 * elim param
 *
 * Revision 1.1  2000/03/13  18:23:14  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		put_ss
 *  Date:           Mar-2000
 *  Author:         Tom Videen
 *	Description:	Write scan subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/put_ss.c,v 1.2 2001/11/21 17:01:28 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>
#include <libimage.h>

int             put_ss (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     dtype;
	int             fformat;

	fformat = file->mh->file_format;
	dtype = (DataSetType) file->mh->filetype;

	if (dtype != Sinogram && dtype != AttenCor && dtype != Short3dSinogram) {
		(void) fprintf (stderr, "ERROR [put_ss]: Unsupported data type %d in %s\n", dtype, file->name);
		return (ERROR);
	}
	if (fformat == MATRIX_REV7) {
		if (use_ss7 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_ss]: Error converting %s\n", file->name);
			return (ERROR);
		}
		if (put_ss7 (file, indx) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_ss]: Error writing subheader for plane %d of %s\n",
				 indx->pln, file->name);
			return (ERROR);
		}
	} else if (fformat == MATRIX_REV6) {
		if (use_ss6 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_ss]: Error converting %s\n", file->name);
			return (ERROR);
		}
		if (put_ss6 (file, indx) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_ss]: Error writing subheader for plane %d of %s\n",
				 indx->pln, file->name);
			return (ERROR);
		}
	} else {
		(void) fprintf (stderr, "ERROR [put_ss]: Unsupported file format %d in %s\n",
			 fformat, file->name);
		return (ERROR);
	}
	return (OK);
}
