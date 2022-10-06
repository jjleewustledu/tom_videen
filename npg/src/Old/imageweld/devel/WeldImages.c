/*$Id*/

/*_________________________________________________________________
  File:		WeldImages.c

  Function:	int WeldImages (char *top_scan, char *bottom_scan, 
		char *whole_scan, float topT4 [4][4], float bottomT4 [4][4],
		float top_scaling_factor, float bottom_scaling_factor, 
		float normalization_factor)

  Arguments:	top_scan,		the top scan image name.
		bottom_scan,		the bottom scan image name.
		whole_scan,		the image file name for the welded image.
		topT4,			the T4 matrix for the top scan. It contains all
					the parameters for rotation and translation.
		bottomT4,		the T4 matrix for the bottom scan. It contains 
					all the parameters for rotation and translation.
		top_scaling_factor:	scaling factor for the top scan.
		bottom_scaling_factor:	scaling factor for the bottom scan.
		normalization_factor:	normalization factor applied to the bottom scan
					to make it compatible with the top scan.

  Return:	SUCCEED: successful; FAIL: error.

  Calling:	GetIndexingScanOffset () in GetIndexingScanOffset.c	
		GetSamplingMode () in GetSamplingMode.c	
  
		Library functions in libhp.a include:
		getimagedim ()
		getrealimg ()
		pkg_malloc ()
		writerealimg ()

		Library functions in libcti.a include:
		matrixx_init_image_subheader ();

  Called by:	the main program in imageweld.c.

  Description:	This program reads two ECAT images which represent the
		top and bottom scans of the brain with some overlapped
		slices. It then construct an ECAT image for the whole
		brain. The overlapped slices is constructed in such a
		way that the noise is minimum.

  History:	Created by Tom Yang on 05/05/93. 
_________________________________________________________________*/

#include "imageweld.h"
#include <petutil/matrix.h>
#include <petutil/standard_deviations.h>

static char rcsid []= "$Id: WeldImages.c,v 1.5 1993/10/20 15:52:46 ty7777 Exp $";

