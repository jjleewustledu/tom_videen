/*  $Id
    $Log: wrscan6.c,v $
 * Revision 1.5  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.4  2000/09/28 14:44:29  tom
    calloc

 * Revision 1.3  2000/03/13  20:21:58  tom
 * *** empty log message ***
 *
 * Revision 1.2  2000/03/13  19:32:19  tom
 * *** empty log message ***
 *
 * Revision 1.1  2000/03/13  18:23:14  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		wrscan6
 *  Date:           Mar-2000
 *  Author:         Tom Videen
 *	Description:	Write scan data for 1 plane of a Version 6 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/wrscan6.c,v 1.5 2002/12/17 21:55:15 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             wrscan6 (file, indx, matrix)
	ImageFile      *file;
	Index          *indx;
	short          *matrix;
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
		(void) fprintf (stderr, "ERROR [wrscan6]: Version 6 header is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->m6->mh == NULL) {
		(void) fprintf (stderr, "ERROR [wrscan6]: Version 6 mainheader is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->m6->scn == NULL) {
		(void) fprintf (stderr, "ERROR [wrscan6]: Version 6 subheader is not defined for %s\n", fname);
		return (ERROR);
	}
	if ((matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [wrscan6]: Could not get matrix number for %s\n", fname);
		return (ERROR);
	}
	if ((mdata = (MatrixData *) calloc (1, sizeof (MatrixData))) == NULL) {
		(void) fprintf (stderr, "ERROR [wrscan6]: Could not calloc mdata\n");
		return (ERROR);
	}
    dtype = (DataSetType) file->mh->filetype;
	mdata->mat_type = dtype;
	mdata->matnum = matnum;
	mdata->shptr = (char *) file->m6->scn;
	mdata->data_ptr = (char *) matrix;
	mdata->nviews = ydim;
	mdata->nelements = xdim * ydim;
	mdata->nblks = xdim * ydim * sizeof (short) / 512;
	mdata->data_type = SUN_I2;

	if (matrix_write (file->m6->mfp, matnum, mdata) == ERROR) {
		(void) fprintf (stderr, "ERROR [wrscan6]: writing mdata for %s\n", fname);
		return (ERROR);
	}
	return (OK);
}
