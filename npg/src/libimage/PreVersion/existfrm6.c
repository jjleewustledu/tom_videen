/*  $Id: existfrm6.c,v 1.4 2000/02/18 21:39:20 tom Exp $
 *  $Log: existfrm6.c,v $
 * Revision 1.4  2000/02/18  21:39:20  tom
 * add Sinogram AttenCor
 *
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1997/05/20  13:18:29  tom
 * pln1
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Function:       existfrm6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *  Description:    Check if an image frame and plane exists.
 *                  Bypasses calls with error messages.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/existfrm6.c,v 1.4 2000/02/18 21:39:20 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             existfrm6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     datatype;
	struct MatDir  *matdir;
	int             matnum;
	int				pln1 = 1;

	datatype = (DataSetType) file->mh->filetype;
	if (datatype != PetImage && datatype != Sinogram && datatype != AttenCor)
		return (ERROR);
	if (file->m6->mfp == NULL) {
		file->m6->mfp = matrix_open (file->name, file->mode);
		if (file->m6->mfp == NULL)
			return (ERROR);
	}
	matnum = mat_numcod (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	if ((matdir = matrix_find (matnum, file->m6->mfp)) == NULL)
		return (ERROR);
	else {
		free (matdir);
		return (OK);
	}
}
