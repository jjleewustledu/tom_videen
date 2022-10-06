/*$Id: GetPetParameters.c,v 1.3 1996/08/01 15:46:35 yang Exp $*/
/*$Log: GetPetParameters.c,v $
 * Revision 1.3  1996/08/01  15:46:35  yang
 * Updated for pett6 images.
 *
 * Revision 1.2  1996/07/10  20:51:53  yang
 * Worked for both PetVolume and others.
 *
 * Revision 1.1  1996/07/10  15:08:17  yang
 * Initial revision
 **/

#ifndef lint
static char sccsid [] = "@(#)GetPetParameters.c	10/29/92  Copyright Neural Pet Group, Washington University 1992";
#endif

static char rcsid [] = "$Header: /home/petsun4/yang/src/libnpg/RCS/GetPetParameters.c,v 1.3 1996/08/01 15:46:35 yang Exp $";

/*___________________________________________________________________________
  File:		GetPetParameters.c

  Function:	int GetPetParameters (char petimage_name, char trim, int pettnum,
			BOOLEAN encoded, SXR_OUTPUT *sxr_output, llist petlist)

  Arguments:	petimage_name,	pet image file name.
		trim,		number of pixels to trim.
		pettnum,	pet number, possible values are ECAT953_SCANNER, 
				ECAT961_SCANNER, PETT6_SCANNER, etc..
		encoded,	TRUE: encoded; FALSE: decoded.
		sxr_output,	a SXR_OUTPUT structure, changed upon return.
		petlist,	linked list of PET_SLICE_DATA.

  Description:	This function obtains the parameters that are pertinent to
		the PET image.

  Called by:	xray.c and sxrweld.c.	

  Calling:	FindScanPeak () in this file.
		Template () in this file.

  History:	Created by Tom Yang on 10/29/92. 
___________________________________________________________________________*/

#include <math.h>
#include <values.h>
#include <petutil/petutil.h>
#include <petutil/matrix_64.h>

PRIVATE int FindScanPeak ();
PRIVATE int PetSliceCmp ();
PRIVATE void Thresh ();
PRIVATE int Template ();
PRIVATE void Trim ();

