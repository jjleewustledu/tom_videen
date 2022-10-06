/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : Jan. 1998
  Modifications :
* endtopic History

******************************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include <sys/time.h>
#include "matrix.h"
#include "matrix_extra.h"
#include "NChrplus.h"

#define ERROR -1

/*======================================================================*/
void           *convert_the_data (
				                  void *indataptr,
				                  int xdim,
				                  int ydim,
				                  float inscalefactor,
				                  MatrixDataType indtype,
				                  MatrixDataType outdtype,
				                  float realmax,
				                  float scaledmax,
				                  float *imagemin,
				                  float *imagemax,
				                  float *scalefactor,
				                  int verbose)
{
	float          *imgf;
	short          *imgs;
	float          *p_inpf;
	float          *p_outpf;
	short          *p_inps;
	short          *p_outps;
	float           slicemax;
	float           unscale_scale;
	int             npixels;
	int             i;


	*imagemax = 0.0;
	*imagemin = 1e17;
	npixels = xdim * ydim;
	if (verbose)
		printf ("image of %d x %d pixels\n", xdim, ydim);
	switch (indtype) {
	case IeeeFloat:
		switch (outdtype)
			/**************/
		{
		case IeeeFloat:
			/* =========== */
			if (verbose)
				printf ("float --> float\n");
			imgf = (float *) calloc (npixels, sizeof (float));
			p_inpf = (float *) indataptr;
			p_outpf = imgf;
			for (i = 0; i < npixels; i++) {
				if (*p_inpf > *imagemax)
					*imagemax = *p_inpf;
				if (*p_inpf < *imagemin)
					*imagemin = *p_inpf;
				*p_outpf++ = *p_inpf++ * inscalefactor;
			}
			*scalefactor = 1.0;
			return ((void *) imgf);

		case SunShort:
			/* =========== */
			if (verbose)
				printf ("float --> short\n");
			imgs = (short *) calloc (npixels, sizeof (short));
			p_inpf = (float *) indataptr;
			slicemax = *p_inpf;
			for (i = 0; i < npixels; i++) {
				if (*p_inpf > slicemax)
					slicemax = *p_inpf;
				else if ((-*p_inpf) > slicemax)
					slicemax = -*p_inpf;
				p_inpf++;
			}
			slicemax *= inscalefactor;
			if (realmax == 0.0)
				realmax = slicemax;
			else if (slicemax > realmax + 0.0000001) {
				printf ("!!Warning: maximum of |slice| %g is higher than global max %g\n",
					slicemax, realmax);
			}
			p_inpf = (float *) indataptr;
			p_outps = imgs;
			*scalefactor = realmax / scaledmax;
			unscale_scale = inscalefactor / *scalefactor;
			for (i = 0; i < npixels; i++) {
				*p_outps = (short) (*p_inpf++ * unscale_scale + 0.5);
				if (*p_outps > *imagemax)
					*imagemax = *p_outps;
				if (*p_outps < *imagemin)
					*imagemin = *p_outps;
				p_outps++;
			}
			return ((void *) imgs);

		default:
			break;
		}
		break;

	case SunShort:
		switch (outdtype)
			/**************/
		{
		case IeeeFloat:
			/* =========== */
			if (verbose)
				printf ("short --> float\n");
			imgf = (float *) calloc (npixels, sizeof (float));
			p_outpf = imgf;
			p_inps = (short *) indataptr;
			for (i = 0; i < npixels; i++) {
				*p_outpf = inscalefactor * *p_inps++;
				if (*p_outpf > *imagemax)
					*imagemax = *p_outpf;
				if (*p_outpf < *imagemin)
					*imagemin = *p_outpf;
				p_outpf++;
			}
			*scalefactor = 1.0;
			return ((void *) imgf);

		case SunShort:
			/* =========== */
			if (verbose)
				printf ("short --> short\n");
			imgs = (short *) calloc (npixels, sizeof (short));
			p_outps = imgs;
			p_inps = (short *) indataptr;
			slicemax = *p_inps;
			for (i = 0; i < npixels; i++) {
				if (*p_inps > slicemax)
					slicemax = *p_inps;
				else if (-*p_inps > slicemax)
					slicemax = -*p_inps;
				p_inps++;
			}
			slicemax *= inscalefactor;
			if (realmax == 0.0)
				realmax = slicemax;
			else if (slicemax > realmax + 0.0000001) {
				printf ("!!Warning: maximum of |slice| %g is higher than global max %g\n",
					slicemax, realmax);
			}
			*scalefactor = realmax / scaledmax;
			unscale_scale = inscalefactor / *scalefactor;
			p_inps = (short *) indataptr;
			for (i = 0; i < npixels; i++) {
				*p_outps = (short) (*p_inps++ * unscale_scale + 0.5);
				if (*p_outps > *imagemax)
					*imagemax = *p_outps;
				if (*p_outps < *imagemin)
					*imagemin = *p_outps;
				p_outps++;
			}
			return ((void *) imgs);

		default:
			break;
		}
		break;

	default:
		break;
	}

	printf ("Only conversions between IeeeFloat and SunShort are supported\n");
	exit (0);
}

