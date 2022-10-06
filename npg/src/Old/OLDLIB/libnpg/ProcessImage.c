/*$Id: ProcessImage.c,v 1.16 1995/11/10 16:27:48 tom Exp $*/
/*$Log: ProcessImage.c,v $
 * Revision 1.16  1995/11/10  16:27:48  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.15  1995/11/03  23:07:14  ty7777
 * Added ECAT961_SCANNER processing.
 *
 * Revision 1.14  1995/09/28  18:39:54  ty7777
 * Updated for 961 data.
 *
 * Revision 1.12  1994/11/09  16:35:06  ty7777
 * Changed the function prototype.
 *
 * Revision 1.11  1994/11/09  16:20:21  ty7777
 * Same as the last one.
 *
 * Revision 1.10  1994/10/14  16:28:49  ty7777
 * Same as the last version.
 *
 * Revision 1.9  1994/06/24  21:32:41  ty7777
 * Changed the error range to 0.001.
 *
 * Revision 1.8  1994/06/24  21:31:34  ty7777
 * Added error range.
 *
 * Revision 1.7  1994/06/21  17:03:48  ty7777
 * Use real numbers for log ().
 *
 * Revision 1.6  1994/04/14  15:41:42  ty7777
 * NONDECAY_HALFLIFE is changed from 999999.00 to 0.00.
 *
 * Revision 1.5  1994/04/13  19:11:01  ty7777
 * AChecking if the frame start time is correct.
 *
 * Revision 1.4  1994/04/13  16:55:45  ty7777
 * Added processing for well counter.
 *
 * Revision 1.3  1994/04/13  16:03:27  ty7777
 * Added processing for well counts.
 *
 * Revision 1.2  1994/04/08  15:01:53  ty7777
 * Delected voimean.h.
 *
 * Revision 1.1  1994/04/08  14:43:42  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		ProcessImage.c

  Function:	PUBLIC int ProcessImage (float *image3d, long dimension, 
		int processing, float halflife, char *imgfile, Matval matval)

  Arguments:	image3d,	a real array containing the image data, changed 
				upon return.
		dimension,	dimension of image.
		processing,	processing code.
		halflife,	halflife of isotope, used by decayed tissue
				activity processing.
		imgfile,	image file name.
		matval,		specify the frame number to work with, only used
				for processing tissue activity for ECAT images.

  Return:	SUCCEED: successful; FAIL: error.

  Description:	This function processes an image for CBF, CBV, and TISACT.

  Called by:	ProcessOxygen.c	

  Calling:	None.	

  History:	Created by Tom Yang on 03/18/94. 
___________________________________________________________________________*/

#include <imglist.h>
#include <matrix_64.h>
#include <values.h>
#include <math.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/ProcessImage.c,v 1.16 1995/11/10 16:27:48 tom Exp $";

PUBLIC int ProcessImage (image3d, dimension, processing, halflife, imgfile, matval)
	float		*image3d;
	long		dimension;
	int		processing;
	float		halflife;
	char		*imgfile;
	struct Matval	matval;
{
	MatrixData	*matrix_read ();

	BOOLEAN			correct_frame_start_time;
	BOOLEAN			encoded_flag;
	BOOLEAN			return_value;
	FILE			*recfp;
	HEADER_DATA		header_data;
	Image_subheader		*image_subheader;
	Image_subheader_64	image_subheader64;
	Main_header_64		main_header64;
	MatrixData		*matrixx_data;
	MatrixFile		*fptr;
	PETT6HDR		*pett6_header;
	char			*cheader;
	char			complete_hdrfile [MAXPATH];
	char			*extension;
	char			recfile [MAXPATH];
	char			short_hdrfile [MAXLINE];
	char			version_label [MAXLINE];
	float			decay_constant;
	float			frame_duration;
	float			frame_start_time;
	float			pixel_height;
	float			pixel_width;
	float			plane_separation;
	float			scan_start_time;
	float			sx;
	float			value;
	float			version_id;
	int			fd;
	int			image_type;
	int			matnum;
	int			scanner;
	long			i;
	short			height;
	short			num_slices;
	short			*short_header;
	short			width;

	if (processing == PROCESSING_NONE)
		return SUCCEED;

	gethdrfile (imgfile, short_hdrfile, complete_hdrfile);

