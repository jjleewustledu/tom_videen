/*  $Id: wrimg7.c,v 1.12 2003/07/01 19:14:19 tom Exp $
    $Log: wrimg7.c,v $
 * Revision 1.12  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 * Revision 1.11  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.10  2002/05/30 13:58:03  tom
    use REVNSCALE for non-calibrated images

 * Revision 1.9  2001/10/04  14:23:46  tom
 * prevent zero divide
 *
 * Revision 1.8  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.7  2000/07/12  19:51:08  tom
 * *** empty log message ***
 *
 * Revision 1.6  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.5  1997/05/12  20:11:35  tom
 * fix error in computing reclen
 *
 * Revision 1.4  1996/08/26  17:47:44  tom
 * fix scaling
 * >> .
 *
 * Revision 1.3  1996/08/22  17:29:13  tom
 * correct scaling min/max
 *
 * Revision 1.2  1996/08/05  14:13:54  tom
 * move scaling from writetensor & wrtensor to this function
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		wrimg7
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Write image data for Version 7 file.
 *	Notes:
 *			Writes one frame with a subheader.
 *			If (sh7->scale_factor = 0) or (mh7->calibration_factor = 0) or (indx->pln = 0)
 *			a sh7->scale_factor will be recomputed.
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/wrimg7.c,v 1.12 2003/07/01 19:14:19 tom Exp $";

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

#define MAXSCL 16383.0

int             wrimg7 (file, indx, img)
	ImageFile      *file;
	Index          *indx;
	float          *img;
{
	Main_header7   *mh7;
	Image_subheader7 *sh7;
	DataSetType     dtype;
	char           *fname;
	short          *simg;
	float           scaler, scl, max, min;
	int             xdim, ydim, zdim;
	int             begrec, endrec;
	int             matnum, i;
	int             status;
	int             nrec;
	long            len;
	int             pln1 = 1;

	fname = file->name;
	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg7]: Version 7 header is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->m7->mh == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg7]: Version 7 mainheader is not defined for %s\n", fname);
		return (ERROR);
	}
	mh7 = file->m7->mh;
	if (file->m7->sh == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg7]: Version 7 subheader is not defined for %s\n", fname);
		return (ERROR);
	}
	sh7 = file->m7->sh;
	dtype = (DataSetType) mh7->file_type;
	if (dtype != PetVolume) {
		(void) fprintf (stderr, "ERROR [wrimg7]: Unsupported file type %d in %s\n", dtype, fname);
		return (ERROR);
	}
	xdim = sh7->x_dimension;
	ydim = sh7->y_dimension;
	zdim = sh7->z_dimension;
	len = xdim * ydim * zdim;

	if ((matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [wrimg7]: Could not get matrix number for %s\n", fname);
		return (ERROR);
	}

/*	Scale the data */

	if (mh7->calibration_factor == 1 || sh7->filter_scatter_fraction == 0
	    || sh7->filter_scatter_slope == 0)
		scl = REV7NSCALE;		 /* non-quantitative image and 7.0 release file */
	else
		scl = REV7SCALER;		 /* quantitative image */

	if (sh7->frame_duration == 0)
		scaler = 1. / ATNSCALER;
	else if (strncmp (mh7->data_units, "1/cm", 4) == 0)
		scaler = 1. / ATNSCALER;
	else
		scaler = 1. / (scl * (float) file->sh->duration);

	min = 0;
	max = 0;
	for (i = 0; i < len; i++) {
		img[i] *= scaler;
		if (img[i] > max)
			max = img[i];
		if (img[i] < min)
			min = img[i];
	}
	if (max > -min)
		scaler = MAXSCL / max;
	else if (min != 0)
		scaler = -MAXSCL / min;
	else
		scaler = 0;
	max = max * scaler;
	min = min * scaler;
	sh7->image_max = (short) max;
	sh7->image_min = (short) min;
	if (scaler != 0)
		sh7->scale_factor = 1.0 / (scaler * mh7->calibration_factor);
	else
		sh7->scale_factor = 1.0;

	simg = (short *) calloc (len, sizeof (short));
	if (simg == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg7]: Could not calloc memory for img\n");
		return (ERROR);
	}
	for (i = 0; i < len; i++)
		simg[i] = scaler * img[i];

	if (len % LENREC_S == 0)
		nrec = len / LENREC_S;
	else
		nrec = len / LENREC_S + 1;

	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, nrec)) < 0) {
			(void) fprintf (stderr, "ERROR [wrimg7]: Could not create entry for matrix #%x in %s\n",
					matnum, fname);
			free (simg);
			return (ERROR);
		}
	}
	if (put_sh7 (file, indx) == ERROR) {
		(void) fprintf (stderr, "ERROR [wrimg7]: Could not write subheader to %s frame %d\n",
				fname, indx->frm);
		free (simg);
		return (ERROR);
	}
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		swap_byte ((char *) simg, len * (long) sizeof (short));

	status = wrtrec (file->fp, (char *) simg, begrec + NUMREC_ISHDR, nrec);
	free (simg);
	return (status);
}
