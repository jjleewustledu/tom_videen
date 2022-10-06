/*$Id: Ecat2Pett6.c,v 1.22 1995/10/30 22:39:01 tom Exp $*/
/*$Log: Ecat2Pett6.c,v $
 * Revision 1.22  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.21  1995/09/28  18:26:40  ty7777
 * Updated for 961 data.
 *
 * Revision 1.20  1995/09/05  17:24:57  ty7777
 * Use ERROR_BOUND.
 *
 * Revision 1.19  1995/05/18  19:20:35  ty7777
 * Same as the last version.
 *
 * Revision 1.18  1994/11/07  20:53:51  ty7777
 * Edited the documentation.
 *
 * Revision 1.17  1994/10/14  16:28:32  ty7777
 * Added math.h.
 *
 * Revision 1.16  1994/03/03  19:52:48  ty7777
 * Replace id with header.
 *
 * Revision 1.15  1993/10/08  17:16:23  ty7777
 * Added processing for ECAT images created from PETT VI images.
 *
 * Revision 1.14  1993/10/08  15:06:51  ty7777
 * Assign ecat_orientation to 1 after reflection.
 *
 * Revision 1.13  1993/10/07  14:42:15  ty7777
 * Moved ECAT_NUM_SLICES to /usr/local/include/petutil/petutil.h.
 *
 * Revision 1.12  1993/10/07  14:39:38  ty7777
 * Same as the las one.
 *
 * Revision 1.11  1993/10/05  21:08:27  ty7777
 * Added comment_info ().
 *
 * Revision 1.10  1993/10/04  19:49:13  ty7777
 * Added reflection to slice_data if the ECAT image is not reflected.
 *
 * Revision 1.9  1993/07/23  15:33:28  ty7777
 * Move MILLISECONDS_PER_SECOND to /usr/local/include/petutil.h
 *
 * Revision 1.8  1993/07/14  19:21:39  ty7777
 * Added DEFAULT_FRAME_DURATION.
 *
 * Revision 1.7  1993/07/14  19:16:24  ty7777
 * Added ecat_calibration_fctr in the scaling factor.
 *
 * Revision 1.6  1993/07/12  23:48:35  ty7777
 * Adding ecat_calibration_fctr to scaling factor.
 *
 * Revision 1.5  1993/05/10  21:01:17  ty7777
 * filter code -1 is ramp filter.
 *
 * Revision 1.4  1993/05/05  18:55:00  ty7777
 * After getrealimg () and writerealimg () worked.
 *
 * Revision 1.3  1993/04/28  16:00:13  ty7777
 * Modified array copying.
 *
 * Revision 1.2  1993/04/21  16:58:43  ty7777
 * Initial checking in.
 *
 * Revision 1.1  1993/03/03  20:29:13  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid[]="@(#)Ecat2Pett6.c	10/09/92  Copyright Neural Pet Group, Washington University 1992";
#endif

static char rcsid []	= "$Header: /home/npggw/tom/src/libhp/RCS/Ecat2Pett6.c,v 1.22 1995/10/30 22:39:01 tom Exp $";

/*___________________________________________________________________________________
	File Name:	Ecat2Pett6.c

	Function Name:	Ecat2Pett6

	Arguments:	fptr,		Pointer to a MatrixFile structure.
					Get this pointer before calling
					with matrixx_open () function from 
					the CTI library. Free memory after
					the pointer is not used anymore.
			scale_factor,	Multiplied times each pixel.
			pett6_header,	Pointer to a PETT6HDR structure.
					Allocate memory before calling.
			slice_data,	Pointer to a short integer array.
					Allocate memory before calling.
			num_slices,	number of slices in image.
			matval,		structure containing information of
					frame, plane, gate, data, and bed.
	Return Value:	SUCCEED: successful; FAIL: failed.

	Description:	Read a slice of ECAT image and fill the fields of a PETT6HDR 
			structure with either default information or information from 
			the ECAT main header and the ECAT image subheader.

	Called by:	getimage.c in this library.
			ecat2pett6.c in the program "ecat2pett6".

	Calling:	FillHeaderPett6 (), ieee2pe (), itoa (), mat_numcod (), 
			matrixx_read ().
	 
	Author:		Tom Yang

	Date:		04/10/1992
___________________________________________________________________________________*/

#include <stdio.h>
#include <math.h>
#include <matrix_64.h>
#include <petutil.h>
#include <pettable.h>

#define	MAX_YEAR_LEN		4

int Ecat2Pett6 (fptr, scale_factor, pett6_header, slice_data, num_slices, matval)
MatrixFile	*fptr;
float		scale_factor;
PETT6HDR	*pett6_header;
short		*slice_data;
short		num_slices;
struct Matval	matval;
{
	MatrixData	*matrix_read ();
	int		mat_numcod ();

	COMMENT_INFO		comment_info_data;
	Main_header		*main_header;
	Main_header_64		main_header64;
	MatDirNode		*node;
	MatrixData		*matrix_data;
	MatrixData		*volume;
	Image_subheader		*image_subheader;
	Image_subheader_64	image_subheader64;
	char			day [MAX_YEAR_LEN + 1];
	char			filter_params [FILTER_PARAMS_LEN + 1];
	char			month [MAX_YEAR_LEN + 1];
	char			year [MAX_YEAR_LEN + 1];
	int			dimension;
	int			filter_name_index;
	int			frame_duration;
	int			i;
	int			matnum;
	int			slice_index;
	float			decay;
	float			ecat_calibration_fctr;
	float			quant_factor;
	short			*raw_slice_data;

	/*
	 * Get the fields filled for the MatrixData structure.
	 */
	main_header	= fptr->mhptr;
	mh64_convert (&main_header64, main_header);

