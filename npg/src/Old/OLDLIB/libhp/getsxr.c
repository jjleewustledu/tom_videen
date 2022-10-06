/*$Id: getsxr.c,v 1.21 1995/10/30 22:39:01 tom Exp $
/*$Log: getsxr.c,v $
 * Revision 1.21  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.20  1994/09/28  15:20:08  ty7777
 * Change scale to xscale.
 *
 * Revision 1.19  1994/09/28  15:16:44  ty7777
 * Added xscale.
 *
 * Revision 1.18  1994/09/21  20:46:19  ty7777
 * Read peakslice.
 *
 * Revision 1.17  1994/09/20  15:53:08  ty7777
 * Change the constant for peakslice to PEAK_SLICE_UNDEFINED.
 *
 * Revision 1.16  1994/09/20  15:44:08  ty7777
 * Reading the new sxr file with type 22.
 *
 * Revision 1.15  1994/03/03  20:22:37  ty7777
 * Replace id with header.
 *
 * Revision 1.14  1993/10/07  14:27:32  ty7777
 * Added loop for flipping old xray generated data.
 *
 * Revision 1.13  1993/10/06  18:54:41  ty7777
 * Change ENCODED_SLICE_WIDTH to ENCODED_SLICE_WIDTH + 1.
 *
 * Revision 1.12  1993/10/06  14:42:39  ty7777
 * Processing for non flipped ECAT images.
 *
 * Revision 1.11  1993/10/05  22:19:40  ty7777
 * Change the center of slices for older versions of xray.
 *
 * Revision 1.10  1993/09/21  22:15:31  ty7777
 * Just read the 2nd line if not xray file.
 *
 * Revision 1.9  1993/09/13  22:01:33  ty7777
 * Read scan.
 *
 * Revision 1.8  1993/09/10  20:42:10  ty7777
 * Change scantype to peaktype.
 *
 * Revision 1.7  1993/09/10  20:36:52  ty7777
 * Introduced peaktype parameter for SXR_OUTPUT.
 *
 * Revision 1.6  1993/09/09  19:15:30  ty7777
 * Before sxrweld worked.
 *
 * Revision 1.5  1993/08/12  15:06:33  ty7777
 * Processing incorrect delta-z string.
 *
 * Revision 1.4  1993/07/28  19:33:31  ty7777
 * Adding rcsid.
 *
 * Revision 1.3  1993/06/18  16:51:50  ty7777
 * Adding processing for old .sxr file format.
 *
 * Revision 1.2  1993/06/16  19:33:23  ty7777
 * Fix read ref_slice problem.
 *
 * Revision 1.1  1993/04/30  14:05:15  ty7777
 * Initial revision
 *

/*___________________________________________________________________________
  File:		getsxr.c

  Function:	int getsxr (char *sxr_name, SXR_OUTPUT *sxr_output)

  Arguments:	sxr_name,	Name of .sxr file.
		sxr_output,	Structure containing all the information of a
				.sxr file.
  Return:	SUCCEED: successful; FAIL: error.

  Description:	This function gets skull xray data from an sxr file and write
		it to a structure. The user of this function is responsible for
		freeing the memory of sxr_output->rlpetdim, sxr_output->rlcenter,
		sxr_output->vertical_petdim, sxr_output->rlslicedim after the
		structure sxr_output is no longer used.

		The sxr file can be either generated from "sarploc" on Medusa or 
		be generated from running "xray" on the Sun workstations.

  Called by:	xray, t88mean.	

  Calling:	

  Author:	Tom Yang.

  History:	Created by Tom Yang on 04/20/93. 
___________________________________________________________________________*/

#include <petutil.h>

#define	INCORRECT_DZ	"incorrect delta-z"
#define	DEFAULT_XSCALE	0.0

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/getsxr.c,v 1.21 1995/10/30 22:39:01 tom Exp $";

