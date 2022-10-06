/*	$Id: getsxr.c,v 2.1 1999/12/13 17:04:17 tom Exp $
	$Log: getsxr.c,v $
 * Revision 2.1  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			getsxr.c
 *	Date:			Apr-93
 *	Author:			Tom Yang
 *	Description:
 *		This function gets skull xray data from an sxr file and write
 *		it to a structure. The user of this function is responsible for
 *		freeing the memory of sxrdata->rlpetdim, sxrdata->rlcenter,
 *		sxrdata->vertical_petdim, sxrdata->rlslicedim after the
 *		structure sxrdata is no longer used.
 * ===============================================================================
 */
#ifndef lint
static char		rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/getsxr.c,v 2.1 1999/12/13 17:04:17 tom Exp $";
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imgutil.h>

#define	INCORRECT_DZ	"incorrect delta-z"
#define	DEFAULT_XSCALE	0.0

SXR_DATA      *getsxr (fname)
	char           *fname;
{
	SXR_DATA       *sxrdata;
	BOOLEAN         xray_sxr;
	FILE           *sxrfp;
	char            line[MAXLINE];
	char            sbuffer[MAXLINE];
	char            sbuffer1[MAXLINE];
	char            sbuffer2[MAXLINE];
	float           fbuffer[11];
	int             i;
	int             num_scanned;
	int             ibuffer[3];

	sxrfp = fopen (fname, "r");
	if (sxrfp == NULL) {
		fprintf (stderr, "ERROR [getsxr]: cannot open %s\n", fname);
		return (NULL);
	}

/*
 * Get SXR file type.
 */
	sxrdata = (SXR_DATA *) malloc (sizeof (SXR_DATA));
	(void) fgets (line, MAXLINE, sxrfp);
	num_scanned = sscanf (line, "%d", ibuffer);
	if (num_scanned == 1)
		sxrdata->type = ibuffer[0];
	else {
		fprintf (stderr, "ERROR [getsxr]: sxr file %s does not have a recognizable type\n", fname);
		return (NULL);
	}
	if (strstr (line, INCORRECT_DZ) != NULL)
		sxrdata->incorrect_dz = TRUE;
	else
		sxrdata->incorrect_dz = FALSE;

	if (strstr (line, "xray") != NULL)
		xray_sxr = TRUE;
	else
		xray_sxr = FALSE;

/*
 * Get threshold of PET image.
 */
	(void) fgets (line, MAXLINE, sxrfp);
	if (xray_sxr) {
		if (sxrdata->incorrect_dz)
			num_scanned = sscanf (line, "%s %s %d%%%s",
					      sbuffer, sbuffer1, ibuffer, sbuffer2);
		else
			num_scanned = sscanf (line, "%s %d%%%s",
					      sbuffer, ibuffer, sbuffer2);
		if ((sxrdata->incorrect_dz && num_scanned == 4) ||
		    !sxrdata->incorrect_dz && num_scanned == 3) {
			sxrdata->threshold = ibuffer[0];
			if (strcmp (sbuffer2, "slice") == 0)
				sxrdata->peaktype = PEAK_PER_SLICE;
			else
				sxrdata->peaktype = PEAK_PER_SCAN;
		} else {
			fprintf (stderr, "ERROR [getsxr]: cannot get threshold from %s\n", fname);
			return (NULL);
		}
	}

/*
 * Get the 3rd line.
 */
	(void) fgets (line, MAXLINE, sxrfp);

/*
* Get the variables from the 4th line.
 */
	(void) fgets (line, MAXLINE, sxrfp);
	num_scanned = sscanf (line, "%f %f %d %f %d %f %f %f %f %f %d %f", fbuffer,
			      fbuffer + 1, ibuffer, fbuffer + 2, ibuffer + 1, fbuffer + 3,
			      fbuffer + 4, fbuffer + 5, fbuffer + 6, fbuffer + 7, ibuffer + 2,
			      fbuffer + 8);
	sxrdata->pixel_size = fbuffer[0];
	sxrdata->magnification_factor = fbuffer[1];
	sxrdata->number_slices = ibuffer[0];
	sxrdata->slice_separation = fbuffer[2];
	sxrdata->reference_slice = ibuffer[1];
	sxrdata->zatlas_dimension = fbuffer[3];
	sxrdata->apatlas_dimension = fbuffer[4];
	sxrdata->peakslice = PEAK_SLICE_UNDEFINED;
	sxrdata->xscale = DEFAULT_XSCALE;
	if (num_scanned >= 10) {
		sxrdata->film_position = fbuffer[5];
		sxrdata->scan_position = fbuffer[6];
		sxrdata->reference_offset = fbuffer[7];
		if (num_scanned >= 11)
			sxrdata->peakslice = ibuffer[2];
		if (num_scanned >= 12)
			sxrdata->xscale = fbuffer[8];
	} else {
		sxrdata->film_position = 0.0;
		sxrdata->scan_position = 0.0;
		sxrdata->reference_offset = 0.0;
	}

/*
 * Get the variables from the 5th line.
 */
	(void) fgets (line, MAXLINE, sxrfp);
	num_scanned = sscanf (line, "%f %f %f %f %f %f %f %f %f %f %f",
			      fbuffer, fbuffer + 1, fbuffer + 2, fbuffer + 3,
			      fbuffer + 4, fbuffer + 5, fbuffer + 6, fbuffer + 7,
			      fbuffer + 8, fbuffer + 9, fbuffer + 10);
	if (num_scanned == 11) {
		sxrdata->apxray = fbuffer[0];
		sxrdata->degx = fbuffer[1];
		sxrdata->zxray = fbuffer[2];
		sxrdata->dz = fbuffer[3];
		sxrdata->dap = fbuffer[4];
		sxrdata->apcenter = fbuffer[5];
		sxrdata->ear_separation = fbuffer[6];
		sxrdata->eardap = fbuffer[7];
		sxrdata->eardz = fbuffer[8];
		sxrdata->degy = fbuffer[9];
		sxrdata->degz = fbuffer[10];
	} else {
		fprintf (stderr, "ERROR [getsxr]: 5th line of sxr file %s cannot be read\n", fname);
		return (NULL);
	}

	sxrdata->rlpetdim = (float *) malloc (sxrdata->number_slices * sizeof (float));
	sxrdata->rlcenter = (float *) malloc (sxrdata->number_slices * sizeof (float));
	sxrdata->vertical_petdim = (float *) malloc (sxrdata->number_slices * sizeof (float));
	sxrdata->rlslicedim = (float *) malloc (sxrdata->number_slices * sizeof (float));

	for (i = 0; i < sxrdata->number_slices; i++) {
		(void) fgets (line, MAXLINE, sxrfp);
		num_scanned = sscanf (line, "%f %f %f %f\n", fbuffer,
				      fbuffer + 1, fbuffer + 2, fbuffer + 3);
		if (num_scanned == 4) {
			sxrdata->vertical_petdim[i] = fbuffer[2];
			sxrdata->rlslicedim[i] = fbuffer[3];
		} else if (num_scanned <= 2) {
			if (num_scanned < 2)
				num_scanned = sscanf (line, "%f, %f\n", fbuffer, fbuffer + 1);
			if (num_scanned == 2) {
				sxrdata->vertical_petdim[i] = 0;
				sxrdata->rlslicedim[i] = 0;
			} else {
				fprintf (stderr, "ERROR [getsxr]: format error for PET dimensions in %s\n", fname);
				return (NULL);
			}
		}
		sxrdata->rlpetdim[i] = fbuffer[0];
		sxrdata->rlcenter[i] = fbuffer[1];
	}

/*
 * Processing for sxr files generated with old ECAT images with the Radiology convertion.
 */
	if (sxrdata->type <= XRAY_TYPE_RADIOLOGY
	    && sxrdata->number_slices == ECAT_NUM_SLICES) {
		for (i = 0; i < sxrdata->number_slices; i++) {
			sxrdata->rlcenter[i] = (ENCODED_SLICE_WIDTH + 1)
				- sxrdata->rlcenter[i];
		}
	}
	fclose (sxrfp);
	return (sxrdata);
}
