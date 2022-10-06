/*  $Id: rdimgif.c,v 1.5 2001/11/21 17:01:28 tom Exp $
 *  $Log: rdimgif.c,v $
 * Revision 1.5  2001/11/21  17:01:28  tom
 * *** empty log message ***
 *
 * Revision 1.4  2000/10/12  15:41:39  tom
 * use nbytes from ifh
 *
 * Revision 1.3  2000/10/12  15:24:56  tom
 * get correct frame offset
 *
 * Revision 1.2  2000/07/18  20:09:18  tom
 * *** empty log message ***
 *
 * Revision 1.1  2000/07/18  15:40:38  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		rdimgif
 *  Date:           Jul 2000
 *  Author:         Tom Videen
 *	Description:	Get image data from Interfile
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/rdimgif.c,v 1.5 2001/11/21 17:01:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <libimage.h>

float          *rdimgif (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	MainHeader_if  *ifh;
	char           *bimg;
	short          *simg;
	float          *img;
	long            frame, plane, nbytes, i;
	long            offset, frm_offset, pln_offset;


	if (get_ifh (file) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdimgif]: Could not read header of %s\n", file->name);
		return (NULL);
	}
	ifh = (MainHeader_if *) file->ih->mh;
	nbytes = ifh->nbytes;
	plane = (long) ifh->dim1 * (long) ifh->dim2;
	frame = (long) ifh->dim3 * plane;
    frm_offset = (long) ((indx->frm) - 1) * frame * nbytes;
    pln_offset = (long) ((indx->pln) - 1) * plane * nbytes;
    offset = frm_offset + pln_offset;

    if (file->fp != NULL)
		if (ftell(file->fp) >= 0)
        	(void) fclose (file->fp);
    file->fp = fopen (ifh->data_file, file->mode);
    if (file->fp == NULL) {
		(void) fprintf (stderr, "ERROR [rdimgif]: Could not open %s\n", ifh->data_file);
		return (NULL);
	}
	if ((fseek (file->fp, offset, SEEK_SET)) != 0) {
		(void) fprintf (stderr, "ERROR [rdimgif]: Reading %s plane %d\n", file->name, indx->pln);
		return (NULL);
	}
	if ((img = (float *) calloc (plane, sizeof (float))) == NULL) {
		(void) fprintf (stderr, "ERROR [rdimgif]: Could not calloc memory for img\n");
		return (NULL);
	}
	if (ifh->number_format == INT_DATA || ifh->number_format == SHORT_DATA) {
		simg = (short *) calloc (plane, sizeof (short));
		if ((fread (simg, sizeof (short), plane, file->fp)) != plane) {
			(void) fprintf (stderr, "ERROR [rdimgif]: Reading %s plane %d\n", file->name, indx->pln);
			return (NULL);
		}
		for (i = 0; i < plane; i++)
			img[i] = (float) simg[i];
		free (simg);
	} else if (ifh->number_format == FLOAT_DATA) {
		if ((fread (img, sizeof (float), plane, file->fp)) != plane) {
			(void) fprintf (stderr, "ERROR [rdimgif]: Reading %s plane %d\n", file->name, indx->pln);
			return (NULL);
		}
	} else if (ifh->number_format == BYTE_DATA) {
		bimg = (char *) calloc (plane, nbytes);
		if ((fread (bimg, sizeof (char), plane, file->fp)) != plane) {
			(void) fprintf (stderr, "ERROR [rdimgif]: Reading %s plane %d\n", file->name, indx->pln);
			return (NULL);
		}
		for (i = 0; i < plane; i++)
			img[i] = (float) bimg[i];
		free (bimg);
	}
	return (img);
}
