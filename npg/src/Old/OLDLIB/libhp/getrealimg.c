/*$Id: getrealimg.c,v 1.34 1995/10/30 22:39:01 tom Exp $*/
/*$Log: getrealimg.c,v $
 * Revision 1.34  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.33  1995/09/28  18:34:01  ty7777
 * Updated for 961 data.
 *
 * Revision 1.32  1995/09/05  17:15:54  ty7777
 * Used public defined ERROR_BOUND.
 *
 * Revision 1.31  1995/09/05  17:01:26  ty7777
 * Used ERROR_BOUND.
 *
 * Revision 1.30  1995/06/16  19:01:13  ty7777
 * Changed the PETT VI system type from 6 to 953.
 *
 * Revision 1.29  1995/05/26  15:48:46  ty7777
 * Same as the last version.
 *
 * Revision 1.28  1994/10/31  21:35:44  ty7777
 * Modified the comments.
 *
 * Revision 1.27  1994/09/13  16:26:10  ty7777
 * Modify the documentation.
 *
 * Revision 1.26  1994/04/07  15:27:31  ty7777
 * After use purify on the PETT VI images.
 *
 * Revision 1.25  1994/03/25  22:03:57  ty7777
 * Changed the error message.
 *
 * Revision 1.24  1994/03/03  20:16:48  ty7777
 * Replace id with header.
 *
 * Revision 1.23  1994/02/07  17:52:52  ty7777
 * Moved the frame_duration outside the loop.
 *
 * Revision 1.22  1993/10/25  21:27:57  ty7777
 * Processing for multiple frames.
 *
 * Revision 1.21  1993/10/14  19:44:20  ty7777
 * Same as before.
 * Checked in so other programmers can access it.
 *
 * Revision 1.20  1993/10/08  17:15:44  ty7777
 * Added processing for ECAT images created from PETT VI images.
 *
 * Revision 1.19  1993/10/06  18:53:24  ty7777
 * Change matval checking.
 *
 * Revision 1.18  1993/10/05  21:09:23  ty7777
 * Use X:r to check if an image is reflected.
 *
 * Revision 1.17  1993/10/04  19:51:16  ty7777
 * Delected debugging messages.
 *
 * Revision 1.16  1993/10/04  19:49:57  ty7777
 * Added reflection.
 *
 * Revision 1.15  1993/08/06  14:53:26  ty7777
 * Added recid.
 *
 * Revision 1.14  1993/07/28  19:29:57  ty7777
 * Adding rcsid.
 *
 * Revision 1.13  1993/07/27  16:59:00  ty7777
 * Change comments.
 *
 * Revision 1.12  1993/07/23  15:36:59  ty7777
 * Change pixel value from counts per ms to counts per frame.
 *
 * Revision 1.11  1993/07/16  22:26:29  ty7777
 * Use short image name for original_file_name.
 *
 * Revision 1.10  1993/07/14  19:17:56  ty7777
 * Added ecat_calibration_factor in the computation of scaling factor.
 *
 * Revision 1.9  1993/07/12  23:49:09  ty7777
 * Adding ecat_calibration_fctr to scaling factor.
 *
 * Revision 1.8  1993/06/22  22:31:53  ty7777
 * Add documentation.
 *
 * Revision 1.7  1993/06/22  19:45:42  ty7777
 * Changed comments.
 *
 * Revision 1.6  1993/06/22  17:21:35  ty7777
 * Corrected date conversion.
 *
 * Revision 1.5  1993/06/22  16:35:04  ty7777
 * Removed MIR from description string.
 *
 * Revision 1.4  1993/06/22  16:25:53  ty7777
 * Adding lseek for reading PETT VI image header.
 *
 * Revision 1.3  1993/06/22  15:17:36  ty7777
 * Add error checking for PETT VI images.
 *
 * Revision 1.2  1993/05/05  18:55:39  ty7777
 * Updated and working version.
 *
 * Revision 1.1  1993/04/30  14:04:33  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		getrealimg.c

  Function:	void getrealimg (char *image_file, int image_type, struct Matval matval, 
			short width, short height, short num_slices, float *image3d, 
			Main_header *main_header, Image_subheader **subheader_list)

  Arguments:	
		image_file,	PET image file name, either PETT VI or ECAT.
		image_type,	image type. Possible values are PETT6_SCANNER (6),
				SUPER_PETT_I_SCANNER (7), SUPER_PETT_II_SCANNER (8),
				ECAT_SCANNER (10), ERROR_SCANNER (-1).
		matval,		structure containing number of frames, planes, etc, 
				used only if image_file is an ECAT PET image.
		width,		width of slices, passed in.
		height,		height of slices, passed in.
		num_slices,	number of slices in image, passed in;
		image3d,	3D real array. The 3D image is stored slice by slice, 
				and in each slice, row by row. Note that each pixel value 
				contained in the 3D array is counts per frame. This 3D array 
				is filled upon return.
		main_header,	ECAT image main header, returned only for ECAT images.
				The user can pass NULL if image_file is PETT VI. Note
				that comment field of the main_header is used to store
				information of such as the zorigin, the flip flag, the 
				scaling factor, whether the image is PETT VI or ECAT, and 
				which 3D filter is used. The default zorigin is 0. The flip
				flag is always set to TRUE after getrealimg () is caled.
		subheader_list,	ECAT image subheader list, the list size is the same 
				as the number of slices, returned only for ECAT images.
				The user can pass NULL if image_file is PETT VI.

  Description:	This function reads either a PETT VI image or an ECAT image and creates a 
		3D image with real pixel values. The 3D real array can be manipulated
		and the manipulated 3D real array can be written in ECAT image format by 
		using the function writerealimg ().

		Any fileds of the main_header and subheader_list can be changed by the
		programmer after calling getrealimg. Make sure the zorigin is what you
		want before writing a main header data structure out. The general sequence
		of data processing is as follows:

			1) calling getrealimg ().

			2) Processing the 3D image array and change the fields of
			   the main header and the subheaders if necessary. Make sure
			   zorigin is correct.

			3) Writing image by calling writerealimg ().

  Return:	SUCCEED for successful; FAIL for error.

  Author:	Tom Yang.

  History:	Created by Tom Yang on 04/21/92. 
___________________________________________________________________________*/

