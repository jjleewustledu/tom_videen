/*$Id: readifimg.c,v 1.7 1995/10/30 22:39:01 tom Exp $*/
/*$Log: readifimg.c,v $
 * Revision 1.7  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.6  1995/06/01  19:15:27  ty7777
 * Restore to FACING_LEFT and FACING_RIGHT processing.
 **/

/*___________________________________________________________________________
 * File:		readifimg.c
 *
 * Function:	int readifimg (char *ifimgfile, int orientation, int slice_number,
 *		IFH ifhdr, long dimension, float *slice_data)
 *
 * Arguments:	ifimgfile,	interfile image, passed in.
 *		orientation,	orientation of the slice.
 *		slice_number,	slice number.
 *		ifhdr,		a structure defined in ifhdr.h, passed in.
 *		dimension,	dimension of slice, passed in.
 *		slice_data,	2D real array, filled upon return. The slice data is 
 *				always arranged as column major.
 *
 * Description:	This function reads a interfile image file and puts the information
 *		it contains in a real array.
 *
 * Return:	SUCCEED for successful; FAIL for error.
 *
 * Author:	Tom Yang, May 3, 1995.
 *
 * History:	Created by Tom Yang on 05/03/95. 
___________________________________________________________________________*/

#include <unistd.h>
#include <petutil.h>
#include <ifh.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/readifimg.c,v 1.7 1995/10/30 22:39:01 tom Exp $";

PRIVATE void data_conversion ();

