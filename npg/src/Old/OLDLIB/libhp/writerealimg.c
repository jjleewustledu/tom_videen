/*$Id: writerealimg.c,v 1.16 1995/10/30 22:39:01 tom Exp $*/
/*$Log: writerealimg.c,v $
 * Revision 1.16  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.15  1995/10/13  17:54:02  ty7777
 * Use matrix7.h.
 *
 * Revision 1.14  1995/09/28  18:38:00  ty7777
 * Updated for 961 data.
 *
 * Revision 1.13  1995/06/01  17:38:44  ty7777
 * Same as the last version.
 *
 * Revision 1.12  1994/11/02  16:35:51  ty7777
 * Before building on the HPs.
 *
 * Revision 1.11  1994/08/11  19:00:40  ty7777
 * Changes the comments.
 *
 * Revision 1.10  1994/03/30  22:07:45  ty7777
 * Deleted free (main_header).
 *
 * Revision 1.9  1994/03/03  20:46:00  ty7777
 * Replace id with header.
 *
 * Revision 1.8  1993/10/05  21:10:06  ty7777
 * Same as the last version.
 *
 * Revision 1.7  1993/07/28  19:30:44  ty7777
 * Adding rcsid.
 *
 * Revision 1.6  1993/07/23  15:43:28  ty7777
 * Added comeents for counts per frame for pixel value.
 *
 * Revision 1.5  1993/07/23  15:40:17  ty7777
 * Introduced frame_duration.
 *
 * Revision 1.4  1993/07/16  22:27:00  ty7777
 * Use short image name for original_file_name.
 *
 * Revision 1.3  1993/06/22  22:32:11  ty7777
 * Add documentation.
 *
 * Revision 1.2  1993/05/05  18:56:21  ty7777
 * Updated and working version.
 *
 * Revision 1.1  1993/04/30  14:05:56  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		writerealimg.c

  Function:	int writerealimg (char *ecat_image_file, struct Matval matval, 
			short width, short height, short num_slices, float *image3d, 
			Main_header *main_header_proto, Image_subheader **subheader_list)

  Arguments:	
		ecat_image_file,	name of output ECAT image file.
		matval,			structure containing number of frames, planes, etc.
		width,			spatial dimension, width.
		height,			spatial dimension, height.
		num_slices,		number of slices in image;
		image3d,		3D image expressed in real. The 3D image is stored 
					slice by slice, and in each slice, column by column.
					Note that each pixel in the 3D real array must be
					the counts per frame. This array should be passed in.
		main_header_proto,	ECAT image main header. If the user specifies NULL as
					the main_header prototype, the program will only fill
					in the necessary fields needed for image creation. It 
					is a good practice to use an existing Main_header 
					structure if there is one. If the user trys to create
					a Main_header structure, he can use the library 
					function 
						matrix_init_main_header ().
		subheader_list,		ECAT image subheader list, the list size is the same 
					as the number of slices. Image_subheader structure 
					contains fields which are very important for the 
					image stored in a plane, such as frame_duration, 
					pixel_size, and slice_width. If these fields are 
					wrong, the final image would be wrong. Each item in
					the subheader_list can be set to NULL, but it is a 
					good practice to use an Image_subheader structure if
					one exists. To copy an existing Image_subheader 
					structure to a pointer on the subheader_list and do 
					memory allocation, one can use the group's library
					function
       						matrix_init_image_subheader ().

  Return:	SUCCEED for successful; FAIL for error.

  Description:	This function writes a 3D image to an ECAT matrix file.

  Author:	Tom Yang.

  History:	Created by Tom Yang on 04/22/93. 
___________________________________________________________________________*/

#include <values.h>
#include <petutil.h>
#include <matrix7.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/writerealimg.c,v 1.16 1995/10/30 22:39:01 tom Exp $";

