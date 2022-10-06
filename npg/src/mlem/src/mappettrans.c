/*
 general information:
 Maximum-a-posteriori of PET transmission scan, with optional correction for
 emission contamination.
 The program produces an image of the attenuation, and attenuation correction
 sinograms (.atn or .a file).
 File I/O is based on the software from UCL, Louvain-la-Neuve, Belgium.
 The reconstruction is carried out using the library libNCproj_bproj.a.
*/
/******************************************************************************
* topic History
  Author(s)     : J Nuyts
  Date          : may 1999
  Modifications :
* endtopic History
******************************************************************************/

#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "matrix.h"
#include "matrix_extra.h"
#include "NCproj_bproj.h"
#include "NChrplus.h"

#define   IMAGE_MAX          32767		 /* Max integer value of an image */

typedef struct {
	float           loss_correction;
	int             nrdet;
	int             nrproj;
	int             nplanes;
	float           frame_duration;
	float           frame_starttime;
	float           halflife;
	float           pixelsizecm;
	short           data_type;
	int             gate_duration;
	int             r_wave_offset;
	int             num_accepted_beats;
	time_t          starttime;
	short           ring_difference;
	short           axial_compression;
}               SinoAdmin;

/*****************************************************************************
 * Routine only used for test debugging purposes: dumps an image in a flat float file
 * easily readable with software like idl or matlab. 
 */

void            dump_imgfile (void *img,
			                      char *typestring,
			                      char *filename,
			                      int ncols,
			                      int nrows,
			                      int nplanes)
{
	float          *img_f = NULL;
	int            *img_i = NULL;
	short          *img_s = NULL;
	char           *img_c = NULL;
	char            infname[200];
	int             nrpoints, point;
	FILE           *fptr;

	printf ("Dumping a file %s, %d x %d x %d\n", filename,
		ncols, nrows, nplanes);
	strcpy (infname, filename);
	strcat (infname, ".inf");
	fptr = fopen (infname, "w");
	if (fptr == NULL) {
		printf ("Error: unable to create %s\n", infname);
		return;
	}
	fprintf (fptr, "ncols   := %d\n", ncols);
	fprintf (fptr, "nrows   := %d\n", nrows);
	fprintf (fptr, "nplanes := %d\n", nplanes);
	fprintf (fptr, "type    := %s\n", typestring);
	fclose (fptr);

	fptr = fopen (filename, "wb");
	if (fptr == NULL) {
		printf ("Error: unable to create %s\n", filename);
		return;
	}
	if (strcmp (typestring, "float") == 0)
		img_f = (float *) img;
	else if (strcmp (typestring, "int") == 0)
		img_i = (int *) img;
	else if (strcmp (typestring, "short") == 0)
		img_s = (short *) img;
	else if (strcmp (typestring, "char") == 0)
		img_c = (char *) img;
	else {
		printf ("Error: your typestring %s is illegal\n", typestring);
		return;
	}
	nrpoints = ncols * nrows * nplanes;
	if (img_f != NULL)
		fwrite (img_f++, sizeof (float), nrpoints, fptr);
	else if (img_i != NULL)
		fwrite (img_i++, sizeof (int), nrpoints, fptr);
	else if (img_s != NULL)
		fwrite (img_s++, sizeof (short), nrpoints, fptr);
	else if (img_c != NULL)
		fwrite (img_c++, sizeof (char), nrpoints, fptr);
	fclose (fptr);
}

/*****************************************************************************/
float           cpu_s ()
{
	long            micros;
	float           seconds;

	micros = clock ();
	seconds = micros / 1000000.;
	return (seconds);
}

/****************************************************************************
 * Produces a decay correction factor compensation from decay (possible negative decay) from time
 * of ref_admin to time of decay_admin.
 */
float           decaycorrfactor (SinoAdmin * decay_admin,
				                 SinoAdmin * ref_admin)
{
	float           delay;

	delay = ((float) (decay_admin->starttime - ref_admin->starttime))
		+ ((float) decay_admin->frame_starttime)
		+ ((float) decay_admin->frame_duration) / 2.0
		- ((float) ref_admin->frame_starttime)
		- ((float) ref_admin->frame_duration) / 2.0;

	return ((float) exp (delay / decay_admin->halflife * log (2.0)));
}

/****************************************************************************
 * Reads the sinogram and creates a small structure with useful information about the sinogram
 */
float          *MAPread_sinogram (MatrixFile ** sinofile,
				                  MatrixData ** sinomatrix,
				                  MatrixData ** sinovolume,
				                  SinoAdmin * sinoadmin,
				                  int *is3dsino,
				                  char *name,
				                  int plane,
				                  int frame,
				                  int gate,
				                  int bed,
				                  int verbose)
{
	int             matnum;
	int             matnum_vol;
	Scan_subheader *scn_sub;
	Scan3D_subheader *scn3d_sub;
	float          *sinogram;

	/*
	 * Open scan ******
	 */
	if (*sinofile == NULL) {
		if (verbose)
			printf ("Opening %s...\n", name);

		*sinofile = matrix_open (name, MAT_READ_ONLY, Sinogram);

		if (*sinofile == NULL) {
			if (verbose)
				printf ("Not a 2D sinogram, opening as 3D sinogram...\n");
			*sinofile = matrix_open (name, MAT_READ_ONLY, Short3dSinogram);
			if (*sinofile == NULL) {
				printf ("Problems opening %s as 2D or 3D sinogram\n", name);
				exit (0);
			}
			*is3dsino = 1;
		} else {
			*is3dsino = 0;
		}
	}

	/*
	 * Compute matnum ***************
	 */
	if (verbose)
		printf ("Reading plane %d, frame %d, gate %d, bed %d\n",
			plane, frame, gate, bed);
	matnum = mat_numcod (frame, plane, gate, 0, bed);

	/*
	 * Read the sinogram ******************
	 */
	if (*is3dsino) {
		matnum_vol = mat_numcod (frame, 1, gate, 0, bed);
		if (*sinovolume == NULL)
			*sinovolume = matrix_read (*sinofile, matnum_vol, MAT_SUB_HEADER);
		*sinomatrix = matrix_read_slice (*sinofile, *sinovolume, plane - 1, 0);
	} else
		*sinomatrix = matrix_read (*sinofile, matnum, UnknownMatDataType);

	if (*sinomatrix == NULL) {
		printf ("Unable to find specified matrix\n");
		exit (0);
	}
	if (*is3dsino) {
		scn3d_sub = (Scan3D_subheader *) (*sinovolume)->shptr;
		sinoadmin->loss_correction = scn3d_sub->loss_correction_fctr;
		sinoadmin->nrdet = scn3d_sub->num_r_elements;
		sinoadmin->nrproj = scn3d_sub->num_angles;
		sinoadmin->nplanes = scn3d_sub->num_z_elements[0];
		sinoadmin->frame_duration = (float) scn3d_sub->frame_duration / 1000.0;
		sinoadmin->frame_starttime = (float) scn3d_sub->frame_start_time / 1000.0;
		sinoadmin->pixelsizecm = scn3d_sub->x_resolution;
		sinoadmin->data_type = scn3d_sub->data_type;
		sinoadmin->gate_duration = scn3d_sub->gate_duration;
		sinoadmin->r_wave_offset = scn3d_sub->r_wave_offset;
		sinoadmin->num_accepted_beats = scn3d_sub->num_accepted_beats;
		sinoadmin->ring_difference = scn3d_sub->ring_difference;
		sinoadmin->axial_compression = scn3d_sub->axial_compression;
	} else {
		scn_sub = (Scan_subheader *) (*sinomatrix)->shptr;
		sinoadmin->loss_correction = scn_sub->loss_correction_fctr;
		sinoadmin->nrdet = scn_sub->num_r_elements;
		sinoadmin->nrproj = scn_sub->num_angles;
		sinoadmin->nplanes = (*sinofile)->mhptr->num_planes;
		sinoadmin->frame_duration = (float) scn_sub->frame_duration / 1000.0;
		sinoadmin->frame_starttime = (float) scn_sub->frame_start_time / 1000.0;
		sinoadmin->pixelsizecm = scn_sub->x_resolution;
		sinoadmin->data_type = scn_sub->data_type;
		sinoadmin->gate_duration = scn_sub->gate_duration;
		sinoadmin->r_wave_offset = scn_sub->r_wave_offset;
		sinoadmin->num_accepted_beats = scn_sub->num_accepted_beats;
		sinoadmin->ring_difference = scn_sub->ring_difference;
		sinoadmin->axial_compression = 3;
	}
	sinoadmin->starttime = (*sinofile)->mhptr->scan_start_time;
	sinoadmin->halflife = (*sinofile)->mhptr->isotope_halflife;

	if (verbose) {
		printf ("loss_correction = %g\n", sinoadmin->loss_correction);
		printf ("frame_duration = %g s\n", sinoadmin->frame_duration);
		printf ("pixelsize = %g cm\n", sinoadmin->pixelsizecm);
	}
	sinogram = NCread_mat_float (*sinomatrix, 1.0);
	return (sinogram);
}

