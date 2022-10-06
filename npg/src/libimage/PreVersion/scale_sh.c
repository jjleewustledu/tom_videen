/*  $Id: scale_sh.c,v 1.6 2000/03/13 18:23:14 tom Exp $
    $Log: scale_sh.c,v $
 * Revision 1.6  2000/03/13  18:23:14  tom
 * *** empty log message ***
 *
 * Revision 1.5  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.4  1996/08/02  13:59:03  tom
 * remove unused img
 *
 * Revision 1.3  1996/04/30  19:54:53  tom
 * *** empty log message ***
 *
 * Revision 1.2  1996/04/30  19:38:04  tom
 * fix bug for Matrix6
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		scale_sh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Scale an image by changing subheader scale factors.
 *	Note:	Avoids assumption of SUN data type made by putsbhdr.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/scale_sh.c,v 1.6 2000/03/13 18:23:14 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>
#include <libimage.h>

int             scale_sh (file, indx, factor)
	ImageFile      *file;
	Index          *indx;
	float           factor;
{
	int             fformat;
	DataSetType     datatype;
	int             stat;
	int             pln, nplanes;

	fformat = file->mh->file_format;
	datatype = (DataSetType) file->mh->filetype;

	if (fformat == MATRIX_REV7) {
		if (get_sh7 (file, indx) == ERROR)
			return (ERROR);
		if (use_sh7 (file) == ERROR)
			return (ERROR);
		if (datatype != PetVolume) {
			(void) fprintf (stderr, "ERROR [scale_sh]: Unsupported data type %d\n", datatype);
			return (ERROR);
		}
		stat = scale_sh7 (file, indx, factor);
		return (stat);

	} else if (fformat == MATRIX_REV6) {
		if (get_sh6 (file, indx) == ERROR)
			return (ERROR);
		if (use_sh6 (file) == ERROR)
			return (ERROR);
		if (datatype != PetImage) {
			(void) fprintf (stderr, "ERROR [scale_sh]: Unsupported data type %d\n", datatype);
			return (ERROR);
		}
		nplanes = file->mh->nplane;
		for (pln = 1; pln <= nplanes; pln++) {
			indx->pln = pln;
			if (scale_sh6 (file, indx, factor) == ERROR)
				return (ERROR);
		}
		return (OK);

	} else if (fformat == PETT6) {
		(void) fprintf (stderr, "ERROR [scale_sh]: PETT6 format not supported\n");
		return (ERROR);

	} else if (fformat == INTERFILE) {
		(void) fprintf (stderr, "ERROR [scale_sh]: INTERFILE format not supported\n");
		return (ERROR);

	} else {
		(void) fprintf (stderr, "ERROR [scale_sh]: Unsupported file format\n");
		return (ERROR);
	}
}
