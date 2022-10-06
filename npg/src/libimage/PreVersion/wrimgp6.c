/*  $Id: wrimgp6.c,v 1.4 2002/12/17 21:55:15 tom Exp $
 *  $Log: wrimgp6.c,v $
 * Revision 1.4  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
 *  Revision 1.3  1999/12/13 17:45:50  tom
 *  tov revision
 *
 * Revision 1.2  1996/05/02  13:11:25  tom
 * use ROUND for conversion to short
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		wrimgp6
 *  Date:           Nov-95
 *  Author:         Tom Videen
 *	Description:	Write PETT VI image slice with header.
 *	Note:
 *		100x100 slices must be padded with 112 short integers before write.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/wrimgp6.c,v 1.4 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <imgutil.h>

int             wrimgp6 (file, img)
	ImageFile      *file;
	float          *img;
{
	int             i, xdim, ydim, len, all;
	short          *simg;

	if (file->fp == NULL) {
		(void) fprintf (stderr, "ERROR [wrimgp6]: Could not create file %s\n", file->name);
		return (ERROR);
	}
	if ((use_shp6 (file)) != OK) {
		(void) fprintf (stderr, "ERROR [wrimgp6]: Error converting header for %s\n", file->name);
		return (ERROR);
	}
	xdim = file->sh->dim1;
	ydim = file->sh->dim2;
	if (xdim != 100 || ydim != 100) {
		(void) fprintf (stderr, "ERROR [wrimgp6]: Error non-standard array size for %s\n", file->name);
		return (ERROR);
	} else {
		len = 10000;
		all = 79 * 128;
	}

	if ((simg = (short *) calloc (all, sizeof (short))) == NULL) {
		(void) fprintf (stderr, "ERROR [wrimgp6]: Could not calloc memory for simg\n");
		return (ERROR);
	}
	for (i = 0; i < len; i++)
		simg[i] = ROUND (img[i]);

	if ((put_hp6 (file)) != OK) {
		(void) fprintf (stderr, "ERROR [wrimgp6]: Writing slice header to %s\n", file->name);
		return (ERROR);
	}
	if ((fwrite (simg, sizeof (short), all, file->fp)) != all) {
		(void) fprintf (stderr, "ERROR [wrimgp6]: Writing data to %s\n", file->name);
		return (ERROR);
	}
	free (simg);
	return (OK);
}
