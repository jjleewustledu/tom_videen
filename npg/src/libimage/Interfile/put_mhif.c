/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       put_mhif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Write an Interfile header.
 *  History:
 *      Mar-96  TOV Modified name, return value, and calling parameters.
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
#include <math.h>
#include <image.h>
#include <libimage.h>

int             put_mhif (file)
	ImageFile      *file;
{
	MainHeader_if  *mh;
	IFheader       *ifh;
	MainHeader_if  *mhif;
	FILE           *fpr, *fpw;
	char           *data[2000];
	char            string[MAXLINE];
	char           *hdrfile;
	char           *ptr;
	int             i, frm, pln, ikey, key_written[NUM_KEYS], bed, line, num_lines;

	for (i = 0; i < NUM_KEYS; i++)
		key_written[i] = 0;

	*ifh = *(file->ifh);
	if (ifh == NULL) {
		fprintf (stderr, "ERROR [put_mhif]: No Interfile header data for %s\n", file->name);
		return (ERROR);
	}
	*mhif = *(file->ifh->mh);
	if (mhif == NULL) {
		fprintf (stderr, "ERROR [put_mhif]: No Interfile mainheader data for %s\n", file->name);
		return (ERROR);
	}
	hdrfile = header_file (file->name);
	ifh->fname = (char *) malloc (1 + strlen (hdrfile));
	strcpy (ifh->fname, hdrfile);

	num_lines = 0;
	if ((fpr = fopen (hdrfile, "r")) != NULL) {
		while (fgets (string, 200, fpr) != NULL) {
			if (string != NULL) {
				data[num_lines] = (char *) malloc (1 + strlen (string));
				strcpy (data[num_lines], string);
			}
			++num_lines;
		}
		fclose (fpr);
	}
	if ((fpw = fopen (hdrfile, "w")) == NULL)
		return (ERROR);

	bed = 0;
	for (line = 0; line < num_lines; line++) {
		for (ikey = 0; ikey < NUM_KEYS; ikey++) {
			if (strstr (data[line], key[ikey]))	/* Line is in my header definition */
				break;
		}

		if (ikey == NUM_KEYS)		 /* Line is not in my header definition */
			fprintf (fpw, "%s", data[line]);
		else if (key_written[ikey] == 0) {	/* Update value of data and write it out. */
			key_written[ikey] = 1;
			write_key (fpw, data[line], ifh, bed);
			if (!strcmp (key[ikey], "bed offset"))
				++bed;
		}
		free (data[line]);
	}

	for (ikey = 0; ikey < NUM_MHKEYS; ikey++) {
		if (key_written[ikey] == 0)
			if (!strcmp (key[ikey], "bed offset")) {
				++bed;
				write_key (fpw, key[ikey], ifh, bed);
			} else
				write_key (fpw, key[ikey], ifh, bed);
	}

	fflush (fpw);
	fclose (fpw);
	free (hdrfile);
	return (OK);
}