PUBLIC int writerealimg (ecat_image_file, matval, width, height, num_slices, 
	image3d, main_header_proto, subheader_list)
	char		*ecat_image_file;
	struct Matval	matval;
	short		width;
	short		height;
	short		num_slices;
	float		*image3d;
	Main_header	*main_header_proto;
	Image_subheader	**subheader_list;
{
	MatrixFile		*matrix_create ();
	int			mat_numcod ();
	int			matrix_write ();

	Image_subheader		*image_subheader;
	Main_header		*main_header;
	MatrixData		*matrixx_data; 
	MatrixFile		*fptr_out;
	char			*short_image_file;
	float			abs_slice_min;
	float			abs_slice_max;
	float			ecat_calibration_fctr;
	float			max_pixel_value;
	float			scale;
	float			slice_min;
	float			slice_max;
	float			temp;
	int			frame_duration;
	int			i;
	int			index;
	int			j;
	int			matnum;
	int			slice_dimension;
	int			status;
	short			*matrix_data_array;

	main_header		= matrix_init_main_header (ecat_image_file, 
					PetImage, main_header_proto);
	short_image_file	= strrchr (ecat_image_file, '/');
	if (short_image_file != NULL)
		strncpy (main_header->original_file_name, short_image_file + 1, 20);

	/*
	 * Assign number of frames, planesm gates and bed positions to main header.
	 */
 	main_header->num_frames		= 1;
	main_header->num_planes		= num_slices;

	fptr_out	= matrix_create (ecat_image_file, MAT_CREATE, main_header);
	if (fptr_out == NULL) 
	{
		fprintf (stderr, "Error while in writerealimg: cannot open %s\n", ecat_image_file);
		return FAIL;
	}

	slice_dimension		= width * height;

	/*
	 * Build matrices and write to output file.   
	 */
	for (j = 1; j <= num_slices; j++) 
	{
       		image_subheader	= matrixx_init_image_subheader (*(subheader_list + j - 1));
		if (*(subheader_list + j - 1) == NULL)
		{
			image_subheader->data_type	= VAX_I2;
			image_subheader->x_dimension	= width;
			image_subheader->y_dimension	= height;
			image_subheader->z_dimension	= 1;
		}
		image_subheader->num_dimensions	= 2;
		matrixx_data	= CreateMatrixData (image_subheader);
       		image_subheader	= (Image_subheader*) matrixx_data->shptr;

		matnum	= mat_numcod (matval.frame, j, matval.gate, matval.data, matval.bed);

		index		= (j - 1) * slice_dimension; 	
		slice_min	= image3d [index];
		slice_max	= slice_min;

		for (i = 0; i < slice_dimension; i++)
		{
			temp	= image3d [index + i];
			if (slice_min > temp)
				slice_min	= temp;
			else if (slice_max < temp)
				slice_max	= temp;
		}

		abs_slice_min	= pkg_abs (slice_min);
		abs_slice_max	= pkg_abs (slice_max);
		max_pixel_value	= pkg_max (abs_slice_max, abs_slice_min);
		scale		= max_pixel_value / MAXSHORT; 
		if (image_subheader->frame_duration != 0)
			frame_duration	= image_subheader->frame_duration;
		else	
			frame_duration	= DEFAULT_FRAME_DURATION;
		if (pkg_abs (main_header->calibration_factor) >= ERROR_BOUND)
			ecat_calibration_fctr	= main_header->calibration_factor;
		else	
			ecat_calibration_fctr	= DEFAULT_CALIBRATION_FACTOR;

		matrix_data_array	= (short *) calloc (slice_dimension,  sizeof (short));
		if (matrix_data_array == (short *) NULL)
		{
			fprintf (stderr, "writerealimg: Can't allocate matrix_data_array.\n");
			return FAIL;
		}

		if (max_pixel_value > 0.0)
		{
			for (i = 0; i < slice_dimension; i++)
			{
				temp	= (image3d [index + i] * MAXSHORT) / max_pixel_value;
				matrix_data_array [i]	= pkg_min (ROUND (temp), MAXSHORT);
			}
		}

        	matrixx_data->data_ptr		= (caddr_t) matrix_data_array;
		matrixx_data->matnum		= matnum;

		image_subheader->scale_factor	= scale / (ecat_calibration_fctr * frame_duration); 
		if (max_pixel_value > 0.0)
		{
			temp				= (slice_min * MAXSHORT) / max_pixel_value;
			image_subheader->image_min	= pkg_max (ROUND (temp), -MAXSHORT);
			temp				= (slice_max * MAXSHORT) / max_pixel_value;
			image_subheader->image_max	= pkg_min (ROUND (temp), MAXSHORT);
		}
		else
		{
			image_subheader->image_min	= 0;
			image_subheader->image_max	= 0;
		}

		status	= matrix_write (fptr_out, matnum, matrixx_data);
		if (status == ERROR) 
		{
        		free_matrix_data (matrixx_data);
			return FAIL;
		}
		free_matrix_data (matrixx_data);
	}

	matrix_close (fptr_out);

	return SUCCEED;
}