PUBLIC int readifimg (ifimgfile, orientation, slice_number, dimension, ifhdr, slice_data) 
	char	*ifimgfile;
	int	orientation;
	int	slice_number;
	long	dimension;
	IFH	ifhdr;
	float	*slice_data;
{
	FILE		*fp;
	float		*current_data;
	float		*farray;
	float		fbuffer;
	int		i;
	int		j;
	long		index;
	long		plane_dim;
	void		*varray;

	/*
	 * Check the range of slice_number.
	 */
	if (orientation == TRANSVERSE && (slice_number < 1 
	|| slice_number > ifhdr.matrix_size [2]))
	{
		fprintf (stderr, "Error: your slice number is outside the allowed range [%d, %d]\n",
		1, ifhdr.matrix_size [2]);
	}

	if (orientation == CORONAL && (slice_number < 1 
	|| slice_number > ifhdr.matrix_size [1]))
	{
		fprintf (stderr, "Error: your slice number is outside the allowed range [%d, %d]\n",
		1, ifhdr.matrix_size [1]);
	}

	if ((orientation == FACING_LEFT || orientation == FACING_RIGHT) 
	&& (slice_number < 1 || slice_number > ifhdr.matrix_size [0]))
	{
		fprintf (stderr, "Error: your slice number is outside the allowed range [%d, %d]\n",
		1, ifhdr.matrix_size [1]);
	}

	plane_dim	= ((long) ifhdr.matrix_size [0]) * ifhdr.matrix_size [1];

	/*
	 * Open the image file.
	 */
	fp	= fopen (ifimgfile, "r");
	if (fp == (FILE *) NULL)
	{
		fprintf (stderr, "Error in opening interfile %s\n", ifimgfile);
		return FAIL;
	}

	/*
	 * Read the image and fill in the "slice_data" array.
	 */
	if (strcmp (ifhdr.matrix_initial_element [0], "left") == 0
	&& strcmp (ifhdr.matrix_initial_element [1], "anterior") == 0
	&& strcmp (ifhdr.matrix_initial_element [2], "superior") == 0)
	{
		if (orientation == TRANSVERSE)
		{
			/*
			 * Go to the right position.
			 */
			index	= (slice_number - 1) * plane_dim * ifhdr.number_of_bytes_per_pixel;
			fseek (fp, index, SEEK_SET);

			/*
			 * Read the slice data for slice_index = slice_number.
			 */
			varray	= (void *) calloc (plane_dim, ifhdr.number_of_bytes_per_pixel);
			i	= fread (varray, ifhdr.number_of_bytes_per_pixel, plane_dim, fp);
			if (i < plane_dim)
			{
				fprintf (stderr, "varray: only %d objects are read\n", i);
				return FAIL;
			}

			/*
			 * Conversion of data to the original data type.
			 */
			data_conversion (ifhdr.number_format, dimension, varray, slice_data);

			free (varray);
		}
		else if (orientation == CORONAL)
		{
			for (i = 0; i < ifhdr.matrix_size [2]; i++)
			{
				/*
			 	 * Go to the right position.
			 	 */
				index	= (i * plane_dim + (slice_number - 1) * ifhdr.matrix_size [0]) 
					* ifhdr.number_of_bytes_per_pixel;
				fseek (fp, index, SEEK_SET);

				/*
			 	 * Read the specific row.
			 	 */
				varray	= (void *) calloc (ifhdr.matrix_size [0], 
					ifhdr.number_of_bytes_per_pixel);
				index	= fread (varray, ifhdr.number_of_bytes_per_pixel, 
					ifhdr.matrix_size [0], fp);
				if (index < ifhdr.matrix_size [0])
				{
					fprintf (stderr, "varray: only %d objects are read\n", index);
					return FAIL;
				}

				/*
			 	 * Conversion of data to the original data type.
			 	 */
				data_conversion (ifhdr.number_format, ifhdr.matrix_size [0], 
				varray, slice_data + i * ifhdr.matrix_size [0]);

				free (varray);
			}
		}
		else if (orientation == FACING_LEFT || orientation == FACING_RIGHT)
		{
			for (i = 0; i < ifhdr.matrix_size [2]; i++)
			{
				/*
			 	 * Read the slice data for slice_index = i + 1.
			 	 */
				varray	= (void *) calloc (plane_dim, ifhdr.number_of_bytes_per_pixel);
				farray	= (float *) calloc (plane_dim, sizeof (float));
				index	= fread (varray, ifhdr.number_of_bytes_per_pixel, plane_dim, fp);
				if (index < plane_dim)
				{
					fprintf (stderr, "varray: only %d objects are read\n", index);
					return FAIL;
				}

				/*
			 	 * Conversion of data to the original data type.
			 	 */
				data_conversion (ifhdr.number_format, plane_dim, varray, farray);
				free (varray);

				if (orientation == FACING_LEFT)
				{
					for (j = 0; j < ifhdr.matrix_size [1]; j++)
					{
						slice_data [i * ifhdr.matrix_size [1] + j]
						= farray [j * ifhdr.matrix_size [0] + slice_number - 1];
					}
				}
				else if (orientation == FACING_RIGHT)
				{
					for (j = 0; j < ifhdr.matrix_size [1]; j++)
					{
						slice_data [i * ifhdr.matrix_size [1] + j]
						= farray [(ifhdr.matrix_size [1] - 1 - j) 
						* ifhdr.matrix_size [0] + slice_number - 1];
					}
				}

				free (farray);
			}
		}
	}
	else if (strcmp (ifhdr.matrix_initial_element [0], "right") == 0
	&& strcmp (ifhdr.matrix_initial_element [1], "posterior") == 0
	&& strcmp (ifhdr.matrix_initial_element [2], "inferior") == 0)
	{
		if (orientation == TRANSVERSE)
		{
			/*
			 * Point to the right position.
			 */
			index	= (ifhdr.matrix_size [2] - slice_number) * plane_dim 
				* ifhdr.number_of_bytes_per_pixel;
			fseek (fp, index, SEEK_SET);

			/*
			 * Read the slice data for slice_index = matrix_size [2] - slice_number + 1.
			 */
			varray	= (void *) calloc (plane_dim, ifhdr.number_of_bytes_per_pixel);
			i	= fread (varray, ifhdr.number_of_bytes_per_pixel, plane_dim, fp);
			if (i < plane_dim)
			{
				fprintf (stderr, "varray: only %d objects are read\n", i);
				return FAIL;
			}

			/*
			 * Conversion of data to the original data type.
			 */
			data_conversion (ifhdr.number_format, dimension, varray, slice_data);

			free (varray);

			/*
			 * Reverse the read in data.
			 */
			index	= dimension / 2;
			for (i = 0; i < index; i++)
			{
				fbuffer				= slice_data [i];
				slice_data [i]			= slice_data [dimension - 1 - i];
				slice_data [dimension - 1 - i]	= fbuffer;
			}
		}
		else if (orientation == CORONAL)
		{
			for (i = 0; i < ifhdr.matrix_size [2]; i++)
			{
				/*
			 	 * Point to the right position.
			 	 */
				index	= ( (ifhdr.matrix_size [2] - 1 - i) * plane_dim
					+ (ifhdr.matrix_size [1] - slice_number) 
					* ifhdr.matrix_size [0] ) 
					* ifhdr.number_of_bytes_per_pixel;
				fseek (fp, index, SEEK_SET);

				/*
			 	 * Read the specific row.
			 	 */
				varray	= (void *) calloc (ifhdr.matrix_size [0], 
					ifhdr.number_of_bytes_per_pixel);
				index	= fread (varray, ifhdr.number_of_bytes_per_pixel, 
					ifhdr.matrix_size [0], fp);
				if (index < ifhdr.matrix_size [0])
				{
					fprintf (stderr, "varray: only %d objects are read\n", index);
					return FAIL;
				}

				/*
			 	 * Conversion of data to the original data type.
			 	 */
				current_data	= slice_data + i * ifhdr.matrix_size [0];
				data_conversion (ifhdr.number_format, ifhdr.matrix_size [0], 
						varray, current_data);

				free (varray);

				/*
			 	 * Reverse the read in data.
			 	 */
				index	= ifhdr.matrix_size [0] / 2;
				for (j = 0; j < index; j++)
				{
					fbuffer = current_data [j];
					current_data [j] = current_data [ifhdr.matrix_size [0] - 1 - j];
					current_data [ifhdr.matrix_size [0] - 1 - j]	= fbuffer;
				}
			}
		}
		else if (orientation == FACING_LEFT || FACING_RIGHT)
		{
			for (i = 0; i < ifhdr.matrix_size [2]; i++)
			{
				/*
			 	 * Read the slice data for slice_index = i.
			 	 */
				varray	= (void *) calloc (plane_dim, ifhdr.number_of_bytes_per_pixel);
				farray	= (float *) calloc (plane_dim, sizeof (float));
				index	= fread (varray, ifhdr.number_of_bytes_per_pixel, plane_dim, fp);
				if (index < plane_dim)
				{
					fprintf (stderr, "varray: only %d objects are read\n", index);
					return FAIL;
				}

				/*
			 	 * Conversion of data to the original data type.
			 	 */
				data_conversion (ifhdr.number_format, plane_dim, varray, farray);
				free (varray);

				if (orientation == FACING_LEFT)
				{
					for (j = 0; j < ifhdr.matrix_size [1]; j++)
					{
						slice_data [(ifhdr.matrix_size [2] - 1 - i) 
						* ifhdr.matrix_size [1] + j]
						= farray [(ifhdr.matrix_size [1] - 1 - j) 
						* ifhdr.matrix_size [0] 
						+ ifhdr.matrix_size [0] - slice_number];
					}
				}
				else if (orientation == FACING_RIGHT)
				{
					for (j = 0; j < ifhdr.matrix_size [1]; j++)
					{
						slice_data [(ifhdr.matrix_size [2] - 1 - i) 
						* ifhdr.matrix_size [1] + j]
						= farray [j * ifhdr.matrix_size [0] 
						+ ifhdr.matrix_size [0] - slice_number];
					}
				}

				free (farray);
			}
		}
	}

	fclose (fp);

	return SUCCEED;
}