/****************************************************************************
 * Rebins the sinogram into smaller pixels by summing neighboring pixels. Reduces the nr of
 * columns (detectors), preserves nr rows (angles). Applied to counts: the mean value is
 * preserved, total counts decreases. This is important: the ballance between prior and likelihood
 * is sensitive to the absolute number of counts in the transmission scan! Preserving the mean
 * seems better to keep the balance constant.
 */
void            sum_detectors (
			                       float **sino,
			                       SinoAdmin * admin,
			                       int sumdet)
{
	float          *smallsino;
	float          *p_smallsino;
	float          *p_sino;
	float           fsumdet;
	int             pixel;
	int             newnrdet;
	int             num;

	if ((((int) (admin->nrdet / sumdet / 2)) * 2 * sumdet) != admin->nrdet) {
		printf ("Error: nr summed detectors should be a divider of the \n");
		printf ("       original number of detectors. In addition, the\n");
		printf ("       resulting number of detectors should be an even number\n");
		exit (0);
	}
	newnrdet = admin->nrdet / sumdet;
	smallsino = (float *) calloc (admin->nrproj * newnrdet, sizeof (float));
	fsumdet = (float) sumdet;
	p_smallsino = smallsino;
	p_sino = *sino;
	for (pixel = 0; pixel < (newnrdet * admin->nrproj); pixel++) {
		*p_smallsino = *p_sino++;
		for (num = 1; num < sumdet; num++)
			*p_smallsino += *p_sino++;
		*p_smallsino++ /= fsumdet;
	}

	admin->pixelsizecm *= sumdet;
	admin->nrdet = newnrdet;
	free (*sino);
	*sino = smallsino;
}

/****************************************************************************
 * Rebins a sinogram into a larger one, by duplicating pixels, followed by convolution with [0.25
 * 0.5 0.25]. Applied to attenuation correction factors, so the mean value is preserved, (the
 * total value increases). Increases number of detectors, preserves number of projection angles.
 */
void            unsum_detectors (
				                 float **sino,
				                 SinoAdmin * admin,
				                 int sumdet)
{
	float          *bigsino;
	float          *p_bigsino;
	float          *p_sino;
	float          *buffer;
	float          *p_buffer;
	int             row, col;
	int             newnrdet;
	int             num;

	newnrdet = admin->nrdet * sumdet;
	bigsino = (float *) calloc (admin->nrproj * newnrdet, sizeof (float));
	buffer = (float *) calloc (newnrdet, sizeof (float));

	p_bigsino = bigsino;
	p_sino = *sino;
	for (row = 0; row < admin->nrproj; row++) {
		p_buffer = buffer;
		for (col = 0; col < admin->nrdet; col++) {
			*p_buffer++ = *p_sino;
			for (num = 1; num < sumdet; num++)
				*p_buffer++ = *p_sino;
			p_sino++;
		}
		p_buffer = buffer;
		*p_bigsino++ = *p_buffer++;
		for (col = 1; col < newnrdet - 1; col++) {
			*p_bigsino++ = 0.25 * *(p_buffer - 1) +
				0.5 * *p_buffer +
				0.25 * *(p_buffer + 1);
			p_buffer++;
		}
		*p_bigsino++ = *p_buffer;
	}

	admin->pixelsizecm /= sumdet;
	admin->nrdet = newnrdet;
	free (*sino);
	free (buffer);
	*sino = bigsino;
}

