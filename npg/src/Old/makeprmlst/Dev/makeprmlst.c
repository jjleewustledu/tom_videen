/* $Log$ */
/*
 * ===============================================================================
 *  Module:         makeprmlst
 *  Date:           Aug-96
 *  Author:         Tom Videen
 *  Description:
 *		Create an abbreviated parameter file and image list which will
 *		permit t88mean to compute the scalers from the named image file.
 * ===============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header$";

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <image.h>
#include <libimage.h>
#include <myvalues.h>

main (argc, argv)
	int             argc;
	char           *argv[];
{
	ImageFile      *file;
	MainHeader     *mh;
	FILE           *fp;
	char           *str1, *str2;
	char           *subject, *scanid;
	char           *imgfile;
	char           *lstfile;
	char           *prmfile;
	char           *ptr;
	int             i;

	imgfile = argv[1];
	prmfile = argv[2];
	lstfile = argv[3];

	file = make_imgfile (imgfile);
	if (open_imgfile (file) == ERROR) {
		fprintf (stderr, "ERROR [makeprmlst]: Could not open %s\n", imgfile);
		return (ERROR);
	}
	if (get_mh (file) == ERROR) {
		fprintf (stderr, "ERROR [makeprmlst]: Could not get MainHeader from %s\n", imgfile);
		return (ERROR);
	}
	mh = file->mh;

	fp = fopen (prmfile, "w");
	if (fp == (FILE *) NULL) {
		fprintf (stderr, "Error in opening file %s\n", prmfile);
		return (ERROR);
	}
	fprintf (fp, "#Created by makeprmlst\n");
	fprintf (fp, "category:       individual\n");
	fprintf (fp, "datatype:       pet_image\n");
	fprintf (fp, "smg_compute:    no\n");
	fprintf (fp, "acquisition:    low_resolution\n");
	fprintf (fp, "reconstruction: filtered_backprojection\n");
	fprintf (fp, "recfilter:      ramp\n");

	if (mh->scanner_type == 6)
		fprintf (fp, "scanner:        pett6\n");
	else if (mh->scanner_type == 953)
		fprintf (fp, "scanner:        953\n");
	else if (mh->scanner_type == 961)
		fprintf (fp, "scanner:        961\n");

	str1 = (char *) malloc (MAXPATH);
	str2 = (char *) malloc (MAXPATH);
	subject = (char *) malloc (10);
	scanid = (char *) malloc (10);
	memset (str1, 0, MAXPATH);
	memset (str2, 0, MAXPATH);
	memset (subject, 0, 10);
	memset (scanid, 0, 10);

	strcpy (str1, imgfile);
	ptr = strpbrk (str1, "1234567890");
	strcpy (ptr, "");
	fprintf (fp, "species:        %s\n", str1);

	strcpy (str1, imgfile);
	ptr = strpbrk (str1, "1234567890");
	strcpy (str2, ptr);
	i = strspn (str2, "1234567890");
	strncpy (subject, str2, i);

	strcpy (str1, imgfile);
	ptr = strpbrk (str1, "1234567890");
	strcpy (str2, ptr);
	i = strspn (str2, "1234567890");
	strcpy (str1, str2 + i);
	ptr = strpbrk (str1, "1234567890");
	strcpy (ptr, "");
	fprintf (fp, "tracer:         %s\n", str1);

	strcpy (str1, imgfile);
	ptr = strpbrk (str1, "1234567890");
	strcpy (str2, ptr);
	i = strspn (str2, "1234567890");
	strcpy (str1, str2 + i);
	ptr = strpbrk (str1, "1234567890");
	i = strspn (ptr, "1234567890");
	strncpy (scanid, ptr, i);

	strcpy (str1, imgfile);
	ptr = strrchr (str1, '.');
	fprintf (fp, "extension:      %s\n", ptr + 1);

	strcpy (str1, imgfile);
	ptr = strpbrk (str1, "_");
	if (ptr == NULL)
		fprintf (fp, "imgfilter:      none\n");
	else {
		strcpy (str2, ptr + 1);
		ptr = strrchr (str2, '.');
		strcpy (ptr, "");
		if (strncmp (str2, "g", 1) == 0)
			fprintf (fp, "imgfilter:      gauss 0.%s\n", str2 + 1);
		else if (strncmp (str2 + 1, "b", 1) == 0) {
			memset (str1, 0, MAXPATH);
			strncpy (str1, str2, 1);
			fprintf (fp, "imgfilter:      butterworth 0.%s %s\n", str1, str2 + 2);
		}
	}

	fclose (fp);

	fp = fopen (lstfile, "w");
	if (fp == (FILE *) NULL) {
		fprintf (stderr, "Error in opening %s&n", lstfile);
		return (ERROR);
	}
	fprintf (fp, "%s %s\n", subject, scanid);
	fclose (fp);

	exit (0);
}
