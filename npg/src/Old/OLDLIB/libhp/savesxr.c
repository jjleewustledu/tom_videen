/*$Id: savesxr.c,v 1.10 1995/10/30 22:39:01 tom Exp $*/
/*$Log: savesxr.c,v $
 * Revision 1.10  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
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

#ifndef lint
static char sccsid [] = "@(#)savesxr.c	11/03/92  Copyright Neural Pet Group, Washington University 1992";
#endif

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/savesxr.c,v 1.10 1995/10/30 22:39:01 tom Exp $";

/*___________________________________________________________________________
  File:		savesxr.c

  Function:	int savesxr (char *sxr_name, char *petfile, char *sxr_root,
		char *program, BOOLEAN save_rlslicedim, BOOLEAN incorrect_delta_z,
		SXR_OUTPUT sxr_output)

  Arguments:	sxr_name,		sxr file name.
		petfile,		pet image file name.
		sxr_root,		sxr file root.
		program,		program name.
		save_rlslicedim,	TRUE: save xray dimensions for each slice;
					FALSE: save only pet parameters.
		incorrect_delta_z,	TRUE: incorrect_delta_z;
		sxr_output,		structure containing xray data.

  Description:	This function writes skull xray data and atlas/tomograph
		constants to a file.

  Called by:	

  Calling:	

  Author:	Tom Yang.

  History:	Created by Tom Yang on 11/03/92. 
___________________________________________________________________________*/

#include <petutil.h>

PUBLIC int savesxr (sxr_name, petfile, sxr_root, program, save_rlslicedim, incorrect_delta_z, sxr_output)
	char		*sxr_name;
	char		*petfile;
	char		*sxr_root;
	char		*program;
	BOOLEAN		save_rlslicedim;
	BOOLEAN		incorrect_delta_z;
	SXR_OUTPUT	sxr_output;
{
	FILE	*sxrfp;
	char	user_name [MAXLINE];
	int	i;
	long	date [DATE_COMPONENTS];

	sxrfp	= fopen (sxr_name, "w");
	if (sxrfp == (FILE *) NULL)
	{
		fprintf (stderr, "Error in opening file %s\n", sxr_name);
		return FAIL;
	}

	intdate_ (date);
	getuser_ (user_name);

	if (incorrect_delta_z)
	{
		fprintf (sxrfp, 
		"%3d   {Created by %s} !!!temporary file with incorrect delta-z value!!!\n", 
		sxr_output.type, program);  
	}
	else
	{
		fprintf (sxrfp, "%3d   {Created by %s}\n", sxr_output.type, program);  
	}

	fprintf (sxrfp, "%s; %d%% %s peak template of %s, slices 1-%d; %d/%2.2d/%d; %s\n", 
		sxr_name, sxr_output.threshold, 
		(sxr_output.peaktype == PEAK_PER_SLICE)? "slice": "scan", petfile, 
		sxr_output.number_slices, date [MONTH], date [DAY], date [YEAR], user_name);
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

	if (save_rlslicedim)
	{
		for (i = 0; i < sxr_output.number_slices; i++)
		{
			fprintf (sxrfp, "%5.1f  %5.1f  %5.1f  %5.1f\n", 
			sxr_output.rlpetdim [i], 
			sxr_output.rlcenter [i], 
			sxr_output.vertical_petdim [i],
			sxr_output.rlslicedim [i]);
		}
	}
	else
	{
		for (i = 0; i < sxr_output.number_slices; i++)
		{
			fprintf (sxrfp, "%5.1f  %5.1f  %5.1f\n", 
			sxr_output.rlpetdim [i], 
			sxr_output.rlcenter [i], 
			sxr_output.vertical_petdim [i]);
		}
	}

	fclose (sxrfp);

	return SUCCEED;
}
