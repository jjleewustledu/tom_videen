/*
 *	General information:
 *		ML-EM reconstruction of PET emission studies, including normalization and correction for attenuation.
 *		File I/O is based on the software from UCL, Louvain-la-Neuve, Belgium.
 *		The reconstruction is carried out using the library libNCproj_bproj.a.
 *		After reconstruction, the module zoom_matrix is called to zoom and shift the image,
 *		using exactly the same conventions as those in the reconstruction software of CTI-Siemens, for the Ecat 931.
 *	Author(s)     : J Nuyts, extension of mlempet.c for HR+
 *	Date          : dec 1997
 *	Modifications : TOV (Mar 2009)
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include <sys/time.h>
#include "matrix.h"
#include "matrix_extra.h"
#include "NCproj_bproj.h"
#include "NChrplus.h"
/*
#include "zoom_matrix.c"
*/
#define   IMAGE_MAX          32767		 /* Max integer value of an image */
#define   SMALL_Z_DIFFERENCE 2			 /* mm difference between bed positions */
#define   SMALL_COUNT        0.01
#define round(value) \
     ((int) (value + 0.5))

/*****************************************************************************/
float           cpu_s ()
{
	long            micros;
	float           seconds;

	micros = clock ();
	seconds = micros / 1000000.;
	return (seconds);
}

/*****************************************************************************
 * This module adds pixel values to compute the value of a larger pixel. 
 * This routine is only useful for zoom factors smaller than 1. When applied with zoomfactors larger than 1, 
 * it applies nearest neighbor interpolation, doing the computations inefficiently. 
 */
float           unzoom_2d (float *image,
			                   float coord[],
			                   int dim[],
			                   float zoomfactor)
{
	double          floor ();
	double          ceil ();
	float           weights[20][20];
	float           gewicht;
	float           totweight;
	float           halfwidth;
	float           col0, col1, row0, row1;
	float           value;
	int             curcol;
	int             currow;
	int             colpoints, rowpoints;
	int             col, row;
	int             lastcol, lastrow;

	halfwidth = 0.5 / zoomfactor;

	/* These are the edgepoints of the big pixel, in the coordinates of the original small pixels image.	*/

	col0 = coord[0] - halfwidth;
	col1 = coord[0] + halfwidth;
	row0 = coord[1] - halfwidth;
	row1 = coord[1] + halfwidth;

	colpoints = round (col1) - round (col0) + 1;
	rowpoints = round (row1) - round (row0) + 1;
	lastcol = colpoints - 1;
	lastrow = rowpoints - 1;

	/* Compute the weights proportional to the overlap of the big pixel and the original small pixels. */
	
	for (col = 0; col < colpoints; col++)
		for (row = 0; row < rowpoints; row++)
			weights[col][row] = 1.0;

	gewicht = 0.5 + round (col0) - col0;
	for (row = 0; row < rowpoints; row++)
		weights[0][row] *= gewicht;

	gewicht = 0.5 + round (row0) - row0;
	for (col = 0; col < colpoints; col++)
		weights[col][0] *= gewicht;

	gewicht = 0.5 + col1 - round (col1);
	for (row = 0; row < rowpoints; row++)
		weights[lastcol][row] *= gewicht;

	gewicht = 0.5 + row1 - round (row1);
	for (col = 0; col < colpoints; col++)
		weights[col][lastrow] *= gewicht;

	/* Scan all the pixels under the original big pixel, and if they are in the image, weight them and add them. */

	value = 0.0;
	totweight = 0.0;

	currow = round (row0);
	for (row = 0; row < rowpoints; row++) {
		if ((currow >= 0) && (currow < dim[1])) {
			curcol = round (col0);
			for (col = 0; col < colpoints; col++) {
				if ((curcol >= 0) && (curcol < dim[0])) {
					value += image[currow * dim[0] + curcol] * weights[col][row];
					totweight += weights[col][row];
				}
				curcol++;
			}
			currow++;
		}
	}

	/*
	 * printf("(\n"); for (row = 0; row < rowpoints; row++) { for (col = 0; col < colpoints;
	 * col++) printf("%7.3f ", weights[col][row]); printf("\n"); } printf(")\n");
	 */

	return (value / totweight);
}

/****************************************************************************
 * This module applies linear interpolation, to be used for zoom factors higher than 1. 
 * When applied with zoom factors smaller than 1, it applies undersampling.
 */
float           linear_2d_interpol (float *image,
				                    float coord[],
				                    int dim[])
{

	float           weight[2];
	float           grey_scale[4];
	int             x_pos_int;
	int             y_pos_int;
	double          floor ();

	/* Check first if point is not outside the original image */

	if ((coord[0] < 0) || (coord[0] > (dim[0] - 1)) ||
	    (coord[1] < 0) || (coord[1] > (dim[1] - 1)))
		return (0.0);
	else {
		x_pos_int = (int) floor (coord[0]);
		y_pos_int = (int) floor (coord[1]);
		weight[0] = coord[0] - x_pos_int;
		weight[1] = coord[1] - y_pos_int;

		grey_scale[0] = *(image + x_pos_int + (y_pos_int * dim[0]));

		if (x_pos_int < (dim[0] - 1))
			grey_scale[1] = *(image + x_pos_int + 1 + (y_pos_int * dim[0]));
		else
			grey_scale[1] = 0.0;

		if (y_pos_int < (dim[1] - 1))
			grey_scale[2] = *(image + x_pos_int + ((y_pos_int + 1) * dim[0]));
		else
			grey_scale[2] = 0.0;

		if ((x_pos_int < (dim[0] - 1)) && (y_pos_int < (dim[1] - 1)))
			grey_scale[3] = *(image + x_pos_int + 1 + ((y_pos_int + 1) * dim[0]));
		else
			grey_scale[3] = 0.0;

		grey_scale[0] = grey_scale[0] * (1 - weight[0]) +
			grey_scale[1] * weight[0];
		grey_scale[1] = grey_scale[2] * (1 - weight[0]) +
			grey_scale[3] * weight[0];
		grey_scale[0] = grey_scale[0] * (1 - weight[1]) +
			grey_scale[1] * weight[1];
		return (grey_scale[0]);
	}
}