#include <values.h>
#include <sys/time.h>
#include <petutil.h>
#include <matrix_64.h>

#define	NPG_SYSTEM_TYPE		953	/* an assigned system type for PETT VI. */
#define	TWENTIETH_CENTURY	1900	/* first year of the twentieth century */

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/getrealimg.c,v 1.34 1995/10/30 22:39:01 tom Exp $";

PUBLIC int getrealimg (image_file, image_type, matval, width, height, 
		num_slices, image3d, main_header, subheader_list)
	char		*image_file;
	int		image_type;
	struct Matval	matval;
	short		width;
	short		height;
	short		num_slices;
	float		*image3d;
	Main_header	*main_header;
	Image_subheader	**subheader_list;
{
	MatrixData		*matrix_read ();

	BOOLEAN			encoded_flag;
	BOOLEAN			error_flag;
/*
	BOOLEAN			matval_assigned;
*/
	COMMENT_INFO		comment_info_data;
	Image_subheader		*image_subheader;
	Image_subheader_64	image_subheader64;
	Main_header_64		main_header64;
	MatrixData		*matrix_data;
	MatrixFile		*fptr;
	Mlist			*mlist;
	PETT6HDR		*pett6_header;
	char			*cheader;
	char			day [3];
	char			isotope [MAXLINE];
	char			month [3];
	char			*short_image_file;
	char			year [3];
	int			diff;  /* number of differences between spec and actual */
	int			dimension;
	int			fd;
	int			frame_duration;
	int			i;
	int			j;
	int			matnum;
	int			row_index;
	int			slice_index;
	float			decay_constant;
	float			ecat_calibration_fctr;
	float			halflife;
	float			pixel_size;
	float			plane_separation;
	float			scale;
	short			*header;
	short			*slice_data;
	struct tm		startTime;

	MatrixData *volume, *slice;
	MatDirNode *node;
	int        segment = 0;
	int	   npixels;

	dimension	= width * height;

	if (image_type == ECAT_SCANNER)
	{
		fptr	= OpenEcat (image_file, &num_slices, &width, &height, 
				&pixel_size, &plane_separation, matval);
		if (fptr == (MatrixFile *) NULL)
		{
			return FAIL;
		}
		bcopy (fptr->mhptr, main_header, sizeof (Main_header));
		mh64_convert (&main_header64, main_header);

		if (pkg_abs (main_header->plane_separation) < ERROR_BOUND)
			if (main_header->system_type == SYSTEM_TYPE_961)
				main_header->plane_separation	= DEFAULT_ECAT961_SEP;
			else
				main_header->plane_separation	= DEFAULT_ECAT_SEP;

		/*
		 * Get comment information.
		 */
		comment_info (main_header64.study_description, &comment_info_data);

		/*
	 	 * Build an 3D image array.
 		 */
		if (main_header->file_type == PetImage || main_header->file_type == ByteImage 
		|| main_header->file_type == ByteVolume || main_header->file_type == InterfileImage)
		{
			slice_index	= 0;
			for (i = 1; i <= num_slices; i++)
			{
				matnum	= mat_numcod (matval.frame, i, matval.gate, 
								matval.data, matval.bed);
				matrix_data	= matrix_read (fptr, matnum, UnknownMatDataType);
				if (matrix_data == NULL)
				{
					fprintf (stderr, "matrix_read error in getrealimg.\n");
					return FAIL;
				}

				slice_data	= (short *) matrix_data->data_ptr;
       				image_subheader	= (Image_subheader *) matrix_data->shptr;
				ih64_convert (&image_subheader64, image_subheader, main_header);
				if (image_subheader64.frame_duration != 0)
					frame_duration	= image_subheader64.frame_duration;
				else	
					frame_duration	= DEFAULT_FRAME_DURATION;
				if (pkg_abs (image_subheader64.ecat_calibration_fctr) >= ERROR_BOUND)
					ecat_calibration_fctr	= image_subheader64.ecat_calibration_fctr;
				else	
					ecat_calibration_fctr	= DEFAULT_CALIBRATION_FACTOR;

				/*
			 	 * PETT VI pixel is counts per frame.
			 	 * ECAT pixel is counts per milliseconds. Hence we need to 
				 * multiply the frame duration for each pixel to convert
				 * the 3D real array to counts per frame.
			 	 */
				if (main_header->system_type == SYSTEM_TYPE_961)
					scale	= image_subheader64.quant_scale 
						* ecat_calibration_fctr;
				else
					scale	= image_subheader64.quant_scale 
						* ecat_calibration_fctr * frame_duration;
				slice_index			= (i - 1) * dimension;
				image_subheader->z_pixel_size	= main_header->plane_separation;
       				*(subheader_list + i - 1)	
					= matrixx_init_image_subheader (image_subheader);

				if (!(comment_info_data.xflip_flag || comment_info_data.pett6_flag))
				{
					reflection (width, height, slice_data);
				}

				for (j = 0; j < dimension; j++)
				{
					image3d [slice_index + j] = slice_data [j] * scale;
				}

				free_matrix_data (matrix_data);
			}
		}
		else if (main_header->file_type == PetVolume)
		{
			slice_index	= 0;
			node = fptr->dirlist->first;
			while (node) 
			{
				volume = matrix_read (fptr, node->matnum, MAT_SUB_HEADER);

       				image_subheader	= (Image_subheader *) volume->shptr;
				ih64_convert (&image_subheader64, image_subheader, main_header);
				if (image_subheader64.frame_duration != 0)
					frame_duration	= image_subheader64.frame_duration;
				else	
					frame_duration	= DEFAULT_FRAME_DURATION;
				if (pkg_abs (image_subheader64.ecat_calibration_fctr) >= ERROR_BOUND)
					ecat_calibration_fctr	= image_subheader64.ecat_calibration_fctr;
				else	
					ecat_calibration_fctr	= DEFAULT_CALIBRATION_FACTOR;

				if (main_header->system_type == SYSTEM_TYPE_961)
					scale	= image_subheader64.quant_scale * ecat_calibration_fctr;
				else
					scale	= image_subheader64.quant_scale * ecat_calibration_fctr
						* frame_duration;

				for (i = 0; i < volume->zdim; i++) 
				{
					slice      = matrix_read_slice (fptr, volume, i, segment);
					slice_data = (short *) slice->data_ptr;

       					*(subheader_list + slice_index / dimension)	
					= matrixx_init_image_subheader (image_subheader);

					if (!(comment_info_data.xflip_flag 
					|| comment_info_data.pett6_flag))
					{
						reflection (width, height, slice_data);
					}

					for (j = 0; j < dimension; j++)
					{
						image3d [slice_index + j] = slice_data [j] * scale;
if (slice_data [j] * scale > 0.0)
	printf ("Yale 4: %f\n", image3d [slice_index + j]);
					}

					slice_index	+= dimension;

					free_matrix_data (slice);
				}
				free_matrix_data (volume);
				node = node->next;
			}
		}
		matrix_close (fptr);
	}
	else	/* PET VI image processing */
	{
		/* 
	 	 * Allocate header 
	 	 */
		header = (short *) calloc (HEADER_SIZE / sizeof (short), sizeof (short));
		if (header == (short *) NULL)
		{
			fprintf (stderr, "Error: Can't allocate header in getrealimg ().\n");
			return (FAIL);
		}

		fd	= open (image_file, O_RDONLY, 0);
		if (fd == -1)
		{
			fprintf (stderr, "Can't open %d\n", image_file); 
			return (FAIL);
		}

		/*
		 * Get the encoded_flag which will be used in getimage () 
		 * latter in this function. 
		 */
		image_type	= getimage_type (fd, image_file, &encoded_flag, &num_slices);
		if (image_type == ERROR_SCANNER)
		{
			fprintf (stderr, "getimage_type error in getrealimg ()\n"); 
			return FAIL;
		}

		/*
		 * Go to begining of image and read header.
		 */
		lseek (fd, 0L, 0);
		cheader	= (char *) header;
		if (read (fd, cheader, HEADER_SIZE) == -1)
		{
			fprintf (stderr, "Can not read PETT VI header in getrealimg ().\n");
			return FAIL;
		}
		pett6_header	= (PETT6HDR *) cheader;

		short_image_file	= strrchr (image_file, '/');
		if (short_image_file != NULL)
			strncpy (main_header->original_file_name, short_image_file + 1, 32);
		main_header->sw_version		= 64;
		main_header->system_type	= SYSTEM_TYPE_961;
		main_header->file_type		= 2;
		month [0]			= pett6_header->date [0];
		month [1]			= pett6_header->date [1];
		month [2]			= '\0';
		day [0]				= pett6_header->date [2];
		day [1]				= pett6_header->date [3];
		day [2]				= '\0';
		year [0]			= pett6_header->date [4];
		year [1]			= pett6_header->date [5];
		year [2]			= '\0';
		startTime.tm_mday		= atoi (day);
		startTime.tm_mon		= atoi (month);
		startTime.tm_year		= TWENTIETH_CENTURY + atoi (year);
		startTime.tm_hour		= 0;
		startTime.tm_min		= 0;
		startTime.tm_sec		= 0;
		main_header->scan_start_time	= mktime (&startTime);

		decay_constant			= pe2ieee (pett6_header->decay_constant);
		if (decay_constant >= 5.5e-3 && decay_constant <= 5.7e-3)
		{
			strcpy (isotope, "O-15");
			halflife	= 122.3;
		}
		else if (decay_constant >= 1.1e-3 && decay_constant <= 1.2e-3)
		{
			strcpy (isotope, "N-14");
			halflife	= 597.8;
		}
		else if (decay_constant >= 5.5e-4 && decay_constant <= 5.7e-4)
		{
			strcpy (isotope, "C-11");
			halflife	= 1223.0;
		}
		else if (decay_constant >= 1.6e-4 && decay_constant <= 1.7e-4)
		{
			strcpy (isotope, "Ga-68");
			halflife	= 4100.0;
		}
		else if (decay_constant >= 1.0e-4 && decay_constant <= 1.1e-4)
		{
			strcpy (isotope, "F-18");
			halflife	= 6583.2;
		}
		else
		{
			strcpy (isotope, "unknown");
			halflife	= 0.0;
		}
		strcpy (main_header->isotope_code, isotope);
		main_header->isotope_halflife	= halflife;

		strcpy (main_header->radiopharmaceutical, pett6_header->run_name);
		main_header->num_planes		= num_slices;
		main_header->num_frames		= 1;
		main_header->num_gates		= 1;
		main_header->num_bed_pos	= 0;
		strncpy (main_header->study_name, pett6_header->subject_id, 5);
		strncpy (main_header->patient_id, pett6_header->subject_id, 5);
		strcpy (main_header->facility_name, "Washington Univ.");
		strcpy (main_header->study_description, PETTVI_IMAGE_LABEL);
		comment_info (main_header->study_description, &comment_info_data);

		if (pett6_header->file_type == TRANSMISSION_PET1010)
		{
			main_header->acquisition_type	= TRANSMISSION_ECAT;
		}
		else if (pett6_header->file_type == EMISSION_PET1020 || 
			pett6_header->file_type == EMISSION_PET2000 || 
			pett6_header->file_type == EMISSION_PET2010)
		{
			main_header->acquisition_type	= EMISSION_ECAT;
		}

		if (num_slices == PETT6_7SLICES)
			main_header->plane_separation	= PETT6_SEPARATION;
		else if (num_slices == PETT6_14)
			main_header->plane_separation	= PETT6_SEPARATION / 2;
		else 
			main_header->plane_separation	= SMG_SEPARATION;

		frame_duration			= pett6_header->scan_time 
						* MILISECONDS_PER_SECOND;	
		if (frame_duration == 0)
		{
			fprintf (stderr, 
			"Error: scan time in header of %s is 0.\n",
			image_file);
			return FAIL;
		}

		for (i = 1; i <= num_slices; i++)
		{
			slice_index	= (i - 1) * width * height;
			image_subheader	= matrixx_init_image_subheader (NULL);
			image_subheader->data_type	= 2;
			image_subheader->num_dimensions	= 2;
			image_subheader->x_dimension	= width;
			image_subheader->y_dimension	= height;
			image_subheader->z_dimension	= 1;
			if (num_slices == PETT6_7SLICES || num_slices == PETT6_14)
				image_subheader->x_pixel_size	= PETT6_PIXEL_SIZE;
			else 
				image_subheader->x_pixel_size	= SMG_PIXEL_SIZE;
			image_subheader->y_pixel_size	= image_subheader->x_pixel_size;
			image_subheader->z_pixel_size	= main_header->plane_separation;

			/*
			 * PETT VI scan time is in seconds.
			 * ECAT frame_duration in in milliseconds.
			 */
			image_subheader->frame_duration		= frame_duration;
			main_header->calibration_factor		= DEFAULT_CALIBRATION_FACTOR;

			*(subheader_list + i - 1)	
					= matrixx_init_image_subheader (image_subheader);
			free (image_subheader);

			slice_data	= (short *) calloc (dimension, sizeof (short));
			getimage (slice_data, header, fd, image_file, i, 
				encoded_flag, num_slices, image_type, &error_flag);
			if (error_flag)
			{
				fprintf (stderr, "getimage error in getrealimg ()\n");
				return FAIL;
			}

			for (j = 0; j < dimension; j++)
			{
				image3d [slice_index + j]	= ((float) slice_data [j]);
			}
			free (slice_data);
		}
		close (fd);
		free (header);
	}

	comment_info_data.xflip_flag	= TRUE;
	info2comment (comment_info_data, main_header64.study_description);

	return SUCCEED;
}