	if (gethdr (short_hdrfile, &header_data) != SUCCEED)
	{
		if (gethdr (complete_hdrfile, &header_data) != SUCCEED)
		{
			fprintf (stderr, "Error: header file can not be found as %s or %s\n", 
				short_hdrfile, complete_hdrfile);

			return (FAIL);
		}
	}

	return_value	= SUCCEED;

	switch (processing)
	{
		case	PROCESSING_CBF:
			if (header_data.ntype == 2 || header_data.ntype == 4) 
			{
				for (i = 0; i < dimension; i++) 
				{
					value		= image3d [i];
					image3d [i]	= value * (header_data.aflow1 * value 
							+ header_data.bflow1);
				}
			}
			else
				return_value	= FAIL;

			break;
		case	PROCESSING_CBV:
			if (header_data.ntype == 3) 
			{
				for (i = 0; i < dimension; i++) 
				{
					image3d [i]	*= header_data.bvfactor1;
				}
			}
			else
				return_value	= FAIL;
			break;

		case	PROCESSING_TISACT:
		case	PROCESSING_WELLCOUNTS:
			if (header_data.ntype == 5) 
			{
				if (getimagedim (imgfile, matval, &scanner, &width, 
				&height, &num_slices, &pixel_width, &pixel_height, 
				&plane_separation) == FAIL) 
				{
					fprintf (stderr, "Error: getimagedim () from image %s.\n",
					imgfile);

					return FAIL;
				}

				if (scanner == PETT6_SCANNER)
				{
					if (pkg_abs (halflife - NONDECAY_HALFLIFE) <= 0.001)
						sx	= header_data.pettcnv1;
					else
						sx	= header_data.decay_factor1;

					if (processing == PROCESSING_WELLCOUNTS)
					{
						/* 
	 	 				 * Allocate header 
	 	 				 */
						short_header	= (short *) calloc (HEADER_SIZE 
								/ sizeof (short), sizeof (short));
						if (short_header == (short *) NULL)
						{
							fprintf (stderr, 
							"Error: Can't allocate header in ProcessImage ().\n");

							return (FAIL);
						}

						fd	= open (imgfile, O_RDONLY, 0);
						if (fd == -1)
						{
							fprintf (stderr, "Error: Can't open %s\n", imgfile); 

							return (FAIL);
						}

						/*
		 				 * Get the encoded_flag which will be used in getimage () 
		 				 * latter in this function. 
		 				 */
						image_type	= getimage_type (fd, imgfile, 
								&encoded_flag, &num_slices);
						if (image_type == ERROR_SCANNER)
						{
							fprintf (stderr, 
							"getimage_type error in ProcessImage ().\n"); 

							return FAIL;
						}

						/*
		 				 * Go to begining of image and read header.
	 					 */
						lseek (fd, 0L, 0);
						cheader	= (char *) short_header;
						if (read (fd, cheader, HEADER_SIZE) == -1)
						{
							fprintf (stderr, 
							"Can not read PETT VI header in ProcessImage ().\n");

							return FAIL;
						}
						pett6_header	= (PETT6HDR *) cheader;
						frame_duration	= pett6_header->scan_time;
					}
				}
				else if (scanner == ECAT953_SCANNER || scanner == ECAT961_SCANNER)
				{
					/*
					 * Get the starting time and duration of scan.
					 */
					fptr	= OpenEcat (imgfile, &num_slices, &width, &height, 
						&pixel_width, &plane_separation, matval);

					if (fptr == (MatrixFile *) NULL) 
					{
						fprintf (stderr, 
					"Error (ProcessImage.c): ecat image %s cannot be openned.\n",
					imgfile);

						return FAIL;
					}

					mh64_convert (&main_header64, fptr->mhptr);

					if (fptr->mhptr->file_type == PetImage 
					|| fptr->mhptr->file_type == PetVolume
					|| fptr->mhptr->file_type == ByteImage
					|| fptr->mhptr->file_type == ByteVolume
					|| fptr->mhptr->file_type == InterfileImage)
					{
						fprintf (stderr, 
			"Error (ProcessImage.c): ecat image %s does not contain image data.\n",
						imgfile);

						matrix_close (fptr);
						return FAIL;
					}

					matnum	= mat_numcod (matval.frame, matval.plane, 
						matval.gate, matval.data, matval.bed);

					matrixx_data	= matrix_read (fptr, matnum, MAT_SUB_HEADER);
					if (matrixx_data == NULL)
					{
						fprintf (stderr, 
						"matrixx_read () error in ProcessImage.c.\n");

						matrix_close (fptr);

						return FAIL;
					}

					/*
					 * Find the recfile and determine if the frame start
					 * time is correct or not.
					 */
					strncpy (recfile, "\0", MAXPATH);
					extension	= strrchr (imgfile, '.');
					if (extension != NULL)
					{
						strncpy (recfile, imgfile, 
						strlen (imgfile)  - strlen (extension));
					}
					else
						strcpy (recfile, imgfile);
					strcat (recfile, REC_EXT);

					correct_frame_start_time	= FALSE;
					recfp				= fopen (recfile, "r");
					if (recfp != NULL)
					{
						while (fscanf (recfp, "%s %f", 
						version_label, &version_id) != EOF)
						{
							if (strstr (version_label, "bkproj_sun") != NULL
							&& version_id < 1.5)
								correct_frame_start_time	= TRUE;
						}

						fclose (recfp);
					}

					/*
					 * Obtain the frame_duration and frame_start_time.
					 */
       					image_subheader		= (Image_subheader *) matrixx_data->shptr;
					ih64_convert(&image_subheader64, image_subheader, fptr->mhptr);

					frame_duration		= image_subheader64.frame_duration 
								/ ((float) MILISECONDS_PER_SECOND);
					frame_start_time	= image_subheader64.frame_start_time; 
					if (!correct_frame_start_time)
						frame_start_time	/= ((float) MILISECONDS_PER_SECOND);

					free_matrix_data (matrixx_data);

					/*
					 * Obtain the scan start time.
					 */
					matnum	= mat_numcod (1, matval.plane, 
						matval.gate, matval.data, matval.bed);

					matrixx_data	= matrix_read (fptr, matnum, MAT_SUB_HEADER);
					if (matrixx_data == NULL)
					{
						fprintf (stderr, 
						"matrixx_read () error in ProcessImage.c.\n");

						matrix_close (fptr);

						return FAIL;
					}

       					image_subheader	= (Image_subheader *) matrixx_data->shptr;
					ih64_convert(&image_subheader64, image_subheader, fptr->mhptr);
					scan_start_time	= image_subheader64.frame_start_time 
							/ ((float) MILISECONDS_PER_SECOND);

					free_matrix_data (matrixx_data);

					matrix_close (fptr);

					/*
					 * Compute the scaling factor.
					 */
					if (pkg_abs (halflife - NONDECAY_HALFLIFE) <= 0.001)
					{
						sx	= header_data.pieslope * 60
							/ frame_duration;
					}
					else
					{
						decay_constant	= log (2.0) / halflife; 
						sx	= header_data.pieslope * 60
							* decay_constant 
							* exp (decay_constant * (frame_start_time
							- scan_start_time))
							/ (1 - exp (-decay_constant * frame_duration));
					}
				}

				if (processing == PROCESSING_WELLCOUNTS)
				{
					sx	*= frame_duration;
				}

				for (i = 0; i < dimension; i++) 
				{
					image3d [i]	*= sx;
				}
			}
			else
				return_value	= FAIL;

			break;

		case	PROCESSING_NONE:

		default:
			break;
	}