/****************************************************************************/
float           apply_zoom (
			                    float *image_out,
			                    float *image_in,
			                    float pixel_size,
			                    int dim_in_x,
			                    int dim_in_y,
			                    int dim_out_x,
			                    int dim_out_y,
			                    float orig_zoom,
			                    float x_off,
			                    float y_off,
			                    float degrees,
			                    int verbose)
{
	float           zoom;

	float           x_shift;
	float           y_shift;
	float           angle;
	float           cosangle;
	float           sinangle;
	float           center_y_in;
	float           center_x_in;
	float           center_y_out;
	float           center_x_out;
	float           x;
	float           y;
	float           xrot;
	float           yrot;
	float           coord[2];
	float          *left;
	float          *pin, *pout;

	double          cos ();
	double          sin ();

	int             dim[2];
	int             x_pos;
	int             y_pos;
	int             copy = 0;


	zoom = orig_zoom * dim_out_x / dim_in_x;
	x_shift = -x_off / pixel_size;
	y_shift = -y_off / pixel_size;

	if ((zoom == 1) && (dim_in_x == dim_out_x) &&
	    (x_shift == 0.) && (y_shift == 0.) && (degrees == 0.))
		copy = 1;

	if (verbose) {
		printf ("Effective zoom is %g: ", zoom);
		if (copy) {
			printf ("No zoom, no rotation: straightforward copy.\n");
		} else {
			if (zoom >= 1)
				printf ("Applying linear interpolation to zoom.\n");
			else
				printf ("Averaging over pixels to unzoom.\n");
		}
	}
	if (!copy) {
		angle = -degrees * M_PI / 180;
		cosangle = cos (angle);
		sinangle = sin (angle);

		center_y_in = (dim_in_y - 1.0) / 2.0 + y_shift;
		center_x_in = (dim_in_x - 1.0) / 2.0 - x_shift;
		center_y_out = (dim_out_x - 1.0) / 2.0;
		center_x_out = (dim_out_y - 1.0) / 2.0;

		for (y_pos = 0; y_pos < dim_out_y; y_pos++) {
			for (x_pos = 0; x_pos < dim_out_x; x_pos++) {
				x = (x_pos - center_x_out);
				y = -(y_pos - center_y_out);
				xrot = (cosangle * x + sinangle * y) / zoom;
				yrot = (-sinangle * x + cosangle * y) / zoom;
				coord[0] = center_x_in + xrot;
				coord[1] = center_y_in - yrot;
				dim[0] = dim_in_x;
				dim[1] = dim_in_y;

				/*
				 * If zoom >= 1, then use linear interpolation. Otherwise, use the
				 * routine unzoom_2d, which adds pixels to produce a larger pixel. ******
				 */
				if (zoom >= 1)
					*(image_out + x_pos + y_pos * dim_out_x)
						= linear_2d_interpol (image_in, coord, dim);
				else
					*(image_out + x_pos + y_pos * dim_out_x)
						= unzoom_2d (image_in, coord, dim, zoom);
			}
		}
	} else {
		pin = image_in;
		pout = image_out;
		for (y_pos = 0; y_pos < dim_out_x; y_pos++)
			for (x_pos = 0; x_pos < dim_out_y; x_pos++)
				*pout++ = *pin++;
	}
	return (zoom);
}

/****************************************************************************/
float          *read_norm_sinogram (
				                    char *normalisation_name,
				                    char *sinogram_name,
				                    short int is3dsino,
				                    int plane,
				                    int frame,
				                    int gate,
				                    int bed,
				                    int nrdet,
				                    int nrproj,
				                    int verbose
)
{
	float          *norm_fl;
	MatrixFile     *norm_file;
	MatrixData     *normmatrix;
	float           meanval;
	int             matnum_nrm;

	if (is3dsino) {
		norm_fl = NCreadnormv7 (sinogram_name, normalisation_name, plane, frame, gate, bed, verbose, 1, 1, 1);
		return (norm_fl);
	} else {
		matnum_nrm = mat_numcod (1, plane, 1, 0, 0);
		if (verbose)
			printf ("Reading %s...\n", normalisation_name);
		norm_file = matrix_open (normalisation_name, MAT_READ_ONLY, Normalization);
		if (!norm_file) {
			if (verbose)
				printf ("Problems opening %s as normalisation, retrying as sinogram.\n",
					normalisation_name);
			norm_file = matrix_open (normalisation_name, MAT_READ_ONLY, Sinogram);
		}
		if (!norm_file) {
			printf ("Unable to read the normalisation file %s\n", normalisation_name);
			return (NULL);
		}
		normmatrix = matrix_read (norm_file, matnum_nrm, UnknownMatDataType);
		norm_fl = NCread_mat_float (normmatrix, 1.0);
		meanval = NCextend_nrm (norm_fl, norm_fl, nrdet, nrproj, 0.1);
		if (verbose)
			printf ("Mean normalization value = %g\n", meanval);
		return (norm_fl);
	}
}

/***************************************************************************
 *	This module computes the reduced sinogram. Reduction is done by adding
 *	neighboring pixels. In order to still have the correct sensitivity
 *	weight, the following is computed in the case of averaging over two pixels:
 *		original sinogram:    p1 and p2
 *		original sensitivy:   s1 and s2
 *		reduced sinogram:     p = p1 + p2
 *		reduced sensitivity:  s = (p1 + p2) / (p1/s1 + p2/s2)
 */

