/*	$Id: getrealimg.c,v 1.39 1995/11/10 23:29:58 tom Exp tom $
	$Log: getrealimg.c,v $
 * Revision 1.39  1995/11/10  23:29:58  tom
 * *** empty log message ***
 * */
/* ===============================================================================
 *	Module:			getrealimg.c
 *	Date:			04/21/92
 *	Author:			Tom Yang
 *	Description:	Reads PETT VI, ECAT 953 or ECAT 961 images into a 3D array, image3d.
 *					Sets xflip_flag = TRUE (changing the comment field in main_header).
 *	Input parameters:
 *		filename
 *		scanner
 *	Output parameters:
 *		matval
 *		width
 *		height
 *		num_slices
 *		image3d
 *		main_header
 *		sub_header
 *	History:
 *		13-Nov-95	Modified TOV
 *	
 * ===============================================================================
 */
#include <values.h>
#include <sys/time.h>
#include <petutil.h>
#include <matrix_64.h>

#define	TWENTIETH_CENTURY	1900		 /* first year of the twentieth century */

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/getrealimg.c,v 1.39 1995/11/10 23:29:58 tom Exp tom $";

PUBLIC int      getrealimg (image_file, scanner, matval, width, height,
			                    num_slices, image3d, main_header, subheader_list)
	char           *image_file;			/* filename */
	int             scanner;			/* type of scanner (6-12) declared in petutil.h */
	struct Matval   matval;				/* CTI structure */
	short           width;				/* number of pixels */
	short           height;				/* number of pixels */
	short           num_slices;			/* number of slices */
	float          *image3d;			/* 3D array of counts pre frame */
	Main_header    *main_header;		/* returned for ECAT images only */
	Image_subheader **subheader_list;	/* returned for ECAT images only */
{
	MatrixData     *matrix_read ();

	BOOLEAN         encoded_flag;
	BOOLEAN         error_flag;
	COMMENT_INFO    comment_info_data;
	Image_subheader *image_subheader;
	Image_subheader_64 image_subheader64;
	Main_header_64  main_header64;
	MatrixData     *matrix_data;
	MatrixFile     *fptr;
	Mlist          *mlist;
	PETT6HDR       *pett6_header;
	char           *cheader;
	char            day[3];
	char            isotope[MAXLINE];
	char            month[3];
	char           *short_image_file;
	char            year[3];
	int             dimension;
	int             fd;
	int             frame_duration;
	int             i, j;
	int             matnum;
	int             row_index;
	int             slice_index;
	float           decay_constant;
	float           ecat_calibration_fctr;
	float           halflife;
	float           pixel_size;
	float           plane_separation;
	float           scale;
	short          *header;
	short          *slice_data;
	struct tm       startTime;

	MatrixData     *volume, *slice;
	MatDirNode     *node;
	int             segment = 0;
	int             npixels;

	dimension = width * height;

/*
 *	ECAT image files (953 or 961)
 */

/*	Read ECAT files and convert to 7.0 header structure */

	if (scanner == ECAT953_SCANNER || scanner == ECAT961_SCANNER) {
		fptr = OpenEcat (image_file, &num_slices, &width, &height,
				 &pixel_size, &plane_separation, matval);
		if (fptr == (MatrixFile *) NULL) return FAIL;
		bcopy (fptr->mhptr, main_header, sizeof (Main_header));
		printf ("Cal[X] = %f\n", main_header->calibration_factor);
		mh64_convert (&main_header64, main_header);

		if (pkg_abs (main_header->plane_separation) < ERROR_BOUND)
			if (main_header->system_type == SYSTEM_TYPE_961)
				main_header->plane_separation = DEFAULT_ECAT961_SEP;
			else
				main_header->plane_separation = DEFAULT_ECAT_SEP;

/* Get comment information */

		comment_info (main_header->study_description, &comment_info_data);

/* Build an 3D image array */

		if (main_header->file_type == PetImage || main_header->file_type == ByteImage
		    || main_header->file_type == ByteVolume || main_header->file_type == InterfileImage) {
			slice_index = 0;
			for (i = 1; i <= num_slices; i++) {
				matnum = mat_numcod (matval.frame, i, matval.gate, matval.data, matval.bed);
				matrix_data = matrix_read (fptr, matnum, UnknownMatDataType);
				if (matrix_data == NULL) {
					fprintf (stderr, "ERROR [getrealimg]: matrix_read of %s\n", image_file);
					return FAIL;
				}
				slice_data = (short *) matrix_data->data_ptr;
				image_subheader = (Image_subheader *) matrix_data->shptr;
				printf ("Cal[0A] = %f\n", main_header->calibration_factor);
				ih64_convert (&image_subheader64, image_subheader, main_header);
				printf ("Cal[0a] = %f\n", main_header->calibration_factor);
				if (image_subheader64.frame_duration != 0)
					frame_duration = image_subheader64.frame_duration;
				else
					frame_duration = DEFAULT_FRAME_DURATION;

/*	if calibration_fctr is not > 0, set it to 1.0 */

				if (pkg_abs (image_subheader64.ecat_calibration_fctr) >= ERROR_BOUND) {
					ecat_calibration_fctr = image_subheader64.ecat_calibration_fctr;
					printf ("Cal[1A] = %f\n", image_subheader64.ecat_calibration_fctr);
				} else {
					ecat_calibration_fctr = DEFAULT_CALIBRATION_FACTOR;
					printf ("Cal[1] = %f\n", image_subheader64.ecat_calibration_fctr);
				}

/*
 *	ECAT pixels are multiplied by the frame duration to get pixel values
 *	in counts per frame. (ECAT953 pixel is counts per msec)
 */
				if (main_header->system_type == SYSTEM_TYPE_961)
					scale = image_subheader64.quant_scale * ecat_calibration_fctr;
				else
					scale = image_subheader64.quant_scale * ecat_calibration_fctr * frame_duration;
				slice_index = (i - 1) * dimension;
				image_subheader->z_pixel_size = main_header->plane_separation;
				*(subheader_list + i - 1) = matrixx_init_image_subheader (image_subheader);

				if (!(comment_info_data.xflip_flag || comment_info_data.pett6_flag))
					reflection (width, height, slice_data);
				for (j = 0; j < dimension; j++)
					image3d[slice_index + j] = slice_data[j] * scale;
				free_matrix_data (matrix_data);
			}
		} else if (main_header->file_type == PetVolume) {
			slice_index = 0;
			node = fptr->dirlist->first;
			while (node) {
				volume = matrix_read (fptr, node->matnum, MAT_SUB_HEADER);
				image_subheader = (Image_subheader *) volume->shptr;
				ih64_convert (&image_subheader64, image_subheader, main_header);
				if (image_subheader64.frame_duration != 0)
					frame_duration = image_subheader64.frame_duration;
				else
					frame_duration = DEFAULT_FRAME_DURATION;

/*	if calibration_fctr is not > 0, set it to 1.0 */

				if (pkg_abs (image_subheader64.ecat_calibration_fctr) >= ERROR_BOUND) {
					ecat_calibration_fctr = image_subheader64.ecat_calibration_fctr;
					printf ("Cal[2A] = %f\n", image_subheader64.ecat_calibration_fctr);
				} else {
					ecat_calibration_fctr = DEFAULT_CALIBRATION_FACTOR;
					printf ("Cal[2] = %f\n", image_subheader64.ecat_calibration_fctr);
				}

				if (main_header->system_type == SYSTEM_TYPE_961)
					scale = image_subheader64.quant_scale * ecat_calibration_fctr;
				else
					scale = image_subheader64.quant_scale * ecat_calibration_fctr
						* frame_duration;

				for (i = 0; i < volume->zdim; i++) {
					slice = matrix_read_slice (fptr, volume, i, segment);
					slice_data = (short *) slice->data_ptr;

					*(subheader_list + slice_index / dimension)
						= matrixx_init_image_subheader (image_subheader);

					if (!(comment_info_data.xflip_flag || comment_info_data.pett6_flag))
						reflection (width, height, slice_data);
					for (j = 0; j < dimension; j++)
						image3d[slice_index + j] = slice_data[j] * scale;
					slice_index += dimension;
					free_matrix_data (slice);
				}
				free_matrix_data (volume);
				node = node->next;
			}
		}
		matrix_close (fptr);

/*
 *	PETT VI Image Files
 */

	} else {
		header = (short *) calloc (HEADER_SIZE / sizeof (short), sizeof (short));
		if (header == (short *) NULL) {
			fprintf (stderr, "ERROR [getrealimg]: cannot allocate header for %s\n", image_file);
			return (FAIL);
		}
		fd = open (image_file, O_RDONLY, 0);
		if (fd == -1) {
			fprintf (stderr, "ERROR [getrealimg]: cannot open %s\n", image_file);
			return (FAIL);
		}

/* Get the encoded_flag which will be used in getimage () latter in this function	*/

		scanner = getimage_type (fd, image_file, &encoded_flag, &num_slices);
		if (scanner == ERROR_SCANNER) {
			fprintf (stderr, "ERROR [getrealimg]: illegal scanner for %s\n", image_file);
			return FAIL;
		}

/* Go to begining of image and read header	*/

		lseek (fd, 0L, 0);
		cheader = (char *) header;
		if (read (fd, cheader, HEADER_SIZE) == -1) {
			fprintf (stderr, "ERROR [getrealimg]: reading PETT VI header for %s\n", image_file);
			return FAIL;
		}
		pett6_header = (PETT6HDR *) cheader;

		short_image_file = strrchr (image_file, '/');
		if (short_image_file != NULL)
			strncpy (main_header->original_file_name, short_image_file + 1, 32);
		main_header->sw_version = 64;
		main_header->system_type = SYSTEM_TYPE_PETT6;
		main_header->file_type = 2;
		month[0] = pett6_header->date[0];
		month[1] = pett6_header->date[1];
		month[2] = '\0';
		day[0] = pett6_header->date[2];
		day[1] = pett6_header->date[3];
		day[2] = '\0';
		year[0] = pett6_header->date[4];
		year[1] = pett6_header->date[5];
		year[2] = '\0';
		startTime.tm_mday = atoi (day);
		startTime.tm_mon = atoi (month);
		startTime.tm_year = TWENTIETH_CENTURY + atoi (year);
		startTime.tm_hour = 0;
		startTime.tm_min = 0;
		startTime.tm_sec = 0;
		main_header->scan_start_time = mktime (&startTime);

		decay_constant = pe2ieee (pett6_header->decay_constant);
		if (decay_constant >= 5.5e-3 && decay_constant <= 5.7e-3) {
			strcpy (isotope, "O-15");
			halflife = 122.3;
		} else if (decay_constant >= 1.1e-3 && decay_constant <= 1.2e-3) {
			strcpy (isotope, "N-14");
			halflife = 597.8;
		} else if (decay_constant >= 5.5e-4 && decay_constant <= 5.7e-4) {
			strcpy (isotope, "C-11");
			halflife = 1223.0;
		} else if (decay_constant >= 1.6e-4 && decay_constant <= 1.7e-4) {
			strcpy (isotope, "Ga-68");
			halflife = 4100.0;
		} else if (decay_constant >= 1.0e-4 && decay_constant <= 1.1e-4) {
			strcpy (isotope, "F-18");
			halflife = 6583.2;
		} else {
			strcpy (isotope, "unknown");
			halflife = 0.0;
		}
		strcpy (main_header->isotope_code, isotope);
		main_header->isotope_halflife = halflife;

		strcpy (main_header->radiopharmaceutical, pett6_header->run_name);
		main_header->num_planes = num_slices;
		main_header->num_frames = 1;
		main_header->num_gates = 1;
		main_header->num_bed_pos = 0;
		strncpy (main_header->study_name, pett6_header->subject_id, 5);
		strncpy (main_header->patient_id, pett6_header->subject_id, 5);
		strcpy (main_header->facility_name, "Washington Univ.");
		strcpy (main_header->study_description, PETTVI_IMAGE_LABEL);
		comment_info (main_header->study_description, &comment_info_data);

		if (pett6_header->file_type == TRANSMISSION_PET1010) {
			main_header->acquisition_type = TRANSMISSION_ECAT;
		} else if (pett6_header->file_type == EMISSION_PET1020 ||
			   pett6_header->file_type == EMISSION_PET2000 ||
			   pett6_header->file_type == EMISSION_PET2010) {
			main_header->acquisition_type = EMISSION_ECAT;
		}
		if (num_slices == PETT6_7SLICES)
			main_header->plane_separation = PETT6_SEPARATION;
		else if (num_slices == PETT6_14)
			main_header->plane_separation = PETT6_SEPARATION / 2;
		else
			main_header->plane_separation = SMG_SEPARATION;

		frame_duration = pett6_header->scan_time * MILISECONDS_PER_SECOND;
		if (frame_duration == 0) {
			fprintf (stderr, "ERROR [getrealimg]: frame_duration = 0 in %s\n", image_file);
			return FAIL;
		}
		for (i = 1; i <= num_slices; i++) {
			slice_index = (i - 1) * width * height;
			image_subheader = matrixx_init_image_subheader (NULL);
			image_subheader->data_type = 2;
			image_subheader->num_dimensions = 2;
			image_subheader->x_dimension = width;
			image_subheader->y_dimension = height;
			image_subheader->z_dimension = 1;
			if (num_slices == PETT6_7SLICES || num_slices == PETT6_14)
				image_subheader->x_pixel_size = PETT6_PIXEL_SIZE;
			else
				image_subheader->x_pixel_size = SMG_PIXEL_SIZE;
			image_subheader->y_pixel_size = image_subheader->x_pixel_size;
			image_subheader->z_pixel_size = main_header->plane_separation;

/*	Convert frame_duration to msec (PETT VI scan time is in sec) */

			image_subheader->frame_duration = frame_duration;
			main_header->calibration_factor = DEFAULT_CALIBRATION_FACTOR;
			printf ("Cal[3] set to 1 for PETT VI\n");
			*(subheader_list + i - 1)  = matrixx_init_image_subheader (image_subheader);
			free (image_subheader);

			slice_data = (short *) calloc (dimension, sizeof (short));
			getimage (slice_data, header, fd, image_file, i,
				  encoded_flag, num_slices, scanner, &error_flag);
			if (error_flag) {
				fprintf (stderr, "ERROR [getrealimg]: getimage for %s\n", image_file);
				return FAIL;
			}
			for (j = 0; j < dimension; j++) 
				image3d[slice_index + j] = ((float) slice_data[j]);
			free (slice_data);
		}
		close (fd);
		free (header);
	}

	comment_info_data.xflip_flag = TRUE;
	info2comment (comment_info_data, main_header->study_description);
	return SUCCEED;
}