PUBLIC int GetPetParameters (petimage_name, trim, pettnum, encoded, sxr_output, petlist)
	char		*petimage_name;
	char		trim;		
	int		pettnum;	
	BOOLEAN		encoded;
	SXR_OUTPUT	*sxr_output;
	llist		petlist;
{
	BOOLEAN		ecat_flag;
	MatDirNode	*node;
	MatrixData	*volume;
	MatrixFile	*fptr;
	PET_SLICE_DATA	*slice;
	POINT2D		bottom;
	POINT2D		left;
	POINT2D		right;
	POINT2D		top;
	float		fpeak;
	float		maxpetdim;
	float		pixel_size;
	float		plane_separation;
	float		*slice_peaks;
	int		fd;
	int		i;
	int		index;
	int		matnum;
	int		peak_slice;
	int		rows;
	int		slice_count;
	int		x;
	int		y;
	short		height;
	short		peakvalue;
	short		*template;
	short		num_slices;
	short		width;
	struct Matval	matval;

	/* 
	 * Clear memory corresponding to list.
	 */
	llhead (petlist);			/* step through the list */
	while (llsize (petlist) > 0)
	{
		/* 
		 * Get a slice. 
		 */
		slice = (PET_SLICE_DATA *) llretrieve (petlist);

		free (slice->input);

		free (slice);
		lldelete (petlist);
	}

	/* 
	 * Find peaks of each slices and the whole scan.
	 */
	slice_peaks	= (float *) malloc (sxr_output->number_slices * sizeof (float));
	FindScanPeak (petimage_name, slice_peaks, &peak_slice);
	if (sxr_output->peakslice == PEAK_SLICE_UNDEFINED)
		sxr_output->peakslice	= peak_slice;
	peakvalue	= ROUND (slice_peaks [sxr_output->peakslice - 1]);
	fprintf (stdout, "Peak Value = %f\n", slice_peaks [sxr_output->peakslice - 1]);

	/* 
	 * Set comparison function for llinsert. 
	 */
	llcmp	= PetSliceCmp;

	/* 
	 * Build the pet list. 
	 */
	maxpetdim	= 0.0;

	fd	= open (petimage_name, O_RDONLY, 0);
	if (fd == -1)
		fprintf (stderr, "Can't open %s\n", petimage_name); 

	ecat_flag	= FALSE;
	if (pettnum == ECAT953_SCANNER || pettnum == ECAT961_SCANNER)
	{
		matval.frame	= 1;
		matval.plane	= 1;
		matval.gate	= 1;
		matval.data	= 0;
		matval.bed	= 0;

		fptr	= OpenEcat (petimage_name, &num_slices, &width, &height, 
				&pixel_size, &plane_separation, matval);
		if (fptr == (MatrixFile *) NULL)
		{
			close (fd);
			return FAIL;
		}

		ecat_flag	= TRUE;
		node	= fptr->dirlist->first;
		if (fptr->mhptr->file_type == PetVolume && node)
		{
			volume		= matrix_read (fptr, node->matnum, MAT_SUB_HEADER);
		}
	}

	slice_count	= 0;
	for (i = 0; i < sxr_output->number_slices && (!ecat_flag || ecat_flag && node); i++)
	{
		template	= (short *) calloc (ENCODED_SLICE_DIM, sizeof (short));
		if (template == (short *) NULL)
		{
			fprintf (stderr, "Error: can't allocate memory for *template.\n"); 
			return FAIL;
		}

		if (ecat_flag)
			matnum	= node->matnum;
		else
			matnum	= 0;
		Template (template, petimage_name, fd, fptr, matnum, sxr_output->threshold, trim, 
			i + 1, slice_count, &peakvalue, sxr_output->peaktype, sxr_output->number_slices, pettnum, encoded);

		/*
		 * Compute pet related parameters.
		 */
		left.x		= ENCODED_SLICE_WIDTH - 1;
		left.y		= 0;
		right.x		= 0;
		right.y		= 0;
		top.x		= 0;
		top.y		= ENCODED_SLICE_HEIGHT - 1;
		bottom.x	= 0;
		bottom.y	= 0;

		for (y = 0; y < ENCODED_SLICE_HEIGHT; y++) 
		{
			rows	= y * ENCODED_SLICE_WIDTH;
			for (x = 0; x < ENCODED_SLICE_WIDTH; x++) 
			{
				if (template [rows + x] == 1)
				{
					if (x < left.x)
					{
						left.x	= x;
						left.y	= y;
					}

					if (x > right.x)
					{
						right.x	= x;
						right.y	= y;
					}

					if (y < top.y)
					{
						top.x	= x;
						top.y	= y;
					}

					if (y > bottom.y)
					{
						bottom.x	= x;
						bottom.y	= y;
					}
				}
			}
		}

		if (left.x > right.x || top.y > bottom.y)
		{
			if (left.x > right.x)
			{
				sxr_output->rlpetdim [i]	= 0.0;
				sxr_output->rlcenter [i]	= (ENCODED_SLICE_WIDTH + 1) / 2.0;
			}
			else
			{
				sxr_output->rlpetdim [i]	= (float) (right.x - left.x);
				sxr_output->rlcenter [i]	= (left.x + 1) + (right.x - left.x) / 2.0;
			}

			if (top.y > bottom.y)
				sxr_output->vertical_petdim [i]	= 0.0;
			else
				sxr_output->vertical_petdim [i]	= (float) (bottom.y - top.y);

			fprintf (stderr, "Warning: Slice %d has 0 width or height\n", i + 1);
		}
		else
		{
			sxr_output->rlpetdim [i]	= (float) (right.x - left.x);
			sxr_output->rlcenter [i]	= ((left.x + 1)  + (right.x + 1)) / 2.0;
			sxr_output->vertical_petdim [i]	= (float) (bottom.y - top.y);
		}


		if (sxr_output->rlpetdim [i] > maxpetdim)
		{
			maxpetdim	= sxr_output->rlpetdim [i];
		}

		/* 
		 * Allocate memory for a slice. 
		 */
		slice	= (PET_SLICE_DATA *) pkg_malloc (sizeof (PET_SLICE_DATA), 
						"GetPetParameters", "slice");

		slice->slice_index	= i + 1;
		slice->xstart		= left.x;
		slice->ystart		= top.y; 
		slice->width		= right.x - left.x + 1; 
		slice->height		= bottom.y - top.y + 1; 
		slice->bottom.x		= bottom.x;
		slice->bottom.y		= bottom.y;
		slice->left.x		= left.x;
		slice->left.y		= left.y;
		slice->right.x		= right.x;
		slice->right.y		= right.y;
		slice->top.x		= top.x;
		slice->top.y		= top.y;
		slice->slice_peak	= slice_peaks [i];

		slice->input	= (short *) pkg_malloc (slice->width 
				* slice->height * sizeof (short), 
				"GetPetParameters", "slice->input");

		index	= 0;
		for (y = top.y; y <= bottom.y; y++)
		{
			rows	= y * ENCODED_SLICE_WIDTH;
			for (x = left.x; x <= right.x; x++)
			{
				slice->input [index++]	= template [rows + x];
			}
		}
		free (template);

		llinsert (petlist, slice);
		if (ecat_flag && fptr->mhptr->file_type == PetVolume)
			slice_count++;
		if (ecat_flag && (fptr->mhptr->file_type != PetVolume
		|| (fptr->mhptr->file_type == PetVolume && slice_count == volume->zdim)))
		{
			if (fptr->mhptr->file_type == PetVolume)
			{
				free_matrix_data (volume);
			}
			node	= node->next;
			if (fptr->mhptr->file_type == PetVolume && node)
			{
				slice_count	= 0;
				volume		= matrix_read (fptr, 
						node->matnum, MAT_SUB_HEADER);
			}
		}
	}

	free (slice_peaks);

	if (pettnum == ECAT953_SCANNER || pettnum == ECAT961_SCANNER)
	{
		matrix_close (fptr);
	}

	close (fd);

	llhead (petlist);
	for (i = 1; i < sxr_output->reference_slice; i++)
	{
		llnext (petlist);
	}
	slice = llretrieve (petlist);
	sxr_output->apcenter		= (slice->bottom.y + 1)
					+ (slice->top.y - slice->bottom.y) / 2.0;
	sxr_output->ear_separation	= maxpetdim * sxr_output->pixel_size + EARSEP_OFFSET;

	return SUCCEED;
}