PUBLIC int getsxr (sxr_name, sxr_output)
	char		*sxr_name;
	SXR_OUTPUT	*sxr_output;
{
	BOOLEAN	xray_sxr;
	FILE	*sxrfp;
	char	line [MAXLINE];
	char	sbuffer [MAXLINE];
	char	sbuffer1 [MAXLINE];
	char	sbuffer2 [MAXLINE];
	float	fbuffer [11];
	int	i;
	int	num_scanned;
	int	ibuffer [3];

	sxrfp	= fopen (sxr_name, "r");
	if (sxrfp == NULL)
	{
		fprintf (stderr, "Error in opening sxr file %s\n", sxr_name);
		return FAIL;
	}

	/*
	 * Get SXR file type.
	 */
	fgets (line, MAXLINE, sxrfp);
	num_scanned	= sscanf (line, "%d", ibuffer);
	if (num_scanned == 1)
		sxr_output->type	= ibuffer [0];
	else
	{
		fprintf (stderr, "Make sure your sxr file %s has a number as type\n", sxr_name);
		return FAIL;
	}
	if (strstr (line, INCORRECT_DZ) != NULL)
		sxr_output->incorrect_dz	= TRUE;
	else
		sxr_output->incorrect_dz	= FALSE;

	if (strstr (line, "xray") != NULL)
		xray_sxr	= TRUE;
	else
		xray_sxr	= FALSE;

	/*
	 * Get threshold of PET image.
	 */
	fgets (line, MAXLINE, sxrfp);
	if (xray_sxr)
	{
		if (sxr_output->incorrect_dz)
			num_scanned	= sscanf (line, "%s %s %d%%%s", 
					sbuffer, sbuffer1, ibuffer, sbuffer2);
		else
			num_scanned	= sscanf (line, "%s %d%%%s", 
					sbuffer, ibuffer, sbuffer2);
		if ((sxr_output->incorrect_dz && num_scanned == 4) ||
		!sxr_output->incorrect_dz && num_scanned == 3)
		{
			sxr_output->threshold	= ibuffer [0];
			if (strcmp (sbuffer2, "slice") == 0)
				sxr_output->peaktype	= PEAK_PER_SLICE;
			else
				sxr_output->peaktype	= PEAK_PER_SCAN;
		}
		else
		{
			fprintf (stderr, 
			"Error: cannot extract threshold from your sxr file %s\n", sxr_name);
			return FAIL;
		}
	}

	/*
	 * Get the 3rd line.
	 */
	fgets (line, MAXLINE, sxrfp);

	/*
	 * Get the variables from the 4th line.
	 */
	fgets (line, MAXLINE, sxrfp);
	num_scanned	= sscanf (line, "%f %f %d %f %d %f %f %f %f %f %d %f", fbuffer, 
			fbuffer + 1, ibuffer, fbuffer + 2, ibuffer + 1, fbuffer + 3, 
			fbuffer + 4, fbuffer + 5, fbuffer + 6, fbuffer + 7, ibuffer + 2, 
			fbuffer + 8);
	sxr_output->pixel_size			= fbuffer [0];
	sxr_output->magnification_factor	= fbuffer [1];
	sxr_output->number_slices		= ibuffer [0];
	sxr_output->slice_separation		= fbuffer [2];
	sxr_output->reference_slice		= ibuffer [1];
	sxr_output->zatlas_dimension		= fbuffer [3];
	sxr_output->apatlas_dimension		= fbuffer [4];
	sxr_output->peakslice			= PEAK_SLICE_UNDEFINED;
	sxr_output->xscale			= DEFAULT_XSCALE;
	if (num_scanned >= 10)
	{
		sxr_output->film_position	= fbuffer [5];
		sxr_output->scan_position	= fbuffer [6];
		sxr_output->reference_offset	= fbuffer [7];
		if (num_scanned >= 11)
			sxr_output->peakslice	= ibuffer [2];
		if (num_scanned >= 12)
			sxr_output->xscale	= fbuffer [8];
	}
	else
	{
		sxr_output->film_position	= 0.0;
		sxr_output->scan_position	= 0.0; 
		sxr_output->reference_offset	= 0.0;
	}

	/*
	 * Get the variables from the 5th line.
	 */
	fgets (line, MAXLINE, sxrfp);
	num_scanned	= sscanf (line, "%f %f %f %f %f %f %f %f %f %f %f", 
				fbuffer, fbuffer + 1, fbuffer + 2, fbuffer + 3, 
				fbuffer + 4, fbuffer + 5, fbuffer + 6, fbuffer + 7, 
				fbuffer + 8, fbuffer + 9, fbuffer + 10);
	if (num_scanned == 11)
	{
		sxr_output->apxray		= fbuffer [0];
		sxr_output->degx		= fbuffer [1];
		sxr_output->zxray		= fbuffer [2];
		sxr_output->dz			= fbuffer [3];
		sxr_output->dap			= fbuffer [4];
		sxr_output->apcenter		= fbuffer [5];
		sxr_output->ear_separation	= fbuffer [6];
		sxr_output->eardap		= fbuffer [7];
		sxr_output->eardz		= fbuffer [8];
		sxr_output->degy		= fbuffer [9];
		sxr_output->degz		= fbuffer [10];
	}
	else
	{
		fprintf (stderr, "Error: The 5th line of your sxr file %s is wrong.\n", sxr_name);
		return FAIL;
	}

	sxr_output->rlpetdim		= (float *) malloc (sxr_output->number_slices * sizeof (float));
	sxr_output->rlcenter		= (float *) malloc (sxr_output->number_slices * sizeof (float));
	sxr_output->vertical_petdim	= (float *) malloc (sxr_output->number_slices * sizeof (float));
	sxr_output->rlslicedim		= (float *) malloc (sxr_output->number_slices * sizeof (float));

	for (i = 0; i < sxr_output->number_slices; i++)
	{
		fgets (line, MAXLINE, sxrfp);
		num_scanned	= sscanf (line, "%f %f %f %f\n", fbuffer, 
					fbuffer + 1, fbuffer + 2, fbuffer + 3);
		if (num_scanned == 4)
		{
			sxr_output->vertical_petdim [i]	= fbuffer [2];
			sxr_output->rlslicedim [i]	= fbuffer [3];
		}
		else if (num_scanned <= 2)
		{
			if (num_scanned < 2)
				num_scanned	= sscanf (line, "%f, %f\n", fbuffer, fbuffer + 1);
			if (num_scanned == 2)
			{
				sxr_output->vertical_petdim [i]	= 0;
				sxr_output->rlslicedim [i]	= 0;
			}
			else
			{
				fprintf (stderr, "Error in reading sxr file %s.\n", sxr_name);
				fprintf (stderr, "Unknown format for PET dim and center.\n");
				return FAIL;
			}
		}
		sxr_output->rlpetdim [i]	= fbuffer [0];
		sxr_output->rlcenter [i]	= fbuffer [1];
	}

	/*
	 * Processing for sxr files generated with old ECAT images
	 * with the Radiology convertion.
	 */
	if (sxr_output->type <= XRAY_TYPE_RADIOLOGY 
	&& sxr_output->number_slices == ECAT_NUM_SLICES)
	{
		for (i = 0; i < sxr_output->number_slices; i++)
		{
			sxr_output->rlcenter [i]	= (ENCODED_SLICE_WIDTH + 1) 
							- sxr_output->rlcenter [i];
		}
	}

	fclose (sxrfp);

	return SUCCEED;
}
