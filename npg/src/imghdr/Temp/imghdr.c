/*	$Id: imghdr.c,v 2.21 2003/09/16 16:51:28 tom Exp $
	$Log: imghdr.c,v $
 * Revision 2.21  2003/09/16  16:51:28  tom
 * omit scatter for 9110
 *
 * Revision 2.20  2003/09/12  16:03:32  tom
 * *** empty log message ***
 *
 * Revision 2.19  2003/01/23  15:19:06  tom
 * add processing_code check
 *
 * Revision 2.18  2002/12/26  20:33:24  tom
 * add O-15
 *
	Revision 2.17  2002/07/16 15:28:34  tom
	add Compound to output

 * Revision 2.16  2002/05/30  14:00:38  tom
 * use REV7NSCALE for all non-calibrated matrix7 images
 *
 * Revision 2.15  2002/03/27  15:11:22  tom
 * add facility name
 *
 * Revision 2.14  2001/08/30  14:21:19  tom
 * add original filename for Matrix 7
 *
 * Revision 2.13  2000/07/18  17:12:12  tom
 * *** empty log message ***
 *
 * Revision 2.12  1999/12/15  21:10:38  tom
 * lint clean and y2k fix
 *
 * Revision 2.11  1999/10/05  13:08:03  tom
 * remove check for old norm
 *
 * Revision 2.10  1997/05/13  16:33:11  tom
 * fractional start time and duration
 *
 * Revision 2.9  1997/03/10  16:48:48  tom
 * use firstimgfrm
 *
 * Revision 2.8  1997/03/10  15:24:22  tom
 * look for frame 3
 *
 * Revision 2.7  1996/10/16  15:56:09  tom
 * new libimage
 *
 * Revision 2.6  1996/09/10  16:01:45  tom
 * change mm to cm
 *
 * Revision 2.5  1996/08/15  18:40:09  tom
 * corrected Max/Min for Matrix 7
 *
 * Revision 2.4  1996/08/13  17:53:51  tom
 * add File format and Z-origin
 *
 * Revision 2.3  1996/05/24  18:10:49  tom
 * use ATNSCALER instead of 10000
 *
 * Revision 2.2  1996/05/22  20:45:54  tom
 * fix s1 for attenuation images
 *
 * Revision 2.1  1996/05/22  20:35:37  tom
 * add decay correction factor
 *
 * Revision 2.0  1996/04/30  20:56:03  tom
 * new libimage.a
 *
 * Revision 1.17  1996/04/30  20:55:35  tom
 * *** empty log message ***
 *
 * Revision 1.16  1996/03/12  23:05:53  tom
 * *** empty log message ***
 *
 * Revision 1.15  1996/02/19  22:50:34  tom
 * change default scaler for scanlen=0 to 10000.
 *
 * Revision 1.14  1996/02/19  22:25:53  tom
 * set s1 to 1 for transmission images (where scan duration = 0)
 *
 * Revision 1.13  1996/01/03  19:58:32  tom
 * set version to 1.16
 * ,
 *
 * Revision 1.12  1996/01/03  19:57:48  tom
 * link with new libimg.a
 *
 * Revision 1.11  1996/01/03  18:06:18  tom
 * add archive type
 *
 * Revision 1.10  1995/12/20  16:07:28  tom
 * new libimg.a
 *
 * Revision 1.9  1995/12/19  15:10:30  tom
 * add frame start time
 *
 * Revision 1.8  1995/12/19  14:00:00  tom
 * add Max, Min, Frame length
 *
 * Revision 1.7  1995/12/18  17:17:32  tom
 * add printout of max in min values in frame
 *
 * Revision 1.6  1995/12/18  15:00:22  tom
 * elim unused variables
 *
 * Revision 1.5  1995/12/13  22:00:20  tom
 * add Image scaled by
 *
 * Revision 1.4  1995/12/12  18:51:46  tom
 * check frame 2 if no frame 1
 *
 * Revision 1.3  1995/11/30  22:00:01  tom
 * *** empty log message ***
 *
 * Revision 1.2  1995/11/28  21:25:16  tom
 * add Ver for 961
 *
 * Revision 1.1  1995/11/28  21:18:26  tom
 * Initial revision
 * */
