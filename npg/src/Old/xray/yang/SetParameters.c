/*$Id: SetParameters.c,v 1.13 1996/07/10 14:54:46 yang Exp $*/
/*$Log: SetParameters.c,v $
 * Revision 1.13  1996/07/10  14:54:46  yang
 * Added ECT961_REFERENCE.
 *
 * Revision 1.12  1995/11/30  19:14:30  ty7777
 * Added processing for 961 images.
 *
 * Revision 1.11  1995/11/27  20:41:24  ty7777
 * Updated for 953B new offset.
 *
 * Revision 1.10  1995/11/03  21:32:25  ty7777
 * Updated for ECAT 7.0.
 *
 * Revision 1.9  1995/06/30  16:05:50  ty7777
 * Added processing for Emission 1000.
 *
 * Revision 1.8  1995/06/27  19:10:47  ty7777
 * Updated to take any file names for HR data.
 *
 * Revision 1.7  1995/06/27  16:42:32  ty7777
 * Added processing for HR961 data.
 *
 * Revision 1.6  1995/02/13  22:02:24  ty7777
 * Added sxr_root to the argument list.
 *
 * Revision 1.5  1995/02/08  15:27:41  ty7777
 * Fixed the logic bug for checking scans acquired after oct., 95.
 *
 * Revision 1.4  1994/11/02  15:32:36  ty7777
 * Added new argument acquired_after_oct94.
 *
 * Revision 1.3  1994/09/20  15:59:13  ty7777
 * Added peakslice.
 *
 * Revision 1.2  1993/10/06  14:51:05  ty7777
 * Change XRAY_TYPE to XRAY_TYPE_NEUROLOGY.
 *
 * Revision 1.1  1993/09/14  16:16:21  ty7777
 * Initial revision
 *
 * Revision 1.1  1993/09/10  20:35:08  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid [] = "@(#)SetParameters.c	11/03/92  Copyright Neural Pet Group, Washington University 1992";
#endif

static char rcsid [] = "$Header: /home/petsun4/yang/src/xray/RCS/SetParameters.c,v 1.13 1996/07/10 14:54:46 yang Exp $";

/*___________________________________________________________________________
  File:		SetParameters.c

  Function:	void SetParameters ()

  Arguments:	

  Description:	This function assigns the parameters type, pix, slcsize, zatldim, apaldim.

  Called by:	xray.c and sxrweld.c 	

  Calling:	

  History:	Created by Tom Yang on 11/03/92. 
___________________________________________________________________________*/

#include <petutil/petutil.h>
#include <petutil/matrix_64.h>
#include "xray.h"
#define	CUTOFF_YEAR	1994
#define	CUTOFF_MONTH	11

