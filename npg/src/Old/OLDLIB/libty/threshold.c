/*$Id: threshold.c,v 1.3 1995/11/10 16:29:29 tom Exp $*/
/*$Log: threshold.c,v $
 * Revision 1.3  1995/11/10  16:29:29  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.2  1995/10/13  17:53:21  ty7777
 * Use matrix7.h.
 *
 * Revision 1.1  1994/03/11  22:57:20  ty7777
 * Initial revision
 **/

/*______________________________________________________________________________
File Name:	threshold.c

Function:	PUBLIC int threshold (char *mskimg_filtered, int scanner, int mskpct, 
		short width, short height, short num_slices, int first_slice,
		int last_slice, short *mask3d, long *total_pixel)

Arguments:	mskimg_filtered:	filtered mask image file.
		scanner:		scanner type.
		mskpct:			mask percentage.
		width:			width of pet image slice.
		height:			height of pet image slice.
		num_slices:		number of slices.
		first_slice:		first slice to mask.
		last_slice:		last slice to mask.
		mask3d:			3d array for masked image.
		total_pixel:		total pixels in mask.

Return:		SUCCEED = 0, successful.
		FAIL = -1, failed.

Description:	This function creats an thresholded mask image with mskpct% of the
		whole brain maximum.

Algorithm:	a) Find the maximum of the whole brain.

		b) Get the threshold value which is mskpct% of the whole brain maximum.

		c) Create a template with mskpct% of the whole brain maximum.

			for (each slice) do
				Assign 1 to all points >= threshold value and 
				assign 0 to all points < threshold value. 

				Assign 2 to all points along the boundaries of
				the array on which the pixel values are not 1. 

				Starting with the boundaries, assign 2 to all points
				directly adjacent to a point whose value is 2 and which 
				are currently 0.

				All points that are < 2 are assigned 1 and all points 
				that are >= 2 are assigned 0. Compute the total number
				of pixels which are assigned to 1.

				Save templated image in a 3D short integer array.
			end do

Called by:	

Calling:			

Creator:	Tom Yang

History:	Created by Tom Yang on 07/28/1993

______________________________________________________________________________*/

#include <imglist.h>
#include <matrix7.h>
#include <values.h>

#define INSIDE		1	
#define OUTSIDE		2	

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/threshold.c,v 1.3 1995/11/10 16:29:29 tom Exp $";

