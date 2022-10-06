/*  $Id: rdscan6.c,v 1.3 2002/12/17 21:55:15 tom Exp $
    $Log: rdscan6.c,v $
 * Revision 1.3  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.2  2002/03/13 21:56:41  tom
    lint cleaning

 * Revision 1.1  2000/02/18  21:49:13  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		rdscan6
 *  Date:           Feb-2000
 *  Author:         Tom Videen 
 *	Description:	Get scan data for 1 plane of a Version 6 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/rdscan6.c,v 1.3 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

short          *rdscan6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	Scan_subheader *ss6;
	DataSetType     dtype;
	MatrixData     *mdata;
	short          *scn;
	float           scaler;
	int             xdim, ydim;
	int             matnum;
	int             i;

	dtype = (DataSetType) file->m6->mh->file_type;
	if (dtype != Sinogram) {
		(void) fprintf (stderr, "ERROR [rdscan6]: Unsupported file type %d for %s\n", dtype, file->name);
		return (NULL);
	}
	if (file->m6->mfp == NULL) {
		(void) fprintf (stderr, "ERROR [rdscan6]: Could not get file pointer for %s\n", file->name);
		return (NULL);
	}
	if ((matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [rdscan6]: Could not get matrix number for %s\n", file->name);
		return (NULL);
	}
	mdata = matrix_read (file->m6->mfp, matnum);
	if (mdata == NULL) {
		(void) fprintf (stderr, "ERROR [rdscan6]: Could not read mdata for %s\n", file->name);
		return (NULL);
	}
	file->m6->scn = (Scan_subheader *) mdata->shptr;
	ss6 = file->m6->scn;
	scaler = ss6->scale_factor;
	xdim = ss6->dimension_1;
	ydim = ss6->dimension_2;
	file->sh->start_time = ss6->frame_start_time;
	file->sh->duration = ss6->frame_duration;

	scn = (short *) mdata->data_ptr;
	for (i = 0; i < xdim * ydim; i++)
		scn[i] *= scaler;
	return (scn);
}
