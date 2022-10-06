/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : Feb. 1998
  Modifications :
* endtopic History

******************************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "matrix.h"
#include "matrix_extra.h"
#include "NChrplus.h"

/*======================================================================*/
void            generate_filter (
				                 float sigma,	/* The sigma of the filter in 1
								 * dimension (in pixels ) */
				                 float n_sigmas,	/* Request a number of
									 * sigma's wide */
				                 int *size,	/* Returned half size of the filter */
				                 float **filter	/* Filter elements, created here */
)
{

	float           exp_scale, scale;
	int             i;

	/*
	 * Determine filter size ( one side of the filter, since it is symmetrical, this is
	 * sufficient )
	 */
	*size = (int) (n_sigmas * sigma + 0.5);
	/* Allocate memory for filter */
	*filter = (float *) calloc (*size + 1, sizeof (float));

	/* Fill in */
	/* Central element */
	(*filter)[0] = 1.0;

	/* if size == 0: identity filter 1 */
	if (*size == 0)
		return;

	/* Create filter */
	exp_scale = 1.0 / (2.0 * sigma * sigma);
	scale = 0.0;
	for (i = 1; i <= *size; i++) {
		(*filter)[i] = exp (-(float) (i * i) * exp_scale);
		scale += (*filter)[i];
	}
	scale = 2 * scale + 1.0;

	/* Rescale filter such that sum of coefficients equals 1. */
	for (i = 0; i <= *size; i++) {
		(*filter)[i] /= scale;
	}

	return;
}

/*======================================================================*/
void            convol_horizontal (
				                   float *imgin,	/* images have to be
									 * different */
				                   float *imgout,
				                   int ncols,
				                   int nrows,
				                   float *mask,
				                   int masksize)
{
	int             col;
	int             row;
	int             i;
	int             curcol;
	int             maskel;
	float          *p_in;
	float          *p_out;
	float           weight;

	p_out = imgout;
	for (i = 0; i < ncols * nrows; i++)
		*p_out++ = 0;

	for (col = 0; col < ncols; col++) {
		for (maskel = -masksize; maskel <= masksize; maskel++) {
			curcol = maskel + col;
			if ((curcol >= 0) && (curcol < ncols)) {
				p_out = imgout + col;
				p_in = imgin + curcol;
				weight = mask[abs (maskel)];
				for (row = 0; row < nrows; row++) {
					*p_out += weight * *p_in;
					p_in += ncols;
					p_out += ncols;
				}
			}
		}
	}

}

/*======================================================================*/
void            convol_vertical (
				                 float *imgin,	/* images have to be different */
				                 float *imgout,
				                 int ncols,
				                 int nrows,
				                 float *mask,
				                 int masksize)
{
	int             col;
	int             row;
	int             i;
	int             currow;
	int             maskel;
	float          *p_in;
	float          *p_out;
	float           weight;

	p_out = imgout;
	for (i = 0; i < ncols * nrows; i++)
		*p_out++ = 0;

	for (row = 0; row < nrows; row++) {
		for (maskel = -masksize; maskel <= masksize; maskel++) {
			currow = maskel + row;
			if ((currow >= 0) && (currow < nrows)) {
				p_out = imgout + row * ncols;
				p_in = imgin + currow * ncols;
				weight = mask[abs (maskel)];
				for (col = 0; col < ncols; col++) {
					*p_out++ += weight * *p_in++;
				}
			}
		}
	}
}

/*======================================================================*/
void            convol_axial (
			                      float **imgin,	/* array of images of size
								 * 2*masksize+1 */
			                      float *imgout,
			                      int ncols,
			                      int nrows,
			                      float *mask,
			                      int masksize)
{
	int             maskel;
	int             num;
	int             npixels;
	int             i;
	float          *p_in;
	float          *p_out;
	float           weight;
	float           scale;

	npixels = nrows * ncols;
	p_out = imgout;
	for (i = 0; i < npixels; i++)
		*p_out++ = 0;

	scale = 0;
	for (maskel = -masksize; maskel <= masksize; maskel++) {
		p_out = imgout;
		num = maskel + masksize;
		p_in = *(imgin + num);
		if (p_in != NULL) {
			weight = mask[abs (maskel)];
			for (i = 0; i < npixels; i++) {
				*p_out++ += weight * *p_in++;
			}
			scale += weight;
		}
	}
	p_out = imgout;
	for (i = 0; i < npixels; i++)
		*p_out++ /= scale;

}