	if (return_value == FAIL)
	{
		fprintf (stderr,
		"Error: processing %d and data type %d in header file %s does not match.\n",
		processing, header_data.ntype, short_hdrfile);
	}

	return return_value;
}

PUBLIC int gethdrfile (imgfile, short_hdrfile, complete_hdrfile)
	char	*imgfile;
	char	*short_hdrfile;
	char	*complete_hdrfile;
{
	char	*cbuffer;
	char	*extension;
	char	short_imgfile [MAXLINE];

	extension	= strrchr (imgfile, '/');

	strncpy (complete_hdrfile, "\0", MAXLINE);
	if (extension != NULL)
	{
		cbuffer	= extension + 1; 
		strncpy (complete_hdrfile, imgfile, strlen (imgfile)  - strlen (extension) + 1);
	}
	else
	{
		cbuffer	= imgfile;
	}
	strcpy (short_imgfile,	cbuffer); 

	strncpy (short_hdrfile, "\0", MAXLINE);
	extension	= strrchr (short_imgfile, '.');
	if (extension != NULL)
	{
		strncpy (short_hdrfile, short_imgfile, strlen (short_imgfile)  - strlen (extension));
	}
	else
		strcpy (short_hdrfile, short_imgfile);

	strcat (short_hdrfile, HDR_EXT);
	strcat (complete_hdrfile, short_hdrfile);

	return SUCCEED;
}