	if (main_header->file_type == PetImage || main_header->file_type == PetVolume
	|| main_header->file_type == ByteImage || main_header->file_type == ByteVolume
	|| main_header->file_type == InterfileImage)
	{
		if (main_header->file_type == PetVolume)
		{
			slice_index	= 0;
			node = fptr->dirlist->first;
			while (node && slice_index < matval.plane) 
			{
				volume = matrix_read (fptr, node->matnum, MAT_SUB_HEADER);

				for (i = 0; i < volume->zdim && slice_index < matval.plane; i++) 
				{
					slice_index++;
				}
				if (slice_index < matval.plane)
					free_matrix_data (volume);

				node = node->next;
			}
			if (slice_index == matval.plane)
			{
				matrix_data      = matrix_read_slice (fptr, volume, i - 1, 0);
				free_matrix_data (volume);
			}
		}
		else
		{
			matnum		= mat_numcod (matval.frame, matval.plane, 
						matval.gate, matval.data, matval.bed);
			matrix_data	= matrix_read (fptr, matnum, main_header64.data_type);
		}
		if (matrix_data == NULL)
			return FAIL;

		image_subheader	= (Image_subheader *) matrix_data->shptr;
		ih64_convert (&image_subheader64, image_subheader, main_header);

		raw_slice_data	= (short *) matrix_data->data_ptr;

		/*
	 	 * Get the information in comment if there is any.
	 	 */
		comment_info (main_header64.study_description, &comment_info_data);

		/*
	 	 * Fill the PETT6HDR header with the necessary information.
	 	 */
		FillHeaderPett6 (pett6_header);

		pett6_header->scan_time	= image_subheader64.frame_duration 
						/ MILISECONDS_PER_SECOND,

		itoa (main_header64.scan_start_day, day);
		itoa (main_header64.scan_start_month, month);
		itoa (main_header64.scan_start_year, year);

		/*
		 * Find the date. Check for single or double digit.
		 */
		if (strlen (month) == 1)
		{
			pett6_header->date[0]	= '0';
			pett6_header->date[1]	= month[0];
		}
		else
		{
			pett6_header->date[0]	= month[0];
			pett6_header->date[1]	= month[1];
		}

		if (strlen (day) == 1)
		{
			pett6_header->date[2]	= '0';
			pett6_header->date[3]	= day[0];
		}
		else
		{
			pett6_header->date[2]	= day[0];
			pett6_header->date[3]	= day[1];
		}

		if (strlen (year) == 2)
		{
			pett6_header->date[4]	= year[0];
			pett6_header->date[5]	= year[1];
		}
		else
		{
			pett6_header->date[4]	= year[2];
			pett6_header->date[5]	= year[3];
		}

		/*
		 * Convert decay constant to PE representation.
		 */
		decay	= log (2.0) / main_header64.isotope_halflife;
		pett6_header->decay_constant	= ieee2pe (&decay); 

		pett6_header->slice_number	= matval.plane;

		/*
		 * Get the compound name.
		 */
		for (i = 0; i < PETT6_COMPOUND_LEN; i++)
			pett6_header->compound [i]	
				= main_header64.radiopharmaceutical [i];

		pett6_header->number_of_slices	= num_slices;
		pett6_header->file_type		= ECAT_FILE_TYPE;
		if (image_subheader64.filter_code < GAUSSIAN_FILTER_CODE - 1)
			filter_name_index	= image_subheader64.filter_code;
		else if (image_subheader64.filter_code >= GAUSSIAN_FILTER_CODE - 1 &&
			image_subheader64.filter_code < BUTTERWORTH_FILTER_CODE - 1)
			filter_name_index	= GAUSS_INDEX;
		else 
			filter_name_index	= BUTTERWORTH_INDEX;
		if (filter_name_index < 0)
		{
			filter_name_index	= RAMP_INDEX;
		}
		strcpy (pett6_header->filter, filterTable [filter_name_index]);	
		sprintf (filter_params, "%5f", image_subheader64.filter_params [0]);

		/*
		 * Filter name.
		 */
		for (i = 0; i < FILTER_PARAMS_LEN; i++)
			pett6_header->filter[FILTER_NAME_LEN + i]	
				= filter_params [i]; 

		/*
		 * Factor multiplied to ecah pixel value.
		 */
		if (image_subheader64.frame_duration != 0)
			frame_duration	= image_subheader64.frame_duration;
		else	
			frame_duration	= DEFAULT_FRAME_DURATION;
		if (pkg_abs (image_subheader64.ecat_calibration_fctr) >= ERROR_BOUND)
			ecat_calibration_fctr	= image_subheader64.ecat_calibration_fctr;
		else	
			ecat_calibration_fctr	= DEFAULT_CALIBRATION_FACTOR;
		if (main_header->system_type == SYSTEM_TYPE_961)
			quant_factor	= image_subheader64.quant_scale * ecat_calibration_fctr
					* scale_factor;
		else
			quant_factor	= image_subheader64.quant_scale * ecat_calibration_fctr
					* frame_duration * scale_factor;

		dimension	= image_subheader64.dimension_1 * image_subheader64.dimension_2;

		if (!(comment_info_data.xflip_flag || comment_info_data.pett6_flag))
		{
			reflection (image_subheader64.dimension_1, 
			image_subheader64.dimension_2, raw_slice_data);
		}
		pett6_header->ecat_orientation	= 1;

		for (i = 0; i < dimension; i++)
		{
			slice_data [i]	= ROUND (raw_slice_data [i] * quant_factor);
		}

		free_matrix_data (matrix_data);
	}

	return SUCCEED;
}