/*	===============================================================================
 *	Module:			imghdr.c
 *	Date:			Dec-95
 *	Author:			Tom Videen
 *	Description:	Read headers from ECAT, PETT VI or Interfiles
 *	===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Id: imghdr.c,v 2.21 2003/09/16 16:51:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>
#include <pettable.h>

main (argc, argv)
	int             argc;
	char           *argv[];
{
	ImageFile      *file;
	Index           indx;
	int             index;
	int             index2;
	int             filtercode;
	float           factor = 0;

	int             frm;
	int             pc;
	char           *fname;
	char           *isotope;
	char           *radiopharmaceutical;
	char            fileformat[5][12];
	char            pcode[80];
	SubHeader      *sh;
	MainHeader     *mh;
	float           minval, maxval;
	float           mintmp, maxtmp;
	float           scaler, s1;
	float           subscaler = 1;
	int             pln;

/*
 *	Get command line arguments.
 */

	if (argc < 2 || argc > 3) {
		fprintf (stderr, "Usage: imghdr filename [frame]\n");
		return (ERROR);
	}
	fname = argv[1];
	if (argc == 3)
		frm = atoi (argv[2]);
	else
		frm = 0;
	indx.frm = frm;
	indx.pln = 1;
	indx.gate = 1;
	indx.data = 0;
	indx.bed = 0;

	(void) strcpy (fileformat[0], "Unknown");/* see image.h defines */
	(void) strcpy (fileformat[1], "Matrix 6");	/* see image.h defines */
	(void) strcpy (fileformat[2], "Matrix 7");
	(void) strcpy (fileformat[3], "Interfile");
	(void) strcpy (fileformat[4], "PETT VI");

	file = make_imgfile (fname);
	if (open_imgfile (file) == ERROR) {
		fprintf (stderr, "ERROR [imghdr]: Could not open %s\n", fname);
		return (ERROR);
	}
	if (get_mh (file) == ERROR) {
		fprintf (stderr, "ERROR [imghdr]: Could not get MainHeader from %s\n", fname);
		return (ERROR);
	}
	if (indx.frm == 0) {
		indx.frm = firstimgfrm (file);
		if (indx.frm <= 0) {
			fprintf (stderr, "ERROR [imghdr]: cannot find %s frame 1-%d\n", fname, -indx.frm);
			return (ERROR);
		}
	}
	if (existimgfrm (file, &indx) == ERROR) {
		fprintf (stderr, "ERROR [imghdr]: cannot find %s frame %d\n", file->name, frm);
		return (ERROR);
	}
	if (get_sh (file, &indx) == ERROR) {
		fprintf (stderr, "ERROR [imghdr]: Could not read file subheader in %s\n", file->name);
		return (ERROR);
	}
	mh = file->mh;
	sh = file->sh;
	printf ("------------------------------------------------\n");
	if (strncmp(mh->date_time,"12/31/1969",10) == 0)
		printf ("%s     (no date)\n", file->name);
	else
		printf ("%s     %s\n", file->name, mh->date_time);
	printf ("File format = %s\n", fileformat[mh->file_format]);
	if (mh->file_format == 2)
		printf ("Original file name = %s\n", file->m7->mh->original_file_name);

	if (mh->pett6_flag == 1 || mh->scanner_type == 6)
		printf ("PETT VI\n");
	else if (mh->file_format != INTERFILE) {
		if (mh->scanner_type == 9110)
			printf ("ECAT %d  (GE Advance)\n", mh->scanner_type);
		else
			printf ("ECAT %d\n", mh->scanner_type);
		if (mh->wobble_speed > 0)
			printf ("Wobbled acquisition\n");
		else
			printf ("Stationary acquisition\n");

		(void) strcpy (pcode, "");
		pc = sh->processing_code;
		if (pc > 0) {
			if (pc & 1)
				(void) strcat (pcode, "Norm");
			if (pc & 2)
				(void) strcat (pcode, "+AttenMeas");
			if (pc & 4)
				(void) strcat (pcode, "+AttenCalc");
			if (pc & 8)
				(void) strcat (pcode, "+XSmooth");
			if (pc & 16)
				(void) strcat (pcode, "+YSmooth");
			if (pc & 32)
				(void) strcat (pcode, "+ZSmooth");
			if (pc & 64)
				(void) strcat (pcode, "+ScatCor2d");
			if (pc & 128)
				(void) strcat (pcode, "+ScatCor3d");
			if (pc & 256)
				(void) strcat (pcode, "+ArcCor");
			if (pc & 512)
				(void) strcat (pcode, "+DecayCor");
		}
		if ((sh->filter_scatter_fraction > 0) && (sh->filter_scatter_slope > 0)
			|| (pc & 64) || (pc & 128))
			printf ("Scatter-corrected  (%s)\n", pcode);
		else if (mh->scanner_type != 9110)
			printf ("No correction for scatter\n");

	}
	if (mh->file_format == 1)
		printf ("Software (Ver %d)  Facility = %s\n", mh->sw_version, file->m6->mh->facility_name);
	else if (mh->file_format == 2)
		printf ("Software (Ver %d)  Facility = %s\n", mh->sw_version, file->m7->mh->facility_name);
	else
		printf ("Software (Ver %d)\n", mh->sw_version);

	index2 = 0;
	if (mh->file_format != INTERFILE) {
		filtercode = sh->filter_code;
		if (abs (filtercode - GAUSSIAN_FILTER_CODE) <= 6) {
			index = abs (filtercode - GAUSSIAN_FILTER_CODE);
			index2 = GAUSS_INDEX;
		} else if (abs (filtercode - BUTTERWORTH_FILTER_CODE) <= 6) {
			index = abs (filtercode - BUTTERWORTH_FILTER_CODE);
			index2 = BUTTERWORTH_INDEX;
		} else
			index = abs (filtercode);

		if (mh->pett6_flag != 1)
			printf ("Filter = %s  Reconstruction Cutoff = %f\n", filterTable[index],
				sh->filter_cutoff_frequency);

		if (index2 == BUTTERWORTH_INDEX)
			printf ("3-D Butterworth:  Cutoff = %.2f  Order = %.0f\n",
				sh->filter_ramp_slope, sh->filter_order);
		else if (index2 == GAUSS_INDEX)
			printf ("3-D Gaussian:  Cutoff = %.2f\n", sh->filter_ramp_slope);

		factor = mh->calibration_factor;
		if (mh->pett6_flag != 1)
			printf ("ECAT calibration factor = %f\n", factor);
		if (mh->file_format == MATRIX_REV7)
			printf ("Decay correction factor = %f\n", file->m7->sh->decay_corr_fctr);
	}
	printf ("Pixel size = %f cm\n", sh->sample_size_1);
	printf ("Plane_separation = %f cm\n", mh->slicesep);
	isotope = mh->isotope;
	radiopharmaceutical = mh->radiopharmaceutical;
	printf ("Isotope =  %s   Compound = %s\n", isotope, radiopharmaceutical);

	if (factor < 1.0001 && factor > 0.9999) {
		if (strcmp (isotope, "F-18") != 0 &&
		    strcmp (isotope, "C-11") != 0 &&
		    strcmp (isotope, "O-15") != 0 &&
		    strcmp (isotope, "Ge-68") != 0 &&
		    strcmp (isotope, "Ga-68") != 0) {
			printf ("UNKNOWN isotope: %s\n", isotope);
			printf ("Unable to correct pie factor!\n");
		}
	}
	if (mh->scanner_type != 6)
		printf ("%d slices   %d frames   Bed Position: %f cm\n", mh->nplane,
			mh->nframe, mh->init_bed_pos);
	else
		printf ("%d slices   %d frames\n", mh->nplane, mh->nframe);

	if (mh->zorigin != 0)
		printf ("Z-origin = %d\n", mh->zorigin);

	if (mh->orientation == 0)
		printf ("Right-on-Right\n");
	else if (mh->orientation == 1)
		printf ("Right-on-Left\n");
	else if (mh->orientation == 2)
		printf ("Right-on-Left; Flipped Anterior/Posterior\n");
	else
		printf ("Non-standard Orientation\n");
	if (mh->scale_flag != 0)
		printf ("Image scaled by %f\n", mh->pr_scaler);

	if (mh->file_format == MATRIX_REV6 || mh->file_format == MATRIX_REV7) {
		printf ("Created with: %s\n", mh->sw_revision);
		printf ("Date: %s\n", mh->filetime);
	}
	if (mh->file_format == MATRIX_REV6) {
		scaler = mh->calibration_factor;
		s1 = (float) sh->duration;
		if (sh->duration == 0)
			s1 = ATNSCALER;
		if (sh->scale_factor > 0.)
			subscaler = s1 * sh->scale_factor;
		if (sh->calibration_factor > 0.)
			subscaler *= sh->calibration_factor;
		maxval = scaler * subscaler * sh->max;
		minval = scaler * subscaler * sh->min;
		for (pln = 2; pln <= mh->nplane; pln++) {
			indx.pln = pln;
			if (get_sh (file, &indx) != ERROR) {
				if (sh->scale_factor > 0.)
					subscaler = s1 * sh->scale_factor;
				if (sh->calibration_factor > 0.)
					subscaler *= sh->calibration_factor;
				maxtmp = scaler * subscaler * sh->max;
				mintmp = scaler * subscaler * sh->min;
				if (maxtmp > maxval)
					maxval = maxtmp;
				if (mintmp < minval)
					minval = mintmp;
			}
		}
		printf ("Max = %f   Min = %f\n", maxval, minval);

	} else if (mh->file_format == MATRIX_REV7) {
		scaler = mh->calibration_factor;
		s1 = (float) sh->duration;
		if (mh->file_format == MATRIX_REV7) {
			if (strncmp (file->m7->mh->data_units, "1/cm", 4) == 0)
				s1 = ATNSCALER;
			if (file->m7->mh->calibration_factor == 1 || file->m7->sh->filter_scatter_fraction == 0
			    || file->m7->sh->filter_scatter_slope == 0)
				s1 = s1 * REV7NSCALE / file->m7->sh->decay_corr_fctr;
			else
				s1 = s1 * REV7SCALER / file->m7->sh->decay_corr_fctr;
		}
		if (sh->duration == 0)
			s1 = ATNSCALER;
		if (sh->scale_factor > 0.)
			subscaler = s1 * sh->scale_factor;
		if (sh->calibration_factor > 0.)
			subscaler *= sh->calibration_factor;
		maxval = scaler * subscaler * sh->max;
		minval = scaler * subscaler * sh->min;
		printf ("Max = %f   Min = %f\n", maxval, minval);
	}
	printf ("Frame length = %.3f sec\n", (float) sh->duration / 1000.);
	printf ("Start time = %.3f sec\n", (float) sh->start_time / 1000.);
	if (mh->file_format != INTERFILE)
		printf ("Archive Type: %s %s %s %s\n", mh->scanner, mh->acquisition, mh->reconstruction, mh->filtername);
	free (file->name);
	free_imgfile (file);
	return (OK);
}