/*======================================================================*/
MatrixData     *read_a_slice (
			                      MatrixFile * inputfile,
			                      MatrixData ** inputvolume,
			                      int is3d,
			                      int frame,
			                      int plane,
			                      int gate,
			                      int data,
			                      int bed)
{
	int             matnum;
	MatrixData     *inmatrix;
	if (is3d) {
		if (*inputvolume == NULL) {
			matnum = mat_numcod (frame, 1, gate, data, bed);
			*inputvolume = matrix_read (inputfile, matnum, MAT_SUB_HEADER);
			if (!*inputvolume) {
				printf ("Unable to read the volume\n");
				exit (0);
			}
		}
		inmatrix = matrix_read_slice (inputfile, *inputvolume, plane - 1, 0);
		if (!inmatrix) {
			printf ("Unable to read the slice from the volume");
			exit (0);
		}
	} else {
		matnum = mat_numcod (frame, plane, gate, data, bed);
		inmatrix = matrix_read (inputfile, matnum, UnknownMatDataType);
		if (!inmatrix) {
			printf ("Unable to read the slice from the image file\n");
			exit (0);
		}
	}
	return (inmatrix);

}

/*======================================================================*/
void            scale_image (
			                     float *img,
			                     int ncols,
			                     int nrows,
			                     float *scalefactor,
			                     short *simg,
			                     float *imagemin,
			                     float *imagemax,
			                     int version)
{
	int             i;
	float          *p_img;
	short          *p_simg;

	p_img = img;

/* If not version 70, scale this slice independently
****************************************************/
	if (version < 70) {
		*imagemax = *p_img;
		*imagemin = *p_img;
	}
	for (i = 0; i < ncols * nrows; i++) {
		if (*p_img > *imagemax)
			*imagemax = *p_img;
		if (*p_img < *imagemin)
			*imagemin = *p_img;
		p_img++;
	}

/* If not version 70, compute a scale factor, otherwise,
 * use the one from the original study.
 ****************************************/
	if (version < 70) {
		if ((-*imagemin) > *imagemax)
			*scalefactor = (-*imagemin) / 32760.0;
		else {
			if (*imagemax > 0)
				*scalefactor = *imagemax / 32760.0;
			else
				*scalefactor = 1.0;
		}
	}
	p_simg = simg;
	p_img = img;
	for (i = 0; i < ncols * nrows; i++) {
		*p_simg++ = (short) (*p_img++ / *scalefactor + 0.5);
	}


}

/*======================================================================*/
int main (argc, argv)
	int             argc;
	char           *argv[];