PRIVATE void data_conversion (format, array_size, varray, array)
	char	*format;
	long	array_size;
	void	*varray;
	float	*array;
{
	char		*carray;
	double		*darray;
	float		*farray;
	int		*iarray;
	long		i;
	long		*larray;
	short int	*sarray;
	unsigned int	*uarray;

	if (strcmp (format, "signed integer") == 0
	|| strcmp (format, "short integer") == 0)
	{
		sarray	= (short *) varray;
		for (i = 0; i < array_size; i++)
		{
			array [i]	= (float) sarray [i];
		}
	}
	else if (strcmp (format, "unsigned integer") == 0)
	{
		uarray	= (unsigned int *) varray;
		for (i = 0; i < array_size; i++)
		{
			array [i]	= (float) uarray [i];
		}
	}
	else if (strcmp (format, "long integer") == 0)
	{
		larray	= (long *) varray;
		for (i = 0; i < array_size; i++)
		{
			array [i]	= (float) larray [i];
		}
	}
	else if (strcmp (format, "float") == 0)
	{
		farray	= (float *) varray;
		for (i = 0; i < array_size; i++)
		{
			array [i]	= (float) farray [i];
		}
	}
	else if (strcmp (format, "double") == 0)
	{
		darray	= (double *) varray;
		for (i = 0; i < array_size; i++)
		{
			array [i]	= (float) darray [i];
		}
	}
	else if (strcmp (format, "byte") == 0)
	{
		carray	= (char *) varray;
		for (i = 0; i < array_size; i++)
		{
			array [i]	= (float) carray [i];
		}
	}
}