/****************************************************************************/
void            map_output_image (
				                  float *recon,
				                  int ncols,
				                  int nrows,
				                  Main_header * sino_mhdr,
				                  MatrixData * sinomatrix,
				                  SinoAdmin * sinoadmin,
				                  int version,
				                  int verbose,
				                  char *recon_name,
				                  int frame,
				                  int plane,
				                  int gate,
				                  int bed,
				                  int nplanes,
				                  float globalmax	/* if globalmax == 0 and
									 * version >= 6 then output
									 * image will be float */
)
{
	Main_header     proto;
	float           minval;
	float           maxval;
	float           scale;
	float           scaledvalue;
	short           data_type;
	short          *recon_short;
	short          *p_recon_short;
	float          *p_recon;
	MatrixFile     *recon_file;
	MatrixData     *reconmatrix;
	int             matnum;
	int             pixnum;
	int             npixels;
	Image_subheader *img_sub;


	memcpy (&proto, sino_mhdr, sizeof (Main_header));
	proto.file_type = PetImage;		 /* image   */

	if (version == 0)
		version = proto.sw_version;

	if (version <= 6) {
		proto.sw_version = 6;
		data_type = SUN_I2;
	} else {
		proto.sw_version = V7;
		if (globalmax == 0)
			data_type = IeeeFloat;
		else
			data_type = SUN_I2;
	}

	if (version >= V7)
		proto.file_type = PetVolume;
	if (verbose) {
		printf ("Write images in software version %d, datatype ", version);
		if (data_type == SUN_I2)
			printf ("Sun short int\n");
		if (data_type == IeeeFloat)
			printf ("Sun float\n");
	}
	minval = 0.0;
	maxval = 0.0;
	p_recon = recon;
	npixels = ncols * nrows;
	for (pixnum = 0; pixnum < npixels; pixnum++) {
		if (*p_recon < minval)
			minval = *p_recon;
		if (*p_recon > maxval)
			maxval = *p_recon;
		p_recon++;
	}

/* If data_type is short, find min and max and rescale into short buffer	***/

	if (data_type != IeeeFloat) {
		recon_short = (short *) calloc (npixels, sizeof (short));
		if ((-minval) > maxval)
			maxval = -minval;
		if (maxval == 0)
			maxval = 1.0;
		if ((globalmax == 0) || (version <= 6))
			scale = ((float) IMAGE_MAX) / maxval;
		else
			scale = ((float) IMAGE_MAX) / globalmax;

		p_recon = recon;
		p_recon_short = recon_short;
		for (pixnum = 0; pixnum < npixels; pixnum++) {
			scaledvalue = floor (scale * *p_recon++ + 0.5);
			if (scaledvalue > IMAGE_MAX)
				scaledvalue = IMAGE_MAX;
			*p_recon_short++ = (short int) (scaledvalue);
		}
	} else
		scale = 1.0;

/* Opening or creating the output file ****/

	if (verbose)
		printf ("Opening %s as PetImage...\n", recon_name);
	recon_file = matrix_open (recon_name, MAT_READ_WRITE, PetImage);
	if (!recon_file) {
		if (verbose)
			printf ("Opening %s as PetVolume...\n", recon_name);
		recon_file = matrix_open (recon_name, MAT_READ_WRITE, PetVolume);
	}
	if (!recon_file) {
		if (verbose)
			printf ("Creating %s...\n", recon_name);
		recon_file = matrix_create (recon_name, MAT_OPEN_EXISTING, &proto);
		if (!recon_file) {
			printf ("Error creating or opening image file %s\n", recon_name);
			exit (0);
		}
	}
/* Fill in the MatrixData structure for the output image. ****/

	reconmatrix = (MatrixData *) calloc (1, sizeof (MatrixData));

	memcpy (reconmatrix, sinomatrix, sizeof (MatrixData));

	matnum = mat_numcod (frame, plane, gate, 0, bed);
	reconmatrix->matnum = matnum;
	reconmatrix->matfile = recon_file;
	reconmatrix->mat_type = PetImage;
	reconmatrix->data_type = data_type;
	reconmatrix->xdim = ncols;
	reconmatrix->ydim = nrows;

/* Allocate an image sub-header.
 * Adapt the MatrixData structure to the new, zoomed image ***/

	img_sub = (Image_subheader *) calloc (1, sizeof (Image_subheader));
	reconmatrix->shptr = (caddr_t) img_sub;
	if (data_type == IeeeFloat) {
		reconmatrix->data_ptr = (caddr_t) recon;
		reconmatrix->data_size = npixels * sizeof (float);
	} else {
		reconmatrix->data_ptr = (caddr_t) recon_short;
		reconmatrix->data_size = npixels * 2;
	}
	reconmatrix->xdim = ncols;
	reconmatrix->ydim = nrows;
	reconmatrix->zdim = 1;
	reconmatrix->scale_factor = 1.0 / scale;
	reconmatrix->pixel_size = sinoadmin->pixelsizecm;
	reconmatrix->y_size = sinoadmin->pixelsizecm;
	reconmatrix->data_min = minval;
	reconmatrix->data_max = maxval;

/* Fill the image sub header 	****/

	img_sub->data_type = data_type;
	img_sub->num_dimensions = 2;
	img_sub->x_dimension = ncols;
	img_sub->y_dimension = nrows;
	if (version <= 6)
		img_sub->z_dimension = 1;
	else
		img_sub->z_dimension = nplanes;
	img_sub->z_offset = 0.0;
	img_sub->x_offset = 0.0;
	img_sub->y_offset = 0.0;
	img_sub->recon_zoom = 1.0;
	img_sub->scale_factor = 1.0 / scale;
	img_sub->image_min = (short int) (minval * scale);
	img_sub->image_max = (short int) (maxval * scale);
	img_sub->x_pixel_size = sinoadmin->pixelsizecm;
	img_sub->y_pixel_size = sinoadmin->pixelsizecm;
	img_sub->z_pixel_size = sino_mhdr->plane_separation;
	img_sub->frame_duration = sinoadmin->frame_duration * 1000;
	img_sub->frame_start_time = sinoadmin->frame_starttime * 1000;
	img_sub->filter_code = 0;
	img_sub->x_resolution = 0.0;
	img_sub->y_resolution = 0.0;
	img_sub->z_resolution = 0.0;
	img_sub->num_r_elements = ncols;
	img_sub->num_angles = sinoadmin->nrproj;
	img_sub->z_rotation_angle = 0;
	img_sub->decay_corr_fctr = 1.0;
	img_sub->processing_code = 0.0;
	img_sub->gate_duration = sinoadmin->gate_duration;
	img_sub->r_wave_offset = sinoadmin->r_wave_offset;
	img_sub->num_accepted_beats = sinoadmin->num_accepted_beats;
	img_sub->filter_cutoff_frequency = 0;
	img_sub->filter_resolution = 0;
	img_sub->filter_ramp_slope = 0;
	img_sub->filter_order = 0;
	img_sub->filter_scatter_fraction = 0;
	img_sub->filter_scatter_slope = 0;
	strcpy (img_sub->annotation, "MAP-recon");
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

	NCmatrix_write_plane (recon_file, reconmatrix,
			      plane, frame, gate, bed);

	if (data_type != IeeeFloat)
		free (recon_short);
}

/****************************************************************************/
void            map_output_atten (
				                  float *atten,
				                  int ncols,
				                  int nrows,
				                  Main_header * sino_mhdr,
				                  MatrixData * sinomatrix,
				                  SinoAdmin * sinoadmin,
				                  int version,
				                  int verbose,
				                  char *filename,
				                  int frame,
				                  int plane,
				                  int gate,
				                  int bed,
				                  int nplanes
)
{
	Main_header     proto;
	MatrixFile     *atten_file;
	MatrixData     *attenmatrix;
	int             matnum;
	int             pixnum;
	int             npixels;
	int             i;
	short           data_type;
	Attn_subheader *atten_sub;


	npixels = ncols * nrows;

	memcpy (&proto, sino_mhdr, sizeof (Main_header));
	proto.file_type = AttenCor;		 /* image   */

	if (version == 0)
		version = proto.sw_version;

	if (version <= 6) {
		proto.sw_version = 6;
		data_type = VAX_Rx4;
	} else {
		proto.sw_version = V7;
		data_type = IeeeFloat;
	}



/* Opening or creating the output file
*******************************/
	if (verbose)
		printf ("Opening %s as AttenCor...\n", filename);
	atten_file = matrix_open (filename, MAT_READ_WRITE, AttenCor);
	if (!atten_file) {
		if (verbose)
			printf ("Creating %s...\n", filename);
		atten_file = matrix_create (filename, MAT_OPEN_EXISTING, &proto);
		if (!atten_file) {
			printf ("Error creating or opening image file %s\n", filename);
			exit (0);
		}
	}
/* Fill in the MatrixData structure for the output image.
********************************************************/
	attenmatrix = (MatrixData *) calloc (1, sizeof (MatrixData));

	memcpy (attenmatrix, sinomatrix, sizeof (MatrixData));

	matnum = mat_numcod (frame, plane, gate, 0, bed);
	attenmatrix->matnum = matnum;
	attenmatrix->matfile = atten_file;
	attenmatrix->mat_type = AttenCor;
	attenmatrix->data_type = data_type;
	attenmatrix->xdim = ncols;
	attenmatrix->ydim = nrows;

/* Allocate an image sub-header.
 * Adapt the MatrixData structure to the new, zoomed image
 **********************************************************/
	atten_sub = (Attn_subheader *) calloc (1, sizeof (Attn_subheader));
	attenmatrix->shptr = (caddr_t) atten_sub;
	attenmatrix->data_ptr = (caddr_t) atten;
	attenmatrix->data_size = npixels * sizeof (float);
	attenmatrix->zdim = 1;
	attenmatrix->scale_factor = 1.0;
	attenmatrix->pixel_size = sinoadmin->pixelsizecm;
	attenmatrix->y_size = sinoadmin->pixelsizecm;
	attenmatrix->data_min = 0.0;
	attenmatrix->data_max = 0.0;

/* Fill the atten sub header
***************************/
	atten_sub->data_type = data_type;
	atten_sub->num_dimensions = 2;
	atten_sub->attenuation_type = 1;
	atten_sub->num_r_elements = ncols;
	atten_sub->num_angles = nrows;
	atten_sub->num_z_elements = nplanes;
	atten_sub->ring_difference = sinoadmin->ring_difference;
	atten_sub->x_resolution = sinoadmin->pixelsizecm;
	atten_sub->y_resolution = sinoadmin->pixelsizecm;
	atten_sub->z_resolution = sino_mhdr->plane_separation;
	atten_sub->w_resolution = 0.0;
	atten_sub->scale_factor = 1.0;
	atten_sub->x_offset = 0.0;
	atten_sub->y_offset = 0.0;
	atten_sub->x_radius = 0.0;
	atten_sub->y_radius = 0.0;
	atten_sub->tilt_angle = 0.0;
	atten_sub->attenuation_coeff = 0.0;
	atten_sub->attenuation_min = 0.0;
	atten_sub->attenuation_max = 0.0;
	atten_sub->skull_thickness = 0.0;
	atten_sub->num_additional_atten_coeff = 0;
	for (i = 0; i < 7; i++)
		atten_sub->additional_atten_coeff[i] = 0.0;
	atten_sub->edge_finding_threshold = 0.0;
	atten_sub->storage_order = 0;		 /* view */
	atten_sub->span = sinoadmin->axial_compression;
	atten_sub->z_elements[0] = nplanes;
	for (i = 1; i < 64; i++)
		atten_sub->z_elements[i] = 0;

	NCmatrix_write_plane (atten_file, attenmatrix,
			      plane, frame, gate, bed);

/*
dump_imgfile(attenmatrix->data_ptr, "float", "testatten", ncols, nrows, 1);
*/
}

