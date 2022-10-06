/*$Log: savesxr.c,v $
 * Revision 1.4  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.3  1996/09/26  21:19:16  tom
 * *** empty log message ***
 *
 * Revision 1.2  1996/09/26  20:26:27  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/09/26  20:05:39  tom
 * Initial revision
 *
 * Revision 1.10  1995/11/10  16:29:26  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.9  1995/02/08  15:36:57  ty7777
 * Added one more decimal place for magnification factor.
 *
 * Revision 1.8  1994/12/22  17:50:42  ty7777
 * Added new argument incorrect_delta_z.
 *
 * Revision 1.7  1994/11/22  16:47:48  ty7777
 * Edited the comments.
 *
 * Revision 1.6  1994/10/04  15:56:29  ty7777
 * Four digits after the decimal point for xscale.
 *
 * Revision 1.5  1994/09/28  15:15:33  ty7777
 * Added output for xsale.
 * "xsale" should be xscale.
 *
 * Revision 1.4  1994/09/20  15:29:51  ty7777
 * Updated to write peakslice to sxr file.
 *
 * Revision 1.3  1994/03/03  20:44:03  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/09/13  22:01:55  ty7777
 * Add argument save_rlatlasdim.
 *
 * Revision 1.1  1993/09/09  19:34:00  ty7777
 * Initial revision
 **/
/* ===============================================================================
 *  Module:         savesxr.c
 *  Date:           11/03/92
 *  Author:         Tom Yang
 *  Description:	Write skull xray data to a file.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/savesxr.c,v 1.4 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdio.h>
#include <myvalues.h>
#include <imgutil.h>

int      savesxr (sxr_name, petfile, sxr_root, program, save_rlslicedim, sxrdata)
	char           *sxr_name;			/* sxr filename */
	char           *petfile;			/* PET filename */
	char           *sxr_root;			/* sxr file root */
	char           *program;			/* program name */
	BOOLEAN         save_rlslicedim;	/* TRUE: save xray dimensions for each slice
										 * FALSE: save only pet parameters */
	SXR_DATA       *sxrdata;			/* structure containing xray data */
{
	FILE           *sxrfp;
	char            user_name[MAXLINE];
	int             i;
	long            date[3];

	sxrfp = fopen (sxr_name, "w");
	if (sxrfp == (FILE *) NULL) {
		fprintf (stderr, "Error in opening file %s\n", sxr_name);
		return ERROR;
	}
	intdate (date);
	getuser (user_name);

	if (sxrdata->incorrect_dz == TRUE)
		fprintf (sxrfp, "%3d   {Created by %s} !!!temporary file with incorrect delta-z value!!!\n",
			 sxrdata->type, program);
	else 
		fprintf (sxrfp, "%3d   {Created by %s}\n", sxrdata->type, program);

	fprintf (sxrfp, "%s; %d%% %s peak template of %s, slices 1-%d; %d/%2.2d/%d; %s\n",
		 sxr_name, sxrdata->threshold,
		 (sxrdata->peaktype == PEAK_PER_SLICE) ? "slice" : "scan", petfile,
		 sxrdata->number_slices, (int) date[1], (int) date[2], (int) date[0], user_name);
	fprintf (sxrfp, "%s\n", sxr_root);
	fprintf (sxrfp, "%6.3f  %7.4f  %2d  %6.3f %2d  %5.2f  %5.2f %7.1f %7.1f %7.1f  %3d %7.4f\n",
		 sxrdata->pixel_size, sxrdata->magnification_factor, sxrdata->number_slices,
	     sxrdata->slice_separation, sxrdata->reference_slice, sxrdata->zatlas_dimension,
		 sxrdata->apatlas_dimension, sxrdata->film_position, sxrdata->scan_position,
		 sxrdata->reference_offset, sxrdata->peakslice, sxrdata->xscale);
	fprintf (sxrfp, "%6.3f %7.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %7.2f %7.2f\n",
		 sxrdata->apxray, sxrdata->degx, sxrdata->zxray,
		 sxrdata->dz, sxrdata->dap, sxrdata->apcenter,
		 sxrdata->ear_separation, sxrdata->eardap,
		 sxrdata->eardz, sxrdata->degy, sxrdata->degz);

	if (save_rlslicedim) {
		for (i = 0; i < sxrdata->number_slices; i++) {
			fprintf (sxrfp, "%5.1f  %5.1f  %5.1f  %5.1f\n",
				 sxrdata->rlpetdim[i],
				 sxrdata->rlcenter[i],
				 sxrdata->vertical_petdim[i],
				 sxrdata->rlslicedim[i]);
		}
	} else {
		for (i = 0; i < sxrdata->number_slices; i++) {
			fprintf (sxrfp, "%5.1f  %5.1f  %5.1f\n",
				 sxrdata->rlpetdim[i],
				 sxrdata->rlcenter[i],
				 sxrdata->vertical_petdim[i]);
		}
	}
	fclose (sxrfp);
	return OK;
}