/*__________________________________________________________________________	
	Function:
		PetSliceCmp	
	Arguments:
		s1: pet slice structure.
		s2: pet slice structure.
	Description:
		Function for llcmp assignment which compares two pet slices
		by their slice number. Returns:

			-1: is slice number of s1 < slice number of s2
			 0: is slice number of s1 = slice number of s2
			 1: is slice number of s1 > slice number of s2
__________________________________________________________________________*/

PRIVATE int PetSliceCmp (s1, s2)
PET_SLICE_DATA	*s1;
PET_SLICE_DATA	*s2;
{
	if (s1->slice_index < s2->slice_index)
		return (-1);
	if (s1->slice_index > s2->slice_index)
		return (1);
	return (0);
}

/*___________________________________________________________________________
  Function:	void FindScanPeak ()

  Arguments:	

  Return Value:

  Description:	This function finds the peak of a scan if necessary.

  Calling:	Library functions getimage () from libpetutil.a.

  Called by:	GetPetParameters () in this file.	

  History:	Created by Tom Yang on 10/29/92. 
___________________________________________________________________________*/
PRIVATE int FindScanPeak (image_file, slice_peaks, peak_slice)
	char		*image_file;
	float		*slice_peaks;
	int		*peak_slice;
{
	Image_subheader	**subheader_list;
	Main_header	*main_header;
	float		*image3d;
	float		peak_value;
	float		pixel_size;
	float		plane_separation;
	float		*slice_data;
	float		tmp_slice_peak;
	int		scanner;
	long		dimension;
	long		i;
	long		j;
	long		slice_dimension;
	short		width;
	short		height;
	short		num_slices;
	struct Matval	matval;

	matval.frame	= 1;
	matval.plane	= 1;
	matval.gate	= 1;
	matval.data	= 0;
	matval.bed	= 0;

	if (getimagedim (image_file, matval, &scanner, &width, &height, &num_slices, 
	&pixel_size, &pixel_size, &plane_separation) == FAIL) 
	{
		fprintf (stderr, "Error: image file %s cannot be found\n", image_file);
		return FAIL;
	}

	subheader_list	= (Image_subheader **) pkg_malloc (num_slices * sizeof (caddr_t), 
			"FindScanPeak", "subheader_list");

	slice_dimension	= width * ((long) height);
	dimension	= slice_dimension * num_slices;
	image3d		= (float *) pkg_malloc (dimension * sizeof (float), 
			"FindScanPeak", "image3d");
	main_header	= (Main_header *) calloc (1, sizeof (Main_header));

	if (getrealimg (image_file, scanner, matval, width, height, num_slices, image3d, 
		main_header, subheader_list) == FAIL)
	{
		fprintf (stderr, "Error in getrealimg ().\n");
		free (main_header);
		free (image3d);
		return FAIL;
	}

	peak_value	= 0.0;
	*peak_slice	= 1;
	for (i = 0; i < num_slices; i++)
	{
		tmp_slice_peak	= 0.0;
		slice_data	= image3d + i * slice_dimension;
		for (j = 0; j < slice_dimension; j++)
		{
			if (slice_data [j] > tmp_slice_peak)
				tmp_slice_peak	= slice_data [j];
		}

		slice_peaks [i]	= tmp_slice_peak;

		if (tmp_slice_peak > peak_value)
		{
			peak_value	= tmp_slice_peak;
			*peak_slice	= i + 1;
		}
	}

	for (i = 0; i < num_slices; i++)
	{
		free (subheader_list [i]);
	}
	free (subheader_list);
	free (main_header);
	free (image3d);

	return SUCCEED;
}