/****************************************************************************/
float          *map_read_dwell (char *dwell_name,
				                int nrdet)
{
	FILE           *fileptr;
	float          *dwellprofile;
	float          *p_dwell;
	int             point;

	fileptr = fopen (dwell_name, "r");
	if (fileptr == NULL) {
		printf ("Error: unable to open dwell profile file %s\n", dwell_name);
		exit (0);
	}
	dwellprofile = (float *) calloc (nrdet, sizeof (float));
	p_dwell = dwellprofile;
	for (point = 0; point < nrdet; point++) {
		if (fscanf (fileptr, "%f", p_dwell++) != 1) {
			printf ("Unable to read %d points from the dwell profile:\n", nrdet);
			printf ("error reading point %d from %s\n", point, dwell_name);
			exit (0);
		}
	}

	fclose (fileptr);
	return (dwellprofile);
}

/****************************************************************************/
void            sum_planes (
			                    float *sino[],
			                    int nrsmoothplanes,
			                    int nrdet,
			                    int nrproj,
			                    float axialsmooth)
{
	float          *p_sino0;
	float          *p_sino1;
	int             planenum;
	int             nrpixels;
	int             pixel;

	nrpixels = nrdet * nrproj;

	for (planenum = 1; planenum < nrsmoothplanes; planenum++) {
		p_sino0 = sino[0];
		p_sino1 = sino[planenum];
		for (pixel = 0; pixel < nrpixels; pixel++) {
			*p_sino0++ += axialsmooth * *p_sino1++;
		}
	}
}

/****************************************************************************/
float           reverse_planes (
				                float *image,
				                int ncols,
				                int nrows,
				                int plane,
				                int nplanes)
{
	float          *p_left;
	float          *p_right;
	float           buffer;
	int             col;
	int             row;

	for (row = 0; row < nrows; row++) {
		p_left = image + row * ncols;
		p_right = p_left + ncols - 1;
		for (col = 0; col < ncols / 2; col++) {
			buffer = *p_left;
			*p_left++ = *p_right;
			*p_right-- = buffer;
		}
	}

	return (nplanes - plane + 1);
}

/****************************************************************************/
main (argc, argv)
	int             argc;
	char           *argv[];