PUBLIC int SetParameters (image_name, sxr_root, scanner_type, encoded, num_slices, 
	sxr_output, petimagetype, trim, acquired_after_oct94, hrscanner, assigned_threshold)
	char		*image_name;
	char		*sxr_root;
	int		scanner_type;
	BOOLEAN		encoded;
	short int	*num_slices;
	SXR_OUTPUT	*sxr_output;
	char		*petimagetype;
	char		*trim;
	BOOLEAN		*acquired_after_oct94;
	BOOLEAN		*hrscanner;
	BOOLEAN		assigned_threshold;
{
	BOOLEAN		err_value;
	Main_header_64	main_header64;
	MatrixFile	*fptr;
	PETT6HDR	*pett6_header;
	char		*string_patient_id;
	float		pixel_size;
	float		plane_separation;
	int		diff;  /* number of differences between spec and actual */
	int		fd;
	int		file_type;
	int		patient_id;
	short		height;
	short		*header;
	short		*slice_data;
	short		width;
	struct Matval	matval;

	sxr_output->type	= XRAY_TYPE_XRAY32;

	fd	= open (image_name, O_RDONLY, 0);
	if (fd == -1)
	{
		fprintf (stderr, "Can't open %s\n", image_name); 
		return FAIL;
	}

	if (scanner_type != ECAT953_SCANNER && scanner_type != ECAT961_SCANNER)
	{
		/* 
	 	 * Allocate header. 
	 	 */
		header = (short *) calloc (HEADER_SIZE / sizeof (short), sizeof (short));
		if (header == (short *) NULL)
		{
			pkg_message (PKG_ERROR, "xray", "SetParameters", "calloc",
					"Error: Can't allocate header.\n");
			return FAIL;
		}

		slice_data	= (short *) calloc (ENCODED_SLICE_DIM, sizeof (short));
		if (slice_data == (short *) NULL)
		{
			pkg_message (PKG_ERROR, "xray", "SetPetParameters", "calloc",
					"Error: Can't allocate slice_data.\n");
			return FAIL;
		}

		getimage (slice_data, header, fd, image_name, 1, 
				encoded, *num_slices, scanner_type, &err_value);
		pett6_header	= (PETT6HDR *) header;
		file_type	= pett6_header->file_type;
		if (file_type == TRANSMISSION_PET1010)
		{
			*petimagetype	= TRANSMISSION_SCAN;
			if (!assigned_threshold)
				sxr_output->threshold	= DEFAULT_PETT6_TRANS_THRESH;
		}
		else if (file_type == EMISSION_PET1000 || file_type == EMISSION_PET1020 
		|| file_type == EMISSION_PET2000 || file_type == EMISSION_PET2010)
		{
			*petimagetype	= EMISSION_SCAN;
			*trim		= 0;
		}
		else
		{
			fprintf (stderr, "Unknown PETT VI scan type %d.\n", file_type);
			return FAIL;
		}

		sxr_output->scan_position	= 0.0;
		sxr_output->pixel_size		= PETT6_PIXEL_SIZE;
		sxr_output->slice_separation	= PETT6_SEPARATION;
		sxr_output->reference_slice	= PETT6_REFERENCE;
		sxr_output->reference_offset	= PETT6_OFFSET;

		free (header);
		free (slice_data);
	}
	else
	{
		/*
	 	 * Assign default values for matval.
	 	 */
		matval.frame	= 1;
		matval.plane	= 1;
		matval.gate	= 1;
		matval.data	= 0;
		matval.bed	= 0;

		fptr	= OpenEcat (image_name, num_slices, &width, &height, 
				&pixel_size, &plane_separation, matval);
		if (fptr == (MatrixFile *) NULL)
		{
			fprintf (stderr, "SetParameters: OpenEcat error.\n");
			return (FAIL);
		}

		mh64_convert (&main_header64, fptr->mhptr);

		if (fptr->mhptr->system_type == SYSTEM_TYPE_961)
		{
			*hrscanner			= TRUE;
			sxr_output->scan_position	= ECAT961_OFFSET;
			sxr_output->reference_slice	= ECAT961_REFERENCE;
		}
		else
		{
			*hrscanner	= FALSE;

			string_patient_id	= strpbrk (sxr_root, "0123456789");
			if (string_patient_id == NULL)
				return FAIL;
			patient_id		= atoi (string_patient_id);

			if (patient_id >= FIRST_SCAN_AFTER_OCT95)
			{
				*acquired_after_oct94		= TRUE;
				sxr_output->scan_position	= ECAT953_OFFSET_AFTER_OCT95;
			}
			else if (patient_id >= FIRST_SCAN_AFTER_OCT94)
			{
				*acquired_after_oct94		= TRUE;
				sxr_output->scan_position	= ECAT_OFFSET_AFTER_OCT94;
			}
			else if (main_header64.scan_start_year > CUTOFF_YEAR 
			|| (main_header64.scan_start_year == CUTOFF_YEAR 
			&& main_header64.scan_start_month >= CUTOFF_MONTH))
			{
				fprintf (stderr, 
				"Error: your scan date and patient id do not match in image.\n"); 
				return FAIL;
			}
			else
			{
				*acquired_after_oct94		= FALSE;
				sxr_output->scan_position	= ECAT_OFFSET;
			}
			sxr_output->reference_slice	= ECAT_REFERENCE;
		}

		if (plane_separation == 0.0)
			plane_separation	= DEFAULT_ECAT_SEP;

		if (main_header64.acquisition_type == TRANSMISSION_ECAT)
		{
			*petimagetype	= TRANSMISSION_SCAN;
			if (!assigned_threshold)
				sxr_output->threshold	= DEFAULT_ECAT_TRANS_THRESH;
		}
		else if (main_header64.acquisition_type > TRANSMISSION_ECAT)
		{
			*petimagetype	= EMISSION_SCAN;
			if (!assigned_threshold)
				sxr_output->threshold	= DEFAULT_ECAT_EMISSION_THRESH;
			*trim		= 0;
		}
		else
		{
			fprintf (stderr, "Unknown ECAT scan type %d.\n", scanner_type);
			return FAIL;
		}

		matrix_close (fptr);

		sxr_output->pixel_size		= pixel_size;
		sxr_output->slice_separation	= plane_separation;
		sxr_output->reference_offset	= 0.0;

	}
	close (fd);

	sxr_output->zatlas_dimension	= ZATLAS_DIMENSION;
	sxr_output->apatlas_dimension	= APATLAS_DIMENSION;

	return SUCCEED;
}
