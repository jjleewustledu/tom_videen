/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       putshdrif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Write Interfile subheader.
 *
 *	Copyright 1991-1996, Washington University.
 *	All rights reserved. Unauthorized reproduction prohibited.
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header$";

#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <interfile.h>
#include <imagio.h>
#include <libimg.h>
#include <myvalues.h>

int             putshdrif (sh, filnam_in, plane, frame)
	SubHeader_if   *sh;
	char           *filnam_in;
	int             plane, frame;
{
	IFheader       *ifh;
	FILE           *fpr, *fpw;
	char            line[MAXLINE], *hdrfil, *ptr;
	int             i, last_frame = 0, pln = 0;

	hdrfil = header_file (filnam_in);

	if ((ifh = get_ifh (hdrfil)) == NULL) {
		fprintf (stderr, "ERROR [putshdrif]: Main header must be written before subheader\n");
		return (ERROR);
	}
	if (frame > ifh->mh->dim4) {
		fprintf (stderr, "ERROR [putshdrif]: Frame number inconsistent from number of frames in main header: %s\n", hdrfil);
		return (ERROR);
	}
	if (plane > ifh->mh->dim3) {
		fprintf (stderr, "ERROR [putshdrif]: Plane number inconsistent from number of planes in main header: %s\n", hdrfil);
		return (ERROR);
	}
	if (ifh->sh[frame - 1][plane - 1] == NULL)
		ifh->sh[frame - 1][plane - 1] = (SubHeader_if *) malloc (sizeof (SubHeader_if));
	ifh->sh[frame - 1][plane - 1]->dim1 = sh->dim1;
	ifh->sh[frame - 1][plane - 1]->dim2 = sh->dim2;
	ifh->sh[frame - 1][plane - 1]->voxel_size_1 = sh->voxel_size_1;
	ifh->sh[frame - 1][plane - 1]->voxel_size_2 = sh->voxel_size_2;
	ifh->sh[frame - 1][plane - 1]->storage_sf = sh->storage_sf;
	ifh->sh[frame - 1][plane - 1]->deadtime_corr = sh->deadtime_corr;
	ifh->sh[frame - 1][plane - 1]->trues = sh->trues;
	ifh->sh[frame - 1][plane - 1]->randoms = sh->randoms;
	ifh->sh[frame - 1][plane - 1]->multiples = sh->multiples;
	if (sh->comment != NULL) {
		ifh->sh[frame - 1][plane - 1]->comment = (char *) malloc (1 + strlen (sh->comment));
		strcpy (ifh->sh[frame - 1][plane - 1]->comment, sh->comment);
	}
	if (ifh->fh[frame - 1] == NULL)
		ifh->fh[frame - 1] = (FrameHeader_if *) malloc (sizeof (FrameHeader_if));
	ifh->fh[frame - 1]->starting_time = sh->starting_time;
	ifh->fh[frame - 1]->duration = sh->duration;

	if ((fpr = fopen (hdrfil, "r+")) == NULL)
		return (ERROR);
	while (fgets (line, 200, fpr) != NULL) {
		if (strstr (line, "frame number"))
			last_frame = atoi (getstr (line));
		else if (strstr (line, "plane number"))
			pln = atoi (getstr (line));
		if (last_frame == frame && pln == plane) {
			fprintf (fpr, "storage scale factor := %g\n", ifh->sh[frame - 1][plane - 1]->storage_sf);
			fprintf (fpr, "number of trues := %d\n", ifh->sh[frame - 1][plane - 1]->trues);
			fprintf (fpr, "number of randoms := %d\n", ifh->sh[frame - 1][plane - 1]->randoms);
			fprintf (fpr, "number of multiples := %d\n", ifh->sh[frame - 1][plane - 1]->multiples);
			fprintf (fpr, "deadtime correction factor := %f\n", ifh->sh[frame - 1][plane - 1]->deadtime_corr);
			fprintf (fpr, "user defined comment := %s\n", ifh->sh[frame - 1][plane - 1]->comment);
			fflush (fpr);
			fclose (fpr);
			free (hdrfil);
			return (OK);
		}
	}

	if (frame > last_frame) {
		fprintf (fpr, "frame number := %d\n", frame);
		fprintf (fpr, "frame duration := %d\n", ifh->fh[frame - 1]->duration);
		fprintf (fpr, "frame starting time := %d\n", ifh->fh[frame - 1]->starting_time);
	} else if (frame < last_frame) {
		fprintf (stderr, "ERROR [putshdrif]: Frames written out of order: putshdrif aborted\n");
		return (ERROR);
	}
	fprintf (fpr, "plane number := %d\n", plane);
	fprintf (fpr, "storage scale factor := %g\n", ifh->sh[frame - 1][plane - 1]->storage_sf);
	fprintf (fpr, "number of trues := %d\n", ifh->sh[frame - 1][plane - 1]->trues);
	fprintf (fpr, "number of randoms := %d\n", ifh->sh[frame - 1][plane - 1]->randoms);
	fprintf (fpr, "number of multiples := %d\n", ifh->sh[frame - 1][plane - 1]->multiples);
	fprintf (fpr, "deadtime correction factor := %f\n", ifh->sh[frame - 1][plane - 1]->deadtime_corr);
	fprintf (fpr, "user defined comment := %s\n", ifh->sh[frame - 1][plane - 1]->comment);
	fflush (fpr);
	fclose (fpr);
	free (hdrfil);
	return (OK);
}