/*___________________________________________________________________________
  Function:	void Template ()

  Arguments:	

  Return Value:

  Description:	This function applying threshold to the template image
		and trim the templated image if necessary.

  Calling:	Thresh () in this file.	

  Called by:	GetPetParameters () in this file.	

  History:	Created by Tom Yang on 10/29/92. 
___________________________________________________________________________*/
PRIVATE int Template (template, petimage_name, imgfd, fptr, matnum, threshold, trim, 
	slice_index, slice_count_in_volume, peakvalue, peaktype, num_slices, 
	pettnum, encoded)
	short		*template;
	char		*petimage_name;
	int		imgfd;
	MatrixFile	*fptr;
	int		matnum;
	char		threshold;
	char		trim;
	short		slice_index;
	short		*peakvalue;
	char		peaktype;		
	short		num_slices;
	int		pettnum;	
	BOOLEAN		encoded;
{
	MatrixData		*matrix_read ();

	BOOLEAN			err_value;
	COMMENT_INFO		comment_info_data;
	Image_subheader		*image_subheader;
	Image_subheader_64	image_subheader64;
	Main_header_64		main_header64;
	MatrixData		*matrix_data;
	MatrixData		*volume;
	float			ecat_calibration_fctr;
	float			scale;
	int			frame_duration;
	long			dimension;
	long			i;
	short			*actual_slice_data;
	short			cutoff;
	short			*header;
	short			*raw_slice_data;

	/* 
	 * Allocate header. 
	 */
	header = (short *) calloc (HEADER_SIZE / sizeof (short), sizeof (short));
	if (header == (short *) NULL)
	{
		pkg_message (PKG_ERROR, "xray", "Template", "calloc",
					"Error: Can't allocate header.\n");
	}

	actual_slice_data	= (short *) calloc (ENCODED_SLICE_DIM, sizeof (short));
	if (actual_slice_data == (short *) NULL)
	{
		pkg_message (PKG_ERROR, "xray", "Template", "calloc",
					"Error: Can't allocate actual_slice_data.\n");
	}

	if (pettnum != ECAT953_SCANNER && pettnum != ECAT961_SCANNER)
	{
		getimage (actual_slice_data, header, imgfd, petimage_name, slice_index, 
				encoded, num_slices, pettnum, &err_value);
	}
	else
	{
       		/* 
		 * Read the matrix data. 
		 */
		mh64_convert (&main_header64, fptr->mhptr);
		if (fptr->mhptr->file_type != PetVolume)
		{
			matrix_data	= matrix_read (fptr, matnum, 
						main_header64.data_type);
		}
		else
		{
			volume		= matrix_read (fptr, matnum, MAT_SUB_HEADER);
			matrix_data	= matrix_read_slice (fptr, volume, 
					slice_count_in_volume, 0);
			free_matrix_data (volume);
		}

		if (!matrix_data) 
		{
			fprintf (stderr, "filter: matrix_data reading error.\n");
			return FAIL;
		}

		image_subheader	= (Image_subheader *) matrix_data->shptr;
		ih64_convert (&image_subheader64, image_subheader, fptr->mhptr);
		raw_slice_data	= (short *) matrix_data->data_ptr;

		/*
		 * Factor multiplied to ecah pixel value.
		 */
		if (pkg_abs (image_subheader64.ecat_calibration_fctr) >= ERROR_BOUND)
			ecat_calibration_fctr	= image_subheader64.ecat_calibration_fctr;
		else	
			ecat_calibration_fctr	= DEFAULT_CALIBRATION_FACTOR;
		if (image_subheader64.frame_duration != 0)
			frame_duration	= image_subheader64.frame_duration;
		else	
			frame_duration	= DEFAULT_FRAME_DURATION;
		if (fptr->mhptr->system_type == SYSTEM_TYPE_961)
			scale	= image_subheader64.quant_scale * ecat_calibration_fctr;
		else
			scale	= image_subheader64.quant_scale * frame_duration * ecat_calibration_fctr;

		dimension	= image_subheader64.dimension_1 * image_subheader64.dimension_2;

		for (i = 0; i < dimension; i++)
		{
			actual_slice_data [i] = ROUND (raw_slice_data [i] * scale);
		}

		/*
	 	 * Get the information in comment if there is any.
	 	 */
		comment_info (fptr->mhptr->study_description, &comment_info_data);

		if (!(comment_info_data.xflip_flag))
		{
			reflection (image_subheader64.dimension_1, 
			image_subheader64.dimension_2, actual_slice_data);
		}

		free_matrix_data (matrix_data);
	}

	/*
	 * Find peak value of slice if needed.
	 */
	if (peaktype == PEAK_PER_SLICE)
	{
		*peakvalue	= 0;	
		for (i = 0; i < ENCODED_SLICE_DIM; i++)
		{
			if (actual_slice_data [i] > *peakvalue)	
			{
				*peakvalue	= actual_slice_data [i];
			}
		}
	}

	/*
	 * Calculate cutoff.
	 */
	cutoff	= ROUND ((PERCENT_FACTOR * threshold) * *peakvalue);

	/*
	 * Call Thresh.
	 */
	Thresh (actual_slice_data, cutoff);

	/*
	 * Call Trim.
	 */
	Trim (actual_slice_data, trim);

	for (i = 0; i < ENCODED_SLICE_DIM; i++)
	{
		if (actual_slice_data [i] == -MAXSHORT)
			template [i]	= 0;
		else
			template [i]	= 1;
	}

	free (actual_slice_data);

	return SUCCEED;
}