void            sum_detectors (
			                       float **sensit,
			                       float **sinogram,
			                       int sumdet,
			                       int nrproj,
			                       int *nrdet,
			                       float *arc_radius,
			                       float *pixelsizecm)
{
	float          *smallsensit = NULL;	 /* sensitivity with reduced size */
	float          *smallsino;		 /* sinogram with reduced size */
	float          *p_sensit;
	float          *p_sino;
	float          *p_smallsensit;
	float          *p_smallsino;
	float           denom;
	float           meansens;

	int             newdet;
	int             det, proj, i;

	if ((((int) (*nrdet / sumdet / 2)) * 2 * sumdet) != *nrdet) {
		printf ("Error: nr summed detectors should be a divider of the \n");
		printf ("       original number of detectors. In addition, the\n");
		printf ("       resulting number of detectors should be an even number\n");
		exit (0);
	}
	newdet = *nrdet / sumdet;
	if (*sensit != NULL) {
		smallsensit = (float *) calloc (newdet * nrproj, sizeof (float));
		p_smallsensit = smallsensit;
		for (i = 0; i < newdet * nrproj; i++)
			*p_smallsensit++ = 0.0;
	}
	smallsino = (float *) calloc (newdet * nrproj, sizeof (float));
	p_smallsino = smallsino;
	for (i = 0; i < newdet * nrproj; i++)
		*p_smallsino++ = 0.0;

	p_sensit = *sensit;
	p_sino = *sinogram;
	p_smallsensit = smallsensit;
	p_smallsino = smallsino;

	if (*sensit == NULL) {
		for (proj = 0; proj < nrproj; proj++) {
			for (det = 0; det < newdet; det++) {
				for (i = 0; i < sumdet; i++) {
					*p_smallsino += *p_sino++;
				}
				p_smallsino++;
			}
		}
	} else {
		for (proj = 0; proj < nrproj; proj++) {
			for (det = 0; det < newdet; det++) {
				denom = 0.0;
				meansens = 0.0;
				for (i = 0; i < sumdet; i++) {
					*p_smallsino += *p_sino;
					meansens += *p_sensit;
					denom += *p_sino++ / *p_sensit++;
				}
				if (denom > SMALL_COUNT)
					*p_smallsensit = *p_smallsino / denom;
				else
					*p_smallsensit = meansens / (float) sumdet;
				if (*p_smallsensit < 0)
					*p_smallsensit = 0.0;
				p_smallsensit++;
				p_smallsino++;
			}
		}
	}
	*nrdet = newdet;
	*arc_radius /= sumdet;			 /* arc_radius is in pixels */
	*pixelsizecm *= sumdet;

	if (*sensit != NULL)
		free (*sensit);
	free (*sinogram);

	*sensit = smallsensit;
	*sinogram = smallsino;

}

/****************************************************************************/
int main (argc, argv)
	int             argc;
	char           *argv[];