PUBLIC int threshold (mskimg_filtered, scanner, mskpct, width, height, num_slices, 
	first_slice, last_slice, mask3d, total_pixel)
	char 	*mskimg_filtered;
	int	scanner;
	int 	mskpct;
	short	width;
	short	height;
	short	num_slices;
	int	first_slice;
	int	last_slice;
	short	*mask3d;
	long	*total_pixel;
{
	Image_subheader	**subheader_list;
	BOOLEAN		more;
	Main_header	*main_header;
	float		*mask_image2d;
	float		*mask_image3d;
	float		threshold;
	float		whole_brain_max;
	int		i;
	int		j;
	int		k;
	int		slice_index;
	long		image_dimension;
	long		slice_dimension;
	short		*mask2d;
	struct Matval	matval;

	/*______________________________________________________
	 * Initialization for matval.
	 *______________________________________________________*/
	matval.frame	= 1;
	matval.plane	= 1;
	matval.gate	= 1;
	matval.data	= 0;
	matval.bed	= 0;

	/*
	 * Reads in filtered mask 3D array.
	 */
	main_header	= (Main_header *) calloc (1, sizeof (Main_header));
	subheader_list	= (Image_subheader **) pkg_malloc (num_slices * sizeof (caddr_t), 
				"threshold", "subheader_list");
	slice_dimension	= ((long) width) * height;
	image_dimension	= slice_dimension * num_slices;
	mask_image3d	= (float *) pkg_malloc (image_dimension * sizeof (float), 
				"threshold", "mask_image3d");
	if (main_header == (Main_header *) NULL)
	{
		fprintf (stderr, "Memory allocation error for main_header in threshold.c.\n"); 
		return FAIL;
	}

	if (getrealimg (mskimg_filtered, scanner, matval, width, height, num_slices, mask_image3d, 
		main_header, subheader_list) == FAIL)
	{
		fprintf (stderr, "Error in getrealimg ().\n");
		free (mask_image3d);
		return FAIL;
	}

	/*
	 * Find the maximum of the whole brain.
	 */
	whole_brain_max	= -MAXFLOAT;
	for (i = 0; i < image_dimension; i++)
	{
		if (mask_image3d [i] > whole_brain_max)
			whole_brain_max	= mask_image3d [i];
	}

	/*
	 * Get the threshold value.
	 */
	threshold	= (whole_brain_max * mskpct) / ONE_HUNDRED; 

	/*
	 * Create a template with mskpct% of the whole brain maximum.
	 */
	*total_pixel	= 0;
	for (k = first_slice - 1; k < last_slice; k++)
	{
		slice_index	= k * slice_dimension;
		mask2d		= mask3d + slice_index;
		mask_image2d	= mask_image3d + slice_index;

		/* 
		 * Assign INSIDE to all points >= threshold.
		 * Assign 0 to all points < threshold,
		 */
		for (i = 0; i < slice_dimension; i++)
		{
			if (mask_image2d [i] >= threshold)
				mask2d [i]	= INSIDE;
			else
				mask2d [i]	= 0;
		}

		/* 
		 * Assign OUTSIDE to all points along the boundaries of the 2d mask
		 * array which are nor INSIDE the template.
		 */
		for (i = 0; i < width; i++)
		{
			/*
			 * Top row.
			 */
			if (mask2d [i] != INSIDE) 
				mask2d [i]	= OUTSIDE;

			/*
			 * Bottom row.
			 */
			if (mask2d [(height - 1) * width + i] != INSIDE) 
				mask2d [(height - 1) * width + i]	= OUTSIDE;
		}

		for (j = 0; j < height; j++)
		{
			/*
			 * Left column.
			 */
			if (mask2d [j * width] != INSIDE)	
				mask2d [j * width]	= OUTSIDE;

			/*
			 * Right column.
			 */
			if (mask2d [j * width + width - 1] != INSIDE)	
				mask2d [j * width + width - 1]	= OUTSIDE;
		}

		/* 
		 * Now, starting with the boundaries of the array, expand the
		 * area excluded from the template by assigning OUTSIDE to all 
		 * points directly adjacent to a point whose value is OUTSIDE 
		 * and which are currently zero (neither INSIDE nor OUTSIDE).
		 */
		more	= FALSE;

		for (i = 1; i < width - 1; i++) /* Outside rows */
		{
			/*
			 * Row below the top row.
			 */
			if (mask2d [i] == OUTSIDE && mask2d [width + i] < INSIDE) 
			{
				mask2d [width + i]	= OUTSIDE;
				more			= TRUE;
			}

			/*
			 * Row above the bottom row.
			 */
			if (mask2d [(height - 1) * width + i] == OUTSIDE && 
			mask2d [(height - 2) * width + i] < INSIDE)
			{
				mask2d [(height - 2) * width + i]	= OUTSIDE;
				more					= TRUE;
			}
		}

		for (j = 1; j < height - 1; j++) /* Outside columns */
		{
			/*
			 * Column next to the left most column.
			 */
			if (mask2d [j * width] == OUTSIDE && mask2d [j * width + 1] < INSIDE)	
			{
				mask2d [j * width + 1]	= OUTSIDE;
				more			= TRUE;
			}

			/*
			 * Column next to the right most column.
			 */
			if (mask2d [j * width + width - 1] == INSIDE && 
			mask2d [j * width + width - 2 < INSIDE])	
			{
				mask2d [j * width + width - 2]	= OUTSIDE;
				more				= TRUE;
			}
		}

		/*
		 * The following loop is repeated until it cycles with no more points
		 * added to the outside of the region.
		 */
		while (more)
		{
			more	= FALSE;

			for (j = 1; j < height - 1; j++)
			{
				for (i = 1; i < width - 1; i++)
				{
					if (mask2d [j * width + i] == OUTSIDE)
					{
						if (mask2d [j * width + i - 1] < INSIDE)
						{
							mask2d [j * width + i - 1]	= OUTSIDE;
							more				= TRUE;
						}

						if (mask2d [j * width + i + 1] < INSIDE)
						{
							mask2d [j * width + i + 1]	= OUTSIDE;
							more				= TRUE;
						}

						if (mask2d [(j - 1) * width + i] < INSIDE)
						{
							mask2d [(j  - 1) * width + i]	= OUTSIDE;
							more				= TRUE;
						}

						if (mask2d [(j + 1) * width + i] < INSIDE)
						{
							mask2d [(j  + 1) * width + i]	= OUTSIDE;
							more				= TRUE;
						}
					}
				}
			}
		}

		/*
		 * All points that are < OUTSIDE are assigned INSIDE.
		 * All points that are >= OUTSIDE are assigned 0.
		 */
		for (i = 0; i < slice_dimension; i++)
		{
			if (mask2d [i] < OUTSIDE)
			{
				mask2d [i]	= INSIDE;
				(*total_pixel)++;
			}
			else
				mask2d [i]	= 0;
		}
	}

	for (i = 0; i < num_slices; i++)
	{
		free (subheader_list [i]);
	}
	free (subheader_list);
	free (main_header);
	free (mask_image3d);

	return SUCCEED;
}