/*___________________________________________________________________________
  Function:	void Thresh ()

  Arguments:	

  Return Value:

  Description:	This function assigns -MAXSHORT to all pixels out of %-of-peak 
		bound for a slice.

  Calling:	

  Called by:	Template () in this file.	

  History:	Created by Tom Yang on 11/02/92. 
___________________________________________________________________________*/
PRIVATE void Thresh (slice_data, cutoff)
	short	*slice_data;
	short	cutoff;
{
	int	i;
	int	j;
	int	k;
	long	rows;

	for (j = 0; j < ENCODED_SLICE_HEIGHT; j++)
	{
		rows	= j * ENCODED_SLICE_WIDTH;
		for (i = 0; i < ENCODED_SLICE_WIDTH; i++)
		{
			if (slice_data [rows + i] < cutoff)
				slice_data [rows + i]	= -MAXSHORT;
			else
				break;
		}
		k	= i;

		if (k < ENCODED_SLICE_WIDTH)
		{
			for (i = ENCODED_SLICE_WIDTH - 1; i >= k; i--)
			{
				if (slice_data [rows + i] < cutoff)
					slice_data [rows + i]	= -MAXSHORT;
				else
					break;
			}
		}
	}
}

/*___________________________________________________________________________
  Function:	void Trim ()

  Arguments:	

  Return Value:

  Description:	This function trims a passed number of pixels from a thresholded 
		array.

  Calling:	None.	

  Called by:	Template () in this file.	

  History:	Created by Tom Yang on 11/02/92. 
___________________________________________________________________________*/
PRIVATE void Trim (slice_data, trim)
	short	*slice_data;
	char	trim;
{
	int	i;
	int	j;
	int	k;
	long	rows;

	if (trim <= 0)
		return;

	for (k = 1; k <= trim; k++)
	{
		for (j = 0; j < ENCODED_SLICE_HEIGHT; j++)
		{
			rows	= j * ENCODED_SLICE_WIDTH;
			for (i = 0; i < ENCODED_SLICE_WIDTH; i++)
			{
				if (slice_data [rows + i] != -MAXSHORT)
				{
					if ((i - 1 >= 0 && slice_data [rows + i - 1] == -MAXSHORT) 
					|| (i + 1 < ENCODED_SLICE_WIDTH 
					&& slice_data [rows + i + 1] == -MAXSHORT)
					|| (j - 1 >= 0
					&& slice_data [(j - 1) * ENCODED_SLICE_WIDTH + i] == -MAXSHORT)
					|| (j + 1 < ENCODED_SLICE_WIDTH
					&& slice_data [(j + 1) * ENCODED_SLICE_WIDTH + i] == -MAXSHORT))
						slice_data [rows + i]	= -MAXSHORT + 1;
				}
			}
		}
	}

	for (j = 0; j < ENCODED_SLICE_HEIGHT; j++)
	{
		rows	= j * ENCODED_SLICE_WIDTH;
		for (i = 0; i < ENCODED_SLICE_WIDTH; i++)
		{
			if (slice_data [rows + i] == -MAXSHORT + 1)
			{
				slice_data [rows + i]	= -MAXSHORT;
			}
		}
	}
}
