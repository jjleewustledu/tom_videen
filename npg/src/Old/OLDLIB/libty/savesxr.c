/*  $Id$
    $Log$
 */
/* ===============================================================================
 *  Module:         savesxr.c
 *  Date:           11/03/92
 *  Author:         Tom Yang
 *  Description:    Write skull xray data and atlas/tomograph constants to a file.
 *  Return:         SUCCEED or FAIL
 *  History:
 *      13-Nov-95   Modified TOV
 * ===============================================================================
 */
#include <petutil.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/savesxr.c,v 1.10 1995/11/10 16:29:26 tom Exp $";

PUBLIC int      savesxr (sxr_name, petfile, sxr_root, program, save_rlslicedim, incorrect_delta_z, sxr_output)
	char           *sxr_name;			/* sxr filename */
	char           *petfile;			/* PET filename */
	char           *sxr_root;			/* sxr file root */
	char           *program;			/* program name */
	BOOLEAN         save_rlslicedim;	/* TRUE: save xray dimensions for each slice */
										/* FALSE: save only pet parameters (/
	BOOLEAN         incorrect_delta_z;	/* TRUE: incorrect_delta_z */
	SXR_OUTPUT      sxr_output;			/* structure containing xray data */
{
	FILE           *sxrfp;
	char            user_name[MAXLINE];
	int             i;
	long            date[DATE_COMPONENTS];

	sxrfp = fopen (sxr_name, "w");
	if (sxrfp == (FILE *) NULL) {
		fprintf (stderr, "Error in opening file %s\n", sxr_name);
		return FAIL;
	}
	intdate_ (date);
	getuser_ (user_name);

	if (incorrect_delta_z) {
		fprintf (sxrfp,
		      "%3d   {Created by %s} !!!temporary file with incorrect delta-z value!!!\n",
			 sxr_output.type, program);
	} else {
		fprintf (sxrfp, "%3d   {Created by %s}\n", sxr_output.type, program);
	}

	fprintf (sxrfp, "%s; %d%% %s peak template of %s, slices 1-%d; %d/%2.2d/%d; %s\n",
		 sxr_name, sxr_output.threshold,
		 (sxr_output.peaktype == PEAK_PER_SLICE) ? "slice" : "scan", petfile,
		 sxr_output.number_slices, date[MONTH], date[DAY], date[YEAR], user_name);
	fprintf (sxrfp, "%s\n", sxr_root);
	fprintf (sxrfp, "%6.3f  %7.4f  %2d  %6.3f %2d  %5.2f  %5.2f %7.1f %7.1f %7.1f  %3d %7.4f\n",
		 sxr_output.pixel_size, sxr_output.magnification_factor, sxr_output.number_slices,
	     sxr_output.slice_separation, sxr_output.reference_slice, sxr_output.zatlas_dimension,
		 sxr_output.apatlas_dimension, sxr_output.film_position, sxr_output.scan_position,
		 sxr_output.reference_offset, sxr_output.peakslice, sxr_output.xscale);
	fprintf (sxrfp, "%6.3f %7.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %7.2f %7.2f\n",
		 sxr_output.apxray, sxr_output.degx, sxr_output.zxray,
		 sxr_output.dz, sxr_output.dap, sxr_output.apcenter,
		 sxr_output.ear_separation, sxr_output.eardap,
		 sxr_output.eardz, sxr_output.degy, sxr_output.degz);

	if (save_rlslicedim) {
		for (i = 0; i < sxr_output.number_slices; i++) {
			fprintf (sxrfp, "%5.1f  %5.1f  %5.1f  %5.1f\n",
				 sxr_output.rlpetdim[i],
				 sxr_output.rlcenter[i],
				 sxr_output.vertical_petdim[i],
				 sxr_output.rlslicedim[i]);
		}
	} else {
		for (i = 0; i < sxr_output.number_slices; i++) {
			fprintf (sxrfp, "%5.1f  %5.1f  %5.1f\n",
				 sxr_output.rlpetdim[i],
				 sxr_output.rlcenter[i],
				 sxr_output.vertical_petdim[i]);
		}
	}
	fclose (sxrfp);
	return SUCCEED;
}
