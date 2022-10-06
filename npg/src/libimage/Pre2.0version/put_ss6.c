/*  $Id: put_ss6.c,v 1.7 2003/07/01 19:14:19 tom Exp $
 *	$Log: put_ss6.c,v $
 * Revision 1.7  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 * Revision 1.6  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
 *	Revision 1.5  2002/03/13 21:56:41  tom
 *	lint cleaning
 *
 * Revision 1.4  2000/03/13  20:41:04  tom
 * fix MatDir alloc
 *
 * Revision 1.3  2000/03/13  20:26:05  tom
 * declare matrix functions (in libcti)
 *
 * Revision 1.2  2000/03/13  20:18:09  tom
 * create MatDir if necessary
 *
 * Revision 1.1  2000/03/13  18:23:14  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		put_ss6
 *  Date:           Feb-2000
 *  Author:         Tom Videen
 *	Description:	Write Version 6 ECAT sinogram subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/put_ss6.c,v 1.7 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             put_ss6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	int             mat_enter ();
	int             insert_mdir ();
	MatrixFile     *mfp;
	DataSetType     dtype;
	struct MatDir  *matdir;
	int             matnum, blkno, nblks, xdim, ydim;
	int             status = OK;

	mfp = file->m6->mfp;
	if (mfp == NULL) {
		(void) fprintf (stderr, "ERROR [put_ss6]: Could not get file pointer for %s\n", file->name);
		return (ERROR);
	}
	if ((matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [put_ss6]: Could not get matrix number for %s\n", file->name);
		return (ERROR);
	}
	if ((matdir = matrix_find (matnum, mfp)) == NULL) {
		xdim = file->sh->dim1;
		ydim = file->sh->dim2;
		nblks = xdim * ydim * sizeof (short) / 512;
		blkno = mat_enter (mfp->fptr, matnum, nblks);
		matdir = (struct MatDir *) malloc (sizeof (struct MatDir));
		matdir->matnum = matnum;
		matdir->strtblk = blkno;
		matdir->endblk = matdir->strtblk + nblks - 1;
		matdir->matstat = 1;
		insert_mdir (matdir, mfp->dirlist);
	}
	dtype = (DataSetType) file->m6->mh->file_type;
	if (dtype == Sinogram || dtype == Short3dSinogram) {
		if ((mat_write_scan_subheader (mfp->fptr, matdir->strtblk, file->m6->scn)) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_ss6]: Writing subheader for %s\n", file->name);
			status = ERROR;
		}
	} else if (dtype == AttenCor) {
		if ((mat_write_attn_subheader (mfp->fptr, matdir->strtblk, file->m6->atn)) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_ss6]: Writing subheader for %s\n", file->name);
			status = ERROR;
		}
	} else {
		(void) fprintf (stderr, "ERROR [put_ss6]: Unsupported data type %d for %s\n", dtype, file->name);
		status = ERROR;
	}
	free (matdir);
	return (status);
}