{
	char            infilename[150];
	char            outfilename[150];
	int             showhelp = 0;
	int             frame = 1;
	int             firstplane = 1;
	int             lastplane = -1;
	int             inplane;
	int             centerplane;
	int             gate = 1;
	int             data = 0;
	int             bed = 0;
	int             i;
	int             version = 0;
	int             verbose = 0;
	int             is3d;
	int             ncols = -1;
	int             nrows = -1;
	int             nplanes = -1;
	int             matnum_out;
	int             incm = 0;
	float           imagemin;
	float           imagemax;
	float           scalefactor;
	float           pixelsizecm;
	MatrixFile     *inputfile = NULL;
	MatrixData     *inputvolume = NULL;
	MatrixData     *inmatrix;
	MatrixFile     *outputfile = NULL;
	MatrixData     *outmatrix;
	Image_subheader *input_sub;
	Image_subheader *output_sub;
	Main_header     proto;
	float         **inimages;
	int             n_inimgs;
	float          *workimg1;
	float          *workimg2;
	float           sigma_xy = 1.5;
	float           fwhm_xy = -1.0;
	float           sigma_z = -1.0;
	float           fwhm_z = -1.0;
	float           tosigma;
	float           n_sigmas = 2.;
	int             masksize;
	float          *mask;
	float           n_sigmas_z = -1;
	int             masksize_z;
	float          *mask_z;
	short          *shortdata = NULL;

/* Initializations
******************/
	tosigma = 1. / sqrt (8. * log (2.));

/* Get the arguments
********************/
	strcpy (infilename, "");
	strcpy (outfilename, "");
	i = 1;
	while (i < argc) {
		if (strcmp (argv[i], "-i") == 0)
			strcpy (infilename, argv[i++ + 1]);
		if (strcmp (argv[i], "-o") == 0)
			strcpy (outfilename, argv[i++ + 1]);
		if (strcmp (argv[i], "-sx") == 0)
			sscanf (argv[i++ + 1], "%f", &sigma_xy);
		if (strcmp (argv[i], "-sz") == 0)
			sscanf (argv[i++ + 1], "%f", &sigma_z);
		if (strcmp (argv[i], "-fx") == 0) {
			sscanf (argv[i++ + 1], "%f", &fwhm_xy);
			sigma_xy = fwhm_xy * tosigma;
		}
		if (strcmp (argv[i], "-fz") == 0) {
			sscanf (argv[i++ + 1], "%f", &fwhm_z);
			sigma_z = fwhm_z * tosigma;
		}
		if (strcmp (argv[i], "-ns") == 0)
			sscanf (argv[i++ + 1], "%f", &n_sigmas);
		if (strcmp (argv[i], "-p0") == 0)
			sscanf (argv[i++ + 1], "%d", &firstplane);
		if (strcmp (argv[i], "-p1") == 0)
			sscanf (argv[i++ + 1], "%d", &lastplane);
		if (strcmp (argv[i], "-b") == 0)
			sscanf (argv[i++ + 1], "%d", &bed);
		if (strcmp (argv[i], "-g") == 0)
			sscanf (argv[i++ + 1], "%d", &gate);
		if (strcmp (argv[i], "-f") == 0)
			sscanf (argv[i++ + 1], "%d", &frame);
		if (strcmp (argv[i], "-V") == 0)
			sscanf (argv[i++ + 1], "%d", &version);
		if (strcmp (argv[i], "-h") == 0)
			showhelp = 1;
		if (strcmp (argv[i], "-v") == 0)
			verbose = 1;
		if (strcmp (argv[i], "-cm") == 0)
			incm = 1;
		i++;
	}

	if (sigma_z < 0)
		sigma_z = sigma_xy;
	if (n_sigmas_z < 0)
		n_sigmas_z = n_sigmas;

	showhelp = showhelp || (strcmp (infilename, "") == 0);
	if (showhelp) {
		printf ("Syntax: programname -i inputfile -o outputfile\n");
		printf ("         [-sx sigma] [-fx fwhm] [-sz sigma_z] [-fz fwhm_z] [-cm]\n");
		printf ("         [-ns nsigma] [-p0 firstplane] [-p1 lastplane]\n");
		printf ("         [-f frame] [-b bed] [-g gate] [-h] [-V version] [-v]\n");
		printf ("inputfile:  input file (PetVolume version 7.0 or image version 6\n");
		printf ("outputfile: output file\n");
		printf ("version:    version of the output file: 6 or 70\n");
		printf ("sigma:      standard deviation of the Gaussian mask\n");
		printf ("fwhm:       full width at half max of the Gaussian mask\n");
		printf ("sigma_z:    axial st. dev. of the Gaussian mask\n");
		printf ("            default: same as sigma\n");
		printf ("fwhm_z:     axial fwhm of the Gaussian mask\n");
		printf ("            default: same as fwhm\n");
		printf ("-cm:        when set, the arguments sigma, fwhm, sigma_z and\n");
		printf ("            fwhm_z are in cm, otherwise, they are in pixels\n");
		printf ("nsigma:     Gaussian is truncated at nsigma standard deviations\n");
		printf ("            default: 2.0\n");
		printf ("firstplane: default: 1\n");
		printf ("lastplane:  default: all planes are processed\n");
		printf ("frame, gate, bed:  default 1, 1, 0\n");
		printf ("-v:         verbose\n");
		printf ("-h:         show this help and stop\n");
		exit (0);
	}
/* Open the input file
**********************/
	inputfile = matrix_open (infilename, MAT_READ_ONLY, PetImage);
	if (!inputfile) {
		is3d = 1;
		inputfile = matrix_open (infilename, MAT_READ_ONLY, PetVolume);
		if (!inputfile) {
			printf ("Unable to open the file %s as an image or a volume\n",
				infilename);
			exit (0);
		}
	} else
		is3d = 0;

/* Read a slice to get the subheader.
*************************************/
	inmatrix = read_a_slice (inputfile, &inputvolume, is3d, frame, firstplane,
				 gate, data, bed);
	input_sub = (Image_subheader *) inmatrix->shptr;
	ncols = input_sub->x_dimension;
	nrows = input_sub->y_dimension;
	nplanes = input_sub->z_dimension;
	if (!is3d)
		nplanes = inputfile->mhptr->num_planes;
	if (verbose)
		printf ("Volume size is %d x %d x %d\n",
			ncols, nrows, nplanes);

	if (lastplane < 0)
		lastplane = nplanes;
	if (lastplane > nplanes) {
		printf ("Only %d planes in the volume, last plane set to %d\n", nplanes,
			nplanes);
	}
	if (verbose)
		printf ("Convolving planes %d to %d\n", firstplane, lastplane);

	if (incm) {
		sigma_xy /= input_sub->x_pixel_size;
		sigma_z /= input_sub->z_pixel_size;
		if (verbose) {
			printf ("Transaxial pixel size is %g cm, axial %g cm.\n",
				input_sub->x_pixel_size, input_sub->z_pixel_size);
			printf ("Transaxial sigma is %g pixels, axial %g pixels.\n",
				sigma_xy, sigma_z);
		}
	}
/* Generate the transaxial and axial masks.
*******************************************/
	generate_filter (sigma_xy, n_sigmas, &masksize, &mask);
	generate_filter (sigma_z, n_sigmas_z, &masksize_z, &mask_z);
	if (verbose) {
		printf ("Trans mask: ");
		for (i = 0; i <= masksize; i++)
			printf ("%g ", mask[i]);
		printf ("\n");
		printf ("Axial mask: ");
		for (i = 0; i <= masksize_z; i++)
			printf ("%g ", mask_z[i]);
		printf ("\n");
	}
/* Read the first images. Images that don't exist are set to NULL
****************************************/
	n_inimgs = 2 * masksize_z + 1;
	inimages = (float **) calloc (n_inimgs, sizeof (float *));

	if (verbose)
		printf ("Reading first images...\n");
	for (i = -masksize_z; i <= masksize_z; i++) {
		inplane = i + firstplane;
		if ((inplane >= 1) && (inplane <= nplanes)) {
			if (verbose)
				printf ("..%s, plane %d\n", infilename, inplane);
			inmatrix = read_a_slice (inputfile, &inputvolume, is3d, frame, inplane,
						 gate, data, bed);
			inimages[i + masksize_z] = NCread_mat_float (inmatrix, 1.0);

		} else {
			inimages[i + masksize_z] = NULL;
		}
	}


/* Check if the output file exists, if not, create it.
 * If it does exist: quit!
*****************************************************/
	memcpy (&proto, inputfile->mhptr, sizeof (Main_header));
	if (version == 0) {
		if (is3d)
			version = 70;
		else
			version = 6;
	}
	proto.sw_version = version;
	if (is3d)
		proto.file_type = PetVolume;
	else
		proto.file_type = PetImage;
	proto.num_planes = nplanes;
	if (verbose)
		printf ("Opening %s\n", outfilename);
	outputfile = matrix_create (outfilename, MAT_OPEN_EXISTING, &proto);
	if (!outputfile) {
		printf ("Error creating or opening image file %s\n", outfilename);
		exit (0);
	}
/* Make the matrixdata structure.
********************************/
	if (version == 70)
		matnum_out = mat_numcod (frame, 1, gate, data, bed);
	else
		matnum_out = mat_numcod (frame, centerplane, gate, data, bed);
	outmatrix = (MatrixData *) calloc (1, sizeof (MatrixData));
	memcpy (outmatrix, inmatrix, sizeof (MatrixData));
	output_sub = (Image_subheader *) calloc (1, sizeof (Image_subheader));
	memcpy (output_sub, input_sub, sizeof (Image_subheader));
	if (version == 70)
		outmatrix->mat_type = PetVolume;
	else
		outmatrix->mat_type = PetImage;
	scalefactor = output_sub->scale_factor;

	outmatrix->matfile = outputfile;
	outmatrix->shptr = (caddr_t) output_sub;
	outmatrix->data_ptr = (caddr_t) NULL;

	workimg1 = (float *) calloc (ncols * nrows, sizeof (float));
	workimg2 = (float *) calloc (ncols * nrows, sizeof (float));
	imagemin = 1e9;
	imagemax = 0;

	for (centerplane = firstplane; centerplane <= lastplane; centerplane++) {
		if (verbose)
			printf ("Smoothing plane %d\n", centerplane);

		/*
		 * Convolve in z-direction ************************
		 */
		convol_axial (inimages, workimg1, ncols, nrows, mask_z, masksize_z);

		/*
		 * Convolve in x and y directions *******************************
		 */
		convol_horizontal (workimg1, workimg2, ncols, nrows, mask, masksize);
		convol_vertical (workimg2, workimg1, ncols, nrows, mask, masksize);

		/*
		 * Scale and find min and max ***************************
		 */
		switch (output_sub->data_type) {
		case SunShort:
		case VAX_Ix2:
			if (shortdata == NULL)
				shortdata = (short *) calloc (ncols * nrows, sizeof (short));
			scale_image (workimg1, ncols, nrows, &scalefactor, shortdata,
				     &imagemin, &imagemax, version);
			outmatrix->data_min = (short) (imagemin / scalefactor + 0.5);
			outmatrix->data_max = (short) (imagemax / scalefactor + 0.5);
			outmatrix->data_ptr = (caddr_t) shortdata;
			output_sub->scale_factor = scalefactor;
			break;

		case IeeeFloat:
			outmatrix->data_ptr = (caddr_t) workimg1;
			break;

		default:
			printf ("Datatype %d not supported.\n", output_sub->data_type);
			exit (0);
		}

		/*
		 * Write matrix to file *********************
		 */
		if (version < 70) {
			output_sub->z_dimension = 1;
		} else {
			output_sub->z_dimension = nplanes;
		}
		if (verbose)
			printf ("Writing plane %d to %s\n", centerplane, outfilename);

		if (!NCmatrix_write_plane (outputfile, outmatrix, centerplane, frame, gate,
					   bed)) {
			printf ("Unable to write the plane\n");
		}

		/*
		 * Shift images one left, freeing the leftmost *******      *************************************
		 */
		free (inimages[0]);
		for (i = 0; i < n_inimgs - 1; i++)
			inimages[i] = inimages[i + 1];
		inimages[n_inimgs - 1] = NULL;

		/*
		 * Read rightmost image if available.       **********************************
		 */
		inplane++;
		if ((inplane >= 1) && (inplane <= nplanes)) {
			matrix_free (inmatrix);
			if (verbose)
				printf ("Reading %s, plane %d\n", infilename, inplane);
			inmatrix = read_a_slice (inputfile, &inputvolume, is3d, frame, inplane,
						 gate, data, bed);
		}
		inimages[n_inimgs - 1] = NCread_mat_float (inmatrix, 1.0);
	}
}