{
	time_t          timesec;
	struct tm      *timeb;

	p_ProjCoeff     projcoeff = NULL;
	p_AbsPrior      absprior = NULL;
	p_MarkovPrior   markovprior = NULL;

	char            blank_name[150];
	char            trans_name[150];
	char            contam_name[150];
	char            norm_name[150];
	char            recon_name[150];
	char            acf_name[150];
	char            img_name[150];
	char            dwell_name[150];

	float          *image_fl;
	float          *p_image_fl;
	float          *blanksino[3] = {NULL, NULL, NULL};
	float          *transsino[3] = {NULL, NULL, NULL};
	float          *contamsino[3] = {NULL, NULL, NULL};
	float          *transnorm[3] = {NULL, NULL, NULL};
	float          *blanknorm[3] = {NULL, NULL, NULL};
	float          *contamnorm[3] = {NULL, NULL, NULL};
	float          *image_zoom = NULL;
	float          *p_blank;
	float          *p_trans;
	float          *p_contam;
	float          *p_transnorm;
	float          *p_blanknorm;
	float          *p_contamnorm;
	float          *p_image;
	float          *likelihood;
	float          *dwellprofile = NULL;
	float          *p_dwell;
	MatrixFile     *blank_file = NULL;
	MatrixData     *blankvolume = NULL;
	MatrixData     *blankmatrix = NULL;
	MatrixFile     *trans_file = NULL;
	MatrixData     *transvolume = NULL;
	MatrixData     *transmatrix = NULL;
	MatrixFile     *contam_file = NULL;
	MatrixData     *contamvolume = NULL;
	MatrixData     *contammatrix = NULL;

	MatrixData     *reconmatrix;
	Image_subheader *recon_sub;

	SinoAdmin      *transadmin[3];
	SinoAdmin      *blankadmin[3];
	SinoAdmin      *contamadmin[3];

	float           pixelsizecm;
	float           zoom = 1.0;
	float           globalzoom;
	float           x_offset = 0.0;
	float           y_offset = 0.0;
	float           rotation = 0.0;
	int             size_image = 128;
	int             reverseplane = 0;
	int             revplanecode = 2;

	float           contamweight = 0.0;
	float           startangle, deltaangle;
	float           factor;
	float           arc_radius = 0.0;
	float           radius;
	float           relaxation = 1.0;
	float           mutissue_cm = 0.095;
	float           mutissue;
	float           center, radkwad;
	float           decaycorr;
	float          *cor_offset;		 /* center of rotation offset: 0.5 pixels */
	float          *p_cor;
	float           corvalue;


	int             singleplane = 0;
	int             contamframe = 1;
	int             contambed = -1;
	int             i, num;
	int             col, row;
	int             help = 0;
	int             n_iter = -1;
	int             iter;
	int             curiter, mainiter, totiter;
	int             iteraties[20];
	int             sub_iteraties[20];
	int             planenum;
	int             plane = 1;
	int             frame = 1;
	int             gate = 1;
	int             bed = 0;
	int             verbose = 0;
	int             is3dsino = 0;
	int             sumdet = 1;
	int             calclik = 0;
	int             matnum, matnum_vol, matnum_blank, matnum_blankvol;
	int             nrdet, unbinnednrdet, nrproj;
	int             pixel, projnum;
	int             version = 0;

	int             curprior;
	int             priormodes = 0;
	float           axialsmooth = 0.0;
	int             nrsmoothplanes = 1;
	int             smoothplanes[3];
	float           priorweight = 1.0;
	float           priormean[20];
	float           priorstd[20];
	float           priorwidth[20];
	float           globalmaxmu = 0.095 * 10.0;

	int             markovneighborhood = 4;
	float           markovdelta = 0.095 * 0.15;	/* 15% of tissue */
	float           markovweight = 1.0;
	char            markovfunction = 'n';

	strcpy (blank_name, "");
	strcpy (trans_name, "");
	strcpy (contam_name, "");
	strcpy (norm_name, "");
	strcpy (recon_name, "");
	strcpy (acf_name, "");
	strcpy (img_name, "");
	strcpy (dwell_name, "");

	i = 1;
	while (i < argc) {
		if (strcmp (argv[i], "-bl") == 0)
			strcpy (blank_name, argv[i + 1]);
		else if (strcmp (argv[i], "-tr") == 0)
			strcpy (trans_name, argv[i + 1]);
		else if (strcmp (argv[i], "-n") == 0)
			strcpy (norm_name, argv[i + 1]);
		else if (strcmp (argv[i], "-orecon") == 0)
			strcpy (recon_name, argv[i + 1]);
		else if (strcmp (argv[i], "-oacf") == 0)
			strcpy (acf_name, argv[i + 1]);
		else if (strcmp (argv[i], "-oimg") == 0)
			strcpy (img_name, argv[i + 1]);
		else if (strcmp (argv[i], "-c") == 0)
			strcpy (contam_name, argv[i + 1]);
		else if (strcmp (argv[i], "-cw") == 0)
			sscanf (argv[i + 1], "%f", &contamweight);
		else if (strcmp (argv[i], "-cd") == 0)
			strcpy (dwell_name, argv[i + 1]);
		else if (strcmp (argv[i], "-cf") == 0)
			sscanf (argv[i + 1], "%d", &contamframe);
		else if (strcmp (argv[i], "-cb") == 0)
			sscanf (argv[i + 1], "%d", &contambed);
		else if (strcmp (argv[i], "-i") == 0) {
			n_iter++;
			sscanf (argv[i + 1], "%d", &iteraties[n_iter]);
		} else if (strcmp (argv[i], "-V") == 0)
			sscanf (argv[i + 1], "%d", &version);
		else if (strcmp (argv[i], "-j") == 0)
			sscanf (argv[i + 1], "%d", &sub_iteraties[n_iter]);
		else if (strcmp (argv[i], "-p") == 0)
			sscanf (argv[i + 1], "%d", &plane);
		else if (strcmp (argv[i], "-f") == 0)
			sscanf (argv[i + 1], "%d", &frame);
		else if (strcmp (argv[i], "-g") == 0)
			sscanf (argv[i + 1], "%d", &gate);
		else if (strcmp (argv[i], "-b") == 0)
			sscanf (argv[i + 1], "%d", &bed);
		else if (strcmp (argv[i], "-sumd") == 0)
			sscanf (argv[i + 1], "%d", &sumdet);
		else if (strcmp (argv[i], "-R") == 0)
			sscanf (argv[i + 1], "%f", &arc_radius);
		else if (strcmp (argv[i], "-rel") == 0)
			sscanf (argv[i + 1], "%f", &relaxation);
		else if (strcmp (argv[i], "-priorw") == 0)
			sscanf (argv[i + 1], "%f", &priorweight);
		else if (strcmp (argv[i], "-z") == 0)
			sscanf (argv[i + 1], "%f", &zoom);
		else if (strcmp (argv[i], "-X") == 0)
			sscanf (argv[i + 1], "%d", &size_image);
		else if (strcmp (argv[i], "-x") == 0)
			sscanf (argv[i + 1], "%f", &x_offset);
		else if (strcmp (argv[i], "-y") == 0)
			sscanf (argv[i + 1], "%f", &y_offset);
		else if (strcmp (argv[i], "-r") == 0)
			sscanf (argv[i + 1], "%f", &rotation);
		else if (strcmp (argv[i], "-rev") == 0)
			sscanf (argv[i + 1], "%d", &revplanecode);
		else if (strcmp (argv[i], "-prior") == 0) {
			sscanf (argv[i + 1], "%f", priormean + priormodes);
			sscanf (argv[i + 2], "%f", priorstd + priormodes);
			sscanf (argv[i + 3], "%f", priorwidth + priormodes);
			i += 2;
			priormodes++;
		} else if (strcmp (argv[i], "-markovf") == 0)
			sscanf (argv[i + 1], "%c", &markovfunction);
		else if (strcmp (argv[i], "-markovd") == 0)
			sscanf (argv[i + 1], "%f", &markovdelta);
		else if (strcmp (argv[i], "-markovw") == 0)
			sscanf (argv[i + 1], "%f", &markovweight);
		else if (strcmp (argv[i], "-markovn") == 0)
			sscanf (argv[i + 1], "%d", &markovneighborhood);
		else if (strcmp (argv[i], "-sa") == 0) {
			sscanf (argv[i + 1], "%f", &axialsmooth);
		} else if (strcmp (argv[i], "-lik") == 0) {
			i--;
			calclik = 1;
		} else if (strcmp (argv[i], "-h") == 0) {
			i--;
			help = 1;
		} else if (strcmp (argv[i], "-v") == 0) {
			i--;
			verbose = 1;
		} else if (strcmp (argv[i], "-SP") == 0) {
			i--;
			singleplane = 1;
		} else {
			printf ("illegal parameter %s\n", argv[i]);
			exit (0);
		}
		i += 2;
	}

	calclik = calclik && verbose;

	help = help || (strcmp (trans_name, "") == 0);

	if (help) {
		printf ("Syntax: program_name -bl blank  -tr trans [-n norm]\n");
		printf ("  [-orecon recon] [-oacf acf] [-oimg img]\n");
		printf ("  [-c contamination -cd dwellprofile -cw contamweight]\n");
		printf ("  [-cf contamframe] [-cb contambed]]\n");
		printf ("  [-i nr_iterations -j subiterations]\n");
		printf ("  [-sa axialsmooth] [-V version] [-SP]\n");
		printf ("  [-p plane] [-f frame] [-g gate] [-b bed]\n");
		printf ("  [-R arc_radius] [-sumd sumdets] [-rel rel] [-v] [-lik]\n");
		printf ("  [-z zoom] [-X image_size] [-x x_offset] [-y y_offset]\n");
		printf ("  [-r rotation] [-rev reversecode]\n");
		printf ("  [-prior priormean priorstd priorwidth] [-prior ...]\n");
		printf ("  [-priorw priorweight]\n");
		printf ("  [-markovf function] [-markovd delta] [-markovw weight]\n");
		printf ("  [-markovn neighborhood]\n");
		printf ("\n");
		printf ("* Normalization only required if sinogram dependent,\n");
		printf ("  e.g.: not required for ECAT931, required for HR+\n");
		printf ("* Recon: the ``raw'' attenuation image: lin.att.coeff. per pixel\n");
		printf ("         useful if you want to reproject with other software.\n");
		printf ("* ACF: attenuation correction factors: .a or .atn file\n");
		printf ("* img: image output file, containing lin.att.coeff per cm\n");
		printf ("       This image can be zoomed, rotated and translated\n");
		printf ("       and its size adapted, similar to mlempetv7 reconstr.\n");
		printf ("       Allows exact alignment with corresponding emission img.\n");
		printf ("       rotation in degrees, offset in cm.\n");
		printf ("* Contamination: sinogram estimating an undesired contribution\n");
		printf ("                 e.g. in post-inj transmission\n");
		printf ("  Dwellprofile: ascii file with one value per line, representing\n");
		printf ("                the detector sensitivity during windowing.\n");
		printf ("  Contamweight: weight for the estimate. (Mean dwell profile value)\n");
		printf ("        For the HR+ we found a weight of 0.03 in the center.\n");
		printf ("  If -c is present, either -cd or -cw should be supplied as well\n");
		printf ("  Contamframe: allows to select frame from dyn. emission scan\n");
		printf ("* The sequence <-i nriter -j nrsubiter> may be repeated.\n");
		printf ("* -sa axialsmooth: raw data of 3 planes are smoothed with\n");
		printf ("       [axialsmooth, 1, axialsmooth]. Default 0: no smooth.\n");
		printf ("* Version: 6 or 70\n");
		printf ("* -SP:     when set, output files will contain 1 plane only\n");
		printf ("* Arc-correction is in cm. Use 52 for ECAT931, 42.2 for HR+\n");
		printf ("  These values include 1 cm depth of interaction\n");
		printf ("* sumdets: the number of detectors to be added (reduce sampling)\n");
		printf ("* With relaxation = 1, convergence is guaranteed\n");
		printf ("  Higher values may accelerate, but oscillations might occur!\n");
		printf ("* -v:      verbose\n");
		printf ("* -lik:    compute the likelihood (significant cpu!)\n");
		printf ("* -z, -X, -x, -y, -r: see img\n");
		printf ("* -rev reversecode:  -rev 0: no plane reversal\n");
		printf ("  -rev 1: reverse planes, -rev 2: reverse planes if FeetFirst\n");
		printf ("* -prior: specify absolute prior values:\n");
		printf ("     priormean: absolute value in 1/cm,\n");
		printf ("     priorstd:  standard deviation in 1/cm, determines strenght\n");
		printf ("     priorwidth: relative range,\n");
		printf ("     priorweight: weight, default 1.\n");
		printf ("  -markovf: g = Geman, h = Huber, q = Quadratic\n");
		printf ("  -markovd: delta: standard deviation (or similar), parameter\n");
		printf ("            of the energy function (Geman, Huber or Quad.) [1/cm]\n");
		printf ("  -markovw: weight, default 1. If delta is well chosen, value\n");
		printf ("            should be in the order of 0.1 - 10.\n");
		printf ("  -markovn: 4 or 8. Our experience is that 4-neighborhood works\n");
		printf ("            fine. Using 8 neighbors requires more cpu.\n");
		printf ("\n");
		printf ("* Defaults:  plane, frame, gate, bed = 1, 1, 1, 0\n");
		printf ("             contamframe = 1, contambed = bed");
		printf ("             sumdets     = 1\n");
		printf ("             rel         = 1\n");
		printf ("             arc_radius  = 0 (= no arc-correction)\n");
		printf ("             version     = version of the scan file (6,70)\n");
		printf ("             -z 1 -X 128 -x 0 -y 0 -r 0 -rev 2\n");
	}
	if (n_iter == -1) {
		printf ("Please specify the number of iterations\n");
		exit (0);
	} else
		n_iter++;

	if (contambed < 0)
		contambed = bed;
	if (strcmp (contam_name, "") != 0) {
		if ((contamweight == 0.0) && (dwell_name[0] == '\0')) {
			printf ("Please specify also the contamination weight: -cw contamweight\n");
			printf ("or the name of the dwell profile file: -cd dwellprofile-name\n");
			exit (0);
		}
	}
	smoothplanes[0] = plane;

/**********************/
/* Read the sinograms */
/**********************/
/* Blank scan
********/
	blankadmin[0] = (SinoAdmin *) calloc (1, sizeof (SinoAdmin));
	blanksino[0] = MAPread_sinogram (&blank_file, &blankmatrix, &blankvolume,
					 blankadmin[0], &is3dsino, blank_name,
					 plane, 1, 1, 0, verbose);
	if (blanksino[0] == NULL) {
		printf ("Memory allocation problem (or so) for blank scan\n");
		exit (0);
	}
	nrdet = blankadmin[0]->nrdet;
	nrproj = blankadmin[0]->nrproj;
	if (verbose)
		printf ("Sinogram size is %d x %d\n", nrdet, nrproj);

	if (axialsmooth > 0.0) {
		if (verbose)
			printf ("Axial smoothing: combining planes %d", plane);
		if (plane > 1) {
			smoothplanes[nrsmoothplanes] = plane - 1;
			if (verbose)
				printf (" and %d", smoothplanes[nrsmoothplanes]);
			nrsmoothplanes++;
		}
		if (plane < (blankadmin[0]->nplanes - 1)) {
			smoothplanes[nrsmoothplanes] = plane + 1;
			if (verbose)
				printf (" and %d", smoothplanes[nrsmoothplanes]);
			nrsmoothplanes++;
		}
		if (verbose)
			printf ("\n");
	}
	for (planenum = 1; planenum < nrsmoothplanes; planenum++) {
		blankadmin[planenum] = (SinoAdmin *) calloc (1, sizeof (SinoAdmin));
		blanksino[planenum] = MAPread_sinogram (&blank_file, &blankmatrix,
							&blankvolume, blankadmin[planenum],
							&is3dsino, blank_name,
							smoothplanes[planenum], 1, 1, 0, verbose);
		if (blanksino[planenum] == NULL) {
			printf ("Memory allocation problem (or so) for blank scan\n");
			exit (0);
		}
	}


/* Transmission scan
********/
	for (planenum = 0; planenum < nrsmoothplanes; planenum++) {
		transadmin[planenum] = (SinoAdmin *) calloc (1, sizeof (SinoAdmin));
		transsino[planenum] = MAPread_sinogram (&trans_file, &transmatrix,
						    &transvolume, transadmin[planenum], &is3dsino,
							trans_name, smoothplanes[planenum],
							frame, gate, bed, verbose);
		if (transsino[planenum] == NULL) {
			printf ("Memory allocation problem (or so) for transmission scan\n");
			exit (0);
		}
		if ((nrdet != transadmin[planenum]->nrdet)
		    || (nrproj != transadmin[planenum]->nrproj)) {
			printf ("Blank and transmission should have same dimensions\n");
			exit (0);
		}
	}

/* contamination scan
********/
	if (contam_name[0] != '\0') {
		for (planenum = 0; planenum < nrsmoothplanes; planenum++) {
			contamadmin[planenum] = (SinoAdmin *) calloc (1, sizeof (SinoAdmin));
			contamsino[planenum] = MAPread_sinogram (&contam_file, &contammatrix,
							     &contamvolume, contamadmin[planenum],
						   &is3dsino, contam_name, smoothplanes[planenum],
							   contamframe, gate, contambed, verbose);
			if (contamsino[planenum] == NULL) {
				printf ("Memory allocation problem (or so) for contammission scan\n");
				exit (0);
			}
			if ((nrdet != contamadmin[planenum]->nrdet)
			    || (nrproj != contamadmin[planenum]->nrproj)) {
				printf ("Blank and contamination sinogram should have same dimensions\n");
				exit (0);
			}
			if (dwell_name[0] != '\0') {
				contamweight = 1.0;
				if (verbose)
					printf ("Multiplying contamination estimate with dwell profile\n");

				if (planenum == 0) {
					dwellprofile = map_read_dwell (dwell_name, nrdet);
				}
				p_contam = contamsino[planenum];
				for (projnum = 0; projnum < nrproj; projnum++) {
					p_dwell = dwellprofile;
					for (pixel = 0; pixel < nrdet; pixel++) {
						*p_contam++ *= *p_dwell++;
					}	 /* pixel */
				}		 /* projnum */
			}			 /* dwell_name */
		}				 /* planenum */
	}					 /* contam_name */

	/*
	 * Read normalizations *******
	 */
	if (norm_name[0] != '\0') {
		if (is3dsino) {
			if (verbose) {
				printf ("------- N o r m a l i z a t i o n....begin  ----------\n");
				printf ("Reading normalization for blank scan\n");
			}
			for (planenum = 0; planenum < nrsmoothplanes; planenum++) {
				blanknorm[planenum] = NCreadnormv7 (blank_name, norm_name,
								  smoothplanes[planenum], 1, 1, 0,
								    verbose, 1, 1, 1);
				if (verbose)
					printf ("Reading normalization for transmission scan\n");
				transnorm[planenum] = NCreadnormv7 (trans_name, norm_name,
							      smoothplanes[planenum], frame, gate,
								    bed, verbose, 1, 1, 1);
				if (contamsino[0] != NULL) {
					if (verbose)
						printf ("Reading normalization for contamination scan\n");
					contamnorm[planenum] = NCreadnormv7 (contam_name, norm_name,
							      smoothplanes[planenum], contamframe,
							       gate, contambed, verbose, 1, 1, 1);
				}
			}
			if (verbose)
				printf ("------- N o r m a l i z a t i o n....end  ----------\n");
		} else {
			printf ("No need to read the sinogram independent normalization\n");
		}
	} else if (is3dsino) {
		printf ("\nWarning: %c", 7);
		printf ("For newer camera's the normalization is sinogram dependent\n");
		printf ("and must not be ignored. Please specify a normalization file\n\n");
	}
/****************************************************/
/* Apply correction to blank scan and emission scan */
/****************************************************/
/* Correct for difference in frame duration trans <==> blank
 * and for difference in loss correction. Correction for transmission
 * are applied to the blank, to preserve noise in transmission.
 *=========================================================*/
	for (planenum = 0; planenum < nrsmoothplanes; planenum++) {
		factor = transadmin[planenum]->frame_duration
			/ blankadmin[planenum]->frame_duration;
		if (verbose)
			printf ("Blank/trans: Frame duration ratio: %g\n", factor);
		factor *= blankadmin[planenum]->loss_correction
			/ transadmin[planenum]->loss_correction;
		if (verbose)
			printf ("Frame duration ratio x loss-corr. ratio: %g\n", factor);
		p_blank = blanksino[planenum];
		for (pixel = 0; pixel < (nrdet * nrproj); pixel++)
			*p_blank++ *= factor;
	}

/* Correct for difference in frame duration trans <==> contam
 *     and for difference in loss correction,
 *     and for difference decay
 *       (time = start-of-scan + start-of-frame + frame-dur/2
 *=========================================================*/
	if (contamsino[0] != NULL) {
		for (planenum = 0; planenum < nrsmoothplanes; planenum++) {
			factor = transadmin[planenum]->frame_duration
				/ contamadmin[planenum]->frame_duration;
			if (verbose)
				printf ("Contam/trans: Frame duration ratio: %g\n", factor);
			factor *= contamadmin[planenum]->loss_correction
				/ transadmin[planenum]->loss_correction;
			factor *= contamweight;
			if (verbose)
				printf ("Framedur ratio x losscorr ratio x contamweight (%g)= %g\n",
					contamweight, factor);
			decaycorr = decaycorrfactor (contamadmin[planenum], transadmin[planenum]);
			if (verbose)
				printf ("Decay correction factor for contamination is %g\n", decaycorr);
			factor *= decaycorr;

			p_contam = contamsino[planenum];
			for (pixel = 0; pixel < (nrdet * nrproj); pixel++)
				*p_contam++ *= factor;
		}
	}
/* Correct blank for differences in normalization
 *=====================*/
	if ((blanknorm[0] != NULL) && (transnorm[0] != NULL)) {
		if (verbose)
			printf ("Correcting blank for differences in normalization\n");
		for (planenum = 0; planenum < nrsmoothplanes; planenum++) {
			p_blank = blanksino[planenum];
			p_blanknorm = blanknorm[planenum];
			p_transnorm = transnorm[planenum];
			for (pixel = 0; pixel < (nrdet * nrproj); pixel++)
				*p_blank++ *= *p_blanknorm++ / *p_transnorm++;

			free (blanknorm[planenum]);
		}
	}
/* Correct contam for differences in normalization
 *=====================*/
	if ((contamnorm[0] != NULL) && (transnorm[0] != NULL)) {
		if (verbose)
			printf ("Correcting contamination for differences in normalization\n");
		for (planenum = 0; planenum < nrsmoothplanes; planenum++) {
			p_contam = contamsino[planenum];
			p_contamnorm = contamnorm[planenum];
			p_transnorm = transnorm[planenum];
			for (pixel = 0; pixel < (nrdet * nrproj); pixel++)
				*p_contam++ *= *p_contamnorm++ / *p_transnorm++;

			free (contamnorm[planenum]);
		}
	}
	if (transnorm[0] != NULL)
		for (planenum = 0; planenum < nrsmoothplanes; planenum++)
			free (transnorm[planenum]);

/* If axialsmooth is set, combine the planes
************************************/
	if (nrsmoothplanes > 1) {
		if (verbose)
			printf ("Summing planes [%g, 1, %g]\n", axialsmooth, axialsmooth);
		sum_planes (blanksino, nrsmoothplanes, nrdet, nrproj, axialsmooth);
		sum_planes (transsino, nrsmoothplanes, nrdet, nrproj, axialsmooth);
		if (contamsino[0] != NULL)
			sum_planes (contamsino, nrsmoothplanes, nrdet, nrproj, axialsmooth);
	}
/* Rebin the sinograms if requested.
************************************/
	if (sumdet > 1) {
		if (verbose) {
			printf ("Pixel size is %g cm\n", transadmin[0]->pixelsizecm);
			printf ("Rebinning blank from (%d x %d) to (%d x %d)\n",
				nrdet, nrproj, nrdet / sumdet, nrproj);
		}
		sum_detectors (&(blanksino[0]), blankadmin[0], sumdet);

		if (verbose)
			printf ("Rebinning transmission from (%d x %d) to (%d x %d)\n",
				nrdet, nrproj, nrdet / sumdet, nrproj);
		sum_detectors (&(transsino[0]), transadmin[0], sumdet);

		if (contamsino[0] != NULL) {
			if (verbose)
				printf ("Rebinning contamination from (%d x %d) to (%d x %d)\n",
					nrdet, nrproj, nrdet / sumdet, nrproj);
			sum_detectors (&(contamsino[0]), contamadmin[0], sumdet);
		}
		nrdet = transadmin[0]->nrdet;
	}
	if (verbose)
		printf ("Pixel size is %g cm\n", transadmin[0]->pixelsizecm);

	arc_radius /= transadmin[0]->pixelsizecm;

	mutissue = mutissue_cm * transadmin[0]->pixelsizecm;

/****************************/
/* Absolute intensity prior */
/****************************/
	if (priormodes > 0) {

		/*
		 * Allocate absprior *********
		 */
		absprior = (p_AbsPrior) calloc (1, sizeof (s_AbsPrior));
		absprior->number = priormodes;
		absprior->weight = priorweight;
		absprior->mean = (float *) calloc (priormodes, sizeof (float));
		absprior->std = (float *) calloc (priormodes, sizeof (float));
		absprior->width = (float *) calloc (priormodes, sizeof (float));
		absprior->allocated = 0;
		absprior->gprior = NULL;
		absprior->g2prior = NULL;

		/*
		 * Fill in the mean, std, width in ascending order **********
		 */
		for (i = 0; i < priormodes; i++) {
			curprior = 0;
			for (num = 0; num < priormodes; num++) {
				if (priormean[num] < priormean[curprior])
					curprior = num;
			}
			absprior->mean[i] = priormean[curprior] * transadmin[0]->pixelsizecm;
			absprior->std[i] = priorstd[curprior] * transadmin[0]->pixelsizecm;
			absprior->width[i] = priorwidth[curprior];
			priormean[curprior] = 1e15;
		}
		if (verbose) {
			printf ("Absolute intensity prior (per pixel): mean, std, width:\n");
			for (i = 0; i < absprior->number; i++)
				printf ("%d) %g,  %g,  %g\n", i, absprior->mean[i], absprior->std[i],
					absprior->width[i]);
		}
	} else if (verbose)
		printf ("No absolute intensity prior\n");

/*****************************/
/* Markov Random Field prior */
/*****************************/
	if (markovfunction != 'n') {

		/*
		 * Allocate markovprior *********
		 */
		markovprior = (p_MarkovPrior) calloc (1, sizeof (s_MarkovPrior));
		markovprior->functiontype = markovfunction;
		markovprior->neighborhood = markovneighborhood;
		markovprior->delta = markovdelta * transadmin[0]->pixelsizecm;
		markovprior->weight = markovweight;
		markovprior->allocated = 0;
		markovprior->gprior = NULL;
		markovprior->g2prior = 0.0;
		if (verbose) {
			if (markovprior->functiontype == 'q')
				printf ("Markov: Quadratic, ");
			else if (markovprior->functiontype == 'g')
				printf ("Markov: Geman, ");
			else if (markovprior->functiontype == 'h')
				printf ("Markov: Huber, ");
			else {
				printf ("Illegal Markov function: %c, use q, g or h\n");
				exit (0);
			}
			printf ("weight = %g, delta = %g, neighborhood = %d.\n",
			      markovprior->weight, markovprior->delta, markovprior->neighborhood);
		}
	} else if (verbose)
		printf ("No Markov prior\n");


/* Compute the center of rotation offset: half a sinogram pixel at
 * full resolution, and of course smaller if detectors have been
 * summed: 1 / (2 * sumdet)
 ******************************************************************/
	corvalue = 1. / (2. * sumdet);
	if (verbose)
		printf ("Center of rotation offset: %g pixels\n", corvalue);
	cor_offset = (float *) calloc (nrproj, sizeof (float));
	p_cor = cor_offset;
	for (pixel = 0; pixel < nrproj; pixel++)
		*p_cor++ = corvalue;

/* Compute the interpolation coefficients for (back)projection
**********************/
	if (verbose)
		printf ("Computing the interpolation coefficients (cpu = %g)...\n", cpu_s ());
	radius = 1.05 * (float) nrdet / 2.0;
	startangle = 0.0;
	deltaangle = M_PI / nrproj;
	projcoeff = NCprojcoeff3 (nrdet, nrproj, startangle, deltaangle, radius,
				  cor_offset, arc_radius);


/* Allocate the reconstruction image
************************************/
	image_fl = (float *) calloc (nrdet * nrdet, sizeof (float));
	if (!image_fl) {
		printf ("Problems allocating memory for reconstruction image\n");
		exit (0);
	}
/* Set just circular FOV to 1.0
*****************************/
	p_image_fl = image_fl;
	center = (nrdet - 1.0) / 2.0;
	radkwad = center * center;
	for (col = 0; col < nrdet; col++)
		for (row = 0; row < nrdet; row++) {
			if (((col - center) * (col - center) + (row - center) * (row - center))
			    < radkwad)
				*p_image_fl++ = mutissue / 3;
			else
				*p_image_fl++ = 0.0;
		}

/*######################
  # ML-TR with SUBSETS #
  ######################*/
	if (verbose)
		printf ("Starting Maximum Likelihood Gradient Ascent\n");


/* Apply ML-OSTR as often as requested.
**************************************/
	if (verbose)
		printf ("Starting reconstruction, cpu = %g\n", cpu_s ());
	curiter = 0;
	totiter = 0;
	for (iter = 0; iter < n_iter; iter++)
		totiter += iteraties[iter];
	for (iter = 0; iter < n_iter; iter++) {
		for (mainiter = 0; mainiter < iteraties[iter]; mainiter++) {

			/*
			 * If multiple iterations are applied, it is a good idea to start with
			 * lower prior values (absprior and Geman have local minima!). ******
			 */
			if (totiter > 2) {
				if (curiter == 0) {
					if (absprior != NULL)
						absprior->weight = priorweight * 0.1;
					if (markovfunction == 'g')
						markovprior->weight = markovweight * 0.1;
				} else if (curiter == 1) {
					if (absprior != NULL)
						absprior->weight = priorweight * 0.5;
					if (markovfunction == 'g')
						markovprior->weight = markovweight * 0.5;
				} else {
					if (absprior != NULL)
						absprior->weight = priorweight;
					if (markovfunction == 'g')
						markovprior->weight = markovweight;
				}
				if (verbose) {
					if (absprior != NULL)
						printf ("Using abs. intensity prior weight: %g\n", absprior->weight);
					if (markovprior != NULL)
						printf ("Using Markov prior weight: %g\n", markovprior->weight);
				}
			}
			if (calclik) {
				NCmlostr (image_fl, transsino[0], blanksino[0], contamsino[0], 1,
					  sub_iteraties[iter], relaxation, &likelihood, projcoeff,
					  absprior, markovprior, verbose);
			} else {
				NCmlostr (image_fl, transsino[0], blanksino[0], contamsino[0], 1,
					  sub_iteraties[iter], relaxation, NULL, projcoeff,
					  absprior, markovprior, verbose);
			}
			curiter++;
		}
	}
	if (verbose)
		printf ("End of reconstruction, cpu = %g\n", cpu_s ());


/* Save attenuation per pixel image if requested
*****************************/
	if (recon_name[0] != '\0') {
		if (singleplane) {
			map_output_image (image_fl, nrdet, nrdet, trans_file->mhptr,
					  transmatrix, transadmin[0], version, verbose,
					  recon_name, 1, 1, 1, 0, 1, 0);
		} else {
			map_output_image (image_fl, nrdet, nrdet, trans_file->mhptr,
					  transmatrix, transadmin[0], version, verbose,
					  recon_name, frame, plane, gate, bed,
					  transadmin[0]->nplanes, 0);
		}
	}
	pixelsizecm = transadmin[0]->pixelsizecm;

/* Reprojection to produce the attenuation correction factors.
 * We can use the memory from transsino for this purpose, since
 * the transmission scan is no longer needed.
 * Compute exp(reproj) to produce atten. corr. factors.
*****************************/
	if (acf_name[0] != '\0') {
		if (verbose)
			printf ("Forward projection of attenuation image\n");
		for (projnum = 0; projnum < nrproj; projnum++) {
			NCproj_forward (image_fl, transsino[0], projnum, projcoeff);
		}

		if (verbose)
			printf ("Computing attenuation correction factors\n");
		p_trans = transsino[0];
		for (pixel = 0; pixel < nrdet * nrproj; pixel++) {
			*p_trans = exp (*p_trans);
			p_trans++;
		}

		/*
		 * Undo the rebinning if detectors have been summed. ***************************
		 */
		if (sumdet > 1) {
			if (verbose)
				printf ("Rebinning to original sinogram size\n");
			unsum_detectors (&(transsino[0]), transadmin[0], sumdet);
		}
		unbinnednrdet = transadmin[0]->nrdet;

		/*
		 * Save attenuation correction factors if requested ***************************
		 */
		if (singleplane) {
			map_output_atten (transsino[0], unbinnednrdet, nrproj, trans_file->mhptr,
					  transmatrix, transadmin[0], version, verbose,
					  acf_name, 1, 1, 1, 0, 1);
		} else {
			map_output_atten (transsino[0], unbinnednrdet, nrproj, trans_file->mhptr,
					  transmatrix, transadmin[0], version, verbose,
					  acf_name, frame, plane, gate, bed,
					  transadmin[0]->nplanes);
		}
	}
	if (img_name[0] != '\0') {

		/*
		 * If zoomed image is requested, then A) zoom the reconstruction to allow
		 * registration with emission images. ***************************
		 */
		image_zoom = (float *) calloc (size_image * size_image, sizeof (float));
		globalzoom = NCzoom_image (image_zoom, image_fl, pixelsizecm,
					   nrdet, nrdet, size_image, size_image, zoom,
					   x_offset, y_offset, rotation, verbose);

		/*
		 * B) rescale the attenuation per cm: correct for the pixel size of the original
		 * image (since zooming preserved mean count). ****************************
		 */
		p_image = image_zoom;
		for (pixel = 0; pixel < (size_image * size_image); pixel++) {
			*p_image++ /= pixelsizecm;
		}
		pixelsizecm /= globalzoom;


		/*
		 * C) Save attenuation per mm image We will reuse transadmin, so we have to adapt
		 * its pixelsize. ****************************
		 */
		transadmin[0]->pixelsizecm = pixelsizecm;
		transadmin[0]->nrdet = size_image;

		/*
		 * If this is version V7 or higher, then we must reverse the planes if FeetFirst
		 * has been specified. We have to do this, since CTI is doing it as well. ***********
		 */
		switch (revplanecode) {
		case 0:
			reverseplane = 0;
			break;
		case 1:
			reverseplane = 1;
			break;
		case 2:
			if ((version >= V7) ||
			    ((version == 0) && (trans_file->mhptr->sw_version >= V7))) {
				if (verbose)
					printf ("Feet first: reversing planes\n");
				reverseplane =
					((trans_file->mhptr->patient_orientation == FeetFirstProne) ||
				    (trans_file->mhptr->patient_orientation == FeetFirstSupine) ||
				     (trans_file->mhptr->patient_orientation == FeetFirstRight) ||
				       (trans_file->mhptr->patient_orientation == FeetFirstLeft));
			} else {
				reverseplane = 0;
			}
			break;
		}
		if (reverseplane) {
			if (verbose)
				printf ("Reversing planes\n");
			plane = reverse_planes (image_zoom, size_image, size_image,
						plane, transadmin[0]->nplanes);
		}
		if (singleplane) {
			map_output_image (image_zoom, size_image, size_image, trans_file->mhptr,
					  transmatrix, transadmin[0], version, verbose,
					  img_name, 1, 1, 1, 0, 1, globalmaxmu);
		} else {
			map_output_image (image_zoom, size_image, size_image, trans_file->mhptr,
					  transmatrix, transadmin[0], version, verbose,
					  img_name, frame, plane, gate, bed,
					  transadmin[0]->nplanes, globalmaxmu);
		}
	}
}