{
	p_ProjCoeff     projcoeff;

	time_t          timesec;
	struct tm      *timeb;

	char            sinogram_name[150];
	char            attenuation_name[150];
	char            normalisation_name[150];
	char            image_name[150];
	char            commandline[1000];
	char            commandlogname[150];

	short           data_type;

	int             iteraties[20];
	int             sub_iteraties[20];
	int             plane = 1;
	int             frame = 1;
	int             gate = 1;
	int             bed = 0;
	int             outplane, outframe, outgate, outbed;
	int             i, iter, iternum, mainiter;
	int             n_iter = -1;
	int             size_image = 128;
	int             version = 0;
	int             verbose = 0;
	int             decay = 1;
	int             help = 0;
	int             singleplane = 0;
	int             attn;
	int             norm;
	int             matnum, matnum_atn, matnum_vol;
	int             nrdet, nrproj;
	int             gate_duration;
	int             r_wave_offset;
	int             num_accepted_beats;
	int             col, row, pixel;
	int             nplanes;
	int             sumdet = 1;		 /* number of detectors to be added */
	int             calclik = 0;

	float           zoom = 1.0;
	float           globalzoom;
	float           x_offset = 0.0;
	float           y_offset = 0.0;
	float           rotation = 0.0;
	float           lsrelaxation = 0.0;
	float           relaxation;
	float           loss_correction;
	float           radius;
	float           startangle, deltaangle;
	float           image_min, image_max;
	float           cscale;
	float           decay_factor;
	float           scale_factor;
	float           sumdet_factor;
	float           half_life, frame_duration, frame_starttime;
	float           init_bed_pos;
	float           pixelsizecm = 0.0;
	float           arc_radius = 0.0;
	float           center, radkwad;
	float           startvalue;
	float          *cor_offset;		 /* center of rotation offset: 0.5 pixels */
	float          *p_cor;
	float           corvalue;

	short int      *image_short = NULL;
	short int      *p_image_short;
	short int       is3dsino = 0;
	short int       isatn70 = 0;

	float          *sinogram_fl = NULL;
	float          *norm_fl = NULL;
	float          *atten_fl = NULL;
	float          *sensit = NULL;
	float          *image_fl = NULL;
	float          *image_zoom = NULL;
	float          *sinovar = NULL;
	float          *p_sino;
	float          *p_sensit;
	float          *p_atten;
	float          *p_norm;
	float          *p_image_fl;
	float          *p_image_zoom;
	float          *p_sinovar;
	float          *p_calcsino;
	float          *backmlem = NULL;
	float          *likelihood;

	FILE           *logfile;
	MatrixData     *sinovolume;
	MatrixData     *atnvolume;
	MatrixFile     *sinogram_file;
	MatrixFile     *image_file;
	MatrixFile     *norm_file;
	MatrixFile     *atten_file;
	Main_header     proto;
	MatrixData     *imagematrix;
	MatrixData     *sinomatrix;
	MatrixData     *attenmatrix;

	Scan_subheader *scn_sub;
	Scan3D_subheader *scn3d_sub;
	Image_subheader *img_sub;

	strcpy (attenuation_name, "none\0");
	strcpy (normalisation_name, "none\0");
	strcpy (sinogram_name, "error\0");
	strcpy (image_name, "error\0");
	strcpy (commandlogname, "");

	/* get time of start reconstruction */

	time (&timesec);
	timeb = localtime (&timesec);

	i = 1;
	while (i < argc) {
		if (strcmp (argv[i], "-s") == 0)
			strcpy (sinogram_name, argv[i + 1]);
		if (strcmp (argv[i], "-a") == 0)
			strcpy (attenuation_name, argv[i + 1]);
		if (strcmp (argv[i], "-n") == 0)
			strcpy (normalisation_name, argv[i + 1]);
		if (strcmp (argv[i], "-o") == 0)
			strcpy (image_name, argv[i + 1]);
		if (strcmp (argv[i], "-l") == 0)
			strcpy (commandlogname, argv[i + 1]);
		if (strcmp (argv[i], "-i") == 0) {
			n_iter++;
			sscanf (argv[i + 1], "%d", &iteraties[n_iter]);
		}
		if (strcmp (argv[i], "-j") == 0)
			sscanf (argv[i + 1], "%d", &sub_iteraties[n_iter]);
		if (strcmp (argv[i], "-p") == 0)
			sscanf (argv[i + 1], "%d", &plane);
		if (strcmp (argv[i], "-f") == 0)
			sscanf (argv[i + 1], "%d", &frame);
		if (strcmp (argv[i], "-g") == 0)
			sscanf (argv[i + 1], "%d", &gate);
		if (strcmp (argv[i], "-b") == 0)
			sscanf (argv[i + 1], "%d", &bed);
		if (strcmp (argv[i], "-z") == 0)
			sscanf (argv[i + 1], "%f", &zoom);
		if (strcmp (argv[i], "-X") == 0)
			sscanf (argv[i + 1], "%d", &size_image);
		if (strcmp (argv[i], "-x") == 0)
			sscanf (argv[i + 1], "%f", &x_offset);
		if (strcmp (argv[i], "-y") == 0)
			sscanf (argv[i + 1], "%f", &y_offset);
		if (strcmp (argv[i], "-r") == 0)
			sscanf (argv[i + 1], "%f", &rotation);
		if (strcmp (argv[i], "-R") == 0)
			sscanf (argv[i + 1], "%f", &arc_radius);
		if (strcmp (argv[i], "-sumd") == 0)
			sscanf (argv[i + 1], "%d", &sumdet);
		if (strcmp (argv[i], "-ls") == 0)
			sscanf (argv[i + 1], "%f", &lsrelaxation);
		if (strcmp (argv[i], "-V") == 0)
			sscanf (argv[i + 1], "%d", &version);
		if (strcmp (argv[i], "-d") == 0) {
			if ((argv[i + 1][0] == 'n') || (argv[i + 1][0] == 'N') || (argv[i + 1][0] == '0'))
				decay = 0;
		}
		if (strcmp (argv[i], "-h") == 0) {
			i--;
			help = 1;
		} else if (strcmp (argv[i], "-v") == 0) {
			i--;
			verbose = 1;
		} else if (strcmp (argv[i], "-lik") == 0) {
			i--;
			calclik = 1;
		} else if (strcmp (argv[i], "-SP") == 0) {
			i--;
			singleplane = 1;
		}
		i += 2;
	}

	if ((strcmp (sinogram_name, "error") == 0) ||
	    (strcmp (image_name, "error") == 0) || (help)) {

		printf ("Syntax: program_name -s sinogram -o outputfile [-a attenuation]\n");
		printf ("  [-n normalisation] [-i nr_iterations  -j nr_subiterations]\n");
		printf ("  [-p plane] [-f frame] [-g gate] [-b bed] [-SP]\n");
		printf ("  [-z zoom] [-X image_size] [-x x_offset] [-y y_offset]\n");
		printf ("  [-r rotation] [-sumd sumdets] [-ls lsrelax] [-lik] [-v]\n");
		printf ("  [-d decay] [-V version] [-R arc_radius] [-l commandlog]\n\n");
		printf ("nr_iterations:    number of ML-EM iterations,\n");
		printf ("nr_subiterations: number of ordered subsets per ML-EM iteration\n");
		printf ("    When multiple combinations of -i # -j # occur, the sequence\n");
		printf ("    of iteration schemes is applied in the same order.\n");
		printf ("lsrelax: when set to a non-zero value, the EM-algorithm is\n");
		printf ("         NOT used! Instead, a weighted least squares gradient\n");
		printf ("         ascent method is applied, allowing negative values.\n");
		printf ("         Relaxation value lsrelax should be of the order 0.5..2\n");
		printf ("         However, first main iteration is always done with ls=0\n");
		printf ("         for robustness. Only useful when no attenuation corr!\n");
		printf ("");
		printf ("\n");
		printf ("Defaults: attenuation    none   [filename]\n");
		printf ("          normalisation  none   [filename]\n");
		printf ("          iterations     1      [number]\n");
		printf ("          sub_iterations 1      [number]\n");
		printf ("          plane          1\n");
		printf ("          frame          1\n");
		printf ("          gate           1\n");
		printf ("          bed            0\n");
		printf ("          zoom           1\n");
		printf ("          image size   128     [pixels]\n");
		printf ("          x offset       0     [cm]\n");
		printf ("          y offset       0     [cm]\n");
		printf ("          rotation       0     [degrees]\n");
		printf ("          decay          y\n");
		printf ("          sumdets        1    [nr of detectors to be summed]\n");
		printf ("          lsrelax        0    [least squares relaxation]\n");
		printf ("          commandlog     none  [filename]\n");
		printf ("          version        version of the scan file (6,70)\n");
		printf ("          arc_radius     0      (effective radius in cm)\n");
		printf ("               0 = no arccor, Ecat931: 52, Ecat962: 42.2 (includes 1 cm DOI)\n");
		printf ("  -SP     single plane output: output goes to 1,1,1,0,0\n");
		printf ("  -v      verbose\n");
		printf ("  -lik    compute and show likelihood (only if -v is set)\n");
		exit (0);
	}
	if (n_iter == -1)
		n_iter = 1;
	else
		n_iter++;

	if ((strncmp (normalisation_name, "NONE", 4) == 0) ||
	    (strncmp (normalisation_name, "none", 4) == 0))
		norm = 0;
	else
		norm = 1;

	if ((strncmp (attenuation_name, "NONE", 4) == 0) ||
	    (strncmp (attenuation_name, "none", 4) == 0))
		attn = 0;
	else
		attn = 1;

	calclik = calclik && verbose;

	/* Generate command line string */

	if (strcmp (commandlogname, "") != 0) {
		strcpy (commandline, "");
		for (i = 0; i < argc; i++) {
			strcat (commandline, argv[i]);
			if (i < argc - 1)
				strcat (commandline, " ");
		}
		logfile = fopen (commandlogname, "w");
		if (logfile == NULL) {
			printf ("WARNING: Unable to create the file %s\n", commandlogname);
		} else {
			fprintf (logfile, "%s\n", commandline);
			fclose (logfile);
		}
	}
	/*********************/
	/* Open all the files */
	/*********************/

	/* Open scan file */
	/******************/
	if (verbose)
		printf ("Reading %s...\n", sinogram_name);
	sinogram_file = matrix_open (sinogram_name, MAT_READ_ONLY, Sinogram);

	if (!sinogram_file) {
		if (verbose)
			printf ("Not a 2D sinogram, opening as 3D sinogram...\n");
		sinogram_file = matrix_open (sinogram_name, MAT_READ_ONLY, Short3dSinogram);
		if (!sinogram_file) {
			printf ("Problems opening %s as 2D or 3D sinogram\n", sinogram_name);
			exit (0);
		}
		is3dsino = 1;
	} else {
		is3dsino = 0;
	}

	if (verbose)
		printf ("Sinogram version: %d\n", sinogram_file->mhptr->sw_version);

	/* Compute matnum */

	if (verbose)
		printf ("plane %d, frame %d, gate %d, bed %d\n", plane, frame, gate, bed);
	matnum = mat_numcod (frame, plane, gate, 0, bed);
	matnum_atn = mat_numcod (1, plane, 1, 0, bed);

	/* Read the sinogram */

	if (is3dsino) {

		/* The 2D sinogram is stored in a 3D file format. It is read by opening the entire
		 * volume with matrix_read, and then reading the desired slice with matrix_read_slice.
		 * The segment is set to 0, since only the 2D data are supposed to be available.
		 */
		matnum_vol = mat_numcod (frame, 1, gate, 0, bed);
		sinovolume = matrix_read (sinogram_file, matnum_vol, MAT_SUB_HEADER);
		sinomatrix = matrix_read_slice (sinogram_file, sinovolume, plane - 1, 0);
	} else
		sinomatrix = matrix_read (sinogram_file, matnum, UnknownMatDataType);

	if (sinomatrix == NULL) {
		printf ("Unable to find specified matrix\n");
		exit (0);
	}
	init_bed_pos = sinogram_file->mhptr->init_bed_position;

	if (is3dsino) {
		scn3d_sub = (Scan3D_subheader *) sinovolume->shptr;
		loss_correction = scn3d_sub->loss_correction_fctr;
		nrdet = scn3d_sub->num_r_elements;
		nrproj = scn3d_sub->num_angles;
		nplanes = scn3d_sub->num_z_elements[0];
		frame_duration = (float) scn3d_sub->frame_duration;
		frame_starttime = (float) scn3d_sub->frame_start_time;
		pixelsizecm = scn3d_sub->x_resolution;
		data_type = scn3d_sub->data_type;
		gate_duration = scn3d_sub->gate_duration;
		r_wave_offset = scn3d_sub->r_wave_offset;
		num_accepted_beats = scn3d_sub->num_accepted_beats;
	} else {
		scn_sub = (Scan_subheader *) sinomatrix->shptr;
		loss_correction = scn_sub->loss_correction_fctr;
		nrdet = scn_sub->num_r_elements;
		nrproj = scn_sub->num_angles;
		nplanes = sinogram_file->mhptr->num_planes;
		frame_duration = (float) scn_sub->frame_duration;
		frame_starttime = (float) scn_sub->frame_start_time;
		pixelsizecm = scn_sub->x_resolution;
		data_type = scn_sub->data_type;
		gate_duration = scn_sub->gate_duration;
		r_wave_offset = scn_sub->r_wave_offset;
		num_accepted_beats = scn_sub->num_accepted_beats;
	}

	if (verbose) {
		printf ("loss_correction = %g\n", loss_correction);
		printf ("frame_duration = %g\n", frame_duration);
		printf ("pixelsize = %g cm\n", pixelsizecm);
	}
	sinogram_fl = NCread_mat_float (sinomatrix, loss_correction);
	half_life = sinogram_file->mhptr->isotope_halflife;
	arc_radius = arc_radius / pixelsizecm;
	if (verbose) {
		printf ("sinogram size: %d detectors x %d angles\n", nrdet, nrproj);
		printf ("Using arc-radius of %g pixels\n", arc_radius);
	}

	/* Create or open the image file Create a prototype mainheader to do so. 
	 * If no version is specified, the same version and datatype as that of the sinogram are used. 
	 * If a version is specified, the datatype is adapted accordingly. 
	 * All version equal or below 5 are the same and are set to 5. 
	 */
	memcpy (&proto, sinogram_file->mhptr, sizeof (Main_header));
	proto.file_type = PetImage;		 /* image   */
	if (version != 0) {
		if ((version > 0) && (version < 6)) {
			proto.sw_version = 5;
			data_type = VAX_I2;
		} else {
			if (version == 6) {
				proto.sw_version = 6;
				data_type = SUN_I2;
			} else {
				proto.sw_version = V7;
				data_type = IeeeFloat;
			}
		}
	}
	version = proto.sw_version;
	if (version >= V7)
		proto.file_type = PetVolume;
	if (verbose) {
		printf ("Write in software version %d, datatype ", version);
		if (data_type == VAX_I2)
			printf ("VAX_I2\n");
		if (data_type == SUN_I2)
			printf ("Sun short int\n");
		if (data_type == IeeeFloat)
			printf ("Sun float\n");
	}

	/* Opening or creating the output file */

	if (verbose)
		printf ("Opening %s as PetImage...\n", image_name);
	image_file = matrix_open (image_name, MAT_READ_WRITE, PetImage);
	if (!image_file) {
		if (verbose)
			printf ("Opening %s as PetVolume...\n", image_name);
		image_file = matrix_open (image_name, MAT_READ_WRITE, PetVolume);
	}
	if (!image_file) {
		if (verbose)
			printf ("Creating %s...\n", image_name);
		image_file = matrix_create (image_name, MAT_OPEN_EXISTING, &proto);
		if (!image_file) {
			printf ("Error creating or opening image file %s\n", image_name);
			exit (0);
		}
	}

	/* Open and read normalization file */

	if (norm) {
		norm_fl = read_norm_sinogram (normalisation_name, sinogram_name, is3dsino,
					      plane, frame, gate, bed, nrdet, nrproj, verbose);
		if (norm_fl == NULL) {
			printf ("Unable to read the normalisation file %s\n", normalisation_name);
			exit (0);
		}
	}

	/* Open and read attenuation file */

	if (attn) {
		if (verbose)
			printf ("Reading %s...\n", attenuation_name);
		atten_file = matrix_open (attenuation_name, MAT_READ_ONLY, AttenCor);
		if (!atten_file) {
			printf ("Problems opening %s\n", attenuation_name);
			exit (0);
		}
		if (fabs (atten_file->mhptr->init_bed_position - init_bed_pos)
		    > SMALL_Z_DIFFERENCE) {
			printf ("different initial bed position: attenuation: %g\n",
				atten_file->mhptr->init_bed_position);
			printf ("                                sinogram:    %g\n",
				init_bed_pos);
			exit (0);
		}
		isatn70 = atten_file->mhptr->sw_version >= 70;

		if (isatn70) {
			matnum_vol = mat_numcod (1, 1, 1, 0, bed);
			atnvolume = matrix_read (atten_file, matnum_vol, MAT_SUB_HEADER);
			attenmatrix = matrix_read_slice (atten_file, atnvolume, plane - 1, 0);
		} else {
			attenmatrix = matrix_read (atten_file, matnum_atn, UnknownMatDataType);
		}
		if (!attenmatrix) {
			printf ("Cannot read from attenuation file %s\n", attenuation_name);
			exit (0);
		}
		atten_fl = NCread_mat_float (attenmatrix, 1.0);
	}

	/* Allocate memory for sensitivity matrix.
	 * Sensitivity is computed as 1/attenuation_correction/normalisation_correction. 
	 * By using this approach, the statistics will be closer to Poisson than when precorrection is applied. 
	 */
	if (attn || norm) {
		sensit = (float *) calloc (nrdet * nrproj, sizeof (float));
		if (!sensit) {
			printf ("Problems allocating memory for sensitivity matrix\n");
			exit (0);
		}
		p_sensit = sensit - 1;
		p_atten = atten_fl;
		p_norm = norm_fl;

		for (i = 0; i < nrdet * nrproj; i++) {
			*++p_sensit = 1.0;
			if (attn)
				*p_sensit /= *p_atten++;
			if (norm)
				*p_sensit /= *p_norm++;
		}
		if (attn)
			free (atten_fl);
		if (norm)
			free (norm_fl);
	}

	/* Apply sum-detectors: adding adjecent detectors to reduce sinogram size and hence computation time */

	if (sumdet > 1) {
		if (verbose)
			printf ("Summing sinogram %d neighbor pixels\n", sumdet);

		sum_detectors (&sensit, &sinogram_fl, sumdet, nrproj,
			       &nrdet, &arc_radius, &pixelsizecm);
		if (verbose) {
			printf ("New sinogram size is %d x %d\n", nrdet, nrproj);
			printf ("New pixel size is %g cm\n", pixelsizecm);
			printf ("New arc radius is %g pixels\n", arc_radius);
		}
	}

	/* Compute the center of rotation offset: half a sinogram pixel at full resolution, and of
	 * course smaller if detectors have been summed: 1 / (2 * sumdet) */

	corvalue = 1. / (2. * sumdet);
	if (verbose)
		printf ("Center of rotation offset: %g pixels\n", corvalue);
	cor_offset = (float *) calloc (nrproj, sizeof (float));
	p_cor = cor_offset;
	for (pixel = 0; pixel < nrproj; pixel++)
		*p_cor++ = corvalue;

	/* Compute the interpolation coefficients for projection and backprojection */

	if (verbose)
		printf ("Computing the interpolation coefficients (cpu = %g)...\n", cpu_s ());
	radius = 1.05 * (float) nrdet / 2.0;
	startangle = 0.0;
	deltaangle = M_PI / nrproj;
	projcoeff = NCprojcoeff3 (nrdet, nrproj, startangle, deltaangle, radius,
				  cor_offset, arc_radius);

	/* Compute the decay factor	*/

	if (decay)
		if (half_life == 0) {
			printf ("No half life in the main header!\n");
			exit (0);
		} else {
			decay_factor = exp (log (2.0) * ((frame_starttime + 0.5 * frame_duration)
							 / 1000.) / half_life);
		}
	else
		decay_factor = 1.0;

	/* Allocate the reconstruction image */

	image_fl = (float *) calloc (nrdet * nrdet, sizeof (float));
	if (!image_fl) {
		printf ("Problems allocating memory for reconstruction image\n");
		exit (0);
	}
	if (lsrelaxation == 0.0)
		startvalue = 1.0;
	else
		startvalue = 0.0000001;		 /* Better start with a positive step */

	/* Set circular FOV to small positive value	*/

	p_image_fl = image_fl;
	center = (nrdet - 1.0) / 2.0;
	radkwad = center * center;
	for (col = 0; col < nrdet; col++)
		for (row = 0; row < nrdet; row++) {
			if (((col - center) * (col - center) + (row - center) * (row - center))
			    < radkwad)
				*p_image_fl++ = startvalue;
			else
				*p_image_fl++ = 0.0;
		}

	/* ##################### # ML-EM with SUBSETS # #####################	*/

	if (lsrelaxation == 0.0) {
		if (verbose)
			printf ("Starting Maximum Likelihood Expectation Maximization\n");

		/* Apply ML-OSEM as often as requested	*/
		
		if (verbose)
			printf ("Starting reconstruction, cpu = %g\n", cpu_s ());
		for (iter = 0; iter < n_iter; iter++)
			if (calclik) {
				NCmlosem (image_fl, sinogram_fl, sensit, iteraties[iter],
					  sub_iteraties[iter], &likelihood, projcoeff, verbose);
			} else {
				NCmlosem (image_fl, sinogram_fl, sensit, iteraties[iter],
					  sub_iteraties[iter], NULL, projcoeff, verbose);
			}
		if (verbose)
			printf ("End of reconstruction, cpu = %g\n", cpu_s ());

		if (sensit != NULL)
			free (sensit);
	}

	/* #################### # LEAST SQUARES with SUBSETS # ##################### */
	else {
		if (verbose)
			printf ("Starting Least Squares Gradient Ascent\n");

		/* Allocate variance sinogram and set equal to measured sinogram */

		sinovar = (float *) calloc (nrdet * nrproj, sizeof (float));
		p_sinovar = sinovar;
		p_sino = sinogram_fl;
		for (i = 0; i < nrdet * nrproj; i++) {
			*p_sinovar = fabs (*p_sino++);
			if (*p_sinovar < 1.0)
				*p_sinovar = 1.;
			p_sinovar++;
		}

		/* Apply LS-OS as often as requested	*/

		if (verbose)
			printf ("Starting reconstruction, cpu = %g\n", cpu_s ());
		iternum = 0;
		for (iter = 0; iter < n_iter; iter++) {
			for (mainiter = 0; mainiter < iteraties[iter]; mainiter++) {

				/* Starting with regular ML is more robust (prevents streaks which only disappear after sufficient iterations) */

				if (iternum == 0)
					relaxation = 0.0;
				else
					relaxation = lsrelaxation;

				if (calclik) {
					NClsosrecon (image_fl, sinogram_fl, sinovar, sensit,
						     1, sub_iteraties[iter], relaxation,
						   &likelihood, 1, &backmlem, projcoeff, verbose);
				} else {
					NClsosrecon (image_fl, sinogram_fl, sinovar, sensit,
						     1, sub_iteraties[iter], relaxation,
						     NULL, 1, &backmlem, projcoeff, verbose);
				}
				iternum++;
			}			 /* mainiter */
		}				 /* iter */
		if (verbose)
			printf ("End of reconstruction, cpu = %g\n", cpu_s ());

		if (sensit != NULL)
			free (sensit);
		if (backmlem != NULL)
			free (backmlem);
		free (sinovar);
	}

	/* Fill in the MatrixData structure for the output image	*/

	imagematrix = (MatrixData *) calloc (1, sizeof (MatrixData));
	memcpy (imagematrix, sinomatrix, sizeof (MatrixData));
	imagematrix->matnum = matnum;
	imagematrix->matfile = image_file;
	imagematrix->mat_type = PetImage;
	imagematrix->data_type = data_type;
	imagematrix->xdim = nrdet;
	imagematrix->ydim = nrdet;

	/* Apply the zoom */

	image_zoom = (float *) calloc (size_image * size_image, sizeof (float));
	globalzoom = apply_zoom (image_zoom, image_fl, pixelsizecm,
				 nrdet, nrdet, size_image, size_image, zoom,
				 x_offset, y_offset, rotation, verbose);

	if (data_type != IeeeFloat) {

		/* Compute the scale factor and convert float to short int, if that is required by the data_type	*/
		
		p_image_zoom = image_zoom - 1;
		image_min = 9e20;
		image_max = 0.0;
		for (i = 0; i < size_image * size_image; i++) {
			if (*++p_image_zoom < image_min)
				image_min = *p_image_zoom;
			if (*p_image_zoom > image_max)
				image_max = *p_image_zoom;
		}
		if (image_max < fabs (image_min))
			image_max = fabs (image_min);
		if (image_max != 0.0)
			cscale = ((float) IMAGE_MAX) / image_max;
		else {
			printf ("Reconstruction is zero value matrix\n");
			cscale = 1.0;
		}

		image_short = (short int *) calloc (size_image * size_image,
						    sizeof (short int));
		p_image_short = image_short;
		p_image_zoom = image_zoom;
		for (i = 0; i < size_image * size_image; i++)
			*p_image_short++ = (short int) (cscale * *p_image_zoom++);
	} else {
		cscale = 1.0;
	}

	/* The same counts are reconstructed in a smaller number of pixels, so the pixel values
	 * have been raised by sumdet*sumdet. */

	sumdet_factor = 1. / ((float) (sumdet * sumdet));
	scale_factor = decay_factor * sumdet_factor / cscale / (frame_duration / 1000.0);

	/* For float files, scale the whole image. Otherwise, put the scalefactor in the header	*/

	if ((data_type == IeeeFloat)) {
		p_image_zoom = image_zoom;
		image_max = *image_zoom;
		image_min = *image_zoom;
		for (i = 0; i < size_image * size_image; i++) {
			*p_image_zoom = scale_factor * *p_image_zoom;
			if (*p_image_zoom > image_max)
				image_max = *p_image_zoom;
			if (*p_image_zoom < image_min)
				image_min = *p_image_zoom;
			p_image_zoom++;
		}
		scale_factor = 1.0;
	}

	/* If singleplane is set, the output should go to a single plane file. */

	if (singleplane) {
		outplane = 1;
		outframe = 1;
		outgate = 1;
		outbed = 0;
		nplanes = 1;
	} else {
		outplane = plane;
		outframe = frame;
		outgate = gate;
		outbed = bed;
	}

	/* Allocate an image sub-header.   Adapt the MatrixData structure to the new, zoomed image.	*/

	img_sub = (Image_subheader *) calloc (1, sizeof (Image_subheader));
	imagematrix->shptr = (caddr_t) img_sub;
	if (data_type == IeeeFloat) {
		imagematrix->data_ptr = (caddr_t) image_zoom;
		imagematrix->data_size = size_image * size_image * sizeof (float);
	} else {
		imagematrix->data_ptr = (caddr_t) image_short;
		imagematrix->data_size = size_image * size_image * 2;
	}
	imagematrix->xdim = size_image;
	imagematrix->ydim = size_image;
	imagematrix->zdim = 1;
	imagematrix->scale_factor = scale_factor;
	imagematrix->pixel_size = pixelsizecm / globalzoom;
	imagematrix->y_size = pixelsizecm / globalzoom;
	imagematrix->data_min = image_min;
	imagematrix->data_max = image_max;

	/* Fill the image sub header */

	img_sub->data_type = data_type;
	img_sub->num_dimensions = 2;
	img_sub->x_dimension = size_image;
	img_sub->y_dimension = size_image;
	if (version <= 6)
		img_sub->z_dimension = 1;
	else
		img_sub->z_dimension = nplanes;
	img_sub->z_offset = 0.0;
	img_sub->x_offset = x_offset;
	img_sub->y_offset = y_offset;
	img_sub->recon_zoom = zoom;
	img_sub->scale_factor = scale_factor;
	img_sub->image_min = (short int) (image_min * cscale);
	img_sub->image_max = (short int) (image_max * cscale);
	printf ("%2d %2d  min = %f  max = %f\n",frame, plane, image_min, image_max);
	img_sub->x_pixel_size = pixelsizecm / globalzoom;
	img_sub->y_pixel_size = pixelsizecm / globalzoom;
	img_sub->z_pixel_size = sinogram_file->mhptr->plane_separation;
	img_sub->frame_duration = frame_duration;
	img_sub->frame_start_time = frame_starttime;
	img_sub->filter_code = 0;
	img_sub->x_resolution = 0.0;
	img_sub->y_resolution = 0.0;
	img_sub->z_resolution = 0.0;
	img_sub->num_r_elements = nrdet;
	img_sub->num_angles = nrproj;
	img_sub->z_rotation_angle = rotation;
	img_sub->decay_corr_fctr = decay_factor;
	img_sub->processing_code = norm + 2 * attn + 256 * (arc_radius != 0.0) + 512 * decay;
	img_sub->gate_duration = gate_duration;
	img_sub->r_wave_offset = r_wave_offset;
	img_sub->num_accepted_beats = num_accepted_beats;
	img_sub->filter_cutoff_frequency = 0;
	img_sub->filter_resolution = 0;
	img_sub->filter_ramp_slope = 0;
	img_sub->filter_order = 0;
	img_sub->filter_scatter_fraction = 0;
	img_sub->filter_scatter_slope = 0;
	strcpy (img_sub->annotation, "MLEM");
	img_sub->mt_1_1 = 0;
	img_sub->mt_1_2 = 0;
	img_sub->mt_1_3 = 0;
	img_sub->mt_2_1 = 0;
	img_sub->mt_2_2 = 0;
	img_sub->mt_2_3 = 0;
	img_sub->mt_3_1 = 0;
	img_sub->mt_3_2 = 0;
	img_sub->mt_3_3 = 0;
	img_sub->rfilter_cutoff = 0;
	img_sub->rfilter_resolution = 0;
	img_sub->rfilter_code = 0;
	img_sub->rfilter_order = 0;
	img_sub->zfilter_cutoff = 0;
	img_sub->zfilter_resolution = 0;
	img_sub->zfilter_code = 0;
	img_sub->zfilter_order = 0;
	img_sub->mt_1_4 = 0;
	img_sub->mt_2_4 = 0;
	img_sub->mt_3_4 = 0;
	img_sub->scatter_type = 0;
	img_sub->recon_type = 0;
	img_sub->recon_views = 0;

	NCmatrix_write_plane (image_file, imagematrix, outplane, outframe, outgate, outbed);
}
