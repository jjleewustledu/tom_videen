/*  $Id: wrattn6.c,v 1.2 2002/12/17 21:55:15 tom Exp $
    $Log: wrattn6.c,v $
    Revision 1.2  2002/12/17 21:55:15  tom
    fix pointer/cast errors found with gcc

    Revision 1.1  2002/02/26 17:02:14  tom
    Initial revision
 */
/* =============================================================================
 *	Function:		wrattn6
 *  Date:           Feb 2002
 *  Author:         Tom Videen
 *	Description:	Write data for 1 plane of a Version 6 attenuation file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/wrattn6.c,v 1.2 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             wrattn6 (file, indx, matrix)
	ImageFile      *file;
	Index          *indx;
	float          *matrix;
{
	MatrixData     *mdata;
	DataSetType		dtype;
	char           *fname;
	int             matnum;
	int             xdim, ydim;

	fname = file->name;
	xdim = file->sh->dim1;
	ydim = file->sh->dim2;
	if (file->m6 == NULL) {
		(void) fprintf (stderr, "ERROR [wrattn6]: Version 6 header is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->m6->mh == NULL) {
		(void) fprintf (stderr, "ERROR [wrattn6]: Version 6 mainheader is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->m6->atn == NULL) {
		(void) fprintf (stderr, "ERROR [wrattn6]: Version 6 subheader is not defined for %s\n", fname);
		return (ERROR);
	}
	if ((matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [wrattn6]: Could not get matrix number for %s\n", fname);
		return (ERROR);
	}
	if ((mdata = (MatrixData *) calloc (1, sizeof (MatrixData))) == NULL) {
		(void) fprintf (stderr, "ERROR [wrattn6]: Could not calloc mdata\n");
		return (ERROR);
	}
    dtype = (DataSetType) file->mh->filetype;
	mdata->mat_type = dtype;
	mdata->matnum = matnum;
	mdata->shptr = (char *) file->m6->atn;
	mdata->data_ptr = (char *) matrix;
	mdata->nviews = ydim;
	mdata->nelements = xdim * ydim;
	mdata->nblks = xdim * ydim * sizeof (float) / 512;
	mdata->data_type = SUN_R4;

	if (matrix_write (file->m6->mfp, matnum, mdata) == ERROR) {
		(void) fprintf (stderr, "ERROR [wrattn6]: writing mdata for %s\n", fname);
		return (ERROR);
	}
	return (OK);
}