/*======================================================================*/
int             main (argc, argv)
	int             argc;
	char           *argv[];

{
	int             showhelp = 0;
	int             verbose = 0;
	int             inplane = 1;
	int             ingate = 1;
	int             inframe = 1;
	int             indata = 0;
	int             inbed = 0;
	int             outplane = -1;
	int             outgate = -1;
	int             outframe = -1;
	int             outdata = -1;
	int             outbed = -1;
	int             version = 0;
	int             nplanes = 63;
	int             is3d;
	int             dtype = SunShort;
	int             sizeofdtype = 2;
	int             i;
	int             matnum_in, matnum_out, matnum_vol;
	int             reversecode = 0;
	int             rev_planecol = 0;
	float           realmax = 0.0;
	float           scaledmax = 32767.0;
	float           imagemin = 0.0;
	float           imagemax = 0.0;
	float           scalefactor = 0.0;
	char            inputfilename[150];
	char            outputfilename[150];
	MatrixFile     *inputfile = NULL;
	MatrixFile     *outputfile = NULL;
	MatrixData     *inputvolume;
	MatrixData     *inmatrix;
	MatrixData     *outmatrix;
	Main_header     proto;
	Image_subheader *input_sub;
	Image_subheader *output_sub;
	void           *dataptr;

	char            buf_byte;
	char           *left_byte;
	char           *right_byte;
	short int       buf_short;
	short int      *left_short;
	short int      *right_short;
	int             buf_int;
	int            *left_int;
	int            *right_int;
	float           buf_float;
	float          *left_float;
	float          *right_float;
	int             rownum;
	int             colnum;

#define SWITCHCOLS(data_ptr, xdim, ydim, datatype, buffer, left, right) \
for (rownum = 0; rownum < ydim; rownum++) \
  { \
  left = (datatype *) (data_ptr) + rownum * xdim; \
  right = left + xdim-1; \
  for (colnum = 0; colnum < xdim/2; colnum++) \
    { \
    buffer = *left; \
    *left++ = *right; \
    *right-- = buffer; \
    } \
  }

/* Get the arguments
********************/
	strcpy (inputfilename, "");
	strcpy (outputfilename, "");
	i = 1;
	while (i < argc) {
		if (strcmp (argv[i], "-i") == 0)
			strcpy (inputfilename, argv[i + 1]);
		if (strcmp (argv[i], "-o") == 0)
			strcpy (outputfilename, argv[i + 1]);
		if (strcmp (argv[i], "-msi") == 0)
			sscanf (argv[i + 1], "%d,%d,%d,%d,%d", &inframe, &inplane, &ingate,
				&indata, &inbed);
		if (strcmp (argv[i], "-mso") == 0)
			sscanf (argv[i + 1], "%d,%d,%d,%d,%d", &outframe, &outplane, &outgate,
				&outdata, &outbed);
		if (strcmp (argv[i], "-dt") == 0) {
			if (strcmp (argv[i + 1], "s") == 0) {
				dtype = SunShort;
				sizeofdtype = 2;
			} else if (strcmp (argv[i + 1], "f") == 0) {
				dtype = IeeeFloat;
				sizeofdtype = 4;
			} else {
				printf ("Illegal input: <-dt s> or <-dt f> is allowed\n");
				exit (0);
			}
		}
		if (strcmp (argv[i], "-V") == 0)
			sscanf (argv[i + 1], "%d", &version);
		if (strcmp (argv[i], "-rm") == 0)
			sscanf (argv[i + 1], "%g", &realmax);
		if (strcmp (argv[i], "-sm") == 0)
			sscanf (argv[i + 1], "%g", &scaledmax);
		if (strcmp (argv[i], "-np") == 0)
			sscanf (argv[i + 1], "%d", &nplanes);
		if (strcmp (argv[i], "-rev") == 0)
			sscanf (argv[i + 1], "%d", &reversecode);
		if (strcmp (argv[i], "-h") == 0) {
			showhelp = 1;
			i--;
		}
		if (strcmp (argv[i], "-v") == 0) {
			verbose = 1;
			i--;
		}
		i += 2;
	}

	if (outframe < 0)
		outframe = inframe;
	if (outplane < 0)
		outplane = inplane;
	if (outgate < 0)
		outgate = ingate;
	if (outdata < 0)
		outdata = indata;
	if (outbed < 0)
		outbed = inbed;

	showhelp = showhelp || (strcmp (inputfilename, "") == 0)
		|| (strcmp (outputfilename, "") == 0)
		|| (inframe == -1);

/* Show the syntax
*****************/
	if (showhelp) {
		printf ("Syntax: programname -i inputfilename -o outputfilename\n");
		printf ("  -msi frame,plane,gate,date,bed  [-mso frame,plane,gate,data,bed]\n");
		printf ("  [-dt datatype] [-V version] [-v] [-np nplanes]\n");
		printf ("  [-rm realmax] [-sm scaledmax] [-rev rev]\n");
		printf ("\n");
		printf ("-i:    name of image file of version 6 or volume file of version 70\n");
		printf ("-msi:   input file matrix specification: f,p,g,d,b\n");
		printf ("-mso:   output file matrix specification (default same as for input)\n");
		printf ("-dt:      datatype: s = short int (default), f = float\n");
		printf ("-V:       version:  6 or 70 (default same as input file)\n");
		printf ("-np:      number of planes in a single volume (default 63)\n");
		printf ("          ignored when <-V 6> is set\n");
		printf ("-rm:      real maximum of the entire study\n");
		printf ("          Used to scale to a common volume scale factor\n");
		printf ("          Ignored when <-dt f> or <-V 6> is specified.\n");
		printf ("-sm:      Scaled maximum, for short int files. Default 32767\n");
		printf ("          Ignored when <-dt f> is specified.\n");
		printf ("-v:       verbose\n");
		printf ("-rev:     rev == 0, no reversion is done (default)\n");
		printf ("                 1, planes and cols are reversed\n");
		printf ("                 2, planes and cols are reversed if the\n");
		printf ("                    patient orientation is FeetFirst\n");
		exit (0);
	}
/* Open the input file as a PetImage or a PetVolume
****************************************/
	if (verbose)
		printf ("Opening %s as PetImage\n", inputfilename);
	inputfile = matrix_open (inputfilename, MAT_READ_ONLY, PetImage);
	if (!inputfile) {
		is3d = 1;
		if (verbose)
			printf ("Opening %s as PetVolume\n", inputfilename);
		inputfile = matrix_open (inputfilename, MAT_READ_ONLY, PetVolume);
		if (!inputfile) {
			printf ("Unable to open the file %s as an image or a volume\n",
				inputfilename);
			exit (0);
		}
	} else
		is3d = 0;

/* Read the specified plane
**************************/
	matnum_in = mat_numcod (inframe, inplane, ingate, indata, inbed);
	matnum_vol = mat_numcod (inframe, 1, ingate, indata, inbed);

	if (is3d) {
		inputvolume = matrix_read (inputfile, matnum_vol, MAT_SUB_HEADER);
		if (!inputvolume) {
			printf ("Unable to read the volume\n");
			exit (0);
		}
		inmatrix = matrix_read_slice (inputfile, inputvolume, inplane - 1, 0);
		if (!inmatrix) {
			printf ("Unable to read the slice from the volume");
			exit (0);
		}
	} else {
		inmatrix = matrix_read (inputfile, matnum_in, UnknownMatDataType);
		if (!inmatrix) {
			printf ("Unable to read the slice from the image file\n");
			exit (0);
		}
	}
	input_sub = (Image_subheader *) inmatrix->shptr;

/* Check whether the columns and planes must be reversed.
*****************************************************/
	switch (reversecode) {
	case 0:
		rev_planecol = 0;
		break;
	case 1:
		rev_planecol = 1;
		break;
	case 2:
		rev_planecol
			= ((inputfile->mhptr->patient_orientation == FeetFirstProne) ||
			   (inputfile->mhptr->patient_orientation == FeetFirstSupine) ||
			   (inputfile->mhptr->patient_orientation == FeetFirstRight) ||
			   (inputfile->mhptr->patient_orientation == FeetFirstLeft));
		break;
	}

/* Reverse the columns, keeping the rows. In combination with the
 * plane reversal, this results in a 180 degrees rotation about the
 * vertical axis. Typically required for whole body studies of the
 * legs. Data type is preserved.
 **********/
	if (rev_planecol) {
		if (verbose) {
			printf ("Reversing planes: output plane %d becomes %d\n", outplane,
				nplanes - outplane + 1);
			printf ("Reversing columns.\n");
		}
		outplane = nplanes - outplane + 1;

		switch (inmatrix->data_type) {
		case ByteData:			 /* treat as char data */
			SWITCHCOLS (inmatrix->data_ptr, inmatrix->xdim, inmatrix->ydim,
				    char, buf_byte, left_byte, right_byte);
			break;

		case VAX_Ix2:			 /* treat as short int data */
		case SunShort:
			SWITCHCOLS (inmatrix->data_ptr, inmatrix->xdim, inmatrix->ydim,
				    short int, buf_short, left_short, right_short);
			break;

		case VAX_Ix4:			 /* treat as int data */
		case VAX_Rx4:
		case SunLong:
			SWITCHCOLS (inmatrix->data_ptr, inmatrix->xdim, inmatrix->ydim,
				    int, buf_int, left_int, right_int);
			break;

		case IeeeFloat:		 /* treat as float data */
			SWITCHCOLS (inmatrix->data_ptr, inmatrix->xdim, inmatrix->ydim,
				    float, buf_float, left_float, right_float);
			break;
		default:
			break;
		}
	}
/* Check if the output file exists, if not, create it
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
		printf ("Opening %s as PetImage...\n", outputfilename);
	outputfile = matrix_open (outputfilename, MAT_READ_WRITE, PetImage);
	if (!outputfile) {
		if (verbose)
			printf ("Opening %s as PetVolume...\n", outputfilename);
		outputfile = matrix_open (outputfilename, MAT_READ_WRITE, PetVolume);
	}
	if (!outputfile) {
		if (verbose)
			printf ("Creating %s\n", outputfilename);
		outputfile = matrix_create (outputfilename, MAT_OPEN_EXISTING, &proto);
		if (!outputfile) {
			printf ("Error creating or opening image file %s\n", outputfilename);
			exit (0);
		}
	}
/* Make the matrixdata structure.
********************************/
	if (version == 70)
		matnum_out = mat_numcod (outframe, 1, outgate, outdata, outbed);
	else
		matnum_out = mat_numcod (outframe, outplane, outgate, outdata, outbed);

	outmatrix = (MatrixData *) calloc (1, sizeof (MatrixData));
	memcpy (outmatrix, inmatrix, sizeof (MatrixData));
	output_sub = (Image_subheader *) calloc (1, sizeof (Image_subheader));
	memcpy (output_sub, input_sub, sizeof (Image_subheader));

	outmatrix->matnum = matnum_out;
	outmatrix->matfile = outputfile;
	if (version == 70)
		outmatrix->mat_type = PetVolume;
	else
		outmatrix->mat_type = PetImage;
	outmatrix->data_type = dtype;
	outmatrix->shptr = (caddr_t) output_sub;
	outmatrix->data_ptr = (caddr_t) NULL;
	outmatrix->data_size = inmatrix->xdim * inmatrix->ydim * sizeofdtype;
	outmatrix->scale_factor = 1.0;
	outmatrix->data_min = 0.0;
	outmatrix->data_max = 0.0;

/* Convert the data
*******************/
	dataptr = convert_the_data (inmatrix->data_ptr, input_sub->x_dimension,
				    input_sub->y_dimension, input_sub->scale_factor,
				    input_sub->data_type, dtype, realmax, scaledmax,
				    &imagemin, &imagemax, &scalefactor, verbose);
	outmatrix->data_ptr = (caddr_t) dataptr;

	if (verbose)
		printf ("image minimum is %g, maximum is %g, scalefactor is %g\n",
			imagemin, imagemax, scalefactor);

/* Adapt the subheader
*********************/
	if (version < 70)
		nplanes = 1;
	output_sub->data_type = dtype;
	output_sub->scale_factor = scalefactor;
	output_sub->image_min = (short) (imagemin + 0.5);
	output_sub->image_max = (short) (imagemax + 0.5);
	output_sub->z_dimension = nplanes;

	if (verbose) {
		printf ("plane %d, %d cols x %d rows, %d planes in total, datasize = %d bytes\n",
			outplane, output_sub->x_dimension, output_sub->y_dimension,
			output_sub->z_dimension, outmatrix->data_size);
	}
/* Write the plane to the output according to the specs
******************************************/
	if (!NCmatrix_write_plane (outputfile, outmatrix, outplane, outframe, outgate,
				   outbed)) {
		printf ("Unable to write the plane\n");
	}
}
