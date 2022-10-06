/* $Id$
 * $Log$ */
/* ===============================================================================
 *  Module:         writerealimg.c
 *  Date:           04/22/92
 *  Author:         Tom Yang
 *  Description:    Writes a 3D array to an ECAT matrix file.
 *	Returns:		SUCCEED or FAIL
 *	Notes:
 *		if main_header_proto = NULL, fields will be created;
 *		main headers may be created with matrix_init_main_header();
 *		subheaders may be created with matrix_init_subheader();
 *  History:
 *      13-Nov-95   Modified TOV
 * ===============================================================================
 */
#include <values.h>
#include <petutil.h>
#include <matrix7.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/writerealimg.c,v 1.19 1995/11/10 23:29:58 tom Exp tom $";

PUBLIC int      writerealimg (ecat_image_file, matval, width, height, num_slices,
			                      image3d, main_header_proto, subheader_list)
	char           *ecat_image_file;	/* filename */
	struct Matval   matval;				/* CTI structure */
	short           width;				/* number of pixels */
	short           height;				/* number of pixels */
	short           num_slices;			/* number of slices */
	float          *image3d;			/* 3D array of counts pre frame */
	Main_header    *main_header_proto;	/* main header */
	Image_subheader **subheader_list;	/* subheaders */
{
	MatrixFile     *matrix_create ();
	int             mat_numcod ();
	int             matrix_write ();

	Image_subheader *image_subheader;
	Main_header    *main_header;
	MatrixData     *matrixx_data;
	MatrixFile     *fptr_out;
	char           *short_image_file;
	float           abs_slice_min;
	float           abs_slice_max;
	float           ecat_calibration_fctr;
	float           max_pixel_value;
	float           scale;
	float           slice_min;
	float           slice_max;
	float           temp;
	int             frame_duration;
	int             i;
	int             index;
	int             j;
	int             matnum;
	int             slice_dimension;
	int             status;
	short          *matrix_data_array;

	main_header = matrix_init_main_header (ecat_image_file, PetImage, main_header_proto);
	short_image_file = strrchr (ecat_image_file, '/');
	if (short_image_file != NULL)
		strncpy (main_header->original_file_name, short_image_file + 1, 20);

/* Assign number of frames, planesm gates and bed positions to main header	*/

	main_header->num_frames = 1;
	main_header->num_planes = num_slices;

	fptr_out = matrix_create (ecat_image_file, MAT_CREATE, main_header);
	if (fptr_out == NULL) {
		fprintf (stderr, "ERROR [writerealimg]: cannot open %s\n", ecat_image_file);
		return FAIL;
	}
	slice_dimension = width * height;

/* Build matrices and write to output file	*/

	for (j = 1; j <= num_slices; j++) {
		image_subheader = matrixx_init_image_subheader (*(subheader_list + j - 1));
		if (*(subheader_list + j - 1) == NULL) {
			image_subheader->data_type = VAX_I2;
			image_subheader->x_dimension = width;
			image_subheader->y_dimension = height;
			image_subheader->z_dimension = 1;
		}
		image_subheader->num_dimensions = 2;
		matrixx_data = CreateMatrixData (image_subheader);
		image_subheader = (Image_subheader *) matrixx_data->shptr;

		matnum = mat_numcod (matval.frame, j, matval.gate, matval.data, matval.bed);

		index = (j - 1) * slice_dimension;
		slice_min = image3d[index];
		slice_max = slice_min;

		for (i = 0; i < slice_dimension; i++) {
			temp = image3d[index + i];
			if (slice_min > temp)
				slice_min = temp;
			else if (slice_max < temp)
				slice_max = temp;
		}

		abs_slice_min = pkg_abs (slice_min);
		abs_slice_max = pkg_abs (slice_max);
		max_pixel_value = pkg_max (abs_slice_max, abs_slice_min);
		scale = max_pixel_value / MAXSHORT;
		if (image_subheader->frame_duration != 0)
			frame_duration = image_subheader->frame_duration;
		else
			frame_duration = DEFAULT_FRAME_DURATION;

/*	if calibration_factor is not > 0, set it to 1.0 */

		if (pkg_abs (main_header->calibration_factor) >= ERROR_BOUND) {
			ecat_calibration_fctr = main_header->calibration_factor;
			printf ("Cal[4A] = %f\n", main_header->calibration_factor);
		} else {
			ecat_calibration_fctr = DEFAULT_CALIBRATION_FACTOR;
			printf ("Cal[4] = %f\n", main_header->calibration_factor);
		}

		matrix_data_array = (short *) calloc (slice_dimension, sizeof (short));
		if (matrix_data_array == (short *) NULL) {
			fprintf (stderr, "ERROR [writerealimg]: cannot allocate matrix_data_array\n");
			return FAIL;
		}
		if (max_pixel_value > 0.0) {
			for (i = 0; i < slice_dimension; i++) {
				temp = (image3d[index + i] * MAXSHORT) / max_pixel_value;
				matrix_data_array[i] = pkg_min (ROUND (temp), MAXSHORT);
			}
		}
		matrixx_data->data_ptr = (caddr_t) matrix_data_array;
		matrixx_data->matnum = matnum;

		if (main_header->system_type == SYSTEM_TYPE_961)
			image_subheader->scale_factor = scale / ecat_calibration_fctr;
		else
			image_subheader->scale_factor = scale / (ecat_calibration_fctr * frame_duration);
		if (max_pixel_value > 0.0) {
			temp = (slice_min * MAXSHORT) / max_pixel_value;
			image_subheader->image_min = pkg_max (ROUND (temp), -MAXSHORT);
			temp = (slice_max * MAXSHORT) / max_pixel_value;
			image_subheader->image_max = pkg_min (ROUND (temp), MAXSHORT);
		} else {
			image_subheader->image_min = 0;
			image_subheader->image_max = 0;
		}
		status = matrix_write (fptr_out, matnum, matrixx_data);
		if (status == ERROR) {
			free_matrix_data (matrixx_data);
			return FAIL;
		}
		free_matrix_data (matrixx_data);
	}
	matrix_close (fptr_out);
	return SUCCEED;
}