PUBLIC int WeldImages (top_scan, bottom_scan, whole_scan, topT4, bottomT4,
	top_scaling_factor, bottom_scaling_factor, normalization_factor)
	char		*top_scan;
	char		*bottom_scan;
	char		*whole_scan;
	float		topT4 [4][4];
	float		bottomT4 [4][4];
	float		top_scaling_factor;
	float		bottom_scaling_factor;
	float		normalization_factor;
{
	extern		imgval_ ();

	BOOLEAN		top_bottom_flipped;
	Image_subheader	**bottom_subheader_list;
	Image_subheader	**subheader_list;
	Image_subheader	**top_subheader_list;
	Main_header	*bottom_main_header;
	Main_header	*main_header;
	Main_header	*top_main_header;
	float		*bottom_image3d;
	float		bottom_value;
	float		bottom_weight;
	float		*image3d;
	float		offset;
	float		scaling_factor;
	float		*top_image3d;
	float		top_weight;
	float		pixel_size;
	float		plane_separation;
	float		*sigma3d	= standard_deviations3d;
	float		top_value;
	float		*weight_table;
	int		bottom_lslice;
	int		dimension;
	int		i;
	int		j;
	int		k;
	int		kk;
	int		num_overlapped_slices;
	int		num_shifted_slices;
	int		nslice;
	int		nx;
	int		ny;
	int		sampling_mode;
	int		scanner;
	int		slice_dim;
	int		top_lslice;
	int		top_scan_overlap_start;
	long		current_rows;
	long		current_slices;
	short		bottom_num_slices;
	short		height;
	short		num_slices;
	short		top_num_slices;
	short		width;
	struct Matval	matval;

	/*______________________________________________________
	 * Assign default values for matval.
	 *______________________________________________________*/
	matval.frame	= 1;
	matval.plane	= 1;
	matval.gate	= 1;
	matval.data	= 0;
	matval.bed	= 0;

	/*______________________________________________________
	 * Get image type and do error checking.
	 *______________________________________________________*/
	if (getimagedim (top_scan, matval, &scanner, &width, &height, &top_num_slices, 
		&pixel_size, &pixel_size, &plane_separation) == FAIL) 
	{
		fprintf (stderr, "Error: getimagedim () failed.\n");
		return FAIL;
	}

	if (scanner != ECAT_SCANNER)
	{
		fprintf (stderr, "Error: top scan file %s is not an ECAT image.\n", top_scan);
		return FAIL;
	}

	/*______________________________________________________
	 * Determine the sampling mode.
	 *______________________________________________________*/
	if (GetSamplingMode (top_scan, &sampling_mode) == FAIL)
	{
		fprintf (stderr, "Error: in GetSamplingMode for file %s\n", top_scan);
		return FAIL;
	}
	else
		fprintf (stdout, "\nProcessing in %dD mode\n", sampling_mode);

	/*______________________________________________________
	 * Get the offset for the indexing scan.
	 *______________________________________________________*/
	if (GetIndexingScanOffset (top_scan, bottom_scan, &offset, 
	&top_bottom_flipped, TRUE) == FAIL)
	{
		fprintf (stderr, "Error: in GetIndexingScanOffset ().\n");
		return FAIL;
	}

	if (top_bottom_flipped)
	{
		scaling_factor		= top_scaling_factor;
		top_scaling_factor	= bottom_scaling_factor; 
		bottom_scaling_factor	= scaling_factor;

		for (j = 0; j < 4; j++)
		{
			for (i = 0; i < 4; i++)
			{
				scaling_factor	= topT4 [i][j];
				topT4 [i][j]	= bottomT4 [i][j];
				bottomT4 [i][j]	= scaling_factor;
			}
		}
	}

	/*______________________________________________________
	 * Get the number of the overlapped slices.
	 *______________________________________________________*/
	num_shifted_slices	= ROUND (offset / (plane_separation * MMS_PER_CM));
	num_overlapped_slices	= top_num_slices - num_shifted_slices;

	if (offset - num_shifted_slices * plane_separation * MMS_PER_CM != 0.0)
	{
		fprintf (stdout, "Image shift differs from exact slice alignment by %.3f mm\n",
			num_shifted_slices * plane_separation * MMS_PER_CM - offset);
	}
	fprintf (stdout, "Number of overlaped slices = %d\n\n\n", num_overlapped_slices);

	if (num_overlapped_slices > top_num_slices)
	{
		fprintf (stderr, "Error: number of overlapped slices %d is greater than %d.\n", 
			num_overlapped_slices, top_num_slices);
		return FAIL;
	}

	/*______________________________________________________
	 * Get data from the top scan.
	 *______________________________________________________*/
	top_subheader_list	= (Image_subheader **) pkg_malloc (top_num_slices * sizeof (caddr_t), 
					"WeldImages", "top_subheader_list");

	slice_dim	= width * height;
	dimension	= slice_dim * top_num_slices;
	top_image3d	= (float *) pkg_malloc (dimension * sizeof (float), "WeldedImages", "top_image3d");
	top_main_header	= (Main_header *) calloc (1, sizeof (Main_header));

	if (getrealimg (top_scan, scanner, matval, width, height, top_num_slices, top_image3d, 
		top_main_header, top_subheader_list) == FAIL)
	{
		fprintf (stderr, "Error: getrealimg () failed.\n");
		free (top_subheader_list);
		free (top_main_header);
		free (top_image3d);
		return FAIL;
	}

	top_scan_overlap_start	= top_num_slices - num_overlapped_slices + 1;

	/*______________________________________________________
	 * Get data from the bottom scan.
	 *______________________________________________________*/
	if (getimagedim (bottom_scan, matval, &scanner, &width, &height, &bottom_num_slices, 
		&pixel_size, &pixel_size, &plane_separation) == FAIL) 
	{
		fprintf (stderr, "Error: getimagedim () failed.\n");
		return FAIL;
	}

	if (scanner != ECAT_SCANNER)
	{
		fprintf (stderr, "Error: bottom scan file %s is not an ECAT image.\n", bottom_scan);
		return FAIL;
	}

	if (num_overlapped_slices > bottom_num_slices)
	{
		fprintf (stderr, "Error: number of overlapped slices %d is greater than %d.\n", 
			num_overlapped_slices, bottom_num_slices);
		return FAIL;
	}

	bottom_subheader_list	= (Image_subheader **) pkg_malloc (bottom_num_slices * sizeof (caddr_t), 
					"WeldImages", "bottom_subheader_list");

	dimension		= slice_dim * bottom_num_slices;
	bottom_image3d		= (float *) pkg_malloc (dimension * sizeof (float), 
						"WeldedImages", "bottom_image3d");
	bottom_main_header	= (Main_header *) calloc (1, sizeof (Main_header));

	if (getrealimg (bottom_scan, scanner, matval, width, height, bottom_num_slices, bottom_image3d, 
		bottom_main_header, bottom_subheader_list) == FAIL)
	{
		fprintf (stderr, "Error: getrealimg () failed.\n");
		free (bottom_subheader_list);
		free (bottom_main_header);
		free (bottom_image3d);
		return FAIL;
	}

	/*______________________________________________________
	 * Generate merged 3D real array.
	 *______________________________________________________*/
	num_slices	= top_num_slices + bottom_num_slices - num_overlapped_slices;
	subheader_list	= (Image_subheader **) pkg_malloc (num_slices * sizeof (caddr_t), 
					"WeldImages", "subheader_list");
	main_header	= (Main_header *) calloc (1, sizeof (Main_header));
	bcopy (top_main_header, main_header, sizeof (Main_header));
	dimension	= slice_dim * num_slices;
	image3d		= (float *) pkg_malloc (dimension * sizeof (float), "WeldImages", "image3d");

	/* 
	 * Generate the top planes.
	 */
	nx		= width;
	ny		= height;
	nslice		= top_num_slices;

	for (k = 1; k <= top_scan_overlap_start; k++)
	{
		current_slices	= (k - 1) * slice_dim;
		for (j = 1; j <= height; j++)
		{
			current_rows	= current_slices + (j - 1) * width;
			for (i = 1; i <= width; i++)
			{
				imgval_ (topT4, top_image3d, &nx, &ny, &nslice,		
					&i, &j, &k, &top_value, &top_lslice);
				if (top_lslice == 0)
					top_value	= 0.0;
				image3d [current_rows + i - 1] = top_value * top_scaling_factor;
			}
		}

		/*
	 	 * Copy the sub headers.
	 	 */
		*(subheader_list + k - 1)	
		= matrixx_init_image_subheader (*(top_subheader_list + k - 1));
	}

	/*______________________________________________________
	 * Compute the weight table.
	 *______________________________________________________*/
	weight_table	= (float *) pkg_malloc (STANDARD_DEVIATIONS3D_SIZE * sizeof (float), 
				"WeldImages", "weight_table");
	if (sampling_mode == SAMPLING_MODE_3D)
	{
		for (i = 0; i < STANDARD_DEVIATIONS3D_SIZE; i++)
		{
			weight_table [i]	= (1.0 / (sigma3d [i] * sigma3d [i])
						+ 1.0 / (sigma3d [(STANDARD_DEVIATIONS3D_SIZE - 1) -  i]
						* sigma3d [(STANDARD_DEVIATIONS3D_SIZE - 1) - i])) / 2;
		}
	}
	else
	{
		for (i = 0; i < STANDARD_DEVIATIONS3D_SIZE; i++)
		{
			weight_table [i]	= weight2d [i];
		}
	}
	weight_table [0]				= 0.0;
	weight_table [STANDARD_DEVIATIONS3D_SIZE - 1]	= 0.0;

	/* 
	 * Compute the overlapped planes.
	 */
	for (k = top_scan_overlap_start + 1; k <= top_scan_overlap_start + num_overlapped_slices - 2; k++)
	{
		current_slices	= (k - 1) * slice_dim;
		kk		= 1 + k - top_scan_overlap_start;
		for (j = 1; j <= height; j++)
		{
			current_rows	= current_slices + (j - 1) * width;
			for (i = 1; i <= width; i++)
			{
				imgval_ (topT4, top_image3d, &nx, &ny, &nslice,		
					&i, &j, &k, &top_value, &top_lslice);

				if (top_lslice == 0)
					top_value	= 0.0;

				imgval_ (bottomT4, bottom_image3d, &nx, &ny, &nslice,		
					&i, &j, &kk, &bottom_value, &bottom_lslice);

				if (bottom_lslice == 0)
					bottom_value	= 0.0;

				top_lslice	-= 1;
				bottom_lslice	-= 1;

				if (weight_table [top_lslice] + weight_table [bottom_lslice] == 0.0)
				{
					image3d [current_rows + i - 1]	= 0.0; 
				}
				else
				{
					top_weight	= (weight_table [top_lslice] 
							* top_scaling_factor)
							/ (weight_table [top_lslice] 
							+ weight_table [bottom_lslice]);
					bottom_weight	= (weight_table [bottom_lslice] 
							* bottom_scaling_factor
							* normalization_factor)
							/ (weight_table [top_lslice] 
							+ weight_table [bottom_lslice]);
					image3d [current_rows + i - 1]	
					= top_weight * top_value + bottom_weight * bottom_value;
				}
			}
		}

		*(subheader_list + k - 1)	
		= matrixx_init_image_subheader (*(top_subheader_list + k - 1));
	}
	free (weight_table);

	for (i = 0; i < top_num_slices; i++)
	{
		free (top_subheader_list [i]);
	}
	free (top_subheader_list);
	free (top_main_header);
	free (top_image3d);

	/* 
	 * Generate the bottom planes.
	 */
	for (k = top_scan_overlap_start + num_overlapped_slices - 1; k <= num_slices; k++)
	{
		current_slices	= (k - 1) * slice_dim;
		kk		= 1 + k - top_scan_overlap_start;
		for (j = 1; j <= height; j++)
		{
			current_rows	= current_slices + (j - 1) * width;
			for (i = 1; i <= width; i++)
			{
				imgval_ (bottomT4, bottom_image3d, &nx, &ny, &nslice,		
					&i, &j, &kk, &bottom_value, &bottom_lslice);
				if (bottom_lslice == 0)
					bottom_value	= 0.0;
				image3d [current_rows + i - 1] 
				= bottom_value * bottom_scaling_factor * normalization_factor;
			}
		}
		
		*(subheader_list + k - 1)	
		= matrixx_init_image_subheader (*(bottom_subheader_list + kk - 1));
	}

	for (i = 0; i < bottom_num_slices; i++)
	{
		free (bottom_subheader_list [i]);
	}
	free (bottom_subheader_list);
	free (bottom_main_header);
	free (bottom_image3d);

	/*______________________________________________________
	 * Write 3D image to an ECAT matrix file.
	 *______________________________________________________*/
	strcpy (main_header->study_description, "Welded Image");
	strcpy (main_header->facility_name, "Washington Univ");
	if (writerealimg (whole_scan, matval, width, height, num_slices, image3d, 
			main_header, subheader_list) == FAIL)
	{
		fprintf (stderr, "Error: writerealimg () failed.\n");
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
